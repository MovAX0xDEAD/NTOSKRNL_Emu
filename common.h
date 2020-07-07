#define XOR ^
#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))
#define TRY
#define LEAVE   goto __tryLabel;
#define FINALLY __tryLabel:


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

