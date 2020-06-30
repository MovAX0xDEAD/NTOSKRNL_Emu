#define XOR ^
#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))
#define TRY
#define LEAVE   goto __tryLabel;
#define FINALLY __tryLabel:

typedef enum _STORPORT_FUNCTION_CODE_W10 {
    ///////////////////////////////////////////
    //      Windows 2003
    ExtFunctionAllocatePool,
    ExtFunctionFreePool,
    ExtFunctionAllocateMdl,
    ExtFunctionFreeMdl,
    ExtFunctionBuildMdlForNonPagedPool,
    ExtFunctionGetSystemAddress,
    ExtFunctionGetOriginalMdl,
    ExtFunctionCompleteServiceIrp,
    ExtFunctionGetDeviceObjects,
    ExtFunctionBuildScatterGatherList,
    ExtFunctionPutScatterGatherList,

    ///////////////////////////////////////////
    //      Vista
    ExtFunctionAcquireMSISpinLock,
    ExtFunctionReleaseMSISpinLock,
    ExtFunctionGetMessageInterruptInformation,
    ExtFunctionInitializePerformanceOptimizations,
    ExtFunctionGetStartIoPerformanceParameters,

    ///////////////////////////////////////////
    //      Windows 7
    ExtFunctionLogSystemEvent,
    ExtFunctionGetCurrentProcessorNumber,
    ExtFunctionGetActiveGroupCount,
    ExtFunctionGetGroupAffinity,
    ExtFunctionGetActiveNodeCount,
    ExtFunctionGetNodeAffinity,
    ExtFunctionGetHighestNodeNumber,
    ExtFunctionGetLogicalProcessorRelationship,
    ExtFunctionAllocateContiguousMemorySpecifyCacheNode,
    ExtFunctionFreeContiguousMemorySpecifyCache,    // 25
    
    ///////////////////////////////////////////
    //      Windows 8
    ExtFunctionSetPowerSettingNotificationGuids,    // +
    ExtFunctionInvokeAcpiMethod,                    // +
    ExtFunctionGetRequestInfo,
    ExtFunctionInitializeWorker,
    ExtFunctionQueueWorkItem,
    ExtFunctionFreeWorker,
    ExtFunctionInitializeTimer,                     // +
    ExtFunctionRequestTimer,                        // +
    ExtFunctionFreeTimer,                           // +
    ExtFunctionInitializeSListHead,
    ExtFunctionInterlockedFlushSList,
    ExtFunctionInterlockedPopEntrySList,
    ExtFunctionInterlockedPushEntrySList,
    ExtFunctionQueryDepthSList,
    ExtFunctionGetActivityId,                       // +
    ExtFunctionGetSystemPortNumber,
    ExtFunctionGetDataInBufferMdl,
    ExtFunctionGetDataInBufferSystemAddress,
    ExtFunctionGetDataInBufferScatterGatherList,
    ExtFunctionMarkDumpMemory,                      // +
    ExtFunctionSetUnitAttributes,                   // +
    ExtFunctionQueryPerformanceCounter,             // +
    ExtFunctionInitializePoFxPower,                 // +
    ExtFunctionPoFxActivateComponent,               // +    
    ExtFunctionPoFxIdleComponent,                   // +
    ExtFunctionPoFxSetComponentLatency,             // +
    ExtFunctionPoFxSetComponentResidency,           // +
    ExtFunctionPoFxPowerControl,                    // +
    ExtFunctionFlushDataBufferMdl,
    ExtFunctionDeviceOperationAllowed,              // +

    ///////////////////////////////////////////
    //      Windows 8.1
    ExtFunctionGetProcessorIndexFromNumber,         // 56 - available in Win7 
    ExtFunctionPoFxSetIdleTimeout,
    ExtFunctionMiniportEtwEvent2,
    ExtFunctionMiniportEtwEvent4,
    ExtFunctionMiniportEtwEvent8,
    ExtFunctionCurrentOsInstallationUpgrade,

    ///////////////////////////////////////////
    //      Windows 10
    ExtFunctionRegistryReadAdapterKey,
    ExtFunctionRegistryWriteAdapterKey,
    ExtFunctionSetAdapterBusType,
    ExtFunctionPoFxRegisterPerfStates,
    ExtFunctionPoFxSetPerfState,
    ExtFunctionGetD3ColdSupport,
    ExtFunctionInitializeRpmb,
    ExtFunctionAllocateHmb,
    ExtFunctionFreeHmb,
    ExtFunctionPropagateIrpExtension,
    ExtFunctionInterlockedInsertHeadList,
    ExtFunctionInterlockedInsertTailList,
    ExtFunctionInterlockedRemoveHeadList,
    ExtFunctionInitializeSpinlock,
    ExtFunctionGetPfns,
    ExtFunctionInitializeCryptoEngine,
    ExtFunctionGetRequestCryptoInfo,
    ExtFunctionMiniportTelemetry
} STORPORT_FUNCTION_CODE, *PSTORPORT_FUNCTION_CODE;






/////////////////////////////////////////////////////////
// early + external definition

BOOLEAN
KeSetCoalescableTimer_k8 (
    KTIMER         *Timer,
    LARGE_INTEGER   DueTime,
    ULONG           Period,
    ULONG           TolerableDelay,
    KDPC           *Dpc);


BOOLEAN
StorPortRegistryRead (
    PVOID HwDeviceExtension,
    PUCHAR ValueName,
    ULONG Global,
    ULONG Type,
    PUCHAR Buffer,
    PULONG BufferLength );


ULONG
StorPortExtendedFunction (
    STORPORT_FUNCTION_CODE FunctionCode,
    PVOID HwDeviceExtension,
    ... );


void
StorportInitialize(void);

NTSTATUS
EtwRegister_k8 (
    LPCGUID             ProviderId,
    PETWENABLECALLBACK  EnableCallback,
    PVOID               CallbackContext,
    PREGHANDLE          RegHandle );

void PrintNumaCpuConfiguration(void);

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
