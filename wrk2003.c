#include <ntddk.h> 
#include "wrk2003.h"
#include "common.h"

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



//
// These are how you currently pass the flag to FindResource.
//
// VIEW_TO_DATAFILE and DATAFILE_TO_VIEW are idempotent,
// so you can covert a datafile to a datafile with VIEW_TO_DATAFILE.
// Think of better names therefore..
//
#define LDR_VIEW_TO_DATAFILE(x) ((PVOID)(((ULONG_PTR)(x)) |  (ULONG_PTR)1))
#define LDR_IS_DATAFILE(x)              (((ULONG_PTR)(x)) &  (ULONG_PTR)1)
#define LDR_IS_VIEW(x)                  (!LDR_IS_DATAFILE(x))
#define LDR_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)1))

#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK (0x00000001)



NTSTATUS
NTAPI
RtlImageNtHeaderEx(
    ULONG Flags,
    PVOID Base,
    ULONG64 Size,
    OUT PIMAGE_NT_HEADERS * OutHeaders
    )

/*++

Routine Description:

    This function returns the address of the NT Header.

    This function is a bit complicated.
    It is this way because RtlImageNtHeader that it replaces was hard to understand,
      and this function retains compatibility with RtlImageNtHeader.

    RtlImageNtHeader was #ifed such as to act different in each of the three
        boot loader, kernel, usermode flavors.

    boot loader -- no exception handling
    usermode -- limit msdos header to 256meg, catch any exception accessing the msdos-header
                or the pe header
    kernel -- don't cross user/kernel boundary, don't catch the exceptions,
                no 256meg limit

Arguments:

    Flags - RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK -- don't be so picky
                about the image, for compatibility with RtlImageNtHeader
    Base - Supplies the base of the image.
    Size - The size of the view, usually larger than the size of the file on disk.
            This is available from NtMapViewOfSection but not from MapViewOfFile.
    OutHeaders -

Return Value:

    STATUS_SUCCESS -- everything ok
    STATUS_INVALID_IMAGE_FORMAT -- bad filesize or signature value
    STATUS_INVALID_PARAMETER -- bad parameters

--*/

{
    PIMAGE_NT_HEADERS NtHeaders = 0;
    ULONG e_lfanew = 0;
    BOOLEAN RangeCheck = 0;
    NTSTATUS Status = 0;
    const ULONG ValidFlags = 
        RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK;

    if (OutHeaders != NULL) {
        *OutHeaders = NULL;
    }
    if (OutHeaders == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }
    if ((Flags & ~ValidFlags) != 0) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }
    if (Base == NULL || Base == (PVOID)(LONG_PTR)-1) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    RangeCheck = ((Flags & RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK) == 0);
    if (RangeCheck) {
        if (Size < sizeof(IMAGE_DOS_HEADER)) {
            Status = STATUS_INVALID_IMAGE_FORMAT;
            goto Exit;
        }
    }

    //
    // Exception handling is not available in the boot loader, and exceptions
    // were not historically caught here in kernel mode. Drivers are considered
    // trusted, so we can't get an exception here due to a bad file, but we
    // could take an inpage error.
    //
#define EXIT goto Exit
    if (((PIMAGE_DOS_HEADER)Base)->e_magic != IMAGE_DOS_SIGNATURE) {
        Status = STATUS_INVALID_IMAGE_FORMAT;
        EXIT;
    }
    e_lfanew = ((PIMAGE_DOS_HEADER)Base)->e_lfanew;
    if (RangeCheck) {
        if (e_lfanew >= Size
#define SIZEOF_PE_SIGNATURE 4
            || e_lfanew >= (MAXULONG - SIZEOF_PE_SIGNATURE - sizeof(IMAGE_FILE_HEADER))
            || (e_lfanew + SIZEOF_PE_SIGNATURE + sizeof(IMAGE_FILE_HEADER)) >= Size
            ) {
            Status = STATUS_INVALID_IMAGE_FORMAT;
            EXIT;
        }
    }

    NtHeaders = (PIMAGE_NT_HEADERS)((PCHAR)Base + e_lfanew);

    //
    // In kernelmode, do not cross from usermode address to kernelmode address.
    //
    if (Base < MM_HIGHEST_USER_ADDRESS) {
        if ((PVOID)NtHeaders >= MM_HIGHEST_USER_ADDRESS) {
            Status = STATUS_INVALID_IMAGE_FORMAT;
            EXIT;
        }
        //
        // Note that this check is slightly overeager since IMAGE_NT_HEADERS has
        // a builtin array of data_directories that may be larger than the image
        // actually has. A better check would be to add FileHeader.SizeOfOptionalHeader,
        // after ensuring that the FileHeader does not cross the u/k boundary.
        //
        if ((PVOID)((PCHAR)NtHeaders + sizeof (IMAGE_NT_HEADERS)) >= MM_HIGHEST_USER_ADDRESS) {
            Status = STATUS_INVALID_IMAGE_FORMAT;
            EXIT;
        }
    }

    if (NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
        Status = STATUS_INVALID_IMAGE_FORMAT;
        EXIT;
    }
    Status = STATUS_SUCCESS;

Exit:
    if (NT_SUCCESS(Status)) {
        *OutHeaders = NtHeaders;
    }
    return Status;
}
#undef EXIT

PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(
    PVOID Base
    )
{
    PIMAGE_NT_HEADERS NtHeaders = NULL;
    (VOID)RtlImageNtHeaderEx(RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK, Base, 0, &NtHeaders);
    return NtHeaders;
}



PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    )

/*++

Routine Description:

    This function locates a VirtualAddress within the image header
    of a file that is mapped as a file and returns a pointer to the
    section table entry for that virtual address

Arguments:

    NtHeaders - Supplies the pointer to the image or data file.

    Base - Supplies the base of the image or data file.

    Address - Supplies the virtual address to locate.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the pointer of the section entry containing the data.

--*/

{
    ULONG i;
    PIMAGE_SECTION_HEADER NtSection;

    NtSection = IMAGE_FIRST_SECTION( NtHeaders );
    for (i=0; i<NtHeaders->FileHeader.NumberOfSections; i++) {
        if ((ULONG)Address >= NtSection->VirtualAddress &&
            (ULONG)Address < NtSection->VirtualAddress + NtSection->SizeOfRawData
           ) {
            return NtSection;
            }
        ++NtSection;
        }

    return NULL;
}


PVOID
RtlAddressInSectionTable (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    )

/*++

Routine Description:

    This function locates a VirtualAddress within the image header
    of a file that is mapped as a file and returns the seek address
    of the data the Directory describes.

Arguments:

    NtHeaders - Supplies the pointer to the image or data file.

    Base - Supplies the base of the image or data file.

    Address - Supplies the virtual address to locate.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the address of the raw data the directory describes.

--*/

{
    PIMAGE_SECTION_HEADER NtSection;

    NtSection = RtlSectionTableFromVirtualAddress( NtHeaders,
                                                   Base,
                                                   Address
                                                 );
    if (NtSection != NULL) {
        return( ((PCHAR)Base + ((ULONG_PTR)Address - NtSection->VirtualAddress) + NtSection->PointerToRawData) );
        }
    else {
        return( NULL );
        }
}



PVOID
RtlpImageDirectoryEntryToData32 (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS32 NtHeaders
    )
{
    ULONG DirectoryAddress;

    if (DirectoryEntry >= NtHeaders->OptionalHeader.NumberOfRvaAndSizes) {
        return( NULL );
    }

    if (!(DirectoryAddress = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].VirtualAddress)) {
        return( NULL );
    }

    if (Base < MM_HIGHEST_USER_ADDRESS) {
        if ((PVOID)((PCHAR)Base + DirectoryAddress) >= MM_HIGHEST_USER_ADDRESS) {
            return( NULL );
        }
    }

    *Size = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].Size;
    if (MappedAsImage || DirectoryAddress < NtHeaders->OptionalHeader.SizeOfHeaders) {
        return( (PVOID)((PCHAR)Base + DirectoryAddress) );
    }

    return( RtlAddressInSectionTable((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}


PVOID
RtlpImageDirectoryEntryToData64 (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS64 NtHeaders
    )
{
    ULONG DirectoryAddress;

    if (DirectoryEntry >= NtHeaders->OptionalHeader.NumberOfRvaAndSizes) {
        return( NULL );
    }

    if (!(DirectoryAddress = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].VirtualAddress)) {
        return( NULL );
    }

    if (Base < MM_HIGHEST_USER_ADDRESS) {
        if ((PVOID)((PCHAR)Base + DirectoryAddress) >= MM_HIGHEST_USER_ADDRESS) {
            return( NULL );
        }
    }

    *Size = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].Size;
    if (MappedAsImage || DirectoryAddress < NtHeaders->OptionalHeader.SizeOfHeaders) {
        return( (PVOID)((PCHAR)Base + DirectoryAddress) );
    }

    return( RtlAddressInSectionTable((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}




PVOID
RtlImageDirectoryEntryToData (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size
    )

/*++

Routine Description:

    This function locates a Directory Entry within the image header
    and returns either the virtual address or seek address of the
    data the Directory describes.

Arguments:

    Base - Supplies the base of the image or data file.

    MappedAsImage - FALSE if the file is mapped as a data file.
                  - TRUE if the file is mapped as an image.

    DirectoryEntry - Supplies the directory entry to locate.

    Size - Return the size of the directory.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the address of the raw data the directory describes.

--*/

{
    PIMAGE_NT_HEADERS NtHeaders;

    if (LDR_IS_DATAFILE(Base)) {
        Base = LDR_DATAFILE_TO_VIEW(Base);
        MappedAsImage = FALSE;
        }

    NtHeaders = RtlImageNtHeader(Base);

    if (!NtHeaders)
        return NULL;

    if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return (RtlpImageDirectoryEntryToData32(Base,
                                                MappedAsImage,
                                                DirectoryEntry,
                                                Size,
                                                (PIMAGE_NT_HEADERS32)NtHeaders));
    } else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        return (RtlpImageDirectoryEntryToData64(Base,
                                                MappedAsImage,
                                                DirectoryEntry,
                                                Size,
                                                (PIMAGE_NT_HEADERS64)NtHeaders));
    } else {
        return (NULL);
    }
}



PVOID
MiFindExportedRoutineByName (
    IN PVOID DllBase,
    IN PANSI_STRING AnsiImageRoutineName
    )

/*++

Routine Description:

    This function searches the argument module looking for the requested
    exported function name.

Arguments:

    DllBase - Supplies the base address of the requested module.

    AnsiImageRoutineName - Supplies the ANSI routine name being searched for.

Return Value:

    The virtual address of the requested routine or NULL if not found.

--*/

{
    USHORT OrdinalNumber;
    PULONG NameTableBase;
    PUSHORT NameOrdinalTableBase;
    PULONG Addr;
    LONG High;
    LONG Low;
    LONG Middle;
    LONG Result;
    ULONG ExportSize;
    PVOID FunctionAddress;
    PIMAGE_EXPORT_DIRECTORY ExportDirectory;

    PAGED_CODE();

    ExportDirectory = (PIMAGE_EXPORT_DIRECTORY) RtlImageDirectoryEntryToData (
                                DllBase,
                                TRUE,
                                IMAGE_DIRECTORY_ENTRY_EXPORT,
                                &ExportSize);

    if (ExportDirectory == NULL) {
        return NULL;
    }

    //
    // Initialize the pointer to the array of RVA-based ansi export strings.
    //

    NameTableBase = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNames);

    //
    // Initialize the pointer to the array of USHORT ordinal numbers.
    //

    NameOrdinalTableBase = (PUSHORT)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNameOrdinals);

    //
    // Lookup the desired name in the name table using a binary search.
    //

    Low = 0;
    Middle = 0;
    High = ExportDirectory->NumberOfNames - 1;

    while (High >= Low) {

        //
        // Compute the next probe index and compare the import name
        // with the export name entry.
        //

        Middle = (Low + High) >> 1;

        Result = strcmp (AnsiImageRoutineName->Buffer,
                         (PCHAR)DllBase + NameTableBase[Middle]);

        if (Result < 0) {
            High = Middle - 1;
        }
        else if (Result > 0) {
            Low = Middle + 1;
        }
        else {
            break;
        }
    }

    //
    // If the high index is less than the low index, then a matching
    // table entry was not found. Otherwise, get the ordinal number
    // from the ordinal table.
    //

    if (High < Low) {
        return NULL;
    }

    OrdinalNumber = NameOrdinalTableBase[Middle];

    //
    // If the OrdinalNumber is not within the Export Address Table,
    // then this image does not implement the function.  Return not found.
    //

    if ((ULONG)OrdinalNumber >= ExportDirectory->NumberOfFunctions) {
        return NULL;
    }

    //
    // Index into the array of RVA export addresses by ordinal number.
    //

    Addr = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfFunctions);

    FunctionAddress = (PVOID)((PCHAR)DllBase + Addr[OrdinalNumber]);

    //
    // Forwarders are not used by the kernel and HAL to each other.
    //

    ASSERT ((FunctionAddress <= (PVOID)ExportDirectory) ||
            (FunctionAddress >= (PVOID)((PCHAR)ExportDirectory + ExportSize)));

    return FunctionAddress;
}



