#include <ntimage.h>

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


BOOLEAN FASTCALL
ExAcquireRundownProtectionEx_k8 (
     __inout PEX_RUNDOWN_REF RunRef,
     __in ULONG Count
     );

VOID FASTCALL
ExReleaseRundownProtectionEx_k8 (
     __inout PEX_RUNDOWN_REF RunRef,
     __in ULONG Count
     );

PEX_RUNDOWN_REF
FORCEINLINE
EXP_GET_CURRENT_RUNDOWN_REF (
    IN PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    );

PEX_RUNDOWN_REF
FORCEINLINE
EXP_GET_PROCESSOR_RUNDOWN_REF (
    IN PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    IN ULONG Index
    );

PEX_RUNDOWN_REF_CACHE_AWARE
ExAllocateCacheAwareRundownProtection_k8 (
    __in POOL_TYPE PoolType,
    __in ULONG PoolTag
    );

SIZE_T
ExSizeOfRundownProtectionCacheAware_k8 (
    VOID
    );

VOID
ExInitializeRundownProtectionCacheAware_k8 (
    __out PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    __in SIZE_T RunRefSize
    );

VOID
ExFreeCacheAwareRundownProtection_k8 (
    __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    );

BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );

VOID
FASTCALL
ExReleaseRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );

BOOLEAN
FASTCALL
ExAcquireRundownProtectionCacheAwareEx_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     __in ULONG Count
     );

VOID
FASTCALL
ExReleaseRundownProtectionCacheAwareEx_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     __in ULONG Count
     );

VOID
FASTCALL
ExWaitForRundownProtectionReleaseCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );

VOID
FASTCALL
ExReInitializeRundownProtectionCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );

VOID
FASTCALL
ExRundownCompletedCacheAware_k8 (
     __inout PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );

PVOID						// exportable by ntoskrnl
PsGetThreadWin32Thread (PRKTHREAD Thread);

PVOID
ExEnterCriticalRegionAndAcquireResourceExclusive_k8 (
    PERESOURCE Resource );

VOID FASTCALL
ExReleaseResourceAndLeaveCriticalRegion_k8 (
    PERESOURCE Resource );


#define LDR_VIEW_TO_DATAFILE(x) ((PVOID)(((ULONG_PTR)(x)) |  (ULONG_PTR)1))
#define LDR_IS_DATAFILE(x)              (((ULONG_PTR)(x)) &  (ULONG_PTR)1)
#define LDR_IS_VIEW(x)                  (!LDR_IS_DATAFILE(x))
#define LDR_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)1))

#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK (0x00000001)

NTSTATUS
NTAPI
RtlImageNtHeaderEx (
    ULONG Flags,
    PVOID Base,
    ULONG64 Size,
    OUT PIMAGE_NT_HEADERS * OutHeaders
    );

PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader (
    PVOID Base
    );

PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    );

PVOID
RtlAddressInSectionTable (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    );

PVOID
RtlpImageDirectoryEntryToData32 (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS32 NtHeaders
    );

PVOID
RtlpImageDirectoryEntryToData64 (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS64 NtHeaders
    );

PVOID
RtlImageDirectoryEntryToData (
    IN PVOID Base,
    IN BOOLEAN MappedAsImage,
    IN USHORT DirectoryEntry,
    OUT PULONG Size
    );

PVOID
MiFindExportedRoutineByName (
    IN PVOID DllBase,
    IN PANSI_STRING AnsiImageRoutineName
    );

