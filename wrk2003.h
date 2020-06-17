/////////////////////////////////////////
// WinXP SP3 x32 Only Structs !
#if (NTDDI_VERSION >= NTDDI_WINXP && NTDDI_VERSION <= NTDDI_WINXPSP4 )

#endif
///////////////////////////////////////////////////////////////////

          
//
//  Opaque cache-aware rundown ref structure
//
typedef struct _EX_RUNDOWN_REF_CACHE_AWARE  *PEX_RUNDOWN_REF_CACHE_AWARE;
// end_wdm end_ntddk end_ntifs
typedef struct _EX_RUNDOWN_REF_CACHE_AWARE {

    //
    //  Pointer to array of cache-line aligned rundown ref structures
    //

    PEX_RUNDOWN_REF RunRefs;

    //
    //  Points to pool of per-proc rundown refs that needs to be freed
    //

    PVOID PoolToFree;

    //
    //  Size of each padded rundown ref structure
    //

    ULONG RunRefSize;

    //
    //  Indicates # of entries in the array of rundown ref structures
    //

    ULONG Number;
} EX_RUNDOWN_REF_CACHE_AWARE, *PEX_RUNDOWN_REF_CACHE_AWARE;

/*++

Copyright (c) Microsoft Corporation. All rights reserved. 

You may only use this code if you agree to the terms of the Windows Research Kernel Source Code License agreement (see License.txt).
If you do not agree to the terms, do not use the code.


Module Name:

    rundown.c

Abstract:

    This module houses routine that do safe rundown of data structures.

    The basic principle of these routines is to allow fast protection of a data structure that is torn down
    by a single thread. Threads wishing to access the data structure attempt to obtain rundown protection via
    calling ExAcquireRundownProtection. If this function returns TRUE then accesses are safe until the protected
    thread calls ExReleaseRundownProtection. The single teardown thread calls ExWaitForRundownProtectionRelease
    to mark the rundown structure as being run down and the call will return once all protected threads have
    released their protection references.

    Rundown protection is not a lock. Multiple threads may gain rundown protection at the same time.

    The rundown structure has the following format:

    Bottom bit set   : This is a pointer to a rundown wait block (aligned on at least a word boundary)
    Bottom bit clear : This is a count of the total number of accessors multiplied by 2 granted rundown protection.

Revision History:

    Add per-processor cache-aware rundown protection APIs

--*/


//
//  This macro takes a length & rounds it up to a multiple of the alignment
//  Alignment is given as a power of 2
// 
#define EXP_ALIGN_UP_PTR_ON_BOUNDARY(_length, _alignment)                      \
          (PVOID) ((((ULONG_PTR) (_length)) + ((_alignment)-1)) &              \
                              ~(ULONG_PTR)((_alignment) - 1))
            
//
//  Checks if 1st argument is aligned on given power of 2 boundary specified
//  by 2nd argument
//
#define EXP_IS_ALIGNED_ON_BOUNDARY(_pointer, _alignment)                       \
        ((((ULONG_PTR) (_pointer)) & ((_alignment) - 1)) == 0)

//
// This is a block held on the local stack of the rundown thread.
//
typedef struct _EX_RUNDOWN_WAIT_BLOCK {
    ULONG_PTR Count;
    KEVENT WakeEvent;
} EX_RUNDOWN_WAIT_BLOCK, *PEX_RUNDOWN_WAIT_BLOCK;


#if (NTDDI_VERSION <= NTDDI_WINXPSP1)
 #define ExAcquireRundownProtectionEx ExAcquireRundownProtectionEx_k8
 #define ExReleaseRundownProtectionEx ExReleaseRundownProtectionEx_k8
#endif

BOOLEAN FASTCALL
ExAcquireRundownProtectionEx_k8 (
     __inout PEX_RUNDOWN_REF RunRef,
     __in ULONG Count
     )
/*++

Routine Description:

    Reference a rundown block preventing rundown occurring if it hasn't already started
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRef - Rundown block to be referenced
    Count  - Number of references to add

Return Value:

    BOOLEAN - TRUE - rundown protection was acquired, FALSE - rundown is active or completed

--*/
{
    ULONG_PTR Value, NewValue;

    Value = RunRef->Count;
    do {
        //
        // If rundown has started return with an error
        //
        if (Value & EX_RUNDOWN_ACTIVE) {
            return FALSE;
        }

        //
        // Rundown hasn't started yet so attempt to increment the unsage count.
        //
        NewValue = Value + EX_RUNDOWN_COUNT_INC * Count;

        NewValue = (ULONG_PTR) InterlockedCompareExchangePointer (&RunRef->Ptr,
                                                                  (PVOID) NewValue,
                                                                  (PVOID) Value);
        if (NewValue == Value) {
            return TRUE;
        }
        //
        // somebody else changed the variable before we did. Either a protection call came and went or rundown was
        // initiated. We just repeat the whole loop again.
        //
        Value = NewValue;
    } while (TRUE);
}


VOID FASTCALL
ExReleaseRundownProtectionEx_k8 (
     __inout PEX_RUNDOWN_REF RunRef,
     __in ULONG Count
     )
/*

Routine Description:

    Dereference a rundown block and wake the rundown thread if we are the last to exit
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRef - Rundown block to have its reference released
    Count  - Number of reference to remove

Return Value:

    None

*/
{
    ULONG_PTR Value, NewValue;

    Value = RunRef->Count;
    do {
        //
        // If the block is already marked for rundown then decrement the wait block count and wake the
        // rundown thread if we are the last
        //
        if (Value & EX_RUNDOWN_ACTIVE) {
            PEX_RUNDOWN_WAIT_BLOCK WaitBlock;

            //
            // Rundown is active. since we are one of the threads blocking rundown we have the right to follow
            // the pointer and decrement the active count. If we are the last thread then we have the right to
            // wake up the waiter. After doing this we can't touch the data structures again.
            //
            WaitBlock = (PEX_RUNDOWN_WAIT_BLOCK) (Value & (~EX_RUNDOWN_ACTIVE));

            //
            // For cache-aware rundown protection, it's possible the count in the waitblock
            // is actually zero at this point, so this assert is not applicable. We assert weakly
            // only for uni-proc. 
            //
            ASSERT ((WaitBlock->Count >= Count) || (KeNumberProcessors > 1));

#if defined (_WIN64) 
            if (InterlockedExchangeAdd64((PLONGLONG)&WaitBlock->Count, -(LONGLONG)Count) == (LONGLONG) Count) {
#else 
            if (InterlockedExchangeAdd((PLONG)&WaitBlock->Count, -(LONG)Count) == (LONG) Count) {
#endif
                //
                // We are the last thread out. Wake up the waiter.
                //
                KeSetEvent (&WaitBlock->WakeEvent, 0, FALSE);
            }
            return;
        } else {
            //
            // Rundown isn't active. Just try and decrement the count. Some other protector thread way come and/or
            // go as we do this or rundown might be initiated. We detect this because the exchange will fail and
            // we have to retry
            //

            //
            // For cache-aware rundown protection, it's possible that the value on this processor
            // is actually 0 or some integer less than count, so this assert is not applicable.
            // 
            ASSERT ((Value >= EX_RUNDOWN_COUNT_INC * Count) || (KeNumberProcessors > 1));

            NewValue = Value - EX_RUNDOWN_COUNT_INC * Count;

            NewValue = (ULONG_PTR) InterlockedCompareExchangePointer (&RunRef->Ptr,
                                                                      (PVOID) NewValue,
                                                                      (PVOID) Value);
            if (NewValue == Value) {
                return;
            }
            Value = NewValue;
        }

    } while (TRUE);
}


PEX_RUNDOWN_REF
FORCEINLINE
EXP_GET_CURRENT_RUNDOWN_REF(
    IN PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    )
/*++

Routine Description

    Returns the per-processor cache-aligned rundown ref structure for
    the current processor

Arguments

    RunRefCacheAware - Pointer to cache-aware rundown ref structure

Return Value:

    Pointer to a per-processor rundown ref
--*/
{
    return ((PEX_RUNDOWN_REF) (((PUCHAR) RunRefCacheAware->RunRefs) +
                               (KeGetCurrentProcessorNumber() % RunRefCacheAware->Number) * RunRefCacheAware->RunRefSize));
}


PEX_RUNDOWN_REF
FORCEINLINE
EXP_GET_PROCESSOR_RUNDOWN_REF(
    IN PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    IN ULONG Index
    )
/*++

Routine Description

    Returns the per-processor cache-aligned rundown ref structure for given processor

Arguments

    RunRefCacheAware - Pointer to cache-aware rundown ref structure
    Index - Index of the processor

Return Value:

    Pointer to a per-processor rundown ref
--*/
{
    return ((PEX_RUNDOWN_REF) (((PUCHAR) RunRefCacheAware->RunRefs) +
                                (Index % RunRefCacheAware->Number) * RunRefCacheAware->RunRefSize));
}


PEX_RUNDOWN_REF_CACHE_AWARE
ExAllocateCacheAwareRundownProtection_k8(
    __in POOL_TYPE PoolType,
    __in ULONG PoolTag
    )
/*++

Routine Description:

    Allocate a cache-friendly rundown ref structure for MP scenarios

Arguments

    PoolType - Type of pool to allocate from
    PoolTag -  Tag used for the pool allocation

Return Value

    Pointer to cache-aware rundown ref structure

    NULL if out of memory
--*/
{

    PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware;
    PEX_RUNDOWN_REF RunRefPool;
    PEX_RUNDOWN_REF CurrentRunRef;
    ULONG PaddedSize;
    ULONG Index;



    RunRefCacheAware = ExAllocatePoolWithTag (PoolType,
                                              sizeof( EX_RUNDOWN_REF_CACHE_AWARE ),
                                              PoolTag
                                              );

    if (NULL != RunRefCacheAware) {

        //
        // Capture number of processors: this will be the # of rundown counts we use
        //
        RunRefCacheAware->Number = KeNumberProcessors;

        //
        // Determine size of each ref structure
        //
        if (RunRefCacheAware->Number > 1) {
            PaddedSize = KeGetRecommendedSharedDataAlignment ();
            ASSERT ((PaddedSize & (PaddedSize - 1)) == 0);
        } else {
            PaddedSize = sizeof (EX_RUNDOWN_REF);
        }

        ASSERT (sizeof (EX_RUNDOWN_REF) <= PaddedSize);

        //
        //  Remember the size
        //
        RunRefCacheAware->RunRefSize = PaddedSize;

        RunRefPool = ExAllocatePoolWithTag (PoolType,
                                            PaddedSize * RunRefCacheAware->Number,
                                            PoolTag);

        if (RunRefPool == NULL) {
            ExFreePool (RunRefCacheAware);
            return NULL;
        }

        //
        // Check if pool is aligned if this is a multi-proc
        //
        if ((RunRefCacheAware->Number > 1) && 
            !EXP_IS_ALIGNED_ON_BOUNDARY (RunRefPool, PaddedSize)) {

            //
            //  We will allocate a padded size, free the old pool
            //
            ExFreePool (RunRefPool);

            //
            //  Allocate enough padding so we can start the refs on an aligned boundary
            //
            RunRefPool = ExAllocatePoolWithTag (PoolType,
                                                PaddedSize * RunRefCacheAware->Number + PaddedSize,
                                                PoolTag);

            if (RunRefPool == NULL) {
                ExFreePool (RunRefCacheAware);
                return NULL;
            }

            CurrentRunRef = EXP_ALIGN_UP_PTR_ON_BOUNDARY (RunRefPool, PaddedSize);

        } else {

            //
            //  Already aligned pool
            //
            CurrentRunRef = RunRefPool;
        }

        //
        //  Remember the pool block to free
        //
        RunRefCacheAware->PoolToFree = RunRefPool;
        RunRefCacheAware->RunRefs = CurrentRunRef;

        for (Index = 0; Index < RunRefCacheAware->Number; Index++) {
            CurrentRunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
            ExInitializeRundownProtection (CurrentRunRef);
        }
    }

    return RunRefCacheAware;
}


SIZE_T
ExSizeOfRundownProtectionCacheAware_k8(
    VOID
    )
/*++

Routine Description:

    Returns recommended size for a cache-friendly rundown structure

Arguments

    None

Return Value

    Recommended size in bytes
--*/
{
    SIZE_T RundownSize;
    ULONG PaddedSize;
    ULONG Number;



    RundownSize = sizeof (EX_RUNDOWN_REF_CACHE_AWARE);

    //
    // Determine size of each ref structure
    //

    Number = KeNumberProcessors;

    if (Number > 1) {

       //
       // Allocate more to account for alignment (pessimistic size)
       //

       PaddedSize= KeGetRecommendedSharedDataAlignment () * (Number+1);


    } else {
        PaddedSize = sizeof (EX_RUNDOWN_REF);
    }

    RundownSize += PaddedSize;

    return RundownSize;
}


VOID
ExInitializeRundownProtectionCacheAware_k8(
    __out PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    __in SIZE_T RunRefSize
    )
/*++

Routine Description:

    Initializes an embedded cache aware rundown structure.
    This is for drivers who like to allocate this structure as part of their 
    device extension. Callers of this routine must have obtained the required size
    via a call to ExSizeOfCacheAwareRundownProtection and passed a pointer to that 
    structure. 
   
    NOTE:  
    This structure should NOT be freed via ExFreeCacheAwareRundownProtection().

Arguments

    RunRefCacheAware    - Pointer to structure to be initialized allocated in non-paged memory
    RunRefSize          - Size returned by call to ExSizeOfCacheAwareRundownProtection

Return Value

    None
--*/
{
    PEX_RUNDOWN_REF CurrentRunRef;
    ULONG PaddedSize;
    LONG Number;
    ULONG Index;
    ULONG ArraySize;



    //
    //  Reverse engineer the size of each rundown structure based on the size
    //  that is passed in
    //

    CurrentRunRef = (PEX_RUNDOWN_REF) (((ULONG_PTR) RunRefCacheAware) + sizeof (EX_RUNDOWN_REF_CACHE_AWARE));

    ArraySize = (ULONG) (RunRefSize - sizeof( EX_RUNDOWN_REF_CACHE_AWARE )); 
    
    if (ArraySize == sizeof (EX_RUNDOWN_REF)) {
        Number = 1;
        PaddedSize = ArraySize;
    } else {

        PaddedSize = KeGetRecommendedSharedDataAlignment();

        Number =  ArraySize / PaddedSize - 1;

        CurrentRunRef = EXP_ALIGN_UP_PTR_ON_BOUNDARY (CurrentRunRef , PaddedSize);
    }

    RunRefCacheAware->RunRefs = CurrentRunRef;
    RunRefCacheAware->RunRefSize = PaddedSize;
    RunRefCacheAware->Number = Number;

    //
    //  This signature will signify that this structure should not be freed
    //  via ExFreeCacheAwareRundownProtection: if there's a bugcheck with an attempt
    //  to access this address & ExFreeCacheAwareRundownProtection on the stack,
    //  the caller of ExFree* is at fault.
    //

    RunRefCacheAware->PoolToFree = UintToPtr (0x0BADCA11);

    for (Index = 0; Index < RunRefCacheAware->Number; Index++) {
        CurrentRunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
        ExInitializeRundownProtection (CurrentRunRef);
    }
}


VOID
ExFreeCacheAwareRundownProtection_k8(
    __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    )
/*++

Routine Description:

    Free a cache-friendly rundown ref structure

Arguments

    RunRef - pointer to cache-aware rundown ref structure

Return Value

    None

--*/
{


    ASSERT (RunRefCacheAware->PoolToFree != UintToPtr (0x0BADCA11));

    ExFreePool (RunRefCacheAware->PoolToFree);
    ExFreePool (RunRefCacheAware);
}



BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
/*++

Routine Description:

    Reference a rundown block preventing rundown occurring if it hasn't already started
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRefCacheAware - Rundown block to be referenced

Return Value:

    BOOLEAN - TRUE - rundown protection was acquired, FALSE - rundown is active or completed

--*/
{
   return ExAcquireRundownProtection (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware));
}


VOID
FASTCALL
ExReleaseRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
/*++

Routine Description:

    Dereference a rundown block and wake the rundown thread if we are the last to exit
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRefCacheAware - Cache aware rundown block to have its reference released

Return Value:

    None

--*/
{
    ExReleaseRundownProtection (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware));
}


BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAwareEx_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     __in ULONG Count
     )
/*++

Routine Description:

    Reference a rundown block preventing rundown occurring if it hasn't already started
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRefCacheAware - Rundown block to be referenced
    Count  - Number of references to add

Return Value:

    BOOLEAN - TRUE - rundown protection was acquired, FALSE - rundown is active or completed

--*/
{
   return ExAcquireRundownProtectionEx (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware), Count);
}


VOID
FASTCALL
ExReleaseRundownProtectionCacheAwareEx_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     __in ULONG Count
     )
/*++

Routine Description:

    Dereference a rundown block and wake the rundown thread if we are the last to exit
    This routine is NON-PAGED because it is being called on the paging path.

Arguments:

    RunRef - Cache aware rundown block to have its reference released
    Count  - Number of reference to remove

Return Value:

    None

--*/
{
    ExReleaseRundownProtectionEx (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware), Count);
}


VOID
FASTCALL
ExWaitForRundownProtectionReleaseCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
/*++

Routine Description:

    Wait till all outstanding rundown protection calls have exited

Arguments:

    RunRefCacheAware -  Pointer to a rundown structure

Return Value:

    None

--*/
{
    PEX_RUNDOWN_REF RunRef;
    EX_RUNDOWN_WAIT_BLOCK WaitBlock;
    ULONG_PTR Value, NewValue;
    ULONG_PTR TotalCount;
    ULONG Index;


    //
    // Obtain the outstanding references by totalling up all the
    // counts for all the RunRef structures.
    //
    TotalCount = 0;
    WaitBlock.Count = 0;

    for ( Index = 0; Index < RunRefCacheAware->Number; Index++) {

        RunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);

        //
        //  Extract current count &  mark rundown active atomically
        //
        Value = RunRef->Count;

        do {
            
            ASSERT ((Value&EX_RUNDOWN_ACTIVE) == 0);

            //
            //  Indicate that the on-stack count should be used for callers of release rundown protection
            //

            NewValue = ((ULONG_PTR) &WaitBlock) | EX_RUNDOWN_ACTIVE;

            NewValue = (ULONG_PTR) InterlockedCompareExchangePointer (&RunRef->Ptr,
                                                                      (PVOID) NewValue,
                                                                      (PVOID) Value);
            if (NewValue == Value) {

                //
                //  Succeeded in making rundown active
                //

                break;
            }

            Value = NewValue;

        } while (TRUE);

        //
        // Add outstanding references on this processor to the total
        // Ignore overflow: note the rundown active bit will be zero for Value
        // so we will not add up those bits
        //
        TotalCount +=  Value;
    }

    //
    // If total count was zero there are no outstanding references
    // active at this point - since no refs can creep in after we have
    // set the rundown flag on each processor
    //
    ASSERT ((LONG_PTR) TotalCount >= 0);

    //
    //  If total count was zero there are no outstanding references
    //  active at this point - since no refs can creep in after we have
    //  set the rundown flag on each processor
    //
    if (TotalCount != 0) {

        //
        //  Extract actual number of waiters - count is biased by 2
        //
        TotalCount >>= EX_RUNDOWN_COUNT_SHIFT;

        //
        //  Initialize the gate - since the dereferencer can decrement the count as soon
        //  as we add the per-processor count to the on-stack count
        //
        KeInitializeEvent (&WaitBlock.WakeEvent, SynchronizationEvent, FALSE);

        //
        //  Add the total count to the on-stack count. If the result is zero,
        //  there are no more outstanding references. Otherwise wait to be signaled
        //
#if defined(_WIN64)
        if (InterlockedExchangeAdd64 ((PLONGLONG) &WaitBlock.Count,
                                       (LONGLONG) TotalCount)  != (-(LONGLONG) TotalCount)) {
#else
        if (InterlockedExchangeAdd ((PLONG) &WaitBlock.Count,
                                    (LONG) TotalCount)  != (-(LONG)TotalCount)) {
#endif
            KeWaitForSingleObject (&WaitBlock.WakeEvent,
                                   Executive,
                                   KernelMode,
                                   FALSE,
                                   NULL);

        }
    }

    return;
}


VOID
FASTCALL
ExReInitializeRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
/*++

Routine Description:

    Reinitialize rundown protection structure after its been rundown

Arguments:

    RunRef - Rundown block to be referenced

Return Value:

    None

--*/
{
    PEX_RUNDOWN_REF RunRef;
    ULONG Index;


    for ( Index = 0; Index < RunRefCacheAware->Number; Index++) {
        RunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
        ASSERT ((RunRef->Count&EX_RUNDOWN_ACTIVE) != 0);
        InterlockedExchangePointer (&RunRef->Ptr, NULL);
    }
}


VOID
FASTCALL
ExRundownCompletedCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
/*++
Routine Description:

    Mark rundown block has having completed rundown so we can wait again safely.

Arguments:

    RunRef - Rundown block to be referenced

Return Value:

    None
--*/
{
    PEX_RUNDOWN_REF RunRef;
    ULONG Index;

    for ( Index = 0; Index < RunRefCacheAware->Number; Index++) {
        RunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
        ASSERT ((RunRef->Count&EX_RUNDOWN_ACTIVE) != 0);
        InterlockedExchangePointer (&RunRef->Ptr, (PVOID) EX_RUNDOWN_ACTIVE);
    }
}


PVOID						// exportable by ntoskrnl
PsGetThreadWin32Thread( PRKTHREAD  Thread );


PVOID
ExEnterCriticalRegionAndAcquireResourceExclusive_k8 (
    PERESOURCE Resource )
{
	KeEnterCriticalRegion();
	ExAcquireResourceExclusiveLite(Resource, TRUE);
	return PsGetThreadWin32Thread(KeGetCurrentThread());
}


VOID FASTCALL
ExReleaseResourceAndLeaveCriticalRegion_k8 (
    PERESOURCE Resource )
{
	ExReleaseResourceLite(Resource);
	KeLeaveCriticalRegion();
}


