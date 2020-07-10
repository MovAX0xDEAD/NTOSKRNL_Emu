#define XOR ^
#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))
#define TRY
#define LEAVE   goto __tryLabel;
#define FINALLY __tryLabel:

#define RTL_CONSTANT_STRING_k8(s) { sizeof( s ) - sizeof( (s)[0] ), sizeof( s ), s }


typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,
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
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;


NTSTATUS
NtQuerySystemInformation (		// exportable by kernel
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
    );



/////////////////////////////////////////////////////////
// early + external definition

BOOLEAN
KeSetCoalescableTimer_k8 (
    KTIMER         *Timer,
    LARGE_INTEGER   DueTime,
    ULONG           Period,
    ULONG           TolerableDelay,
    KDPC           *Dpc);


NTSTATUS
EtwRegister_k8 (
    LPCGUID             ProviderId,
    PETWENABLECALLBACK  EnableCallback,
    PVOID               CallbackContext,
    PREGHANDLE          RegHandle );

void PrintNumaCpuConfiguration(void);


PVOID
GetRoutineAddress (
    PUNICODE_STRING SystemRoutineName,
    const PCHAR Modulename);

PVOID
GetModuleBaseAddress (
    const PCHAR Modulename);

extern PVOID CREATE_PROCESS_NOTIFY_ROUTINE_asm; // ntoskrn8_helpers.asm

// early + external definition
/////////////////////////////////////////////////////////


#if (NTDDI_VERSION <= NTDDI_WS03SP4)            // Windows XP/2003
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


#if (NTDDI_VERSION <= NTDDI_WINXPSP1)
 #define ExAcquireRundownProtectionEx ExAcquireRundownProtectionEx_k8
 #define ExReleaseRundownProtectionEx ExReleaseRundownProtectionEx_k8
#endif


#if (NTDDI_VERSION <= NTDDI_VISTASP4)
 #define KeSetCoalescableTimer KeSetCoalescableTimer_k8
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows XP x32 internal kernel definitions                        //

#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WINXPSP4) && defined(_X86_)
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

    typedef struct _KIDTENTRY // Size=0x8
    {
        unsigned short Offset;         // Offset=0x0 Size=0x2
        unsigned short Selector;       // Offset=0x2 Size=0x2
        unsigned short Access;         // Offset=0x4 Size=0x2
        unsigned short ExtendedOffset; // Offset=0x6 Size=0x2
    } KIDTENTRY;

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

    struct _EX_PUSH_LOCK // Size=0x4
    {
        union // Size=0xc
        {
            struct // Size=0xc
            {
                unsigned long Waiting : 1;   // Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
                unsigned long Exclusive : 1; // Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
                unsigned long Shared : 30;   // Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1e
            };
            unsigned long Value; // Offset=0x0 Size=0x4
            void *Ptr;           // Offset=0x0 Size=0x4
        };
    };

    struct _EX_FAST_REF // Size=0x4
    {
        union // Size=0x4
        {
            void *Object;             // Offset=0x0 Size=0x4
            unsigned long RefCnt : 3; // Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
            unsigned long Value;      // Offset=0x0 Size=0x4
        };
    };

    struct _KPROCESS // Size=0x6c
    {
        struct _DISPATCHER_HEADER Header;        // Offset=0x0 Size=0x10
        struct _LIST_ENTRY ProfileListHead;      // Offset=0x10 Size=0x8
        unsigned long DirectoryTableBase[2];     // Offset=0x18 Size=0x8
        struct _KGDTENTRY LdtDescriptor;         // Offset=0x20 Size=0x8
        struct _KIDTENTRY Int21Descriptor;       // Offset=0x28 Size=0x8
        unsigned short IopmOffset;               // Offset=0x30 Size=0x2
        unsigned char Iopl;                      // Offset=0x32 Size=0x1
        unsigned char Unused;                    // Offset=0x33 Size=0x1
        unsigned long ActiveProcessors;          // Offset=0x34 Size=0x4
        unsigned long KernelTime;                // Offset=0x38 Size=0x4
        unsigned long UserTime;                  // Offset=0x3c Size=0x4
        struct _LIST_ENTRY ReadyListHead;        // Offset=0x40 Size=0x8
        struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x48 Size=0x4
        void *VdmTrapcHandler;                   // Offset=0x4c Size=0x4
        struct _LIST_ENTRY ThreadListHead;       // Offset=0x50 Size=0x8
        unsigned long ProcessLock;               // Offset=0x58 Size=0x4
        unsigned long Affinity;                  // Offset=0x5c Size=0x4
        unsigned short StackCount;               // Offset=0x60 Size=0x2
        char BasePriority;                       // Offset=0x62 Size=0x1
        char ThreadQuantum;                      // Offset=0x63 Size=0x1
        unsigned char AutoAlignment;             // Offset=0x64 Size=0x1
        unsigned char State;                     // Offset=0x65 Size=0x1
        unsigned char ThreadSeed;                // Offset=0x66 Size=0x1
        unsigned char DisableBoost;              // Offset=0x67 Size=0x1
        unsigned char PowerState;                // Offset=0x68 Size=0x1
        unsigned char DisableQuantum;            // Offset=0x69 Size=0x1
        unsigned char IdealNode;                 // Offset=0x6a Size=0x1
        union                                    // Size=0x1
        {
            struct _KEXECUTE_OPTIONS Flags; // Offset=0x6b Size=0x1
            unsigned char ExecuteOptions;   // Offset=0x6b Size=0x1
        };
    };

    typedef struct _EPROCESS // Size=0x260
    {
        struct _KPROCESS Pcb;                   // Offset=0x0 Size=0x6c
        struct _EX_PUSH_LOCK ProcessLock;       // Offset=0x6c Size=0x4
        union _LARGE_INTEGER CreateTime;        // Offset=0x70 Size=0x8
        union _LARGE_INTEGER ExitTime;          // Offset=0x78 Size=0x8
        struct _EX_RUNDOWN_REF RundownProtect;  // Offset=0x80 Size=0x4
        void *UniqueProcessId;                  // Offset=0x84 Size=0x4
        struct _LIST_ENTRY ActiveProcessLinks;  // Offset=0x88 Size=0x8
        unsigned long QuotaUsage[3];            // Offset=0x90 Size=0xc
        unsigned long QuotaPeak[3];             // Offset=0x9c Size=0xc
        unsigned long CommitCharge;             // Offset=0xa8 Size=0x4
        unsigned long PeakVirtualSize;          // Offset=0xac Size=0x4
        unsigned long VirtualSize;              // Offset=0xb0 Size=0x4
        struct _LIST_ENTRY SessionProcessLinks; // Offset=0xb4 Size=0x8
        void *DebugPort;                        // Offset=0xbc Size=0x4
        void *ExceptionPort;                    // Offset=0xc0 Size=0x4
        struct _HANDLE_TABLE *ObjectTable;      // Offset=0xc4 Size=0x4
        struct _EX_FAST_REF Token;              // Offset=0xc8 Size=0x4
        struct _FAST_MUTEX WorkingSetLock;      // Offset=0xcc Size=0x20
        unsigned long WorkingSetPage;           // Offset=0xec Size=0x4
        struct _FAST_MUTEX AddressCreationLock; // Offset=0xf0 Size=0x20
        unsigned long HyperSpaceLock;           // Offset=0x110 Size=0x4
        struct _ETHREAD *ForkInProgress;        // Offset=0x114 Size=0x4
        unsigned long HardwareTrigger;          // Offset=0x118 Size=0x4
        void *VadRoot;                          // Offset=0x11c Size=0x4
        void *VadHint;                          // Offset=0x120 Size=0x4
        void *CloneRoot;                        // Offset=0x124 Size=0x4
        unsigned long NumberOfPrivatePages;     // Offset=0x128 Size=0x4
        unsigned long NumberOfLockedPages;      // Offset=0x12c Size=0x4
        void *Win32Process;                     // Offset=0x130 Size=0x4
        struct _EJOB *Job;                      // Offset=0x134 Size=0x4
        void *SectionObject;                    // Offset=0x138 Size=0x4
        // ...
    } EPROCESS;

    typedef struct _MMADDRESS_NODE
    { // Size=0x14
        union {
            LONG_PTR Balance : 2;
            struct _MMADDRESS_NODE *Parent;
        } u1;
        struct _MMADDRESS_NODE *LeftChild;
        struct _MMADDRESS_NODE *RightChild;
        ULONG_PTR StartingVpn;
        ULONG_PTR EndingVpn;
    } MMADDRESS_NODE, *PMMADDRESS_NODE;

    struct _CONTROL_AREA // Size=0x30
    {
        struct _SEGMENT *Segment;                // Offset=0x0 Size=0x4
        struct _LIST_ENTRY DereferenceList;      // Offset=0x4 Size=0x8
        unsigned long NumberOfSectionReferences; // Offset=0xc Size=0x4
        unsigned long NumberOfPfnReferences;     // Offset=0x10 Size=0x4
        unsigned long NumberOfMappedViews;       // Offset=0x14 Size=0x4
        unsigned short NumberOfSubsections;      // Offset=0x18 Size=0x2
        unsigned short FlushInProgressCount;     // Offset=0x1a Size=0x2
        unsigned long NumberOfUserReferences;    // Offset=0x1c Size=0x4
        //union __unnamed u;// Offset=0x20 Size=0x4
        unsigned long u;                           // Offset=0x20 Size=0x4
        struct _FILE_OBJECT *FilePointer;          // Offset=0x24 Size=0x4
        struct _EVENT_COUNTER *WaitingForDeletion; // Offset=0x28 Size=0x4
        unsigned short ModifiedWriteCount;         // Offset=0x2c Size=0x2
        unsigned short NumberOfSystemCacheViews;   // Offset=0x2e Size=0x2
    };

    typedef struct _SEGMENT // Size=0x40
    {
        struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
        // ...
    } * PSEGMENT;

    typedef struct _SECTION
    {
        MMADDRESS_NODE Address;
        PSEGMENT Segment;
        // ...
    } SECTION, *PSECTION;

    struct _KAPC_STATE // Size=0x18
    {
        struct _LIST_ENTRY ApcListHead[2]; // Offset=0x0 Size=0x10
        struct _KPROCESS *Process;         // Offset=0x10 Size=0x4
        unsigned char KernelApcInProgress; // Offset=0x14 Size=0x1
        unsigned char KernelApcPending;    // Offset=0x15 Size=0x1
        unsigned char UserApcPending;      // Offset=0x16 Size=0x1
    };

    struct _KTHREAD // Size=0x1c0
    {
        struct _DISPATCHER_HEADER Header;   // Offset=0x0 Size=0x10
        struct _LIST_ENTRY MutantListHead;  // Offset=0x10 Size=0x8
        void *InitialStack;                 // Offset=0x18 Size=0x4
        void *StackLimit;                   // Offset=0x1c Size=0x4
        void *Teb;                          // Offset=0x20 Size=0x4
        void *TlsArray;                     // Offset=0x24 Size=0x4
        void *KernelStack;                  // Offset=0x28 Size=0x4
        unsigned char DebugActive;          // Offset=0x2c Size=0x1
        unsigned char State;                // Offset=0x2d Size=0x1
        unsigned char Alerted[2];           // Offset=0x2e Size=0x2
        unsigned char Iopl;                 // Offset=0x30 Size=0x1
        unsigned char NpxState;             // Offset=0x31 Size=0x1
        char Saturation;                    // Offset=0x32 Size=0x1
        char Priority;                      // Offset=0x33 Size=0x1
        struct _KAPC_STATE ApcState;        // Offset=0x34 Size=0x18
        unsigned long ContextSwitches;      // Offset=0x4c Size=0x4
        unsigned char IdleSwapBlock;        // Offset=0x50 Size=0x1
        unsigned char Spare0[3];            // Offset=0x51 Size=0x3
        long WaitStatus;                    // Offset=0x54 Size=0x4
        unsigned char WaitIrql;             // Offset=0x58 Size=0x1
        char WaitMode;                      // Offset=0x59 Size=0x1
        unsigned char WaitNext;             // Offset=0x5a Size=0x1
        unsigned char WaitReason;           // Offset=0x5b Size=0x1
        struct _KWAIT_BLOCK *WaitBlockList; // Offset=0x5c Size=0x4
        union                               // Size=0x8
        {
            struct _LIST_ENTRY WaitListEntry;        // Offset=0x60 Size=0x8
            struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x60 Size=0x4
        };
        unsigned long WaitTime;                 // Offset=0x68 Size=0x4
        char BasePriority;                      // Offset=0x6c Size=0x1
        unsigned char DecrementCount;           // Offset=0x6d Size=0x1
        char PriorityDecrement;                 // Offset=0x6e Size=0x1
        char Quantum;                           // Offset=0x6f Size=0x1
        struct _KWAIT_BLOCK WaitBlock[4];       // Offset=0x70 Size=0x60
        void *LegoData;                         // Offset=0xd0 Size=0x4
        unsigned long KernelApcDisable;         // Offset=0xd4 Size=0x4
        unsigned long UserAffinity;             // Offset=0xd8 Size=0x4
        unsigned char SystemAffinityActive;     // Offset=0xdc Size=0x1
        unsigned char PowerState;               // Offset=0xdd Size=0x1
        unsigned char NpxIrql;                  // Offset=0xde Size=0x1
        unsigned char InitialNode;              // Offset=0xdf Size=0x1
        void *ServiceTable;                     // Offset=0xe0 Size=0x4
        struct _KQUEUE *Queue;                  // Offset=0xe4 Size=0x4
        unsigned long ApcQueueLock;             // Offset=0xe8 Size=0x4
        struct _KTIMER Timer;                   // Offset=0xf0 Size=0x28
        struct _LIST_ENTRY QueueListEntry;      // Offset=0x118 Size=0x8
        unsigned long SoftAffinity;             // Offset=0x120 Size=0x4
        unsigned long Affinity;                 // Offset=0x124 Size=0x4
        unsigned char Preempted;                // Offset=0x128 Size=0x1
        unsigned char ProcessReadyQueue;        // Offset=0x129 Size=0x1
        unsigned char KernelStackResident;      // Offset=0x12a Size=0x1
        unsigned char NextProcessor;            // Offset=0x12b Size=0x1
        void *CallbackStack;                    // Offset=0x12c Size=0x4
        void *Win32Thread;                      // Offset=0x130 Size=0x4
        struct _KTRAP_FRAME *TrapFrame;         // Offset=0x134 Size=0x4
        struct _KAPC_STATE *ApcStatePointer[2]; // Offset=0x138 Size=0x8
        char PreviousMode;                      // Offset=0x140 Size=0x1
        unsigned char EnableStackSwap;          // Offset=0x141 Size=0x1
        unsigned char LargeStack;               // Offset=0x142 Size=0x1
        unsigned char ResourceIndex;            // Offset=0x143 Size=0x1
        unsigned long KernelTime;               // Offset=0x144 Size=0x4
        unsigned long UserTime;                 // Offset=0x148 Size=0x4
        struct _KAPC_STATE SavedApcState;       // Offset=0x14c Size=0x18
        unsigned char Alertable;                // Offset=0x164 Size=0x1
        unsigned char ApcStateIndex;            // Offset=0x165 Size=0x1
        unsigned char ApcQueueable;             // Offset=0x166 Size=0x1
        unsigned char AutoAlignment;            // Offset=0x167 Size=0x1
        void *StackBase;                        // Offset=0x168 Size=0x4
        struct _KAPC SuspendApc;                // Offset=0x16c Size=0x30
        struct _KSEMAPHORE SuspendSemaphore;    // Offset=0x19c Size=0x14
        struct _LIST_ENTRY ThreadListEntry;     // Offset=0x1b0 Size=0x8
        char FreezeCount;                       // Offset=0x1b8 Size=0x1
        char SuspendCount;                      // Offset=0x1b9 Size=0x1
        unsigned char IdealProcessor;           // Offset=0x1ba Size=0x1
        unsigned char DisableBoost;             // Offset=0x1bb Size=0x1
    };

    typedef struct _ETHREAD // Size=0x258
    {
        struct _KTHREAD Tcb; // Offset=0x0 Size=0x1c0
        union                // Size=0x8
        {
            union _LARGE_INTEGER CreateTime; // Offset=0x1c0 Size=0x8
            struct                           // Size=0x8
            {
                unsigned int NestedFaultCount : 2; // Offset=0x1c0 Size=0x4 BitOffset=0x0 BitSize=0x2
                unsigned int ApcNeeded : 1;        // Offset=0x1c0 Size=0x4 BitOffset=0x2 BitSize=0x1
            };
        };
        union // Size=0x8
        {
            union _LARGE_INTEGER ExitTime;     // Offset=0x1c8 Size=0x8
            struct _LIST_ENTRY LpcReplyChain;  // Offset=0x1c8 Size=0x8
            struct _LIST_ENTRY KeyedWaitChain; // Offset=0x1c8 Size=0x8
        };
        union // Size=0x4
        {
            long ExitStatus; // Offset=0x1d0 Size=0x4
            void *OfsChain;  // Offset=0x1d0 Size=0x4
        };
        struct _LIST_ENTRY PostBlockList; // Offset=0x1d4 Size=0x8
        union                             // Size=0x4
        {
            struct _TERMINATION_PORT *TerminationPort; // Offset=0x1dc Size=0x4
            struct _ETHREAD *ReaperLink;               // Offset=0x1dc Size=0x4
            void *KeyedWaitValue;                      // Offset=0x1dc Size=0x4
        };
        unsigned long ActiveTimerListLock;      // Offset=0x1e0 Size=0x4
        struct _LIST_ENTRY ActiveTimerListHead; // Offset=0x1e4 Size=0x8
        struct _CLIENT_ID Cid;                  // Offset=0x1ec Size=0x8
        // ...
    } ETHREAD;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows 2003 x32 internal kernel definitions                      //
#if (NTDDI_VERSION >= NTDDI_WS03) && (NTDDI_VERSION <= NTDDI_WS03SP4) && defined(_X86_)
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

    typedef struct _KIDTENTRY // Size=0x8
    {
        unsigned short Offset;         // Offset=0x0 Size=0x2
        unsigned short Selector;       // Offset=0x2 Size=0x2
        unsigned short Access;         // Offset=0x4 Size=0x2
        unsigned short ExtendedOffset; // Offset=0x6 Size=0x2
    } KIDTENTRY;

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

    struct _EX_PUSH_LOCK // Size=0x4
    {
        union // Size=0xc
        {
            struct // Size=0xc
            {
                unsigned long Waiting : 1;   // Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x1
                unsigned long Exclusive : 1; // Offset=0x0 Size=0x4 BitOffset=0x1 BitSize=0x1
                unsigned long Shared : 30;   // Offset=0x0 Size=0x4 BitOffset=0x2 BitSize=0x1e
            };
            unsigned long Value; // Offset=0x0 Size=0x4
            void *Ptr;           // Offset=0x0 Size=0x4
        };
    };

    struct _EX_FAST_REF // Size=0x4
    {
        union // Size=0x4
        {
            void *Object;             // Offset=0x0 Size=0x4
            unsigned long RefCnt : 3; // Offset=0x0 Size=0x4 BitOffset=0x0 BitSize=0x3
            unsigned long Value;      // Offset=0x0 Size=0x4
        };
    };

    typedef struct _MMADDRESS_NODE
    { // Size=0x14
        union {
            LONG_PTR Balance : 2;
            struct _MMADDRESS_NODE *Parent;
        } u1;
        struct _MMADDRESS_NODE *LeftChild;
        struct _MMADDRESS_NODE *RightChild;
        ULONG_PTR StartingVpn;
        ULONG_PTR EndingVpn;
    } MMADDRESS_NODE, *PMMADDRESS_NODE;

    struct _MM_AVL_TABLE // Size=0x20
    {
        struct _MMADDRESS_NODE BalancedRoot;           // Offset=0x0 Size=0x14
        unsigned long DepthOfTree : 5;                 // Offset=0x14 Size=0x4 BitOffset=0x0 BitSize=0x5
        unsigned long Unused : 3;                      // Offset=0x14 Size=0x4 BitOffset=0x5 BitSize=0x3
        unsigned long NumberGenericTableElements : 24; // Offset=0x14 Size=0x4 BitOffset=0x8 BitSize=0x18
        void *NodeHint;                                // Offset=0x18 Size=0x4
        void *NodeFreeHint;                            // Offset=0x1c Size=0x4
    };

    struct _KPROCESS // Size=0x78
    {
        struct _DISPATCHER_HEADER Header;        // Offset=0x0 Size=0x10
        struct _LIST_ENTRY ProfileListHead;      // Offset=0x10 Size=0x8
        unsigned long DirectoryTableBase[2];     // Offset=0x18 Size=0x8
        struct _KGDTENTRY LdtDescriptor;         // Offset=0x20 Size=0x8
        struct _KIDTENTRY Int21Descriptor;       // Offset=0x28 Size=0x8
        unsigned short IopmOffset;               // Offset=0x30 Size=0x2
        unsigned char Iopl;                      // Offset=0x32 Size=0x1
        unsigned char Unused;                    // Offset=0x33 Size=0x1
        unsigned long ActiveProcessors;          // Offset=0x34 Size=0x4
        unsigned long KernelTime;                // Offset=0x38 Size=0x4
        unsigned long UserTime;                  // Offset=0x3c Size=0x4
        struct _LIST_ENTRY ReadyListHead;        // Offset=0x40 Size=0x8
        struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x48 Size=0x4
        void *VdmTrapcHandler;                   // Offset=0x4c Size=0x4
        struct _LIST_ENTRY ThreadListHead;       // Offset=0x50 Size=0x8
        unsigned long ProcessLock;               // Offset=0x58 Size=0x4
        unsigned long Affinity;                  // Offset=0x5c Size=0x4
        union                                    // Size=0x10
        {
            struct // Size=0x10
            {
                long AutoAlignment : 1;  // Offset=0x60 Size=0x4 BitOffset=0x0 BitSize=0x1
                long DisableBoost : 1;   // Offset=0x60 Size=0x4 BitOffset=0x1 BitSize=0x1
                long DisableQuantum : 1; // Offset=0x60 Size=0x4 BitOffset=0x2 BitSize=0x1
                long ReservedFlags : 29; // Offset=0x60 Size=0x4 BitOffset=0x3 BitSize=0x1d
            };
            struct // Size=0x11
            {
                long ProcessFlags;        // Offset=0x60 Size=0x4
                char BasePriority;        // Offset=0x64 Size=0x1
                char QuantumReset;        // Offset=0x65 Size=0x1
                unsigned char State;      // Offset=0x66 Size=0x1
                unsigned char ThreadSeed; // Offset=0x67 Size=0x1
                unsigned char PowerState; // Offset=0x68 Size=0x1
                unsigned char IdealNode;  // Offset=0x69 Size=0x1
                unsigned char Visited;    // Offset=0x6a Size=0x1
                union                     // Size=0x1
                {
                    struct _KEXECUTE_OPTIONS Flags; // Offset=0x6b Size=0x1
                    unsigned char ExecuteOptions;   // Offset=0x6b Size=0x1
                };
                unsigned long StackCount; // Offset=0x6c Size=0x4
            };
        };
        struct _LIST_ENTRY ProcessListEntry; // Offset=0x70 Size=0x8
    };

    typedef struct _EPROCESS // Size=0x278
    {
        struct _KPROCESS Pcb;                       // Offset=0x0 Size=0x78
        struct _EX_PUSH_LOCK ProcessLock;           // Offset=0x78 Size=0x4
        unsigned char __align0[4];                  // Offset=0x7c Size=0x4
        union _LARGE_INTEGER CreateTime;            // Offset=0x80 Size=0x8
        union _LARGE_INTEGER ExitTime;              // Offset=0x88 Size=0x8
        struct _EX_RUNDOWN_REF RundownProtect;      // Offset=0x90 Size=0x4
        void *UniqueProcessId;                      // Offset=0x94 Size=0x4
        struct _LIST_ENTRY ActiveProcessLinks;      // Offset=0x98 Size=0x8
        unsigned long QuotaUsage[3];                // Offset=0xa0 Size=0xc
        unsigned long QuotaPeak[3];                 // Offset=0xac Size=0xc
        unsigned long CommitCharge;                 // Offset=0xb8 Size=0x4
        unsigned long PeakVirtualSize;              // Offset=0xbc Size=0x4
        unsigned long VirtualSize;                  // Offset=0xc0 Size=0x4
        struct _LIST_ENTRY SessionProcessLinks;     // Offset=0xc4 Size=0x8
        void *DebugPort;                            // Offset=0xcc Size=0x4
        void *ExceptionPort;                        // Offset=0xd0 Size=0x4
        struct _HANDLE_TABLE *ObjectTable;          // Offset=0xd4 Size=0x4
        struct _EX_FAST_REF Token;                  // Offset=0xd8 Size=0x4
        unsigned long WorkingSetPage;               // Offset=0xdc Size=0x4
        struct _KGUARDED_MUTEX AddressCreationLock; // Offset=0xe0 Size=0x20
        unsigned long HyperSpaceLock;               // Offset=0x100 Size=0x4
        struct _ETHREAD *ForkInProgress;            // Offset=0x104 Size=0x4
        unsigned long HardwareTrigger;              // Offset=0x108 Size=0x4
        struct _MM_AVL_TABLE *PhysicalVadRoot;      // Offset=0x10c Size=0x4
        void *CloneRoot;                            // Offset=0x110 Size=0x4
        unsigned long NumberOfPrivatePages;         // Offset=0x114 Size=0x4
        unsigned long NumberOfLockedPages;          // Offset=0x118 Size=0x4
        void *Win32Process;                         // Offset=0x11c Size=0x4
        struct _EJOB *Job;                          // Offset=0x120 Size=0x4
        void *SectionObject;                        // Offset=0x124 Size=0x4
        // ...
    } EPROCESS;

    struct _CONTROL_AREA // Size=0x38
    {
        struct _SEGMENT *Segment;                // Offset=0x0 Size=0x4
        struct _LIST_ENTRY DereferenceList;      // Offset=0x4 Size=0x8
        unsigned long NumberOfSectionReferences; // Offset=0xc Size=0x4
        unsigned long NumberOfPfnReferences;     // Offset=0x10 Size=0x4
        unsigned long NumberOfMappedViews;       // Offset=0x14 Size=0x4
        unsigned long NumberOfSystemCacheViews;  // Offset=0x18 Size=0x4
        unsigned long NumberOfUserReferences;    // Offset=0x1c Size=0x4
        //union __unnamed u;// Offset=0x20 Size=0x4
        unsigned long u;                           // Offset=0x20 Size=0x4
        struct _FILE_OBJECT *FilePointer;          // Offset=0x24 Size=0x4
        struct _EVENT_COUNTER *WaitingForDeletion; // Offset=0x28 Size=0x4
        unsigned short ModifiedWriteCount;         // Offset=0x2c Size=0x2
        unsigned short FlushInProgressCount;       // Offset=0x2e Size=0x2
        unsigned long WritableUserReferences;      // Offset=0x30 Size=0x4
        unsigned long QuadwordPad;                 // Offset=0x34 Size=0x4
    };

    typedef struct _SEGMENT // Size=0x48
    {
        struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
        // ...
    } * PSEGMENT;

    typedef struct _SECTION
    {
        MMADDRESS_NODE Address;
        PSEGMENT Segment;
        // ...
    } SECTION, *PSECTION;

    struct _KAPC_STATE // Size=0x18
    {
        struct _LIST_ENTRY ApcListHead[2]; // Offset=0x0 Size=0x10
        struct _KPROCESS *Process;         // Offset=0x10 Size=0x4
        unsigned char KernelApcInProgress; // Offset=0x14 Size=0x1
        unsigned char KernelApcPending;    // Offset=0x15 Size=0x1
        unsigned char UserApcPending;      // Offset=0x16 Size=0x1
    };

    struct _KTHREAD // Size=0x1b8
    {
        struct _DISPATCHER_HEADER Header;  // Offset=0x0 Size=0x10
        struct _LIST_ENTRY MutantListHead; // Offset=0x10 Size=0x8
        void *InitialStack;                // Offset=0x18 Size=0x4
        void *StackLimit;                  // Offset=0x1c Size=0x4
        void *KernelStack;                 // Offset=0x20 Size=0x4
        unsigned long ThreadLock;          // Offset=0x24 Size=0x4
        union                              // Size=0x18
        {
            struct _KAPC_STATE ApcState; // Offset=0x28 Size=0x18
            struct                       // Size=0x18
            {
                unsigned char ApcStateFill[23];  // Offset=0x28 Size=0x17
                unsigned char ApcQueueable;      // Offset=0x3f Size=0x1
                unsigned char NextProcessor;     // Offset=0x40 Size=0x1
                unsigned char DeferredProcessor; // Offset=0x41 Size=0x1
                unsigned char AdjustReason;      // Offset=0x42 Size=0x1
                char AdjustIncrement;            // Offset=0x43 Size=0x1
            };
        };
        unsigned long ApcQueueLock;    // Offset=0x44 Size=0x4
        unsigned long ContextSwitches; // Offset=0x48 Size=0x4
        unsigned char State;           // Offset=0x4c Size=0x1
        unsigned char NpxState;        // Offset=0x4d Size=0x1
        unsigned char WaitIrql;        // Offset=0x4e Size=0x1
        char WaitMode;                 // Offset=0x4f Size=0x1
        long WaitStatus;               // Offset=0x50 Size=0x4
        union                          // Size=0x4
        {
            struct _KWAIT_BLOCK *WaitBlockList; // Offset=0x54 Size=0x4
            struct _KGATE *GateObject;          // Offset=0x54 Size=0x4
        };
        unsigned char Alertable;       // Offset=0x58 Size=0x1
        unsigned char WaitNext;        // Offset=0x59 Size=0x1
        unsigned char WaitReason;      // Offset=0x5a Size=0x1
        char Priority;                 // Offset=0x5b Size=0x1
        unsigned char EnableStackSwap; // Offset=0x5c Size=0x1
        unsigned char SwapBusy;        // Offset=0x5d Size=0x1
        unsigned char Alerted[2];      // Offset=0x5e Size=0x2
        union                          // Size=0x8
        {
            struct _LIST_ENTRY WaitListEntry;        // Offset=0x60 Size=0x8
            struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x60 Size=0x4
        };
        struct _KQUEUE *Queue;  // Offset=0x68 Size=0x4
        unsigned long WaitTime; // Offset=0x6c Size=0x4
        union                   // Size=0x4
        {
            struct // Size=0x4
            {
                short KernelApcDisable;  // Offset=0x70 Size=0x2
                short SpecialApcDisable; // Offset=0x72 Size=0x2
            };
            unsigned long CombinedApcDisable; // Offset=0x70 Size=0x4
        };
        void *Teb; // Offset=0x74 Size=0x4
        union      // Size=0x28
        {
            struct _KTIMER Timer; // Offset=0x78 Size=0x28
            struct
            {
                unsigned char TimerFill[40]; // Offset=0x78 Size=0x28

                union // Size=0x10
                {
                    struct // Size=0x10
                    {
                        unsigned long AutoAlignment : 1;  // Offset=0xa0 Size=0x4 BitOffset=0x0 BitSize=0x1
                        unsigned long DisableBoost : 1;   // Offset=0xa0 Size=0x4 BitOffset=0x1 BitSize=0x1
                        unsigned long GuiThread : 1;      // Offset=0xa0 Size=0x4 BitOffset=0x2 BitSize=0x1
                        unsigned long ReservedFlags : 29; // Offset=0xa0 Size=0x4 BitOffset=0x3 BitSize=0x1d
                    };
                    long ThreadFlags; // Offset=0xa0 Size=0x4
                };
            };
        };

        union // Size=0x48
        {
            struct _KWAIT_BLOCK WaitBlock[4]; // Offset=0xa8 Size=0x60
            struct                            // Size=0x18
            {
                unsigned char WaitBlockFill0[23];   // Offset=0xa8 Size=0x17
                unsigned char SystemAffinityActive; // Offset=0xbf Size=0x1
            };
            struct // Size=0x30
            {
                unsigned char WaitBlockFill1[47]; // Offset=0xa8 Size=0x2f
                char PreviousMode;                // Offset=0xd7 Size=0x1
            };
            struct // Size=0x48
            {
                unsigned char WaitBlockFill2[71]; // Offset=0xa8 Size=0x47
                unsigned char ResourceIndex;      // Offset=0xef Size=0x1
            };
            struct // Size=0x48
            {
                unsigned char WaitBlockFill3[95]; // Offset=0xa8 Size=0x5f
                unsigned char LargeStack;         // Offset=0x107 Size=0x1
            };
        };
        struct _LIST_ENTRY QueueListEntry;      // Offset=0x108 Size=0x8
        struct _KTRAP_FRAME *TrapFrame;         // Offset=0x110 Size=0x4
        void *CallbackStack;                    // Offset=0x114 Size=0x4
        void *ServiceTable;                     // Offset=0x118 Size=0x4
        unsigned char ApcStateIndex;            // Offset=0x11c Size=0x1
        unsigned char IdealProcessor;           // Offset=0x11d Size=0x1
        unsigned char Preempted;                // Offset=0x11e Size=0x1
        unsigned char ProcessReadyQueue;        // Offset=0x11f Size=0x1
        unsigned char KernelStackResident;      // Offset=0x120 Size=0x1
        char BasePriority;                      // Offset=0x121 Size=0x1
        char PriorityDecrement;                 // Offset=0x122 Size=0x1
        char Saturation;                        // Offset=0x123 Size=0x1
        unsigned long UserAffinity;             // Offset=0x124 Size=0x4
        struct _KPROCESS *Process;              // Offset=0x128 Size=0x4
        unsigned long Affinity;                 // Offset=0x12c Size=0x4
        struct _KAPC_STATE *ApcStatePointer[2]; // Offset=0x130 Size=0x8
        union                                   // Size=0x18
        {
            struct _KAPC_STATE SavedApcState; // Offset=0x138 Size=0x18
            struct                            // Size=0x18
            {
                unsigned char SavedApcStateFill[23]; // Offset=0x138 Size=0x17
                char FreezeCount;                    // Offset=0x14f Size=0x1
                char SuspendCount;                   // Offset=0x150 Size=0x1
                unsigned char UserIdealProcessor;    // Offset=0x151 Size=0x1
                unsigned char CalloutActive;         // Offset=0x152 Size=0x1
                unsigned char Iopl;                  // Offset=0x153 Size=0x1
            };
        };
        void *Win32Thread; // Offset=0x154 Size=0x4
        void *StackBase;   // Offset=0x158 Size=0x4
        union              // Size=0x30
        {
            struct _KAPC SuspendApc; // Offset=0x15c Size=0x30
            struct                   // Size=0x2
            {
                unsigned char SuspendApcFill0[1]; // Offset=0x15c Size=0x1
                char Quantum;                     // Offset=0x15d Size=0x1
            };
            struct // Size=0x4
            {
                unsigned char SuspendApcFill1[3]; // Offset=0x15c Size=0x3
                unsigned char QuantumReset;       // Offset=0x15f Size=0x1
            };
            struct // Size=0x8
            {
                unsigned char SuspendApcFill2[4]; // Offset=0x15c Size=0x4
                unsigned long KernelTime;         // Offset=0x160 Size=0x4
            };
            struct // Size=0x28
            {
                unsigned char SuspendApcFill3[36]; // Offset=0x15c Size=0x24
                void *TlsArray;                    // Offset=0x180 Size=0x4
            };
            struct // Size=0x2c
            {
                unsigned char SuspendApcFill4[40]; // Offset=0x15c Size=0x28
                void *LegoData;                    // Offset=0x184 Size=0x4
            };
            struct // Size=0x30
            {
                unsigned char SuspendApcFill5[47]; // Offset=0x15c Size=0x2f
                unsigned char PowerState;          // Offset=0x18b Size=0x1
                unsigned long UserTime;            // Offset=0x18c Size=0x4
            };
        };
        union // Size=0x14
        {
            struct _KSEMAPHORE SuspendSemaphore; // Offset=0x190 Size=0x14
            struct
            {
                unsigned char SuspendSemaphorefill[20]; // Offset=0x190 Size=0x14
                unsigned long SListFaultCount;          // Offset=0x1a4 Size=0x4
            };
        };

        struct _LIST_ENTRY ThreadListEntry; // Offset=0x1a8 Size=0x8
        void *SListFaultAddress;            // Offset=0x1b0 Size=0x4
    };

    typedef struct _ETHREAD // Size=0x250
    {
        struct _KTHREAD Tcb;             // Offset=0x0 Size=0x1b8
        union _LARGE_INTEGER CreateTime; // Offset=0x1b8 Size=0x8
        union                            // Size=0x8
        {
            union _LARGE_INTEGER ExitTime;     // Offset=0x1c0 Size=0x8
            struct _LIST_ENTRY LpcReplyChain;  // Offset=0x1c0 Size=0x8
            struct _LIST_ENTRY KeyedWaitChain; // Offset=0x1c0 Size=0x8
        };
        union // Size=0x4
        {
            long ExitStatus; // Offset=0x1c8 Size=0x4
            void *OfsChain;  // Offset=0x1c8 Size=0x4
        };
        struct _LIST_ENTRY PostBlockList; // Offset=0x1cc Size=0x8
        union                             // Size=0x4
        {
            struct _TERMINATION_PORT *TerminationPort; // Offset=0x1d4 Size=0x4
            struct _ETHREAD *ReaperLink;               // Offset=0x1d4 Size=0x4
            void *KeyedWaitValue;                      // Offset=0x1d4 Size=0x4
        };
        unsigned long ActiveTimerListLock;      // Offset=0x1d8 Size=0x4
        struct _LIST_ENTRY ActiveTimerListHead; // Offset=0x1dc Size=0x8
        struct _CLIENT_ID Cid;                  // Offset=0x1e4 Size=0x8
        // ...
    } ETHREAD;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////
//                         Windows XP/2003 x64 internal kernel definitions                   //
#if (NTDDI_VERSION >= NTDDI_WINXP) && (NTDDI_VERSION <= NTDDI_WS03SP4) && defined(_AMD64_)

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

    struct _EX_PUSH_LOCK // Size=0x8
    {
        union // Size=0x28
        {
            struct // Size=0x28
            {
                ULONG_PTR Locked : 1;         // Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x1
                ULONG_PTR Waiting : 1;        // Offset=0x0 Size=0x8 BitOffset=0x1 BitSize=0x1
                ULONG_PTR Waking : 1;         // Offset=0x0 Size=0x8 BitOffset=0x2 BitSize=0x1
                ULONG_PTR MultipleShared : 1; // Offset=0x0 Size=0x8 BitOffset=0x3 BitSize=0x1
                ULONG_PTR Shared : 60;        // Offset=0x0 Size=0x8 BitOffset=0x4 BitSize=0x3c
            };
            ULONG_PTR Value; // Offset=0x0 Size=0x8
            void *Ptr;       // Offset=0x0 Size=0x8
        };
    };

    struct _EX_FAST_REF // Size=0x8
    {
        union // Size=0x8
        {
            void *Object;         // Offset=0x0 Size=0x8
            ULONG_PTR RefCnt : 4; // Offset=0x0 Size=0x8 BitOffset=0x0 BitSize=0x4
            ULONG_PTR Value;      // Offset=0x0 Size=0x8
        };
    };

    typedef struct _MMADDRESS_NODE // Size=0x28
    {
        //union <unnamed-tag> u1;// Offset=0x0 Size=0x8
        ULONG_PTR u1;                       // Offset=0x0 Size=0x8
        struct _MMADDRESS_NODE *LeftChild;  // Offset=0x8 Size=0x8
        struct _MMADDRESS_NODE *RightChild; // Offset=0x10 Size=0x8
        ULONG_PTR StartingVpn;              // Offset=0x18 Size=0x8
        ULONG_PTR EndingVpn;                // Offset=0x20 Size=0x8
    } MMADDRESS_NODE;

    struct _MM_AVL_TABLE // Size=0x40
    {
        struct _MMADDRESS_NODE BalancedRoot;       // Offset=0x0 Size=0x28
        ULONG_PTR DepthOfTree : 5;                 // Offset=0x28 Size=0x8 BitOffset=0x0 BitSize=0x5
        ULONG_PTR Unused : 3;                      // Offset=0x28 Size=0x8 BitOffset=0x5 BitSize=0x3
        ULONG_PTR NumberGenericTableElements : 56; // Offset=0x28 Size=0x8 BitOffset=0x8 BitSize=0x38
        void *NodeHint;                            // Offset=0x30 Size=0x8
        void *NodeFreeHint;                        // Offset=0x38 Size=0x8
    };

    struct _KPROCESS // Size=0xb8
    {
        struct _DISPATCHER_HEADER Header;        // Offset=0x0 Size=0x18
        struct _LIST_ENTRY ProfileListHead;      // Offset=0x18 Size=0x10
        ULONG_PTR DirectoryTableBase[2];         // Offset=0x28 Size=0x10
        unsigned short IopmOffset;               // Offset=0x38 Size=0x2
        unsigned char __align0[6];               // Offset=0x3a Size=0x6
        ULONG_PTR ActiveProcessors;              // Offset=0x40 Size=0x8
        unsigned long KernelTime;                // Offset=0x48 Size=0x4
        unsigned long UserTime;                  // Offset=0x4c Size=0x4
        struct _LIST_ENTRY ReadyListHead;        // Offset=0x50 Size=0x10
        struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x60 Size=0x8
        void *Reserved1;                         // Offset=0x68 Size=0x8
        struct _LIST_ENTRY ThreadListHead;       // Offset=0x70 Size=0x10
        ULONG_PTR ProcessLock;                   // Offset=0x80 Size=0x8
        ULONG_PTR Affinity;                      // Offset=0x88 Size=0x8
        union                                    // Size=0x10
        {
            struct // Size=0x10
            {
                long AutoAlignment : 1;  // Offset=0x90 Size=0x4 BitOffset=0x0 BitSize=0x1
                long DisableBoost : 1;   // Offset=0x90 Size=0x4 BitOffset=0x1 BitSize=0x1
                long DisableQuantum : 1; // Offset=0x90 Size=0x4 BitOffset=0x2 BitSize=0x1
                long ReservedFlags : 29; // Offset=0x90 Size=0x4 BitOffset=0x3 BitSize=0x1d
            };
            struct // Size=0x11
            {
                long ProcessFlags;        // Offset=0x90 Size=0x4
                char BasePriority;        // Offset=0x94 Size=0x1
                char QuantumReset;        // Offset=0x95 Size=0x1
                unsigned char State;      // Offset=0x96 Size=0x1
                unsigned char ThreadSeed; // Offset=0x97 Size=0x1
                unsigned char PowerState; // Offset=0x98 Size=0x1
                unsigned char IdealNode;  // Offset=0x99 Size=0x1
                unsigned char Visited;    // Offset=0x9a Size=0x1
                union                     // Size=0x1
                {
                    struct _KEXECUTE_OPTIONS Flags; // Offset=0x9b Size=0x1
                    unsigned char ExecuteOptions;   // Offset=0x9b Size=0x1
                };
                unsigned char __align1[4]; // Offset=0x9c Size=0x4
            };
        };
        ULONG_PTR StackCount;                // Offset=0xa0 Size=0x8
        struct _LIST_ENTRY ProcessListEntry; // Offset=0xa8 Size=0x10
    };

    typedef struct _EPROCESS // Size=0x3e0
    {
        struct _KPROCESS Pcb;                       // Offset=0x0 Size=0xb8
        struct _EX_PUSH_LOCK ProcessLock;           // Offset=0xb8 Size=0x8
        union _LARGE_INTEGER CreateTime;            // Offset=0xc0 Size=0x8
        union _LARGE_INTEGER ExitTime;              // Offset=0xc8 Size=0x8
        struct _EX_RUNDOWN_REF RundownProtect;      // Offset=0xd0 Size=0x8
        void *UniqueProcessId;                      // Offset=0xd8 Size=0x8
        struct _LIST_ENTRY ActiveProcessLinks;      // Offset=0xe0 Size=0x10
        SIZE_T QuotaUsage[3];                       // Offset=0xf0 Size=0x18
        SIZE_T QuotaPeak[3];                        // Offset=0x108 Size=0x18
        SIZE_T CommitCharge;                        // Offset=0x120 Size=0x8
        SIZE_T PeakVirtualSize;                     // Offset=0x128 Size=0x8
        SIZE_T VirtualSize;                         // Offset=0x130 Size=0x8
        struct _LIST_ENTRY SessionProcessLinks;     // Offset=0x138 Size=0x10
        void *DebugPort;                            // Offset=0x148 Size=0x8
        void *ExceptionPort;                        // Offset=0x150 Size=0x8
        struct _HANDLE_TABLE *ObjectTable;          // Offset=0x158 Size=0x8
        struct _EX_FAST_REF Token;                  // Offset=0x160 Size=0x8
        PFN_NUMBER WorkingSetPage;                  // Offset=0x168 Size=0x8
        struct _KGUARDED_MUTEX AddressCreationLock; // Offset=0x170 Size=0x38
        KSPIN_LOCK HyperSpaceLock;                  // Offset=0x1a8 Size=0x8
        struct _ETHREAD *ForkInProgress;            // Offset=0x1b0 Size=0x8
        ULONG_PTR HardwareTrigger;                  // Offset=0x1b8 Size=0x8
        struct _MM_AVL_TABLE *PhysicalVadRoot;      // Offset=0x1c0 Size=0x8
        void *CloneRoot;                            // Offset=0x1c8 Size=0x8
        SIZE_T NumberOfPrivatePages;                // Offset=0x1d0 Size=0x8
        SIZE_T NumberOfLockedPages;                 // Offset=0x1d8 Size=0x8
        void *Win32Process;                         // Offset=0x1e0 Size=0x8
        struct _EJOB *Job;                          // Offset=0x1e8 Size=0x8
        void *SectionObject;                        // Offset=0x1f0 Size=0x8
        // ...
    } EPROCESS;

    struct _CONTROL_AREA // Size=0x48
    {
        struct _SEGMENT *Segment;               // Offset=0x0 Size=0x8
        struct _LIST_ENTRY DereferenceList;     // Offset=0x8 Size=0x10
        unsigned long NumberOfSectionReferences;   // Offset=0x18 Size=0x4
        unsigned long NumberOfPfnReferences;    // Offset=0x1c Size=0x4
        unsigned long NumberOfMappedViews;      // Offset=0x20 Size=0x4
        unsigned long NumberOfSystemCacheViews; // Offset=0x24 Size=0x4
        unsigned long NumberOfUserReferences;   // Offset=0x28 Size=0x4
        //union <unnamed-tag> u;// Offset=0x2c Size=0x4
        unsigned long LongFlags;                   // Offset=0x0 Size=0x4
        struct _FILE_OBJECT *FilePointer;          // Offset=0x30 Size=0x8
        struct _EVENT_COUNTER *WaitingForDeletion; // Offset=0x38 Size=0x8
        unsigned short ModifiedWriteCount;         // Offset=0x40 Size=0x2
        unsigned short FlushInProgressCount;       // Offset=0x42 Size=0x2
        unsigned long WritableUserReferences;      // Offset=0x44 Size=0x4
    };

    typedef struct _SEGMENT // Size=0x48
    {
        struct _CONTROL_AREA *ControlArea; // Offset=0x0 Size=0x4
        // ...
    } SEGMENT, *PSEGMENT;

    typedef struct _SECTION
    {
        MMADDRESS_NODE Address;
        PSEGMENT Segment;
        // ...
    } SECTION, *PSECTION;

    struct _KAPC_STATE // Size=0x30
    {
        struct _LIST_ENTRY ApcListHead[2]; // Offset=0x0 Size=0x20
        struct _KPROCESS *Process;         // Offset=0x20 Size=0x8
        unsigned char KernelApcInProgress; // Offset=0x28 Size=0x1
        unsigned char KernelApcPending;    // Offset=0x29 Size=0x1
        unsigned char UserApcPending;      // Offset=0x2a Size=0x1
    };

    struct _KTHREAD // Size=0x308
    {
        struct _DISPATCHER_HEADER Header;  // Offset=0x0 Size=0x18
        struct _LIST_ENTRY MutantListHead; // Offset=0x18 Size=0x10
        void *InitialStack;                // Offset=0x28 Size=0x8
        void *StackLimit;                  // Offset=0x30 Size=0x8
        void *KernelStack;                 // Offset=0x38 Size=0x8
        KSPIN_LOCK ThreadLock;             // Offset=0x40 Size=0x8
        union                              // Size=0x30
        {
            struct _KAPC_STATE ApcState; // Offset=0x48 Size=0x30
            struct                       // Size=0x30
            {
                unsigned char ApcStateFill[43];  // Offset=0x48 Size=0x2b
                unsigned char ApcQueueable;      // Offset=0x73 Size=0x1
                unsigned char NextProcessor;     // Offset=0x74 Size=0x1
                unsigned char DeferredProcessor; // Offset=0x75 Size=0x1
                unsigned char AdjustReason;      // Offset=0x76 Size=0x1
                char AdjustIncrement;            // Offset=0x77 Size=0x1
            };
        };
        KSPIN_LOCK ApcQueueLock; // Offset=0x78 Size=0x8
        LONG_PTR WaitStatus;     // Offset=0x80 Size=0x8
        union                    // Size=0x8
        {
            struct _KWAIT_BLOCK *WaitBlockList; // Offset=0x88 Size=0x8
            struct _KGATE *GateObject;          // Offset=0x88 Size=0x8
        };
        unsigned char Alertable;       // Offset=0x90 Size=0x1
        unsigned char WaitNext;        // Offset=0x91 Size=0x1
        unsigned char WaitReason;      // Offset=0x92 Size=0x1
        char Priority;                 // Offset=0x93 Size=0x1
        unsigned char EnableStackSwap; // Offset=0x94 Size=0x1
        unsigned char SwapBusy;        // Offset=0x95 Size=0x1
        unsigned char Alerted[2];      // Offset=0x96 Size=0x2
        union                          // Size=0x10
        {
            struct _LIST_ENTRY WaitListEntry;        // Offset=0x98 Size=0x10
            struct _SINGLE_LIST_ENTRY SwapListEntry; // Offset=0x98 Size=0x8
        };
        struct _KQUEUE *Queue; // Offset=0xa8 Size=0x8
        void *Teb;             // Offset=0xb0 Size=0x8
        union                  // Size=0x40
        {
            struct _KTIMER Timer; // Offset=0xb8 Size=0x40
            struct                // Size=0x40
            {
                unsigned char TimerFill[60]; // Offset=0xb8 Size=0x3c
                union {
                    struct
                    {
                        unsigned long AutoAlignment : 1;  // Offset=0xf4 Size=0x4 BitOffset=0x0 BitSize=0x1
                        unsigned long DisableBoost : 1;   // Offset=0xf4 Size=0x4 BitOffset=0x1 BitSize=0x1
                        unsigned long GuiThread : 1;      // Offset=0xf4 Size=0x4 BitOffset=0x2 BitSize=0x1
                        unsigned long ReservedFlags : 29; // Offset=0xf4 Size=0x4 BitOffset=0x3 BitSize=0x1d
                    };
                    long ThreadFlags;                   // Offset=0xf4 Size=0x4
                };
            };
        };

        union // Size=0xc0
        {
            struct _KWAIT_BLOCK WaitBlock[4]; // Offset=0xf8 Size=0xc0
            struct                            // Size=0x2c
            {
                unsigned char WaitBlockFill0[43];   // Offset=0xf8 Size=0x2b
                unsigned char SystemAffinityActive; // Offset=0x123 Size=0x1
            };
            struct // Size=0x5c
            {
                unsigned char WaitBlockFill1[91]; // Offset=0xf8 Size=0x5b
                char PreviousMode;                // Offset=0x153 Size=0x1
            };
            struct // Size=0x8c
            {
                unsigned char WaitBlockFill2[139]; // Offset=0xf8 Size=0x8b
                unsigned char ResourceIndex;       // Offset=0x183 Size=0x1
            };
            struct // Size=0xbc
            {
                unsigned char WaitBlockFill3[187]; // Offset=0xf8 Size=0xbb
                unsigned char LargeStack;          // Offset=0x1b3 Size=0x1
            };
            struct // Size=0x30
            {
                unsigned char WaitBlockFill4[44]; // Offset=0xf8 Size=0x2c
                unsigned long ContextSwitches;    // Offset=0x124 Size=0x4
            };
            struct // Size=0x60
            {
                unsigned char WaitBlockFill5[92]; // Offset=0xf8 Size=0x5c
                unsigned char State;              // Offset=0x154 Size=0x1
                unsigned char NpxState;           // Offset=0x155 Size=0x1
                unsigned char WaitIrql;           // Offset=0x156 Size=0x1
                char WaitMode;                    // Offset=0x157 Size=0x1
            };
            struct // Size=0x90
            {
                unsigned char WaitBlockFill6[140]; // Offset=0xf8 Size=0x8c
                unsigned long WaitTime;            // Offset=0x184 Size=0x4
            };
            struct // Size=0xc0
            {
                unsigned char WaitBlockFill7[188]; // Offset=0xf8 Size=0xbc
                union {
                    struct
                    {
                        SHORT KernelApcDisable;
                        SHORT SpecialApcDisable;
                    };

                    ULONG CombinedApcDisable;
                };
            };
        };
        struct _LIST_ENTRY QueueListEntry;      // Offset=0x1b8 Size=0x10
        struct _KTRAP_FRAME *TrapFrame;         // Offset=0x1c8 Size=0x8
        void *CallbackStack;                    // Offset=0x1d0 Size=0x8
        unsigned char ApcStateIndex;            // Offset=0x1d8 Size=0x1
        unsigned char IdealProcessor;           // Offset=0x1d9 Size=0x1
        unsigned char Preempted;                // Offset=0x1da Size=0x1
        unsigned char ProcessReadyQueue;        // Offset=0x1db Size=0x1
        unsigned char KernelStackResident;      // Offset=0x1dc Size=0x1
        char BasePriority;                      // Offset=0x1dd Size=0x1
        char PriorityDecrement;                 // Offset=0x1de Size=0x1
        char Saturation;                        // Offset=0x1df Size=0x1
        ULONG_PTR UserAffinity;                 // Offset=0x1e0 Size=0x8
        struct _KPROCESS *Process;              // Offset=0x1e8 Size=0x8
        ULONG_PTR Affinity;                     // Offset=0x1f0 Size=0x8
        struct _KAPC_STATE *ApcStatePointer[2]; // Offset=0x1f8 Size=0x10
        union                                   // Size=0x30
        {
            struct _KAPC_STATE SavedApcState; // Offset=0x208 Size=0x30
            struct                            // Size=0x30
            {
                unsigned char SavedApcStateFill[43]; // Offset=0x208 Size=0x2b
                char FreezeCount;                    // Offset=0x233 Size=0x1
                char SuspendCount;                   // Offset=0x234 Size=0x1
                unsigned char UserIdealProcessor;    // Offset=0x235 Size=0x1
                unsigned char CalloutActive;         // Offset=0x236 Size=0x1
                unsigned char CodePatchInProgress;   // Offset=0x237 Size=0x1
            };
        };
        void *Win32Thread; // Offset=0x238 Size=0x8
        void *StackBase;   // Offset=0x240 Size=0x8
        union              // Size=0x58
        {
            struct _KAPC SuspendApc; // Offset=0x248 Size=0x58
            struct                   // Size=0x2
            {
                unsigned char SuspendApcFill0[1]; // Offset=0x248 Size=0x1
                char Quantum;                     // Offset=0x249 Size=0x1
            };
            struct // Size=0x4
            {
                unsigned char SuspendApcFill1[3]; // Offset=0x248 Size=0x3
                unsigned char QuantumReset;       // Offset=0x24b Size=0x1
            };
            struct // Size=0x8
            {
                unsigned char SuspendApcFill2[4]; // Offset=0x248 Size=0x4
                unsigned long KernelTime;         // Offset=0x24c Size=0x4
            };
            struct // Size=0x48
            {
                unsigned char SuspendApcFill3[64]; // Offset=0x248 Size=0x40
                void *TlsArray;                    // Offset=0x288 Size=0x8
            };
            struct // Size=0x50
            {
                unsigned char SuspendApcFill4[72]; // Offset=0x248 Size=0x48
                void *LegoData;                    // Offset=0x290 Size=0x8
            };
            struct // Size=0x58
            {
                unsigned char SuspendApcFill5[83]; // Offset=0x248 Size=0x53
                unsigned char PowerState;          // Offset=0x29b Size=0x1
                unsigned long UserTime;            // Offset=0x29c Size=0x4
            };
        };
        union // Size=0x20
        {
            struct _KSEMAPHORE SuspendSemaphore; // Offset=0x2a0 Size=0x20
            struct                               // Size=0x20
            {
                unsigned char SuspendSemaphorefill[28]; // Offset=0x2a0 Size=0x1c
                unsigned long SListFaultCount;          // Offset=0x2bc Size=0x4
            };
        };
        struct _LIST_ENTRY ThreadListEntry; // Offset=0x2c0 Size=0x10
        void *SListFaultAddress;            // Offset=0x2d0 Size=0x8
        LONG64 ReadOperationCount;          // Offset=0x2d8 Size=0x8
        LONG64 WriteOperationCount;         // Offset=0x2e0 Size=0x8
        LONG64 OtherOperationCount;         // Offset=0x2e8 Size=0x8
        LONG64 ReadTransferCount;           // Offset=0x2f0 Size=0x8
        LONG64 WriteTransferCount;          // Offset=0x2f8 Size=0x8
        LONG64 OtherTransferCount;          // Offset=0x300 Size=0x8
    };

    struct _ETHREAD // Size=0x410
    {
        struct _KTHREAD Tcb;             // Offset=0x0 Size=0x308
        union _LARGE_INTEGER CreateTime; // Offset=0x308 Size=0x8
        union                            // Size=0x10
        {
            union _LARGE_INTEGER ExitTime;     // Offset=0x310 Size=0x8
            struct _LIST_ENTRY LpcReplyChain;  // Offset=0x310 Size=0x10
            struct _LIST_ENTRY KeyedWaitChain; // Offset=0x310 Size=0x10
        };
        union // Size=0x4
        {
            long ExitStatus; // Offset=0x320 Size=0x4
            void *OfsChain;  // Offset=0x320 Size=0x8
        };
        struct _LIST_ENTRY PostBlockList; // Offset=0x328 Size=0x10
        union                             // Size=0x8
        {
            struct _TERMINATION_PORT *TerminationPort; // Offset=0x338 Size=0x8
            struct _ETHREAD *ReaperLink;               // Offset=0x338 Size=0x8
            void *KeyedWaitValue;                      // Offset=0x338 Size=0x8
        };
        ULONG_PTR ActiveTimerListLock;          // Offset=0x340 Size=0x8
        struct _LIST_ENTRY ActiveTimerListHead; // Offset=0x348 Size=0x10
        struct _CLIENT_ID Cid;                  // Offset=0x358 Size=0x10
        // ...
    };

#endif
