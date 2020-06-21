#include <stddef.h>

#include <ntddk.h> 
#include <wdm.h>        // ntddk + wdm

//#include <storport.h>
#define STOR_STATUS_SUCCESS (0x00000000L)

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
    ExtFunctionFreeContiguousMemorySpecifyCache,
    
    ///////////////////////////////////////////
    //      Windows 8
    ExtFunctionSetPowerSettingNotificationGuids,
    ExtFunctionInvokeAcpiMethod,
    ExtFunctionGetRequestInfo,
    ExtFunctionInitializeWorker,
    ExtFunctionQueueWorkItem,
    ExtFunctionFreeWorker,
    ExtFunctionInitializeTimer,
    ExtFunctionRequestTimer,
    ExtFunctionFreeTimer,
    ExtFunctionInitializeSListHead,
    ExtFunctionInterlockedFlushSList,
    ExtFunctionInterlockedPopEntrySList,
    ExtFunctionInterlockedPushEntrySList,
    ExtFunctionQueryDepthSList,
    ExtFunctionGetActivityId,
    ExtFunctionGetSystemPortNumber,
    ExtFunctionGetDataInBufferMdl,
    ExtFunctionGetDataInBufferSystemAddress,
    ExtFunctionGetDataInBufferScatterGatherList,
    ExtFunctionMarkDumpMemory,
    ExtFunctionSetUnitAttributes,
    ExtFunctionQueryPerformanceCounter,
    ExtFunctionInitializePoFxPower,
    ExtFunctionPoFxActivateComponent,
    ExtFunctionPoFxIdleComponent,
    ExtFunctionPoFxSetComponentLatency,
    ExtFunctionPoFxSetComponentResidency,
    ExtFunctionPoFxPowerControl,
    ExtFunctionFlushDataBufferMdl,
    ExtFunctionDeviceOperationAllowed,

    ///////////////////////////////////////////
    //      Windows 8.1
    ExtFunctionGetProcessorIndexFromNumber,     // 56 - available in Win7 
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


ULONG
StorPortExtendedFunction (
    STORPORT_FUNCTION_CODE FunctionCode,
    PVOID HwDeviceExtension,
    ... );


VOID
StorportInitialize()
{
    ;
}


ULONG __cdecl
StorPortExtendedFunction_k8 (
    STORPORT_FUNCTION_CODE FunctionCode,
    PVOID HwDeviceExtension,
    size_t va1, size_t va2, size_t va3, size_t va4, size_t va5, size_t va6, size_t va7,
    size_t va8, size_t va9, size_t va10, size_t va11, size_t va12, size_t va13 )
{
    if (FunctionCode <= ExtFunctionFreeContiguousMemorySpecifyCache ||
        FunctionCode == ExtFunctionGetProcessorIndexFromNumber)
            return StorPortExtendedFunction(FunctionCode, HwDeviceExtension,
                     va1, va2, va3, va4, va5, va6, va7, va8,
                     va9, va10, va11, va12, va13);


////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionMarkDumpMemory) {
        /*
        ULONG StorPortMarkDumpMemory(
            PVOID HwDeviceExtension,
            PVOID Address,
            PVOID Length,
            PVOID Flags );
        */
        return STOR_STATUS_SUCCESS;
    }

    if (FunctionCode == ExtFunctionInitializePoFxPower) {
        /*
        ULONG StorPortInitializePoFxPower(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            PSTOR_POFX_DEVICE Device,
            PBOOLEAN D3ColdEnabled );
        */
        return STOR_STATUS_SUCCESS;
    }

    if (FunctionCode == ExtFunctionPoFxActivateComponent) {
        /*
        ULONG StorPortPoFxActivateComponent(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            PSCSI_REQUEST_BLOCK Srb,
            ULONG Component,
            ULONG Flags);
        */
        return STOR_STATUS_SUCCESS;
    }

    if (FunctionCode == ExtFunctionPoFxIdleComponent) {
        /*
        ULONG StorPortPoFxIdleComponent(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            PSCSI_REQUEST_BLOCK Srb,
            ULONG Component,
            ULONG Flags);
        */
        return STOR_STATUS_SUCCESS;
    }

    if (FunctionCode == ExtFunctionGetActivityId) {
        /*
        ULONG StorPortGetActivityIdSrb(
            PVOID HwDeviceExtension,
            PSCSI_REQUEST_BLOCK Srb,
            LPGUID ActivityId ); 
        */
        return STOR_STATUS_SUCCESS;
    }

    return STOR_STATUS_SUCCESS;     // default
    
}    



#include "storport_redirects.h"