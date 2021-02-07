#include <ntimage.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const LARGE_INTEGER  MmShortTime;
extern       KIRQL          gGuardedRegion_OldIrql;
extern       LONG           gGuardedRegionCounter;


/*
Copyright (c) Microsoft Corporation. All rights reserved. 

You may only use this code if you agree to the terms of the Windows Research Kernel Source Code License agreement (see License.txt).
If you do not agree to the terms, do not use the code.
*/


typedef USHORT RTL_ATOM, *PRTL_ATOM;
#define RTL_ATOM_MAXIMUM_INTEGER_ATOM   (RTL_ATOM)0xC000
#define RTL_ATOM_INVALID_ATOM           (RTL_ATOM)0x0000
#define RTL_ATOM_TABLE_DEFAULT_NUMBER_OF_BUCKETS 37
#define RTL_ATOM_MAXIMUM_NAME_LENGTH    255
#define RTL_ATOM_PINNED 0x01

#define ALERT_INCREMENT 2           // Alerted unwait priority increment
#define BALANCE_INCREMENT 10        // Balance set priority increment
#define RESUME_INCREMENT 0          // Resume thread priority increment
#define TIMER_EXPIRE_INCREMENT 0    // Timer expiration priority increment

#define THREAD_TO_PROCESS(Thread) ((Thread)->ThreadsProcess)

#define KeGetPreviousMode() (KeGetCurrentThread()->PreviousMode)

#define KeMemoryBarrierWithoutFence() _ReadWriteBarrier()

#define _PsGetCurrentThread() ((PETHREAD)KeGetCurrentThread())

#define KeActiveProcessors KeQueryActiveProcessors()

#define KeYieldProcessor YieldProcessor


#define OB_FLAG_NEW_OBJECT              0x01
#define OB_FLAG_KERNEL_OBJECT           0x02
#define OB_FLAG_CREATOR_INFO            0x04
#define OB_FLAG_EXCLUSIVE_OBJECT        0x08
#define OB_FLAG_PERMANENT_OBJECT        0x10
#define OB_FLAG_DEFAULT_SECURITY_QUOTA  0x20
#define OB_FLAG_SINGLE_HANDLE_ENTRY     0x40
#define OB_FLAG_DELETED_INLINE          0x80

#define EX_PUSH_LOCK_WAITING   0x1
#define EX_PUSH_LOCK_EXCLUSIVE 0x2
#define EX_PUSH_LOCK_SHARE_INC 0x4


typedef enum _KTHREAD_STATE {
    Initialized,
    Ready,
    Running,
    Standby,
    Terminated,
    Waiting,
    Transition,
    DeferredReady,
    GateWait
} KTHREAD_STATE;



#if defined(_AMD64_)
    #define LOCK_QUEUE_TIMER_LOCK_SHIFT 4
    #define LOCK_QUEUE_TIMER_TABLE_LOCKS (1 << (8 - LOCK_QUEUE_TIMER_LOCK_SHIFT))
    #define LockQueueDispatcherLock 0

#else // _X86_
    #define LOCK_QUEUE_TIMER_LOCK_SHIFT 4
    #define LOCK_QUEUE_TIMER_TABLE_LOCKS (1 << (8 - LOCK_QUEUE_TIMER_LOCK_SHIFT))

    typedef enum _KSPIN_LOCK_QUEUE_NUMBER_XP {
        LockQueueDispatcherLock
        //...
    } KSPIN_LOCK_QUEUE_NUMBER_XP, *PKSPIN_LOCK_QUEUE_NUMBER_XP;
#endif


// hal import

VOID FASTCALL
KeAcquireQueuedSpinLockAtDpcLevel (
     PKSPIN_LOCK_QUEUE QueuedLock
     );


VOID FASTCALL
KeReleaseQueuedSpinLockFromDpcLevel (
    PKSPIN_LOCK_QUEUE QueuedLock
    );


#define NtQuery_BUFFERSIZE (100 * 1024) // 100KB/sizeof(RTL_PROCESS_MODULE_INFORMATION) 100*1024/284 = 360 max records

typedef struct _OBJECT_CREATE_INFORMATION {
    ULONG Attributes;
    HANDLE RootDirectory;
    PVOID ParseContext;
    KPROCESSOR_MODE ProbeMode;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    ULONG SecurityDescriptorCharge;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PSECURITY_QUALITY_OF_SERVICE SecurityQos;
    SECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
} OBJECT_CREATE_INFORMATION;


typedef struct _RTL_PROCESS_MODULE_INFORMATION {
    HANDLE Section;                 // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;


typedef struct _RTL_PROCESS_MODULES {
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;


typedef struct _KLDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    // ULONG padding on IA64
    PVOID GpValue;
    PNON_PAGED_DEBUG_INFO NonPagedDebugInfo;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT __Unused5;
    PVOID SectionPointer;
    ULONG CheckSum;
    // ULONG padding on IA64
    PVOID LoadedImports;
    PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;



VOID                            // ntoskrnl.exe
SeReleaseSecurityDescriptor (
    PSECURITY_DESCRIPTOR CapturedSecurityDescriptor,
    KPROCESSOR_MODE RequestorMode,
    BOOLEAN ForceCapture
    );

PVOID                           // ntoskrnl.exe
PsGetThreadWin32Thread (PRKTHREAD Thread);


VOID FASTCALL                   // ntoskrnl.exe
ExfReleasePushLock (
    PEX_PUSH_LOCK PushLock
    );


NTSTATUS                        // ntoskrnl.exe
LpcRequestWaitReplyPort (
    IN PVOID PortAddress,
    IN PPORT_MESSAGE RequestMessage,
    OUT PPORT_MESSAGE ReplyMessage
    );


NTSTATUS                        // ntoskrnl.exe
MmGrowKernelStack (
    PVOID CurrentStack
    );


NTSTATUS
NtAllocateLocallyUniqueId(PLUID Luid); // ntoskrnl.exe



ULONG_PTR                       // ntoskrn8_helpers.asm
KeGetCurrentStackPointer (
    VOID
    );


typedef enum _PP_NPAGED_LOOKASIDE_NUMBER {
    LookasideSmallIrpList,
    LookasideLargeIrpList,
    LookasideMdlList,
    LookasideCreateInfoList,
    LookasideNameBufferList,
    LookasideTwilightList,
    LookasideCompletionList,
    LookasideMaximumList
} PP_NPAGED_LOOKASIDE_NUMBER, *PPP_NPAGED_LOOKASIDE_NUMBER;


typedef struct _EX_RUNDOWN_REF_CACHE_AWARE {
    PEX_RUNDOWN_REF RunRefs;
    PVOID PoolToFree;
    ULONG RunRefSize;
    ULONG Number;
} EX_RUNDOWN_REF_CACHE_AWARE, *PEX_RUNDOWN_REF_CACHE_AWARE;


#define EXP_ALIGN_UP_PTR_ON_BOUNDARY(_length, _alignment)                      \
          (PVOID) ((((ULONG_PTR) (_length)) + ((_alignment)-1)) &              \
                              ~(ULONG_PTR)((_alignment) - 1))
            

#define EXP_IS_ALIGNED_ON_BOUNDARY(_pointer, _alignment)                       \
        ((((ULONG_PTR) (_pointer)) & ((_alignment) - 1)) == 0)


typedef struct _EX_RUNDOWN_WAIT_BLOCK {
    ULONG_PTR Count;
    KEVENT WakeEvent;
} EX_RUNDOWN_WAIT_BLOCK, *PEX_RUNDOWN_WAIT_BLOCK;


typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation,
    MemoryWorkingSetList,
    MemorySectionName,
    MemoryBasicVlmInformation
} MEMORY_INFORMATION_CLASS; 



//////////////////////////////////////////////////////////////////////
//       INLINE FUNCTIONS


VOID FORCEINLINE
KeEnterCriticalRegionThread (
    PKTHREAD Thread
    )
{
    Thread->KernelApcDisable -= 1;
    KeMemoryBarrierWithoutFence();
}


PEX_RUNDOWN_REF FORCEINLINE
EXP_GET_CURRENT_RUNDOWN_REF(
   PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    )
{
    return ((PEX_RUNDOWN_REF) (((PUCHAR) RunRefCacheAware->RunRefs) +
                               (KeGetCurrentProcessorNumber() % RunRefCacheAware->Number) * RunRefCacheAware->RunRefSize));
}


PEX_RUNDOWN_REF FORCEINLINE
EXP_GET_PROCESSOR_RUNDOWN_REF(
   PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
   ULONG Index
    )
{
    return ((PEX_RUNDOWN_REF) (((PUCHAR) RunRefCacheAware->RunRefs) +
                                (Index % RunRefCacheAware->Number) * RunRefCacheAware->RunRefSize));
}


PKPRCB FORCEINLINE
KeGetCurrentPrcb (VOID)
{
    #if defined(_X86_)
        return (PKPRCB) (ULONG_PTR) __readfsdword (FIELD_OFFSET (KPCR, Prcb));
    #else
        return (PKPRCB)__readgsqword(FIELD_OFFSET(KPCR, CurrentPrcb));
    #endif
}


VOID FORCEINLINE
ExFreeToPPLookasideList (
    PP_NPAGED_LOOKASIDE_NUMBER Number,
    PVOID Entry
    )
{

    PGENERAL_LOOKASIDE Lookaside;
    PKPRCB Prcb;

    Prcb = KeGetCurrentPrcb();
    Lookaside = Prcb->PPLookasideList[Number].P;
    Lookaside->TotalFrees += 1;
    if (ExQueryDepthSList(&Lookaside->ListHead) >= Lookaside->Depth) {
        Lookaside->FreeMisses += 1;
        Lookaside = Prcb->PPLookasideList[Number].L;
        Lookaside->TotalFrees += 1;
        if (ExQueryDepthSList(&Lookaside->ListHead) >= Lookaside->Depth) {
            Lookaside->FreeMisses += 1;
            (Lookaside->Free)(Entry);
            return;
        }
    }

    InterlockedPushEntrySList(&Lookaside->ListHead,
                              (PSLIST_ENTRY)Entry);

    return;
}




////////////////////////////////////////////////////////////////////////////



#define ObpFreeObjectCreateInfoBuffer(ObjectCreateInfo) \
    ExFreeToPPLookasideList(LookasideCreateInfoList, ObjectCreateInfo)

#define ObpReleaseObjectCreateInformation(_ObjectCreateInfo) {               \
    if ((_ObjectCreateInfo)->SecurityDescriptor != NULL) {                   \
        SeReleaseSecurityDescriptor((_ObjectCreateInfo)->SecurityDescriptor, \
                                    (_ObjectCreateInfo)->ProbeMode,          \
                                     TRUE);                                  \
        (_ObjectCreateInfo)->SecurityDescriptor = NULL;                      \
    }                                                                        \
}

#define ObpFreeObjectCreateInformation(_ObjectCreateInfo) { \
    ObpReleaseObjectCreateInformation((_ObjectCreateInfo)); \
    ObpFreeObjectCreateInfoBuffer((_ObjectCreateInfo));     \
}


NTSTATUS
LdrFindResource_U(
    PVOID DllHandle,
    CONST ULONG_PTR* ResourceIdPath,
    ULONG ResourceIdPathLength,
    PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    );


NTSTATUS
LdrAccessResource(
    PVOID DllHandle,
    CONST IMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry,
    PVOID *Address,
    PULONG Size
    );


NTSTATUS
LdrFindResourceDirectory_U(
    PVOID DllHandle,
    CONST ULONG_PTR* ResourceIdPath,
    ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DIRECTORY *ResourceDirectory
    );


VOID FASTCALL
KeAcquireInStackQueuedSpinLockRaiseToSynch (
    PKSPIN_LOCK SpinLock,
    PKLOCK_QUEUE_HANDLE LockHandle
    );


BOOLEAN FASTCALL
ExAcquireRundownProtectionEx_k8 (
     PEX_RUNDOWN_REF RunRef,
     ULONG Count
     );


VOID FASTCALL
ExReleaseRundownProtectionEx_k8 (
     PEX_RUNDOWN_REF RunRef,
     ULONG Count
     );


PEX_RUNDOWN_REF_CACHE_AWARE
ExAllocateCacheAwareRundownProtection_k8 (
    POOL_TYPE PoolType,
    ULONG PoolTag
    );


SIZE_T
ExSizeOfRundownProtectionCacheAware_k8 (
    VOID
    );


VOID
ExInitializeRundownProtectionCacheAware_k8 (
    __out PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
    SIZE_T RunRefSize
    );


VOID
ExFreeCacheAwareRundownProtection_k8 (
       PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
    );


BOOLEAN FASTCALL
ExAcquireRundownProtectionCacheAware_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );


VOID FASTCALL
ExReleaseRundownProtectionCacheAware_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );


BOOLEAN FASTCALL
ExAcquireRundownProtectionCacheAwareEx_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     ULONG Count
     );


VOID FASTCALL
ExReleaseRundownProtectionCacheAwareEx_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware,
     ULONG Count
     );


VOID FASTCALL
ExWaitForRundownProtectionReleaseCacheAware_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );


VOID FASTCALL
ExReInitializeRundownProtectionCacheAware_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );


VOID FASTCALL
ExRundownCompletedCacheAware_k8 (
        PEX_RUNDOWN_REF_CACHE_AWARE RunRefCacheAware
     );


PVOID
ExEnterCriticalRegionAndAcquireResourceExclusive_k8 (
    PERESOURCE Resource );


PVOID
ExEnterCriticalRegionAndAcquireResourceShared_k8 (
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
RtlImageNtHeaderEx_k8 (
    ULONG Flags,
    PVOID Base,
    ULONG64 Size,
    OUT PIMAGE_NT_HEADERS * OutHeaders
    );


PIMAGE_NT_HEADERS
RtlImageNtHeader_k8 (
    PVOID Base
    );


PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress_k8 (
    PIMAGE_NT_HEADERS NtHeaders,
    PVOID Base,
    ULONG Address
    );


PVOID
RtlAddressInSectionTable_k8 (
    PIMAGE_NT_HEADERS NtHeaders,
    PVOID Base,
    ULONG Address
    );


PVOID
RtlpImageDirectoryEntryToData32_k8 (
    PVOID Base,
    BOOLEAN MappedAsImage,
    USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS32 NtHeaders
    );


PVOID
RtlpImageDirectoryEntryToData64_k8 (
    PVOID Base,
    BOOLEAN MappedAsImage,
    USHORT DirectoryEntry,
    OUT PULONG Size,
    PIMAGE_NT_HEADERS64 NtHeaders
    );


PVOID
RtlImageDirectoryEntryToData_k8 (
    PVOID Base,
    BOOLEAN MappedAsImage,
    USHORT DirectoryEntry,
    OUT PULONG Size
    );


PVOID
MiFindExportedRoutineByName_k8 (
    PVOID DllBase,
    PANSI_STRING AnsiImageRoutineName
    );


HANDLE
PsGetCurrentThreadProcessId_k8 ( VOID );


BOOLEAN
KeAreAllApcsDisabled_k8 ( VOID );


BOOLEAN
KeInvalidateAllCaches_k8 (VOID);


BOOLEAN
RtlGetIntegerAtom_k8 (
    PWSTR Name,
    PRTL_ATOM Atom OPTIONAL );


VOID FASTCALL
ExEnterCriticalRegionAndAcquireFastMutexUnsafe (
    PFAST_MUTEX FastMutex
    );    


PVOID
GetRoutineAddress_k8 (
    PUNICODE_STRING SystemRoutineName,
    const PCHAR Modulename);


PVOID
GetModuleBaseAddress_k8 (
    const PCHAR Modulename,
    int*  ModuleSize,
    int   ByOrderMode,
    int   OrderNum);


NTSTATUS
ZwQueryVirtualMemory_k8 (
    HANDLE  Handle,
    void const * Ptr,
    MEMORY_INFORMATION_CLASS Class,
    PVOID Ptr2,
    SIZE_T Size,
    PSIZE_T Size2);


#define MODULE_NTOSKRNL 0
#define MODULE_HAL      1

void
WRK2003_Init(void);


#ifdef __cplusplus
}
#endif    