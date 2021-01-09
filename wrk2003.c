#include <ntddk.h> 
#include "common.h"
#include "wrk2003.h"

#ifdef __cplusplus
extern "C" {
#endif


const LARGE_INTEGER MmShortTime = {(ULONG)(-10 * 1000 * 10), -1}; // 10 milliseconds

void
TrampKeBugCheckEx(void)
{
    KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 1);
}


typedef BOOLEAN  (*PFN_BOOLEAN)(void);
typedef BOOLEAN  (*PFN_BOOLEAN_PKTHREAD_KPROCESSOR_MODE)(PKTHREAD, KPROCESSOR_MODE);
typedef BOOLEAN  (*PFN_BOOLEAN_KPROCESSOR_MODE)(KPROCESSOR_MODE);
typedef NTSTATUS (*PFN_NTSTATUS_PVOID_PPORT_MESSAGE_PPORT_MESSAGE)(PVOID, PPORT_MESSAGE, PPORT_MESSAGE);


KIRQL           gGuardedRegion_OldIrql;
LONG            gGuardedRegionCounter;



PFN_BOOLEAN                                         gTramp_KeInvalidateAllCaches =
(PFN_BOOLEAN)                                       TrampKeBugCheckEx;

PFN_BOOLEAN_PKTHREAD_KPROCESSOR_MODE                gTramp_KeAlertThread         =
(PFN_BOOLEAN_PKTHREAD_KPROCESSOR_MODE)              TrampKeBugCheckEx;

PFN_BOOLEAN_KPROCESSOR_MODE                         gTramp_KeTestAlertThread     =
(PFN_BOOLEAN_KPROCESSOR_MODE)                       TrampKeBugCheckEx;

PFN_NTSTATUS_PVOID_PPORT_MESSAGE_PPORT_MESSAGE      gTramp_LpcRequestWaitReplyPortEx =
(PFN_NTSTATUS_PVOID_PPORT_MESSAGE_PPORT_MESSAGE)    TrampKeBugCheckEx;



BOOLEAN FASTCALL
ExAcquireRundownProtectionEx_k8 (
     __inout PEX_RUNDOWN_REF RunRef,
     __in ULONG Count
     )
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



PEX_RUNDOWN_REF_CACHE_AWARE
ExAllocateCacheAwareRundownProtection_k8 (
    __in POOL_TYPE PoolType,
    __in ULONG PoolTag
    )
{
    PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware;
    PEX_RUNDOWN_REF RunRefPool;
    PEX_RUNDOWN_REF CurrentRunRef;
    ULONG PaddedSize;
    ULONG Index;

    RunRefCacheAware = (PEX_RUNDOWN_REF_CACHE_AWARE) ExAllocatePoolWithTag (PoolType,
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
        } else {
            PaddedSize = sizeof (EX_RUNDOWN_REF);
        }


        //
        //  Remember the size
        //
        RunRefCacheAware->RunRefSize = PaddedSize;

        RunRefPool = (PEX_RUNDOWN_REF) ExAllocatePoolWithTag (PoolType,
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
            RunRefPool = (PEX_RUNDOWN_REF) ExAllocatePoolWithTag (PoolType,
                                                PaddedSize * RunRefCacheAware->Number + PaddedSize,
                                                PoolTag);

            if (RunRefPool == NULL) {
                ExFreePool (RunRefCacheAware);
                return NULL;
            }

            CurrentRunRef = (PEX_RUNDOWN_REF) EXP_ALIGN_UP_PTR_ON_BOUNDARY (RunRefPool, PaddedSize);

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
ExSizeOfRundownProtectionCacheAware_k8 (
    VOID
    )
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
ExInitializeRundownProtectionCacheAware_k8 (
    __out PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    __in SIZE_T RunRefSize
    )
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

        CurrentRunRef = (PEX_RUNDOWN_REF) EXP_ALIGN_UP_PTR_ON_BOUNDARY (CurrentRunRef , PaddedSize);
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
ExFreeCacheAwareRundownProtection_k8 (
    __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    )
{
    ExFreePool (RunRefCacheAware->PoolToFree);
    ExFreePool (RunRefCacheAware);
}



BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAware_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
{
   return ExAcquireRundownProtection (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware));
}


VOID
FASTCALL
ExReleaseRundownProtectionCacheAware_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
{
    ExReleaseRundownProtection (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware));
}


BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAwareEx_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     ULONG Count
     )
{
   return ExAcquireRundownProtectionEx (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware), Count);
}


VOID
FASTCALL
ExReleaseRundownProtectionCacheAwareEx_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     ULONG Count
     )
{
    ExReleaseRundownProtectionEx (EXP_GET_CURRENT_RUNDOWN_REF (RunRefCacheAware), Count);
}


VOID
FASTCALL
ExWaitForRundownProtectionReleaseCacheAware_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
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
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
{
    PEX_RUNDOWN_REF RunRef;
    ULONG Index;


    for ( Index = 0; Index < RunRefCacheAware->Number; Index++) {
        RunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
        InterlockedExchangePointer (&RunRef->Ptr, NULL);
    }
}


VOID
FASTCALL
ExRundownCompletedCacheAware_k8 (
     PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     )
{
    PEX_RUNDOWN_REF RunRef;
    ULONG Index;

    for ( Index = 0; Index < RunRefCacheAware->Number; Index++) {
        RunRef = EXP_GET_PROCESSOR_RUNDOWN_REF (RunRefCacheAware, Index);
        InterlockedExchangePointer (&RunRef->Ptr, (PVOID) EX_RUNDOWN_ACTIVE);
    }
}


PVOID
ExEnterCriticalRegionAndAcquireResourceExclusive_k8 (
    PERESOURCE Resource )
{
    KeEnterCriticalRegion();
    ExAcquireResourceExclusiveLite(Resource, TRUE);
    return _PsGetCurrentThread()->Tcb.Win32Thread;
}


PVOID
ExEnterCriticalRegionAndAcquireResourceShared_k8 (
    PERESOURCE Resource )
{
    KeEnterCriticalRegion();
    ExAcquireResourceSharedLite(Resource, TRUE);
    return _PsGetCurrentThread()->Tcb.Win32Thread;
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
RtlImageNtHeaderEx_k8 (
    ULONG Flags,
    PVOID Base,
    ULONG64 Size,
    PIMAGE_NT_HEADERS * OutHeaders
    )
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
RtlImageNtHeader_k8 (
    PVOID Base
    )
{
    PIMAGE_NT_HEADERS NtHeaders = NULL;
    (VOID)RtlImageNtHeaderEx_k8(RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK, Base, 0, &NtHeaders);
    return NtHeaders;
}



PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress_k8 (
   PIMAGE_NT_HEADERS NtHeaders,
   PVOID Base,
   ULONG Address
    )
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
RtlAddressInSectionTable_k8 (
   PIMAGE_NT_HEADERS NtHeaders,
   PVOID Base,
   ULONG Address
    )
{
    PIMAGE_SECTION_HEADER NtSection;

    NtSection = RtlSectionTableFromVirtualAddress_k8( NtHeaders,
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
RtlpImageDirectoryEntryToData32_k8 (
   PVOID Base,
   BOOLEAN MappedAsImage,
   USHORT DirectoryEntry,
    PULONG Size,
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

    return( RtlAddressInSectionTable_k8((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}


PVOID
RtlpImageDirectoryEntryToData64_k8 (
   PVOID Base,
   BOOLEAN MappedAsImage,
   USHORT DirectoryEntry,
    PULONG Size,
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

    return( RtlAddressInSectionTable_k8((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}


PVOID
RtlImageDirectoryEntryToData_k8 (
   PVOID Base,
   BOOLEAN MappedAsImage,
   USHORT DirectoryEntry,
    PULONG Size
    )
{
    PIMAGE_NT_HEADERS NtHeaders;

    if (LDR_IS_DATAFILE(Base)) {
        Base = LDR_DATAFILE_TO_VIEW(Base);
        MappedAsImage = FALSE;
        }

    NtHeaders = RtlImageNtHeader_k8(Base);

    if (!NtHeaders)
        return NULL;

    if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return (RtlpImageDirectoryEntryToData32_k8(Base,
                                                MappedAsImage,
                                                DirectoryEntry,
                                                Size,
                                                (PIMAGE_NT_HEADERS32)NtHeaders));
    } else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        return (RtlpImageDirectoryEntryToData64_k8(Base,
                                                MappedAsImage,
                                                DirectoryEntry,
                                                Size,
                                                (PIMAGE_NT_HEADERS64)NtHeaders));
    } else {
        return (NULL);
    }
}



PVOID
MiFindExportedRoutineByName_k8 (
   PVOID DllBase,
   PANSI_STRING AnsiImageRoutineName
    )
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

    ExportDirectory = (PIMAGE_EXPORT_DIRECTORY) RtlImageDirectoryEntryToData_k8 (
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
    // Lookup the desired namethe name table using a binary search.
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

    return FunctionAddress;
}


HANDLE
PsGetCurrentThreadProcessId_k8 ( VOID )
{
    return _PsGetCurrentThread()->Cid.UniqueProcess;
}


BOOLEAN
KeAreAllApcsDisabled_k8 (VOID)      // XP x32 Only
{   
    return (BOOLEAN)((KeGetCurrentThread()->KernelApcDisable != 0) ||
                     (KeGetCurrentIrql() >= APC_LEVEL));
}


VOID
__wbinvd (VOID);
#pragma intrinsic(__wbinvd)


BOOLEAN
KeInvalidateAllCaches_k8 (VOID)
{
    return gTramp_KeInvalidateAllCaches();
}


VOID FASTCALL
ExEnterCriticalRegionAndAcquireFastMutexUnsafe_k8 (
    PFAST_MUTEX FastMutex
    )
{
    KeEnterCriticalRegion();
    ExAcquireFastMutexUnsafe(FastMutex);
}


VOID FASTCALL
ExReleaseFastMutexUnsafeAndLeaveCriticalRegion_k8 (
    PFAST_MUTEX FastMutex
    )
{

    ExReleaseFastMutexUnsafe(FastMutex);
    KeLeaveCriticalRegion();
    return;
}


PVOID
PsGetCurrentThreadWin32Thread_k8 (
    VOID
     )
{
    return PsGetThreadWin32Thread(KeGetCurrentThread());
}


PVOID
PsGetCurrentProcessWin32Process_k8 (
    VOID
    )
{
    return PsGetCurrentProcess()->Win32Process;
}

PVOID
PsGetCurrentThreadTeb_k8 (
    VOID
    )
{
    return PsGetCurrentThread()->Tcb.Teb;
}


BOOLEAN
PsIsSystemProcess_k8 (
    PEPROCESS Process
     )
{
    return (BOOLEAN)(Process == PsInitialSystemProcess);
}


ULONG
MmGetSessionIdEx_k8 (
    IN PEPROCESS Process
    )
{
    PMM_SESSION_SPACE SessionGlobal;

    if (Process->Vm.Flags.SessionLeader == 1) {

        //
        // smss may transiently have a session space but that's of no interest
        // to our caller.
        //

        return (ULONG)-1;
    }

    //
    // The Session field of the EPROCESS is never cleared once set so these
    // checks can be done lock free.
    //

    SessionGlobal = (PMM_SESSION_SPACE) Process->Session;

    if (SessionGlobal == NULL) {

        //
        // The system process has no session space.
        //

        return (ULONG)-1;
    }

    return SessionGlobal->SessionId;
}



ULONG
PsGetProcessSessionIdEx_k8 (
    PEPROCESS Process
    )
{
    return MmGetSessionIdEx_k8 (Process);
}


PEPROCESS
PsGetCurrentThreadProcess_k8 (
    VOID
    )
{
    return THREAD_TO_PROCESS(PsGetCurrentThread());
}


PVOID
PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion_k8 (
    PHANDLE ProcessId
     )
{
    PETHREAD Thread = PsGetCurrentThread();
    *ProcessId = Thread->Cid.UniqueProcess;
    KeEnterCriticalRegionThread(&Thread->Tcb);
    return Thread->Tcb.Win32Thread;
}


BOOLEAN
KeAlertThread_k8 (
    PKTHREAD Thread,
    KPROCESSOR_MODE AlertMode )
{
    return gTramp_KeAlertThread(Thread, AlertMode);
}


BOOLEAN
KeTestAlertThread_k8 (
    KPROCESSOR_MODE AlertMode
    )
{
    return gTramp_KeTestAlertThread(AlertMode);
}


VOID
ObDeleteCapturedInsertInfo_k8 (
    PVOID Object
    )
{
    POBJECT_HEADER ObjectHeader;

    //
    //  Get the address of the object header and free the object create
    //  information object if the object is being created.
    //

    ObjectHeader = OBJECT_TO_OBJECT_HEADER(Object);

    if (ObjectHeader->Flags & OB_FLAG_NEW_OBJECT) {

        if (ObjectHeader->ObjectCreateInfo != NULL) {

            ObpFreeObjectCreateInformation(ObjectHeader->ObjectCreateInfo);

            ObjectHeader->ObjectCreateInfo = NULL;
        }
    }

    return;
}


VOID FASTCALL
ExfReleasePushLockShared_k8 (
    PEX_PUSH_LOCK PushLock
     )
{
    ExfReleasePushLock(PushLock);
}


VOID FASTCALL
ExfTryToWakePushLock_k8 (
    PEX_PUSH_LOCK PushLock
    )
{
    ExfReleasePushLock(PushLock);
}


NTSTATUS
LpcRequestWaitReplyPortEx_k8 (
    PVOID PortAddress,
    PPORT_MESSAGE RequestMessage,
    PPORT_MESSAGE ReplyMessage
    )
{
    return gTramp_LpcRequestWaitReplyPortEx(PortAddress, RequestMessage, ReplyMessage);
}


NTSTATUS
KeExpandKernelStackAndCallout_k8 (
    PEXPAND_STACK_CALLOUT Callout,
    PVOID Parameter,
    SIZE_T Size
    )
{
    ULONG_PTR   ActualLimit;
    ULONG_PTR   CurrentLimit;
    ULONG_PTR   CurrentStack;
    KIRQL       ExitIrql;
    volatile BOOLEAN InCallout;
    PVOID       LargeStack;
    KIRQL       OldIrql;
    NTSTATUS    Status;
    PKTHREAD    Thread;
    
    if (Size > MAXIMUM_EXPANSION_SIZE) {
        return STATUS_INVALID_PARAMETER_3;
    }
    
    Thread = KeGetCurrentThread();
    CurrentStack = KeGetCurrentStackPointer();
    CurrentLimit = (ULONG_PTR)Thread->StackLimit;
    //ActualLimit = CurrentLimit;
    
    if ((CurrentStack - CurrentLimit) < Size) {
        Status = MmGrowKernelStack((PVOID)CurrentStack);
        if (NT_SUCCESS(Status) == FALSE) {
            return STATUS_NO_MEMORY;
        }
    }

    Status = STATUS_SUCCESS;
    InCallout = TRUE;
    __try {
        __try {
            (Callout)(Parameter);
            InCallout = FALSE;

        } __except (EXCEPTION_EXECUTE_HANDLER) {
            KeBugCheck(KMODE_EXCEPTION_NOT_HANDLED);
        }

    } __finally {
        if (InCallout == TRUE) {
            KeBugCheck(KMODE_EXCEPTION_NOT_HANDLED);
        }
    }
    
    return STATUS_SUCCESS;
}


// TODO trace OldIrql depend on caller
void
KeEnterGuardedRegion_k8 (void)
{
    KeEnterCriticalRegion();
    
    if (gGuardedRegionCounter < 1 &&
        KeGetCurrentIrql() < gGuardedRegion_OldIrql &&
        KeGetCurrentIrql() < APC_LEVEL)
            KeRaiseIrql(APC_LEVEL, &gGuardedRegion_OldIrql);
    
    InterlockedIncrement(&gGuardedRegionCounter);
}


void
KeLeaveGuardedRegion_k8 (void)
{
    InterlockedDecrement(&gGuardedRegionCounter);
    
    if (gGuardedRegionCounter < 1)
        KeLowerIrql(gGuardedRegion_OldIrql);

    if (gGuardedRegionCounter < 0)
        InterlockedExchange(&gGuardedRegionCounter, 0);    
    
    KeLeaveCriticalRegion();
}


void FASTCALL
KeInitializeGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{    
    /*
    {
        volatile LONG Count;
        PKTHREAD Owner;
        ULONG Contention;
        KGATE Gate;
        union {
           struct {
            SHORT KernelApcDisable;
            SHORT SpecialApcDisable;
           };
           ULONG CombinedApcDisable;
        };
    } KGUARDED_MUTEX
    
    {
        volatile LONG Count;
        PKTHREAD Owner;
        ULONG Contention;
        KEVENT Event;
        ULONG OldIrql;
    } FAST_MUTEX, */

    ExInitializeFastMutex((PFAST_MUTEX)Mutex);

}


void FASTCALL
KeAcquireGuardedMutex_k8 (
  PKGUARDED_MUTEX  Mutex )
{
    KeEnterGuardedRegion_k8();
    ExAcquireFastMutex((PFAST_MUTEX)Mutex);
}


BOOLEAN FASTCALL
KeTryToAcquireGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    BOOLEAN    status;
    
    KeEnterGuardedRegion_k8();
    status = ExTryToAcquireFastMutex((PFAST_MUTEX)Mutex);
    
    if (status == FALSE) {
        KeLeaveGuardedRegion_k8();
        KeYieldProcessor();
    }

    return status;
}


void FASTCALL
KeReleaseGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    ExReleaseFastMutex((PFAST_MUTEX)Mutex);
    KeLeaveGuardedRegion_k8();
}


void FASTCALL
KeAcquireGuardedMutexUnsafe_k8 (
    PKGUARDED_MUTEX FastMutex )
{
    ExAcquireFastMutexUnsafe((PFAST_MUTEX)FastMutex);
}


void FASTCALL
KeReleaseGuardedMutexUnsafe_k8 (
    PKGUARDED_MUTEX FastMutex )
{
    ExReleaseFastMutexUnsafe((PFAST_MUTEX)FastMutex);
}



PVOID
GetRoutineAddress_k8 (
    PUNICODE_STRING SystemRoutineName,
    const PCHAR Modulename)
{
    NTSTATUS                Status;
    ANSI_STRING             AnsiString;
    PVOID                   FunctionAddress;
    LOGICAL                 Found;
    ULONG                   ReturnLength;
    PRTL_PROCESS_MODULES    ModuleInformation;
    PRTL_PROCESS_MODULE_INFORMATION ModuleInfo;
    CHAR                   *FileName;
    ULONG_PTR               OldUserProbeAddress;
    ULONG_PTR              *pUserProbeAddress;

    do {
        Status = RtlUnicodeStringToAnsiString(&AnsiString,
                                              SystemRoutineName,
                                              TRUE);
        if (NT_SUCCESS(Status))
        {
            break;
        }

        KeDelayExecutionThread(KernelMode, FALSE, (PLARGE_INTEGER)&MmShortTime);

    } while (TRUE);

    if (KeGetCurrentIrql() >= DISPATCH_LEVEL)
        ModuleInformation = (PRTL_PROCESS_MODULES) ExAllocatePoolWithTag(NonPagedPool, NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');
    else    
        ModuleInformation = (PRTL_PROCESS_MODULES) ExAllocatePoolWithTag(PagedPool,    NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');

    RtlZeroMemory(ModuleInformation, NtQuery_BUFFERSIZE * sizeof(UCHAR));

    pUserProbeAddress = &MmUserProbeAddress;
    OldUserProbeAddress = *pUserProbeAddress;
    *pUserProbeAddress  = (ULONG_PTR)-1; // hack to allow NtQuerySystemInformation write
                                         // to Buffer allocated in kernel space > 0x7FFF0000
    Status = NtQuerySystemInformation(  SystemModuleInformation,
                                        ModuleInformation,
                                        NtQuery_BUFFERSIZE * sizeof(UCHAR),
                                        &ReturnLength );
    *pUserProbeAddress  = OldUserProbeAddress;

    if (Status != STATUS_SUCCESS) {
        RtlFreeAnsiString (&AnsiString);
        ExFreePoolWithTag(ModuleInformation, 'pmuD');
        return NULL;
    }

    Found = FALSE;
    for (ULONG Index = 0; Index < ModuleInformation->NumberOfModules; Index++)
    {
        ModuleInfo = &ModuleInformation->Modules[Index];
        FileName = (CHAR *)ModuleInfo->FullPathName + ModuleInfo->OffsetToFileName;

            if (strcmp(FileName, Modulename) == 0)
            {
                Found = TRUE;
                break;
            }
    }
    
    if (Found == TRUE)
        FunctionAddress = MiFindExportedRoutineByName_k8( ModuleInfo->ImageBase,
                                                       &AnsiString );
    else
        FunctionAddress = NULL;
        
    RtlFreeAnsiString (&AnsiString);
    ExFreePoolWithTag(ModuleInformation, 'pmuD');
    return FunctionAddress;
}


PVOID
GetModuleBaseAddress_k8 (
    const PCHAR Modulename, 
    int*  ModuleSize,
    int   ByOrderMode,
    int   OrderNum)
{
    NTSTATUS                Status;
    PVOID                   FunctionAddress;
    LOGICAL                 Found;
    ULONG                   ReturnLength;
    PRTL_PROCESS_MODULES    ModuleInformation;
    PRTL_PROCESS_MODULE_INFORMATION ModuleInfo;
    CHAR                   *FileName;
    ULONG_PTR               OldUserProbeAddress;
    ULONG_PTR              *pUserProbeAddress;

    if (KeGetCurrentIrql() >= DISPATCH_LEVEL)
        ModuleInformation = (PRTL_PROCESS_MODULES) ExAllocatePoolWithTag(NonPagedPool, NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');
    else    
        ModuleInformation = (PRTL_PROCESS_MODULES) ExAllocatePoolWithTag(PagedPool,    NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');

    RtlZeroMemory(ModuleInformation, NtQuery_BUFFERSIZE * sizeof(UCHAR));

    pUserProbeAddress = &MmUserProbeAddress;
    OldUserProbeAddress = *pUserProbeAddress;
    *pUserProbeAddress  = (ULONG_PTR)-1; // hack to allow NtQuerySystemInformation write
                                         // to Buffer allocated in kernel space > 0x7FFF0000
    Status = NtQuerySystemInformation(  SystemModuleInformation,
                                        ModuleInformation,
                                        NtQuery_BUFFERSIZE * sizeof(UCHAR),
                                        &ReturnLength );
    *pUserProbeAddress  = OldUserProbeAddress;

    if (Status != STATUS_SUCCESS) {
        ExFreePoolWithTag(ModuleInformation, 'pmuD');
        return NULL;
    }

    Found = FALSE;
    for (ULONG Index = 0; Index < ModuleInformation->NumberOfModules; Index++)
    {
        ModuleInfo = &ModuleInformation->Modules[Index];
        FileName = (CHAR *)ModuleInfo->FullPathName + ModuleInfo->OffsetToFileName;

        if (ByOrderMode && Index == OrderNum) {
            Found = TRUE;
            break;
        }

        if (strcmp(FileName, Modulename) == 0) {
            Found = TRUE;
            break;
        }
    }
    
    if (Found == TRUE) {
        FunctionAddress = ModuleInfo->ImageBase;
        if (ModuleSize) {
            *ModuleSize = ModuleInfo->ImageSize;
        }
    } else {
        FunctionAddress = NULL;
        if (ModuleSize) {
            *ModuleSize = 0;
        }
    }

    ExFreePoolWithTag(ModuleInformation, 'pmuD');
    return FunctionAddress;
}


void FASTCALL
KiAcquireQueuedLock_k8 (
    IN PKSPIN_LOCK_QUEUE QueuedLock
    )
{
    PKSPIN_LOCK_QUEUE Previous;
    PKSPIN_LOCK Lock;
    volatile ULONG_PTR * LockPointer;

    LockPointer = (volatile ULONG_PTR *)&QueuedLock->Lock;

    Previous = (PKSPIN_LOCK_QUEUE) InterlockedExchangePointer((volatile PVOID *)QueuedLock->Lock, QueuedLock);

    if (Previous == NULL) {

        //
        // This processor now owns this lock.
        //

#if defined(QLOCK_STAT_CLEAN)

        ULONG LockNumber;

        LockNumber = QueuedLock - KeGetCurrentPrcb()->LockQueue;

        //
        // The following check allows the conversion from QueuedLock to
        // lock number to work (validly) even if in stack queued spin
        // locks are using this routine.
        //

        if (LockNumber < QLOCKS_NUMBER) {
            KiQueuedSpinLockHouse[LockNumber].Clean = 1;
        }
        
#endif

        *LockPointer |= LOCK_QUEUE_OWNER;

    } else {

        //
        // Lock is already held, update thew next pointer in the
        // previous queue entry to point to this new waiter and 
        // wait until the lock is granted.
        //

        *LockPointer |= LOCK_QUEUE_WAIT;
        Previous->Next = QueuedLock;

        while (*LockPointer & LOCK_QUEUE_WAIT) {
            KeYieldProcessor();
        }
    }

    //
    // Lock has been acquired.
    //
}


void FASTCALL
KiReleaseQueuedLock_k8 (
    IN PKSPIN_LOCK_QUEUE QueuedLock
    )
{
    PKSPIN_LOCK_QUEUE Waiter;

    //
    // Get the address of the actual lock and strip out the bottom
    // two bits which are used for status.
    //

    QueuedLock->Lock = (PKSPIN_LOCK)((ULONG_PTR)QueuedLock->Lock & ~3);

    Waiter = (PKSPIN_LOCK_QUEUE)*QueuedLock->Lock;

    if (Waiter == QueuedLock) {

        //
        // Good chance noone is queued on this lock, to be sure
        // we need to do an interlocked operation on it.
        // Note: This is just an optimization, there is no point
        // in doing the interlocked compare exchange if someone
        // else has already joined the queue.
        //

        Waiter = (PKSPIN_LOCK_QUEUE) InterlockedCompareExchangePointer((volatile PVOID *) QueuedLock->Lock,
                                                   NULL,
                                                   QueuedLock);
    }
    if (Waiter != QueuedLock) {

        //
        // There is another waiter.  It is possible for the waiter
        // to have only just performed the exchange that put its 
        // context in the lock and to have not yet updated the
        // 'next' pointer in the previous context (which could be 
        // this context), so we wait for our next pointer to be
        // non-null before continuing.
        //

        volatile PKSPIN_LOCK_QUEUE * NextQueuedLock = &QueuedLock->Next;

        while ((Waiter = *NextQueuedLock) == NULL) {
            KeYieldProcessor();
        }

        //
        // Pass the lock on to the next in line.
        //

        *((PULONG_PTR)&Waiter->Lock) ^= (LOCK_QUEUE_WAIT | LOCK_QUEUE_OWNER);
        QueuedLock->Next = NULL;
    }
}



void FASTCALL
KeAcquireQueuedSpinLockAtDpcLevel_k8 (
    IN PKSPIN_LOCK_QUEUE QueuedLock
    )
{
    KiAcquireQueuedLock_k8(QueuedLock);
}


void FASTCALL
KeReleaseQueuedSpinLockFromDpcLevel_k8 (
    IN PKSPIN_LOCK_QUEUE QueuedLock
    )
{
    KiReleaseQueuedLock_k8(QueuedLock);
}


int
memcmpHexSearch(const unsigned char *s1, const unsigned char *s2, size_t n, const unsigned char* patternmask)
{
    if (n != 0) {
        const unsigned char *p1 = s1, *p2 = s2, *p3 = patternmask;
        do {
            if ( (*p1 != *p2) && (*p3 != 0x00) ) // 0x00 - ignore byte
                return (1); // not match
            else {
                p1++; p2++; p3++;
            }

        } while (--n != 0);
    }
    return 0;
}



char*
MemHexSearch(const unsigned char *s, size_t module_size, const unsigned char *find, size_t pattern_size, const unsigned char* patternmask)
{
  unsigned char first_c = find[0];
  size_t i = 0;

  do {
    if (i) { i++; }; // increment if i > 0

    while (s[i] != first_c && i < module_size)
    {
     i++;
    }
  }
  while (memcmpHexSearch(&s[i], find, pattern_size, patternmask) != 0 && i < module_size );

  if (i >= module_size)
      return NULL;
  else
      return (char *) &s[i];
}



char*
ModuleHexSearch(int ModuleNum, const unsigned char* pattern, size_t pattern_size, const unsigned char* patternmask)
{
    unsigned char* ptr;
    int   mod_size;
    char* found_adr;

    ptr = (unsigned char*) GetModuleBaseAddress_k8("", &mod_size, 1, ModuleNum);

    if (ptr) {
        return MemHexSearch(ptr, mod_size, pattern, pattern_size, patternmask);
    } else {
        return NULL;
    }
}



////////////////////////////////////////////////////// 
//  KeInvalidateAllCaches

//  XP RTM/SP1 MP   *:
//  XP SP2/3   MP PAE: 55 8B EC 51 83 3D xx xx xx xx 06 73 xx

//  XP SP2/3   MP    : 55 8B EC 51 83 3D xx xx xx xx 06 0F 82 xx
//  XP RTM-SP3 UP    :             83 3D xx xx xx xx 06 0F 82 xx xx xx xx 0F 09
//  XP RTM-SP3 UP PAE:             83 3D xx xx xx xx 06 73 04 32 C0 EB 04 0F 09
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4)

const unsigned char KeInvalidateAllCaches_magic1[]     =
{0x55, 0x8B, 0xEC, 0x51, 0x83, 0x3D, 0x11, 0x22, 0x33, 0x44, 0x06, 0x73};
const unsigned char KeInvalidateAllCaches_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};    // 0x00 in mask - ignore byte

const unsigned char KeInvalidateAllCaches_magic2[]     =
{0x55, 0x8B, 0xEC, 0x51, 0x83, 0x3D, 0x11, 0x22, 0x33, 0x44, 0x06, 0x0F, 0x82};
const unsigned char KeInvalidateAllCaches_magic2mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff};

const unsigned char KeInvalidateAllCaches_magic3[]     =
{0x83, 0x3D, 0x11, 0x22, 0x33, 0x44, 0x06, 0x0F, 0x82, 0x11, 0x22, 0x33, 0x44, 0x0F, 0x09};
const unsigned char KeInvalidateAllCaches_magic3mask[] =
{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};

const unsigned char KeInvalidateAllCaches_magic4[]     =
{0x83, 0x3D, 0x11, 0x22, 0x33, 0x44, 0x06, 0x73, 0x04, 0x32, 0xC0, 0xEB, 0x04, 0x0F, 0x09};
const unsigned char KeInvalidateAllCaches_magic4mask[] =
{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#endif
////////////////////////////////////////////////////// 


//////////////////////////////////////////////////////   
//  KeAlertThread

// XP* MP:
// 55 8B EC 83 EC 0C 53 56 8B 75 08 8D 8E E8 00 00 00 8D 55 F4 FF 15 xx xx xx xx 64 A1 20 00 00 00 8D 88 18 04 00 00

// XP* UP:
// 55 8B EC 83 EC 0C 53 56 8B 75 08 8D 8E E8 00 00 00 8D 55 F4 FF 15 xx xx xx xx 8A 4D 0C 0F BE C1 8D 44 30 2E

// 2003 SP2 MP:
// 55 8B EC 83 EC 0C 53 56 8B 75 08 57 8D 4E 44 8D 55 F4 FF 15 xx xx xx xx 64 8B 0D 20 00 00 00 BF 18 04 00 00

// 2003 SP2 UP:
// 55 8B EC 83 EC 0C 53 56 8B 75 08 8D 4E 44 8D 55 F4 FF 15 xx xx xx xx 8A 4D 0C 0F BE C1 8D 44 30 5E
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4)

const unsigned char KeAlertThread_magic1[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0x75, 0x08, 0x8D, 0x8E, 0xE8, 0x00, 0x00, 0x00, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x64, 0xA1, 0x20, 0x00, 0x00, 0x00, 0x8D, 0x88, 0x18, 0x04, 0x00, 0x00};

const unsigned char KeAlertThread_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char KeAlertThread_magic2[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0x75, 0x08, 0x8D, 0x8E, 0xE8, 0x00, 0x00, 0x00, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x8A, 0x4D, 0x0C, 0x0F, 0xBE, 0xC1, 0x8D, 0x44, 0x30, 0x2E};

const unsigned char KeAlertThread_magic2mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#endif

#if (NTDDI_VERSION >= NTDDI_WS03SP1) && (NTDDI_VERSION <= NTDDI_WS03SP4)

const unsigned char KeAlertThread_magic1[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0x75, 0x08, 0x57, 0x8D, 0x4E, 0x44, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x64, 0x8B, 0x0D, 0x20, 0x00, 0x00, 0x00, 0xBF, 0x18, 0x04, 0x00, 0x00};
const unsigned char KeAlertThread_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char KeAlertThread_magic2[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x8B, 0x75, 0x08, 0x8D, 0x4E, 0x44, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x8A, 0x4D, 0x0C, 0x0F, 0xBE, 0xC1, 0x8D, 0x44, 0x30, 0x5E};
const unsigned char KeAlertThread_magic2mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#endif
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////   
//  KeTestAlertThread
// XP* MP:
// 55 8B EC 83 EC 0C 53 56 64 A1 24 01 00 00 8B F0 8D 8E E8 00 00 00 8D 55 F4 FF 15 xx xx xx xx 64 A1 20 00 00 00 8D 88 18 04 00 00

// XP* UP:
// 55 8B EC 83 EC 0C 53 56 64 A1 24 01 00 00 8B F0 8D 8E E8 00 00 00 8D 55 F4 FF 15 xx xx xx xx 0F BE 45 08 8D 44 30 2E

// 2003 SP2 *:
// 55 8B EC 83 EC 0C 53 56 64 8B 35 24 01 00 00 8D 4E 44 8D 55 F4 FF 15 xx xx xx xx 0F BE 45 08 8D 44 30 5E
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4)

const unsigned char KeTestAlertThread_magic1[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x64, 0xA1, 0x24, 0x01, 0x00, 0x00, 0x8B, 0xF0, 0x8D, 0x8E, 0xE8, 0x00, 0x00, 0x00, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x64, 0xA1, 0x20, 0x00, 0x00, 0x00, 0x8D, 0x88, 0x18, 0x04, 0x00, 0x00};
const unsigned char KeTestAlertThread_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char KeTestAlertThread_magic2[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x64, 0xA1, 0x24, 0x01, 0x00, 0x00, 0x8B, 0xF0, 0x8D, 0x8E, 0xE8, 0x00, 0x00, 0x00, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x0F, 0xBE, 0x45, 0x08, 0x8D, 0x44, 0x30, 0x2E};
const unsigned char KeTestAlertThread_magic2mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#endif

#if (NTDDI_VERSION >= NTDDI_WS03SP1) && (NTDDI_VERSION <= NTDDI_WS03SP4)

const unsigned char KeTestAlertThread_magic1[]     =
{0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x0C, 0x53, 0x56, 0x64, 0x8B, 0x35, 0x24, 0x01, 0x00, 0x00, 0x8D, 0x4E, 0x44, 0x8D, 0x55, 0xF4, 0xFF, 0x15, 0x11, 0x22, 0x33, 0x44, 0x0F, 0xBE, 0x45, 0x08, 0x8D, 0x44, 0x30, 0x5E};
const unsigned char KeTestAlertThread_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const unsigned char* KeTestAlertThread_magic2     = KeTestAlertThread_magic1;
const unsigned char* KeTestAlertThread_magic2mask = KeTestAlertThread_magic1mask;

#endif
//////////////////////////////////////////////////////   


//////////////////////////////////////////////////////
//  LpcRequestWaitReplyPortEx
//  XP RTM/SP1 *: 64 A1 24 01 00 00 0F BE 80 40 01 00 00 50 FF 74 24 10 FF 74 24 10 FF 74 24 10 E8 B5 FA FF FF C2 0C 00

//  XP SP2/3   *: 55 8B EC 64 A1 24 01 00 00 0F BE 80 40 01 00 00 50 FF 75 10 FF 75 0C FF 75 08 E8 77 FA FF FF 5D C2 0C 00

//  2003       *: 55 8B EC 64 A1 24 01 00 00 0F BE 80 D7 00 00 00 50 FF 75 10 FF 75 0C FF 75 08 E8 07 F9 FF FF 5D C2 0C 00
// 
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4)

const unsigned char LpcRequestWaitReplyPortEx_magic1[]     =
{0x64, 0xA1, 0x24, 0x01, 0x00, 0x00, 0x0F, 0xBE, 0x80, 0x40, 0x01, 0x00, 0x00, 0x50, 0xFF, 0x74, 0x24, 0x10, 0xFF, 0x74, 0x24, 0x10, 0xFF, 0x74, 0x24, 0x10, 0xE8, 0x11, 0x22, 0x33, 0x44, 0xC2, 0x0C, 0x00};
const unsigned char LpcRequestWaitReplyPortEx_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff};

const unsigned char LpcRequestWaitReplyPortEx_magic2[]     =
{0x55, 0x8B, 0xEC, 0x64, 0xA1, 0x24, 0x01, 0x00, 0x00, 0x0F, 0xBE, 0x80, 0x40, 0x01, 0x00, 0x00, 0x50, 0xFF, 0x75, 0x10, 0xFF, 0x75, 0x0C, 0xFF, 0x75, 0x08, 0xE8, 0x11, 0x22, 0x33, 0x44, 0x5D, 0xC2, 0x0C, 0x00};
const unsigned char LpcRequestWaitReplyPortEx_magic2mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff};

#endif


#if (NTDDI_VERSION >= NTDDI_WS03SP1) && (NTDDI_VERSION <= NTDDI_WS03SP4)

const unsigned char LpcRequestWaitReplyPortEx_magic1[]     =
{0x55, 0x8B, 0xEC, 0x64, 0xA1, 0x24, 0x01, 0x00, 0x00, 0x0F, 0xBE, 0x80, 0xD7, 0x00, 0x00, 0x00, 0x50, 0xFF, 0x75, 0x10, 0xFF, 0x75, 0x0C, 0xFF, 0x75, 0x08, 0xE8, 0x11, 0x22, 0x33, 0x44, 0x5D, 0xC2, 0x0C, 0x00};
const unsigned char LpcRequestWaitReplyPortEx_magic1mask[] =
{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff};

const unsigned char* LpcRequestWaitReplyPortEx_magic2     = LpcRequestWaitReplyPortEx_magic1;
const unsigned char* LpcRequestWaitReplyPortEx_magic2mask = LpcRequestWaitReplyPortEx_magic1mask;

#endif
//////////////////////////////////////////////////////



void WRK2003_Init(void)
{
#if (NTDDI_VERSION < NTDDI_VISTA) && defined(_X86_)

#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4)
////////////////////////////////////////////////////// 
//  KeInvalidateAllCaches
gTramp_KeInvalidateAllCaches = (PFN_BOOLEAN) ModuleHexSearch(MODULE_NTOSKRNL, KeInvalidateAllCaches_magic1, sizeof(KeInvalidateAllCaches_magic1), KeInvalidateAllCaches_magic1mask);
if (!gTramp_KeInvalidateAllCaches)
    gTramp_KeInvalidateAllCaches = (PFN_BOOLEAN) ModuleHexSearch(MODULE_NTOSKRNL, KeInvalidateAllCaches_magic2, sizeof(KeInvalidateAllCaches_magic2), KeInvalidateAllCaches_magic2mask);
    if (!gTramp_KeInvalidateAllCaches)
        gTramp_KeInvalidateAllCaches = (PFN_BOOLEAN) ModuleHexSearch(MODULE_NTOSKRNL, KeInvalidateAllCaches_magic3, sizeof(KeInvalidateAllCaches_magic3), KeInvalidateAllCaches_magic3mask);
            if (!gTramp_KeInvalidateAllCaches)
            gTramp_KeInvalidateAllCaches = (PFN_BOOLEAN) ModuleHexSearch(MODULE_NTOSKRNL, KeInvalidateAllCaches_magic4, sizeof(KeInvalidateAllCaches_magic4), KeInvalidateAllCaches_magic4mask);
                if (!gTramp_KeInvalidateAllCaches)
                    KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 2);
//////////////////////////////////////////////////////
#endif // WinXP RTM <> SP4


//////////////////////////////////////////////////////   
//  KeAlertThread
gTramp_KeAlertThread = (PFN_BOOLEAN_PKTHREAD_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeAlertThread_magic1, sizeof(KeAlertThread_magic1), KeAlertThread_magic1mask);
if (!gTramp_KeAlertThread)
    gTramp_KeAlertThread = (PFN_BOOLEAN_PKTHREAD_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeAlertThread_magic2, sizeof(KeAlertThread_magic2), KeAlertThread_magic2mask);
    if (!gTramp_KeAlertThread)
        KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 3);
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////   
//  KeTestAlertThread
gTramp_KeTestAlertThread = (PFN_BOOLEAN_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeTestAlertThread_magic1, sizeof(KeTestAlertThread_magic1), KeTestAlertThread_magic1mask);
if (!gTramp_KeTestAlertThread)
    gTramp_KeTestAlertThread = (PFN_BOOLEAN_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeTestAlertThread_magic2, sizeof(KeTestAlertThread_magic2), KeTestAlertThread_magic2mask);
    if (!gTramp_KeTestAlertThread)
        KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 4);
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////   
//  KeTestAlertThread
gTramp_KeTestAlertThread = (PFN_BOOLEAN_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeTestAlertThread_magic1, sizeof(KeTestAlertThread_magic1), KeTestAlertThread_magic1mask);
if (!gTramp_KeTestAlertThread)
    gTramp_KeTestAlertThread = (PFN_BOOLEAN_KPROCESSOR_MODE) ModuleHexSearch(MODULE_NTOSKRNL, KeTestAlertThread_magic2, sizeof(KeTestAlertThread_magic2), KeTestAlertThread_magic2mask);
    if (!gTramp_KeTestAlertThread)
        KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 4);
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////   
//  LpcRequestWaitReplyPortEx
gTramp_LpcRequestWaitReplyPortEx = (PFN_NTSTATUS_PVOID_PPORT_MESSAGE_PPORT_MESSAGE) ModuleHexSearch(MODULE_NTOSKRNL, LpcRequestWaitReplyPortEx_magic1, sizeof(LpcRequestWaitReplyPortEx_magic1), LpcRequestWaitReplyPortEx_magic1mask);
if (!gTramp_LpcRequestWaitReplyPortEx)
    gTramp_LpcRequestWaitReplyPortEx = (PFN_NTSTATUS_PVOID_PPORT_MESSAGE_PPORT_MESSAGE) ModuleHexSearch(MODULE_NTOSKRNL, LpcRequestWaitReplyPortEx_magic2, sizeof(LpcRequestWaitReplyPortEx_magic2), LpcRequestWaitReplyPortEx_magic2mask);
    if (!gTramp_LpcRequestWaitReplyPortEx)
        KeBugCheckEx(0xDEADBEEFL, 5, 0, 0, 5);
//////////////////////////////////////////////////////


#endif //  < Vista && x32
}




#ifdef __cplusplus
}
#endif