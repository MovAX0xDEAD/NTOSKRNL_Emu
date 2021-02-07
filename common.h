// structs decompiled by PDB Ripper 2.01

#ifndef _COMMON_H
#define _COMMON_H

#define XOR ^
#define OR  |
#define AND &
#define NOT !


#ifdef __cplusplus
extern "C" {
#endif


#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))

#define TRY
#define LEAVE   goto __tryLabel;
#define FINALLY __tryLabel:

#define uint8_t  UCHAR
#define uint32_t ULONG
#define int32_t  LONG

#define CONSTANT_STR(s) { sizeof( s ) - sizeof( (s)[0] ), sizeof( s ), s }

#define OBJECT_TO_OBJECT_HEADER( o ) \
    CONTAINING_RECORD( (o), OBJECT_HEADER, Body )


typedef struct _CPU_INFO {
    ULONG Eax;
    ULONG Ebx;
    ULONG Ecx;
    ULONG Edx;
} CPU_INFO, *PCPU_INFO;


typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,             // obsolete...delete
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation,
    SystemLocksInformation,
    SystemStackTraceInformation,
    SystemPagedPoolInformation,
    SystemNonPagedPoolInformation,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPageFileInformation,
    SystemVdmInstemulInformation,
    SystemVdmBopInformation,
    SystemFileCacheInformation,
    SystemPoolTagInformation,
    SystemInterruptInformation,
    SystemDpcBehaviorInformation,
    SystemFullMemoryInformation,
    SystemLoadGdiDriverInformation,
    SystemUnloadGdiDriverInformation,
    SystemTimeAdjustmentInformation,
    SystemSummaryMemoryInformation,
    SystemMirrorMemoryInformation,
    SystemPerformanceTraceInformation,
    SystemObsolete0,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemExtendServiceTableInformation,
    SystemPrioritySeperation,
    SystemVerifierAddDriverInformation,
    SystemVerifierRemoveDriverInformation,
    SystemProcessorIdleInformation,
    SystemLegacyDriverInformation,
    SystemCurrentTimeZoneInformation,
    SystemLookasideInformation,
    SystemTimeSlipNotification,
    SystemSessionCreate,
    SystemSessionDetach,
    SystemSessionInformation,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemVerifierThunkExtend,
    SystemSessionProcessInformation,
    SystemLoadGdiDriverInSystemSpace,
    SystemNumaProcessorMap,
    SystemPrefetcherInformation,
    SystemExtendedProcessInformation,
    SystemRecommendedSharedDataAlignment,
    SystemComPlusPackage,
    SystemNumaAvailableMemory,
    SystemProcessorPowerInformation,
    SystemEmulationBasicInformation,
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation,
    SystemLostDelayedWriteInformation,
    SystemBigPoolInformation,
    SystemSessionPoolTagInformation,
    SystemSessionMappedViewInformation,
    SystemHotpatchInformation,
    SystemObjectSecurityMode,
    SystemWatchdogTimerHandler,
    SystemWatchdogTimerInformation,
    SystemLogicalProcessorInformation,
    SystemWow64SharedInformation,
    SystemRegisterFirmwareTableInformationHandler,
    SystemFirmwareTableInformation,
    SystemModuleInformationEx,
    SystemVerifierTriageInformation,
    SystemSuperfetchInformation,
    SystemMemoryListInformation,
    SystemFileCacheInformationEx,
    MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;


/////////////////////////////////////////////////////////
// early + external definition


struct  _PORT_MESSAGE;
typedef struct  _PORT_MESSAGE  PORT_MESSAGE, *PPORT_MESSAGE;


struct _EX_PUSH_LOCK;
typedef struct _EX_PUSH_LOCK EX_PUSH_LOCK, *PEX_PUSH_LOCK;


BOOLEAN
KeSetCoalescableTimer_k8 (
    KTIMER         *Timer,
    LARGE_INTEGER   DueTime,
    ULONG           Period,
    ULONG           TolerableDelay,
    KDPC           *Dpc);



// void PrintNumaCpuConfiguration(void);     // test.c


extern PVOID CREATE_PROCESS_NOTIFY_ROUTINE_asm; // ntoskrn8_helpers.asm


VOID
KeSetSystemAffinityThread_k8 (
    KAFFINITY Affinity
    );


#if defined(_X86_)
    extern void
    CPUID (                 // x32 ntoskrn8_helpers.asm
        ULONG   InEax,
        PULONG  OutEax,
        PULONG  OutEbx,
        PULONG  OutEcx,
        PULONG  OutEdx );
#else    
    extern void
    KiCpuId (               // x64 ntoskrn8_helpers.asm
        ULONG Function,
        ULONG Index,
        PCPU_INFO CpuInfo );
#endif


BOOLEAN
KeAddSystemServiceTable (               // ntoskrnl.exe
    PULONG_PTR Base,
    PULONG Count OPTIONAL,
    ULONG Limit,
    PUCHAR Number,
    ULONG Index );


NTSTATUS
NtQuerySystemInformation (	            // ntoskrnl.exe
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
    );


void                                    // ntoskrn8_redirects.asm
InitTramplineArray(void);


// early + external definition
/////////////////////////////////////////////////////////

typedef struct _IO_WORKITEM_EX *PIO_WORKITEM_EX;

typedef VOID IO_WORKITEM_ROUTINE_EX_k8 (
    PVOID IoObject,
    PVOID Context,
    PIO_WORKITEM_EX IoWorkItem
    );

typedef IO_WORKITEM_ROUTINE_EX_k8 *PIO_WORKITEM_ROUTINE_EX_k8;

typedef struct _IO_WORKITEM_EX {
    WORK_QUEUE_ITEM            WorkItem;
    PIO_WORKITEM_ROUTINE_EX_k8 Routine;
    PDEVICE_OBJECT             IoObject;
    PVOID                      Context;
    ULONG                      Type;
} IO_WORKITEM_EX;


#if (NTDDI_VERSION < NTDDI_VISTA)            // Windows XP/2003
    typedef struct _IO_WORKITEM {
        WORK_QUEUE_ITEM         WorkItem;
        PIO_WORKITEM_ROUTINE    Routine;
        PDEVICE_OBJECT          DeviceObject;
        PVOID                   Context;
    } IO_WORKITEM;

#else                                           //  Vista+
    typedef struct _IO_WORKITEM {
        WORK_QUEUE_ITEM         WorkItem;
        PIO_WORKITEM_ROUTINE_EX Routine;
        PVOID                   IoObject;
        PVOID                   Context;
        ULONG                   Type;
        // struct _GUID ActivityId;             // Windows 8+
    } IO_WORKITEM;
#endif


#if (NTDDI_VERSION < NTDDI_WINXPSP2)           // exportable since WINXP SP2
 #define ExAcquireRundownProtectionEx ExAcquireRundownProtectionEx_k8
 #define ExReleaseRundownProtectionEx ExReleaseRundownProtectionEx_k8
#endif


#if (NTDDI_VERSION < NTDDI_WIN7)                // exportable since WIN7
 #define KeSetCoalescableTimer KeSetCoalescableTimer_k8
#endif


struct _MMSUPPORT_FLAGS             // Size=0x4     WinXP SP3 !!!
{
    unsigned int SessionSpace:1;                // Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
    unsigned int BeingTrimmed:1;                // Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
    unsigned int SessionLeader:1;               // Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
    unsigned int TrimHard:1;                    // Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1
    unsigned int WorkingSetHard:1;              // Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1
    unsigned int AddressSpaceBeingDeleted:1;    // Offset=0x0 Size=0x4 BitOffset=0x5 BitSize=0x1
    unsigned int Available:10;                  // Offset=0x0 Size=0x4 BitOffset=0x6 BitSize=0xa
    unsigned int AllowWorkingSetAdjustment:8;   // Offset=0x0 Size=0x4 BitOffset=0x10 BitSize=0x8
    unsigned int MemoryPriority:8;              // Offset=0x0 Size=0x4 BitOffset=0x18 BitSize=0x8
};

typedef struct _MMSECTION_FLAGS// Size=0x4
{
    unsigned int BeingDeleted:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
    unsigned int BeingCreated:1;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
    unsigned int BeingPurged:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
    unsigned int NoModifiedWriting:1;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1
    unsigned int FailAllIo:1;// Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1
    unsigned int Image:1;// Offset=0x0 Size=0x4 BitOffset=0x5 BitSize=0x1
    unsigned int Based:1;// Offset=0x0 Size=0x4 BitOffset=0x6 BitSize=0x1
    unsigned int File:1;// Offset=0x0 Size=0x4 BitOffset=0x7 BitSize=0x1
    unsigned int Networked:1;// Offset=0x0 Size=0x4 BitOffset=0x8 BitSize=0x1
    unsigned int NoCache:1;// Offset=0x0 Size=0x4 BitOffset=0x9 BitSize=0x1
    unsigned int PhysicalMemory:1;// Offset=0x0 Size=0x4 BitOffset=0xa BitSize=0x1
    unsigned int CopyOnWrite:1;// Offset=0x0 Size=0x4 BitOffset=0xb BitSize=0x1
    unsigned int Reserve:1;// Offset=0x0 Size=0x4 BitOffset=0xc BitSize=0x1
    unsigned int Commit:1;// Offset=0x0 Size=0x4 BitOffset=0xd BitSize=0x1
    unsigned int FloppyMedia:1;// Offset=0x0 Size=0x4 BitOffset=0xe BitSize=0x1
    unsigned int WasPurged:1;// Offset=0x0 Size=0x4 BitOffset=0xf BitSize=0x1
    unsigned int UserReference:1;// Offset=0x0 Size=0x4 BitOffset=0x10 BitSize=0x1
    unsigned int GlobalMemory:1;// Offset=0x0 Size=0x4 BitOffset=0x11 BitSize=0x1
    unsigned int DeleteOnClose:1;// Offset=0x0 Size=0x4 BitOffset=0x12 BitSize=0x1
    unsigned int FilePointerNull:1;// Offset=0x0 Size=0x4 BitOffset=0x13 BitSize=0x1
    unsigned int DebugSymbolsLoaded:1;// Offset=0x0 Size=0x4 BitOffset=0x14 BitSize=0x1
    unsigned int SetMappedFileIoComplete:1;// Offset=0x0 Size=0x4 BitOffset=0x15 BitSize=0x1
    unsigned int CollidedFlush:1;// Offset=0x0 Size=0x4 BitOffset=0x16 BitSize=0x1
    unsigned int NoChange:1;// Offset=0x0 Size=0x4 BitOffset=0x17 BitSize=0x1
    unsigned int HadUserReference:1;// Offset=0x0 Size=0x4 BitOffset=0x18 BitSize=0x1
    unsigned int ImageMappedInSystemSpace:1;// Offset=0x0 Size=0x4 BitOffset=0x19 BitSize=0x1
    unsigned int UserWritable:1;// Offset=0x0 Size=0x4 BitOffset=0x1a BitSize=0x1
    unsigned int Accessed:1;// Offset=0x0 Size=0x4 BitOffset=0x1b BitSize=0x1
    unsigned int GlobalOnlyPerSession:1;// Offset=0x0 Size=0x4 BitOffset=0x1c BitSize=0x1
    unsigned int Rom:1;// Offset=0x0 Size=0x4 BitOffset=0x1d BitSize=0x1
    unsigned int filler:2;// Offset=0x0 Size=0x4 BitOffset=0x1e BitSize=0x2
} MMSECTION_FLAGS;

typedef struct _KEXECUTE_OPTIONS
{
    UCHAR ExecuteDisable : 1;
    UCHAR ExecuteEnable : 1;
    UCHAR DisableThunkEmulation : 1;
    UCHAR Permanent : 1;
    UCHAR ExecuteDispatchEnable : 1;
    UCHAR ImageDispatchEnable : 1;
    UCHAR Spare : 2;
} KEXECUTE_OPTIONS, PKEXECUTE_OPTIONS;

typedef struct _KIDTENTRY // Size=0x8
{
    USHORT Offset;         // Offset=0x0 Size=0x2
    USHORT Selector;       // Offset=0x2 Size=0x2
    USHORT Access;         // Offset=0x4 Size=0x2
    USHORT ExtendedOffset; // Offset=0x6 Size=0x2
} KIDTENTRY;

struct _PP_LOOKASIDE_LIST// Size=0x8
{
    struct _GENERAL_LOOKASIDE *P;// Offset=0x0 Size=0x4
    struct _GENERAL_LOOKASIDE *L;// Offset=0x4 Size=0x4
};

typedef struct _SE_AUDIT_PROCESS_CREATION_INFO {
    POBJECT_NAME_INFORMATION ImageFileName;
} SE_AUDIT_PROCESS_CREATION_INFO, *PSE_AUDIT_PROCESS_CREATION_INFO;

struct PROCESSOR_IDLE_TIMES// Size=0x20
{
    ULONG64 StartTime;// Offset=0x0 Size=0x8
    ULONG64 EndTime;// Offset=0x8 Size=0x8
    ULONG     IdleHandlerReserved[4];// Offset=0x10 Size=0x10
};

typedef struct _MM_SESSION_SPACE_FLAGS// Size=0x4
{
    unsigned long Initialized:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
    unsigned long Filler0:3;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x3
    unsigned long HasWsLock:1;// Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1
    unsigned long DeletePending:1;// Offset=0x0 Size=0x4 BitOffset=0x5 BitSize=0x1
    unsigned long Filler:26;// Offset=0x0 Size=0x4 BitOffset=0x6 BitSize=0x1a
} MM_SESSION_SPACE_FLAGS;

typedef struct _KGDTENTRY
{
    USHORT LimitLow;
    USHORT BaseLow;
    union {
        struct
        {
            UCHAR BaseMid;
            UCHAR Flags1; // Declare as bytes to avoid alignment
            UCHAR Flags2; // Problems.
            UCHAR BaseHi;
        } Bytes;
        struct
        {
            ULONG BaseMid : 8;
            ULONG Type : 5;
            ULONG Dpl : 2;
            ULONG Pres : 1;
            ULONG LimitHi : 4;
            ULONG Sys : 1;
            ULONG Reserved_0 : 1;
            ULONG Default_Big : 1;
            ULONG Granularity : 1;
            ULONG BaseHi : 8;
        } Bits;
    } HighWord;
} KGDTENTRY, *PKGDTENTRY;

#define _HARDWARE_PTE_WORKING_SET_BITS  11
typedef ULONG MM_PROTECTION_MASK;

#ifdef _X86_
#if !defined (_X86PAE_)
    struct _HARDWARE_PTE// Size=0x4 // NOPAE
    {
        unsigned long xxx;// Offset=0x0 Size=0x4
    };
#else
    struct _HARDWARE_PTE// Size=0x8 // PAE
    {
        unsigned long xxx;// Offset=0x0 Size=0x4
        unsigned long xxx2;// Offset=0x4 Size=0x4
    };
#endif
#else
    struct _HARDWARE_PTE {
        ULONG64 Valid : 1;
        ULONG64 Write : 1;                // UP version
        ULONG64 Owner : 1;
        ULONG64 WriteThrough : 1;
        ULONG64 CacheDisable : 1;
        ULONG64 Accessed : 1;
        ULONG64 Dirty : 1;
        ULONG64 LargePage : 1;
        ULONG64 Global : 1;
        ULONG64 CopyOnWrite : 1;          // software field
        ULONG64 Prototype : 1;            // software field
        ULONG64 reserved0 : 1;            // software field
        ULONG64 PageFrameNumber : 28;
        ULONG64 reserved1 : 24 - (_HARDWARE_PTE_WORKING_SET_BITS+1);
        ULONG64 SoftwareWsIndex : _HARDWARE_PTE_WORKING_SET_BITS;
        ULONG64 NoExecute : 1;
    };
#endif

struct _DESCRIPTOR// Size=0x8
{
    unsigned short Pad;// Offset=0x0 Size=0x2
    unsigned short Limit;// Offset=0x2 Size=0x2
    unsigned long Base;// Offset=0x4 Size=0x4
};

struct _FNSAVE_FORMAT// Size=0x6c
{
    unsigned long ControlWord;// Offset=0x0 Size=0x4
    unsigned long StatusWord;// Offset=0x4 Size=0x4
    unsigned long TagWord;// Offset=0x8 Size=0x4
    unsigned long ErrorOffset;// Offset=0xc Size=0x4
    unsigned long ErrorSelector;// Offset=0x10 Size=0x4
    unsigned long DataOffset;// Offset=0x14 Size=0x4
    unsigned long DataSelector;// Offset=0x18 Size=0x4
    unsigned char RegisterArea[80];// Offset=0x1c Size=0x50
};

struct _FXSAVE_FORMAT// Size=0x208
{
    unsigned short ControlWord;// Offset=0x0 Size=0x2
    unsigned short StatusWord;// Offset=0x2 Size=0x2
    unsigned short TagWord;// Offset=0x4 Size=0x2
    unsigned short ErrorOpcode;// Offset=0x6 Size=0x2
    unsigned long ErrorOffset;// Offset=0x8 Size=0x4
    unsigned long ErrorSelector;// Offset=0xc Size=0x4
    unsigned long DataOffset;// Offset=0x10 Size=0x4
    unsigned long DataSelector;// Offset=0x14 Size=0x4
    unsigned long MXCsr;// Offset=0x18 Size=0x4
    unsigned long MXCsrMask;// Offset=0x1c Size=0x4
    unsigned char RegisterArea[128];// Offset=0x20 Size=0x80
    unsigned char Reserved3[128];// Offset=0xa0 Size=0x80
    unsigned char Reserved4[224];// Offset=0x120 Size=0xe0
    unsigned char Align16Byte[8];// Offset=0x200 Size=0x8
};

struct _FX_SAVE_AREA// Size=0x210
{
    union {
        struct _FNSAVE_FORMAT FnArea;   // Offset=0x0 Size=0x6c
        struct _FXSAVE_FORMAT FxArea;   // Offset=0x0 Size=0x208
    } U;                                // Offset=0x0 Size=0x208
    unsigned long NpxSavedCpu;          // Offset=0x208 Size=0x4
    unsigned long Cr0NpxState;          // Offset=0x20c Size=0x4
};



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows XP x32 internal kernel definitions                        //
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4) && defined(_X86_)

typedef struct _MM_SESSION_SPACE// Size=0x1278
{
    unsigned long ReferenceCount;// Offset=0x0 Size=0x4
    union {
        ULONG LongFlags;
        MM_SESSION_SPACE_FLAGS Flags;
    } u;
    ULONG SessionId; // Offset=0x8 Size=0x4
    // ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;

struct _EX_PUSH_LOCK// Size=0x4
{
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long Waiting:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long Exclusive:1;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long Shared:30;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1e
        };
        unsigned long Value;// Offset=0x0 Size=0x4
        void *Ptr;// Offset=0x0 Size=0x4
    };
};

typedef struct _EX_FAST_REF // Size=0x4
{
    union // Size=0x4
    {
        void *Object;// Offset=0x0 Size=0x4
        unsigned long RefCnt:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long Value;// Offset=0x0 Size=0x4
    };
} EX_FAST_REF, *PEX_FAST_REF;

typedef struct _MMADDRESS_NODE {
    ULONG_PTR StartingVpn;
    ULONG_PTR EndingVpn;
    struct _MMADDRESS_NODE *Parent;
    struct _MMADDRESS_NODE *LeftChild;
    struct _MMADDRESS_NODE *RightChild;
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _KPROCESS// Size=0x6c
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY ProfileListHead;// Offset=0x10 Size=0x8
    unsigned long DirectoryTableBase[2];// Offset=0x18 Size=0x8
    struct _KGDTENTRY LdtDescriptor;// Offset=0x20 Size=0x8
    struct _KIDTENTRY Int21Descriptor;// Offset=0x28 Size=0x8
    unsigned short IopmOffset;// Offset=0x30 Size=0x2
    unsigned char Iopl;// Offset=0x32 Size=0x1
    unsigned char Unused;// Offset=0x33 Size=0x1
    unsigned long ActiveProcessors;// Offset=0x34 Size=0x4
    unsigned long KernelTime;// Offset=0x38 Size=0x4
    unsigned long UserTime;// Offset=0x3c Size=0x4
    struct _LIST_ENTRY ReadyListHead;// Offset=0x40 Size=0x8
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x48 Size=0x4
    void *VdmTrapcHandler;// Offset=0x4c Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x50 Size=0x8
    unsigned long ProcessLock;// Offset=0x58 Size=0x4
    unsigned long Affinity;// Offset=0x5c Size=0x4
    unsigned short StackCount;// Offset=0x60 Size=0x2
    char BasePriority;// Offset=0x62 Size=0x1
    char ThreadQuantum;// Offset=0x63 Size=0x1
    unsigned char AutoAlignment;// Offset=0x64 Size=0x1
    unsigned char State;// Offset=0x65 Size=0x1
    unsigned char ThreadSeed;// Offset=0x66 Size=0x1
    unsigned char DisableBoost;// Offset=0x67 Size=0x1
    unsigned char PowerState;// Offset=0x68 Size=0x1
    unsigned char DisableQuantum;// Offset=0x69 Size=0x1
    unsigned char IdealNode;// Offset=0x6a Size=0x1
    union // Size=0x1
    {
        struct _KEXECUTE_OPTIONS Flags;// Offset=0x6b Size=0x1
        unsigned char ExecuteOptions;// Offset=0x6b Size=0x1
    };
};

struct _MMSUPPORT// Size=0x40
{
    union _LARGE_INTEGER LastTrimTime;// Offset=0x0 Size=0x8
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x8 Size=0x4
    unsigned long PageFaultCount;// Offset=0xc Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x10 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x14 Size=0x4
    unsigned long MinimumWorkingSetSize;// Offset=0x18 Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x1c Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x20 Size=0x4
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x24 Size=0x8
    unsigned long Claim;// Offset=0x2c Size=0x4
    unsigned long NextEstimationSlot;// Offset=0x30 Size=0x4
    unsigned long NextAgingSlot;// Offset=0x34 Size=0x4
    unsigned long EstimatedAvailable;// Offset=0x38 Size=0x4
    unsigned long GrowthSinceLastEstimate;// Offset=0x3c Size=0x4
};

typedef struct _EPROCESS // Size=0x260
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0x6c
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0x6c Size=0x4
    union _LARGE_INTEGER CreateTime;// Offset=0x70 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0x78 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x80 Size=0x4
    void *UniqueProcessId;// Offset=0x84 Size=0x4
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0x88 Size=0x8
    unsigned long QuotaUsage[3];// Offset=0x90 Size=0xc
    unsigned long QuotaPeak[3];// Offset=0x9c Size=0xc
    unsigned long CommitCharge;// Offset=0xa8 Size=0x4
    unsigned long PeakVirtualSize;// Offset=0xac Size=0x4
    unsigned long VirtualSize;// Offset=0xb0 Size=0x4
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0xb4 Size=0x8
    void *DebugPort;// Offset=0xbc Size=0x4
    void *ExceptionPort;// Offset=0xc0 Size=0x4
    struct _HANDLE_TABLE *ObjectTable;// Offset=0xc4 Size=0x4
    struct _EX_FAST_REF Token;// Offset=0xc8 Size=0x4
    struct _FAST_MUTEX WorkingSetLock;// Offset=0xcc Size=0x20
    unsigned long WorkingSetPage;// Offset=0xec Size=0x4
    struct _FAST_MUTEX AddressCreationLock;// Offset=0xf0 Size=0x20
    unsigned long HyperSpaceLock;// Offset=0x110 Size=0x4
    struct _ETHREAD *ForkInProgress;// Offset=0x114 Size=0x4
    unsigned long HardwareTrigger;// Offset=0x118 Size=0x4
    void *VadRoot;// Offset=0x11c Size=0x4
    void *VadHint;// Offset=0x120 Size=0x4
    void *CloneRoot;// Offset=0x124 Size=0x4
    unsigned long NumberOfPrivatePages;// Offset=0x128 Size=0x4
    unsigned long NumberOfLockedPages;// Offset=0x12c Size=0x4
    void *Win32Process;// Offset=0x130 Size=0x4
    struct _EJOB *Job;// Offset=0x134 Size=0x4
    void *SectionObject;// Offset=0x138 Size=0x4
    void *SectionBaseAddress;// Offset=0x13c Size=0x4
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x140 Size=0x4
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x144 Size=0x4
    void *Win32WindowStation;// Offset=0x148 Size=0x4
    void *InheritedFromUniqueProcessId;// Offset=0x14c Size=0x4
    void *LdtInformation;// Offset=0x150 Size=0x4
    void *VadFreeHint;// Offset=0x154 Size=0x4
    void *VdmObjects;// Offset=0x158 Size=0x4
    void *DeviceMap;// Offset=0x15c Size=0x4
    struct _LIST_ENTRY PhysicalVadList;// Offset=0x160 Size=0x8
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x168 Size=0x4
        ULONG64 Filler;// Offset=0x168 Size=0x8
    };
    void *Session;// Offset=0x170 Size=0x4
    unsigned char ImageFileName[16];// Offset=0x174 Size=0x10
    struct _LIST_ENTRY JobLinks;// Offset=0x184 Size=0x8
    void *LockedPagesList;// Offset=0x18c Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x190 Size=0x8
    void *SecurityPort;// Offset=0x198 Size=0x4
    void *PaeTop;// Offset=0x19c Size=0x4
    unsigned long ActiveThreads;// Offset=0x1a0 Size=0x4
    unsigned long GrantedAccess;// Offset=0x1a4 Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x1a8 Size=0x4
    long LastThreadExitStatus;// Offset=0x1ac Size=0x4
    struct _PEB *Peb;// Offset=0x1b0 Size=0x4
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x1b4 Size=0x4
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x1b8 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x1c0 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x1c8 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x1d0 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x1d8 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x1e0 Size=0x8
    unsigned long CommitChargeLimit;// Offset=0x1e8 Size=0x4
    unsigned long CommitChargePeak;// Offset=0x1ec Size=0x4
    void *AweInfo;// Offset=0x1f0 Size=0x4
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x1f4 Size=0x4
    struct _MMSUPPORT Vm;// Offset=0x1f8 Size=0x40
    unsigned long LastFaultCount;// Offset=0x238 Size=0x4
    unsigned long ModifiedPageCount;// Offset=0x23c Size=0x4
    unsigned long NumberOfVads;// Offset=0x240 Size=0x4
    unsigned long JobStatus;// Offset=0x244 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x248 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x248 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x248 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x248 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x248 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x248 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x248 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x248 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x248 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x248 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long HasPhysicalVad:1;// Offset=0x248 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x248 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x248 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x248 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long SessionCreationUnderway:1;// Offset=0x248 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x248 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x248 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x248 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x248 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x248 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x248 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x248 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long Unused3:1;// Offset=0x248 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long Unused4:1;// Offset=0x248 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x248 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long Unused:5;// Offset=0x248 Size=0x4 BitOffset=0x19 BitSize=0x5
            unsigned long Unused1:1;// Offset=0x248 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long Unused2:1;// Offset=0x248 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x24c Size=0x4
    unsigned short NextPageColor;// Offset=0x250 Size=0x2
    union // Size=0x2
    {
        struct // Size=0x2
        {
            unsigned char SubSystemMinorVersion;// Offset=0x252 Size=0x1
            unsigned char SubSystemMajorVersion;// Offset=0x253 Size=0x1
        };
        unsigned short SubSystemVersion;// Offset=0x252 Size=0x2
    };
    unsigned char PriorityClass;// Offset=0x254 Size=0x1
    unsigned char WorkingSetAcquiredUnsafe;// Offset=0x255 Size=0x1
    unsigned char __align0[2];// Offset=0x256 Size=0x2
    unsigned long Cookie;// Offset=0x258 Size=0x4
} EPROCESS;

struct _CONTROL_AREA // Size=0x30
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY DereferenceList;// Offset=0x4 Size=0x8
    unsigned long NumberOfSectionReferences;// Offset=0xc Size=0x4
    unsigned long NumberOfPfnReferences;// Offset=0x10 Size=0x4
    unsigned long NumberOfMappedViews;// Offset=0x14 Size=0x4
    unsigned short NumberOfSubsections;// Offset=0x18 Size=0x2
    unsigned short FlushInProgressCount;// Offset=0x1a Size=0x2
    unsigned long NumberOfUserReferences;// Offset=0x1c Size=0x4
    union {// Offset=0x20 Size=0x4
        ULONG LongFlags;
        struct _MMSECTION_FLAGS Flags;
    } u;
    struct _FILE_OBJECT *FilePointer;// Offset=0x24 Size=0x4
    struct _EVENT_COUNTER *WaitingForDeletion;// Offset=0x28 Size=0x4
    unsigned short ModifiedWriteCount;// Offset=0x2c Size=0x2
    unsigned short NumberOfSystemCacheViews;// Offset=0x2e Size=0x2
};

typedef struct _SEGMENT // Size=0x40
{
    struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
    // ...
} *PSEGMENT;

typedef struct _SECTION {
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    ULONG InitialPageProtection;
} SECTION, *PSECTION;

typedef struct _KAPC_STATE // Size=0x18
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x10
    struct _KPROCESS *Process;// Offset=0x10 Size=0x4
    unsigned char KernelApcInProgress;// Offset=0x14 Size=0x1
    unsigned char KernelApcPending;// Offset=0x15 Size=0x1
    unsigned char UserApcPending;// Offset=0x16 Size=0x1
} KAPC_STATE, *PKAPC_STATE;

struct _KTHREAD // Size=0x1c0
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY MutantListHead;// Offset=0x10 Size=0x8
    void *InitialStack;// Offset=0x18 Size=0x4
    void *StackLimit;// Offset=0x1c Size=0x4
    void *Teb;// Offset=0x20 Size=0x4
    void *TlsArray;// Offset=0x24 Size=0x4
    void *KernelStack;// Offset=0x28 Size=0x4
    unsigned char DebugActive;// Offset=0x2c Size=0x1
    unsigned char State;// Offset=0x2d Size=0x1
    unsigned char Alerted[2];// Offset=0x2e Size=0x2
    unsigned char Iopl;// Offset=0x30 Size=0x1
    unsigned char NpxState;// Offset=0x31 Size=0x1
    char Saturation;// Offset=0x32 Size=0x1
    char Priority;// Offset=0x33 Size=0x1
    struct _KAPC_STATE ApcState;// Offset=0x34 Size=0x18
    unsigned long ContextSwitches;// Offset=0x4c Size=0x4
    unsigned char IdleSwapBlock;// Offset=0x50 Size=0x1
    unsigned char Spare0[3];// Offset=0x51 Size=0x3
    long WaitStatus;// Offset=0x54 Size=0x4
    unsigned char WaitIrql;// Offset=0x58 Size=0x1
    char WaitMode;// Offset=0x59 Size=0x1
    unsigned char WaitNext;// Offset=0x5a Size=0x1
    unsigned char WaitReason;// Offset=0x5b Size=0x1
    struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x5c Size=0x4
    union // Size=0x8
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x60 Size=0x8
        struct // Size=0x8
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x60 Size=0x4
            unsigned char __align0[4];// Offset=0x64 Size=0x4
        };
    };
    unsigned long WaitTime;// Offset=0x68 Size=0x4
    char BasePriority;// Offset=0x6c Size=0x1
    unsigned char DecrementCount;// Offset=0x6d Size=0x1
    char PriorityDecrement;// Offset=0x6e Size=0x1
    char Quantum;// Offset=0x6f Size=0x1
    struct _KWAIT_BLOCK WaitBlock[4];// Offset=0x70 Size=0x60
    void *LegoData;// Offset=0xd0 Size=0x4
    unsigned long KernelApcDisable;// Offset=0xd4 Size=0x4
    unsigned long UserAffinity;// Offset=0xd8 Size=0x4
    unsigned char SystemAffinityActive;// Offset=0xdc Size=0x1
    unsigned char PowerState;// Offset=0xdd Size=0x1
    unsigned char NpxIrql;// Offset=0xde Size=0x1
    unsigned char InitialNode;// Offset=0xdf Size=0x1
    void *ServiceTable;// Offset=0xe0 Size=0x4
    struct _KQUEUE *Queue;// Offset=0xe4 Size=0x4
    unsigned long ApcQueueLock;// Offset=0xe8 Size=0x4
    unsigned char __align1[4];// Offset=0xec Size=0x4
    struct _KTIMER Timer;// Offset=0xf0 Size=0x28
    struct _LIST_ENTRY QueueListEntry;// Offset=0x118 Size=0x8
    unsigned long SoftAffinity;// Offset=0x120 Size=0x4
    unsigned long Affinity;// Offset=0x124 Size=0x4
    unsigned char Preempted;// Offset=0x128 Size=0x1
    unsigned char ProcessReadyQueue;// Offset=0x129 Size=0x1
    unsigned char KernelStackResident;// Offset=0x12a Size=0x1
    unsigned char NextProcessor;// Offset=0x12b Size=0x1
    void *CallbackStack;// Offset=0x12c Size=0x4
    void *Win32Thread;// Offset=0x130 Size=0x4
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x134 Size=0x4
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x138 Size=0x8
    char PreviousMode;// Offset=0x140 Size=0x1
    unsigned char EnableStackSwap;// Offset=0x141 Size=0x1
    unsigned char LargeStack;// Offset=0x142 Size=0x1
    unsigned char ResourceIndex;// Offset=0x143 Size=0x1
    unsigned long KernelTime;// Offset=0x144 Size=0x4
    unsigned long UserTime;// Offset=0x148 Size=0x4
    struct _KAPC_STATE SavedApcState;// Offset=0x14c Size=0x18
    unsigned char Alertable;// Offset=0x164 Size=0x1
    unsigned char ApcStateIndex;// Offset=0x165 Size=0x1
    unsigned char ApcQueueable;// Offset=0x166 Size=0x1
    unsigned char AutoAlignment;// Offset=0x167 Size=0x1
    void *StackBase;// Offset=0x168 Size=0x4
    struct _KAPC SuspendApc;// Offset=0x16c Size=0x30
    struct _KSEMAPHORE SuspendSemaphore;// Offset=0x19c Size=0x14
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x1b0 Size=0x8
    char FreezeCount;// Offset=0x1b8 Size=0x1
    char SuspendCount;// Offset=0x1b9 Size=0x1
    unsigned char IdealProcessor;// Offset=0x1ba Size=0x1
    unsigned char DisableBoost;// Offset=0x1bb Size=0x1
};

typedef struct _ETHREAD // Size=0x258
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x1c0
    union // Size=0x8
    {
        union _LARGE_INTEGER CreateTime;// Offset=0x1c0 Size=0x8
        struct // Size=0x8
        {
            unsigned int NestedFaultCount:2;// Offset=0x1c0 Size=0x4 BitOffset=0x0 BitSize=0x2
            unsigned int ApcNeeded:1;// Offset=0x1c0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned char __align0[4];// Offset=0x1c4 Size=0x4
        };
    };
    union // Size=0x8
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x1c8 Size=0x8
        struct _LIST_ENTRY LpcReplyChain;// Offset=0x1c8 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x1c8 Size=0x8
    };
    union // Size=0x4
    {
        long ExitStatus;// Offset=0x1d0 Size=0x4
        void *OfsChain;// Offset=0x1d0 Size=0x4
    };
    struct _LIST_ENTRY PostBlockList;// Offset=0x1d4 Size=0x8
    union // Size=0x4
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x1dc Size=0x4
        struct _ETHREAD *ReaperLink;// Offset=0x1dc Size=0x4
        void *KeyedWaitValue;// Offset=0x1dc Size=0x4
    };
    unsigned long ActiveTimerListLock;// Offset=0x1e0 Size=0x4
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x1e4 Size=0x8
    struct _CLIENT_ID Cid;// Offset=0x1ec Size=0x8
    union // Size=0x14
    {
        struct _KSEMAPHORE LpcReplySemaphore;// Offset=0x1f4 Size=0x14
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x1f4 Size=0x14
    };
    union // Size=0x4
    {
        void *LpcReplyMessage;// Offset=0x208 Size=0x4
        void *LpcWaitingOnPort;// Offset=0x208 Size=0x4
    };
    struct _PS_IMPERSONATION_INFORMATION *ImpersonationInfo;// Offset=0x20c Size=0x4
    struct _LIST_ENTRY IrpList;// Offset=0x210 Size=0x8
    unsigned long TopLevelIrp;// Offset=0x218 Size=0x4
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x21c Size=0x4
    struct _EPROCESS *ThreadsProcess;// Offset=0x220 Size=0x4
    void *StartAddress;// Offset=0x224 Size=0x4
    union // Size=0x4
    {
        void *Win32StartAddress;// Offset=0x228 Size=0x4
        unsigned long LpcReceivedMessageId;// Offset=0x228 Size=0x4
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x22c Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x234 Size=0x4
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x238 Size=0x4
    unsigned long LpcReplyMessageId;// Offset=0x23c Size=0x4
    unsigned long ReadClusterSize;// Offset=0x240 Size=0x4
    unsigned long GrantedAccess;// Offset=0x244 Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x248 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x248 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long DeadThread:1;// Offset=0x248 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x248 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x248 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x248 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x248 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x248 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x248 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x248 Size=0x4 BitOffset=0x8 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x24c Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x24c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x24c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x24c Size=0x4 BitOffset=0x2 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x250 Size=0x4
        struct // Size=0x4
        {
            unsigned char LpcReceivedMsgIdValid:1;// Offset=0x250 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char LpcExitThreadCalled:1;// Offset=0x250 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char AddressSpaceOwner:1;// Offset=0x250 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char __align1[3];// Offset=0x251 Size=0x3
        };
    };
    unsigned char ForwardClusterOnly;// Offset=0x254 Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x255 Size=0x1
} ETHREAD;

struct _KSPECIAL_REGISTERS// Size=0x54
{
    unsigned long Cr0;// Offset=0x0 Size=0x4
    unsigned long Cr2;// Offset=0x4 Size=0x4
    unsigned long Cr3;// Offset=0x8 Size=0x4
    unsigned long Cr4;// Offset=0xc Size=0x4
    unsigned long KernelDr0;// Offset=0x10 Size=0x4
    unsigned long KernelDr1;// Offset=0x14 Size=0x4
    unsigned long KernelDr2;// Offset=0x18 Size=0x4
    unsigned long KernelDr3;// Offset=0x1c Size=0x4
    unsigned long KernelDr6;// Offset=0x20 Size=0x4
    unsigned long KernelDr7;// Offset=0x24 Size=0x4
    struct _DESCRIPTOR Gdtr;// Offset=0x28 Size=0x8
    struct _DESCRIPTOR Idtr;// Offset=0x30 Size=0x8
    unsigned short Tr;// Offset=0x38 Size=0x2
    unsigned short Ldtr;// Offset=0x3a Size=0x2
    unsigned long Reserved[6];// Offset=0x3c Size=0x18
};

struct _KPROCESSOR_STATE// Size=0x320
{
    struct _CONTEXT ContextFrame;// Offset=0x0 Size=0x2cc
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x2cc Size=0x54
};

struct _KDPC_XP// Size=0x20
{
    short Type;// Offset=0x0 Size=0x2
    unsigned char Number;// Offset=0x2 Size=0x1
    unsigned char Importance;// Offset=0x3 Size=0x1
    struct _LIST_ENTRY DpcListEntry;// Offset=0x4 Size=0x8
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0xc Size=0x4
    void *DeferredContext;// Offset=0x10 Size=0x4
    void *SystemArgument1;// Offset=0x14 Size=0x4
    void *SystemArgument2;// Offset=0x18 Size=0x4
    unsigned long *Lock;// Offset=0x1c Size=0x4
};

struct _PROCESSOR_POWER_STATE// Size=0x120
{
    void  ( *IdleFunction)(struct _PROCESSOR_POWER_STATE *);// Offset=0x0 Size=0x4
    unsigned long Idle0KernelTimeLimit;// Offset=0x4 Size=0x4
    unsigned long Idle0LastTime;// Offset=0x8 Size=0x4
    void          *IdleHandlers;// Offset=0xc Size=0x4
    void          *IdleState;// Offset=0x10 Size=0x4
    unsigned long IdleHandlersCount;// Offset=0x14 Size=0x4
    ULONG64     LastCheck;// Offset=0x18 Size=0x8
    struct PROCESSOR_IDLE_TIMES IdleTimes;// Offset=0x20 Size=0x20
    unsigned long IdleTime1;// Offset=0x40 Size=0x4
    unsigned long PromotionCheck;// Offset=0x44 Size=0x4
    unsigned long IdleTime2;// Offset=0x48 Size=0x4
    unsigned char CurrentThrottle;// Offset=0x4c Size=0x1
    unsigned char ThermalThrottleLimit;// Offset=0x4d Size=0x1
    unsigned char CurrentThrottleIndex;// Offset=0x4e Size=0x1
    unsigned char ThermalThrottleIndex;// Offset=0x4f Size=0x1
    unsigned long LastKernelUserTime;// Offset=0x50 Size=0x4
    unsigned long LastIdleThreadKernelTime;// Offset=0x54 Size=0x4
    unsigned long PackageIdleStartTime;// Offset=0x58 Size=0x4
    unsigned long PackageIdleTime;// Offset=0x5c Size=0x4
    unsigned long DebugCount;// Offset=0x60 Size=0x4
    unsigned long LastSysTime;// Offset=0x64 Size=0x4
    ULONG64     TotalIdleStateTime[3];// Offset=0x68 Size=0x18
    unsigned long TotalIdleTransitions[3];// Offset=0x80 Size=0xc
    unsigned char __align0[4];// Offset=0x8c Size=0x4
    ULONG64     PreviousC3StateTime;// Offset=0x90 Size=0x8
    unsigned char KneeThrottleIndex;// Offset=0x98 Size=0x1
    unsigned char ThrottleLimitIndex;// Offset=0x99 Size=0x1
    unsigned char PerfStatesCount;// Offset=0x9a Size=0x1
    unsigned char ProcessorMinThrottle;// Offset=0x9b Size=0x1
    unsigned char ProcessorMaxThrottle;// Offset=0x9c Size=0x1
    unsigned char EnableIdleAccounting;// Offset=0x9d Size=0x1
    unsigned char LastC3Percentage;// Offset=0x9e Size=0x1
    unsigned char LastAdjustedBusyPercentage;// Offset=0x9f Size=0x1
    unsigned long PromotionCount;// Offset=0xa0 Size=0x4
    unsigned long DemotionCount;// Offset=0xa4 Size=0x4
    unsigned long ErrorCount;// Offset=0xa8 Size=0x4
    unsigned long RetryCount;// Offset=0xac Size=0x4
    unsigned long Flags;// Offset=0xb0 Size=0x4
    unsigned char __align1[4];// Offset=0xb4 Size=0x4
    LARGE_INTEGER PerfCounterFrequency;// Offset=0xb8 Size=0x8
    unsigned long PerfTickCount;// Offset=0xc0 Size=0x4
    unsigned char __align2[4];// Offset=0xc4 Size=0x4
    struct _KTIMER PerfTimer;// Offset=0xc8 Size=0x28
    struct _KDPC_XP PerfDpc;// Offset=0xf0 Size=0x20
    struct PROCESSOR_PERF_STATE *PerfStates;// Offset=0x110 Size=0x4
    long  ( *PerfSetThrottle)(unsigned char );// Offset=0x114 Size=0x4
    unsigned long LastC3KernelUserTime;// Offset=0x118 Size=0x4
    unsigned long LastPackageIdleTime;// Offset=0x11c Size=0x4
};

typedef struct _KPRCB// Size=0xc50
{
    unsigned short MinorVersion;// Offset=0x0 Size=0x2
    unsigned short MajorVersion;// Offset=0x2 Size=0x2
    struct _KTHREAD *CurrentThread;// Offset=0x4 Size=0x4
    struct _KTHREAD *NextThread;// Offset=0x8 Size=0x4
    struct _KTHREAD *IdleThread;// Offset=0xc Size=0x4
    char Number;// Offset=0x10 Size=0x1
    char Reserved;// Offset=0x11 Size=0x1
    unsigned short BuildType;// Offset=0x12 Size=0x2
    unsigned long SetMember;// Offset=0x14 Size=0x4
    char CpuType;// Offset=0x18 Size=0x1
    char CpuID;// Offset=0x19 Size=0x1
    unsigned short CpuStep;// Offset=0x1a Size=0x2
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x1c Size=0x320
    unsigned long KernelReserved[16];// Offset=0x33c Size=0x40
    unsigned long HalReserved[16];// Offset=0x37c Size=0x40
    unsigned char PrcbPad0[92];// Offset=0x3bc Size=0x5c
    struct _KSPIN_LOCK_QUEUE LockQueue[16];// Offset=0x418 Size=0x80
    unsigned char PrcbPad1[8];// Offset=0x498 Size=0x8
    struct _KTHREAD *NpxThread;// Offset=0x4a0 Size=0x4
    unsigned long InterruptCount;// Offset=0x4a4 Size=0x4
    unsigned long KernelTime;// Offset=0x4a8 Size=0x4
    unsigned long UserTime;// Offset=0x4ac Size=0x4
    unsigned long DpcTime;// Offset=0x4b0 Size=0x4
    unsigned long DebugDpcTime;// Offset=0x4b4 Size=0x4
    unsigned long InterruptTime;// Offset=0x4b8 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x4bc Size=0x4
    unsigned long PageColor;// Offset=0x4c0 Size=0x4
    unsigned long SkipTick;// Offset=0x4c4 Size=0x4
    unsigned char MultiThreadSetBusy;// Offset=0x4c8 Size=0x1
    unsigned char Spare2[3];// Offset=0x4c9 Size=0x3
    struct _KNODE *ParentNode;// Offset=0x4cc Size=0x4
    unsigned long MultiThreadProcessorSet;// Offset=0x4d0 Size=0x4
    struct _KPRCB *MultiThreadSetMaster;// Offset=0x4d4 Size=0x4
    unsigned long ThreadStartCount[2];// Offset=0x4d8 Size=0x8
    unsigned long CcFastReadNoWait;// Offset=0x4e0 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x4e4 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x4e8 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x4ec Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x4f0 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x4f4 Size=0x4
    unsigned long KeAlignmentFixupCount;// Offset=0x4f8 Size=0x4
    unsigned long KeContextSwitches;// Offset=0x4fc Size=0x4
    unsigned long KeDcacheFlushCount;// Offset=0x500 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x504 Size=0x4
    unsigned long KeFirstLevelTbFills;// Offset=0x508 Size=0x4
    unsigned long KeFloatingEmulationCount;// Offset=0x50c Size=0x4
    unsigned long KeIcacheFlushCount;// Offset=0x510 Size=0x4
    unsigned long KeSecondLevelTbFills;// Offset=0x514 Size=0x4
    unsigned long KeSystemCalls;// Offset=0x518 Size=0x4
    unsigned long SpareCounter0[1];// Offset=0x51c Size=0x4
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x520 Size=0x80
    struct _PP_LOOKASIDE_LIST PPNPagedLookasideList[32];// Offset=0x5a0 Size=0x100
    struct _PP_LOOKASIDE_LIST PPPagedLookasideList[32];// Offset=0x6a0 Size=0x100
    unsigned long volatile PacketBarrier;// Offset=0x7a0 Size=0x4
    unsigned long volatile ReverseStall;// Offset=0x7a4 Size=0x4
    void *IpiFrame;// Offset=0x7a8 Size=0x4
    unsigned char PrcbPad2[52];// Offset=0x7ac Size=0x34
    void volatile *CurrentPacket[3];// Offset=0x7e0 Size=0xc
    unsigned long volatile TargetSet;// Offset=0x7ec Size=0x4
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x7f0 Size=0x4
    unsigned long volatile IpiFrozen;// Offset=0x7f4 Size=0x4
    unsigned char PrcbPad3[40];// Offset=0x7f8 Size=0x28
    unsigned long volatile RequestSummary;// Offset=0x820 Size=0x4
    struct _KPRCB volatile *SignalDone;// Offset=0x824 Size=0x4
    unsigned char PrcbPad4[56];// Offset=0x828 Size=0x38
    struct _LIST_ENTRY DpcListHead;// Offset=0x860 Size=0x8
    void *DpcStack;// Offset=0x868 Size=0x4
    unsigned long DpcCount;// Offset=0x86c Size=0x4
    unsigned long volatile DpcQueueDepth;// Offset=0x870 Size=0x4
    unsigned long volatile DpcRoutineActive;// Offset=0x874 Size=0x4
    unsigned long volatile DpcInterruptRequested;// Offset=0x878 Size=0x4
    unsigned long DpcLastCount;// Offset=0x87c Size=0x4
    unsigned long DpcRequestRate;// Offset=0x880 Size=0x4
    unsigned long MaximumDpcQueueDepth;// Offset=0x884 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x888 Size=0x4
    unsigned long QuantumEnd;// Offset=0x88c Size=0x4
    unsigned char PrcbPad5[16];// Offset=0x890 Size=0x10
    unsigned long DpcLock;// Offset=0x8a0 Size=0x4
    unsigned char PrcbPad6[28];// Offset=0x8a4 Size=0x1c
    struct _KDPC_XP CallDpc;// Offset=0x8c0 Size=0x20
    void *ChainedInterruptList;// Offset=0x8e0 Size=0x4
    long LookasideIrpFloat;// Offset=0x8e4 Size=0x4
    unsigned long SpareFields0[6];// Offset=0x8e8 Size=0x18
    unsigned char VendorString[13];// Offset=0x900 Size=0xd
    unsigned char InitialApicId;// Offset=0x90d Size=0x1
    unsigned char LogicalProcessorsPerPhysicalProcessor;// Offset=0x90e Size=0x1
    unsigned char __align0[1];// Offset=0x90f Size=0x1
    unsigned long MHz;// Offset=0x910 Size=0x4
    unsigned long FeatureBits;// Offset=0x914 Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x918 Size=0x8
    struct _FX_SAVE_AREA NpxSaveArea;// Offset=0x920 Size=0x210
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0xb30 Size=0x120
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0xa4
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x94
    struct _EX_PUSH_LOCK Lock;// Offset=0x94 Size=0x4
    struct _DEVICE_MAP *DeviceMap;// Offset=0x98 Size=0x4
    unsigned long SessionId;// Offset=0x9c Size=0x4
    unsigned short Reserved;// Offset=0xa0 Size=0x2
    unsigned short SymbolicLinkUsageCount;// Offset=0xa2 Size=0x2
} *POBJECT_DIRECTORY;
    
#endif //                  Windows XP x32



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows 2003 x32 internal kernel definitions                      //
#if (NTDDI_VERSION >= NTDDI_WS03) && (NTDDI_VERSION <= NTDDI_WS03SP4) && defined(_X86_)

typedef struct _MM_SESSION_SPACE// Size=0x1ec0
{
    struct _MM_SESSION_SPACE *GlobalVirtualAddress;// Offset=0x0 Size=0x4
    long ReferenceCount;// Offset=0x4 Size=0x4
    union {
        ULONG LongFlags;
        MM_SESSION_SPACE_FLAGS Flags;
    } u;
    ULONG SessionId;    // Offset=0xc Size=0x4
// ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x4
{
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long Locked:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long Waiting:1;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long Waking:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long MultipleShared:1;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Shared:28;// Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1c
        };
        unsigned long Value;// Offset=0x0 Size=0x4
        void *Ptr;// Offset=0x0 Size=0x4
    };
};

typedef struct _EX_FAST_REF // Size=0x4
{
    union // Size=0x4
    {
        void *Object;// Offset=0x0 Size=0x4
        unsigned long RefCnt:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long Value;// Offset=0x0 Size=0x4
    };
} EX_FAST_REF, *PEX_FAST_REF;

typedef struct _MMADDRESS_NODE // Size=0x14
{
    union { // Offset=0x0 Size=0x4
        long Balance:2;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x4
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x4 Size=0x4
    struct _MMADDRESS_NODE *RightChild;// Offset=0x8 Size=0x4
    unsigned long StartingVpn;// Offset=0xc Size=0x4
    unsigned long EndingVpn;// Offset=0x10 Size=0x4
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE // Size=0x20
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x14
    unsigned long DepthOfTree:5;// Offset=0x14 Size=0x4 BitOffset=0x0 BitSize=0x5
    unsigned long Unused:3;// Offset=0x14 Size=0x4 BitOffset=0x5 BitSize=0x3
    unsigned long NumberGenericTableElements:24;// Offset=0x14 Size=0x4 BitOffset=0x8 BitSize=0x18
    void *NodeHint;// Offset=0x18 Size=0x4
    void *NodeFreeHint;// Offset=0x1c Size=0x4
};

struct _KPROCESS// Size=0x78
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY ProfileListHead;// Offset=0x10 Size=0x8
    unsigned long DirectoryTableBase[2];// Offset=0x18 Size=0x8
    struct _KGDTENTRY LdtDescriptor;// Offset=0x20 Size=0x8
    struct _KIDTENTRY Int21Descriptor;// Offset=0x28 Size=0x8
    unsigned short IopmOffset;// Offset=0x30 Size=0x2
    unsigned char Iopl;// Offset=0x32 Size=0x1
    unsigned char Unused;// Offset=0x33 Size=0x1
    unsigned long volatile ActiveProcessors;// Offset=0x34 Size=0x4
    unsigned long KernelTime;// Offset=0x38 Size=0x4
    unsigned long UserTime;// Offset=0x3c Size=0x4
    struct _LIST_ENTRY ReadyListHead;// Offset=0x40 Size=0x8
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x48 Size=0x4
    void *VdmTrapcHandler;// Offset=0x4c Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x50 Size=0x8
    unsigned long ProcessLock;// Offset=0x58 Size=0x4
    unsigned long Affinity;// Offset=0x5c Size=0x4
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long AutoAlignment:1;// Offset=0x60 Size=0x4 BitOffset=0x0 BitSize=0x1
            long DisableBoost:1;// Offset=0x60 Size=0x4 BitOffset=0x1 BitSize=0x1
            long DisableQuantum:1;// Offset=0x60 Size=0x4 BitOffset=0x2 BitSize=0x1
            long ReservedFlags:29;// Offset=0x60 Size=0x4 BitOffset=0x3 BitSize=0x1d
        };
        long ProcessFlags;// Offset=0x60 Size=0x4
    };
    char BasePriority;// Offset=0x64 Size=0x1
    char QuantumReset;// Offset=0x65 Size=0x1
    unsigned char State;// Offset=0x66 Size=0x1
    unsigned char ThreadSeed;// Offset=0x67 Size=0x1
    unsigned char PowerState;// Offset=0x68 Size=0x1
    unsigned char IdealNode;// Offset=0x69 Size=0x1
    unsigned char Visited;// Offset=0x6a Size=0x1
    union // Size=0x1
    {
        struct _KEXECUTE_OPTIONS Flags;// Offset=0x6b Size=0x1
        unsigned char ExecuteOptions;// Offset=0x6b Size=0x1
    };
    unsigned long StackCount;// Offset=0x6c Size=0x4
    struct _LIST_ENTRY ProcessListEntry;// Offset=0x70 Size=0x8
};

struct _MMSUPPORT// Size=0x48
{
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x0 Size=0x8
    union _LARGE_INTEGER LastTrimTime;// Offset=0x8 Size=0x8
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x10 Size=0x4
    unsigned long PageFaultCount;// Offset=0x14 Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x18 Size=0x4
    unsigned long GrowthSinceLastEstimate;// Offset=0x1c Size=0x4
    unsigned long MinimumWorkingSetSize;// Offset=0x20 Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x24 Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x28 Size=0x4
    unsigned long Claim;// Offset=0x2c Size=0x4
    unsigned long NextEstimationSlot;// Offset=0x30 Size=0x4
    unsigned long NextAgingSlot;// Offset=0x34 Size=0x4
    unsigned long EstimatedAvailable;// Offset=0x38 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x3c Size=0x4
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x40 Size=0x4
};

typedef struct _EPROCESS// Size=0x278
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0x78
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0x78 Size=0x4
    unsigned char __align0[4];// Offset=0x7c Size=0x4
    union _LARGE_INTEGER CreateTime;// Offset=0x80 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0x88 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x90 Size=0x4
    void *UniqueProcessId;// Offset=0x94 Size=0x4
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0x98 Size=0x8
    unsigned long QuotaUsage[3];// Offset=0xa0 Size=0xc
    unsigned long QuotaPeak[3];// Offset=0xac Size=0xc
    unsigned long CommitCharge;// Offset=0xb8 Size=0x4
    unsigned long PeakVirtualSize;// Offset=0xbc Size=0x4
    unsigned long VirtualSize;// Offset=0xc0 Size=0x4
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0xc4 Size=0x8
    void *DebugPort;// Offset=0xcc Size=0x4
    void *ExceptionPort;// Offset=0xd0 Size=0x4
    struct _HANDLE_TABLE *ObjectTable;// Offset=0xd4 Size=0x4
    struct _EX_FAST_REF Token;// Offset=0xd8 Size=0x4
    unsigned long WorkingSetPage;// Offset=0xdc Size=0x4
    struct _KGUARDED_MUTEX AddressCreationLock;// Offset=0xe0 Size=0x20
    unsigned long HyperSpaceLock;// Offset=0x100 Size=0x4
    struct _ETHREAD *ForkInProgress;// Offset=0x104 Size=0x4
    unsigned long HardwareTrigger;// Offset=0x108 Size=0x4
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0x10c Size=0x4
    void *CloneRoot;// Offset=0x110 Size=0x4
    unsigned long NumberOfPrivatePages;// Offset=0x114 Size=0x4
    unsigned long NumberOfLockedPages;// Offset=0x118 Size=0x4
    void *Win32Process;// Offset=0x11c Size=0x4
    struct _EJOB *Job;// Offset=0x120 Size=0x4
    void *SectionObject;// Offset=0x124 Size=0x4
    void *SectionBaseAddress;// Offset=0x128 Size=0x4
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x12c Size=0x4
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x130 Size=0x4
    void *Win32WindowStation;// Offset=0x134 Size=0x4
    void *InheritedFromUniqueProcessId;// Offset=0x138 Size=0x4
    void *LdtInformation;// Offset=0x13c Size=0x4
    void *VadFreeHint;// Offset=0x140 Size=0x4
    void *VdmObjects;// Offset=0x144 Size=0x4
    void *DeviceMap;// Offset=0x148 Size=0x4
    void *Spare0[3];// Offset=0x14c Size=0xc
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x158 Size=0x8
        ULONG64 Filler;// Offset=0x158 Size=0x8
    };
    void *Session;// Offset=0x160 Size=0x4
    unsigned char ImageFileName[16];// Offset=0x164 Size=0x10
    struct _LIST_ENTRY JobLinks;// Offset=0x174 Size=0x8
    void *LockedPagesList;// Offset=0x17c Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x180 Size=0x8
    void *SecurityPort;// Offset=0x188 Size=0x4
    void *PaeTop;// Offset=0x18c Size=0x4
    unsigned long ActiveThreads;// Offset=0x190 Size=0x4
    unsigned long GrantedAccess;// Offset=0x194 Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x198 Size=0x4
    long LastThreadExitStatus;// Offset=0x19c Size=0x4
    struct _PEB *Peb;// Offset=0x1a0 Size=0x4
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x1a4 Size=0x4
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x1a8 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x1b0 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x1b8 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x1c0 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x1c8 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x1d0 Size=0x8
    unsigned long CommitChargeLimit;// Offset=0x1d8 Size=0x4
    unsigned long CommitChargePeak;// Offset=0x1dc Size=0x4
    void *AweInfo;// Offset=0x1e0 Size=0x4
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x1e4 Size=0x4
    struct _MMSUPPORT Vm;// Offset=0x1e8 Size=0x48
    struct _LIST_ENTRY MmProcessLinks;// Offset=0x230 Size=0x8
    unsigned long ModifiedPageCount;// Offset=0x238 Size=0x4
    unsigned long JobStatus;// Offset=0x23c Size=0x4
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x240 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x240 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x240 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x240 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x240 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x240 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x240 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x240 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x240 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x240 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x240 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x240 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x240 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x240 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long SessionCreationUnderway:1;// Offset=0x240 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x240 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x240 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x240 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x240 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x240 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x240 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x240 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x240 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x240 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x240 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long SmapAllowed:1;// Offset=0x240 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long CreateFailed:1;// Offset=0x240 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x240 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long Spare1:1;// Offset=0x240 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long Spare2:1;// Offset=0x240 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x244 Size=0x4
    unsigned short NextPageColor;// Offset=0x248 Size=0x2
    union // Size=0x2
    {
        struct // Size=0x2
        {
            unsigned char SubSystemMinorVersion;// Offset=0x24a Size=0x1
            unsigned char SubSystemMajorVersion;// Offset=0x24b Size=0x1
        };
        unsigned short SubSystemVersion;// Offset=0x24a Size=0x2
    };
    unsigned char PriorityClass;// Offset=0x24c Size=0x1
    unsigned char __align1[3];// Offset=0x24d Size=0x3
    struct _MM_AVL_TABLE VadRoot;// Offset=0x250 Size=0x20
    unsigned long Cookie;// Offset=0x270 Size=0x4
} EPROCESS;

struct _CONTROL_AREA // Size=0x38
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY DereferenceList;// Offset=0x4 Size=0x8
    unsigned long NumberOfSectionReferences;// Offset=0xc Size=0x4
    unsigned long NumberOfPfnReferences;// Offset=0x10 Size=0x4
    unsigned long NumberOfMappedViews;// Offset=0x14 Size=0x4
    unsigned long NumberOfSystemCacheViews;// Offset=0x18 Size=0x4
    unsigned long NumberOfUserReferences;// Offset=0x1c Size=0x4
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    struct _FILE_OBJECT *FilePointer;// Offset=0x24 Size=0x4
    struct _EVENT_COUNTER *WaitingForDeletion;// Offset=0x28 Size=0x4
    unsigned short ModifiedWriteCount;// Offset=0x2c Size=0x2
    unsigned short FlushInProgressCount;// Offset=0x2e Size=0x2
    unsigned long WritableUserReferences;// Offset=0x30 Size=0x4
    unsigned long QuadwordPad;// Offset=0x34 Size=0x4
};

typedef struct _SEGMENT // Size=0x40/0x48 NOPAE/PAE
{
    struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
    // ...
} *PSEGMENT;

typedef struct _SECTION {
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

typedef struct _KAPC_STATE// Size=0x18
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x10
    struct _KPROCESS *Process;// Offset=0x10 Size=0x4
    unsigned char KernelApcInProgress;// Offset=0x14 Size=0x1
    unsigned char KernelApcPending;// Offset=0x15 Size=0x1
    unsigned char UserApcPending;// Offset=0x16 Size=0x1
} KAPC_STATE, *PKAPC_STATE;

struct _KTHREAD// Size=0x1b8
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY MutantListHead;// Offset=0x10 Size=0x8
    void *InitialStack;// Offset=0x18 Size=0x4
    void *StackLimit;// Offset=0x1c Size=0x4
    void *KernelStack;// Offset=0x20 Size=0x4
    unsigned long ThreadLock;// Offset=0x24 Size=0x4
    union // Size=0x18
    {
        struct _KAPC_STATE ApcState;// Offset=0x28 Size=0x18
        struct // Size=0x18
        {
            unsigned char ApcStateFill[23];// Offset=0x28 Size=0x17
            unsigned char ApcQueueable;// Offset=0x3f Size=0x1
        };
    };
    unsigned char volatile NextProcessor;// Offset=0x40 Size=0x1
    unsigned char volatile DeferredProcessor;// Offset=0x41 Size=0x1
    unsigned char AdjustReason;// Offset=0x42 Size=0x1
    char AdjustIncrement;// Offset=0x43 Size=0x1
    unsigned long ApcQueueLock;// Offset=0x44 Size=0x4
    unsigned long ContextSwitches;// Offset=0x48 Size=0x4
    unsigned char volatile State;// Offset=0x4c Size=0x1
    unsigned char NpxState;// Offset=0x4d Size=0x1
    unsigned char WaitIrql;// Offset=0x4e Size=0x1
    char WaitMode;// Offset=0x4f Size=0x1
    long WaitStatus;// Offset=0x50 Size=0x4
    union // Size=0x4
    {
        struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x54 Size=0x4
        struct _KGATE *GateObject;// Offset=0x54 Size=0x4
    };
    unsigned char Alertable;// Offset=0x58 Size=0x1
    unsigned char WaitNext;// Offset=0x59 Size=0x1
    unsigned char WaitReason;// Offset=0x5a Size=0x1
    char Priority;// Offset=0x5b Size=0x1
    unsigned char EnableStackSwap;// Offset=0x5c Size=0x1
    unsigned char volatile SwapBusy;// Offset=0x5d Size=0x1
    unsigned char Alerted[2];// Offset=0x5e Size=0x2
    union // Size=0x8
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x60 Size=0x8
        struct // Size=0x8
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x60 Size=0x4
            unsigned char __align0[4];// Offset=0x64 Size=0x4
        };
    };
    struct _KQUEUE *Queue;// Offset=0x68 Size=0x4
    unsigned long WaitTime;// Offset=0x6c Size=0x4
    union // Size=0x4
    {
        struct // Size=0x4
        {
            short KernelApcDisable;// Offset=0x70 Size=0x2
            short SpecialApcDisable;// Offset=0x72 Size=0x2
        };
        unsigned long CombinedApcDisable;// Offset=0x70 Size=0x4
    };
    void *Teb;// Offset=0x74 Size=0x4
    union // Size=0x28
    {
        struct _KTIMER Timer;// Offset=0x78 Size=0x28
        unsigned char TimerFill[40];// Offset=0x78 Size=0x28
    };
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long AutoAlignment:1;// Offset=0xa0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long DisableBoost:1;// Offset=0xa0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long GuiThread:1;// Offset=0xa0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ReservedFlags:29;// Offset=0xa0 Size=0x4 BitOffset=0x3 BitSize=0x1d
        };
        long ThreadFlags;// Offset=0xa0 Size=0x4
    };
    unsigned char __align1[4];// Offset=0xa4 Size=0x4
    union // Size=0x60
    {
        struct _KWAIT_BLOCK WaitBlock[4];// Offset=0xa8 Size=0x60
        struct // Size=0x18
        {
            unsigned char WaitBlockFill0[23];// Offset=0xa8 Size=0x17
            unsigned char SystemAffinityActive;// Offset=0xbf Size=0x1
        };
        struct // Size=0x30
        {
            unsigned char WaitBlockFill1[47];// Offset=0xa8 Size=0x2f
            char PreviousMode;// Offset=0xd7 Size=0x1
        };
        struct // Size=0x48
        {
            unsigned char WaitBlockFill2[71];// Offset=0xa8 Size=0x47
            unsigned char ResourceIndex;// Offset=0xef Size=0x1
        };
        struct // Size=0x60
        {
            unsigned char WaitBlockFill3[95];// Offset=0xa8 Size=0x5f
            unsigned char LargeStack;// Offset=0x107 Size=0x1
        };
    };
    struct _LIST_ENTRY QueueListEntry;// Offset=0x108 Size=0x8
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x110 Size=0x4
    void *CallbackStack;// Offset=0x114 Size=0x4
    void *ServiceTable;// Offset=0x118 Size=0x4
    unsigned char ApcStateIndex;// Offset=0x11c Size=0x1
    unsigned char IdealProcessor;// Offset=0x11d Size=0x1
    unsigned char Preempted;// Offset=0x11e Size=0x1
    unsigned char ProcessReadyQueue;// Offset=0x11f Size=0x1
    unsigned char KernelStackResident;// Offset=0x120 Size=0x1
    char BasePriority;// Offset=0x121 Size=0x1
    char PriorityDecrement;// Offset=0x122 Size=0x1
    char Saturation;// Offset=0x123 Size=0x1
    unsigned long UserAffinity;// Offset=0x124 Size=0x4
    struct _KPROCESS *Process;// Offset=0x128 Size=0x4
    unsigned long Affinity;// Offset=0x12c Size=0x4
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x130 Size=0x8
    union // Size=0x18
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x138 Size=0x18
        struct // Size=0x18
        {
            unsigned char SavedApcStateFill[23];// Offset=0x138 Size=0x17
            char FreezeCount;// Offset=0x14f Size=0x1
        };
    };
    char SuspendCount;// Offset=0x150 Size=0x1
    unsigned char UserIdealProcessor;// Offset=0x151 Size=0x1
    unsigned char CalloutActive;// Offset=0x152 Size=0x1
    unsigned char Iopl;// Offset=0x153 Size=0x1
    void *Win32Thread;// Offset=0x154 Size=0x4
    void *StackBase;// Offset=0x158 Size=0x4
    union // Size=0x30
    {
        struct _KAPC SuspendApc;// Offset=0x15c Size=0x30
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x15c Size=0x1
            char Quantum;// Offset=0x15d Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x15c Size=0x3
            unsigned char QuantumReset;// Offset=0x15f Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x15c Size=0x4
            unsigned long KernelTime;// Offset=0x160 Size=0x4
        };
        struct // Size=0x28
        {
            unsigned char SuspendApcFill3[36];// Offset=0x15c Size=0x24
            void *TlsArray;// Offset=0x180 Size=0x4
        };
        struct // Size=0x2c
        {
            unsigned char SuspendApcFill4[40];// Offset=0x15c Size=0x28
            void *LegoData;// Offset=0x184 Size=0x4
        };
        struct // Size=0x30
        {
            unsigned char SuspendApcFill5[47];// Offset=0x15c Size=0x2f
            unsigned char PowerState;// Offset=0x18b Size=0x1
        };
    };
    unsigned long UserTime;// Offset=0x18c Size=0x4
    union // Size=0x14
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x190 Size=0x14
        unsigned char SuspendSemaphorefill[20];// Offset=0x190 Size=0x14
    };
    unsigned long SListFaultCount;// Offset=0x1a4 Size=0x4
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x1a8 Size=0x8
    void *SListFaultAddress;// Offset=0x1b0 Size=0x4
};

typedef struct _ETHREAD// Size=0x250
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x1b8
    union _LARGE_INTEGER CreateTime;// Offset=0x1b8 Size=0x8
    union // Size=0x8
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x1c0 Size=0x8
        struct _LIST_ENTRY LpcReplyChain;// Offset=0x1c0 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x1c0 Size=0x8
    };
    union // Size=0x4
    {
        long ExitStatus;// Offset=0x1c8 Size=0x4
        void *OfsChain;// Offset=0x1c8 Size=0x4
    };
    struct _LIST_ENTRY PostBlockList;// Offset=0x1cc Size=0x8
    union // Size=0x4
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x1d4 Size=0x4
        struct _ETHREAD *ReaperLink;// Offset=0x1d4 Size=0x4
        void *KeyedWaitValue;// Offset=0x1d4 Size=0x4
    };
    unsigned long ActiveTimerListLock;// Offset=0x1d8 Size=0x4
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x1dc Size=0x8
    struct _CLIENT_ID Cid;// Offset=0x1e4 Size=0x8
    union // Size=0x14
    {
        struct _KSEMAPHORE LpcReplySemaphore;// Offset=0x1ec Size=0x14
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x1ec Size=0x14
    };
    union // Size=0x4
    {
        void *LpcReplyMessage;// Offset=0x200 Size=0x4
        void *LpcWaitingOnPort;// Offset=0x200 Size=0x4
    };
    struct _PS_IMPERSONATION_INFORMATION *ImpersonationInfo;// Offset=0x204 Size=0x4
    struct _LIST_ENTRY IrpList;// Offset=0x208 Size=0x8
    unsigned long TopLevelIrp;// Offset=0x210 Size=0x4
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x214 Size=0x4
    struct _EPROCESS *ThreadsProcess;// Offset=0x218 Size=0x4
    void *StartAddress;// Offset=0x21c Size=0x4
    union // Size=0x4
    {
        void *Win32StartAddress;// Offset=0x220 Size=0x4
        unsigned long LpcReceivedMessageId;// Offset=0x220 Size=0x4
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x224 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x22c Size=0x4
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x230 Size=0x4
    unsigned long LpcReplyMessageId;// Offset=0x234 Size=0x4
    unsigned long ReadClusterSize;// Offset=0x238 Size=0x4
    unsigned long GrantedAccess;// Offset=0x23c Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x240 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x240 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long DeadThread:1;// Offset=0x240 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x240 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x240 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x240 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x240 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x240 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x240 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x240 Size=0x4 BitOffset=0x8 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x244 Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x244 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x244 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x244 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x244 Size=0x4 BitOffset=0x3 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x248 Size=0x4
        struct // Size=0x4
        {
            unsigned char LpcReceivedMsgIdValid:1;// Offset=0x248 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char LpcExitThreadCalled:1;// Offset=0x248 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char AddressSpaceOwner:1;// Offset=0x248 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x248 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x248 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemWorkingSetExclusive:1;// Offset=0x248 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemWorkingSetShared:1;// Offset=0x248 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x248 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x249 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char ApcNeeded:1;// Offset=0x249 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char __align0[2];// Offset=0x24a Size=0x2
        };
    };
    unsigned char ForwardClusterOnly;// Offset=0x24c Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x24d Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x24e Size=0x1
} ETHREAD;

struct _KSPECIAL_REGISTERS// Size=0x54
{
    unsigned long Cr0;// Offset=0x0 Size=0x4
    unsigned long Cr2;// Offset=0x4 Size=0x4
    unsigned long Cr3;// Offset=0x8 Size=0x4
    unsigned long Cr4;// Offset=0xc Size=0x4
    unsigned long KernelDr0;// Offset=0x10 Size=0x4
    unsigned long KernelDr1;// Offset=0x14 Size=0x4
    unsigned long KernelDr2;// Offset=0x18 Size=0x4
    unsigned long KernelDr3;// Offset=0x1c Size=0x4
    unsigned long KernelDr6;// Offset=0x20 Size=0x4
    unsigned long KernelDr7;// Offset=0x24 Size=0x4
    struct _DESCRIPTOR Gdtr;// Offset=0x28 Size=0x8
    struct _DESCRIPTOR Idtr;// Offset=0x30 Size=0x8
    unsigned short Tr;// Offset=0x38 Size=0x2
    unsigned short Ldtr;// Offset=0x3a Size=0x2
    unsigned long Reserved[6];// Offset=0x3c Size=0x18
};

struct _KPROCESSOR_STATE// Size=0x320
{
    struct _CONTEXT ContextFrame;// Offset=0x0 Size=0x2cc
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x2cc Size=0x54
};

struct _KDPC_W2003// Size=0x20
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned char Number;// Offset=0x2 Size=0x1
    unsigned char Expedite;// Offset=0x3 Size=0x1
    struct _LIST_ENTRY DpcListEntry;// Offset=0x4 Size=0x8
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0xc Size=0x4
    void *DeferredContext;// Offset=0x10 Size=0x4
    void *SystemArgument1;// Offset=0x14 Size=0x4
    void *SystemArgument2;// Offset=0x18 Size=0x4
    void *DpcData;// Offset=0x1c Size=0x4
};

struct _PROCESSOR_POWER_STATE// Size=0x120
{
    void  ( *IdleFunction)(struct _PROCESSOR_POWER_STATE *);// Offset=0x0 Size=0x4
    unsigned long Idle0KernelTimeLimit;// Offset=0x4 Size=0x4
    unsigned long Idle0LastTime;// Offset=0x8 Size=0x4
    void *IdleHandlers;// Offset=0xc Size=0x4
    void *IdleState;// Offset=0x10 Size=0x4
    unsigned long IdleHandlersCount;// Offset=0x14 Size=0x4
    ULONG64 LastCheck;// Offset=0x18 Size=0x8
    struct PROCESSOR_IDLE_TIMES IdleTimes;// Offset=0x20 Size=0x20
    unsigned long IdleTime1;// Offset=0x40 Size=0x4
    unsigned long PromotionCheck;// Offset=0x44 Size=0x4
    unsigned long IdleTime2;// Offset=0x48 Size=0x4
    unsigned char CurrentThrottle;// Offset=0x4c Size=0x1
    unsigned char ThermalThrottleLimit;// Offset=0x4d Size=0x1
    unsigned char CurrentThrottleIndex;// Offset=0x4e Size=0x1
    unsigned char ThermalThrottleIndex;// Offset=0x4f Size=0x1
    unsigned long LastKernelUserTime;// Offset=0x50 Size=0x4
    unsigned long LastIdleThreadKernelTime;// Offset=0x54 Size=0x4
    unsigned long PackageIdleStartTime;// Offset=0x58 Size=0x4
    unsigned long PackageIdleTime;// Offset=0x5c Size=0x4
    unsigned long DebugCount;// Offset=0x60 Size=0x4
    unsigned long LastSysTime;// Offset=0x64 Size=0x4
    ULONG64 TotalIdleStateTime[3];// Offset=0x68 Size=0x18
    unsigned long TotalIdleTransitions[3];// Offset=0x80 Size=0xc
    unsigned char __align0[4];// Offset=0x8c Size=0x4
    ULONG64 PreviousC3StateTime;// Offset=0x90 Size=0x8
    unsigned char KneeThrottleIndex;// Offset=0x98 Size=0x1
    unsigned char ThrottleLimitIndex;// Offset=0x99 Size=0x1
    unsigned char PerfStatesCount;// Offset=0x9a Size=0x1
    unsigned char ProcessorMinThrottle;// Offset=0x9b Size=0x1
    unsigned char ProcessorMaxThrottle;// Offset=0x9c Size=0x1
    unsigned char EnableIdleAccounting;// Offset=0x9d Size=0x1
    unsigned char LastC3Percentage;// Offset=0x9e Size=0x1
    unsigned char LastAdjustedBusyPercentage;// Offset=0x9f Size=0x1
    unsigned long PromotionCount;// Offset=0xa0 Size=0x4
    unsigned long DemotionCount;// Offset=0xa4 Size=0x4
    unsigned long ErrorCount;// Offset=0xa8 Size=0x4
    unsigned long RetryCount;// Offset=0xac Size=0x4
    unsigned long Flags;// Offset=0xb0 Size=0x4
    unsigned char __align1[4];// Offset=0xb4 Size=0x4
    union _LARGE_INTEGER PerfCounterFrequency;// Offset=0xb8 Size=0x8
    unsigned long PerfTickCount;// Offset=0xc0 Size=0x4
    unsigned char __align2[4];// Offset=0xc4 Size=0x4
    struct _KTIMER PerfTimer;// Offset=0xc8 Size=0x28
    struct _KDPC_W2003 PerfDpc;// Offset=0xf0 Size=0x20
    struct PROCESSOR_PERF_STATE *PerfStates;// Offset=0x110 Size=0x4
    long  ( *PerfSetThrottle)(unsigned char );// Offset=0x114 Size=0x4
    unsigned long LastC3KernelUserTime;// Offset=0x118 Size=0x4
    unsigned long LastPackageIdleTime;// Offset=0x11c Size=0x4
};

struct _KDPC_DATA// Size=0x14
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x8
    unsigned long DpcLock;// Offset=0x8 Size=0x4
    unsigned long volatile DpcQueueDepth;// Offset=0xc Size=0x4
    unsigned long DpcCount;// Offset=0x10 Size=0x4
};

typedef struct _KPRCB// Size=0xec0
{
    unsigned short MinorVersion;// Offset=0x0 Size=0x2
    unsigned short MajorVersion;// Offset=0x2 Size=0x2
    struct _KTHREAD *CurrentThread;// Offset=0x4 Size=0x4
    struct _KTHREAD *NextThread;// Offset=0x8 Size=0x4
    struct _KTHREAD *IdleThread;// Offset=0xc Size=0x4
    char Number;// Offset=0x10 Size=0x1
    char Reserved;// Offset=0x11 Size=0x1
    unsigned short BuildType;// Offset=0x12 Size=0x2
    unsigned long SetMember;// Offset=0x14 Size=0x4
    char CpuType;// Offset=0x18 Size=0x1
    char CpuID;// Offset=0x19 Size=0x1
    unsigned short CpuStep;// Offset=0x1a Size=0x2
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x1c Size=0x320
    unsigned long KernelReserved[16];// Offset=0x33c Size=0x40
    unsigned long HalReserved[16];// Offset=0x37c Size=0x40
    unsigned char PrcbPad0[92];// Offset=0x3bc Size=0x5c
    struct _KSPIN_LOCK_QUEUE LockQueue[33];// Offset=0x418 Size=0x108
    struct _KTHREAD *NpxThread;// Offset=0x520 Size=0x4
    unsigned long InterruptCount;// Offset=0x524 Size=0x4
    unsigned long KernelTime;// Offset=0x528 Size=0x4
    unsigned long UserTime;// Offset=0x52c Size=0x4
    unsigned long DpcTime;// Offset=0x530 Size=0x4
    unsigned long DebugDpcTime;// Offset=0x534 Size=0x4
    unsigned long InterruptTime;// Offset=0x538 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x53c Size=0x4
    unsigned long PageColor;// Offset=0x540 Size=0x4
    unsigned char SkipTick;// Offset=0x544 Size=0x1
    unsigned char DebuggerSavedIRQL;// Offset=0x545 Size=0x1
    unsigned char NodeColor;// Offset=0x546 Size=0x1
    unsigned char Spare1;// Offset=0x547 Size=0x1
    unsigned long NodeShiftedColor;// Offset=0x548 Size=0x4
    struct _KNODE *ParentNode;// Offset=0x54c Size=0x4
    unsigned long MultiThreadProcessorSet;// Offset=0x550 Size=0x4
    struct _KPRCB *MultiThreadSetMaster;// Offset=0x554 Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x558 Size=0x4
    long Sleeping;// Offset=0x55c Size=0x4
    unsigned long CcFastReadNoWait;// Offset=0x560 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x564 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x568 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x56c Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x570 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x574 Size=0x4
    unsigned long KeAlignmentFixupCount;// Offset=0x578 Size=0x4
    unsigned long SpareCounter0;// Offset=0x57c Size=0x4
    unsigned long KeDcacheFlushCount;// Offset=0x580 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x584 Size=0x4
    unsigned long KeFirstLevelTbFills;// Offset=0x588 Size=0x4
    unsigned long KeFloatingEmulationCount;// Offset=0x58c Size=0x4
    unsigned long KeIcacheFlushCount;// Offset=0x590 Size=0x4
    unsigned long KeSecondLevelTbFills;// Offset=0x594 Size=0x4
    unsigned long KeSystemCalls;// Offset=0x598 Size=0x4
    long volatile IoReadOperationCount;// Offset=0x59c Size=0x4
    long volatile IoWriteOperationCount;// Offset=0x5a0 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0x5a4 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0x5a8 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0x5b0 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0x5b8 Size=0x8
    unsigned long SpareCounter1[8];// Offset=0x5c0 Size=0x20
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x5e0 Size=0x80
    struct _PP_LOOKASIDE_LIST PPNPagedLookasideList[32];// Offset=0x660 Size=0x100
    struct _PP_LOOKASIDE_LIST PPPagedLookasideList[32];// Offset=0x760 Size=0x100
    unsigned long volatile PacketBarrier;// Offset=0x860 Size=0x4
    unsigned long volatile ReverseStall;// Offset=0x864 Size=0x4
    void *IpiFrame;// Offset=0x868 Size=0x4
    unsigned char PrcbPad2[52];// Offset=0x86c Size=0x34
    void volatile *CurrentPacket[3];// Offset=0x8a0 Size=0xc
    unsigned long volatile TargetSet;// Offset=0x8ac Size=0x4
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x8b0 Size=0x4
    unsigned long volatile IpiFrozen;// Offset=0x8b4 Size=0x4
    unsigned char PrcbPad3[40];// Offset=0x8b8 Size=0x28
    unsigned long volatile RequestSummary;// Offset=0x8e0 Size=0x4
    struct _KPRCB volatile *SignalDone;// Offset=0x8e4 Size=0x4
    unsigned char PrcbPad4[56];// Offset=0x8e8 Size=0x38
    struct _KDPC_DATA DpcData[2];// Offset=0x920 Size=0x28
    void *DpcStack;// Offset=0x948 Size=0x4
    unsigned long MaximumDpcQueueDepth;// Offset=0x94c Size=0x4
    unsigned long DpcRequestRate;// Offset=0x950 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x954 Size=0x4
    unsigned char volatile DpcInterruptRequested;// Offset=0x958 Size=0x1
    unsigned char volatile DpcThreadRequested;// Offset=0x959 Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x95a Size=0x1
    unsigned char volatile DpcThreadActive;// Offset=0x95b Size=0x1
    unsigned long PrcbLock;// Offset=0x95c Size=0x4
    unsigned long DpcLastCount;// Offset=0x960 Size=0x4
    unsigned long volatile TimerHand;// Offset=0x964 Size=0x4
    unsigned long volatile TimerRequest;// Offset=0x968 Size=0x4
    void *DpcThread;// Offset=0x96c Size=0x4
    struct _KEVENT DpcEvent;// Offset=0x970 Size=0x10
    unsigned char ThreadDpcEnable;// Offset=0x980 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x981 Size=0x1
    unsigned char PrcbPad50;// Offset=0x982 Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x983 Size=0x1
    long DpcSetEventRequest;// Offset=0x984 Size=0x4
    unsigned char PrcbPad5[18];// Offset=0x988 Size=0x12
    unsigned char __align0[2];// Offset=0x99a Size=0x2
    long TickOffset;// Offset=0x99c Size=0x4
    struct _KDPC_W2003 CallDpc;// Offset=0x9a0 Size=0x20
    unsigned long PrcbPad7[8];// Offset=0x9c0 Size=0x20
    struct _LIST_ENTRY WaitListHead;// Offset=0x9e0 Size=0x8
    unsigned long ReadySummary;// Offset=0x9e8 Size=0x4
    unsigned long QueueIndex;// Offset=0x9ec Size=0x4
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x9f0 Size=0x100
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0xaf0 Size=0x4
    unsigned long PrcbPad72[11];// Offset=0xaf4 Size=0x2c
    void *ChainedInterruptList;// Offset=0xb20 Size=0x4
    long LookasideIrpFloat;// Offset=0xb24 Size=0x4
    long volatile MmPageFaultCount;// Offset=0xb28 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0xb2c Size=0x4
    long volatile MmTransitionCount;// Offset=0xb30 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0xb34 Size=0x4
    long volatile MmDemandZeroCount;// Offset=0xb38 Size=0x4
    long volatile MmPageReadCount;// Offset=0xb3c Size=0x4
    long volatile MmPageReadIoCount;// Offset=0xb40 Size=0x4
    long volatile MmCacheReadCount;// Offset=0xb44 Size=0x4
    long volatile MmCacheIoCount;// Offset=0xb48 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0xb4c Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0xb50 Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0xb54 Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0xb58 Size=0x4
    unsigned long SpareFields0[1];// Offset=0xb5c Size=0x4
    unsigned char VendorString[13];// Offset=0xb60 Size=0xd
    unsigned char InitialApicId;// Offset=0xb6d Size=0x1
    unsigned char LogicalProcessorsPerPhysicalProcessor;// Offset=0xb6e Size=0x1
    unsigned char __align1[1];// Offset=0xb6f Size=0x1
    unsigned long MHz;// Offset=0xb70 Size=0x4
    unsigned long FeatureBits;// Offset=0xb74 Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0xb78 Size=0x8
    ULONG64 volatile IsrTime;// Offset=0xb80 Size=0x8
    ULONG64 SpareField1;// Offset=0xb88 Size=0x8
    struct _FX_SAVE_AREA NpxSaveArea;// Offset=0xb90 Size=0x210
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0xda0 Size=0x120
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0xa0
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x94
    struct _EX_PUSH_LOCK Lock;// Offset=0x94 Size=0x4
    struct _DEVICE_MAP *DeviceMap;// Offset=0x98 Size=0x4
    unsigned long SessionId;// Offset=0x9c Size=0x4
} *POBJECT_DIRECTORY;

#endif //                  Windows 2003 x32



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows Vista x32 internal kernel definitions                     //
#if (NTDDI_VERSION >= NTDDI_VISTA) && (NTDDI_VERSION <= NTDDI_VISTASP4) && defined(_X86_)

typedef struct _MM_SESSION_SPACE// Size=0x1ec0
{
    long volatile ReferenceCount;// Offset=0x0 Size=0x4
    union {
        ULONG LongFlags;// Offset=0x0 Size=0x4
        struct _MM_SESSION_SPACE_FLAGS Flags;// Offset=0x0 Size=0x4
    } u;
    unsigned long SessionId;// Offset=0x8 Size=0x4
    // ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x4
{
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long Locked:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long Waiting:1;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long Waking:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long MultipleShared:1;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Shared:28;// Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1c
        };
        unsigned long Value;// Offset=0x0 Size=0x4
        void *Ptr;// Offset=0x0 Size=0x4
    };
};

typedef struct _EX_FAST_REF// Size=0x4
{
    union // Size=0x4
    {
        void *Object;// Offset=0x0 Size=0x4
        unsigned long RefCnt:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long Value;// Offset=0x0 Size=0x4
    };
} EX_FAST_REF, *PEX_FAST_REF;

typedef struct _MMADDRESS_NODE // Size=0x14
{
    union {// Offset=0x0 Size=0x4
        long Balance:2;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x4
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x4 Size=0x4
    struct _MMADDRESS_NODE *RightChild;// Offset=0x8 Size=0x4
    unsigned long StartingVpn;// Offset=0xc Size=0x4
    unsigned long EndingVpn;// Offset=0x10 Size=0x4
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE// Size=0x20
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x14
    unsigned long DepthOfTree:5;// Offset=0x14 Size=0x4 BitOffset=0x0 BitSize=0x5
    unsigned long Unused:3;// Offset=0x14 Size=0x4 BitOffset=0x5 BitSize=0x3
    unsigned long NumberGenericTableElements:24;// Offset=0x14 Size=0x4 BitOffset=0x8 BitSize=0x18
    void *NodeHint;// Offset=0x18 Size=0x4
    void *NodeFreeHint;// Offset=0x1c Size=0x4
};

struct _KPROCESS// Size=0x80
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY ProfileListHead;// Offset=0x10 Size=0x8
    unsigned long DirectoryTableBase;// Offset=0x18 Size=0x4
    unsigned long Unused0;// Offset=0x1c Size=0x4
    struct _KGDTENTRY LdtDescriptor;// Offset=0x20 Size=0x8
    struct _KIDTENTRY Int21Descriptor;// Offset=0x28 Size=0x8
    unsigned short IopmOffset;// Offset=0x30 Size=0x2
    unsigned char Unused1;// Offset=0x32 Size=0x1
    unsigned char Unused2;// Offset=0x33 Size=0x1
    unsigned long volatile ActiveProcessors;// Offset=0x34 Size=0x4
    unsigned long KernelTime;// Offset=0x38 Size=0x4
    unsigned long UserTime;// Offset=0x3c Size=0x4
    struct _LIST_ENTRY ReadyListHead;// Offset=0x40 Size=0x8
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x48 Size=0x4
    void *VdmTrapcHandler;// Offset=0x4c Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x50 Size=0x8
    unsigned long ProcessLock;// Offset=0x58 Size=0x4
    unsigned long Affinity;// Offset=0x5c Size=0x4
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long volatile AutoAlignment:1;// Offset=0x60 Size=0x4 BitOffset=0x0 BitSize=0x1
            long volatile DisableBoost:1;// Offset=0x60 Size=0x4 BitOffset=0x1 BitSize=0x1
            long volatile DisableQuantum:1;// Offset=0x60 Size=0x4 BitOffset=0x2 BitSize=0x1
            long volatile ReservedFlags:29;// Offset=0x60 Size=0x4 BitOffset=0x3 BitSize=0x1d
        };
        long volatile ProcessFlags;// Offset=0x60 Size=0x4
    };
    char BasePriority;// Offset=0x64 Size=0x1
    char QuantumReset;// Offset=0x65 Size=0x1
    unsigned char State;// Offset=0x66 Size=0x1
    unsigned char ThreadSeed;// Offset=0x67 Size=0x1
    unsigned char PowerState;// Offset=0x68 Size=0x1
    unsigned char IdealNode;// Offset=0x69 Size=0x1
    unsigned char Visited;// Offset=0x6a Size=0x1
    union // Size=0x1
    {
        struct _KEXECUTE_OPTIONS Flags;// Offset=0x6b Size=0x1
        unsigned char ExecuteOptions;// Offset=0x6b Size=0x1
    };
    unsigned long StackCount;// Offset=0x6c Size=0x4
    struct _LIST_ENTRY ProcessListEntry;// Offset=0x70 Size=0x8
    ULONG64 volatile CycleTime;// Offset=0x78 Size=0x8
};

struct _MMSUPPORT// Size=0x48
{
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x0 Size=0x8
    unsigned short LastTrimStamp;// Offset=0x8 Size=0x2
    unsigned short NextPageColor;// Offset=0xa Size=0x2
    struct _MMSUPPORT_FLAGS Flags;// Offset=0xc Size=0x4
    unsigned long PageFaultCount;// Offset=0x10 Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x14 Size=0x4
    unsigned long ChargedWslePages;// Offset=0x18 Size=0x4
    unsigned long MinimumWorkingSetSize;// Offset=0x1c Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x20 Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x24 Size=0x4
    unsigned long Claim;// Offset=0x28 Size=0x4
    unsigned long ActualWslePages;// Offset=0x2c Size=0x4
    unsigned long WorkingSetPrivateSize;// Offset=0x30 Size=0x4
    unsigned long WorkingSetSizeOverhead;// Offset=0x34 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x38 Size=0x4
    struct _KGATE *ExitGate;// Offset=0x3c Size=0x4
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x40 Size=0x4
    void *AccessLog;// Offset=0x44 Size=0x4
};

struct _ALPC_PROCESS_CONTEXT// Size=0x10
{
    struct _EX_PUSH_LOCK Lock;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY ViewListHead;// Offset=0x4 Size=0x8
    unsigned long volatile PagedPoolQuotaCache;// Offset=0xc Size=0x4
};

typedef struct _EPROCESS// Size=0x270
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0x80
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0x80 Size=0x4
    unsigned char __align0[4];// Offset=0x84 Size=0x4
    union _LARGE_INTEGER CreateTime;// Offset=0x88 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0x90 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x98 Size=0x4
    void *UniqueProcessId;// Offset=0x9c Size=0x4
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0xa0 Size=0x8
    unsigned long QuotaUsage[3];// Offset=0xa8 Size=0xc
    unsigned long QuotaPeak[3];// Offset=0xb4 Size=0xc
    unsigned long volatile CommitCharge;// Offset=0xc0 Size=0x4
    unsigned long PeakVirtualSize;// Offset=0xc4 Size=0x4
    unsigned long VirtualSize;// Offset=0xc8 Size=0x4
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0xcc Size=0x8
    void *DebugPort;// Offset=0xd4 Size=0x4
    union // Size=0x4
    {
        void *ExceptionPortData;// Offset=0xd8 Size=0x4
        unsigned long ExceptionPortValue;// Offset=0xd8 Size=0x4
        unsigned long ExceptionPortState:3;// Offset=0xd8 Size=0x4 BitOffset=0x0 BitSize=0x3
    };
    struct _HANDLE_TABLE *ObjectTable;// Offset=0xdc Size=0x4
    struct _EX_FAST_REF Token;// Offset=0xe0 Size=0x4
    unsigned long WorkingSetPage;// Offset=0xe4 Size=0x4
    struct _EX_PUSH_LOCK AddressCreationLock;// Offset=0xe8 Size=0x4
    struct _ETHREAD *RotateInProgress;// Offset=0xec Size=0x4
    struct _ETHREAD *ForkInProgress;// Offset=0xf0 Size=0x4
    unsigned long HardwareTrigger;// Offset=0xf4 Size=0x4
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0xf8 Size=0x4
    void *CloneRoot;// Offset=0xfc Size=0x4
    unsigned long volatile NumberOfPrivatePages;// Offset=0x100 Size=0x4
    unsigned long volatile NumberOfLockedPages;// Offset=0x104 Size=0x4
    void *Win32Process;// Offset=0x108 Size=0x4
    struct _EJOB *Job;// Offset=0x10c Size=0x4
    void *SectionObject;// Offset=0x110 Size=0x4
    void *SectionBaseAddress;// Offset=0x114 Size=0x4
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x118 Size=0x4
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x11c Size=0x4
    void *Win32WindowStation;// Offset=0x120 Size=0x4
    void *InheritedFromUniqueProcessId;// Offset=0x124 Size=0x4
    void *LdtInformation;// Offset=0x128 Size=0x4
    void *Spare;// Offset=0x12c Size=0x4
    void *VdmObjects;// Offset=0x130 Size=0x4
    void *DeviceMap;// Offset=0x134 Size=0x4
    void *EtwDataSource;// Offset=0x138 Size=0x4
    void *FreeTebHint;// Offset=0x13c Size=0x4
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x140 Size=0x4
        ULONG64 Filler;// Offset=0x140 Size=0x8
    };
    void *Session;// Offset=0x148 Size=0x4
    unsigned char ImageFileName[16];// Offset=0x14c Size=0x10
    struct _LIST_ENTRY JobLinks;// Offset=0x15c Size=0x8
    void *LockedPagesList;// Offset=0x164 Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x168 Size=0x8
    void *SecurityPort;// Offset=0x170 Size=0x4
    void *PaeTop;// Offset=0x174 Size=0x4
    unsigned long volatile ActiveThreads;// Offset=0x178 Size=0x4
    unsigned long ImagePathHash;// Offset=0x17c Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x180 Size=0x4
    long LastThreadExitStatus;// Offset=0x184 Size=0x4
    struct _PEB *Peb;// Offset=0x188 Size=0x4
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x18c Size=0x4
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x190 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x198 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x1a0 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x1a8 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x1b0 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x1b8 Size=0x8
    unsigned long CommitChargeLimit;// Offset=0x1c0 Size=0x4
    unsigned long volatile CommitChargePeak;// Offset=0x1c4 Size=0x4
    void *AweInfo;// Offset=0x1c8 Size=0x4
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x1cc Size=0x4
    struct _MMSUPPORT Vm;// Offset=0x1d0 Size=0x48
    struct _LIST_ENTRY MmProcessLinks;// Offset=0x218 Size=0x8
    unsigned long ModifiedPageCount;// Offset=0x220 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags2;// Offset=0x224 Size=0x4
        struct // Size=0x4
        {
            unsigned long JobNotReallyActive:1;// Offset=0x224 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long AccountingFolded:1;// Offset=0x224 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long NewProcessReported:1;// Offset=0x224 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ExitProcessReported:1;// Offset=0x224 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long ReportCommitChanges:1;// Offset=0x224 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long LastReportMemory:1;// Offset=0x224 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long ReportPhysicalPageChanges:1;// Offset=0x224 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long HandleTableRundown:1;// Offset=0x224 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long NeedsHandleRundown:1;// Offset=0x224 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long RefTraceEnabled:1;// Offset=0x224 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long NumaAware:1;// Offset=0x224 Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long ProtectedProcess:1;// Offset=0x224 Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long DefaultPagePriority:3;// Offset=0x224 Size=0x4 BitOffset=0xc BitSize=0x3
            unsigned long PrimaryTokenFrozen:1;// Offset=0x224 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessVerifierTarget:1;// Offset=0x224 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long StackRandomizationDisabled:1;// Offset=0x224 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long AffinityPermanent:1;// Offset=0x224 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long AffinityUpdateEnable:1;// Offset=0x224 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long CrossSessionCreate:1;// Offset=0x224 Size=0x4 BitOffset=0x14 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x228 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x228 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x228 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x228 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x228 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x228 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x228 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x228 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x228 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x228 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x228 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x228 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x228 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x228 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long DeprioritizeViews:1;// Offset=0x228 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x228 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x228 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x228 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x228 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x228 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x228 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x228 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x228 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x228 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x228 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long SmapAllowed:1;// Offset=0x228 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long ProcessInserted:1;// Offset=0x228 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x228 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long ProcessSelfDelete:1;// Offset=0x228 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long SpareProcessFlags:1;// Offset=0x228 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x22c Size=0x4
    unsigned short Spare7;// Offset=0x230 Size=0x2
    union // Size=0x2
    {
        struct // Size=0x2
        {
            unsigned char SubSystemMinorVersion;// Offset=0x232 Size=0x1
            unsigned char SubSystemMajorVersion;// Offset=0x233 Size=0x1
        };
        unsigned short SubSystemVersion;// Offset=0x232 Size=0x2
    };
    unsigned char PriorityClass;// Offset=0x234 Size=0x1
    unsigned char __align1[3];// Offset=0x235 Size=0x3
    struct _MM_AVL_TABLE VadRoot;// Offset=0x238 Size=0x20
    unsigned long Cookie;// Offset=0x258 Size=0x4
    struct _ALPC_PROCESS_CONTEXT AlpcContext;// Offset=0x25c Size=0x10
} EPROCESS;

struct _CONTROL_AREA// Size=0x48
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY DereferenceList;// Offset=0x4 Size=0x8
    unsigned long NumberOfSectionReferences;// Offset=0xc Size=0x4
    unsigned long NumberOfPfnReferences;// Offset=0x10 Size=0x4
    unsigned long NumberOfMappedViews;// Offset=0x14 Size=0x4
    unsigned long NumberOfUserReferences;// Offset=0x18 Size=0x4
    union {// Offset=0x1c Size=0x4
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    union {// Offset=0x20 Size=0x4
        struct // Size=0x4
        {
            unsigned short ModifiedWriteCount;// Offset=0x0 Size=0x2
            unsigned short FlushInProgressCount;// Offset=0x2 Size=0x2
        } e2;
    } u1;
    struct _EX_FAST_REF FilePointer;// Offset=0x24 Size=0x4
    long volatile ControlAreaLock;// Offset=0x28 Size=0x4
    unsigned long StartingFrame;// Offset=0x2c Size=0x4
    struct _MI_SECTION_CREATION_GATE *WaitingForDeletion;// Offset=0x30 Size=0x4
    union {// Offset=0x34 Size=0xc
        struct // Size=0xc
        {
            union // Size=0x4
            {
                unsigned long NumberOfSystemCacheViews;// Offset=0x0 Size=0x4
                unsigned long ImageRelocationStartBit;// Offset=0x0 Size=0x4
            };
            union // Size=0x4
            {
                long volatile WritableUserReferences;// Offset=0x4 Size=0x4
                struct // Size=0x4
                {
                    unsigned long ImageRelocationSizeIn64k:16;// Offset=0x4 Size=0x4 BitOffset=0x0 BitSize=0x10
                    unsigned long Unused:14;// Offset=0x4 Size=0x4 BitOffset=0x10 BitSize=0xe
                    unsigned long BitMap64:1;// Offset=0x4 Size=0x4 BitOffset=0x1e BitSize=0x1
                    unsigned long ImageActive:1;// Offset=0x4 Size=0x4 BitOffset=0x1f BitSize=0x1
                };
            };
            union // Size=0x4
            {
                struct _MM_SUBSECTION_AVL_TABLE *SubsectionRoot;// Offset=0x8 Size=0x4
                struct _MI_IMAGE_SECURITY_REFERENCE *SeImageStub;// Offset=0x8 Size=0x4
            };
        } e2;    
    } u2;
    LONG64 volatile LockedPages;// Offset=0x40 Size=0x8
};

typedef struct _SEGMENT // Size=0x30/0x38 NOPAE/PAE
{
    struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
    // ...
} *PSEGMENT;

typedef struct _SECTION// Size=0x28
{
    struct _MMADDRESS_NODE Address;// Offset=0x0 Size=0x14
    struct _SEGMENT *Segment;// Offset=0x14 Size=0x4
    union _LARGE_INTEGER SizeOfSection;// Offset=0x18 Size=0x8
    union {// Offset=0x20 Size=0x4
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    unsigned long InitialPageProtection:12;// Offset=0x24 Size=0x4 BitOffset=0x0 BitSize=0xc
    unsigned long SessionId:20;// Offset=0x24 Size=0x4 BitOffset=0xc BitSize=0x14
} SECTION, *PSECTION;

struct _KAPC_STATE// Size=0x18
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x10
    struct _KPROCESS *Process;// Offset=0x10 Size=0x4
    unsigned char KernelApcInProgress;// Offset=0x14 Size=0x1
    unsigned char KernelApcPending;// Offset=0x15 Size=0x1
    unsigned char UserApcPending;// Offset=0x16 Size=0x1
};

struct _KTHREAD// Size=0x1e0
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    ULONG64 volatile CycleTime;// Offset=0x10 Size=0x8
    unsigned long volatile HighCycleTime;// Offset=0x18 Size=0x4
    unsigned char __align0[4];// Offset=0x1c Size=0x4
    ULONG64 QuantumTarget;// Offset=0x20 Size=0x8
    void *InitialStack;// Offset=0x28 Size=0x4
    void volatile *StackLimit;// Offset=0x2c Size=0x4
    void *KernelStack;// Offset=0x30 Size=0x4
    unsigned long ThreadLock;// Offset=0x34 Size=0x4
    union // Size=0x18
    {
        struct _KAPC_STATE ApcState;// Offset=0x38 Size=0x18
        struct // Size=0x18
        {
            unsigned char ApcStateFill[23];// Offset=0x38 Size=0x17
            char Priority;// Offset=0x4f Size=0x1
        };
    };
    unsigned short volatile NextProcessor;// Offset=0x50 Size=0x2
    unsigned short volatile DeferredProcessor;// Offset=0x52 Size=0x2
    unsigned long ApcQueueLock;// Offset=0x54 Size=0x4
    unsigned long ContextSwitches;// Offset=0x58 Size=0x4
    unsigned char volatile State;// Offset=0x5c Size=0x1
    unsigned char NpxState;// Offset=0x5d Size=0x1
    unsigned char WaitIrql;// Offset=0x5e Size=0x1
    char WaitMode;// Offset=0x5f Size=0x1
    long WaitStatus;// Offset=0x60 Size=0x4
    union // Size=0x4
    {
        struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x64 Size=0x4
        struct _KGATE *GateObject;// Offset=0x64 Size=0x4
    };
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long KernelStackResident:1;// Offset=0x68 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ReadyTransition:1;// Offset=0x68 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessReadyQueue:1;// Offset=0x68 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long WaitNext:1;// Offset=0x68 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemAffinityActive:1;// Offset=0x68 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long Alertable:1;// Offset=0x68 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long GdiFlushActive:1;// Offset=0x68 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long UserStackWalkActive:1;// Offset=0x68 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long Reserved:24;// Offset=0x68 Size=0x4 BitOffset=0x8 BitSize=0x18
        };
        long MiscFlags;// Offset=0x68 Size=0x4
    };
    unsigned char WaitReason;// Offset=0x6c Size=0x1
    unsigned char volatile SwapBusy;// Offset=0x6d Size=0x1
    unsigned char Alerted[2];// Offset=0x6e Size=0x2
    union // Size=0x8
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x70 Size=0x8
        struct // Size=0x8
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x70 Size=0x4
            unsigned char __align1[4];// Offset=0x74 Size=0x4
        };
    };
    struct _KQUEUE *Queue;// Offset=0x78 Size=0x4
    unsigned long WaitTime;// Offset=0x7c Size=0x4
    union // Size=0x4
    {
        struct // Size=0x4
        {
            short KernelApcDisable;// Offset=0x80 Size=0x2
            short SpecialApcDisable;// Offset=0x82 Size=0x2
        };
        unsigned long CombinedApcDisable;// Offset=0x80 Size=0x4
    };
    void *Teb;// Offset=0x84 Size=0x4
    union // Size=0x28
    {
        struct _KTIMER Timer;// Offset=0x88 Size=0x28
        unsigned char TimerFill[40];// Offset=0x88 Size=0x28
    };
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long volatile AutoAlignment:1;// Offset=0xb0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long volatile DisableBoost:1;// Offset=0xb0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long volatile EtwStackTraceApc1Inserted:1;// Offset=0xb0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long volatile EtwStackTraceApc2Inserted:1;// Offset=0xb0 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long volatile CycleChargePending:1;// Offset=0xb0 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long volatile CalloutActive:1;// Offset=0xb0 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long volatile ApcQueueable:1;// Offset=0xb0 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long volatile EnableStackSwap:1;// Offset=0xb0 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long volatile GuiThread:1;// Offset=0xb0 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long volatile ReservedFlags:23;// Offset=0xb0 Size=0x4 BitOffset=0x9 BitSize=0x17
        };
        long volatile ThreadFlags;// Offset=0xb0 Size=0x4
    };
    unsigned char __align2[4];// Offset=0xb4 Size=0x4
    union // Size=0x60
    {
        struct _KWAIT_BLOCK WaitBlock[4];// Offset=0xb8 Size=0x60
        struct // Size=0x18
        {
            unsigned char WaitBlockFill0[23];// Offset=0xb8 Size=0x17
            unsigned char IdealProcessor;// Offset=0xcf Size=0x1
        };
        struct // Size=0x30
        {
            unsigned char WaitBlockFill1[47];// Offset=0xb8 Size=0x2f
            char PreviousMode;// Offset=0xe7 Size=0x1
        };
        struct // Size=0x48
        {
            unsigned char WaitBlockFill2[71];// Offset=0xb8 Size=0x47
            unsigned char ResourceIndex;// Offset=0xff Size=0x1
        };
        struct // Size=0x60
        {
            unsigned char WaitBlockFill3[95];// Offset=0xb8 Size=0x5f
            unsigned char LargeStack;// Offset=0x117 Size=0x1
        };
    };
    struct _LIST_ENTRY QueueListEntry;// Offset=0x118 Size=0x8
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x120 Size=0x4
    void *FirstArgument;// Offset=0x124 Size=0x4
    union // Size=0x4
    {
        void *CallbackStack;// Offset=0x128 Size=0x4
        unsigned long CallbackDepth;// Offset=0x128 Size=0x4
    };
    void *ServiceTable;// Offset=0x12c Size=0x4
    unsigned char ApcStateIndex;// Offset=0x130 Size=0x1
    char BasePriority;// Offset=0x131 Size=0x1
    char PriorityDecrement;// Offset=0x132 Size=0x1
    unsigned char Preempted;// Offset=0x133 Size=0x1
    unsigned char AdjustReason;// Offset=0x134 Size=0x1
    char AdjustIncrement;// Offset=0x135 Size=0x1
    unsigned char Spare01;// Offset=0x136 Size=0x1
    char Saturation;// Offset=0x137 Size=0x1
    unsigned long SystemCallNumber;// Offset=0x138 Size=0x4
    unsigned long FreezeCount;// Offset=0x13c Size=0x4
    unsigned long UserAffinity;// Offset=0x140 Size=0x4
    struct _KPROCESS *Process;// Offset=0x144 Size=0x4
    unsigned long volatile Affinity;// Offset=0x148 Size=0x4
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x14c Size=0x8
    union // Size=0x18
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x154 Size=0x18
        struct // Size=0x18
        {
            unsigned char SavedApcStateFill[23];// Offset=0x154 Size=0x17
            unsigned char Spare02;// Offset=0x16b Size=0x1
        };
    };
    char SuspendCount;// Offset=0x16c Size=0x1
    unsigned char UserIdealProcessor;// Offset=0x16d Size=0x1
    unsigned char Spare03;// Offset=0x16e Size=0x1
    unsigned char OtherPlatformFill;// Offset=0x16f Size=0x1
    void volatile *Win32Thread;// Offset=0x170 Size=0x4
    void *StackBase;// Offset=0x174 Size=0x4
    union // Size=0x30
    {
        struct _KAPC SuspendApc;// Offset=0x178 Size=0x30
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x178 Size=0x1
            char Spare04;// Offset=0x179 Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x178 Size=0x3
            unsigned char QuantumReset;// Offset=0x17b Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x178 Size=0x4
            unsigned long KernelTime;// Offset=0x17c Size=0x4
        };
        struct // Size=0x28
        {
            unsigned char SuspendApcFill3[36];// Offset=0x178 Size=0x24
            struct _KPRCB *WaitPrcb;// Offset=0x19c Size=0x4
        };
        struct // Size=0x2c
        {
            unsigned char SuspendApcFill4[40];// Offset=0x178 Size=0x28
            void *LegoData;// Offset=0x1a0 Size=0x4
        };
        struct // Size=0x30
        {
            unsigned char SuspendApcFill5[47];// Offset=0x178 Size=0x2f
            unsigned char PowerState;// Offset=0x1a7 Size=0x1
        };
    };
    unsigned long UserTime;// Offset=0x1a8 Size=0x4
    union // Size=0x14
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x1ac Size=0x14
        unsigned char SuspendSemaphorefill[20];// Offset=0x1ac Size=0x14
    };
    unsigned long SListFaultCount;// Offset=0x1c0 Size=0x4
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x1c4 Size=0x8
    struct _LIST_ENTRY MutantListHead;// Offset=0x1cc Size=0x8
    void *SListFaultAddress;// Offset=0x1d4 Size=0x4
    void volatile *MdlForLockedTeb;// Offset=0x1d8 Size=0x4
};

union _PS_CLIENT_SECURITY_CONTEXT// Size=0x4
{
    unsigned long ImpersonationData;// Offset=0x0 Size=0x4
    void *ImpersonationToken;// Offset=0x0 Size=0x4
    struct // Size=0x4
    {
        unsigned long ImpersonationLevel:2;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x2
        unsigned long EffectiveOnly:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
    };
};

typedef struct _ETHREAD// Size=0x288
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x1e0
    union _LARGE_INTEGER CreateTime;// Offset=0x1e0 Size=0x8
    union // Size=0x8
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x1e8 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x1e8 Size=0x8
    };
    union // Size=0x4
    {
        long ExitStatus;// Offset=0x1f0 Size=0x4
        void *OfsChain;// Offset=0x1f0 Size=0x4
    };
    union // Size=0x8
    {
        struct _LIST_ENTRY PostBlockList;// Offset=0x1f4 Size=0x8
        struct // Size=0x8
        {
            void *ForwardLinkShadow;// Offset=0x1f4 Size=0x4
            void *StartAddress;// Offset=0x1f8 Size=0x4
        };
    };
    union // Size=0x4
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x1fc Size=0x4
        struct _ETHREAD *ReaperLink;// Offset=0x1fc Size=0x4
        void *KeyedWaitValue;// Offset=0x1fc Size=0x4
        void *Win32StartParameter;// Offset=0x1fc Size=0x4
    };
    unsigned long ActiveTimerListLock;// Offset=0x200 Size=0x4
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x204 Size=0x8
    struct _CLIENT_ID Cid;// Offset=0x20c Size=0x8
    union // Size=0x14
    {
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x214 Size=0x14
        struct _KSEMAPHORE AlpcWaitSemaphore;// Offset=0x214 Size=0x14
    };
    union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity;// Offset=0x228 Size=0x4
    struct _LIST_ENTRY IrpList;// Offset=0x22c Size=0x8
    unsigned long TopLevelIrp;// Offset=0x234 Size=0x4
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x238 Size=0x4
    union _PSP_RATE_APC *RateControlApc;// Offset=0x23c Size=0x4
    void *Win32StartAddress;// Offset=0x240 Size=0x4
    void *SparePtr0;// Offset=0x244 Size=0x4
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x248 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x250 Size=0x4
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x254 Size=0x4
    unsigned long ReadClusterSize;// Offset=0x258 Size=0x4
    long volatile MmLockOrdering;// Offset=0x25c Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x260 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x260 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ThreadInserted:1;// Offset=0x260 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x260 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x260 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x260 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x260 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x260 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x260 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x260 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long CopyTokenOnOpen:1;// Offset=0x260 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long ThreadIoPriority:3;// Offset=0x260 Size=0x4 BitOffset=0xa BitSize=0x3
            unsigned long ThreadPagePriority:3;// Offset=0x260 Size=0x4 BitOffset=0xd BitSize=0x3
            unsigned long RundownFail:1;// Offset=0x260 Size=0x4 BitOffset=0x10 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x264 Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x264 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x264 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x264 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ClonedThread:1;// Offset=0x264 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x264 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long RateApcState:2;// Offset=0x264 Size=0x4 BitOffset=0x5 BitSize=0x2
            unsigned long SelfTerminate:1;// Offset=0x264 Size=0x4 BitOffset=0x7 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x268 Size=0x4
        struct // Size=0x4
        {
            unsigned char Spare:1;// Offset=0x268 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char volatile StartAddressInvalid:1;// Offset=0x268 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char EtwPageFaultCalloutActive:1;// Offset=0x268 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x268 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x268 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemWorkingSetExclusive:1;// Offset=0x268 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemWorkingSetShared:1;// Offset=0x268 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x268 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x269 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceExclusive:1;// Offset=0x269 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceShared:1;// Offset=0x269 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char SuppressSymbolLoad:1;// Offset=0x269 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char Prefetching:1;// Offset=0x269 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsDynamicMemoryShared:1;// Offset=0x269 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsChangeControlAreaExclusive:1;// Offset=0x269 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsChangeControlAreaShared:1;// Offset=0x269 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char Spare1:8;// Offset=0x26a Size=0x1 BitOffset=0x0 BitSize=0x8
            unsigned char PriorityRegionActive;// Offset=0x26b Size=0x1
        };
    };
    unsigned char CacheManagerActive;// Offset=0x26c Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x26d Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x26e Size=0x1
    unsigned char __align0[1];// Offset=0x26f Size=0x1
    unsigned long AlpcMessageId;// Offset=0x270 Size=0x4
    union // Size=0x4
    {
        void *AlpcMessage;// Offset=0x274 Size=0x4
        unsigned long AlpcReceiveAttributeSet;// Offset=0x274 Size=0x4
    };
    struct _LIST_ENTRY AlpcWaitListEntry;// Offset=0x278 Size=0x8
    unsigned long CacheManagerCount;// Offset=0x280 Size=0x4
} ETHREAD;

struct _KSPECIAL_REGISTERS// Size=0x54
{
    unsigned long Cr0;// Offset=0x0 Size=0x4
    unsigned long Cr2;// Offset=0x4 Size=0x4
    unsigned long Cr3;// Offset=0x8 Size=0x4
    unsigned long Cr4;// Offset=0xc Size=0x4
    unsigned long KernelDr0;// Offset=0x10 Size=0x4
    unsigned long KernelDr1;// Offset=0x14 Size=0x4
    unsigned long KernelDr2;// Offset=0x18 Size=0x4
    unsigned long KernelDr3;// Offset=0x1c Size=0x4
    unsigned long KernelDr6;// Offset=0x20 Size=0x4
    unsigned long KernelDr7;// Offset=0x24 Size=0x4
    struct _DESCRIPTOR Gdtr;// Offset=0x28 Size=0x8
    struct _DESCRIPTOR Idtr;// Offset=0x30 Size=0x8
    unsigned short Tr;// Offset=0x38 Size=0x2
    unsigned short Ldtr;// Offset=0x3a Size=0x2
    unsigned long Reserved[6];// Offset=0x3c Size=0x18
};

struct _KPROCESSOR_STATE// Size=0x320
{
    struct _CONTEXT ContextFrame;// Offset=0x0 Size=0x2cc
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x2cc Size=0x54
};

struct _KDPC_VISTA// Size=0x20
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned short volatile Number;// Offset=0x2 Size=0x2
    struct _LIST_ENTRY DpcListEntry;// Offset=0x4 Size=0x8
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0xc Size=0x4
    void *DeferredContext;// Offset=0x10 Size=0x4
    void *SystemArgument1;// Offset=0x14 Size=0x4
    void *SystemArgument2;// Offset=0x18 Size=0x4
    void *DpcData;// Offset=0x1c Size=0x4
};

union unnamed_tag0// Size=0x2
{
    unsigned short AsUSHORT;// Offset=0x0 Size=0x2
    struct // Size=0x2
    {
        unsigned short PStateDomain:1;// Offset=0x0 Size=0x2 BitOffset=0x0 BitSize=0x1
        unsigned short PStateDomainIdleAccounting:1;// Offset=0x0 Size=0x2 BitOffset=0x1 BitSize=0x1
        unsigned short Reserved:14;// Offset=0x0 Size=0x2 BitOffset=0x2 BitSize=0xe
    };
};

struct _PROCESSOR_POWER_STATE// Size=0xc8
{
    struct _PPM_IDLE_STATES *IdleStates;// Offset=0x0 Size=0x4
    unsigned char __align0[4];// Offset=0x4 Size=0x4
    ULONG64 LastTimeCheck;// Offset=0x8 Size=0x8
    ULONG64 IdleTimeAccumulated;// Offset=0x10 Size=0x8
    union // Size=0x8
    {
        struct
        {
            ULONG64 IdleTransitionTime;
        } Native;
        struct
        {
            ULONG64 LastIdleCheck;
        } Hv;
    };
    struct PPM_IDLE_ACCOUNTING *IdleAccounting;// Offset=0x20 Size=0x4
    struct _PPM_PERF_STATES *PerfStates;// Offset=0x24 Size=0x4
    unsigned long LastKernelUserTime;// Offset=0x28 Size=0x4
    unsigned long LastIdleThreadKTime;// Offset=0x2c Size=0x4
    ULONG64 LastGlobalTimeHv;// Offset=0x30 Size=0x8
    ULONG64 LastProcessorTimeHv;// Offset=0x38 Size=0x8
    unsigned char ThermalConstraint;// Offset=0x40 Size=0x1
    unsigned char LastBusyPercentage;// Offset=0x41 Size=0x1
    union unnamed_tag0 Flags;// Offset=0x42 Size=0x2
    unsigned char __align1[4];// Offset=0x44 Size=0x4
    struct _KTIMER PerfTimer;// Offset=0x48 Size=0x28
    struct _KDPC_VISTA PerfDpc;// Offset=0x70 Size=0x20
    unsigned long LastSysTime;// Offset=0x90 Size=0x4
    struct _KPRCB *PStateMaster;// Offset=0x94 Size=0x4
    unsigned long PStateSet;// Offset=0x98 Size=0x4
    unsigned long CurrentPState;// Offset=0x9c Size=0x4
    unsigned long DesiredPState;// Offset=0xa0 Size=0x4
    unsigned long volatile PStateIdleStartTime;// Offset=0xa4 Size=0x4
    unsigned long PStateIdleTime;// Offset=0xa8 Size=0x4
    unsigned long LastPStateIdleTime;// Offset=0xac Size=0x4
    unsigned long PStateStartTime;// Offset=0xb0 Size=0x4
    unsigned long DiaIndex;// Offset=0xb4 Size=0x4
    unsigned long Reserved0;// Offset=0xb8 Size=0x4
    unsigned long WmiDispatchPtr;// Offset=0xbc Size=0x4
    long WmiInterfaceEnabled;// Offset=0xc0 Size=0x4
};

struct _KDPC_DATA// Size=0x14
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x8
    unsigned long DpcLock;// Offset=0x8 Size=0x4
    long volatile DpcQueueDepth;// Offset=0xc Size=0x4
    unsigned long DpcCount;// Offset=0x10 Size=0x4
};

typedef struct _KPRCB// Size=0x2008
{
    unsigned short MinorVersion;// Offset=0x0 Size=0x2
    unsigned short MajorVersion;// Offset=0x2 Size=0x2
    struct _KTHREAD *CurrentThread;// Offset=0x4 Size=0x4
    struct _KTHREAD *NextThread;// Offset=0x8 Size=0x4
    struct _KTHREAD *IdleThread;// Offset=0xc Size=0x4
    unsigned char Number;// Offset=0x10 Size=0x1
    unsigned char NestingLevel;// Offset=0x11 Size=0x1
    unsigned short BuildType;// Offset=0x12 Size=0x2
    unsigned long SetMember;// Offset=0x14 Size=0x4
    char CpuType;// Offset=0x18 Size=0x1
    char CpuID;// Offset=0x19 Size=0x1
    union // Size=0x2
    {
        unsigned short CpuStep;// Offset=0x1a Size=0x2
        struct // Size=0x2
        {
            unsigned char CpuStepping;// Offset=0x1a Size=0x1
            unsigned char CpuModel;// Offset=0x1b Size=0x1
        };
    };
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x1c Size=0x320
    unsigned long KernelReserved[16];// Offset=0x33c Size=0x40
    unsigned long HalReserved[16];// Offset=0x37c Size=0x40
    unsigned long CFlushSize;// Offset=0x3bc Size=0x4
    unsigned char CoresPerPhysicalProcessor;// Offset=0x3c0 Size=0x1
    unsigned char LogicalProcessorsPerCore;// Offset=0x3c1 Size=0x1
    unsigned char PrcbPad0[2];// Offset=0x3c2 Size=0x2
    unsigned long MHz;// Offset=0x3c4 Size=0x4
    unsigned char PrcbPad1[80];// Offset=0x3c8 Size=0x50
    struct _KSPIN_LOCK_QUEUE LockQueue[49];// Offset=0x418 Size=0x188
    struct _KTHREAD *NpxThread;// Offset=0x5a0 Size=0x4
    unsigned long InterruptCount;// Offset=0x5a4 Size=0x4
    unsigned long KernelTime;// Offset=0x5a8 Size=0x4
    unsigned long UserTime;// Offset=0x5ac Size=0x4
    unsigned long DpcTime;// Offset=0x5b0 Size=0x4
    unsigned long DpcTimeCount;// Offset=0x5b4 Size=0x4
    unsigned long InterruptTime;// Offset=0x5b8 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x5bc Size=0x4
    unsigned long PageColor;// Offset=0x5c0 Size=0x4
    unsigned char SkipTick;// Offset=0x5c4 Size=0x1
    unsigned char DebuggerSavedIRQL;// Offset=0x5c5 Size=0x1
    unsigned char NodeColor;// Offset=0x5c6 Size=0x1
    unsigned char PollSlot;// Offset=0x5c7 Size=0x1
    unsigned long NodeShiftedColor;// Offset=0x5c8 Size=0x4
    struct _KNODE *ParentNode;// Offset=0x5cc Size=0x4
    unsigned long MultiThreadProcessorSet;// Offset=0x5d0 Size=0x4
    struct _KPRCB *MultiThreadSetMaster;// Offset=0x5d4 Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x5d8 Size=0x4
    unsigned long DpcTimeLimit;// Offset=0x5dc Size=0x4
    unsigned long CcFastReadNoWait;// Offset=0x5e0 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x5e4 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x5e8 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x5ec Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x5f0 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x5f4 Size=0x4
    long volatile MmSpinLockOrdering;// Offset=0x5f8 Size=0x4
    long volatile IoReadOperationCount;// Offset=0x5fc Size=0x4
    long volatile IoWriteOperationCount;// Offset=0x600 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0x604 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0x608 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0x610 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0x618 Size=0x8
    unsigned long CcFastMdlReadNoWait;// Offset=0x620 Size=0x4
    unsigned long CcFastMdlReadWait;// Offset=0x624 Size=0x4
    unsigned long CcFastMdlReadNotPossible;// Offset=0x628 Size=0x4
    unsigned long CcMapDataNoWait;// Offset=0x62c Size=0x4
    unsigned long CcMapDataWait;// Offset=0x630 Size=0x4
    unsigned long CcPinMappedDataCount;// Offset=0x634 Size=0x4
    unsigned long CcPinReadNoWait;// Offset=0x638 Size=0x4
    unsigned long CcPinReadWait;// Offset=0x63c Size=0x4
    unsigned long CcMdlReadNoWait;// Offset=0x640 Size=0x4
    unsigned long CcMdlReadWait;// Offset=0x644 Size=0x4
    unsigned long CcLazyWriteHotSpots;// Offset=0x648 Size=0x4
    unsigned long CcLazyWriteIos;// Offset=0x64c Size=0x4
    unsigned long CcLazyWritePages;// Offset=0x650 Size=0x4
    unsigned long CcDataFlushes;// Offset=0x654 Size=0x4
    unsigned long CcDataPages;// Offset=0x658 Size=0x4
    unsigned long CcLostDelayedWrites;// Offset=0x65c Size=0x4
    unsigned long CcFastReadResourceMiss;// Offset=0x660 Size=0x4
    unsigned long CcCopyReadWaitMiss;// Offset=0x664 Size=0x4
    unsigned long CcFastMdlReadResourceMiss;// Offset=0x668 Size=0x4
    unsigned long CcMapDataNoWaitMiss;// Offset=0x66c Size=0x4
    unsigned long CcMapDataWaitMiss;// Offset=0x670 Size=0x4
    unsigned long CcPinReadNoWaitMiss;// Offset=0x674 Size=0x4
    unsigned long CcPinReadWaitMiss;// Offset=0x678 Size=0x4
    unsigned long CcMdlReadNoWaitMiss;// Offset=0x67c Size=0x4
    unsigned long CcMdlReadWaitMiss;// Offset=0x680 Size=0x4
    unsigned long CcReadAheadIos;// Offset=0x684 Size=0x4
    unsigned long KeAlignmentFixupCount;// Offset=0x688 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x68c Size=0x4
    unsigned long KeSystemCalls;// Offset=0x690 Size=0x4
    unsigned long PrcbPad2[3];// Offset=0x694 Size=0xc
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x6a0 Size=0x80
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];// Offset=0x720 Size=0x900
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];// Offset=0x1020 Size=0x900
    unsigned long volatile PacketBarrier;// Offset=0x1920 Size=0x4
    long volatile ReverseStall;// Offset=0x1924 Size=0x4
    void *IpiFrame;// Offset=0x1928 Size=0x4
    unsigned char PrcbPad3[52];// Offset=0x192c Size=0x34
    void volatile *CurrentPacket[3];// Offset=0x1960 Size=0xc
    unsigned long volatile TargetSet;// Offset=0x196c Size=0x4
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x1970 Size=0x4
    unsigned long volatile IpiFrozen;// Offset=0x1974 Size=0x4
    unsigned char PrcbPad4[40];// Offset=0x1978 Size=0x28
    unsigned long volatile RequestSummary;// Offset=0x19a0 Size=0x4
    struct _KPRCB volatile *SignalDone;// Offset=0x19a4 Size=0x4
    unsigned char PrcbPad5[56];// Offset=0x19a8 Size=0x38
    struct _KDPC_DATA DpcData[2];// Offset=0x19e0 Size=0x28
    void *DpcStack;// Offset=0x1a08 Size=0x4
    long MaximumDpcQueueDepth;// Offset=0x1a0c Size=0x4
    unsigned long DpcRequestRate;// Offset=0x1a10 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x1a14 Size=0x4
    unsigned char volatile DpcInterruptRequested;// Offset=0x1a18 Size=0x1
    unsigned char volatile DpcThreadRequested;// Offset=0x1a19 Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x1a1a Size=0x1
    unsigned char volatile DpcThreadActive;// Offset=0x1a1b Size=0x1
    unsigned long PrcbLock;// Offset=0x1a1c Size=0x4
    unsigned long DpcLastCount;// Offset=0x1a20 Size=0x4
    unsigned long volatile TimerHand;// Offset=0x1a24 Size=0x4
    unsigned long volatile TimerRequest;// Offset=0x1a28 Size=0x4
    void *PrcbPad41;// Offset=0x1a2c Size=0x4
    struct _KEVENT DpcEvent;// Offset=0x1a30 Size=0x10
    unsigned char ThreadDpcEnable;// Offset=0x1a40 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x1a41 Size=0x1
    unsigned char PrcbPad50;// Offset=0x1a42 Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x1a43 Size=0x1
    long DpcSetEventRequest;// Offset=0x1a44 Size=0x4
    long Sleeping;// Offset=0x1a48 Size=0x4
    unsigned long PeriodicCount;// Offset=0x1a4c Size=0x4
    unsigned long PeriodicBias;// Offset=0x1a50 Size=0x4
    unsigned char PrcbPad51[6];// Offset=0x1a54 Size=0x6
    unsigned char __align0[2];// Offset=0x1a5a Size=0x2
    long TickOffset;// Offset=0x1a5c Size=0x4
    struct _KDPC_VISTA CallDpc;// Offset=0x1a60 Size=0x20
    long ClockKeepAlive;// Offset=0x1a80 Size=0x4
    unsigned char ClockCheckSlot;// Offset=0x1a84 Size=0x1
    unsigned char ClockPollCycle;// Offset=0x1a85 Size=0x1
    unsigned char PrcbPad6[2];// Offset=0x1a86 Size=0x2
    long DpcWatchdogPeriod;// Offset=0x1a88 Size=0x4
    long DpcWatchdogCount;// Offset=0x1a8c Size=0x4
    long ThreadWatchdogPeriod;// Offset=0x1a90 Size=0x4
    long ThreadWatchdogCount;// Offset=0x1a94 Size=0x4
    unsigned long PrcbPad70[2];// Offset=0x1a98 Size=0x8
    struct _LIST_ENTRY WaitListHead;// Offset=0x1aa0 Size=0x8
    unsigned long WaitLock;// Offset=0x1aa8 Size=0x4
    unsigned long ReadySummary;// Offset=0x1aac Size=0x4
    unsigned long QueueIndex;// Offset=0x1ab0 Size=0x4
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0x1ab4 Size=0x4
    ULONG64 StartCycles;// Offset=0x1ab8 Size=0x8
    ULONG64 CycleTime;// Offset=0x1ac0 Size=0x8
    ULONG64 PrcbPad71[3];// Offset=0x1ac8 Size=0x18
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x1ae0 Size=0x100
    void *ChainedInterruptList;// Offset=0x1be0 Size=0x4
    long LookasideIrpFloat;// Offset=0x1be4 Size=0x4
    long volatile MmPageFaultCount;// Offset=0x1be8 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0x1bec Size=0x4
    long volatile MmTransitionCount;// Offset=0x1bf0 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0x1bf4 Size=0x4
    long volatile MmDemandZeroCount;// Offset=0x1bf8 Size=0x4
    long volatile MmPageReadCount;// Offset=0x1bfc Size=0x4
    long volatile MmPageReadIoCount;// Offset=0x1c00 Size=0x4
    long volatile MmCacheReadCount;// Offset=0x1c04 Size=0x4
    long volatile MmCacheIoCount;// Offset=0x1c08 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0x1c0c Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0x1c10 Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0x1c14 Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0x1c18 Size=0x4
    unsigned long volatile CachedCommit;// Offset=0x1c1c Size=0x4
    unsigned long volatile CachedResidentAvailable;// Offset=0x1c20 Size=0x4
    void *HyperPte;// Offset=0x1c24 Size=0x4
    unsigned char CpuVendor;// Offset=0x1c28 Size=0x1
    unsigned char PrcbPad8[3];// Offset=0x1c29 Size=0x3
    unsigned char VendorString[13];// Offset=0x1c2c Size=0xd
    unsigned char InitialApicId;// Offset=0x1c39 Size=0x1
    unsigned char LogicalProcessorsPerPhysicalProcessor;// Offset=0x1c3a Size=0x1
    unsigned char PrcbPad9[5];// Offset=0x1c3b Size=0x5
    unsigned long FeatureBits;// Offset=0x1c40 Size=0x4
    unsigned char __align1[4];// Offset=0x1c44 Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x1c48 Size=0x8
    ULONG64 volatile IsrTime;// Offset=0x1c50 Size=0x8
    ULONG64 SpareField1;// Offset=0x1c58 Size=0x8
    struct _FX_SAVE_AREA NpxSaveArea;// Offset=0x1c60 Size=0x210
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0x1e70 Size=0xc8
    struct _KDPC_VISTA DpcWatchdogDpc;// Offset=0x1f38 Size=0x20
    struct _KTIMER DpcWatchdogTimer;// Offset=0x1f58 Size=0x28
    void *WheaInfo;// Offset=0x1f80 Size=0x4
    void *EtwSupport;// Offset=0x1f84 Size=0x4
    union _SLIST_HEADER InterruptObjectPool;// Offset=0x1f88 Size=0x8
    union _SLIST_HEADER HypercallPageList;// Offset=0x1f90 Size=0x8
    void *HypercallPageVirtual;// Offset=0x1f98 Size=0x4
    void *VirtualApicAssist;// Offset=0x1f9c Size=0x4
    ULONG64 *StatisticsPage;// Offset=0x1fa0 Size=0x4
    void *RateControl;// Offset=0x1fa4 Size=0x4
    struct _CACHE_DESCRIPTOR Cache[5];// Offset=0x1fa8 Size=0x3c
    unsigned long CacheCount;// Offset=0x1fe4 Size=0x4
    unsigned long CacheProcessorMask[5];// Offset=0x1fe8 Size=0x14
    unsigned long PackageProcessorSet;// Offset=0x1ffc Size=0x4
    unsigned long CoreProcessorSet;// Offset=0x2000 Size=0x4
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0xa8
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x94
    struct _EX_PUSH_LOCK Lock;// Offset=0x94 Size=0x4
    struct _DEVICE_MAP *DeviceMap;// Offset=0x98 Size=0x4
    unsigned long SessionId;// Offset=0x9c Size=0x4
    void *NamespaceEntry;// Offset=0xa0 Size=0x4
    unsigned long Flags;// Offset=0xa4 Size=0x4
} *POBJECT_DIRECTORY;

#endif //                  Windows Vista x32



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows 7 x32 internal kernel definitions                         //
#if (NTDDI_VERSION >= NTDDI_WIN7) && (NTDDI_VERSION <= NTDDI_WIN7) && defined(_X86_)

typedef struct _MM_SESSION_SPACE// Size=0x2000
{
    long volatile ReferenceCount;// Offset=0x0 Size=0x4
    union {
        ULONG LongFlags;// Offset=0x0 Size=0x4
        struct _MM_SESSION_SPACE_FLAGS Flags;// Offset=0x0 Size=0x4
    } u;
    unsigned long SessionId;// Offset=0x8 Size=0x4
    // ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x4
{
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long Locked:1;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long Waiting:1;// Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long Waking:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long MultipleShared:1;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Shared:28;// Offset=0x0 Size=0x4 BitOffset=0x4 BitSize=0x1c
        };
        unsigned long Value;// Offset=0x0 Size=0x4
        void *Ptr;// Offset=0x0 Size=0x4
    };
};

typedef struct _EX_FAST_REF// Size=0x4
{
    union // Size=0x4
    {
        void *Object;// Offset=0x0 Size=0x4
        unsigned long RefCnt:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long Value;// Offset=0x0 Size=0x4
    };
} EX_FAST_REF, *PEX_FAST_REF;

typedef struct _MMADDRESS_NODE // Size=0x14
{ 
    union {// Offset=0x0 Size=0x4
        long Balance:2;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x4
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x4 Size=0x4
    struct _MMADDRESS_NODE *RightChild;// Offset=0x8 Size=0x4
    unsigned long StartingVpn;// Offset=0xc Size=0x4
    unsigned long EndingVpn;// Offset=0x10 Size=0x4
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE// Size=0x20
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x14
    unsigned long DepthOfTree:5;// Offset=0x14 Size=0x4 BitOffset=0x0 BitSize=0x5
    unsigned long Unused:3;// Offset=0x14 Size=0x4 BitOffset=0x5 BitSize=0x3
    unsigned long NumberGenericTableElements:24;// Offset=0x14 Size=0x4 BitOffset=0x8 BitSize=0x18
    void *NodeHint;// Offset=0x18 Size=0x4
    void *NodeFreeHint;// Offset=0x1c Size=0x4
};

struct _KAFFINITY_EX// Size=0xc
{
    unsigned short Count;// Offset=0x0 Size=0x2
    unsigned short Size;// Offset=0x2 Size=0x2
    unsigned long Reserved;// Offset=0x4 Size=0x4
    unsigned long Bitmap[1];// Offset=0x8 Size=0x4
};

union _KSTACK_COUNT// Size=0x4
{
    long volatile Value;// Offset=0x0 Size=0x4
    struct // Size=0x4
    {
        unsigned long volatile State:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long StackCount:29;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1d
    };
};

struct _KPROCESS// Size=0x98
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    struct _LIST_ENTRY ProfileListHead;// Offset=0x10 Size=0x8
    unsigned long DirectoryTableBase;// Offset=0x18 Size=0x4
    struct _KGDTENTRY LdtDescriptor;// Offset=0x1c Size=0x8
    struct _KIDTENTRY Int21Descriptor;// Offset=0x24 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x2c Size=0x8
    unsigned long ProcessLock;// Offset=0x34 Size=0x4
    struct _KAFFINITY_EX Affinity;// Offset=0x38 Size=0xc
    struct _LIST_ENTRY ReadyListHead;// Offset=0x44 Size=0x8
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x4c Size=0x4
    struct _KAFFINITY_EX volatile ActiveProcessors;// Offset=0x50 Size=0xc
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long volatile AutoAlignment:1;// Offset=0x5c Size=0x4 BitOffset=0x0 BitSize=0x1
            long volatile DisableBoost:1;// Offset=0x5c Size=0x4 BitOffset=0x1 BitSize=0x1
            long volatile DisableQuantum:1;// Offset=0x5c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long volatile ActiveGroupsMask:1;// Offset=0x5c Size=0x4 BitOffset=0x3 BitSize=0x1
            long volatile ReservedFlags:28;// Offset=0x5c Size=0x4 BitOffset=0x4 BitSize=0x1c
        };
        long volatile ProcessFlags;// Offset=0x5c Size=0x4
    };
    char BasePriority;// Offset=0x60 Size=0x1
    char QuantumReset;// Offset=0x61 Size=0x1
    unsigned char Visited;// Offset=0x62 Size=0x1
    unsigned char Unused3;// Offset=0x63 Size=0x1
    unsigned long ThreadSeed[1];// Offset=0x64 Size=0x4
    unsigned short IdealNode[1];// Offset=0x68 Size=0x2
    unsigned short IdealGlobalNode;// Offset=0x6a Size=0x2
    struct _KEXECUTE_OPTIONS Flags;// Offset=0x6c Size=0x1
    unsigned char Unused1;// Offset=0x6d Size=0x1
    unsigned short IopmOffset;// Offset=0x6e Size=0x2
    unsigned long Unused4;// Offset=0x70 Size=0x4
    union _KSTACK_COUNT StackCount;// Offset=0x74 Size=0x4
    struct _LIST_ENTRY ProcessListEntry;// Offset=0x78 Size=0x8
    ULONG64 volatile CycleTime;// Offset=0x80 Size=0x8
    unsigned long KernelTime;// Offset=0x88 Size=0x4
    unsigned long UserTime;// Offset=0x8c Size=0x4
    void *VdmTrapcHandler;// Offset=0x90 Size=0x4
};

struct _MMSUPPORT// Size=0x6c
{
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x0 Size=0x4
    struct _KGATE *ExitGate;// Offset=0x4 Size=0x4
    void *AccessLog;// Offset=0x8 Size=0x4
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0xc Size=0x8
    unsigned long AgeDistribution[7];// Offset=0x14 Size=0x1c
    unsigned long MinimumWorkingSetSize;// Offset=0x30 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x34 Size=0x4
    unsigned long WorkingSetPrivateSize;// Offset=0x38 Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x3c Size=0x4
    unsigned long ChargedWslePages;// Offset=0x40 Size=0x4
    unsigned long ActualWslePages;// Offset=0x44 Size=0x4
    unsigned long WorkingSetSizeOverhead;// Offset=0x48 Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x4c Size=0x4
    unsigned long HardFaultCount;// Offset=0x50 Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x54 Size=0x4
    unsigned short NextPageColor;// Offset=0x58 Size=0x2
    unsigned short LastTrimStamp;// Offset=0x5a Size=0x2
    unsigned long PageFaultCount;// Offset=0x5c Size=0x4
    unsigned long RepurposeCount;// Offset=0x60 Size=0x4
    unsigned long Spare[1];// Offset=0x64 Size=0x4
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x68 Size=0x4
};

struct _ALPC_PROCESS_CONTEXT// Size=0x10
{
    struct _EX_PUSH_LOCK Lock;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY ViewListHead;// Offset=0x4 Size=0x8
    unsigned long volatile PagedPoolQuotaCache;// Offset=0xc Size=0x4
};

typedef struct _EPROCESS// Size=0x2c0
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0x98
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0x98 Size=0x4
    unsigned char __align0[4];// Offset=0x9c Size=0x4
    union _LARGE_INTEGER CreateTime;// Offset=0xa0 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0xa8 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0xb0 Size=0x4
    void *UniqueProcessId;// Offset=0xb4 Size=0x4
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0xb8 Size=0x8
    unsigned long ProcessQuotaUsage[2];// Offset=0xc0 Size=0x8
    unsigned long ProcessQuotaPeak[2];// Offset=0xc8 Size=0x8
    unsigned long volatile CommitCharge;// Offset=0xd0 Size=0x4
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0xd4 Size=0x4
    struct _PS_CPU_QUOTA_BLOCK *CpuQuotaBlock;// Offset=0xd8 Size=0x4
    unsigned long PeakVirtualSize;// Offset=0xdc Size=0x4
    unsigned long VirtualSize;// Offset=0xe0 Size=0x4
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0xe4 Size=0x8
    void *DebugPort;// Offset=0xec Size=0x4
    union // Size=0x4
    {
        void *ExceptionPortData;// Offset=0xf0 Size=0x4
        unsigned long ExceptionPortValue;// Offset=0xf0 Size=0x4
        unsigned long ExceptionPortState:3;// Offset=0xf0 Size=0x4 BitOffset=0x0 BitSize=0x3
    };
    struct _HANDLE_TABLE *ObjectTable;// Offset=0xf4 Size=0x4
    struct _EX_FAST_REF Token;// Offset=0xf8 Size=0x4
    unsigned long WorkingSetPage;// Offset=0xfc Size=0x4
    struct _EX_PUSH_LOCK AddressCreationLock;// Offset=0x100 Size=0x4
    struct _ETHREAD *RotateInProgress;// Offset=0x104 Size=0x4
    struct _ETHREAD *ForkInProgress;// Offset=0x108 Size=0x4
    unsigned long HardwareTrigger;// Offset=0x10c Size=0x4
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0x110 Size=0x4
    void *CloneRoot;// Offset=0x114 Size=0x4
    unsigned long volatile NumberOfPrivatePages;// Offset=0x118 Size=0x4
    unsigned long volatile NumberOfLockedPages;// Offset=0x11c Size=0x4
    void *Win32Process;// Offset=0x120 Size=0x4
    struct _EJOB volatile *Job;// Offset=0x124 Size=0x4
    void *SectionObject;// Offset=0x128 Size=0x4
    void *SectionBaseAddress;// Offset=0x12c Size=0x4
    unsigned long Cookie;// Offset=0x130 Size=0x4
    unsigned long Spare8;// Offset=0x134 Size=0x4
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x138 Size=0x4
    void *Win32WindowStation;// Offset=0x13c Size=0x4
    void *InheritedFromUniqueProcessId;// Offset=0x140 Size=0x4
    void *LdtInformation;// Offset=0x144 Size=0x4
    void *VdmObjects;// Offset=0x148 Size=0x4
    unsigned long ConsoleHostProcess;// Offset=0x14c Size=0x4
    void *DeviceMap;// Offset=0x150 Size=0x4
    void *EtwDataSource;// Offset=0x154 Size=0x4
    void *FreeTebHint;// Offset=0x158 Size=0x4
    unsigned char __align1[4];// Offset=0x15c Size=0x4
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x160 Size=0x4
        ULONG64 Filler;// Offset=0x160 Size=0x8
    };
    void *Session;// Offset=0x168 Size=0x4
    unsigned char ImageFileName[15];// Offset=0x16c Size=0xf
    unsigned char PriorityClass;// Offset=0x17b Size=0x1
    struct _LIST_ENTRY JobLinks;// Offset=0x17c Size=0x8
    void *LockedPagesList;// Offset=0x184 Size=0x4
    struct _LIST_ENTRY ThreadListHead;// Offset=0x188 Size=0x8
    void *SecurityPort;// Offset=0x190 Size=0x4
    void *PaeTop;// Offset=0x194 Size=0x4
    unsigned long volatile ActiveThreads;// Offset=0x198 Size=0x4
    unsigned long ImagePathHash;// Offset=0x19c Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x1a0 Size=0x4
    long LastThreadExitStatus;// Offset=0x1a4 Size=0x4
    struct _PEB *Peb;// Offset=0x1a8 Size=0x4
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x1ac Size=0x4
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x1b0 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x1b8 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x1c0 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x1c8 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x1d0 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x1d8 Size=0x8
    unsigned long CommitChargeLimit;// Offset=0x1e0 Size=0x4
    unsigned long volatile CommitChargePeak;// Offset=0x1e4 Size=0x4
    void *AweInfo;// Offset=0x1e8 Size=0x4
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x1ec Size=0x4
    struct _MMSUPPORT Vm;// Offset=0x1f0 Size=0x6c
    struct _LIST_ENTRY MmProcessLinks;// Offset=0x25c Size=0x8
    void *HighestUserAddress;// Offset=0x264 Size=0x4
    unsigned long ModifiedPageCount;// Offset=0x268 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags2;// Offset=0x26c Size=0x4
        struct // Size=0x4
        {
            unsigned long JobNotReallyActive:1;// Offset=0x26c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long AccountingFolded:1;// Offset=0x26c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long NewProcessReported:1;// Offset=0x26c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ExitProcessReported:1;// Offset=0x26c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long ReportCommitChanges:1;// Offset=0x26c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long LastReportMemory:1;// Offset=0x26c Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long ReportPhysicalPageChanges:1;// Offset=0x26c Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long HandleTableRundown:1;// Offset=0x26c Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long NeedsHandleRundown:1;// Offset=0x26c Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long RefTraceEnabled:1;// Offset=0x26c Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long NumaAware:1;// Offset=0x26c Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long ProtectedProcess:1;// Offset=0x26c Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long DefaultPagePriority:3;// Offset=0x26c Size=0x4 BitOffset=0xc BitSize=0x3
            unsigned long PrimaryTokenFrozen:1;// Offset=0x26c Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessVerifierTarget:1;// Offset=0x26c Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long StackRandomizationDisabled:1;// Offset=0x26c Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long AffinityPermanent:1;// Offset=0x26c Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long AffinityUpdateEnable:1;// Offset=0x26c Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long PropagateNode:1;// Offset=0x26c Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long ExplicitAffinity:1;// Offset=0x26c Size=0x4 BitOffset=0x15 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x270 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x270 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x270 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x270 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x270 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x270 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x270 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x270 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x270 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x270 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x270 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x270 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x270 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x270 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long DeprioritizeViews:1;// Offset=0x270 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x270 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x270 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x270 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x270 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x270 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x270 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x270 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x270 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x270 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x270 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long CrossSessionCreate:1;// Offset=0x270 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long ProcessInserted:1;// Offset=0x270 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x270 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long ProcessSelfDelete:1;// Offset=0x270 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long SetTimerResolutionLink:1;// Offset=0x270 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x274 Size=0x4
    struct _MM_AVL_TABLE VadRoot;// Offset=0x278 Size=0x20
    struct _ALPC_PROCESS_CONTEXT AlpcContext;// Offset=0x298 Size=0x10
    struct _LIST_ENTRY TimerResolutionLink;// Offset=0x2a8 Size=0x8
    unsigned long RequestedTimerResolution;// Offset=0x2b0 Size=0x4
    unsigned long ActiveThreadsHighWatermark;// Offset=0x2b4 Size=0x4
    unsigned long SmallestTimerResolution;// Offset=0x2b8 Size=0x4
    struct _PO_DIAG_STACK_RECORD *TimerResolutionStackRecord;// Offset=0x2bc Size=0x4
} EPROCESS;

union _KWAIT_STATUS_REGISTER// Size=0x1
{
    unsigned char Flags;// Offset=0x0 Size=0x1
    struct // Size=0x1
    {
        unsigned char State:2;// Offset=0x0 Size=0x1 BitOffset=0x0 BitSize=0x2
        unsigned char Affinity:1;// Offset=0x0 Size=0x1 BitOffset=0x2 BitSize=0x1
        unsigned char Priority:1;// Offset=0x0 Size=0x1 BitOffset=0x3 BitSize=0x1
        unsigned char Apc:1;// Offset=0x0 Size=0x1 BitOffset=0x4 BitSize=0x1
        unsigned char UserApc:1;// Offset=0x0 Size=0x1 BitOffset=0x5 BitSize=0x1
        unsigned char Alert:1;// Offset=0x0 Size=0x1 BitOffset=0x6 BitSize=0x1
        unsigned char Unused:1;// Offset=0x0 Size=0x1 BitOffset=0x7 BitSize=0x1
    };
};

struct _CONTROL_AREA// Size=0x50
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY DereferenceList;// Offset=0x4 Size=0x8
    unsigned long NumberOfSectionReferences;// Offset=0xc Size=0x4
    unsigned long NumberOfPfnReferences;// Offset=0x10 Size=0x4
    unsigned long NumberOfMappedViews;// Offset=0x14 Size=0x4
    unsigned long NumberOfUserReferences;// Offset=0x18 Size=0x4
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    unsigned long FlushInProgressCount;// Offset=0x20 Size=0x4
    struct _EX_FAST_REF FilePointer;// Offset=0x24 Size=0x4
    long volatile ControlAreaLock;// Offset=0x28 Size=0x4
    union // Size=0x4
    {
        unsigned long ModifiedWriteCount;// Offset=0x2c Size=0x4
        unsigned long StartingFrame;// Offset=0x2c Size=0x4
    };
    struct _MI_SECTION_CREATION_GATE *WaitingForDeletion;// Offset=0x30 Size=0x4
    union {
        struct // Size=0xc
        {
            union // Size=0x4
            {
                unsigned long NumberOfSystemCacheViews;// Offset=0x0 Size=0x4
                unsigned long ImageRelocationStartBit;// Offset=0x0 Size=0x4
            };
            union // Size=0x4
            {
                long volatile WritableUserReferences;// Offset=0x4 Size=0x4
                struct // Size=0x4
                {
                    unsigned long ImageRelocationSizeIn64k:16;// Offset=0x4 Size=0x4 BitOffset=0x0 BitSize=0x10
                    unsigned long Unused:14;// Offset=0x4 Size=0x4 BitOffset=0x10 BitSize=0xe
                    unsigned long BitMap64:1;// Offset=0x4 Size=0x4 BitOffset=0x1e BitSize=0x1
                    unsigned long ImageActive:1;// Offset=0x4 Size=0x4 BitOffset=0x1f BitSize=0x1
                };
            };
            union // Size=0x4
            {
                struct _MM_SUBSECTION_AVL_TABLE *SubsectionRoot;// Offset=0x8 Size=0x4
                struct _MI_IMAGE_SECURITY_REFERENCE *SeImageStub;// Offset=0x8 Size=0x4
            };
        } e2;    
    } u2;// Offset=0x34 Size=0xc
    LONG64 volatile LockedPages;// Offset=0x40 Size=0x8
    struct _LIST_ENTRY ViewList;// Offset=0x48 Size=0x8
};

typedef struct _SEGMENT // Size=0x30/0x38 NOPAE/PAE
{
    struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
    // ...
} *PSEGMENT;

typedef struct _SECTION {
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

typedef struct _KAPC_STATE// Size=0x18
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x10
    struct _KPROCESS *Process;// Offset=0x10 Size=0x4
    unsigned char KernelApcInProgress;// Offset=0x14 Size=0x1
    unsigned char KernelApcPending;// Offset=0x15 Size=0x1
    unsigned char UserApcPending;// Offset=0x16 Size=0x1
} KAPC_STATE, *PKAPC_STATE;

struct _KTHREAD// Size=0x200
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x10
    ULONG64 volatile CycleTime;// Offset=0x10 Size=0x8
    unsigned long volatile HighCycleTime;// Offset=0x18 Size=0x4
    unsigned char __align0[4];// Offset=0x1c Size=0x4
    ULONG64 QuantumTarget;// Offset=0x20 Size=0x8
    void *InitialStack;// Offset=0x28 Size=0x4
    void volatile *StackLimit;// Offset=0x2c Size=0x4
    void *KernelStack;// Offset=0x30 Size=0x4
    unsigned long ThreadLock;// Offset=0x34 Size=0x4
    union _KWAIT_STATUS_REGISTER WaitRegister;// Offset=0x38 Size=0x1
    unsigned char volatile Running;// Offset=0x39 Size=0x1
    unsigned char Alerted[2];// Offset=0x3a Size=0x2
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long KernelStackResident:1;// Offset=0x3c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ReadyTransition:1;// Offset=0x3c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessReadyQueue:1;// Offset=0x3c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long WaitNext:1;// Offset=0x3c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemAffinityActive:1;// Offset=0x3c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long Alertable:1;// Offset=0x3c Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long GdiFlushActive:1;// Offset=0x3c Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long UserStackWalkActive:1;// Offset=0x3c Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ApcInterruptRequest:1;// Offset=0x3c Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long ForceDeferSchedule:1;// Offset=0x3c Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long QuantumEndMigrate:1;// Offset=0x3c Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long UmsDirectedSwitchEnable:1;// Offset=0x3c Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long TimerActive:1;// Offset=0x3c Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x3c Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long Reserved:18;// Offset=0x3c Size=0x4 BitOffset=0xe BitSize=0x12
        };
        long MiscFlags;// Offset=0x3c Size=0x4
    };
    union // Size=0x18
    {
        struct _KAPC_STATE ApcState;// Offset=0x40 Size=0x18
        struct // Size=0x18
        {
            unsigned char ApcStateFill[23];// Offset=0x40 Size=0x17
            char Priority;// Offset=0x57 Size=0x1
        };
    };
    unsigned long volatile NextProcessor;// Offset=0x58 Size=0x4
    unsigned long volatile DeferredProcessor;// Offset=0x5c Size=0x4
    unsigned long ApcQueueLock;// Offset=0x60 Size=0x4
    unsigned long ContextSwitches;// Offset=0x64 Size=0x4
    unsigned char volatile State;// Offset=0x68 Size=0x1
    char NpxState;// Offset=0x69 Size=0x1
    unsigned char WaitIrql;// Offset=0x6a Size=0x1
    char WaitMode;// Offset=0x6b Size=0x1
    long volatile WaitStatus;// Offset=0x6c Size=0x4
    struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x70 Size=0x4
    union // Size=0x8
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x74 Size=0x8
        struct // Size=0x8
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x74 Size=0x4
            unsigned char __align1[4];// Offset=0x78 Size=0x4
        };
    };
    struct _KQUEUE volatile *Queue;// Offset=0x7c Size=0x4
    unsigned long WaitTime;// Offset=0x80 Size=0x4
    union // Size=0x4
    {
        struct // Size=0x4
        {
            short KernelApcDisable;// Offset=0x84 Size=0x2
            short SpecialApcDisable;// Offset=0x86 Size=0x2
        };
        unsigned long CombinedApcDisable;// Offset=0x84 Size=0x4
    };
    void *Teb;// Offset=0x88 Size=0x4
    unsigned char __align2[4];// Offset=0x8c Size=0x4
    struct _KTIMER Timer;// Offset=0x90 Size=0x28
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long volatile AutoAlignment:1;// Offset=0xb8 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long volatile DisableBoost:1;// Offset=0xb8 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long volatile EtwStackTraceApc1Inserted:1;// Offset=0xb8 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long volatile EtwStackTraceApc2Inserted:1;// Offset=0xb8 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long volatile CalloutActive:1;// Offset=0xb8 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long volatile ApcQueueable:1;// Offset=0xb8 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long volatile EnableStackSwap:1;// Offset=0xb8 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long volatile GuiThread:1;// Offset=0xb8 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long volatile UmsPerformingSyscall:1;// Offset=0xb8 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long volatile VdmSafe:1;// Offset=0xb8 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long volatile UmsDispatched:1;// Offset=0xb8 Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long volatile ReservedFlags:21;// Offset=0xb8 Size=0x4 BitOffset=0xb BitSize=0x15
        };
        long volatile ThreadFlags;// Offset=0xb8 Size=0x4
    };
    void *ServiceTable;// Offset=0xbc Size=0x4
    struct _KWAIT_BLOCK WaitBlock[4];// Offset=0xc0 Size=0x60
    struct _LIST_ENTRY QueueListEntry;// Offset=0x120 Size=0x8
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x128 Size=0x4
    void *FirstArgument;// Offset=0x12c Size=0x4
    union // Size=0x4
    {
        void *CallbackStack;// Offset=0x130 Size=0x4
        unsigned long CallbackDepth;// Offset=0x130 Size=0x4
    };
    unsigned char ApcStateIndex;// Offset=0x134 Size=0x1
    char BasePriority;// Offset=0x135 Size=0x1
    union // Size=0x1
    {
        char PriorityDecrement;// Offset=0x136 Size=0x1
        struct // Size=0x1
        {
            unsigned char ForegroundBoost:4;// Offset=0x136 Size=0x1 BitOffset=0x0 BitSize=0x4
            unsigned char UnusualBoost:4;// Offset=0x136 Size=0x1 BitOffset=0x4 BitSize=0x4
        };
    };
    unsigned char Preempted;// Offset=0x137 Size=0x1
    unsigned char AdjustReason;// Offset=0x138 Size=0x1
    char AdjustIncrement;// Offset=0x139 Size=0x1
    char PreviousMode;// Offset=0x13a Size=0x1
    char Saturation;// Offset=0x13b Size=0x1
    unsigned long SystemCallNumber;// Offset=0x13c Size=0x4
    unsigned long FreezeCount;// Offset=0x140 Size=0x4
    struct _GROUP_AFFINITY volatile UserAffinity;// Offset=0x144 Size=0xc
    struct _KPROCESS *Process;// Offset=0x150 Size=0x4
    struct _GROUP_AFFINITY volatile Affinity;// Offset=0x154 Size=0xc
    unsigned long IdealProcessor;// Offset=0x160 Size=0x4
    unsigned long UserIdealProcessor;// Offset=0x164 Size=0x4
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x168 Size=0x8
    union // Size=0x18
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x170 Size=0x18
        struct // Size=0x18
        {
            unsigned char SavedApcStateFill[23];// Offset=0x170 Size=0x17
            unsigned char WaitReason;// Offset=0x187 Size=0x1
        };
    };
    char SuspendCount;// Offset=0x188 Size=0x1
    char Spare1;// Offset=0x189 Size=0x1
    unsigned char OtherPlatformFill;// Offset=0x18a Size=0x1
    unsigned char __align3[1];// Offset=0x18b Size=0x1
    void volatile *Win32Thread;// Offset=0x18c Size=0x4
    void *StackBase;// Offset=0x190 Size=0x4
    union // Size=0x30
    {
        struct _KAPC SuspendApc;// Offset=0x194 Size=0x30
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x194 Size=0x1
            unsigned char ResourceIndex;// Offset=0x195 Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x194 Size=0x3
            unsigned char QuantumReset;// Offset=0x197 Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x194 Size=0x4
            unsigned long KernelTime;// Offset=0x198 Size=0x4
        };
        struct // Size=0x28
        {
            unsigned char SuspendApcFill3[36];// Offset=0x194 Size=0x24
            struct _KPRCB volatile *WaitPrcb;// Offset=0x1b8 Size=0x4
        };
        struct // Size=0x2c
        {
            unsigned char SuspendApcFill4[40];// Offset=0x194 Size=0x28
            void *LegoData;// Offset=0x1bc Size=0x4
        };
        struct // Size=0x30
        {
            unsigned char SuspendApcFill5[47];// Offset=0x194 Size=0x2f
            unsigned char LargeStack;// Offset=0x1c3 Size=0x1
        };
    };
    unsigned long UserTime;// Offset=0x1c4 Size=0x4
    union // Size=0x14
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x1c8 Size=0x14
        unsigned char SuspendSemaphorefill[20];// Offset=0x1c8 Size=0x14
    };
    unsigned long SListFaultCount;// Offset=0x1dc Size=0x4
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x1e0 Size=0x8
    struct _LIST_ENTRY MutantListHead;// Offset=0x1e8 Size=0x8
    void *SListFaultAddress;// Offset=0x1f0 Size=0x4
    struct _KTHREAD_COUNTERS *ThreadCounters;// Offset=0x1f4 Size=0x4
    struct _XSTATE_SAVE *XStateSave;// Offset=0x1f8 Size=0x4
};

union _PS_CLIENT_SECURITY_CONTEXT// Size=0x4
{
    unsigned long ImpersonationData;// Offset=0x0 Size=0x4
    void *ImpersonationToken;// Offset=0x0 Size=0x4
    struct // Size=0x4
    {
        unsigned long ImpersonationLevel:2;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x2
        unsigned long EffectiveOnly:1;// Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1
    };
};

typedef struct _ETHREAD// Size=0x2b8
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x200
    union _LARGE_INTEGER CreateTime;// Offset=0x200 Size=0x8
    union // Size=0x8
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x208 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x208 Size=0x8
    };
    long ExitStatus;// Offset=0x210 Size=0x4
    union // Size=0x8
    {
        struct _LIST_ENTRY PostBlockList;// Offset=0x214 Size=0x8
        struct // Size=0x8
        {
            void *ForwardLinkShadow;// Offset=0x214 Size=0x4
            void *StartAddress;// Offset=0x218 Size=0x4
        };
    };
    union // Size=0x4
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x21c Size=0x4
        struct _ETHREAD *ReaperLink;// Offset=0x21c Size=0x4
        void *KeyedWaitValue;// Offset=0x21c Size=0x4
    };
    unsigned long ActiveTimerListLock;// Offset=0x220 Size=0x4
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x224 Size=0x8
    struct _CLIENT_ID Cid;// Offset=0x22c Size=0x8
    union // Size=0x14
    {
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x234 Size=0x14
        struct _KSEMAPHORE AlpcWaitSemaphore;// Offset=0x234 Size=0x14
    };
    union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity;// Offset=0x248 Size=0x4
    struct _LIST_ENTRY IrpList;// Offset=0x24c Size=0x8
    unsigned long TopLevelIrp;// Offset=0x254 Size=0x4
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x258 Size=0x4
    union _PSP_CPU_QUOTA_APC *CpuQuotaApc;// Offset=0x25c Size=0x4
    void *Win32StartAddress;// Offset=0x260 Size=0x4
    void *LegacyPowerObject;// Offset=0x264 Size=0x4
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x268 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x270 Size=0x4
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x274 Size=0x4
    unsigned long ReadClusterSize;// Offset=0x278 Size=0x4
    long volatile MmLockOrdering;// Offset=0x27c Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x280 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x280 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ThreadInserted:1;// Offset=0x280 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x280 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x280 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Reserved:1;// Offset=0x280 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x280 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x280 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x280 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x280 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long CopyTokenOnOpen:1;// Offset=0x280 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long ThreadIoPriority:3;// Offset=0x280 Size=0x4 BitOffset=0xa BitSize=0x3
            unsigned long ThreadPagePriority:3;// Offset=0x280 Size=0x4 BitOffset=0xd BitSize=0x3
            unsigned long RundownFail:1;// Offset=0x280 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long NeedsWorkingSetAging:1;// Offset=0x280 Size=0x4 BitOffset=0x11 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x284 Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x284 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x284 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x284 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ClonedThread:1;// Offset=0x284 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x284 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long RateApcState:2;// Offset=0x284 Size=0x4 BitOffset=0x5 BitSize=0x2
            unsigned long SelfTerminate:1;// Offset=0x284 Size=0x4 BitOffset=0x7 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x288 Size=0x4
        struct // Size=0x4
        {
            unsigned char Spare:1;// Offset=0x288 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char volatile StartAddressInvalid:1;// Offset=0x288 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char EtwPageFaultCalloutActive:1;// Offset=0x288 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x288 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x288 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemCacheWorkingSetExclusive:1;// Offset=0x288 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemCacheWorkingSetShared:1;// Offset=0x288 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x288 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x289 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceExclusive:1;// Offset=0x289 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceShared:1;// Offset=0x289 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char SuppressSymbolLoad:1;// Offset=0x289 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char Prefetching:1;// Offset=0x289 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsDynamicMemoryShared:1;// Offset=0x289 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsChangeControlAreaExclusive:1;// Offset=0x289 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsChangeControlAreaShared:1;// Offset=0x289 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsPagedPoolWorkingSetExclusive:1;// Offset=0x28a Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsPagedPoolWorkingSetShared:1;// Offset=0x28a Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsSystemPtesWorkingSetExclusive:1;// Offset=0x28a Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsSystemPtesWorkingSetShared:1;// Offset=0x28a Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char TrimTrigger:2;// Offset=0x28a Size=0x1 BitOffset=0x4 BitSize=0x2
            unsigned char Spare1:2;// Offset=0x28a Size=0x1 BitOffset=0x6 BitSize=0x2
            unsigned char PriorityRegionActive;// Offset=0x28b Size=0x1
        };
    };
    unsigned char CacheManagerActive;// Offset=0x28c Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x28d Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x28e Size=0x1
    unsigned char LockOrderState;// Offset=0x28f Size=0x1
    unsigned long AlpcMessageId;// Offset=0x290 Size=0x4
    union // Size=0x4
    {
        void *AlpcMessage;// Offset=0x294 Size=0x4
        unsigned long AlpcReceiveAttributeSet;// Offset=0x294 Size=0x4
    };
    struct _LIST_ENTRY AlpcWaitListEntry;// Offset=0x298 Size=0x8
    unsigned long CacheManagerCount;// Offset=0x2a0 Size=0x4
    unsigned long IoBoostCount;// Offset=0x2a4 Size=0x4
    unsigned long IrpListLock;// Offset=0x2a8 Size=0x4
    void *ReservedForSynchTracking;// Offset=0x2ac Size=0x4
    struct _SINGLE_LIST_ENTRY CmCallbackListHead;// Offset=0x2b0 Size=0x4
} ETHREAD;

struct _KSPECIAL_REGISTERS// Size=0x54
{
    unsigned long Cr0;// Offset=0x0 Size=0x4
    unsigned long Cr2;// Offset=0x4 Size=0x4
    unsigned long Cr3;// Offset=0x8 Size=0x4
    unsigned long Cr4;// Offset=0xc Size=0x4
    unsigned long KernelDr0;// Offset=0x10 Size=0x4
    unsigned long KernelDr1;// Offset=0x14 Size=0x4
    unsigned long KernelDr2;// Offset=0x18 Size=0x4
    unsigned long KernelDr3;// Offset=0x1c Size=0x4
    unsigned long KernelDr6;// Offset=0x20 Size=0x4
    unsigned long KernelDr7;// Offset=0x24 Size=0x4
    struct _DESCRIPTOR Gdtr;// Offset=0x28 Size=0x8
    struct _DESCRIPTOR Idtr;// Offset=0x30 Size=0x8
    unsigned short Tr;// Offset=0x38 Size=0x2
    unsigned short Ldtr;// Offset=0x3a Size=0x2
    unsigned long Reserved[6];// Offset=0x3c Size=0x18
};

struct _KPROCESSOR_STATE// Size=0x320
{
    struct _CONTEXT ContextFrame;// Offset=0x0 Size=0x2cc
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x2cc Size=0x54
};

struct _KDPC_W7// Size=0x20
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned short volatile Number;// Offset=0x2 Size=0x2
    struct _LIST_ENTRY DpcListEntry;// Offset=0x4 Size=0x8
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0xc Size=0x4
    void *DeferredContext;// Offset=0x10 Size=0x4
    void *SystemArgument1;// Offset=0x14 Size=0x4
    void *SystemArgument2;// Offset=0x18 Size=0x4
    void *DpcData;// Offset=0x1c Size=0x4
};

struct _PPM_FFH_THROTTLE_STATE_INFO// Size=0x20
{
    unsigned char EnableLogging;// Offset=0x0 Size=0x1
    unsigned char __align0[3];// Offset=0x1 Size=0x3
    unsigned long MismatchCount;// Offset=0x4 Size=0x4
    unsigned char Initialized;// Offset=0x8 Size=0x1
    unsigned char __align1[7];// Offset=0x9 Size=0x7
    ULONG64 LastValue;// Offset=0x10 Size=0x8
    union _LARGE_INTEGER LastLogTickCount;// Offset=0x18 Size=0x8
};

struct _PROC_IDLE_SNAP// Size=0x10
{
    ULONG64 Time;// Offset=0x0 Size=0x8
    ULONG64 Idle;// Offset=0x8 Size=0x8
};

struct _PROCESSOR_POWER_STATE// Size=0xc8
{
    struct _PPM_IDLE_STATES *IdleStates;// Offset=0x0 Size=0x4
    unsigned char __align0[4];// Offset=0x4 Size=0x4
    ULONG64 IdleTimeLast;// Offset=0x8 Size=0x8
    ULONG64 IdleTimeTotal;// Offset=0x10 Size=0x8
    ULONG64 IdleTimeEntry;// Offset=0x18 Size=0x8
    struct _PROC_IDLE_ACCOUNTING *IdleAccounting;// Offset=0x20 Size=0x4
    enum _PROC_HYPERVISOR_STATE Hypervisor;// Offset=0x24 Size=0x4
    unsigned long PerfHistoryTotal;// Offset=0x28 Size=0x4
    unsigned char ThermalConstraint;// Offset=0x2c Size=0x1
    unsigned char PerfHistoryCount;// Offset=0x2d Size=0x1
    unsigned char PerfHistorySlot;// Offset=0x2e Size=0x1
    unsigned char Reserved;// Offset=0x2f Size=0x1
    unsigned long LastSysTime;// Offset=0x30 Size=0x4
    unsigned long WmiDispatchPtr;// Offset=0x34 Size=0x4
    long WmiInterfaceEnabled;// Offset=0x38 Size=0x4
    unsigned char __align1[4];// Offset=0x3c Size=0x4
    struct _PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo;// Offset=0x40 Size=0x20
    struct _KDPC_W7 PerfActionDpc;// Offset=0x60 Size=0x20
    long volatile PerfActionMask;// Offset=0x80 Size=0x4
    unsigned char __align2[4];// Offset=0x84 Size=0x4
    struct _PROC_IDLE_SNAP IdleCheck;// Offset=0x88 Size=0x10
    struct _PROC_IDLE_SNAP PerfCheck;// Offset=0x98 Size=0x10
    struct _PROC_PERF_DOMAIN *Domain;// Offset=0xa8 Size=0x4
    struct _PROC_PERF_CONSTRAINT *PerfConstraint;// Offset=0xac Size=0x4
    struct _PROC_PERF_LOAD *Load;// Offset=0xb0 Size=0x4
    struct _PROC_HISTORY_ENTRY *PerfHistory;// Offset=0xb4 Size=0x4
    unsigned long Utility;// Offset=0xb8 Size=0x4
    unsigned long OverUtilizedHistory;// Offset=0xbc Size=0x4
    unsigned long volatile AffinityCount;// Offset=0xc0 Size=0x4
    unsigned long AffinityHistory;// Offset=0xc4 Size=0x4
};

struct _KDPC_DATA// Size=0x14
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x8
    unsigned long DpcLock;// Offset=0x8 Size=0x4
    long volatile DpcQueueDepth;// Offset=0xc Size=0x4
    unsigned long DpcCount;// Offset=0x10 Size=0x4
};

struct _KTIMER_TABLE_ENTRY// Size=0x18
{
    unsigned long Lock;// Offset=0x0 Size=0x4
    struct _LIST_ENTRY Entry;// Offset=0x4 Size=0x8
    unsigned char __align0[4];// Offset=0xc Size=0x4
    union _ULARGE_INTEGER Time;// Offset=0x10 Size=0x8
};

struct _KTIMER_TABLE// Size=0x1840
{
    struct _KTIMER *TimerExpiry[16];// Offset=0x0 Size=0x40
    struct _KTIMER_TABLE_ENTRY TimerEntries[256];// Offset=0x40 Size=0x1800
};

typedef struct _KPRCB// Size=0x3628
{
    unsigned short MinorVersion;// Offset=0x0 Size=0x2
    unsigned short MajorVersion;// Offset=0x2 Size=0x2
    struct _KTHREAD *CurrentThread;// Offset=0x4 Size=0x4
    struct _KTHREAD *NextThread;// Offset=0x8 Size=0x4
    struct _KTHREAD *IdleThread;// Offset=0xc Size=0x4
    unsigned char LegacyNumber;// Offset=0x10 Size=0x1
    unsigned char NestingLevel;// Offset=0x11 Size=0x1
    unsigned short BuildType;// Offset=0x12 Size=0x2
    char CpuType;// Offset=0x14 Size=0x1
    char CpuID;// Offset=0x15 Size=0x1
    union // Size=0x2
    {
        unsigned short CpuStep;// Offset=0x16 Size=0x2
        struct // Size=0x2
        {
            unsigned char CpuStepping;// Offset=0x16 Size=0x1
            unsigned char CpuModel;// Offset=0x17 Size=0x1
        };
    };
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x18 Size=0x320
    unsigned long KernelReserved[16];// Offset=0x338 Size=0x40
    unsigned long HalReserved[16];// Offset=0x378 Size=0x40
    unsigned long CFlushSize;// Offset=0x3b8 Size=0x4
    unsigned char CoresPerPhysicalProcessor;// Offset=0x3bc Size=0x1
    unsigned char LogicalProcessorsPerCore;// Offset=0x3bd Size=0x1
    unsigned char PrcbPad0[2];// Offset=0x3be Size=0x2
    unsigned long MHz;// Offset=0x3c0 Size=0x4
    unsigned char CpuVendor;// Offset=0x3c4 Size=0x1
    unsigned char GroupIndex;// Offset=0x3c5 Size=0x1
    unsigned short Group;// Offset=0x3c6 Size=0x2
    unsigned long GroupSetMember;// Offset=0x3c8 Size=0x4
    unsigned long Number;// Offset=0x3cc Size=0x4
    unsigned char PrcbPad1[72];// Offset=0x3d0 Size=0x48
    struct _KSPIN_LOCK_QUEUE LockQueue[17];// Offset=0x418 Size=0x88
    struct _KTHREAD *NpxThread;// Offset=0x4a0 Size=0x4
    unsigned long InterruptCount;// Offset=0x4a4 Size=0x4
    unsigned long KernelTime;// Offset=0x4a8 Size=0x4
    unsigned long UserTime;// Offset=0x4ac Size=0x4
    unsigned long DpcTime;// Offset=0x4b0 Size=0x4
    unsigned long DpcTimeCount;// Offset=0x4b4 Size=0x4
    unsigned long InterruptTime;// Offset=0x4b8 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x4bc Size=0x4
    unsigned long PageColor;// Offset=0x4c0 Size=0x4
    unsigned char DebuggerSavedIRQL;// Offset=0x4c4 Size=0x1
    unsigned char NodeColor;// Offset=0x4c5 Size=0x1
    unsigned char PrcbPad20[2];// Offset=0x4c6 Size=0x2
    unsigned long NodeShiftedColor;// Offset=0x4c8 Size=0x4
    struct _KNODE *ParentNode;// Offset=0x4cc Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x4d0 Size=0x4
    unsigned long DpcTimeLimit;// Offset=0x4d4 Size=0x4
    unsigned long PrcbPad21[2];// Offset=0x4d8 Size=0x8
    unsigned long CcFastReadNoWait;// Offset=0x4e0 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x4e4 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x4e8 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x4ec Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x4f0 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x4f4 Size=0x4
    long volatile MmSpinLockOrdering;// Offset=0x4f8 Size=0x4
    long volatile IoReadOperationCount;// Offset=0x4fc Size=0x4
    long volatile IoWriteOperationCount;// Offset=0x500 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0x504 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0x508 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0x510 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0x518 Size=0x8
    unsigned long CcFastMdlReadNoWait;// Offset=0x520 Size=0x4
    unsigned long CcFastMdlReadWait;// Offset=0x524 Size=0x4
    unsigned long CcFastMdlReadNotPossible;// Offset=0x528 Size=0x4
    unsigned long CcMapDataNoWait;// Offset=0x52c Size=0x4
    unsigned long CcMapDataWait;// Offset=0x530 Size=0x4
    unsigned long CcPinMappedDataCount;// Offset=0x534 Size=0x4
    unsigned long CcPinReadNoWait;// Offset=0x538 Size=0x4
    unsigned long CcPinReadWait;// Offset=0x53c Size=0x4
    unsigned long CcMdlReadNoWait;// Offset=0x540 Size=0x4
    unsigned long CcMdlReadWait;// Offset=0x544 Size=0x4
    unsigned long CcLazyWriteHotSpots;// Offset=0x548 Size=0x4
    unsigned long CcLazyWriteIos;// Offset=0x54c Size=0x4
    unsigned long CcLazyWritePages;// Offset=0x550 Size=0x4
    unsigned long CcDataFlushes;// Offset=0x554 Size=0x4
    unsigned long CcDataPages;// Offset=0x558 Size=0x4
    unsigned long CcLostDelayedWrites;// Offset=0x55c Size=0x4
    unsigned long CcFastReadResourceMiss;// Offset=0x560 Size=0x4
    unsigned long CcCopyReadWaitMiss;// Offset=0x564 Size=0x4
    unsigned long CcFastMdlReadResourceMiss;// Offset=0x568 Size=0x4
    unsigned long CcMapDataNoWaitMiss;// Offset=0x56c Size=0x4
    unsigned long CcMapDataWaitMiss;// Offset=0x570 Size=0x4
    unsigned long CcPinReadNoWaitMiss;// Offset=0x574 Size=0x4
    unsigned long CcPinReadWaitMiss;// Offset=0x578 Size=0x4
    unsigned long CcMdlReadNoWaitMiss;// Offset=0x57c Size=0x4
    unsigned long CcMdlReadWaitMiss;// Offset=0x580 Size=0x4
    unsigned long CcReadAheadIos;// Offset=0x584 Size=0x4
    unsigned long KeAlignmentFixupCount;// Offset=0x588 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x58c Size=0x4
    unsigned long KeSystemCalls;// Offset=0x590 Size=0x4
    unsigned long AvailableTime;// Offset=0x594 Size=0x4
    unsigned long PrcbPad22[2];// Offset=0x598 Size=0x8
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x5a0 Size=0x80
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];// Offset=0x620 Size=0x900
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];// Offset=0xf20 Size=0x900
    unsigned long volatile PacketBarrier;// Offset=0x1820 Size=0x4
    long volatile ReverseStall;// Offset=0x1824 Size=0x4
    void *IpiFrame;// Offset=0x1828 Size=0x4
    unsigned char PrcbPad3[52];// Offset=0x182c Size=0x34
    void volatile *CurrentPacket[3];// Offset=0x1860 Size=0xc
    unsigned long volatile TargetSet;// Offset=0x186c Size=0x4
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x1870 Size=0x4
    unsigned long volatile IpiFrozen;// Offset=0x1874 Size=0x4
    unsigned char PrcbPad4[40];// Offset=0x1878 Size=0x28
    unsigned long volatile RequestSummary;// Offset=0x18a0 Size=0x4
    struct _KPRCB volatile *SignalDone;// Offset=0x18a4 Size=0x4
    unsigned char PrcbPad50[56];// Offset=0x18a8 Size=0x38
    struct _KDPC_DATA DpcData[2];// Offset=0x18e0 Size=0x28
    void *DpcStack;// Offset=0x1908 Size=0x4
    long MaximumDpcQueueDepth;// Offset=0x190c Size=0x4
    unsigned long DpcRequestRate;// Offset=0x1910 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x1914 Size=0x4
    unsigned long DpcLastCount;// Offset=0x1918 Size=0x4
    unsigned long PrcbLock;// Offset=0x191c Size=0x4
    struct _KGATE DpcGate;// Offset=0x1920 Size=0x10
    unsigned char ThreadDpcEnable;// Offset=0x1930 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x1931 Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x1932 Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x1933 Size=0x1
    union // Size=0x4
    {
        long volatile DpcRequestSummary;// Offset=0x1934 Size=0x4
        short DpcRequestSlot[2];// Offset=0x1934 Size=0x4
        struct // Size=0x4
        {
            short NormalDpcState;// Offset=0x1934 Size=0x2
            union // Size=0x2
            {
                unsigned short volatile DpcThreadActive:1;// Offset=0x1936 Size=0x2 BitOffset=0x0 BitSize=0x1
                short ThreadDpcState;// Offset=0x1936 Size=0x2
            };
        };
    };
    unsigned long volatile TimerHand;// Offset=0x1938 Size=0x4
    unsigned long LastTick;// Offset=0x193c Size=0x4
    long MasterOffset;// Offset=0x1940 Size=0x4
    unsigned long PrcbPad41[2];// Offset=0x1944 Size=0x8
    unsigned long PeriodicCount;// Offset=0x194c Size=0x4
    unsigned long PeriodicBias;// Offset=0x1950 Size=0x4
    unsigned char __align0[4];// Offset=0x1954 Size=0x4
    ULONG64 TickOffset;// Offset=0x1958 Size=0x8
    struct _KTIMER_TABLE TimerTable;// Offset=0x1960 Size=0x1840
    struct _KDPC_W7 CallDpc;// Offset=0x31a0 Size=0x20
    long ClockKeepAlive;// Offset=0x31c0 Size=0x4
    unsigned char ClockCheckSlot;// Offset=0x31c4 Size=0x1
    unsigned char ClockPollCycle;// Offset=0x31c5 Size=0x1
    unsigned char PrcbPad6[2];// Offset=0x31c6 Size=0x2
    long DpcWatchdogPeriod;// Offset=0x31c8 Size=0x4
    long DpcWatchdogCount;// Offset=0x31cc Size=0x4
    long ThreadWatchdogPeriod;// Offset=0x31d0 Size=0x4
    long ThreadWatchdogCount;// Offset=0x31d4 Size=0x4
    long volatile KeSpinLockOrdering;// Offset=0x31d8 Size=0x4
    unsigned long PrcbPad70[1];// Offset=0x31dc Size=0x4
    struct _LIST_ENTRY WaitListHead;// Offset=0x31e0 Size=0x8
    unsigned long WaitLock;// Offset=0x31e8 Size=0x4
    unsigned long ReadySummary;// Offset=0x31ec Size=0x4
    unsigned long QueueIndex;// Offset=0x31f0 Size=0x4
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0x31f4 Size=0x4
    ULONG64 StartCycles;// Offset=0x31f8 Size=0x8
    ULONG64 volatile CycleTime;// Offset=0x3200 Size=0x8
    unsigned long volatile HighCycleTime;// Offset=0x3208 Size=0x4
    unsigned long PrcbPad71;// Offset=0x320c Size=0x4
    ULONG64 PrcbPad72[2];// Offset=0x3210 Size=0x10
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x3220 Size=0x100
    void *ChainedInterruptList;// Offset=0x3320 Size=0x4
    long LookasideIrpFloat;// Offset=0x3324 Size=0x4
    long volatile MmPageFaultCount;// Offset=0x3328 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0x332c Size=0x4
    long volatile MmTransitionCount;// Offset=0x3330 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0x3334 Size=0x4
    long volatile MmDemandZeroCount;// Offset=0x3338 Size=0x4
    long volatile MmPageReadCount;// Offset=0x333c Size=0x4
    long volatile MmPageReadIoCount;// Offset=0x3340 Size=0x4
    long volatile MmCacheReadCount;// Offset=0x3344 Size=0x4
    long volatile MmCacheIoCount;// Offset=0x3348 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0x334c Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0x3350 Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0x3354 Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0x3358 Size=0x4
    unsigned long volatile CachedCommit;// Offset=0x335c Size=0x4
    unsigned long volatile CachedResidentAvailable;// Offset=0x3360 Size=0x4
    void *HyperPte;// Offset=0x3364 Size=0x4
    unsigned char PrcbPad8[4];// Offset=0x3368 Size=0x4
    unsigned char VendorString[13];// Offset=0x336c Size=0xd
    unsigned char InitialApicId;// Offset=0x3379 Size=0x1
    unsigned char LogicalProcessorsPerPhysicalProcessor;// Offset=0x337a Size=0x1
    unsigned char PrcbPad9[5];// Offset=0x337b Size=0x5
    unsigned long FeatureBits;// Offset=0x3380 Size=0x4
    unsigned char __align1[4];// Offset=0x3384 Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x3388 Size=0x8
    ULONG64 volatile IsrTime;// Offset=0x3390 Size=0x8
    ULONG64 RuntimeAccumulation;// Offset=0x3398 Size=0x8
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0x33a0 Size=0xc8
    struct _KDPC_W7 DpcWatchdogDpc;// Offset=0x3468 Size=0x20
    struct _KTIMER DpcWatchdogTimer;// Offset=0x3488 Size=0x28
    void *WheaInfo;// Offset=0x34b0 Size=0x4
    void *EtwSupport;// Offset=0x34b4 Size=0x4
    union _SLIST_HEADER InterruptObjectPool;// Offset=0x34b8 Size=0x8
    union _SLIST_HEADER HypercallPageList;// Offset=0x34c0 Size=0x8
    void *HypercallPageVirtual;// Offset=0x34c8 Size=0x4
    void *VirtualApicAssist;// Offset=0x34cc Size=0x4
    ULONG64 *StatisticsPage;// Offset=0x34d0 Size=0x4
    void *RateControl;// Offset=0x34d4 Size=0x4
    struct _CACHE_DESCRIPTOR Cache[5];// Offset=0x34d8 Size=0x3c
    unsigned long CacheCount;// Offset=0x3514 Size=0x4
    unsigned long CacheProcessorMask[5];// Offset=0x3518 Size=0x14
    struct _KAFFINITY_EX PackageProcessorSet;// Offset=0x352c Size=0xc
    unsigned long PrcbPad91[1];// Offset=0x3538 Size=0x4
    unsigned long CoreProcessorSet;// Offset=0x353c Size=0x4
    struct _KDPC_W7 TimerExpirationDpc;// Offset=0x3540 Size=0x20
    unsigned long SpinLockAcquireCount;// Offset=0x3560 Size=0x4
    unsigned long SpinLockContentionCount;// Offset=0x3564 Size=0x4
    unsigned long SpinLockSpinCount;// Offset=0x3568 Size=0x4
    unsigned long IpiSendRequestBroadcastCount;// Offset=0x356c Size=0x4
    unsigned long IpiSendRequestRoutineCount;// Offset=0x3570 Size=0x4
    unsigned long IpiSendSoftwareInterruptCount;// Offset=0x3574 Size=0x4
    unsigned long ExInitializeResourceCount;// Offset=0x3578 Size=0x4
    unsigned long ExReInitializeResourceCount;// Offset=0x357c Size=0x4
    unsigned long ExDeleteResourceCount;// Offset=0x3580 Size=0x4
    unsigned long ExecutiveResourceAcquiresCount;// Offset=0x3584 Size=0x4
    unsigned long ExecutiveResourceContentionsCount;// Offset=0x3588 Size=0x4
    unsigned long ExecutiveResourceReleaseExclusiveCount;// Offset=0x358c Size=0x4
    unsigned long ExecutiveResourceReleaseSharedCount;// Offset=0x3590 Size=0x4
    unsigned long ExecutiveResourceConvertsCount;// Offset=0x3594 Size=0x4
    unsigned long ExAcqResExclusiveAttempts;// Offset=0x3598 Size=0x4
    unsigned long ExAcqResExclusiveAcquiresExclusive;// Offset=0x359c Size=0x4
    unsigned long ExAcqResExclusiveAcquiresExclusiveRecursive;// Offset=0x35a0 Size=0x4
    unsigned long ExAcqResExclusiveWaits;// Offset=0x35a4 Size=0x4
    unsigned long ExAcqResExclusiveNotAcquires;// Offset=0x35a8 Size=0x4
    unsigned long ExAcqResSharedAttempts;// Offset=0x35ac Size=0x4
    unsigned long ExAcqResSharedAcquiresExclusive;// Offset=0x35b0 Size=0x4
    unsigned long ExAcqResSharedAcquiresShared;// Offset=0x35b4 Size=0x4
    unsigned long ExAcqResSharedAcquiresSharedRecursive;// Offset=0x35b8 Size=0x4
    unsigned long ExAcqResSharedWaits;// Offset=0x35bc Size=0x4
    unsigned long ExAcqResSharedNotAcquires;// Offset=0x35c0 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAttempts;// Offset=0x35c4 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresExclusive;// Offset=0x35c8 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresShared;// Offset=0x35cc Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;// Offset=0x35d0 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveWaits;// Offset=0x35d4 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveNotAcquires;// Offset=0x35d8 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAttempts;// Offset=0x35dc Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresExclusive;// Offset=0x35e0 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresShared;// Offset=0x35e4 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive;// Offset=0x35e8 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveWaits;// Offset=0x35ec Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveNotAcquires;// Offset=0x35f0 Size=0x4
    unsigned long ExSetResOwnerPointerExclusive;// Offset=0x35f4 Size=0x4
    unsigned long ExSetResOwnerPointerSharedNew;// Offset=0x35f8 Size=0x4
    unsigned long ExSetResOwnerPointerSharedOld;// Offset=0x35fc Size=0x4
    unsigned long ExTryToAcqExclusiveAttempts;// Offset=0x3600 Size=0x4
    unsigned long ExTryToAcqExclusiveAcquires;// Offset=0x3604 Size=0x4
    unsigned long ExBoostExclusiveOwner;// Offset=0x3608 Size=0x4
    unsigned long ExBoostSharedOwners;// Offset=0x360c Size=0x4
    unsigned long ExEtwSynchTrackingNotificationsCount;// Offset=0x3610 Size=0x4
    unsigned long ExEtwSynchTrackingNotificationsAccountedCount;// Offset=0x3614 Size=0x4
    struct _CONTEXT *Context;// Offset=0x3618 Size=0x4
    unsigned long ContextFlags;// Offset=0x361c Size=0x4
    struct _XSAVE_AREA *ExtendedState;// Offset=0x3620 Size=0x4
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0xa8
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x94
    struct _EX_PUSH_LOCK Lock;// Offset=0x94 Size=0x4
    struct _DEVICE_MAP *DeviceMap;// Offset=0x98 Size=0x4
    unsigned long SessionId;// Offset=0x9c Size=0x4
    void *NamespaceEntry;// Offset=0xa0 Size=0x4
    unsigned long Flags;// Offset=0xa4 Size=0x4
} *POBJECT_DIRECTORY;
#endif //                  Windows 7 x32



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows XP/2003 x64 internal kernel definitions                   //
#if (NTDDI_VERSION >= NTDDI_WS03SP1) && (NTDDI_VERSION <= NTDDI_WS03SP4) && defined(_AMD64_)

typedef struct _MM_SESSION_SPACE// Size=0x1d80
{
    struct _MM_SESSION_SPACE *GlobalVirtualAddress;// Offset=0x0 Size=0x8
    long ReferenceCount;// Offset=0x8 Size=0x4
    union { // Offset=0xc Size=0x4
        ULONG LongFlags;
        MM_SESSION_SPACE_FLAGS Flags;
    } u;
    unsigned long SessionId;// Offset=0x10 Size=0x4
    // ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x8
{
    union // Size=0x8
    {
        struct // Size=0x8
        {
            ULONG64 Locked:1;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x1
            ULONG64 Waiting:1;// Offset=0x0 Size=0x8 BitOffset=0x1 BitSize=0x1
            ULONG64 Waking:1;// Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
            ULONG64 MultipleShared:1;// Offset=0x0 Size=0x8 BitOffset=0x3 BitSize=0x1
            ULONG64 Shared:60;// Offset=0x0 Size=0x8 BitOffset=0x4 BitSize=0x3c
        };
        ULONG64 Value;// Offset=0x0 Size=0x8
        void *Ptr;// Offset=0x0 Size=0x8
    };
};

struct _EX_FAST_REF// Size=0x8
{
    union // Size=0x8
    {
        void *Object;// Offset=0x0 Size=0x8
        ULONG64 RefCnt:4;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x4
        ULONG64 Value;// Offset=0x0 Size=0x8
    };
};

typedef struct _MMADDRESS_NODE // Size=0x28
{
    union {// Offset=0x0 Size=0x8
        LONG64 Balance:2;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x8
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x8 Size=0x8
    struct _MMADDRESS_NODE *RightChild;// Offset=0x10 Size=0x8
    ULONG64 StartingVpn;// Offset=0x18 Size=0x8
    ULONG64 EndingVpn;// Offset=0x20 Size=0x8
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE// Size=0x40
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x28
    ULONG64 DepthOfTree:5;// Offset=0x28 Size=0x8 BitOffset=0x0 BitSize=0x5
    ULONG64 Unused:3;// Offset=0x28 Size=0x8 BitOffset=0x5 BitSize=0x3
    ULONG64 NumberGenericTableElements:56;// Offset=0x28 Size=0x8 BitOffset=0x8 BitSize=0x38
    void *NodeHint;// Offset=0x30 Size=0x8
    void *NodeFreeHint;// Offset=0x38 Size=0x8
};

struct _KPROCESS// Size=0xb8
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    struct _LIST_ENTRY ProfileListHead;// Offset=0x18 Size=0x10
    ULONG64 DirectoryTableBase[2];// Offset=0x28 Size=0x10
    unsigned short IopmOffset;// Offset=0x38 Size=0x2
    unsigned char __align0[6];// Offset=0x3a Size=0x6
    ULONG64 volatile ActiveProcessors;// Offset=0x40 Size=0x8
    unsigned long KernelTime;// Offset=0x48 Size=0x4
    unsigned long UserTime;// Offset=0x4c Size=0x4
    struct _LIST_ENTRY ReadyListHead;// Offset=0x50 Size=0x10
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x60 Size=0x8
    void *Reserved1;// Offset=0x68 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x70 Size=0x10
    ULONG64 ProcessLock;// Offset=0x80 Size=0x8
    ULONG64 Affinity;// Offset=0x88 Size=0x8
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long AutoAlignment:1;// Offset=0x90 Size=0x4 BitOffset=0x0 BitSize=0x1
            long DisableBoost:1;// Offset=0x90 Size=0x4 BitOffset=0x1 BitSize=0x1
            long DisableQuantum:1;// Offset=0x90 Size=0x4 BitOffset=0x2 BitSize=0x1
            long ReservedFlags:29;// Offset=0x90 Size=0x4 BitOffset=0x3 BitSize=0x1d
        };
        long ProcessFlags;// Offset=0x90 Size=0x4
    };
    char BasePriority;// Offset=0x94 Size=0x1
    char QuantumReset;// Offset=0x95 Size=0x1
    unsigned char State;// Offset=0x96 Size=0x1
    unsigned char ThreadSeed;// Offset=0x97 Size=0x1
    unsigned char PowerState;// Offset=0x98 Size=0x1
    unsigned char IdealNode;// Offset=0x99 Size=0x1
    unsigned char Visited;// Offset=0x9a Size=0x1
    union // Size=0x1
    {
        struct _KEXECUTE_OPTIONS Flags;// Offset=0x9b Size=0x1
        unsigned char ExecuteOptions;// Offset=0x9b Size=0x1
    };
    unsigned char __align1[4];// Offset=0x9c Size=0x4
    ULONG64 StackCount;// Offset=0xa0 Size=0x8
    struct _LIST_ENTRY ProcessListEntry;// Offset=0xa8 Size=0x10
};

struct _MMSUPPORT// Size=0x58
{
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x0 Size=0x10
    union _LARGE_INTEGER LastTrimTime;// Offset=0x10 Size=0x8
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x18 Size=0x4
    unsigned long PageFaultCount;// Offset=0x1c Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x20 Size=0x4
    unsigned long GrowthSinceLastEstimate;// Offset=0x24 Size=0x4
    unsigned long MinimumWorkingSetSize;// Offset=0x28 Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x2c Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x30 Size=0x8
    unsigned long Claim;// Offset=0x38 Size=0x4
    unsigned long NextEstimationSlot;// Offset=0x3c Size=0x4
    unsigned long NextAgingSlot;// Offset=0x40 Size=0x4
    unsigned long EstimatedAvailable;// Offset=0x44 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x48 Size=0x4
    unsigned char __align0[4];// Offset=0x4c Size=0x4
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x50 Size=0x8
};

typedef struct _EPROCESS// Size=0x3e0
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0xb8
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0xb8 Size=0x8
    union _LARGE_INTEGER CreateTime;// Offset=0xc0 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0xc8 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0xd0 Size=0x8
    void *UniqueProcessId;// Offset=0xd8 Size=0x8
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0xe0 Size=0x10
    ULONG64 QuotaUsage[3];// Offset=0xf0 Size=0x18
    ULONG64 QuotaPeak[3];// Offset=0x108 Size=0x18
    ULONG64 CommitCharge;// Offset=0x120 Size=0x8
    ULONG64 PeakVirtualSize;// Offset=0x128 Size=0x8
    ULONG64 VirtualSize;// Offset=0x130 Size=0x8
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0x138 Size=0x10
    void *DebugPort;// Offset=0x148 Size=0x8
    void *ExceptionPort;// Offset=0x150 Size=0x8
    struct _HANDLE_TABLE *ObjectTable;// Offset=0x158 Size=0x8
    struct _EX_FAST_REF Token;// Offset=0x160 Size=0x8
    ULONG64 WorkingSetPage;// Offset=0x168 Size=0x8
    struct _KGUARDED_MUTEX AddressCreationLock;// Offset=0x170 Size=0x38
    ULONG64 HyperSpaceLock;// Offset=0x1a8 Size=0x8
    struct _ETHREAD *ForkInProgress;// Offset=0x1b0 Size=0x8
    ULONG64 HardwareTrigger;// Offset=0x1b8 Size=0x8
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0x1c0 Size=0x8
    void *CloneRoot;// Offset=0x1c8 Size=0x8
    ULONG64 NumberOfPrivatePages;// Offset=0x1d0 Size=0x8
    ULONG64 NumberOfLockedPages;// Offset=0x1d8 Size=0x8
    void *Win32Process;// Offset=0x1e0 Size=0x8
    struct _EJOB *Job;// Offset=0x1e8 Size=0x8
    void *SectionObject;// Offset=0x1f0 Size=0x8
    void *SectionBaseAddress;// Offset=0x1f8 Size=0x8
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x200 Size=0x8
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x208 Size=0x8
    void *Win32WindowStation;// Offset=0x210 Size=0x8
    void *InheritedFromUniqueProcessId;// Offset=0x218 Size=0x8
    void *LdtInformation;// Offset=0x220 Size=0x8
    void *VadFreeHint;// Offset=0x228 Size=0x8
    void *VdmObjects;// Offset=0x230 Size=0x8
    void *DeviceMap;// Offset=0x238 Size=0x8
    void *Spare0[3];// Offset=0x240 Size=0x18
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x258 Size=0x8
        ULONG64 Filler;// Offset=0x258 Size=0x8
    };
    void *Session;// Offset=0x260 Size=0x8
    unsigned char ImageFileName[16];// Offset=0x268 Size=0x10
    struct _LIST_ENTRY JobLinks;// Offset=0x278 Size=0x10
    void *LockedPagesList;// Offset=0x288 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x290 Size=0x10
    void *SecurityPort;// Offset=0x2a0 Size=0x8
    struct _WOW64_PROCESS *Wow64Process;// Offset=0x2a8 Size=0x8
    unsigned long ActiveThreads;// Offset=0x2b0 Size=0x4
    unsigned long GrantedAccess;// Offset=0x2b4 Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x2b8 Size=0x4
    long LastThreadExitStatus;// Offset=0x2bc Size=0x4
    struct _PEB *Peb;// Offset=0x2c0 Size=0x8
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x2c8 Size=0x8
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x2d0 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x2d8 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x2e0 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x2e8 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x2f0 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x2f8 Size=0x8
    ULONG64 CommitChargeLimit;// Offset=0x300 Size=0x8
    ULONG64 CommitChargePeak;// Offset=0x308 Size=0x8
    void *AweInfo;// Offset=0x310 Size=0x8
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x318 Size=0x8
    struct _MMSUPPORT Vm;// Offset=0x320 Size=0x58
    unsigned long Spares[2];// Offset=0x378 Size=0x8
    unsigned long ModifiedPageCount;// Offset=0x380 Size=0x4
    unsigned long JobStatus;// Offset=0x384 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x388 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x388 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x388 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x388 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x388 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x388 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x388 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x388 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x388 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x388 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x388 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x388 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x388 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x388 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long SessionCreationUnderway:1;// Offset=0x388 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x388 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x388 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x388 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x388 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x388 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x388 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x388 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x388 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x388 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x388 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long SmapAllowed:1;// Offset=0x388 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long CreateFailed:1;// Offset=0x388 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x388 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long Spare1:1;// Offset=0x388 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long Spare2:1;// Offset=0x388 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x38c Size=0x4
    unsigned short NextPageColor;// Offset=0x390 Size=0x2
    union // Size=0x2
    {
        struct // Size=0x2
        {
            unsigned char SubSystemMinorVersion;// Offset=0x392 Size=0x1
            unsigned char SubSystemMajorVersion;// Offset=0x393 Size=0x1
        };
        unsigned short SubSystemVersion;// Offset=0x392 Size=0x2
    };
    unsigned char PriorityClass;// Offset=0x394 Size=0x1
    unsigned char __align0[3];// Offset=0x395 Size=0x3
    struct _MM_AVL_TABLE VadRoot;// Offset=0x398 Size=0x40
    unsigned long Cookie;// Offset=0x3d8 Size=0x4
} EPROCESS;

struct _CONTROL_AREA // Size=0x48
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY DereferenceList;// Offset=0x8 Size=0x10
    unsigned long NumberOfSectionReferences;// Offset=0x18 Size=0x4
    unsigned long NumberOfPfnReferences;// Offset=0x1c Size=0x4
    unsigned long NumberOfMappedViews;// Offset=0x20 Size=0x4
    unsigned long NumberOfSystemCacheViews;// Offset=0x24 Size=0x4
    unsigned long NumberOfUserReferences;// Offset=0x28 Size=0x4
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    struct _FILE_OBJECT *FilePointer;// Offset=0x30 Size=0x8
    struct _EVENT_COUNTER *WaitingForDeletion;// Offset=0x38 Size=0x8
    unsigned short ModifiedWriteCount;// Offset=0x40 Size=0x2
    unsigned short FlushInProgressCount;// Offset=0x42 Size=0x2
    unsigned long WritableUserReferences;// Offset=0x44 Size=0x4
};

typedef struct _SEGMENT// Size=0x68
{
    struct _CONTROL_AREA *ControlArea;// Offset=0x0 Size=0x8
    // ...
} SEGMENT, *PSEGMENT;

typedef struct _SECTION
{
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

struct _KAPC_STATE// Size=0x30
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x20
    struct _KPROCESS *Process;// Offset=0x20 Size=0x8
    unsigned char KernelApcInProgress;// Offset=0x28 Size=0x1
    unsigned char KernelApcPending;// Offset=0x29 Size=0x1
    unsigned char UserApcPending;// Offset=0x2a Size=0x1
};

struct _KTHREAD// Size=0x308
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    struct _LIST_ENTRY MutantListHead;// Offset=0x18 Size=0x10
    void *InitialStack;// Offset=0x28 Size=0x8
    void *StackLimit;// Offset=0x30 Size=0x8
    void *KernelStack;// Offset=0x38 Size=0x8
    ULONG64 ThreadLock;// Offset=0x40 Size=0x8
    union // Size=0x30
    {
        struct _KAPC_STATE ApcState;// Offset=0x48 Size=0x30
        struct // Size=0x30
        {
            unsigned char ApcStateFill[43];// Offset=0x48 Size=0x2b
            unsigned char ApcQueueable;// Offset=0x73 Size=0x1
            unsigned char volatile NextProcessor;// Offset=0x74 Size=0x1
            unsigned char volatile DeferredProcessor;// Offset=0x75 Size=0x1
            unsigned char AdjustReason;// Offset=0x76 Size=0x1
            char AdjustIncrement;// Offset=0x77 Size=0x1
        };
    };
    ULONG64 ApcQueueLock;// Offset=0x78 Size=0x8
    LONG64 WaitStatus;// Offset=0x80 Size=0x8
    union // Size=0x8
    {
        struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x88 Size=0x8
        struct _KGATE *GateObject;// Offset=0x88 Size=0x8
    };
    unsigned char Alertable;// Offset=0x90 Size=0x1
    unsigned char WaitNext;// Offset=0x91 Size=0x1
    unsigned char WaitReason;// Offset=0x92 Size=0x1
    char Priority;// Offset=0x93 Size=0x1
    unsigned char EnableStackSwap;// Offset=0x94 Size=0x1
    unsigned char volatile SwapBusy;// Offset=0x95 Size=0x1
    unsigned char Alerted[2];// Offset=0x96 Size=0x2
    union // Size=0x10
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x98 Size=0x10
        struct // Size=0x10
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x98 Size=0x8
            unsigned char __align0[8];// Offset=0xa0 Size=0x8
        };
    };
    struct _KQUEUE *Queue;// Offset=0xa8 Size=0x8
    void *Teb;// Offset=0xb0 Size=0x8
    union // Size=0x40
    {
        struct _KTIMER Timer;// Offset=0xb8 Size=0x40
        struct // Size=0x40
        {
            unsigned char TimerFill[60];// Offset=0xb8 Size=0x3c
            union // Size=0x4
            {
                struct // Size=0x4
                {
                    unsigned long AutoAlignment:1;// Offset=0xf4 Size=0x4 BitOffset=0x0 BitSize=0x1
                    unsigned long DisableBoost:1;// Offset=0xf4 Size=0x4 BitOffset=0x1 BitSize=0x1
                    unsigned long GuiThread:1;// Offset=0xf4 Size=0x4 BitOffset=0x2 BitSize=0x1
                    unsigned long ReservedFlags:29;// Offset=0xf4 Size=0x4 BitOffset=0x3 BitSize=0x1d
                };
                long ThreadFlags;// Offset=0xf4 Size=0x4
            };
        };
    };
    union // Size=0xc0
    {
        struct _KWAIT_BLOCK WaitBlock[4];// Offset=0xf8 Size=0xc0
        struct // Size=0x2c
        {
            unsigned char WaitBlockFill0[43];// Offset=0xf8 Size=0x2b
            unsigned char SystemAffinityActive;// Offset=0x123 Size=0x1
        };
        struct // Size=0x5c
        {
            unsigned char WaitBlockFill1[91];// Offset=0xf8 Size=0x5b
            char PreviousMode;// Offset=0x153 Size=0x1
        };
        struct // Size=0x8c
        {
            unsigned char WaitBlockFill2[139];// Offset=0xf8 Size=0x8b
            unsigned char ResourceIndex;// Offset=0x183 Size=0x1
        };
        struct // Size=0xbc
        {
            unsigned char WaitBlockFill3[187];// Offset=0xf8 Size=0xbb
            unsigned char LargeStack;// Offset=0x1b3 Size=0x1
        };
        struct // Size=0x30
        {
            unsigned char WaitBlockFill4[44];// Offset=0xf8 Size=0x2c
            unsigned long ContextSwitches;// Offset=0x124 Size=0x4
        };
        struct // Size=0x60
        {
            unsigned char WaitBlockFill5[92];// Offset=0xf8 Size=0x5c
            unsigned char volatile State;// Offset=0x154 Size=0x1
            unsigned char NpxState;// Offset=0x155 Size=0x1
            unsigned char WaitIrql;// Offset=0x156 Size=0x1
            char WaitMode;// Offset=0x157 Size=0x1
        };
        struct // Size=0x90
        {
            unsigned char WaitBlockFill6[140];// Offset=0xf8 Size=0x8c
            unsigned long WaitTime;// Offset=0x184 Size=0x4
        };
        struct // Size=0xc0
        {
            unsigned char WaitBlockFill7[188];// Offset=0xf8 Size=0xbc
            union // Size=0x4
            {
                struct // Size=0x4
                {
                    short KernelApcDisable;// Offset=0x1b4 Size=0x2
                    short SpecialApcDisable;// Offset=0x1b6 Size=0x2
                };
                unsigned long CombinedApcDisable;// Offset=0x1b4 Size=0x4
            };
        };
    };
    struct _LIST_ENTRY QueueListEntry;// Offset=0x1b8 Size=0x10
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x1c8 Size=0x8
    void *CallbackStack;// Offset=0x1d0 Size=0x8
    unsigned char ApcStateIndex;// Offset=0x1d8 Size=0x1
    unsigned char IdealProcessor;// Offset=0x1d9 Size=0x1
    unsigned char Preempted;// Offset=0x1da Size=0x1
    unsigned char ProcessReadyQueue;// Offset=0x1db Size=0x1
    unsigned char KernelStackResident;// Offset=0x1dc Size=0x1
    char BasePriority;// Offset=0x1dd Size=0x1
    char PriorityDecrement;// Offset=0x1de Size=0x1
    char Saturation;// Offset=0x1df Size=0x1
    ULONG64 UserAffinity;// Offset=0x1e0 Size=0x8
    struct _KPROCESS *Process;// Offset=0x1e8 Size=0x8
    ULONG64 Affinity;// Offset=0x1f0 Size=0x8
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x1f8 Size=0x10
    union // Size=0x30
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x208 Size=0x30
        struct // Size=0x30
        {
            unsigned char SavedApcStateFill[43];// Offset=0x208 Size=0x2b
            char FreezeCount;// Offset=0x233 Size=0x1
            char SuspendCount;// Offset=0x234 Size=0x1
            unsigned char UserIdealProcessor;// Offset=0x235 Size=0x1
            unsigned char CalloutActive;// Offset=0x236 Size=0x1
            unsigned char CodePatchInProgress;// Offset=0x237 Size=0x1
        };
    };
    void *Win32Thread;// Offset=0x238 Size=0x8
    void *StackBase;// Offset=0x240 Size=0x8
    union // Size=0x58
    {
        struct _KAPC SuspendApc;// Offset=0x248 Size=0x58
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x248 Size=0x1
            char Quantum;// Offset=0x249 Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x248 Size=0x3
            unsigned char QuantumReset;// Offset=0x24b Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x248 Size=0x4
            unsigned long KernelTime;// Offset=0x24c Size=0x4
        };
        struct // Size=0x48
        {
            unsigned char SuspendApcFill3[64];// Offset=0x248 Size=0x40
            void *TlsArray;// Offset=0x288 Size=0x8
        };
        struct // Size=0x50
        {
            unsigned char SuspendApcFill4[72];// Offset=0x248 Size=0x48
            void *LegoData;// Offset=0x290 Size=0x8
        };
        struct // Size=0x58
        {
            unsigned char SuspendApcFill5[83];// Offset=0x248 Size=0x53
            unsigned char PowerState;// Offset=0x29b Size=0x1
            unsigned long UserTime;// Offset=0x29c Size=0x4
        };
    };
    union // Size=0x20
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x2a0 Size=0x20
        struct // Size=0x20
        {
            unsigned char SuspendSemaphorefill[28];// Offset=0x2a0 Size=0x1c
            unsigned long SListFaultCount;// Offset=0x2bc Size=0x4
        };
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x2c0 Size=0x10
    void *SListFaultAddress;// Offset=0x2d0 Size=0x8
    LONG64 ReadOperationCount;// Offset=0x2d8 Size=0x8
    LONG64 WriteOperationCount;// Offset=0x2e0 Size=0x8
    LONG64 OtherOperationCount;// Offset=0x2e8 Size=0x8
    LONG64 ReadTransferCount;// Offset=0x2f0 Size=0x8
    LONG64 WriteTransferCount;// Offset=0x2f8 Size=0x8
    LONG64 OtherTransferCount;// Offset=0x300 Size=0x8
};

struct _ETHREAD// Size=0x410
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x308
    union _LARGE_INTEGER CreateTime;// Offset=0x308 Size=0x8
    union // Size=0x10
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x310 Size=0x8
        struct _LIST_ENTRY LpcReplyChain;// Offset=0x310 Size=0x10
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x310 Size=0x10
    };
    union // Size=0x8
    {
        long ExitStatus;// Offset=0x320 Size=0x4
        void *OfsChain;// Offset=0x320 Size=0x8
    };
    struct _LIST_ENTRY PostBlockList;// Offset=0x328 Size=0x10
    union // Size=0x8
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x338 Size=0x8
        struct _ETHREAD *ReaperLink;// Offset=0x338 Size=0x8
        void *KeyedWaitValue;// Offset=0x338 Size=0x8
    };
    ULONG64 ActiveTimerListLock;// Offset=0x340 Size=0x8
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x348 Size=0x10
    struct _CLIENT_ID Cid;// Offset=0x358 Size=0x10
    union // Size=0x20
    {
        struct _KSEMAPHORE LpcReplySemaphore;// Offset=0x368 Size=0x20
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x368 Size=0x20
    };
    union // Size=0x8
    {
        void *LpcReplyMessage;// Offset=0x388 Size=0x8
        void *LpcWaitingOnPort;// Offset=0x388 Size=0x8
    };
    struct _PS_IMPERSONATION_INFORMATION *ImpersonationInfo;// Offset=0x390 Size=0x8
    struct _LIST_ENTRY IrpList;// Offset=0x398 Size=0x10
    ULONG64 TopLevelIrp;// Offset=0x3a8 Size=0x8
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x3b0 Size=0x8
    struct _EPROCESS *ThreadsProcess;// Offset=0x3b8 Size=0x8
    void *StartAddress;// Offset=0x3c0 Size=0x8
    union // Size=0x8
    {
        void *Win32StartAddress;// Offset=0x3c8 Size=0x8
        struct // Size=0x8
        {
            unsigned long LpcReceivedMessageId;// Offset=0x3c8 Size=0x4
            unsigned char __align0[4];// Offset=0x3cc Size=0x4
        };
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x3d0 Size=0x10
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x3e0 Size=0x8
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x3e8 Size=0x8
    unsigned long LpcReplyMessageId;// Offset=0x3f0 Size=0x4
    unsigned long ReadClusterSize;// Offset=0x3f4 Size=0x4
    unsigned long GrantedAccess;// Offset=0x3f8 Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x3fc Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x3fc Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long DeadThread:1;// Offset=0x3fc Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x3fc Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x3fc Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x3fc Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x3fc Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x3fc Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x3fc Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x3fc Size=0x4 BitOffset=0x8 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x400 Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x400 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x400 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x400 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x400 Size=0x4 BitOffset=0x3 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x404 Size=0x4
        struct // Size=0x4
        {
            unsigned char LpcReceivedMsgIdValid:1;// Offset=0x404 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char LpcExitThreadCalled:1;// Offset=0x404 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char AddressSpaceOwner:1;// Offset=0x404 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x404 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x404 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemWorkingSetExclusive:1;// Offset=0x404 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemWorkingSetShared:1;// Offset=0x404 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x404 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x405 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char ApcNeeded:1;// Offset=0x405 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char __align1[2];// Offset=0x406 Size=0x2
        };
    };
    unsigned char ForwardClusterOnly;// Offset=0x408 Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x409 Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x40a Size=0x1
};

struct _KDESCRIPTOR// Size=0x10
{
    unsigned short Pad[3];// Offset=0x0 Size=0x6
    unsigned short Limit;// Offset=0x6 Size=0x2
    void *Base;// Offset=0x8 Size=0x8
};

struct _KSPECIAL_REGISTERS// Size=0xd8
{
    ULONG64 Cr0;// Offset=0x0 Size=0x8
    ULONG64 Cr2;// Offset=0x8 Size=0x8
    ULONG64 Cr3;// Offset=0x10 Size=0x8
    ULONG64 Cr4;// Offset=0x18 Size=0x8
    ULONG64 KernelDr0;// Offset=0x20 Size=0x8
    ULONG64 KernelDr1;// Offset=0x28 Size=0x8
    ULONG64 KernelDr2;// Offset=0x30 Size=0x8
    ULONG64 KernelDr3;// Offset=0x38 Size=0x8
    ULONG64 KernelDr6;// Offset=0x40 Size=0x8
    ULONG64 KernelDr7;// Offset=0x48 Size=0x8
    struct _KDESCRIPTOR Gdtr;// Offset=0x50 Size=0x10
    struct _KDESCRIPTOR Idtr;// Offset=0x60 Size=0x10
    unsigned short Tr;// Offset=0x70 Size=0x2
    unsigned short Ldtr;// Offset=0x72 Size=0x2
    unsigned long MxCsr;// Offset=0x74 Size=0x4
    ULONG64 DebugControl;// Offset=0x78 Size=0x8
    ULONG64 LastBranchToRip;// Offset=0x80 Size=0x8
    ULONG64 LastBranchFromRip;// Offset=0x88 Size=0x8
    ULONG64 LastExceptionToRip;// Offset=0x90 Size=0x8
    ULONG64 LastExceptionFromRip;// Offset=0x98 Size=0x8
    ULONG64 Cr8;// Offset=0xa0 Size=0x8
    ULONG64 MsrGsBase;// Offset=0xa8 Size=0x8
    ULONG64 MsrGsSwap;// Offset=0xb0 Size=0x8
    ULONG64 MsrStar;// Offset=0xb8 Size=0x8
    ULONG64 MsrLStar;// Offset=0xc0 Size=0x8
    ULONG64 MsrCStar;// Offset=0xc8 Size=0x8
    ULONG64 MsrSyscallMask;// Offset=0xd0 Size=0x8
};

struct _KPROCESSOR_STATE// Size=0x5b0
{
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x0 Size=0xd8
    unsigned char __align0[8];// Offset=0xd8 Size=0x8
    struct _CONTEXT ContextFrame;// Offset=0xe0 Size=0x4d0
};

struct _KDPC_XP64// Size=0x40
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned char Number;// Offset=0x2 Size=0x1
    unsigned char Expedite;// Offset=0x3 Size=0x1
    unsigned char __align0[4];// Offset=0x4 Size=0x4
    struct _LIST_ENTRY DpcListEntry;// Offset=0x8 Size=0x10
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0x18 Size=0x8
    void *DeferredContext;// Offset=0x20 Size=0x8
    void *SystemArgument1;// Offset=0x28 Size=0x8
    void *SystemArgument2;// Offset=0x30 Size=0x8
    void *DpcData;// Offset=0x38 Size=0x8
};

struct _PROCESSOR_POWER_STATE// Size=0x170
{
    void  ( *IdleFunction)(struct _PROCESSOR_POWER_STATE *);// Offset=0x0 Size=0x8
    unsigned long Idle0KernelTimeLimit;// Offset=0x8 Size=0x4
    unsigned long Idle0LastTime;// Offset=0xc Size=0x4
    void *IdleHandlers;// Offset=0x10 Size=0x8
    void *IdleState;// Offset=0x18 Size=0x8
    unsigned long IdleHandlersCount;// Offset=0x20 Size=0x4
    unsigned char __align0[4];// Offset=0x24 Size=0x4
    ULONG64 LastCheck;// Offset=0x28 Size=0x8
    struct PROCESSOR_IDLE_TIMES IdleTimes;// Offset=0x30 Size=0x20
    unsigned long IdleTime1;// Offset=0x50 Size=0x4
    unsigned long PromotionCheck;// Offset=0x54 Size=0x4
    unsigned long IdleTime2;// Offset=0x58 Size=0x4
    unsigned char CurrentThrottle;// Offset=0x5c Size=0x1
    unsigned char ThermalThrottleLimit;// Offset=0x5d Size=0x1
    unsigned char CurrentThrottleIndex;// Offset=0x5e Size=0x1
    unsigned char ThermalThrottleIndex;// Offset=0x5f Size=0x1
    unsigned long LastKernelUserTime;// Offset=0x60 Size=0x4
    unsigned long LastIdleThreadKernelTime;// Offset=0x64 Size=0x4
    unsigned long PackageIdleStartTime;// Offset=0x68 Size=0x4
    unsigned long PackageIdleTime;// Offset=0x6c Size=0x4
    unsigned long DebugCount;// Offset=0x70 Size=0x4
    unsigned long LastSysTime;// Offset=0x74 Size=0x4
    ULONG64 TotalIdleStateTime[3];// Offset=0x78 Size=0x18
    unsigned long TotalIdleTransitions[3];// Offset=0x90 Size=0xc
    unsigned char __align1[4];// Offset=0x9c Size=0x4
    ULONG64 PreviousC3StateTime;// Offset=0xa0 Size=0x8
    unsigned char KneeThrottleIndex;// Offset=0xa8 Size=0x1
    unsigned char ThrottleLimitIndex;// Offset=0xa9 Size=0x1
    unsigned char PerfStatesCount;// Offset=0xaa Size=0x1
    unsigned char ProcessorMinThrottle;// Offset=0xab Size=0x1
    unsigned char ProcessorMaxThrottle;// Offset=0xac Size=0x1
    unsigned char EnableIdleAccounting;// Offset=0xad Size=0x1
    unsigned char LastC3Percentage;// Offset=0xae Size=0x1
    unsigned char LastAdjustedBusyPercentage;// Offset=0xaf Size=0x1
    unsigned long PromotionCount;// Offset=0xb0 Size=0x4
    unsigned long DemotionCount;// Offset=0xb4 Size=0x4
    unsigned long ErrorCount;// Offset=0xb8 Size=0x4
    unsigned long RetryCount;// Offset=0xbc Size=0x4
    unsigned long Flags;// Offset=0xc0 Size=0x4
    unsigned char __align2[4];// Offset=0xc4 Size=0x4
    union _LARGE_INTEGER PerfCounterFrequency;// Offset=0xc8 Size=0x8
    unsigned long PerfTickCount;// Offset=0xd0 Size=0x4
    unsigned char __align3[4];// Offset=0xd4 Size=0x4
    struct _KTIMER PerfTimer;// Offset=0xd8 Size=0x40
    struct _KDPC_XP64 PerfDpc;// Offset=0x118 Size=0x40
    struct PROCESSOR_PERF_STATE *PerfStates;// Offset=0x158 Size=0x8
    long  ( *PerfSetThrottle)(unsigned char );// Offset=0x160 Size=0x8
    unsigned long LastC3KernelUserTime;// Offset=0x168 Size=0x4
    unsigned long LastPackageIdleTime;// Offset=0x16c Size=0x4
};

struct _KDPC_DATA// Size=0x20
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x10
    ULONG64 DpcLock;// Offset=0x10 Size=0x8
    long volatile DpcQueueDepth;// Offset=0x18 Size=0x4
    unsigned long DpcCount;// Offset=0x1c Size=0x4
};

struct _KREQUEST_PACKET// Size=0x20
{
    void *CurrentPacket[3];// Offset=0x0 Size=0x18
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x18 Size=0x8
};

struct _REQUEST_MAILBOX// Size=0x40
{
    LONG64 RequestSummary;// Offset=0x0 Size=0x8
    union // Size=0x38
    {
        struct _KREQUEST_PACKET RequestPacket;// Offset=0x8 Size=0x20
        void *Virtual[7];// Offset=0x8 Size=0x38
    };
};

typedef struct _KPRCB// Size=0x2480
{
    unsigned long MxCsr;// Offset=0x0 Size=0x4
    unsigned char Number;// Offset=0x4 Size=0x1
    unsigned char NestingLevel;// Offset=0x5 Size=0x1
    unsigned char InterruptRequest;// Offset=0x6 Size=0x1
    unsigned char IdleHalt;// Offset=0x7 Size=0x1
    struct _KTHREAD *CurrentThread;// Offset=0x8 Size=0x8
    struct _KTHREAD *NextThread;// Offset=0x10 Size=0x8
    struct _KTHREAD *IdleThread;// Offset=0x18 Size=0x8
    ULONG64 UserRsp;// Offset=0x20 Size=0x8
    ULONG64 RspBase;// Offset=0x28 Size=0x8
    ULONG64 PrcbLock;// Offset=0x30 Size=0x8
    ULONG64 SetMember;// Offset=0x38 Size=0x8
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x40 Size=0x5b0
    char CpuType;// Offset=0x5f0 Size=0x1
    char CpuID;// Offset=0x5f1 Size=0x1
    unsigned short CpuStep;// Offset=0x5f2 Size=0x2
    unsigned long MHz;// Offset=0x5f4 Size=0x4
    ULONG64 HalReserved[8];// Offset=0x5f8 Size=0x40
    unsigned short MinorVersion;// Offset=0x638 Size=0x2
    unsigned short MajorVersion;// Offset=0x63a Size=0x2
    unsigned char BuildType;// Offset=0x63c Size=0x1
    unsigned char CpuVendor;// Offset=0x63d Size=0x1
    unsigned char InitialApicId;// Offset=0x63e Size=0x1
    unsigned char LogicalProcessorsPerPhysicalProcessor;// Offset=0x63f Size=0x1
    unsigned long ApicMask;// Offset=0x640 Size=0x4
    unsigned char CFlushSize;// Offset=0x644 Size=0x1
    unsigned char PrcbPad0x[3];// Offset=0x645 Size=0x3
    void *AcpiReserved;// Offset=0x648 Size=0x8
    ULONG64 PrcbPad00[4];// Offset=0x650 Size=0x20
    struct _KSPIN_LOCK_QUEUE LockQueue[33];// Offset=0x670 Size=0x210
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x880 Size=0x100
    struct _PP_LOOKASIDE_LIST PPNPagedLookasideList[32];// Offset=0x980 Size=0x200
    struct _PP_LOOKASIDE_LIST PPPagedLookasideList[32];// Offset=0xb80 Size=0x200
    ULONG64 volatile PacketBarrier;// Offset=0xd80 Size=0x8
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0xd88 Size=0x8
    long volatile MmPageFaultCount;// Offset=0xd90 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0xd94 Size=0x4
    long volatile MmTransitionCount;// Offset=0xd98 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0xd9c Size=0x4
    long volatile MmDemandZeroCount;// Offset=0xda0 Size=0x4
    long volatile MmPageReadCount;// Offset=0xda4 Size=0x4
    long volatile MmPageReadIoCount;// Offset=0xda8 Size=0x4
    long volatile MmCacheReadCount;// Offset=0xdac Size=0x4
    long volatile MmCacheIoCount;// Offset=0xdb0 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0xdb4 Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0xdb8 Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0xdbc Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0xdc0 Size=0x4
    long LookasideIrpFloat;// Offset=0xdc4 Size=0x4
    unsigned long KeSystemCalls;// Offset=0xdc8 Size=0x4
    long volatile IoReadOperationCount;// Offset=0xdcc Size=0x4
    long volatile IoWriteOperationCount;// Offset=0xdd0 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0xdd4 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0xdd8 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0xde0 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0xde8 Size=0x8
    unsigned long KeContextSwitches;// Offset=0xdf0 Size=0x4
    unsigned char PrcbPad2[12];// Offset=0xdf4 Size=0xc
    ULONG64 volatile TargetSet;// Offset=0xe00 Size=0x8
    unsigned long volatile IpiFrozen;// Offset=0xe08 Size=0x4
    unsigned char PrcbPad3[116];// Offset=0xe0c Size=0x74
    struct _REQUEST_MAILBOX RequestMailbox[64];// Offset=0xe80 Size=0x1000
    ULONG64 volatile SenderSummary;// Offset=0x1e80 Size=0x8
    unsigned char PrcbPad4[120];// Offset=0x1e88 Size=0x78
    struct _KDPC_DATA DpcData[2];// Offset=0x1f00 Size=0x40
    void *DpcStack;// Offset=0x1f40 Size=0x8
    void *SavedRsp;// Offset=0x1f48 Size=0x8
    long MaximumDpcQueueDepth;// Offset=0x1f50 Size=0x4
    unsigned long DpcRequestRate;// Offset=0x1f54 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x1f58 Size=0x4
    unsigned char volatile DpcInterruptRequested;// Offset=0x1f5c Size=0x1
    unsigned char volatile DpcThreadRequested;// Offset=0x1f5d Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x1f5e Size=0x1
    unsigned char volatile DpcThreadActive;// Offset=0x1f5f Size=0x1
    union // Size=0x8
    {
        ULONG64 volatile TimerHand;// Offset=0x1f60 Size=0x8
        ULONG64 volatile TimerRequest;// Offset=0x1f60 Size=0x8
    };
    long TickOffset;// Offset=0x1f68 Size=0x4
    long MasterOffset;// Offset=0x1f6c Size=0x4
    unsigned long DpcLastCount;// Offset=0x1f70 Size=0x4
    unsigned char ThreadDpcEnable;// Offset=0x1f74 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x1f75 Size=0x1
    unsigned char PrcbPad50;// Offset=0x1f76 Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x1f77 Size=0x1
    long DpcSetEventRequest;// Offset=0x1f78 Size=0x4
    long PrcbPad40;// Offset=0x1f7c Size=0x4
    void *DpcThread;// Offset=0x1f80 Size=0x8
    struct _KEVENT DpcEvent;// Offset=0x1f88 Size=0x18
    struct _KDPC_XP64 CallDpc;// Offset=0x1fa0 Size=0x40
    ULONG64 PrcbPad7[4];// Offset=0x1fe0 Size=0x20
    struct _LIST_ENTRY WaitListHead;// Offset=0x2000 Size=0x10
    unsigned long ReadySummary;// Offset=0x2010 Size=0x4
    unsigned long QueueIndex;// Offset=0x2014 Size=0x4
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x2018 Size=0x200
    unsigned long InterruptCount;// Offset=0x2218 Size=0x4
    unsigned long KernelTime;// Offset=0x221c Size=0x4
    unsigned long UserTime;// Offset=0x2220 Size=0x4
    unsigned long DpcTime;// Offset=0x2224 Size=0x4
    unsigned long InterruptTime;// Offset=0x2228 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x222c Size=0x4
    unsigned char SkipTick;// Offset=0x2230 Size=0x1
    unsigned char DebuggerSavedIRQL;// Offset=0x2231 Size=0x1
    unsigned char PollSlot;// Offset=0x2232 Size=0x1
    unsigned char PrcbPad8[13];// Offset=0x2233 Size=0xd
    struct _KNODE *ParentNode;// Offset=0x2240 Size=0x8
    ULONG64 MultiThreadProcessorSet;// Offset=0x2248 Size=0x8
    struct _KPRCB *MultiThreadSetMaster;// Offset=0x2250 Size=0x8
    long Sleeping;// Offset=0x2258 Size=0x4
    unsigned long PrcbPad90[1];// Offset=0x225c Size=0x4
    unsigned long DebugDpcTime;// Offset=0x2260 Size=0x4
    unsigned long PageColor;// Offset=0x2264 Size=0x4
    unsigned long NodeColor;// Offset=0x2268 Size=0x4
    unsigned long NodeShiftedColor;// Offset=0x226c Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x2270 Size=0x4
    unsigned char PrcbPad9[12];// Offset=0x2274 Size=0xc
    unsigned long CcFastReadNoWait;// Offset=0x2280 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x2284 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x2288 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x228c Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x2290 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x2294 Size=0x4
    unsigned long KeAlignmentFixupCount;// Offset=0x2298 Size=0x4
    unsigned long KeDcacheFlushCount;// Offset=0x229c Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x22a0 Size=0x4
    unsigned long KeFirstLevelTbFills;// Offset=0x22a4 Size=0x4
    unsigned long KeFloatingEmulationCount;// Offset=0x22a8 Size=0x4
    unsigned long KeIcacheFlushCount;// Offset=0x22ac Size=0x4
    unsigned long KeSecondLevelTbFills;// Offset=0x22b0 Size=0x4
    unsigned char VendorString[13];// Offset=0x22b4 Size=0xd
    unsigned char PrcbPad10[2];// Offset=0x22c1 Size=0x2
    unsigned char __align0[1];// Offset=0x22c3 Size=0x1
    unsigned long FeatureBits;// Offset=0x22c4 Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x22c8 Size=0x8
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0x22d0 Size=0x170
    struct _CACHE_DESCRIPTOR Cache[5];// Offset=0x2440 Size=0x3c
    unsigned long CacheCount;// Offset=0x247c Size=0x4
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0x140
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x128
    struct _EX_PUSH_LOCK Lock;// Offset=0x128 Size=0x8
    struct _DEVICE_MAP *DeviceMap;// Offset=0x130 Size=0x8
    unsigned long SessionId;// Offset=0x138 Size=0x4
} *POBJECT_DIRECTORY;

#endif //                  Windows XP/2003 x64



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Vista x64 internal kernel definitions                     //
#if (NTDDI_VERSION >= NTDDI_VISTA) && (NTDDI_VERSION <= NTDDI_VISTASP4) && defined(_AMD64_)

typedef struct _MM_SESSION_SPACE// Size=0x1e00
{
    long volatile ReferenceCount;// Offset=0x0 Size=0x4
    union {
        ULONG LongFlags;
        MM_SESSION_SPACE_FLAGS Flags;
    } u;
    unsigned long SessionId;// Offset=0x8 Size=0x4
// ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x8
{
    union // Size=0x8
    {
        struct // Size=0x8
        {
            ULONG64 Locked:1;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x1
            ULONG64 Waiting:1;// Offset=0x0 Size=0x8 BitOffset=0x1 BitSize=0x1
            ULONG64 Waking:1;// Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
            ULONG64 MultipleShared:1;// Offset=0x0 Size=0x8 BitOffset=0x3 BitSize=0x1
            ULONG64 Shared:60;// Offset=0x0 Size=0x8 BitOffset=0x4 BitSize=0x3c
        };
        ULONG64 Value;// Offset=0x0 Size=0x8
        void *Ptr;// Offset=0x0 Size=0x8
    };
};

struct _EX_FAST_REF// Size=0x8
{
    union // Size=0x8
    {
        void *Object;// Offset=0x0 Size=0x8
        ULONG64 RefCnt:4;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x4
        ULONG64 Value;// Offset=0x0 Size=0x8
    };
};

typedef struct _MMADDRESS_NODE// Size=0x28
{
    union {// Offset=0x0 Size=0x8
        LONG64 Balance:2;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x8
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x8 Size=0x8
    struct _MMADDRESS_NODE *RightChild;// Offset=0x10 Size=0x8
    ULONG64 StartingVpn;// Offset=0x18 Size=0x8
    ULONG64 EndingVpn;// Offset=0x20 Size=0x8
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE// Size=0x40
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x28
    ULONG64 DepthOfTree:5;// Offset=0x28 Size=0x8 BitOffset=0x0 BitSize=0x5
    ULONG64 Unused:3;// Offset=0x28 Size=0x8 BitOffset=0x5 BitSize=0x3
    ULONG64 NumberGenericTableElements:56;// Offset=0x28 Size=0x8 BitOffset=0x8 BitSize=0x38
    void *NodeHint;// Offset=0x30 Size=0x8
    void *NodeFreeHint;// Offset=0x38 Size=0x8
};

struct _KPROCESS// Size=0xc0
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    struct _LIST_ENTRY ProfileListHead;// Offset=0x18 Size=0x10
    ULONG64 DirectoryTableBase;// Offset=0x28 Size=0x8
    ULONG64 Unused0;// Offset=0x30 Size=0x8
    unsigned short IopmOffset;// Offset=0x38 Size=0x2
    unsigned char __align0[6];// Offset=0x3a Size=0x6
    ULONG64 volatile ActiveProcessors;// Offset=0x40 Size=0x8
    unsigned long KernelTime;// Offset=0x48 Size=0x4
    unsigned long UserTime;// Offset=0x4c Size=0x4
    struct _LIST_ENTRY ReadyListHead;// Offset=0x50 Size=0x10
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x60 Size=0x8
    void *InstrumentationCallback;// Offset=0x68 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x70 Size=0x10
    ULONG64 ProcessLock;// Offset=0x80 Size=0x8
    ULONG64 Affinity;// Offset=0x88 Size=0x8
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long volatile AutoAlignment:1;// Offset=0x90 Size=0x4 BitOffset=0x0 BitSize=0x1
            long volatile DisableBoost:1;// Offset=0x90 Size=0x4 BitOffset=0x1 BitSize=0x1
            long volatile DisableQuantum:1;// Offset=0x90 Size=0x4 BitOffset=0x2 BitSize=0x1
            long volatile ReservedFlags:29;// Offset=0x90 Size=0x4 BitOffset=0x3 BitSize=0x1d
        };
        long volatile ProcessFlags;// Offset=0x90 Size=0x4
    };
    char BasePriority;// Offset=0x94 Size=0x1
    char QuantumReset;// Offset=0x95 Size=0x1
    unsigned char State;// Offset=0x96 Size=0x1
    unsigned char ThreadSeed;// Offset=0x97 Size=0x1
    unsigned char PowerState;// Offset=0x98 Size=0x1
    unsigned char IdealNode;// Offset=0x99 Size=0x1
    unsigned char Visited;// Offset=0x9a Size=0x1
    union // Size=0x1
    {
        struct _KEXECUTE_OPTIONS Flags;// Offset=0x9b Size=0x1
        unsigned char ExecuteOptions;// Offset=0x9b Size=0x1
    };
    unsigned char __align1[4];// Offset=0x9c Size=0x4
    ULONG64 StackCount;// Offset=0xa0 Size=0x8
    struct _LIST_ENTRY ProcessListEntry;// Offset=0xa8 Size=0x10
    ULONG64 volatile CycleTime;// Offset=0xb8 Size=0x8
};

struct _MMSUPPORT// Size=0x68
{
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x0 Size=0x10
    unsigned short LastTrimStamp;// Offset=0x10 Size=0x2
    unsigned short NextPageColor;// Offset=0x12 Size=0x2
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x14 Size=0x4
    unsigned long PageFaultCount;// Offset=0x18 Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x1c Size=0x4
    unsigned long ChargedWslePages;// Offset=0x20 Size=0x4
    unsigned long MinimumWorkingSetSize;// Offset=0x24 Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x28 Size=0x4
    unsigned char __align0[4];// Offset=0x2c Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x30 Size=0x8
    unsigned long Claim;// Offset=0x38 Size=0x4
    unsigned long ActualWslePages;// Offset=0x3c Size=0x4
    unsigned long WorkingSetPrivateSize;// Offset=0x40 Size=0x4
    unsigned long WorkingSetSizeOverhead;// Offset=0x44 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x48 Size=0x4
    unsigned char __align1[4];// Offset=0x4c Size=0x4
    struct _KGATE *ExitGate;// Offset=0x50 Size=0x8
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x58 Size=0x8
    void *AccessLog;// Offset=0x60 Size=0x8
};

struct _ALPC_PROCESS_CONTEXT// Size=0x20
{
    struct _EX_PUSH_LOCK Lock;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY ViewListHead;// Offset=0x8 Size=0x10
    ULONG64 volatile PagedPoolQuotaCache;// Offset=0x18 Size=0x8
};

typedef struct _EPROCESS// Size=0x3e8
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0xc0
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0xc0 Size=0x8
    union _LARGE_INTEGER CreateTime;// Offset=0xc8 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0xd0 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0xd8 Size=0x8
    void *UniqueProcessId;// Offset=0xe0 Size=0x8
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0xe8 Size=0x10
    ULONG64 QuotaUsage[3];// Offset=0xf8 Size=0x18
    ULONG64 QuotaPeak[3];// Offset=0x110 Size=0x18
    ULONG64 volatile CommitCharge;// Offset=0x128 Size=0x8
    ULONG64 PeakVirtualSize;// Offset=0x130 Size=0x8
    ULONG64 VirtualSize;// Offset=0x138 Size=0x8
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0x140 Size=0x10
    void *DebugPort;// Offset=0x150 Size=0x8
    union // Size=0x8
    {
        void *ExceptionPortData;// Offset=0x158 Size=0x8
        ULONG64 ExceptionPortValue;// Offset=0x158 Size=0x8
        ULONG64 ExceptionPortState:3;// Offset=0x158 Size=0x8 BitOffset=0x0 BitSize=0x3
    };
    struct _HANDLE_TABLE *ObjectTable;// Offset=0x160 Size=0x8
    struct _EX_FAST_REF Token;// Offset=0x168 Size=0x8
    ULONG64 WorkingSetPage;// Offset=0x170 Size=0x8
    struct _EX_PUSH_LOCK AddressCreationLock;// Offset=0x178 Size=0x8
    struct _ETHREAD *RotateInProgress;// Offset=0x180 Size=0x8
    struct _ETHREAD *ForkInProgress;// Offset=0x188 Size=0x8
    ULONG64 HardwareTrigger;// Offset=0x190 Size=0x8
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0x198 Size=0x8
    void *CloneRoot;// Offset=0x1a0 Size=0x8
    ULONG64 volatile NumberOfPrivatePages;// Offset=0x1a8 Size=0x8
    ULONG64 volatile NumberOfLockedPages;// Offset=0x1b0 Size=0x8
    void *Win32Process;// Offset=0x1b8 Size=0x8
    struct _EJOB *Job;// Offset=0x1c0 Size=0x8
    void *SectionObject;// Offset=0x1c8 Size=0x8
    void *SectionBaseAddress;// Offset=0x1d0 Size=0x8
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x1d8 Size=0x8
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x1e0 Size=0x8
    void *Win32WindowStation;// Offset=0x1e8 Size=0x8
    void *InheritedFromUniqueProcessId;// Offset=0x1f0 Size=0x8
    void *LdtInformation;// Offset=0x1f8 Size=0x8
    void *Spare;// Offset=0x200 Size=0x8
    void *VdmObjects;// Offset=0x208 Size=0x8
    void *DeviceMap;// Offset=0x210 Size=0x8
    void *EtwDataSource;// Offset=0x218 Size=0x8
    void *FreeTebHint;// Offset=0x220 Size=0x8
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x228 Size=0x8
        ULONG64 Filler;// Offset=0x228 Size=0x8
    };
    void *Session;// Offset=0x230 Size=0x8
    unsigned char ImageFileName[16];// Offset=0x238 Size=0x10
    struct _LIST_ENTRY JobLinks;// Offset=0x248 Size=0x10
    void *LockedPagesList;// Offset=0x258 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x260 Size=0x10
    void *SecurityPort;// Offset=0x270 Size=0x8
    void *Wow64Process;// Offset=0x278 Size=0x8
    unsigned long volatile ActiveThreads;// Offset=0x280 Size=0x4
    unsigned long ImagePathHash;// Offset=0x284 Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x288 Size=0x4
    long LastThreadExitStatus;// Offset=0x28c Size=0x4
    struct _PEB *Peb;// Offset=0x290 Size=0x8
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x298 Size=0x8
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x2a0 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x2a8 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x2b0 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x2b8 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x2c0 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x2c8 Size=0x8
    ULONG64 CommitChargeLimit;// Offset=0x2d0 Size=0x8
    ULONG64 volatile CommitChargePeak;// Offset=0x2d8 Size=0x8
    void *AweInfo;// Offset=0x2e0 Size=0x8
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x2e8 Size=0x8
    struct _MMSUPPORT Vm;// Offset=0x2f0 Size=0x68
    struct _LIST_ENTRY MmProcessLinks;// Offset=0x358 Size=0x10
    unsigned long ModifiedPageCount;// Offset=0x368 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags2;// Offset=0x36c Size=0x4
        struct // Size=0x4
        {
            unsigned long JobNotReallyActive:1;// Offset=0x36c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long AccountingFolded:1;// Offset=0x36c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long NewProcessReported:1;// Offset=0x36c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ExitProcessReported:1;// Offset=0x36c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long ReportCommitChanges:1;// Offset=0x36c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long LastReportMemory:1;// Offset=0x36c Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long ReportPhysicalPageChanges:1;// Offset=0x36c Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long HandleTableRundown:1;// Offset=0x36c Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long NeedsHandleRundown:1;// Offset=0x36c Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long RefTraceEnabled:1;// Offset=0x36c Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long NumaAware:1;// Offset=0x36c Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long ProtectedProcess:1;// Offset=0x36c Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long DefaultPagePriority:3;// Offset=0x36c Size=0x4 BitOffset=0xc BitSize=0x3
            unsigned long PrimaryTokenFrozen:1;// Offset=0x36c Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessVerifierTarget:1;// Offset=0x36c Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long StackRandomizationDisabled:1;// Offset=0x36c Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long AffinityPermanent:1;// Offset=0x36c Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long AffinityUpdateEnable:1;// Offset=0x36c Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long CrossSessionCreate:1;// Offset=0x36c Size=0x4 BitOffset=0x14 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x370 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x370 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x370 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x370 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x370 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x370 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x370 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x370 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x370 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x370 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x370 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x370 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x370 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x370 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long DeprioritizeViews:1;// Offset=0x370 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x370 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x370 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x370 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x370 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x370 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x370 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x370 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x370 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x370 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x370 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long SmapAllowed:1;// Offset=0x370 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long ProcessInserted:1;// Offset=0x370 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x370 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long ProcessSelfDelete:1;// Offset=0x370 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long SpareProcessFlags:1;// Offset=0x370 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x374 Size=0x4
    unsigned short Spare7;// Offset=0x378 Size=0x2
    union // Size=0x2
    {
        struct // Size=0x2
        {
            unsigned char SubSystemMinorVersion;// Offset=0x37a Size=0x1
            unsigned char SubSystemMajorVersion;// Offset=0x37b Size=0x1
        };
        unsigned short SubSystemVersion;// Offset=0x37a Size=0x2
    };
    unsigned char PriorityClass;// Offset=0x37c Size=0x1
    unsigned char __align0[3];// Offset=0x37d Size=0x3
    struct _MM_AVL_TABLE VadRoot;// Offset=0x380 Size=0x40
    unsigned long Cookie;// Offset=0x3c0 Size=0x4
    unsigned char __align1[4];// Offset=0x3c4 Size=0x4
    struct _ALPC_PROCESS_CONTEXT AlpcContext;// Offset=0x3c8 Size=0x20
} EPROCESS;

struct _CONTROL_AREA// Size=0x70
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY DereferenceList;// Offset=0x8 Size=0x10
    ULONG64 NumberOfSectionReferences;// Offset=0x18 Size=0x8
    ULONG64 NumberOfPfnReferences;// Offset=0x20 Size=0x8
    ULONG64 NumberOfMappedViews;// Offset=0x28 Size=0x8
    ULONG64 NumberOfUserReferences;// Offset=0x30 Size=0x8
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    union {
        struct // Size=0x4
        {
            unsigned short ModifiedWriteCount;// Offset=0x0 Size=0x2
            unsigned short FlushInProgressCount;// Offset=0x2 Size=0x2
        } e2;
    } u1;// Offset=0x3c Size=0x4
    struct _EX_FAST_REF FilePointer;// Offset=0x40 Size=0x8
    long volatile ControlAreaLock;// Offset=0x48 Size=0x4
    unsigned long StartingFrame;// Offset=0x4c Size=0x4
    struct _MI_SECTION_CREATION_GATE *WaitingForDeletion;// Offset=0x50 Size=0x8
    union {
        struct {
            union // Size=0x4
            {
                unsigned long NumberOfSystemCacheViews;// Offset=0x0 Size=0x4
                unsigned long ImageRelocationStartBit;// Offset=0x0 Size=0x4
            };
            union // Size=0x4
            {
                long volatile WritableUserReferences;// Offset=0x4 Size=0x4
                struct // Size=0x4
                {
                    unsigned long ImageRelocationSizeIn64k:16;// Offset=0x4 Size=0x4 BitOffset=0x0 BitSize=0x10
                    unsigned long Unused:14;// Offset=0x4 Size=0x4 BitOffset=0x10 BitSize=0xe
                    unsigned long BitMap64:1;// Offset=0x4 Size=0x4 BitOffset=0x1e BitSize=0x1
                    unsigned long ImageActive:1;// Offset=0x4 Size=0x4 BitOffset=0x1f BitSize=0x1
                };
            };
            union // Size=0x8
            {
                struct _MM_SUBSECTION_AVL_TABLE *SubsectionRoot;// Offset=0x8 Size=0x8
                struct _MI_IMAGE_SECURITY_REFERENCE *SeImageStub;// Offset=0x8 Size=0x8
            };
        } e2;
    } u2;// Offset=0x58 Size=0x10
    LONG64 volatile LockedPages;// Offset=0x68 Size=0x8
};

typedef struct _SEGMENT// Size=0x50
{
    struct _CONTROL_AREA *ControlArea;// Offset=0x0 Size=0x8
    // ...
} SEGMENT, *PSEGMENT;

typedef struct _SECTION
{
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

struct _KAPC_STATE// Size=0x30
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x20
    struct _KPROCESS *Process;// Offset=0x20 Size=0x8
    unsigned char KernelApcInProgress;// Offset=0x28 Size=0x1
    unsigned char KernelApcPending;// Offset=0x29 Size=0x1
    unsigned char UserApcPending;// Offset=0x2a Size=0x1
};

struct _KTHREAD// Size=0x330
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    ULONG64 volatile CycleTime;// Offset=0x18 Size=0x8
    ULONG64 QuantumTarget;// Offset=0x20 Size=0x8
    void *InitialStack;// Offset=0x28 Size=0x8
    void volatile *StackLimit;// Offset=0x30 Size=0x8
    void *KernelStack;// Offset=0x38 Size=0x8
    ULONG64 ThreadLock;// Offset=0x40 Size=0x8
    union // Size=0x30
    {
        struct _KAPC_STATE ApcState;// Offset=0x48 Size=0x30
        struct // Size=0x30
        {
            unsigned char ApcStateFill[43];// Offset=0x48 Size=0x2b
            char Priority;// Offset=0x73 Size=0x1
            unsigned short volatile NextProcessor;// Offset=0x74 Size=0x2
            unsigned short volatile DeferredProcessor;// Offset=0x76 Size=0x2
        };
    };
    ULONG64 ApcQueueLock;// Offset=0x78 Size=0x8
    LONG64 WaitStatus;// Offset=0x80 Size=0x8
    union // Size=0x8
    {
        struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x88 Size=0x8
        struct _KGATE *GateObject;// Offset=0x88 Size=0x8
    };
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long KernelStackResident:1;// Offset=0x90 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ReadyTransition:1;// Offset=0x90 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessReadyQueue:1;// Offset=0x90 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long WaitNext:1;// Offset=0x90 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemAffinityActive:1;// Offset=0x90 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long Alertable:1;// Offset=0x90 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long GdiFlushActive:1;// Offset=0x90 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long UserStackWalkActive:1;// Offset=0x90 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long Reserved:24;// Offset=0x90 Size=0x4 BitOffset=0x8 BitSize=0x18
        };
        long MiscFlags;// Offset=0x90 Size=0x4
    };
    unsigned char WaitReason;// Offset=0x94 Size=0x1
    unsigned char volatile SwapBusy;// Offset=0x95 Size=0x1
    unsigned char Alerted[2];// Offset=0x96 Size=0x2
    union // Size=0x10
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0x98 Size=0x10
        struct // Size=0x10
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x98 Size=0x8
            unsigned char __align0[8];// Offset=0xa0 Size=0x8
        };
    };
    struct _KQUEUE *Queue;// Offset=0xa8 Size=0x8
    void *Teb;// Offset=0xb0 Size=0x8
    union // Size=0x40
    {
        struct _KTIMER Timer;// Offset=0xb8 Size=0x40
        struct // Size=0x40
        {
            unsigned char TimerFill[60];// Offset=0xb8 Size=0x3c
            union // Size=0x4
            {
                struct // Size=0x4
                {
                    unsigned long volatile AutoAlignment:1;// Offset=0xf4 Size=0x4 BitOffset=0x0 BitSize=0x1
                    unsigned long volatile DisableBoost:1;// Offset=0xf4 Size=0x4 BitOffset=0x1 BitSize=0x1
                    unsigned long volatile EtwStackTraceApc1Inserted:1;// Offset=0xf4 Size=0x4 BitOffset=0x2 BitSize=0x1
                    unsigned long volatile EtwStackTraceApc2Inserted:1;// Offset=0xf4 Size=0x4 BitOffset=0x3 BitSize=0x1
                    unsigned long volatile CycleChargePending:1;// Offset=0xf4 Size=0x4 BitOffset=0x4 BitSize=0x1
                    unsigned long volatile CalloutActive:1;// Offset=0xf4 Size=0x4 BitOffset=0x5 BitSize=0x1
                    unsigned long volatile ApcQueueable:1;// Offset=0xf4 Size=0x4 BitOffset=0x6 BitSize=0x1
                    unsigned long volatile EnableStackSwap:1;// Offset=0xf4 Size=0x4 BitOffset=0x7 BitSize=0x1
                    unsigned long volatile GuiThread:1;// Offset=0xf4 Size=0x4 BitOffset=0x8 BitSize=0x1
                    unsigned long volatile ReservedFlags:23;// Offset=0xf4 Size=0x4 BitOffset=0x9 BitSize=0x17
                };
                long volatile ThreadFlags;// Offset=0xf4 Size=0x4
            };
        };
    };
    union // Size=0xc0
    {
        struct _KWAIT_BLOCK WaitBlock[4];// Offset=0xf8 Size=0xc0
        struct // Size=0x2c
        {
            unsigned char WaitBlockFill0[43];// Offset=0xf8 Size=0x2b
            unsigned char IdealProcessor;// Offset=0x123 Size=0x1
        };
        struct // Size=0x5c
        {
            unsigned char WaitBlockFill1[91];// Offset=0xf8 Size=0x5b
            char PreviousMode;// Offset=0x153 Size=0x1
        };
        struct // Size=0x8c
        {
            unsigned char WaitBlockFill2[139];// Offset=0xf8 Size=0x8b
            unsigned char ResourceIndex;// Offset=0x183 Size=0x1
        };
        struct // Size=0xbc
        {
            unsigned char WaitBlockFill3[187];// Offset=0xf8 Size=0xbb
            unsigned char LargeStack;// Offset=0x1b3 Size=0x1
        };
        struct // Size=0x30
        {
            unsigned char WaitBlockFill4[44];// Offset=0xf8 Size=0x2c
            unsigned long ContextSwitches;// Offset=0x124 Size=0x4
        };
        struct // Size=0x60
        {
            unsigned char WaitBlockFill5[92];// Offset=0xf8 Size=0x5c
            unsigned char volatile State;// Offset=0x154 Size=0x1
            unsigned char NpxState;// Offset=0x155 Size=0x1
            unsigned char WaitIrql;// Offset=0x156 Size=0x1
            char WaitMode;// Offset=0x157 Size=0x1
        };
        struct // Size=0x90
        {
            unsigned char WaitBlockFill6[140];// Offset=0xf8 Size=0x8c
            unsigned long WaitTime;// Offset=0x184 Size=0x4
        };
        struct // Size=0xc0
        {
            unsigned char WaitBlockFill7[188];// Offset=0xf8 Size=0xbc
            union // Size=0x4
            {
                struct // Size=0x4
                {
                    short KernelApcDisable;// Offset=0x1b4 Size=0x2
                    short SpecialApcDisable;// Offset=0x1b6 Size=0x2
                };
                unsigned long CombinedApcDisable;// Offset=0x1b4 Size=0x4
            };
        };
    };
    struct _LIST_ENTRY QueueListEntry;// Offset=0x1b8 Size=0x10
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x1c8 Size=0x8
    void *FirstArgument;// Offset=0x1d0 Size=0x8
    union // Size=0x8
    {
        void *CallbackStack;// Offset=0x1d8 Size=0x8
        ULONG64 CallbackDepth;// Offset=0x1d8 Size=0x8
    };
    unsigned char ApcStateIndex;// Offset=0x1e0 Size=0x1
    char BasePriority;// Offset=0x1e1 Size=0x1
    char PriorityDecrement;// Offset=0x1e2 Size=0x1
    unsigned char Preempted;// Offset=0x1e3 Size=0x1
    unsigned char AdjustReason;// Offset=0x1e4 Size=0x1
    char AdjustIncrement;// Offset=0x1e5 Size=0x1
    unsigned char Spare01;// Offset=0x1e6 Size=0x1
    char Saturation;// Offset=0x1e7 Size=0x1
    unsigned long SystemCallNumber;// Offset=0x1e8 Size=0x4
    unsigned long FreezeCount;// Offset=0x1ec Size=0x4
    ULONG64 UserAffinity;// Offset=0x1f0 Size=0x8
    struct _KPROCESS *Process;// Offset=0x1f8 Size=0x8
    ULONG64 volatile Affinity;// Offset=0x200 Size=0x8
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x208 Size=0x10
    union // Size=0x30
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x218 Size=0x30
        struct // Size=0x30
        {
            unsigned char SavedApcStateFill[43];// Offset=0x218 Size=0x2b
            unsigned char Spare02;// Offset=0x243 Size=0x1
            char SuspendCount;// Offset=0x244 Size=0x1
            unsigned char UserIdealProcessor;// Offset=0x245 Size=0x1
            unsigned char Spare03;// Offset=0x246 Size=0x1
            unsigned char CodePatchInProgress;// Offset=0x247 Size=0x1
        };
    };
    void volatile *Win32Thread;// Offset=0x248 Size=0x8
    void *StackBase;// Offset=0x250 Size=0x8
    union // Size=0x58
    {
        struct _KAPC SuspendApc;// Offset=0x258 Size=0x58
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x258 Size=0x1
            char Spare04;// Offset=0x259 Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x258 Size=0x3
            unsigned char QuantumReset;// Offset=0x25b Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x258 Size=0x4
            unsigned long KernelTime;// Offset=0x25c Size=0x4
        };
        struct // Size=0x48
        {
            unsigned char SuspendApcFill3[64];// Offset=0x258 Size=0x40
            struct _KPRCB *WaitPrcb;// Offset=0x298 Size=0x8
        };
        struct // Size=0x50
        {
            unsigned char SuspendApcFill4[72];// Offset=0x258 Size=0x48
            void *LegoData;// Offset=0x2a0 Size=0x8
        };
        struct // Size=0x58
        {
            unsigned char SuspendApcFill5[83];// Offset=0x258 Size=0x53
            unsigned char PowerState;// Offset=0x2ab Size=0x1
            unsigned long UserTime;// Offset=0x2ac Size=0x4
        };
    };
    union // Size=0x20
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x2b0 Size=0x20
        struct // Size=0x20
        {
            unsigned char SuspendSemaphorefill[28];// Offset=0x2b0 Size=0x1c
            unsigned long SListFaultCount;// Offset=0x2cc Size=0x4
        };
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x2d0 Size=0x10
    struct _LIST_ENTRY MutantListHead;// Offset=0x2e0 Size=0x10
    void *SListFaultAddress;// Offset=0x2f0 Size=0x8
    LONG64 ReadOperationCount;// Offset=0x2f8 Size=0x8
    LONG64 WriteOperationCount;// Offset=0x300 Size=0x8
    LONG64 OtherOperationCount;// Offset=0x308 Size=0x8
    LONG64 ReadTransferCount;// Offset=0x310 Size=0x8
    LONG64 WriteTransferCount;// Offset=0x318 Size=0x8
    LONG64 OtherTransferCount;// Offset=0x320 Size=0x8
    void volatile *MdlForLockedTeb;// Offset=0x328 Size=0x8
};

union _PS_CLIENT_SECURITY_CONTEXT// Size=0x8
{
    ULONG64 ImpersonationData;// Offset=0x0 Size=0x8
    void *ImpersonationToken;// Offset=0x0 Size=0x8
    struct // Size=0x8
    {
        ULONG64 ImpersonationLevel:2;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x2
        ULONG64 EffectiveOnly:1;// Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
    };
};

struct _ETHREAD// Size=0x450
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x330
    union _LARGE_INTEGER CreateTime;// Offset=0x330 Size=0x8
    union // Size=0x10
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x338 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x338 Size=0x10
    };
    union // Size=0x8
    {
        long ExitStatus;// Offset=0x348 Size=0x4
        void *OfsChain;// Offset=0x348 Size=0x8
    };
    union // Size=0x10
    {
        struct _LIST_ENTRY PostBlockList;// Offset=0x350 Size=0x10
        struct // Size=0x10
        {
            void *ForwardLinkShadow;// Offset=0x350 Size=0x8
            void *StartAddress;// Offset=0x358 Size=0x8
        };
    };
    union // Size=0x8
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x360 Size=0x8
        struct _ETHREAD *ReaperLink;// Offset=0x360 Size=0x8
        void *KeyedWaitValue;// Offset=0x360 Size=0x8
        void *Win32StartParameter;// Offset=0x360 Size=0x8
    };
    ULONG64 ActiveTimerListLock;// Offset=0x368 Size=0x8
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x370 Size=0x10
    struct _CLIENT_ID Cid;// Offset=0x380 Size=0x10
    union // Size=0x20
    {
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x390 Size=0x20
        struct _KSEMAPHORE AlpcWaitSemaphore;// Offset=0x390 Size=0x20
    };
    union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity;// Offset=0x3b0 Size=0x8
    struct _LIST_ENTRY IrpList;// Offset=0x3b8 Size=0x10
    ULONG64 TopLevelIrp;// Offset=0x3c8 Size=0x8
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x3d0 Size=0x8
    union _PSP_RATE_APC *RateControlApc;// Offset=0x3d8 Size=0x8
    void *Win32StartAddress;// Offset=0x3e0 Size=0x8
    void *SparePtr0;// Offset=0x3e8 Size=0x8
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x3f0 Size=0x10
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x400 Size=0x8
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x408 Size=0x8
    unsigned long ReadClusterSize;// Offset=0x410 Size=0x4
    long volatile MmLockOrdering;// Offset=0x414 Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x418 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x418 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ThreadInserted:1;// Offset=0x418 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x418 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x418 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x418 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x418 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x418 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x418 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x418 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long CopyTokenOnOpen:1;// Offset=0x418 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long ThreadIoPriority:3;// Offset=0x418 Size=0x4 BitOffset=0xa BitSize=0x3
            unsigned long ThreadPagePriority:3;// Offset=0x418 Size=0x4 BitOffset=0xd BitSize=0x3
            unsigned long RundownFail:1;// Offset=0x418 Size=0x4 BitOffset=0x10 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x41c Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x41c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x41c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x41c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ClonedThread:1;// Offset=0x41c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x41c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long RateApcState:2;// Offset=0x41c Size=0x4 BitOffset=0x5 BitSize=0x2
            unsigned long SelfTerminate:1;// Offset=0x41c Size=0x4 BitOffset=0x7 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x420 Size=0x4
        struct // Size=0x4
        {
            unsigned char Spare:1;// Offset=0x420 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char volatile StartAddressInvalid:1;// Offset=0x420 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char EtwPageFaultCalloutActive:1;// Offset=0x420 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x420 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x420 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemWorkingSetExclusive:1;// Offset=0x420 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemWorkingSetShared:1;// Offset=0x420 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x420 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x421 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceExclusive:1;// Offset=0x421 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceShared:1;// Offset=0x421 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char SuppressSymbolLoad:1;// Offset=0x421 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char Prefetching:1;// Offset=0x421 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsDynamicMemoryShared:1;// Offset=0x421 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsChangeControlAreaExclusive:1;// Offset=0x421 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsChangeControlAreaShared:1;// Offset=0x421 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char Spare1:8;// Offset=0x422 Size=0x1 BitOffset=0x0 BitSize=0x8
            unsigned char PriorityRegionActive;// Offset=0x423 Size=0x1
        };
    };
    unsigned char CacheManagerActive;// Offset=0x424 Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x425 Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x426 Size=0x1
    unsigned char __align0[1];// Offset=0x427 Size=0x1
    ULONG64 AlpcMessageId;// Offset=0x428 Size=0x8
    union // Size=0x8
    {
        void *AlpcMessage;// Offset=0x430 Size=0x8
        struct // Size=0x8
        {
            unsigned long AlpcReceiveAttributeSet;// Offset=0x430 Size=0x4
            unsigned char __align1[4];// Offset=0x434 Size=0x4
        };
    };
    struct _LIST_ENTRY AlpcWaitListEntry;// Offset=0x438 Size=0x10
    unsigned long CacheManagerCount;// Offset=0x448 Size=0x4
};

struct _KDESCRIPTOR// Size=0x10
{
    unsigned short Pad[3];// Offset=0x0 Size=0x6
    unsigned short Limit;// Offset=0x6 Size=0x2
    void *Base;// Offset=0x8 Size=0x8
};

struct _KSPECIAL_REGISTERS// Size=0xd8
{
    ULONG64 Cr0;// Offset=0x0 Size=0x8
    ULONG64 Cr2;// Offset=0x8 Size=0x8
    ULONG64 Cr3;// Offset=0x10 Size=0x8
    ULONG64 Cr4;// Offset=0x18 Size=0x8
    ULONG64 KernelDr0;// Offset=0x20 Size=0x8
    ULONG64 KernelDr1;// Offset=0x28 Size=0x8
    ULONG64 KernelDr2;// Offset=0x30 Size=0x8
    ULONG64 KernelDr3;// Offset=0x38 Size=0x8
    ULONG64 KernelDr6;// Offset=0x40 Size=0x8
    ULONG64 KernelDr7;// Offset=0x48 Size=0x8
    struct _KDESCRIPTOR Gdtr;// Offset=0x50 Size=0x10
    struct _KDESCRIPTOR Idtr;// Offset=0x60 Size=0x10
    unsigned short Tr;// Offset=0x70 Size=0x2
    unsigned short Ldtr;// Offset=0x72 Size=0x2
    unsigned long MxCsr;// Offset=0x74 Size=0x4
    ULONG64 DebugControl;// Offset=0x78 Size=0x8
    ULONG64 LastBranchToRip;// Offset=0x80 Size=0x8
    ULONG64 LastBranchFromRip;// Offset=0x88 Size=0x8
    ULONG64 LastExceptionToRip;// Offset=0x90 Size=0x8
    ULONG64 LastExceptionFromRip;// Offset=0x98 Size=0x8
    ULONG64 Cr8;// Offset=0xa0 Size=0x8
    ULONG64 MsrGsBase;// Offset=0xa8 Size=0x8
    ULONG64 MsrGsSwap;// Offset=0xb0 Size=0x8
    ULONG64 MsrStar;// Offset=0xb8 Size=0x8
    ULONG64 MsrLStar;// Offset=0xc0 Size=0x8
    ULONG64 MsrCStar;// Offset=0xc8 Size=0x8
    ULONG64 MsrSyscallMask;// Offset=0xd0 Size=0x8
};

struct _KPROCESSOR_STATE// Size=0x5b0
{
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x0 Size=0xd8
    unsigned char __align0[8];// Offset=0xd8 Size=0x8
    struct _CONTEXT ContextFrame;// Offset=0xe0 Size=0x4d0
};

struct _KDPC_VISTA64// Size=0x40
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned short volatile Number;// Offset=0x2 Size=0x2
    unsigned char __align0[4];// Offset=0x4 Size=0x4
    struct _LIST_ENTRY DpcListEntry;// Offset=0x8 Size=0x10
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0x18 Size=0x8
    void *DeferredContext;// Offset=0x20 Size=0x8
    void *SystemArgument1;// Offset=0x28 Size=0x8
    void *SystemArgument2;// Offset=0x30 Size=0x8
    void *DpcData;// Offset=0x38 Size=0x8
};

struct _PROCESSOR_POWER_STATE// Size=0x118
{
    struct _PPM_IDLE_STATES *IdleStates;// Offset=0x0 Size=0x8
    ULONG64 LastTimeCheck;// Offset=0x8 Size=0x8
    ULONG64 IdleTimeAccumulated;// Offset=0x10 Size=0x8
    union // Size=0x8
    {
        struct // Size=0x8
        {
            ULONG64 IdleTransitionTime;// Offset=0x0 Size=0x8
        } Native;// Offset=0x18 Size=0x8
        struct
        {
            ULONG64 LastIdleCheck;// Offset=0x0 Size=0x8
        } Hv;// Offset=0x18 Size=0x8
    };
    struct PPM_IDLE_ACCOUNTING *IdleAccounting;// Offset=0x20 Size=0x8
    struct _PPM_PERF_STATES *PerfStates;// Offset=0x28 Size=0x8
    unsigned long LastKernelUserTime;// Offset=0x30 Size=0x4
    unsigned long LastIdleThreadKTime;// Offset=0x34 Size=0x4
    ULONG64 LastGlobalTimeHv;// Offset=0x38 Size=0x8
    ULONG64 LastProcessorTimeHv;// Offset=0x40 Size=0x8
    unsigned char ThermalConstraint;// Offset=0x48 Size=0x1
    unsigned char LastBusyPercentage;// Offset=0x49 Size=0x1
    union {
        unsigned short AsUSHORT;// Offset=0x0 Size=0x2
        struct // Size=0x2
        {
            unsigned short PStateDomain:1;// Offset=0x0 Size=0x2 BitOffset=0x0 BitSize=0x1
            unsigned short PStateDomainIdleAccounting:1;// Offset=0x0 Size=0x2 BitOffset=0x1 BitSize=0x1
            unsigned short Reserved:14;// Offset=0x0 Size=0x2 BitOffset=0x2 BitSize=0xe
        };
    } Flags;// Offset=0x4a Size=0x2
    unsigned char __align0[4];// Offset=0x4c Size=0x4
    struct _KTIMER PerfTimer;// Offset=0x50 Size=0x40
    struct _KDPC_VISTA64 PerfDpc;// Offset=0x90 Size=0x40
    unsigned long LastSysTime;// Offset=0xd0 Size=0x4
    unsigned char __align1[4];// Offset=0xd4 Size=0x4
    struct _KPRCB *PStateMaster;// Offset=0xd8 Size=0x8
    ULONG64 PStateSet;// Offset=0xe0 Size=0x8
    unsigned long CurrentPState;// Offset=0xe8 Size=0x4
    unsigned long DesiredPState;// Offset=0xec Size=0x4
    unsigned long volatile PStateIdleStartTime;// Offset=0xf0 Size=0x4
    unsigned long PStateIdleTime;// Offset=0xf4 Size=0x4
    unsigned long LastPStateIdleTime;// Offset=0xf8 Size=0x4
    unsigned long PStateStartTime;// Offset=0xfc Size=0x4
    unsigned long DiaIndex;// Offset=0x100 Size=0x4
    unsigned long Reserved0;// Offset=0x104 Size=0x4
    ULONG64 WmiDispatchPtr;// Offset=0x108 Size=0x8
    long WmiInterfaceEnabled;// Offset=0x110 Size=0x4
};

struct _KDPC_DATA// Size=0x20
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x10
    ULONG64 DpcLock;// Offset=0x10 Size=0x8
    long volatile DpcQueueDepth;// Offset=0x18 Size=0x4
    unsigned long DpcCount;// Offset=0x1c Size=0x4
};

struct _KREQUEST_PACKET// Size=0x20
{
    void *CurrentPacket[3];// Offset=0x0 Size=0x18
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x18 Size=0x8
};

struct _REQUEST_MAILBOX// Size=0x40
{
    LONG64 RequestSummary;// Offset=0x0 Size=0x8
    union // Size=0x38
    {
        struct _KREQUEST_PACKET RequestPacket;// Offset=0x8 Size=0x20
        void *Virtual[7];// Offset=0x8 Size=0x38
    };
};

typedef struct _KPRCB// Size=0x3b20
{
    unsigned long MxCsr;// Offset=0x0 Size=0x4
    unsigned short Number;// Offset=0x4 Size=0x2
    unsigned char InterruptRequest;// Offset=0x6 Size=0x1
    unsigned char IdleHalt;// Offset=0x7 Size=0x1
    struct _KTHREAD *CurrentThread;// Offset=0x8 Size=0x8
    struct _KTHREAD *NextThread;// Offset=0x10 Size=0x8
    struct _KTHREAD *IdleThread;// Offset=0x18 Size=0x8
    unsigned char NestingLevel;// Offset=0x20 Size=0x1
    unsigned char Group;// Offset=0x21 Size=0x1
    unsigned char PrcbPad00[6];// Offset=0x22 Size=0x6
    ULONG64 RspBase;// Offset=0x28 Size=0x8
    ULONG64 PrcbLock;// Offset=0x30 Size=0x8
    ULONG64 SetMember;// Offset=0x38 Size=0x8
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x40 Size=0x5b0
    char CpuType;// Offset=0x5f0 Size=0x1
    char CpuID;// Offset=0x5f1 Size=0x1
    union // Size=0x2
    {
        unsigned short CpuStep;// Offset=0x5f2 Size=0x2
        struct // Size=0x2
        {
            unsigned char CpuStepping;// Offset=0x5f2 Size=0x1
            unsigned char CpuModel;// Offset=0x5f3 Size=0x1
        };
    };
    unsigned long MHz;// Offset=0x5f4 Size=0x4
    ULONG64 HalReserved[8];// Offset=0x5f8 Size=0x40
    unsigned short MinorVersion;// Offset=0x638 Size=0x2
    unsigned short MajorVersion;// Offset=0x63a Size=0x2
    unsigned char BuildType;// Offset=0x63c Size=0x1
    unsigned char CpuVendor;// Offset=0x63d Size=0x1
    unsigned char CoresPerPhysicalProcessor;// Offset=0x63e Size=0x1
    unsigned char LogicalProcessorsPerCore;// Offset=0x63f Size=0x1
    unsigned long ApicMask;// Offset=0x640 Size=0x4
    unsigned long CFlushSize;// Offset=0x644 Size=0x4
    void *AcpiReserved;// Offset=0x648 Size=0x8
    unsigned long InitialApicId;// Offset=0x650 Size=0x4
    unsigned long Stride;// Offset=0x654 Size=0x4
    ULONG64 PrcbPad01[3];// Offset=0x658 Size=0x18
    struct _KSPIN_LOCK_QUEUE LockQueue[49];// Offset=0x670 Size=0x310
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x980 Size=0x100
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];// Offset=0xa80 Size=0xc00
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];// Offset=0x1680 Size=0xc00
    ULONG64 volatile PacketBarrier;// Offset=0x2280 Size=0x8
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0x2288 Size=0x8
    long volatile MmPageFaultCount;// Offset=0x2290 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0x2294 Size=0x4
    long volatile MmTransitionCount;// Offset=0x2298 Size=0x4
    long volatile MmDemandZeroCount;// Offset=0x229c Size=0x4
    long volatile MmPageReadCount;// Offset=0x22a0 Size=0x4
    long volatile MmPageReadIoCount;// Offset=0x22a4 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0x22a8 Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0x22ac Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0x22b0 Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0x22b4 Size=0x4
    unsigned long KeSystemCalls;// Offset=0x22b8 Size=0x4
    unsigned long KeContextSwitches;// Offset=0x22bc Size=0x4
    unsigned long CcFastReadNoWait;// Offset=0x22c0 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x22c4 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x22c8 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x22cc Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x22d0 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x22d4 Size=0x4
    long LookasideIrpFloat;// Offset=0x22d8 Size=0x4
    long volatile IoReadOperationCount;// Offset=0x22dc Size=0x4
    long volatile IoWriteOperationCount;// Offset=0x22e0 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0x22e4 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0x22e8 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0x22f0 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0x22f8 Size=0x8
    ULONG64 volatile TargetSet;// Offset=0x2300 Size=0x8
    unsigned long volatile IpiFrozen;// Offset=0x2308 Size=0x4
    unsigned char PrcbPad3[116];// Offset=0x230c Size=0x74
    struct _REQUEST_MAILBOX RequestMailbox[64];// Offset=0x2380 Size=0x1000
    ULONG64 volatile SenderSummary;// Offset=0x3380 Size=0x8
    unsigned char PrcbPad4[120];// Offset=0x3388 Size=0x78
    struct _KDPC_DATA DpcData[2];// Offset=0x3400 Size=0x40
    void *DpcStack;// Offset=0x3440 Size=0x8
    void *SparePtr0;// Offset=0x3448 Size=0x8
    long MaximumDpcQueueDepth;// Offset=0x3450 Size=0x4
    unsigned long DpcRequestRate;// Offset=0x3454 Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x3458 Size=0x4
    unsigned char volatile DpcInterruptRequested;// Offset=0x345c Size=0x1
    unsigned char volatile DpcThreadRequested;// Offset=0x345d Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x345e Size=0x1
    unsigned char volatile DpcThreadActive;// Offset=0x345f Size=0x1
    union // Size=0x8
    {
        ULONG64 volatile TimerHand;// Offset=0x3460 Size=0x8
        ULONG64 volatile TimerRequest;// Offset=0x3460 Size=0x8
    };
    long TickOffset;// Offset=0x3468 Size=0x4
    long MasterOffset;// Offset=0x346c Size=0x4
    unsigned long DpcLastCount;// Offset=0x3470 Size=0x4
    unsigned char ThreadDpcEnable;// Offset=0x3474 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x3475 Size=0x1
    unsigned char PrcbPad50;// Offset=0x3476 Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x3477 Size=0x1
    long DpcSetEventRequest;// Offset=0x3478 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x347c Size=0x4
    struct _KEVENT DpcEvent;// Offset=0x3480 Size=0x18
    void *PrcbPad51;// Offset=0x3498 Size=0x8
    struct _KDPC_VISTA64 CallDpc;// Offset=0x34a0 Size=0x40
    long ClockKeepAlive;// Offset=0x34e0 Size=0x4
    unsigned char ClockCheckSlot;// Offset=0x34e4 Size=0x1
    unsigned char ClockPollCycle;// Offset=0x34e5 Size=0x1
    unsigned char PrcbPad6[2];// Offset=0x34e6 Size=0x2
    long DpcWatchdogPeriod;// Offset=0x34e8 Size=0x4
    long DpcWatchdogCount;// Offset=0x34ec Size=0x4
    ULONG64 PrcbPad70[2];// Offset=0x34f0 Size=0x10
    struct _LIST_ENTRY WaitListHead;// Offset=0x3500 Size=0x10
    ULONG64 WaitLock;// Offset=0x3510 Size=0x8
    unsigned long ReadySummary;// Offset=0x3518 Size=0x4
    unsigned long QueueIndex;// Offset=0x351c Size=0x4
    ULONG64 PrcbPad71[12];// Offset=0x3520 Size=0x60
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x3580 Size=0x200
    unsigned long InterruptCount;// Offset=0x3780 Size=0x4
    unsigned long KernelTime;// Offset=0x3784 Size=0x4
    unsigned long UserTime;// Offset=0x3788 Size=0x4
    unsigned long DpcTime;// Offset=0x378c Size=0x4
    unsigned long InterruptTime;// Offset=0x3790 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x3794 Size=0x4
    unsigned char SkipTick;// Offset=0x3798 Size=0x1
    unsigned char DebuggerSavedIRQL;// Offset=0x3799 Size=0x1
    unsigned char PollSlot;// Offset=0x379a Size=0x1
    unsigned char PrcbPad80[5];// Offset=0x379b Size=0x5
    unsigned long DpcTimeCount;// Offset=0x37a0 Size=0x4
    unsigned long DpcTimeLimit;// Offset=0x37a4 Size=0x4
    unsigned long PeriodicCount;// Offset=0x37a8 Size=0x4
    unsigned long PeriodicBias;// Offset=0x37ac Size=0x4
    ULONG64 PrcbPad81[2];// Offset=0x37b0 Size=0x10
    struct _KNODE *ParentNode;// Offset=0x37c0 Size=0x8
    ULONG64 MultiThreadProcessorSet;// Offset=0x37c8 Size=0x8
    struct _KPRCB *MultiThreadSetMaster;// Offset=0x37d0 Size=0x8
    ULONG64 StartCycles;// Offset=0x37d8 Size=0x8
    long volatile MmSpinLockOrdering;// Offset=0x37e0 Size=0x4
    unsigned long PageColor;// Offset=0x37e4 Size=0x4
    unsigned long NodeColor;// Offset=0x37e8 Size=0x4
    unsigned long NodeShiftedColor;// Offset=0x37ec Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x37f0 Size=0x4
    long Sleeping;// Offset=0x37f4 Size=0x4
    ULONG64 CycleTime;// Offset=0x37f8 Size=0x8
    unsigned long CcFastMdlReadNoWait;// Offset=0x3800 Size=0x4
    unsigned long CcFastMdlReadWait;// Offset=0x3804 Size=0x4
    unsigned long CcFastMdlReadNotPossible;// Offset=0x3808 Size=0x4
    unsigned long CcMapDataNoWait;// Offset=0x380c Size=0x4
    unsigned long CcMapDataWait;// Offset=0x3810 Size=0x4
    unsigned long CcPinMappedDataCount;// Offset=0x3814 Size=0x4
    unsigned long CcPinReadNoWait;// Offset=0x3818 Size=0x4
    unsigned long CcPinReadWait;// Offset=0x381c Size=0x4
    unsigned long CcMdlReadNoWait;// Offset=0x3820 Size=0x4
    unsigned long CcMdlReadWait;// Offset=0x3824 Size=0x4
    unsigned long CcLazyWriteHotSpots;// Offset=0x3828 Size=0x4
    unsigned long CcLazyWriteIos;// Offset=0x382c Size=0x4
    unsigned long CcLazyWritePages;// Offset=0x3830 Size=0x4
    unsigned long CcDataFlushes;// Offset=0x3834 Size=0x4
    unsigned long CcDataPages;// Offset=0x3838 Size=0x4
    unsigned long CcLostDelayedWrites;// Offset=0x383c Size=0x4
    unsigned long CcFastReadResourceMiss;// Offset=0x3840 Size=0x4
    unsigned long CcCopyReadWaitMiss;// Offset=0x3844 Size=0x4
    unsigned long CcFastMdlReadResourceMiss;// Offset=0x3848 Size=0x4
    unsigned long CcMapDataNoWaitMiss;// Offset=0x384c Size=0x4
    unsigned long CcMapDataWaitMiss;// Offset=0x3850 Size=0x4
    unsigned long CcPinReadNoWaitMiss;// Offset=0x3854 Size=0x4
    unsigned long CcPinReadWaitMiss;// Offset=0x3858 Size=0x4
    unsigned long CcMdlReadNoWaitMiss;// Offset=0x385c Size=0x4
    unsigned long CcMdlReadWaitMiss;// Offset=0x3860 Size=0x4
    unsigned long CcReadAheadIos;// Offset=0x3864 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0x3868 Size=0x4
    long volatile MmCacheReadCount;// Offset=0x386c Size=0x4
    long volatile MmCacheIoCount;// Offset=0x3870 Size=0x4
    unsigned long PrcbPad91[3];// Offset=0x3874 Size=0xc
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0x3880 Size=0x118
    unsigned long KeAlignmentFixupCount;// Offset=0x3998 Size=0x4
    unsigned char VendorString[13];// Offset=0x399c Size=0xd
    unsigned char PrcbPad10[3];// Offset=0x39a9 Size=0x3
    unsigned long FeatureBits;// Offset=0x39ac Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x39b0 Size=0x8
    struct _KDPC_VISTA64 DpcWatchdogDpc;// Offset=0x39b8 Size=0x40
    struct _KTIMER DpcWatchdogTimer;// Offset=0x39f8 Size=0x40
    struct _CACHE_DESCRIPTOR Cache[5];// Offset=0x3a38 Size=0x3c
    unsigned long CacheCount;// Offset=0x3a74 Size=0x4
    unsigned long volatile CachedCommit;// Offset=0x3a78 Size=0x4
    unsigned long volatile CachedResidentAvailable;// Offset=0x3a7c Size=0x4
    void *HyperPte;// Offset=0x3a80 Size=0x8
    void *WheaInfo;// Offset=0x3a88 Size=0x8
    void *EtwSupport;// Offset=0x3a90 Size=0x8
    unsigned char __align0[8];// Offset=0x3a98 Size=0x8
    union _SLIST_HEADER InterruptObjectPool;// Offset=0x3aa0 Size=0x10
    union _SLIST_HEADER HypercallPageList;// Offset=0x3ab0 Size=0x10
    void *HypercallPageVirtual;// Offset=0x3ac0 Size=0x8
    void *VirtualApicAssist;// Offset=0x3ac8 Size=0x8
    ULONG64 *StatisticsPage;// Offset=0x3ad0 Size=0x8
    void *RateControl;// Offset=0x3ad8 Size=0x8
    ULONG64 CacheProcessorMask[5];// Offset=0x3ae0 Size=0x28
    ULONG64 PackageProcessorSet;// Offset=0x3b08 Size=0x8
    ULONG64 CoreProcessorSet;// Offset=0x3b10 Size=0x8
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0x150
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x128
    struct _EX_PUSH_LOCK Lock;// Offset=0x128 Size=0x8
    struct _DEVICE_MAP *DeviceMap;// Offset=0x130 Size=0x8
    unsigned long SessionId;// Offset=0x138 Size=0x4
    unsigned char __align0[4];// Offset=0x13c Size=0x4
    void *NamespaceEntry;// Offset=0x140 Size=0x8
    unsigned long Flags;// Offset=0x148 Size=0x4
} *POBJECT_DIRECTORY;

#endif //                  Vista x64 internal kernel definitions



///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows 7 x64 internal kernel definitions                         //
#if (NTDDI_VERSION >= NTDDI_WIN7) && (NTDDI_VERSION <= NTDDI_WIN7) && defined(_AMD64_)

typedef struct _MM_SESSION_SPACE// Size=0x1f80
{
    long volatile ReferenceCount;// Offset=0x0 Size=0x4
    union {
        ULONG LongFlags;
        MM_SESSION_SPACE_FLAGS Flags;
    } u;// Offset=0x4 Size=0x4
    unsigned long SessionId;// Offset=0x8 Size=0x4
// ...
} MM_SESSION_SPACE, *PMM_SESSION_SPACE;    

struct _EX_PUSH_LOCK// Size=0x8
{
    union // Size=0x8
    {
        struct // Size=0x8
        {
            ULONG64 Locked:1;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x1
            ULONG64 Waiting:1;// Offset=0x0 Size=0x8 BitOffset=0x1 BitSize=0x1
            ULONG64 Waking:1;// Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
            ULONG64 MultipleShared:1;// Offset=0x0 Size=0x8 BitOffset=0x3 BitSize=0x1
            ULONG64 Shared:60;// Offset=0x0 Size=0x8 BitOffset=0x4 BitSize=0x3c
        };
        ULONG64 Value;// Offset=0x0 Size=0x8
        void *Ptr;// Offset=0x0 Size=0x8
    };
};

struct _EX_FAST_REF// Size=0x8
{
    union // Size=0x8
    {
        void *Object;// Offset=0x0 Size=0x8
        ULONG64 RefCnt:4;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x4
        ULONG64 Value;// Offset=0x0 Size=0x8
    };
};

typedef struct _MMADDRESS_NODE// Size=0x28
{
    union {// Offset=0x0 Size=0x8
        LONG64 Balance:2;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x2
        struct _MMADDRESS_NODE *Parent;// Offset=0x0 Size=0x8
    } u1;
    struct _MMADDRESS_NODE *LeftChild;// Offset=0x8 Size=0x8
    struct _MMADDRESS_NODE *RightChild;// Offset=0x10 Size=0x8
    ULONG64 StartingVpn;// Offset=0x18 Size=0x8
    ULONG64 EndingVpn;// Offset=0x20 Size=0x8
} MMADDRESS_NODE, *PMMADDRESS_NODE;

struct _MM_AVL_TABLE// Size=0x40
{
    struct _MMADDRESS_NODE BalancedRoot;// Offset=0x0 Size=0x28
    ULONG64 DepthOfTree:5;// Offset=0x28 Size=0x8 BitOffset=0x0 BitSize=0x5
    ULONG64 Unused:3;// Offset=0x28 Size=0x8 BitOffset=0x5 BitSize=0x3
    ULONG64 NumberGenericTableElements:56;// Offset=0x28 Size=0x8 BitOffset=0x8 BitSize=0x38
    void *NodeHint;// Offset=0x30 Size=0x8
    void *NodeFreeHint;// Offset=0x38 Size=0x8
};

struct _KAFFINITY_EX// Size=0x28
{
    unsigned short Count;// Offset=0x0 Size=0x2
    unsigned short Size;// Offset=0x2 Size=0x2
    unsigned long Reserved;// Offset=0x4 Size=0x4
    ULONG64 Bitmap[4];// Offset=0x8 Size=0x20
};

union _KSTACK_COUNT// Size=0x4
{
    long volatile Value;// Offset=0x0 Size=0x4
    struct // Size=0x4
    {
        unsigned long volatile State:3;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
        unsigned long StackCount:29;// Offset=0x0 Size=0x4 BitOffset=0x3 BitSize=0x1d
    };
};

union _KGDTENTRY64// Size=0x10
{
    union // Size=0x10
    {
        struct // Size=0x10
        {
            unsigned short LimitLow;// Offset=0x0 Size=0x2
            unsigned short BaseLow;// Offset=0x2 Size=0x2
            union // Size=0x4
            {
                struct {
                    unsigned char BaseMiddle;// Offset=0x0 Size=0x1
                    unsigned char Flags1;// Offset=0x1 Size=0x1
                    unsigned char Flags2;// Offset=0x2 Size=0x1
                    unsigned char BaseHigh;// Offset=0x3 Size=0x1
                } Bytes;// Offset=0x4 Size=0x4
                struct {
                    unsigned long BaseMiddle:8;// Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x8
                    unsigned long Type:5;// Offset=0x0 Size=0x4 BitOffset=0x8 BitSize=0x5
                    unsigned long Dpl:2;// Offset=0x0 Size=0x4 BitOffset=0xd BitSize=0x2
                    unsigned long Present:1;// Offset=0x0 Size=0x4 BitOffset=0xf BitSize=0x1
                    unsigned long LimitHigh:4;// Offset=0x0 Size=0x4 BitOffset=0x10 BitSize=0x4
                    unsigned long System:1;// Offset=0x0 Size=0x4 BitOffset=0x14 BitSize=0x1
                    unsigned long LongMode:1;// Offset=0x0 Size=0x4 BitOffset=0x15 BitSize=0x1
                    unsigned long DefaultBig:1;// Offset=0x0 Size=0x4 BitOffset=0x16 BitSize=0x1
                    unsigned long Granularity:1;// Offset=0x0 Size=0x4 BitOffset=0x17 BitSize=0x1
                    unsigned long BaseHigh:8;// Offset=0x0 Size=0x4 BitOffset=0x18 BitSize=0x8
                } Bits;// Offset=0x4 Size=0x4
            };
            unsigned long BaseUpper;// Offset=0x8 Size=0x4
            unsigned long MustBeZero;// Offset=0xc Size=0x4
        };
        ULONG64 Alignment;// Offset=0x0 Size=0x8
    };
};

struct _KPROCESS// Size=0x160
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    struct _LIST_ENTRY ProfileListHead;// Offset=0x18 Size=0x10
    ULONG64 DirectoryTableBase;// Offset=0x28 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x30 Size=0x10
    ULONG64 ProcessLock;// Offset=0x40 Size=0x8
    struct _KAFFINITY_EX Affinity;// Offset=0x48 Size=0x28
    struct _LIST_ENTRY ReadyListHead;// Offset=0x70 Size=0x10
    struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0x80 Size=0x8
    struct _KAFFINITY_EX volatile ActiveProcessors;// Offset=0x88 Size=0x28
    union // Size=0x4
    {
        struct // Size=0x4
        {
            long volatile AutoAlignment:1;// Offset=0xb0 Size=0x4 BitOffset=0x0 BitSize=0x1
            long volatile DisableBoost:1;// Offset=0xb0 Size=0x4 BitOffset=0x1 BitSize=0x1
            long volatile DisableQuantum:1;// Offset=0xb0 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long volatile ActiveGroupsMask:4;// Offset=0xb0 Size=0x4 BitOffset=0x3 BitSize=0x4
            long volatile ReservedFlags:25;// Offset=0xb0 Size=0x4 BitOffset=0x7 BitSize=0x19
        };
        long volatile ProcessFlags;// Offset=0xb0 Size=0x4
    };
    char BasePriority;// Offset=0xb4 Size=0x1
    char QuantumReset;// Offset=0xb5 Size=0x1
    unsigned char Visited;// Offset=0xb6 Size=0x1
    unsigned char Unused3;// Offset=0xb7 Size=0x1
    unsigned long ThreadSeed[4];// Offset=0xb8 Size=0x10
    unsigned short IdealNode[4];// Offset=0xc8 Size=0x8
    unsigned short IdealGlobalNode;// Offset=0xd0 Size=0x2
    struct _KEXECUTE_OPTIONS Flags;// Offset=0xd2 Size=0x1
    unsigned char Unused1;// Offset=0xd3 Size=0x1
    unsigned long Unused2;// Offset=0xd4 Size=0x4
    unsigned long Unused4;// Offset=0xd8 Size=0x4
    union _KSTACK_COUNT StackCount;// Offset=0xdc Size=0x4
    struct _LIST_ENTRY ProcessListEntry;// Offset=0xe0 Size=0x10
    ULONG64 volatile CycleTime;// Offset=0xf0 Size=0x8
    unsigned long KernelTime;// Offset=0xf8 Size=0x4
    unsigned long UserTime;// Offset=0xfc Size=0x4
    void *InstrumentationCallback;// Offset=0x100 Size=0x8
    union _KGDTENTRY64 LdtSystemDescriptor;// Offset=0x108 Size=0x10
    void *LdtBaseAddress;// Offset=0x118 Size=0x8
    struct _KGUARDED_MUTEX LdtProcessLock;// Offset=0x120 Size=0x38
    unsigned short LdtFreeSelectorHint;// Offset=0x158 Size=0x2
    unsigned short LdtTableLength;// Offset=0x15a Size=0x2
    unsigned char __endalign[4];// Offset=0x15c Size=0x4
};

struct _MMSUPPORT// Size=0x88
{
    struct _EX_PUSH_LOCK WorkingSetMutex;// Offset=0x0 Size=0x8
    struct _KGATE *ExitGate;// Offset=0x8 Size=0x8
    void *AccessLog;// Offset=0x10 Size=0x8
    struct _LIST_ENTRY WorkingSetExpansionLinks;// Offset=0x18 Size=0x10
    unsigned long AgeDistribution[7];// Offset=0x28 Size=0x1c
    unsigned long MinimumWorkingSetSize;// Offset=0x44 Size=0x4
    unsigned long WorkingSetSize;// Offset=0x48 Size=0x4
    unsigned long WorkingSetPrivateSize;// Offset=0x4c Size=0x4
    unsigned long MaximumWorkingSetSize;// Offset=0x50 Size=0x4
    unsigned long ChargedWslePages;// Offset=0x54 Size=0x4
    unsigned long ActualWslePages;// Offset=0x58 Size=0x4
    unsigned long WorkingSetSizeOverhead;// Offset=0x5c Size=0x4
    unsigned long PeakWorkingSetSize;// Offset=0x60 Size=0x4
    unsigned long HardFaultCount;// Offset=0x64 Size=0x4
    struct _MMWSL *VmWorkingSetList;// Offset=0x68 Size=0x8
    unsigned short NextPageColor;// Offset=0x70 Size=0x2
    unsigned short LastTrimStamp;// Offset=0x72 Size=0x2
    unsigned long PageFaultCount;// Offset=0x74 Size=0x4
    unsigned long RepurposeCount;// Offset=0x78 Size=0x4
    unsigned long Spare[2];// Offset=0x7c Size=0x8
    struct _MMSUPPORT_FLAGS Flags;// Offset=0x84 Size=0x4
};

struct _ALPC_PROCESS_CONTEXT// Size=0x20
{
    struct _EX_PUSH_LOCK Lock;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY ViewListHead;// Offset=0x8 Size=0x10
    ULONG64 volatile PagedPoolQuotaCache;// Offset=0x18 Size=0x8
};

typedef struct _EPROCESS// Size=0x4d0
{
    struct _KPROCESS Pcb;// Offset=0x0 Size=0x160
    struct _EX_PUSH_LOCK ProcessLock;// Offset=0x160 Size=0x8
    union _LARGE_INTEGER CreateTime;// Offset=0x168 Size=0x8
    union _LARGE_INTEGER ExitTime;// Offset=0x170 Size=0x8
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x178 Size=0x8
    void *UniqueProcessId;// Offset=0x180 Size=0x8
    struct _LIST_ENTRY ActiveProcessLinks;// Offset=0x188 Size=0x10
    ULONG64 ProcessQuotaUsage[2];// Offset=0x198 Size=0x10
    ULONG64 ProcessQuotaPeak[2];// Offset=0x1a8 Size=0x10
    ULONG64 volatile CommitCharge;// Offset=0x1b8 Size=0x8
    struct _EPROCESS_QUOTA_BLOCK *QuotaBlock;// Offset=0x1c0 Size=0x8
    struct _PS_CPU_QUOTA_BLOCK *CpuQuotaBlock;// Offset=0x1c8 Size=0x8
    ULONG64 PeakVirtualSize;// Offset=0x1d0 Size=0x8
    ULONG64 VirtualSize;// Offset=0x1d8 Size=0x8
    struct _LIST_ENTRY SessionProcessLinks;// Offset=0x1e0 Size=0x10
    void *DebugPort;// Offset=0x1f0 Size=0x8
    union // Size=0x8
    {
        void *ExceptionPortData;// Offset=0x1f8 Size=0x8
        ULONG64 ExceptionPortValue;// Offset=0x1f8 Size=0x8
        ULONG64 ExceptionPortState:3;// Offset=0x1f8 Size=0x8 BitOffset=0x0 BitSize=0x3
    };
    struct _HANDLE_TABLE *ObjectTable;// Offset=0x200 Size=0x8
    struct _EX_FAST_REF Token;// Offset=0x208 Size=0x8
    ULONG64 WorkingSetPage;// Offset=0x210 Size=0x8
    struct _EX_PUSH_LOCK AddressCreationLock;// Offset=0x218 Size=0x8
    struct _ETHREAD *RotateInProgress;// Offset=0x220 Size=0x8
    struct _ETHREAD *ForkInProgress;// Offset=0x228 Size=0x8
    ULONG64 HardwareTrigger;// Offset=0x230 Size=0x8
    struct _MM_AVL_TABLE *PhysicalVadRoot;// Offset=0x238 Size=0x8
    void *CloneRoot;// Offset=0x240 Size=0x8
    ULONG64 volatile NumberOfPrivatePages;// Offset=0x248 Size=0x8
    ULONG64 volatile NumberOfLockedPages;// Offset=0x250 Size=0x8
    void *Win32Process;// Offset=0x258 Size=0x8
    struct _EJOB volatile *Job;// Offset=0x260 Size=0x8
    void *SectionObject;// Offset=0x268 Size=0x8
    void *SectionBaseAddress;// Offset=0x270 Size=0x8
    unsigned long Cookie;// Offset=0x278 Size=0x4
    unsigned long UmsScheduledThreads;// Offset=0x27c Size=0x4
    struct _PAGEFAULT_HISTORY *WorkingSetWatch;// Offset=0x280 Size=0x8
    void *Win32WindowStation;// Offset=0x288 Size=0x8
    void *InheritedFromUniqueProcessId;// Offset=0x290 Size=0x8
    void *LdtInformation;// Offset=0x298 Size=0x8
    void *Spare;// Offset=0x2a0 Size=0x8
    ULONG64 ConsoleHostProcess;// Offset=0x2a8 Size=0x8
    void *DeviceMap;// Offset=0x2b0 Size=0x8
    void *EtwDataSource;// Offset=0x2b8 Size=0x8
    void *FreeTebHint;// Offset=0x2c0 Size=0x8
    void *FreeUmsTebHint;// Offset=0x2c8 Size=0x8
    union // Size=0x8
    {
        struct _HARDWARE_PTE PageDirectoryPte;// Offset=0x2d0 Size=0x8
        ULONG64 Filler;// Offset=0x2d0 Size=0x8
    };
    void *Session;// Offset=0x2d8 Size=0x8
    unsigned char ImageFileName[15];// Offset=0x2e0 Size=0xf
    unsigned char PriorityClass;// Offset=0x2ef Size=0x1
    struct _LIST_ENTRY JobLinks;// Offset=0x2f0 Size=0x10
    void *LockedPagesList;// Offset=0x300 Size=0x8
    struct _LIST_ENTRY ThreadListHead;// Offset=0x308 Size=0x10
    void *SecurityPort;// Offset=0x318 Size=0x8
    void *Wow64Process;// Offset=0x320 Size=0x8
    unsigned long volatile ActiveThreads;// Offset=0x328 Size=0x4
    unsigned long ImagePathHash;// Offset=0x32c Size=0x4
    unsigned long DefaultHardErrorProcessing;// Offset=0x330 Size=0x4
    long LastThreadExitStatus;// Offset=0x334 Size=0x4
    struct _PEB *Peb;// Offset=0x338 Size=0x8
    struct _EX_FAST_REF PrefetchTrace;// Offset=0x340 Size=0x8
    union _LARGE_INTEGER ReadOperationCount;// Offset=0x348 Size=0x8
    union _LARGE_INTEGER WriteOperationCount;// Offset=0x350 Size=0x8
    union _LARGE_INTEGER OtherOperationCount;// Offset=0x358 Size=0x8
    union _LARGE_INTEGER ReadTransferCount;// Offset=0x360 Size=0x8
    union _LARGE_INTEGER WriteTransferCount;// Offset=0x368 Size=0x8
    union _LARGE_INTEGER OtherTransferCount;// Offset=0x370 Size=0x8
    ULONG64 CommitChargeLimit;// Offset=0x378 Size=0x8
    ULONG64 volatile CommitChargePeak;// Offset=0x380 Size=0x8
    void *AweInfo;// Offset=0x388 Size=0x8
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;// Offset=0x390 Size=0x8
    struct _MMSUPPORT Vm;// Offset=0x398 Size=0x88
    struct _LIST_ENTRY MmProcessLinks;// Offset=0x420 Size=0x10
    void *HighestUserAddress;// Offset=0x430 Size=0x8
    unsigned long ModifiedPageCount;// Offset=0x438 Size=0x4
    union // Size=0x4
    {
        unsigned long Flags2;// Offset=0x43c Size=0x4
        struct // Size=0x4
        {
            unsigned long JobNotReallyActive:1;// Offset=0x43c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long AccountingFolded:1;// Offset=0x43c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long NewProcessReported:1;// Offset=0x43c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ExitProcessReported:1;// Offset=0x43c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long ReportCommitChanges:1;// Offset=0x43c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long LastReportMemory:1;// Offset=0x43c Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long ReportPhysicalPageChanges:1;// Offset=0x43c Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long HandleTableRundown:1;// Offset=0x43c Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long NeedsHandleRundown:1;// Offset=0x43c Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long RefTraceEnabled:1;// Offset=0x43c Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long NumaAware:1;// Offset=0x43c Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long ProtectedProcess:1;// Offset=0x43c Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long DefaultPagePriority:3;// Offset=0x43c Size=0x4 BitOffset=0xc BitSize=0x3
            unsigned long PrimaryTokenFrozen:1;// Offset=0x43c Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessVerifierTarget:1;// Offset=0x43c Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long StackRandomizationDisabled:1;// Offset=0x43c Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long AffinityPermanent:1;// Offset=0x43c Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long AffinityUpdateEnable:1;// Offset=0x43c Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long PropagateNode:1;// Offset=0x43c Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long ExplicitAffinity:1;// Offset=0x43c Size=0x4 BitOffset=0x15 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long Flags;// Offset=0x440 Size=0x4
        struct // Size=0x4
        {
            unsigned long CreateReported:1;// Offset=0x440 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long NoDebugInherit:1;// Offset=0x440 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessExiting:1;// Offset=0x440 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ProcessDelete:1;// Offset=0x440 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Wow64SplitPages:1;// Offset=0x440 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long VmDeleted:1;// Offset=0x440 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long OutswapEnabled:1;// Offset=0x440 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long Outswapped:1;// Offset=0x440 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ForkFailed:1;// Offset=0x440 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long Wow64VaSpace4Gb:1;// Offset=0x440 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long AddressSpaceInitialized:2;// Offset=0x440 Size=0x4 BitOffset=0xa BitSize=0x2
            unsigned long SetTimerResolution:1;// Offset=0x440 Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x440 Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long DeprioritizeViews:1;// Offset=0x440 Size=0x4 BitOffset=0xe BitSize=0x1
            unsigned long WriteWatch:1;// Offset=0x440 Size=0x4 BitOffset=0xf BitSize=0x1
            unsigned long ProcessInSession:1;// Offset=0x440 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long OverrideAddressSpace:1;// Offset=0x440 Size=0x4 BitOffset=0x11 BitSize=0x1
            unsigned long HasAddressSpace:1;// Offset=0x440 Size=0x4 BitOffset=0x12 BitSize=0x1
            unsigned long LaunchPrefetched:1;// Offset=0x440 Size=0x4 BitOffset=0x13 BitSize=0x1
            unsigned long InjectInpageErrors:1;// Offset=0x440 Size=0x4 BitOffset=0x14 BitSize=0x1
            unsigned long VmTopDown:1;// Offset=0x440 Size=0x4 BitOffset=0x15 BitSize=0x1
            unsigned long ImageNotifyDone:1;// Offset=0x440 Size=0x4 BitOffset=0x16 BitSize=0x1
            unsigned long PdeUpdateNeeded:1;// Offset=0x440 Size=0x4 BitOffset=0x17 BitSize=0x1
            unsigned long VdmAllowed:1;// Offset=0x440 Size=0x4 BitOffset=0x18 BitSize=0x1
            unsigned long CrossSessionCreate:1;// Offset=0x440 Size=0x4 BitOffset=0x19 BitSize=0x1
            unsigned long ProcessInserted:1;// Offset=0x440 Size=0x4 BitOffset=0x1a BitSize=0x1
            unsigned long DefaultIoPriority:3;// Offset=0x440 Size=0x4 BitOffset=0x1b BitSize=0x3
            unsigned long ProcessSelfDelete:1;// Offset=0x440 Size=0x4 BitOffset=0x1e BitSize=0x1
            unsigned long SetTimerResolutionLink:1;// Offset=0x440 Size=0x4 BitOffset=0x1f BitSize=0x1
        };
    };
    long ExitStatus;// Offset=0x444 Size=0x4
    struct _MM_AVL_TABLE VadRoot;// Offset=0x448 Size=0x40
    struct _ALPC_PROCESS_CONTEXT AlpcContext;// Offset=0x488 Size=0x20
    struct _LIST_ENTRY TimerResolutionLink;// Offset=0x4a8 Size=0x10
    unsigned long RequestedTimerResolution;// Offset=0x4b8 Size=0x4
    unsigned long ActiveThreadsHighWatermark;// Offset=0x4bc Size=0x4
    unsigned long SmallestTimerResolution;// Offset=0x4c0 Size=0x4
    unsigned char __align0[4];// Offset=0x4c4 Size=0x4
    struct _PO_DIAG_STACK_RECORD *TimerResolutionStackRecord;// Offset=0x4c8 Size=0x8
} EPROCESS;

struct _CONTROL_AREA// Size=0x80
{
    struct _SEGMENT *Segment;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY DereferenceList;// Offset=0x8 Size=0x10
    ULONG64 NumberOfSectionReferences;// Offset=0x18 Size=0x8
    ULONG64 NumberOfPfnReferences;// Offset=0x20 Size=0x8
    ULONG64 NumberOfMappedViews;// Offset=0x28 Size=0x8
    ULONG64 NumberOfUserReferences;// Offset=0x30 Size=0x8
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;// Offset=0x38 Size=0x4
    unsigned long FlushInProgressCount;// Offset=0x3c Size=0x4
    struct _EX_FAST_REF FilePointer;// Offset=0x40 Size=0x8
    long volatile ControlAreaLock;// Offset=0x48 Size=0x4
    union // Size=0x4
    {
        unsigned long ModifiedWriteCount;// Offset=0x4c Size=0x4
        unsigned long StartingFrame;// Offset=0x4c Size=0x4
    };
    struct _MI_SECTION_CREATION_GATE *WaitingForDeletion;// Offset=0x50 Size=0x8
    union {
        struct {
            union // Size=0x4
            {
                unsigned long NumberOfSystemCacheViews;// Offset=0x0 Size=0x4
                unsigned long ImageRelocationStartBit;// Offset=0x0 Size=0x4
            };
            union // Size=0x4
            {
                long volatile WritableUserReferences;// Offset=0x4 Size=0x4
                struct // Size=0x4
                {
                    unsigned long ImageRelocationSizeIn64k:16;// Offset=0x4 Size=0x4 BitOffset=0x0 BitSize=0x10
                    unsigned long Unused:14;// Offset=0x4 Size=0x4 BitOffset=0x10 BitSize=0xe
                    unsigned long BitMap64:1;// Offset=0x4 Size=0x4 BitOffset=0x1e BitSize=0x1
                    unsigned long ImageActive:1;// Offset=0x4 Size=0x4 BitOffset=0x1f BitSize=0x1
                };
            };
            union // Size=0x8
            {
                struct _MM_SUBSECTION_AVL_TABLE *SubsectionRoot;// Offset=0x8 Size=0x8
                struct _MI_IMAGE_SECURITY_REFERENCE *SeImageStub;// Offset=0x8 Size=0x8
            };
        } e2;
    } u2;// Offset=0x58 Size=0x10
    LONG64 volatile LockedPages;// Offset=0x68 Size=0x8
    struct _LIST_ENTRY ViewList;// Offset=0x70 Size=0x10
};

typedef struct _SEGMENT// Size=0x50
{
    struct _CONTROL_AREA *ControlArea;// Offset=0x0 Size=0x8
    // ...
} SEGMENT, *PSEGMENT;

typedef struct _SECTION
{
    MMADDRESS_NODE Address;
    PSEGMENT Segment;
    LARGE_INTEGER SizeOfSection;
    union {
        ULONG LongFlags;
        MMSECTION_FLAGS Flags;
    } u;
    MM_PROTECTION_MASK InitialPageProtection;
} SECTION, *PSECTION;

struct _KAPC_STATE// Size=0x30
{
    struct _LIST_ENTRY ApcListHead[2];// Offset=0x0 Size=0x20
    struct _KPROCESS *Process;// Offset=0x20 Size=0x8
    unsigned char KernelApcInProgress;// Offset=0x28 Size=0x1
    unsigned char KernelApcPending;// Offset=0x29 Size=0x1
    unsigned char UserApcPending;// Offset=0x2a Size=0x1
};

union _KWAIT_STATUS_REGISTER// Size=0x1
{
    unsigned char Flags;// Offset=0x0 Size=0x1
    struct // Size=0x1
    {
        unsigned char State:2;// Offset=0x0 Size=0x1 BitOffset=0x0 BitSize=0x2
        unsigned char Affinity:1;// Offset=0x0 Size=0x1 BitOffset=0x2 BitSize=0x1
        unsigned char Priority:1;// Offset=0x0 Size=0x1 BitOffset=0x3 BitSize=0x1
        unsigned char Apc:1;// Offset=0x0 Size=0x1 BitOffset=0x4 BitSize=0x1
        unsigned char UserApc:1;// Offset=0x0 Size=0x1 BitOffset=0x5 BitSize=0x1
        unsigned char Alert:1;// Offset=0x0 Size=0x1 BitOffset=0x6 BitSize=0x1
        unsigned char Unused:1;// Offset=0x0 Size=0x1 BitOffset=0x7 BitSize=0x1
    };
};

struct _KTHREAD// Size=0x360
{
    struct _DISPATCHER_HEADER Header;// Offset=0x0 Size=0x18
    ULONG64 volatile CycleTime;// Offset=0x18 Size=0x8
    ULONG64 QuantumTarget;// Offset=0x20 Size=0x8
    void *InitialStack;// Offset=0x28 Size=0x8
    void volatile *StackLimit;// Offset=0x30 Size=0x8
    void *KernelStack;// Offset=0x38 Size=0x8
    ULONG64 ThreadLock;// Offset=0x40 Size=0x8
    union _KWAIT_STATUS_REGISTER WaitRegister;// Offset=0x48 Size=0x1
    unsigned char volatile Running;// Offset=0x49 Size=0x1
    unsigned char Alerted[2];// Offset=0x4a Size=0x2
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long KernelStackResident:1;// Offset=0x4c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ReadyTransition:1;// Offset=0x4c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long ProcessReadyQueue:1;// Offset=0x4c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long WaitNext:1;// Offset=0x4c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long SystemAffinityActive:1;// Offset=0x4c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long Alertable:1;// Offset=0x4c Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long GdiFlushActive:1;// Offset=0x4c Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long UserStackWalkActive:1;// Offset=0x4c Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long ApcInterruptRequest:1;// Offset=0x4c Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long ForceDeferSchedule:1;// Offset=0x4c Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long QuantumEndMigrate:1;// Offset=0x4c Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long UmsDirectedSwitchEnable:1;// Offset=0x4c Size=0x4 BitOffset=0xb BitSize=0x1
            unsigned long TimerActive:1;// Offset=0x4c Size=0x4 BitOffset=0xc BitSize=0x1
            unsigned long SystemThread:1;// Offset=0x4c Size=0x4 BitOffset=0xd BitSize=0x1
            unsigned long Reserved:18;// Offset=0x4c Size=0x4 BitOffset=0xe BitSize=0x12
        };
        long MiscFlags;// Offset=0x4c Size=0x4
    };
    union // Size=0x30
    {
        struct _KAPC_STATE ApcState;// Offset=0x50 Size=0x30
        struct // Size=0x30
        {
            unsigned char ApcStateFill[43];// Offset=0x50 Size=0x2b
            char Priority;// Offset=0x7b Size=0x1
            unsigned long volatile NextProcessor;// Offset=0x7c Size=0x4
        };
    };
    unsigned long volatile DeferredProcessor;// Offset=0x80 Size=0x4
    unsigned char __align0[4];// Offset=0x84 Size=0x4
    ULONG64 ApcQueueLock;// Offset=0x88 Size=0x8
    LONG64 volatile WaitStatus;// Offset=0x90 Size=0x8
    struct _KWAIT_BLOCK *WaitBlockList;// Offset=0x98 Size=0x8
    union // Size=0x10
    {
        struct _LIST_ENTRY WaitListEntry;// Offset=0xa0 Size=0x10
        struct // Size=0x10
        {
            struct _SINGLE_LIST_ENTRY SwapListEntry;// Offset=0xa0 Size=0x8
            unsigned char __align1[8];// Offset=0xa8 Size=0x8
        };
    };
    struct _KQUEUE volatile *Queue;// Offset=0xb0 Size=0x8
    void *Teb;// Offset=0xb8 Size=0x8
    struct _KTIMER Timer;// Offset=0xc0 Size=0x40
    union // Size=0x4
    {
        struct // Size=0x4
        {
            unsigned long volatile AutoAlignment:1;// Offset=0x100 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long volatile DisableBoost:1;// Offset=0x100 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long volatile EtwStackTraceApc1Inserted:1;// Offset=0x100 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long volatile EtwStackTraceApc2Inserted:1;// Offset=0x100 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long volatile CalloutActive:1;// Offset=0x100 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long volatile ApcQueueable:1;// Offset=0x100 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long volatile EnableStackSwap:1;// Offset=0x100 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long volatile GuiThread:1;// Offset=0x100 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long volatile UmsPerformingSyscall:1;// Offset=0x100 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long volatile VdmSafe:1;// Offset=0x100 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long volatile UmsDispatched:1;// Offset=0x100 Size=0x4 BitOffset=0xa BitSize=0x1
            unsigned long volatile ReservedFlags:21;// Offset=0x100 Size=0x4 BitOffset=0xb BitSize=0x15
        };
        long volatile ThreadFlags;// Offset=0x100 Size=0x4
    };
    unsigned long Spare0;// Offset=0x104 Size=0x4
    union // Size=0xc0
    {
        struct _KWAIT_BLOCK WaitBlock[4];// Offset=0x108 Size=0xc0
        struct // Size=0x30
        {
            unsigned char WaitBlockFill4[44];// Offset=0x108 Size=0x2c
            unsigned long ContextSwitches;// Offset=0x134 Size=0x4
        };
        struct // Size=0x60
        {
            unsigned char WaitBlockFill5[92];// Offset=0x108 Size=0x5c
            unsigned char volatile State;// Offset=0x164 Size=0x1
            char NpxState;// Offset=0x165 Size=0x1
            unsigned char WaitIrql;// Offset=0x166 Size=0x1
            char WaitMode;// Offset=0x167 Size=0x1
        };
        struct // Size=0x90
        {
            unsigned char WaitBlockFill6[140];// Offset=0x108 Size=0x8c
            unsigned long WaitTime;// Offset=0x194 Size=0x4
        };
        struct // Size=0xb8
        {
            unsigned char WaitBlockFill7[168];// Offset=0x108 Size=0xa8
            void *TebMappedLowVa;// Offset=0x1b0 Size=0x8
            struct _UMS_CONTROL_BLOCK *Ucb;// Offset=0x1b8 Size=0x8
        };
        struct // Size=0xc0
        {
            unsigned char WaitBlockFill8[188];// Offset=0x108 Size=0xbc
            union // Size=0x4
            {
                struct // Size=0x4
                {
                    short KernelApcDisable;// Offset=0x1c4 Size=0x2
                    short SpecialApcDisable;// Offset=0x1c6 Size=0x2
                };
                unsigned long CombinedApcDisable;// Offset=0x1c4 Size=0x4
            };
        };
    };
    struct _LIST_ENTRY QueueListEntry;// Offset=0x1c8 Size=0x10
    struct _KTRAP_FRAME *TrapFrame;// Offset=0x1d8 Size=0x8
    void *FirstArgument;// Offset=0x1e0 Size=0x8
    union // Size=0x8
    {
        void *CallbackStack;// Offset=0x1e8 Size=0x8
        ULONG64 CallbackDepth;// Offset=0x1e8 Size=0x8
    };
    unsigned char ApcStateIndex;// Offset=0x1f0 Size=0x1
    char BasePriority;// Offset=0x1f1 Size=0x1
    union // Size=0x1
    {
        char PriorityDecrement;// Offset=0x1f2 Size=0x1
        struct // Size=0x1
        {
            unsigned char ForegroundBoost:4;// Offset=0x1f2 Size=0x1 BitOffset=0x0 BitSize=0x4
            unsigned char UnusualBoost:4;// Offset=0x1f2 Size=0x1 BitOffset=0x4 BitSize=0x4
        };
    };
    unsigned char Preempted;// Offset=0x1f3 Size=0x1
    unsigned char AdjustReason;// Offset=0x1f4 Size=0x1
    char AdjustIncrement;// Offset=0x1f5 Size=0x1
    char PreviousMode;// Offset=0x1f6 Size=0x1
    char Saturation;// Offset=0x1f7 Size=0x1
    unsigned long SystemCallNumber;// Offset=0x1f8 Size=0x4
    unsigned long FreezeCount;// Offset=0x1fc Size=0x4
    struct _GROUP_AFFINITY volatile UserAffinity;// Offset=0x200 Size=0x10
    struct _KPROCESS *Process;// Offset=0x210 Size=0x8
    struct _GROUP_AFFINITY volatile Affinity;// Offset=0x218 Size=0x10
    unsigned long IdealProcessor;// Offset=0x228 Size=0x4
    unsigned long UserIdealProcessor;// Offset=0x22c Size=0x4
    struct _KAPC_STATE *ApcStatePointer[2];// Offset=0x230 Size=0x10
    union // Size=0x30
    {
        struct _KAPC_STATE SavedApcState;// Offset=0x240 Size=0x30
        struct // Size=0x30
        {
            unsigned char SavedApcStateFill[43];// Offset=0x240 Size=0x2b
            unsigned char WaitReason;// Offset=0x26b Size=0x1
            char SuspendCount;// Offset=0x26c Size=0x1
            char Spare1;// Offset=0x26d Size=0x1
            unsigned char CodePatchInProgress;// Offset=0x26e Size=0x1
            unsigned char __align2[1];// Offset=0x26f Size=0x1
        };
    };
    void volatile *Win32Thread;// Offset=0x270 Size=0x8
    void *StackBase;// Offset=0x278 Size=0x8
    union // Size=0x58
    {
        struct _KAPC SuspendApc;// Offset=0x280 Size=0x58
        struct // Size=0x2
        {
            unsigned char SuspendApcFill0[1];// Offset=0x280 Size=0x1
            unsigned char ResourceIndex;// Offset=0x281 Size=0x1
        };
        struct // Size=0x4
        {
            unsigned char SuspendApcFill1[3];// Offset=0x280 Size=0x3
            unsigned char QuantumReset;// Offset=0x283 Size=0x1
        };
        struct // Size=0x8
        {
            unsigned char SuspendApcFill2[4];// Offset=0x280 Size=0x4
            unsigned long KernelTime;// Offset=0x284 Size=0x4
        };
        struct // Size=0x48
        {
            unsigned char SuspendApcFill3[64];// Offset=0x280 Size=0x40
            struct _KPRCB volatile *WaitPrcb;// Offset=0x2c0 Size=0x8
        };
        struct // Size=0x50
        {
            unsigned char SuspendApcFill4[72];// Offset=0x280 Size=0x48
            void *LegoData;// Offset=0x2c8 Size=0x8
        };
        struct // Size=0x58
        {
            unsigned char SuspendApcFill5[83];// Offset=0x280 Size=0x53
            unsigned char LargeStack;// Offset=0x2d3 Size=0x1
            unsigned long UserTime;// Offset=0x2d4 Size=0x4
        };
    };
    union // Size=0x20
    {
        struct _KSEMAPHORE SuspendSemaphore;// Offset=0x2d8 Size=0x20
        struct // Size=0x20
        {
            unsigned char SuspendSemaphorefill[28];// Offset=0x2d8 Size=0x1c
            unsigned long SListFaultCount;// Offset=0x2f4 Size=0x4
        };
    };
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x2f8 Size=0x10
    struct _LIST_ENTRY MutantListHead;// Offset=0x308 Size=0x10
    void *SListFaultAddress;// Offset=0x318 Size=0x8
    LONG64 ReadOperationCount;// Offset=0x320 Size=0x8
    LONG64 WriteOperationCount;// Offset=0x328 Size=0x8
    LONG64 OtherOperationCount;// Offset=0x330 Size=0x8
    LONG64 ReadTransferCount;// Offset=0x338 Size=0x8
    LONG64 WriteTransferCount;// Offset=0x340 Size=0x8
    LONG64 OtherTransferCount;// Offset=0x348 Size=0x8
    struct _KTHREAD_COUNTERS *ThreadCounters;// Offset=0x350 Size=0x8
    struct _XSTATE_SAVE *XStateSave;// Offset=0x358 Size=0x8
};

union _PS_CLIENT_SECURITY_CONTEXT// Size=0x8
{
    ULONG64 ImpersonationData;// Offset=0x0 Size=0x8
    void *ImpersonationToken;// Offset=0x0 Size=0x8
    struct // Size=0x8
    {
        ULONG64 ImpersonationLevel:2;// Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x2
        ULONG64 EffectiveOnly:1;// Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
    };
};

struct _ETHREAD// Size=0x498
{
    struct _KTHREAD Tcb;// Offset=0x0 Size=0x360
    union _LARGE_INTEGER CreateTime;// Offset=0x360 Size=0x8
    union // Size=0x10
    {
        union _LARGE_INTEGER ExitTime;// Offset=0x368 Size=0x8
        struct _LIST_ENTRY KeyedWaitChain;// Offset=0x368 Size=0x10
    };
    long ExitStatus;// Offset=0x378 Size=0x4
    unsigned char __align0[4];// Offset=0x37c Size=0x4
    union // Size=0x10
    {
        struct _LIST_ENTRY PostBlockList;// Offset=0x380 Size=0x10
        struct // Size=0x10
        {
            void *ForwardLinkShadow;// Offset=0x380 Size=0x8
            void *StartAddress;// Offset=0x388 Size=0x8
        };
    };
    union // Size=0x8
    {
        struct _TERMINATION_PORT *TerminationPort;// Offset=0x390 Size=0x8
        struct _ETHREAD *ReaperLink;// Offset=0x390 Size=0x8
        void *KeyedWaitValue;// Offset=0x390 Size=0x8
    };
    ULONG64 ActiveTimerListLock;// Offset=0x398 Size=0x8
    struct _LIST_ENTRY ActiveTimerListHead;// Offset=0x3a0 Size=0x10
    struct _CLIENT_ID Cid;// Offset=0x3b0 Size=0x10
    union // Size=0x20
    {
        struct _KSEMAPHORE KeyedWaitSemaphore;// Offset=0x3c0 Size=0x20
        struct _KSEMAPHORE AlpcWaitSemaphore;// Offset=0x3c0 Size=0x20
    };
    union _PS_CLIENT_SECURITY_CONTEXT ClientSecurity;// Offset=0x3e0 Size=0x8
    struct _LIST_ENTRY IrpList;// Offset=0x3e8 Size=0x10
    ULONG64 TopLevelIrp;// Offset=0x3f8 Size=0x8
    struct _DEVICE_OBJECT *DeviceToVerify;// Offset=0x400 Size=0x8
    union _PSP_CPU_QUOTA_APC *CpuQuotaApc;// Offset=0x408 Size=0x8
    void *Win32StartAddress;// Offset=0x410 Size=0x8
    void *LegacyPowerObject;// Offset=0x418 Size=0x8
    struct _LIST_ENTRY ThreadListEntry;// Offset=0x420 Size=0x10
    struct _EX_RUNDOWN_REF RundownProtect;// Offset=0x430 Size=0x8
    struct _EX_PUSH_LOCK ThreadLock;// Offset=0x438 Size=0x8
    unsigned long ReadClusterSize;// Offset=0x440 Size=0x4
    long volatile MmLockOrdering;// Offset=0x444 Size=0x4
    union // Size=0x4
    {
        unsigned long CrossThreadFlags;// Offset=0x448 Size=0x4
        struct // Size=0x4
        {
            unsigned long Terminated:1;// Offset=0x448 Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ThreadInserted:1;// Offset=0x448 Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long HideFromDebugger:1;// Offset=0x448 Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ActiveImpersonationInfo:1;// Offset=0x448 Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long Reserved:1;// Offset=0x448 Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long HardErrorsAreDisabled:1;// Offset=0x448 Size=0x4 BitOffset=0x5 BitSize=0x1
            unsigned long BreakOnTermination:1;// Offset=0x448 Size=0x4 BitOffset=0x6 BitSize=0x1
            unsigned long SkipCreationMsg:1;// Offset=0x448 Size=0x4 BitOffset=0x7 BitSize=0x1
            unsigned long SkipTerminationMsg:1;// Offset=0x448 Size=0x4 BitOffset=0x8 BitSize=0x1
            unsigned long CopyTokenOnOpen:1;// Offset=0x448 Size=0x4 BitOffset=0x9 BitSize=0x1
            unsigned long ThreadIoPriority:3;// Offset=0x448 Size=0x4 BitOffset=0xa BitSize=0x3
            unsigned long ThreadPagePriority:3;// Offset=0x448 Size=0x4 BitOffset=0xd BitSize=0x3
            unsigned long RundownFail:1;// Offset=0x448 Size=0x4 BitOffset=0x10 BitSize=0x1
            unsigned long NeedsWorkingSetAging:1;// Offset=0x448 Size=0x4 BitOffset=0x11 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadPassiveFlags;// Offset=0x44c Size=0x4
        struct // Size=0x4
        {
            unsigned long ActiveExWorker:1;// Offset=0x44c Size=0x4 BitOffset=0x0 BitSize=0x1
            unsigned long ExWorkerCanWaitUser:1;// Offset=0x44c Size=0x4 BitOffset=0x1 BitSize=0x1
            unsigned long MemoryMaker:1;// Offset=0x44c Size=0x4 BitOffset=0x2 BitSize=0x1
            unsigned long ClonedThread:1;// Offset=0x44c Size=0x4 BitOffset=0x3 BitSize=0x1
            unsigned long KeyedEventInUse:1;// Offset=0x44c Size=0x4 BitOffset=0x4 BitSize=0x1
            unsigned long RateApcState:2;// Offset=0x44c Size=0x4 BitOffset=0x5 BitSize=0x2
            unsigned long SelfTerminate:1;// Offset=0x44c Size=0x4 BitOffset=0x7 BitSize=0x1
        };
    };
    union // Size=0x4
    {
        unsigned long SameThreadApcFlags;// Offset=0x450 Size=0x4
        struct // Size=0x4
        {
            unsigned char Spare:1;// Offset=0x450 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char volatile StartAddressInvalid:1;// Offset=0x450 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char EtwPageFaultCalloutActive:1;// Offset=0x450 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsProcessWorkingSetExclusive:1;// Offset=0x450 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char OwnsProcessWorkingSetShared:1;// Offset=0x450 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsSystemCacheWorkingSetExclusive:1;// Offset=0x450 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsSystemCacheWorkingSetShared:1;// Offset=0x450 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsSessionWorkingSetExclusive:1;// Offset=0x450 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsSessionWorkingSetShared:1;// Offset=0x451 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceExclusive:1;// Offset=0x451 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsProcessAddressSpaceShared:1;// Offset=0x451 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char SuppressSymbolLoad:1;// Offset=0x451 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char Prefetching:1;// Offset=0x451 Size=0x1 BitOffset=0x4 BitSize=0x1
            unsigned char OwnsDynamicMemoryShared:1;// Offset=0x451 Size=0x1 BitOffset=0x5 BitSize=0x1
            unsigned char OwnsChangeControlAreaExclusive:1;// Offset=0x451 Size=0x1 BitOffset=0x6 BitSize=0x1
            unsigned char OwnsChangeControlAreaShared:1;// Offset=0x451 Size=0x1 BitOffset=0x7 BitSize=0x1
            unsigned char OwnsPagedPoolWorkingSetExclusive:1;// Offset=0x452 Size=0x1 BitOffset=0x0 BitSize=0x1
            unsigned char OwnsPagedPoolWorkingSetShared:1;// Offset=0x452 Size=0x1 BitOffset=0x1 BitSize=0x1
            unsigned char OwnsSystemPtesWorkingSetExclusive:1;// Offset=0x452 Size=0x1 BitOffset=0x2 BitSize=0x1
            unsigned char OwnsSystemPtesWorkingSetShared:1;// Offset=0x452 Size=0x1 BitOffset=0x3 BitSize=0x1
            unsigned char TrimTrigger:2;// Offset=0x452 Size=0x1 BitOffset=0x4 BitSize=0x2
            unsigned char Spare1:2;// Offset=0x452 Size=0x1 BitOffset=0x6 BitSize=0x2
            unsigned char PriorityRegionActive;// Offset=0x453 Size=0x1
        };
    };
    unsigned char CacheManagerActive;// Offset=0x454 Size=0x1
    unsigned char DisablePageFaultClustering;// Offset=0x455 Size=0x1
    unsigned char ActiveFaultCount;// Offset=0x456 Size=0x1
    unsigned char LockOrderState;// Offset=0x457 Size=0x1
    ULONG64 AlpcMessageId;// Offset=0x458 Size=0x8
    union // Size=0x8
    {
        void *AlpcMessage;// Offset=0x460 Size=0x8
        struct // Size=0x8
        {
            unsigned long AlpcReceiveAttributeSet;// Offset=0x460 Size=0x4
            unsigned char __align1[4];// Offset=0x464 Size=0x4
        };
    };
    struct _LIST_ENTRY AlpcWaitListEntry;// Offset=0x468 Size=0x10
    unsigned long CacheManagerCount;// Offset=0x478 Size=0x4
    unsigned long IoBoostCount;// Offset=0x47c Size=0x4
    ULONG64 IrpListLock;// Offset=0x480 Size=0x8
    void *ReservedForSynchTracking;// Offset=0x488 Size=0x8
    struct _SINGLE_LIST_ENTRY CmCallbackListHead;// Offset=0x490 Size=0x8
};

struct _KDESCRIPTOR// Size=0x10
{
    unsigned short Pad[3];// Offset=0x0 Size=0x6
    unsigned short Limit;// Offset=0x6 Size=0x2
    void *Base;// Offset=0x8 Size=0x8
};

struct _KSPECIAL_REGISTERS// Size=0xd8
{
    ULONG64 Cr0;// Offset=0x0 Size=0x8
    ULONG64 Cr2;// Offset=0x8 Size=0x8
    ULONG64 Cr3;// Offset=0x10 Size=0x8
    ULONG64 Cr4;// Offset=0x18 Size=0x8
    ULONG64 KernelDr0;// Offset=0x20 Size=0x8
    ULONG64 KernelDr1;// Offset=0x28 Size=0x8
    ULONG64 KernelDr2;// Offset=0x30 Size=0x8
    ULONG64 KernelDr3;// Offset=0x38 Size=0x8
    ULONG64 KernelDr6;// Offset=0x40 Size=0x8
    ULONG64 KernelDr7;// Offset=0x48 Size=0x8
    struct _KDESCRIPTOR Gdtr;// Offset=0x50 Size=0x10
    struct _KDESCRIPTOR Idtr;// Offset=0x60 Size=0x10
    unsigned short Tr;// Offset=0x70 Size=0x2
    unsigned short Ldtr;// Offset=0x72 Size=0x2
    unsigned long MxCsr;// Offset=0x74 Size=0x4
    ULONG64 DebugControl;// Offset=0x78 Size=0x8
    ULONG64 LastBranchToRip;// Offset=0x80 Size=0x8
    ULONG64 LastBranchFromRip;// Offset=0x88 Size=0x8
    ULONG64 LastExceptionToRip;// Offset=0x90 Size=0x8
    ULONG64 LastExceptionFromRip;// Offset=0x98 Size=0x8
    ULONG64 Cr8;// Offset=0xa0 Size=0x8
    ULONG64 MsrGsBase;// Offset=0xa8 Size=0x8
    ULONG64 MsrGsSwap;// Offset=0xb0 Size=0x8
    ULONG64 MsrStar;// Offset=0xb8 Size=0x8
    ULONG64 MsrLStar;// Offset=0xc0 Size=0x8
    ULONG64 MsrCStar;// Offset=0xc8 Size=0x8
    ULONG64 MsrSyscallMask;// Offset=0xd0 Size=0x8
};

struct _KPROCESSOR_STATE// Size=0x5b0
{
    struct _KSPECIAL_REGISTERS SpecialRegisters;// Offset=0x0 Size=0xd8
    unsigned char __align0[8];// Offset=0xd8 Size=0x8
    struct _CONTEXT ContextFrame;// Offset=0xe0 Size=0x4d0
};

struct _KDPC_W7// Size=0x40
{
    unsigned char Type;// Offset=0x0 Size=0x1
    unsigned char Importance;// Offset=0x1 Size=0x1
    unsigned short volatile Number;// Offset=0x2 Size=0x2
    unsigned char __align0[4];// Offset=0x4 Size=0x4
    struct _LIST_ENTRY DpcListEntry;// Offset=0x8 Size=0x10
    void  ( *DeferredRoutine)(struct _KDPC *,void *,void *,void *);// Offset=0x18 Size=0x8
    void *DeferredContext;// Offset=0x20 Size=0x8
    void *SystemArgument1;// Offset=0x28 Size=0x8
    void *SystemArgument2;// Offset=0x30 Size=0x8
    void *DpcData;// Offset=0x38 Size=0x8
};

struct _PPM_FFH_THROTTLE_STATE_INFO// Size=0x20
{
    unsigned char EnableLogging;// Offset=0x0 Size=0x1
    unsigned char __align0[3];// Offset=0x1 Size=0x3
    unsigned long MismatchCount;// Offset=0x4 Size=0x4
    unsigned char Initialized;// Offset=0x8 Size=0x1
    unsigned char __align1[7];// Offset=0x9 Size=0x7
    ULONG64 LastValue;// Offset=0x10 Size=0x8
    union _LARGE_INTEGER LastLogTickCount;// Offset=0x18 Size=0x8
};

struct _PROC_IDLE_SNAP// Size=0x10
{
    ULONG64 Time;// Offset=0x0 Size=0x8
    ULONG64 Idle;// Offset=0x8 Size=0x8
};

struct _PROCESSOR_POWER_STATE// Size=0x100
{
    struct _PPM_IDLE_STATES *IdleStates;// Offset=0x0 Size=0x8
    ULONG64 IdleTimeLast;// Offset=0x8 Size=0x8
    ULONG64 IdleTimeTotal;// Offset=0x10 Size=0x8
    ULONG64 IdleTimeEntry;// Offset=0x18 Size=0x8
    struct _PROC_IDLE_ACCOUNTING *IdleAccounting;// Offset=0x20 Size=0x8
    enum _PROC_HYPERVISOR_STATE Hypervisor;// Offset=0x28 Size=0x4
    unsigned long PerfHistoryTotal;// Offset=0x2c Size=0x4
    unsigned char ThermalConstraint;// Offset=0x30 Size=0x1
    unsigned char PerfHistoryCount;// Offset=0x31 Size=0x1
    unsigned char PerfHistorySlot;// Offset=0x32 Size=0x1
    unsigned char Reserved;// Offset=0x33 Size=0x1
    unsigned long LastSysTime;// Offset=0x34 Size=0x4
    ULONG64 WmiDispatchPtr;// Offset=0x38 Size=0x8
    long WmiInterfaceEnabled;// Offset=0x40 Size=0x4
    unsigned char __align0[4];// Offset=0x44 Size=0x4
    struct _PPM_FFH_THROTTLE_STATE_INFO FFHThrottleStateInfo;// Offset=0x48 Size=0x20
    struct _KDPC_W7 PerfActionDpc;// Offset=0x68 Size=0x40
    long volatile PerfActionMask;// Offset=0xa8 Size=0x4
    unsigned char __align1[4];// Offset=0xac Size=0x4
    struct _PROC_IDLE_SNAP IdleCheck;// Offset=0xb0 Size=0x10
    struct _PROC_IDLE_SNAP PerfCheck;// Offset=0xc0 Size=0x10
    struct _PROC_PERF_DOMAIN *Domain;// Offset=0xd0 Size=0x8
    struct _PROC_PERF_CONSTRAINT *PerfConstraint;// Offset=0xd8 Size=0x8
    struct _PROC_PERF_LOAD *Load;// Offset=0xe0 Size=0x8
    struct _PROC_HISTORY_ENTRY *PerfHistory;// Offset=0xe8 Size=0x8
    unsigned long Utility;// Offset=0xf0 Size=0x4
    unsigned long OverUtilizedHistory;// Offset=0xf4 Size=0x4
    unsigned long volatile AffinityCount;// Offset=0xf8 Size=0x4
    unsigned long AffinityHistory;// Offset=0xfc Size=0x4
};

struct _KDPC_DATA// Size=0x20
{
    struct _LIST_ENTRY DpcListHead;// Offset=0x0 Size=0x10
    ULONG64 DpcLock;// Offset=0x10 Size=0x8
    long volatile DpcQueueDepth;// Offset=0x18 Size=0x4
    unsigned long DpcCount;// Offset=0x1c Size=0x4
};

struct _KREQUEST_PACKET// Size=0x20
{
    void *CurrentPacket[3];// Offset=0x0 Size=0x18
    void  ( *WorkerRoutine)(void *,void *,void *,void *);// Offset=0x18 Size=0x8
};

struct _REQUEST_MAILBOX// Size=0x40
{
    struct _REQUEST_MAILBOX *Next;// Offset=0x0 Size=0x8
    LONG64 RequestSummary;// Offset=0x8 Size=0x8
    struct _KREQUEST_PACKET RequestPacket;// Offset=0x10 Size=0x20
    unsigned char __endalign[16];// Offset=0x30 Size=0x10
};

struct _KTIMER_TABLE_ENTRY// Size=0x20
{
    ULONG64 Lock;// Offset=0x0 Size=0x8
    struct _LIST_ENTRY Entry;// Offset=0x8 Size=0x10
    union _ULARGE_INTEGER Time;// Offset=0x18 Size=0x8
};

struct _KTIMER_TABLE// Size=0x2200
{
    struct _KTIMER *TimerExpiry[64];// Offset=0x0 Size=0x200
    struct _KTIMER_TABLE_ENTRY TimerEntries[256];// Offset=0x200 Size=0x2000
};

typedef struct _KPRCB// Size=0x4d00
{
    unsigned long MxCsr;// Offset=0x0 Size=0x4
    unsigned char LegacyNumber;// Offset=0x4 Size=0x1
    unsigned char ReservedMustBeZero;// Offset=0x5 Size=0x1
    unsigned char InterruptRequest;// Offset=0x6 Size=0x1
    unsigned char IdleHalt;// Offset=0x7 Size=0x1
    struct _KTHREAD *CurrentThread;// Offset=0x8 Size=0x8
    struct _KTHREAD *NextThread;// Offset=0x10 Size=0x8
    struct _KTHREAD *IdleThread;// Offset=0x18 Size=0x8
    unsigned char NestingLevel;// Offset=0x20 Size=0x1
    unsigned char PrcbPad00[3];// Offset=0x21 Size=0x3
    unsigned long Number;// Offset=0x24 Size=0x4
    ULONG64 RspBase;// Offset=0x28 Size=0x8
    ULONG64 PrcbLock;// Offset=0x30 Size=0x8
    ULONG64 PrcbPad01;// Offset=0x38 Size=0x8
    struct _KPROCESSOR_STATE ProcessorState;// Offset=0x40 Size=0x5b0
    char CpuType;// Offset=0x5f0 Size=0x1
    char CpuID;// Offset=0x5f1 Size=0x1
    union // Size=0x2
    {
        unsigned short CpuStep;// Offset=0x5f2 Size=0x2
        struct // Size=0x2
        {
            unsigned char CpuStepping;// Offset=0x5f2 Size=0x1
            unsigned char CpuModel;// Offset=0x5f3 Size=0x1
        };
    };
    unsigned long MHz;// Offset=0x5f4 Size=0x4
    ULONG64 HalReserved[8];// Offset=0x5f8 Size=0x40
    unsigned short MinorVersion;// Offset=0x638 Size=0x2
    unsigned short MajorVersion;// Offset=0x63a Size=0x2
    unsigned char BuildType;// Offset=0x63c Size=0x1
    unsigned char CpuVendor;// Offset=0x63d Size=0x1
    unsigned char CoresPerPhysicalProcessor;// Offset=0x63e Size=0x1
    unsigned char LogicalProcessorsPerCore;// Offset=0x63f Size=0x1
    unsigned long ApicMask;// Offset=0x640 Size=0x4
    unsigned long CFlushSize;// Offset=0x644 Size=0x4
    void *AcpiReserved;// Offset=0x648 Size=0x8
    unsigned long InitialApicId;// Offset=0x650 Size=0x4
    unsigned long Stride;// Offset=0x654 Size=0x4
    unsigned short Group;// Offset=0x658 Size=0x2
    unsigned char __align0[6];// Offset=0x65a Size=0x6
    ULONG64 GroupSetMember;// Offset=0x660 Size=0x8
    unsigned char GroupIndex;// Offset=0x668 Size=0x1
    unsigned char __align1[7];// Offset=0x669 Size=0x7
    struct _KSPIN_LOCK_QUEUE LockQueue[17];// Offset=0x670 Size=0x110
    struct _PP_LOOKASIDE_LIST PPLookasideList[16];// Offset=0x780 Size=0x100
    struct _GENERAL_LOOKASIDE_POOL PPNPagedLookasideList[32];// Offset=0x880 Size=0xc00
    struct _GENERAL_LOOKASIDE_POOL PPPagedLookasideList[32];// Offset=0x1480 Size=0xc00
    long volatile PacketBarrier;// Offset=0x2080 Size=0x4
    unsigned char __align2[4];// Offset=0x2084 Size=0x4
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;// Offset=0x2088 Size=0x8
    long volatile MmPageFaultCount;// Offset=0x2090 Size=0x4
    long volatile MmCopyOnWriteCount;// Offset=0x2094 Size=0x4
    long volatile MmTransitionCount;// Offset=0x2098 Size=0x4
    long volatile MmDemandZeroCount;// Offset=0x209c Size=0x4
    long volatile MmPageReadCount;// Offset=0x20a0 Size=0x4
    long volatile MmPageReadIoCount;// Offset=0x20a4 Size=0x4
    long volatile MmDirtyPagesWriteCount;// Offset=0x20a8 Size=0x4
    long volatile MmDirtyWriteIoCount;// Offset=0x20ac Size=0x4
    long volatile MmMappedPagesWriteCount;// Offset=0x20b0 Size=0x4
    long volatile MmMappedWriteIoCount;// Offset=0x20b4 Size=0x4
    unsigned long KeSystemCalls;// Offset=0x20b8 Size=0x4
    unsigned long KeContextSwitches;// Offset=0x20bc Size=0x4
    unsigned long CcFastReadNoWait;// Offset=0x20c0 Size=0x4
    unsigned long CcFastReadWait;// Offset=0x20c4 Size=0x4
    unsigned long CcFastReadNotPossible;// Offset=0x20c8 Size=0x4
    unsigned long CcCopyReadNoWait;// Offset=0x20cc Size=0x4
    unsigned long CcCopyReadWait;// Offset=0x20d0 Size=0x4
    unsigned long CcCopyReadNoWaitMiss;// Offset=0x20d4 Size=0x4
    long LookasideIrpFloat;// Offset=0x20d8 Size=0x4
    long volatile IoReadOperationCount;// Offset=0x20dc Size=0x4
    long volatile IoWriteOperationCount;// Offset=0x20e0 Size=0x4
    long volatile IoOtherOperationCount;// Offset=0x20e4 Size=0x4
    union _LARGE_INTEGER IoReadTransferCount;// Offset=0x20e8 Size=0x8
    union _LARGE_INTEGER IoWriteTransferCount;// Offset=0x20f0 Size=0x8
    union _LARGE_INTEGER IoOtherTransferCount;// Offset=0x20f8 Size=0x8
    long volatile TargetCount;// Offset=0x2100 Size=0x4
    unsigned long volatile IpiFrozen;// Offset=0x2104 Size=0x4
    unsigned char __align3[120];// Offset=0x2108 Size=0x78
    struct _KDPC_DATA DpcData[2];// Offset=0x2180 Size=0x40
    void *DpcStack;// Offset=0x21c0 Size=0x8
    long MaximumDpcQueueDepth;// Offset=0x21c8 Size=0x4
    unsigned long DpcRequestRate;// Offset=0x21cc Size=0x4
    unsigned long MinimumDpcRate;// Offset=0x21d0 Size=0x4
    unsigned long DpcLastCount;// Offset=0x21d4 Size=0x4
    unsigned char ThreadDpcEnable;// Offset=0x21d8 Size=0x1
    unsigned char volatile QuantumEnd;// Offset=0x21d9 Size=0x1
    unsigned char volatile DpcRoutineActive;// Offset=0x21da Size=0x1
    unsigned char volatile IdleSchedule;// Offset=0x21db Size=0x1
    union // Size=0x4
    {
        long volatile DpcRequestSummary;// Offset=0x21dc Size=0x4
        short DpcRequestSlot[2];// Offset=0x21dc Size=0x4
        struct // Size=0x4
        {
            short NormalDpcState;// Offset=0x21dc Size=0x2
            union // Size=0x2
            {
                unsigned short volatile DpcThreadActive:1;// Offset=0x21de Size=0x2 BitOffset=0x0 BitSize=0x1
                short ThreadDpcState;// Offset=0x21de Size=0x2
            };
        };
    };
    unsigned long volatile TimerHand;// Offset=0x21e0 Size=0x4
    long MasterOffset;// Offset=0x21e4 Size=0x4
    unsigned long LastTick;// Offset=0x21e8 Size=0x4
    unsigned long UnusedPad;// Offset=0x21ec Size=0x4
    ULONG64 PrcbPad50[2];// Offset=0x21f0 Size=0x10
    struct _KTIMER_TABLE TimerTable;// Offset=0x2200 Size=0x2200
    struct _KGATE DpcGate;// Offset=0x4400 Size=0x18
    void *PrcbPad52;// Offset=0x4418 Size=0x8
    struct _KDPC_W7 CallDpc;// Offset=0x4420 Size=0x40
    long ClockKeepAlive;// Offset=0x4460 Size=0x4
    unsigned char ClockCheckSlot;// Offset=0x4464 Size=0x1
    unsigned char ClockPollCycle;// Offset=0x4465 Size=0x1
    unsigned short NmiActive;// Offset=0x4466 Size=0x2
    long DpcWatchdogPeriod;// Offset=0x4468 Size=0x4
    long DpcWatchdogCount;// Offset=0x446c Size=0x4
    ULONG64 TickOffset;// Offset=0x4470 Size=0x8
    long volatile KeSpinLockOrdering;// Offset=0x4478 Size=0x4
    unsigned long PrcbPad70;// Offset=0x447c Size=0x4
    struct _LIST_ENTRY WaitListHead;// Offset=0x4480 Size=0x10
    ULONG64 WaitLock;// Offset=0x4490 Size=0x8
    unsigned long ReadySummary;// Offset=0x4498 Size=0x4
    unsigned long QueueIndex;// Offset=0x449c Size=0x4
    struct _KDPC_W7 TimerExpirationDpc;// Offset=0x44a0 Size=0x40
    ULONG64 PrcbPad72[4];// Offset=0x44e0 Size=0x20
    struct _LIST_ENTRY DispatcherReadyListHead[32];// Offset=0x4500 Size=0x200
    unsigned long InterruptCount;// Offset=0x4700 Size=0x4
    unsigned long KernelTime;// Offset=0x4704 Size=0x4
    unsigned long UserTime;// Offset=0x4708 Size=0x4
    unsigned long DpcTime;// Offset=0x470c Size=0x4
    unsigned long InterruptTime;// Offset=0x4710 Size=0x4
    unsigned long AdjustDpcThreshold;// Offset=0x4714 Size=0x4
    unsigned char DebuggerSavedIRQL;// Offset=0x4718 Size=0x1
    unsigned char PrcbPad80[7];// Offset=0x4719 Size=0x7
    unsigned long DpcTimeCount;// Offset=0x4720 Size=0x4
    unsigned long DpcTimeLimit;// Offset=0x4724 Size=0x4
    unsigned long PeriodicCount;// Offset=0x4728 Size=0x4
    unsigned long PeriodicBias;// Offset=0x472c Size=0x4
    unsigned long AvailableTime;// Offset=0x4730 Size=0x4
    unsigned long KeExceptionDispatchCount;// Offset=0x4734 Size=0x4
    struct _KNODE *ParentNode;// Offset=0x4738 Size=0x8
    ULONG64 StartCycles;// Offset=0x4740 Size=0x8
    ULONG64 PrcbPad82[3];// Offset=0x4748 Size=0x18
    long volatile MmSpinLockOrdering;// Offset=0x4760 Size=0x4
    unsigned long PageColor;// Offset=0x4764 Size=0x4
    unsigned long NodeColor;// Offset=0x4768 Size=0x4
    unsigned long NodeShiftedColor;// Offset=0x476c Size=0x4
    unsigned long SecondaryColorMask;// Offset=0x4770 Size=0x4
    unsigned long PrcbPad83;// Offset=0x4774 Size=0x4
    ULONG64 CycleTime;// Offset=0x4778 Size=0x8
    unsigned long CcFastMdlReadNoWait;// Offset=0x4780 Size=0x4
    unsigned long CcFastMdlReadWait;// Offset=0x4784 Size=0x4
    unsigned long CcFastMdlReadNotPossible;// Offset=0x4788 Size=0x4
    unsigned long CcMapDataNoWait;// Offset=0x478c Size=0x4
    unsigned long CcMapDataWait;// Offset=0x4790 Size=0x4
    unsigned long CcPinMappedDataCount;// Offset=0x4794 Size=0x4
    unsigned long CcPinReadNoWait;// Offset=0x4798 Size=0x4
    unsigned long CcPinReadWait;// Offset=0x479c Size=0x4
    unsigned long CcMdlReadNoWait;// Offset=0x47a0 Size=0x4
    unsigned long CcMdlReadWait;// Offset=0x47a4 Size=0x4
    unsigned long CcLazyWriteHotSpots;// Offset=0x47a8 Size=0x4
    unsigned long CcLazyWriteIos;// Offset=0x47ac Size=0x4
    unsigned long CcLazyWritePages;// Offset=0x47b0 Size=0x4
    unsigned long CcDataFlushes;// Offset=0x47b4 Size=0x4
    unsigned long CcDataPages;// Offset=0x47b8 Size=0x4
    unsigned long CcLostDelayedWrites;// Offset=0x47bc Size=0x4
    unsigned long CcFastReadResourceMiss;// Offset=0x47c0 Size=0x4
    unsigned long CcCopyReadWaitMiss;// Offset=0x47c4 Size=0x4
    unsigned long CcFastMdlReadResourceMiss;// Offset=0x47c8 Size=0x4
    unsigned long CcMapDataNoWaitMiss;// Offset=0x47cc Size=0x4
    unsigned long CcMapDataWaitMiss;// Offset=0x47d0 Size=0x4
    unsigned long CcPinReadNoWaitMiss;// Offset=0x47d4 Size=0x4
    unsigned long CcPinReadWaitMiss;// Offset=0x47d8 Size=0x4
    unsigned long CcMdlReadNoWaitMiss;// Offset=0x47dc Size=0x4
    unsigned long CcMdlReadWaitMiss;// Offset=0x47e0 Size=0x4
    unsigned long CcReadAheadIos;// Offset=0x47e4 Size=0x4
    long volatile MmCacheTransitionCount;// Offset=0x47e8 Size=0x4
    long volatile MmCacheReadCount;// Offset=0x47ec Size=0x4
    long volatile MmCacheIoCount;// Offset=0x47f0 Size=0x4
    unsigned long PrcbPad91[1];// Offset=0x47f4 Size=0x4
    ULONG64 RuntimeAccumulation;// Offset=0x47f8 Size=0x8
    struct _PROCESSOR_POWER_STATE PowerState;// Offset=0x4800 Size=0x100
    unsigned char PrcbPad92[16];// Offset=0x4900 Size=0x10
    unsigned long KeAlignmentFixupCount;// Offset=0x4910 Size=0x4
    unsigned char __align4[4];// Offset=0x4914 Size=0x4
    struct _KDPC_W7 DpcWatchdogDpc;// Offset=0x4918 Size=0x40
    struct _KTIMER DpcWatchdogTimer;// Offset=0x4958 Size=0x40
    struct _CACHE_DESCRIPTOR Cache[5];// Offset=0x4998 Size=0x3c
    unsigned long CacheCount;// Offset=0x49d4 Size=0x4
    unsigned long volatile CachedCommit;// Offset=0x49d8 Size=0x4
    unsigned long volatile CachedResidentAvailable;// Offset=0x49dc Size=0x4
    void *HyperPte;// Offset=0x49e0 Size=0x8
    void *WheaInfo;// Offset=0x49e8 Size=0x8
    void *EtwSupport;// Offset=0x49f0 Size=0x8
    unsigned char __align5[8];// Offset=0x49f8 Size=0x8
    union _SLIST_HEADER InterruptObjectPool;// Offset=0x4a00 Size=0x10
    union _SLIST_HEADER HypercallPageList;// Offset=0x4a10 Size=0x10
    void *HypercallPageVirtual;// Offset=0x4a20 Size=0x8
    void *VirtualApicAssist;// Offset=0x4a28 Size=0x8
    ULONG64 *StatisticsPage;// Offset=0x4a30 Size=0x8
    void *RateControl;// Offset=0x4a38 Size=0x8
    ULONG64 CacheProcessorMask[5];// Offset=0x4a40 Size=0x28
    struct _KAFFINITY_EX PackageProcessorSet;// Offset=0x4a68 Size=0x28
    ULONG64 CoreProcessorSet;// Offset=0x4a90 Size=0x8
    void *PebsIndexAddress;// Offset=0x4a98 Size=0x8
    ULONG64 PrcbPad93[12];// Offset=0x4aa0 Size=0x60
    unsigned long SpinLockAcquireCount;// Offset=0x4b00 Size=0x4
    unsigned long SpinLockContentionCount;// Offset=0x4b04 Size=0x4
    unsigned long SpinLockSpinCount;// Offset=0x4b08 Size=0x4
    unsigned long IpiSendRequestBroadcastCount;// Offset=0x4b0c Size=0x4
    unsigned long IpiSendRequestRoutineCount;// Offset=0x4b10 Size=0x4
    unsigned long IpiSendSoftwareInterruptCount;// Offset=0x4b14 Size=0x4
    unsigned long ExInitializeResourceCount;// Offset=0x4b18 Size=0x4
    unsigned long ExReInitializeResourceCount;// Offset=0x4b1c Size=0x4
    unsigned long ExDeleteResourceCount;// Offset=0x4b20 Size=0x4
    unsigned long ExecutiveResourceAcquiresCount;// Offset=0x4b24 Size=0x4
    unsigned long ExecutiveResourceContentionsCount;// Offset=0x4b28 Size=0x4
    unsigned long ExecutiveResourceReleaseExclusiveCount;// Offset=0x4b2c Size=0x4
    unsigned long ExecutiveResourceReleaseSharedCount;// Offset=0x4b30 Size=0x4
    unsigned long ExecutiveResourceConvertsCount;// Offset=0x4b34 Size=0x4
    unsigned long ExAcqResExclusiveAttempts;// Offset=0x4b38 Size=0x4
    unsigned long ExAcqResExclusiveAcquiresExclusive;// Offset=0x4b3c Size=0x4
    unsigned long ExAcqResExclusiveAcquiresExclusiveRecursive;// Offset=0x4b40 Size=0x4
    unsigned long ExAcqResExclusiveWaits;// Offset=0x4b44 Size=0x4
    unsigned long ExAcqResExclusiveNotAcquires;// Offset=0x4b48 Size=0x4
    unsigned long ExAcqResSharedAttempts;// Offset=0x4b4c Size=0x4
    unsigned long ExAcqResSharedAcquiresExclusive;// Offset=0x4b50 Size=0x4
    unsigned long ExAcqResSharedAcquiresShared;// Offset=0x4b54 Size=0x4
    unsigned long ExAcqResSharedAcquiresSharedRecursive;// Offset=0x4b58 Size=0x4
    unsigned long ExAcqResSharedWaits;// Offset=0x4b5c Size=0x4
    unsigned long ExAcqResSharedNotAcquires;// Offset=0x4b60 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAttempts;// Offset=0x4b64 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresExclusive;// Offset=0x4b68 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresShared;// Offset=0x4b6c Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveAcquiresSharedRecursive;// Offset=0x4b70 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveWaits;// Offset=0x4b74 Size=0x4
    unsigned long ExAcqResSharedStarveExclusiveNotAcquires;// Offset=0x4b78 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAttempts;// Offset=0x4b7c Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresExclusive;// Offset=0x4b80 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresShared;// Offset=0x4b84 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveAcquiresSharedRecursive;// Offset=0x4b88 Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveWaits;// Offset=0x4b8c Size=0x4
    unsigned long ExAcqResSharedWaitForExclusiveNotAcquires;// Offset=0x4b90 Size=0x4
    unsigned long ExSetResOwnerPointerExclusive;// Offset=0x4b94 Size=0x4
    unsigned long ExSetResOwnerPointerSharedNew;// Offset=0x4b98 Size=0x4
    unsigned long ExSetResOwnerPointerSharedOld;// Offset=0x4b9c Size=0x4
    unsigned long ExTryToAcqExclusiveAttempts;// Offset=0x4ba0 Size=0x4
    unsigned long ExTryToAcqExclusiveAcquires;// Offset=0x4ba4 Size=0x4
    unsigned long ExBoostExclusiveOwner;// Offset=0x4ba8 Size=0x4
    unsigned long ExBoostSharedOwners;// Offset=0x4bac Size=0x4
    unsigned long ExEtwSynchTrackingNotificationsCount;// Offset=0x4bb0 Size=0x4
    unsigned long ExEtwSynchTrackingNotificationsAccountedCount;// Offset=0x4bb4 Size=0x4
    unsigned char VendorString[13];// Offset=0x4bb8 Size=0xd
    unsigned char PrcbPad10[3];// Offset=0x4bc5 Size=0x3
    unsigned long FeatureBits;// Offset=0x4bc8 Size=0x4
    unsigned char __align6[4];// Offset=0x4bcc Size=0x4
    union _LARGE_INTEGER UpdateSignature;// Offset=0x4bd0 Size=0x8
    struct _CONTEXT *Context;// Offset=0x4bd8 Size=0x8
    unsigned long ContextFlags;// Offset=0x4be0 Size=0x4
    unsigned char __align7[4];// Offset=0x4be4 Size=0x4
    struct _XSAVE_AREA *ExtendedState;// Offset=0x4be8 Size=0x8
    unsigned char __align8[16];// Offset=0x4bf0 Size=0x10
    struct _REQUEST_MAILBOX *Mailbox;// Offset=0x4c00 Size=0x8
    unsigned char __align9[120];// Offset=0x4c08 Size=0x78
    struct _REQUEST_MAILBOX RequestMailbox[1];// Offset=0x4c80 Size=0x40
    unsigned char __endalign[64];// Offset=0x4cc0 Size=0x40
} KPRCB, *PKPRCB;

typedef struct _OBJECT_DIRECTORY// Size=0x150
{
    struct _OBJECT_DIRECTORY_ENTRY *HashBuckets[37];// Offset=0x0 Size=0x128
    struct _EX_PUSH_LOCK Lock;// Offset=0x128 Size=0x8
    struct _DEVICE_MAP *DeviceMap;// Offset=0x130 Size=0x8
    unsigned long SessionId;// Offset=0x138 Size=0x4
    unsigned char __align0[4];// Offset=0x13c Size=0x4
    void *NamespaceEntry;// Offset=0x140 Size=0x8
    unsigned long Flags;// Offset=0x148 Size=0x4
    unsigned char __endalign[4];// Offset=0x14c Size=0x4
} *POBJECT_DIRECTORY;

#endif //                  Windows 7 x64 internal kernel definitions



typedef struct _OBJECT_HEADER// Size=0x20/0x38
{
    LONG_PTR PointerCount;
    union
    {
        long HandleCount;
        void *NextToFree;
    };

#if (NTDDI_VERSION < NTDDI_WIN7)
    struct _OBJECT_TYPE *Type;
    unsigned char NameInfoOffset;
    unsigned char HandleInfoOffset;
    unsigned char QuotaInfoOffset;
#else
    struct _EX_PUSH_LOCK Lock;
    unsigned char TypeIndex;
    unsigned char TraceFlags;
    unsigned char InfoMask;
#endif

    unsigned char Flags;
    union
    {
        struct _OBJECT_CREATE_INFORMATION *ObjectCreateInfo;
        void *QuotaBlockCharged;
    };
    void *SecurityDescriptor;
    struct _QUAD Body;
} OBJECT_HEADER, *POBJECT_HEADER;

typedef struct _OBJECT_HEADER_NAME_INFO {
    POBJECT_DIRECTORY Directory;
    UNICODE_STRING Name;
    ULONG QueryReferences;
} OBJECT_HEADER_NAME_INFO, *POBJECT_HEADER_NAME_INFO;

struct  _PORT_MESSAGE {
    union
    {
        struct
        {
            CSHORT DataLength;
            CSHORT TotalLength;
        } s1;
        ULONG Length;
    } u1;
    union
    {
        struct
        {
            CSHORT Type;
            CSHORT DataInfoOffset;
        } s2;
        ULONG ZeroInit;
    } u2;
    union
    {
        CLIENT_ID ClientId;
        double DoNotUseThisField;
    };
    ULONG MessageId;
    union
    {
        SIZE_T ClientViewSize;
        ULONG CallbackId;
    };
};

typedef struct _ALPC_MESSAGE_ATTRIBUTES {
    ULONG AllocatedAttributes;
    ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, *PALPC_MESSAGE_ATTRIBUTES;

typedef struct _ALPC_PORT_ATTRIBUTES {
    ULONG                       Flags;
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    SIZE_T                      MaxMessageLength;
    SIZE_T                      MemoryBandwidth;
    SIZE_T                      MaxPoolUsage;
    SIZE_T                      MaxSectionSize;
    SIZE_T                      MaxViewSize;
    SIZE_T                      MaxTotalSectionSize;
    ULONG                       DupObjectTypes;
#ifdef _WIN64
    ULONG Reserved;
#endif
} ALPC_PORT_ATTRIBUTES, *PALPC_PORT_ATTRIBUTES;

#ifdef __cplusplus
}
#endif

#endif // _COMMON_H