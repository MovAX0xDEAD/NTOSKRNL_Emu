/////////////////////////////////////////
//           Windows 2000              //
#if (NTDDI_VERSION <= NTDDI_WIN2KSP4)  //
/////////////////////////////////////////


#endif
/////////////////////////////////////////
//           Windows XP SP1            //
#if (NTDDI_VERSION <= NTDDI_WINXPSP1)  //
/////////////////////////////////////////

//	 KeFlushQueuedDpcs   ()
#define _KeFlushQueuedDpcs
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeFlushQueuedDpcs=_KeFlushQueuedDpcs_k8@0")
#else
 #pragma comment (linker, "/export:KeFlushQueuedDpcs=KeFlushQueuedDpcs_k8")
#endif


//	 ExAcquireRundownProtectionEx  (FASTCALL) (2 * 4)
#define _ExAcquireRundownProtectionEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExAcquireRundownProtectionEx=@ExAcquireRundownProtectionEx_k8@8")
#else
 #pragma comment (linker, "/export:ExAcquireRundownProtectionEx=ExAcquireRundownProtectionEx_k8")
#endif


//	 ExReleaseRundownProtectionEx  (FASTCALL) (2 * 4)
#define _ExReleaseRundownProtectionEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExReleaseRundownProtectionEx=@ExReleaseRundownProtectionEx_k8@8")
#else
 #pragma comment (linker, "/export:ExReleaseRundownProtectionEx=ExReleaseRundownProtectionEx_k8")
#endif




#endif
/////////////////////////////////////////
//         Windows XP SP2+SP3          //
#if (NTDDI_VERSION <= NTDDI_WINXPSP4)  //
/////////////////////////////////////////

//	 KdRefreshDebuggerNotPresent ()
#define _KdRefreshDebuggerNotPresent
#ifndef _AMD64_
 #pragma comment (linker, "/export:KdRefreshDebuggerNotPresent=_KdRefreshDebuggerNotPresent_k8@0")
#else
 #pragma comment (linker, "/export:KdRefreshDebuggerNotPresent=KdRefreshDebuggerNotPresent_k8")
#endif	


//	 KeInitializeGuardedMutex (FASTCALL) (4)
#define _KeInitializeGuardedMutex
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeInitializeGuardedMutex=@KeInitializeGuardedMutex_k8@4")
#else
 #pragma comment (linker, "/export:KeInitializeGuardedMutex=KeInitializeGuardedMutex_k8")
#endif	


//	 KeAcquireGuardedMutex (FASTCALL) (4)
#define _KeAcquireGuardedMutex
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeAcquireGuardedMutex=@KeAcquireGuardedMutex_k8@4")
#else
 #pragma comment (linker, "/export:KeAcquireGuardedMutex=KeAcquireGuardedMutex_k8")
#endif	


//	 KeTryToAcquireGuardedMutex (FASTCALL) (4)
#define _KeTryToAcquireGuardedMutex
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeTryToAcquireGuardedMutex=@KeTryToAcquireGuardedMutex_k8@4")
#else
 #pragma comment (linker, "/export:KeTryToAcquireGuardedMutex=KeTryToAcquireGuardedMutex_k8")
#endif


//	 KeReleaseGuardedMutex (FASTCALL) (4)
#define _KeReleaseGuardedMutex
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeReleaseGuardedMutex=@KeReleaseGuardedMutex_k8@4")
#else
 #pragma comment (linker, "/export:KeReleaseGuardedMutex=KeReleaseGuardedMutex_k8")
#endif	


//	 KeAcquireGuardedMutexUnsafe (FASTCALL) (4)
#define _KeAcquireGuardedMutexUnsafe
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeAcquireGuardedMutexUnsafe=@KeAcquireGuardedMutexUnsafe_k8@4")
#else
 #pragma comment (linker, "/export:KeAcquireGuardedMutexUnsafe=KeAcquireGuardedMutexUnsafe_k8")
#endif	


//	 KeReleaseGuardedMutexUnsafe (FASTCALL) (4)
#define _KeReleaseGuardedMutexUnsafe
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeReleaseGuardedMutexUnsafe=@KeReleaseGuardedMutexUnsafe_k8@4")
#else
 #pragma comment (linker, "/export:KeReleaseGuardedMutexUnsafe=KeReleaseGuardedMutexUnsafe_k8")
#endif


//	 KeEnterGuardedRegion ()
#define _KeEnterGuardedRegion
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeEnterGuardedRegion=_KeEnterGuardedRegion_k8@0")
#else
 #pragma comment (linker, "/export:KeEnterGuardedRegion=KeEnterGuardedRegion_k8")
#endif


//	 KeLeaveGuardedRegion ()
#define _KeLeaveGuardedRegion
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeLeaveGuardedRegion=_KeLeaveGuardedRegion_k8@0")
#else
 #pragma comment (linker, "/export:KeLeaveGuardedRegion=KeLeaveGuardedRegion_k8")
#endif


//	 ExEnterCriticalRegionAndAcquireResourceExclusive (PERESOURCE)
#define _ExEnterCriticalRegionAndAcquireResourceExclusive
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireResourceExclusive=_ExEnterCriticalRegionAndAcquireResourceExclusive_k8@4")
#else
 #pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireResourceExclusive=ExEnterCriticalRegionAndAcquireResourceExclusive_k8")
#endif


//	 ExReleaseResourceAndLeaveCriticalRegion (FASTCALL) (PERESOURCE)
#define _ExReleaseResourceAndLeaveCriticalRegion
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExReleaseResourceAndLeaveCriticalRegion=@ExReleaseResourceAndLeaveCriticalRegion_k8@4")
#else
 #pragma comment (linker, "/export:ExReleaseResourceAndLeaveCriticalRegion=ExReleaseResourceAndLeaveCriticalRegion_k8")
#endif


//	 ExAllocateCacheAwareRundownProtection (2 * 4)
#define _ExAllocateCacheAwareRundownProtection
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExAllocateCacheAwareRundownProtection=_ExAllocateCacheAwareRundownProtection_k8@8")
#else
 #pragma comment (linker, "/export:ExAllocateCacheAwareRundownProtection=ExAllocateCacheAwareRundownProtection_k8")
#endif


//	 ExInitializeRundownProtectionCacheAware (2 * 4)
#define _ExInitializeRundownProtectionCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExInitializeRundownProtectionCacheAware=_ExInitializeRundownProtectionCacheAware_k8@8")
#else
 #pragma comment (linker, "/export:ExInitializeRundownProtectionCacheAware=ExInitializeRundownProtectionCacheAware_k8")
#endif


//	 ExReInitializeRundownProtectionCacheAware (FASTCALL) (4)
#define _ExReInitializeRundownProtectionCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExReInitializeRundownProtectionCacheAware=@ExReInitializeRundownProtectionCacheAware_k8@4")
#else
 #pragma comment (linker, "/export:ExReInitializeRundownProtectionCacheAware=ExReInitializeRundownProtectionCacheAware_k8")
#endif


//	 ExAcquireRundownProtectionCacheAware (FASTCALL) (4)
#define _ExAcquireRundownProtectionCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAware=@ExAcquireRundownProtectionCacheAware_k8@4")
#else
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAware=ExAcquireRundownProtectionCacheAware_k8")
#endif


//	 ExAcquireRundownProtectionCacheAwareEx (FASTCALL) (8)
#define _ExAcquireRundownProtectionCacheAwareEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAwareEx=@ExAcquireRundownProtectionCacheAwareEx_k8@8")
#else
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAwareEx=ExAcquireRundownProtectionCacheAwareEx_k8")
#endif


//	 ExFreeCacheAwareRundownProtection (4)
#define _ExFreeCacheAwareRundownProtection
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExFreeCacheAwareRundownProtection=_ExFreeCacheAwareRundownProtection_k8@4")
#else
 #pragma comment (linker, "/export:ExFreeCacheAwareRundownProtection=ExFreeCacheAwareRundownProtection_k8")
#endif


//	 ExReleaseRundownProtectionCacheAware (FASTCALL) (4)
#define _ExReleaseRundownProtectionCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAware=@ExReleaseRundownProtectionCacheAware_k8@4")
#else
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAware=ExReleaseRundownProtectionCacheAware_k8")
#endif   


//	 ExReleaseRundownProtectionCacheAwareEx (FASTCALL) (8)
#define _ExReleaseRundownProtectionCacheAwareEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAwareEx=@ExReleaseRundownProtectionCacheAwareEx_k8@8")
#else
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAwareEx=ExReleaseRundownProtectionCacheAwareEx_k8")
#endif


//	 ExSizeOfRundownProtectionCacheAware ()
#define _ExSizeOfRundownProtectionCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExSizeOfRundownProtectionCacheAware=_ExSizeOfRundownProtectionCacheAware_k8@0")
#else
 #pragma comment (linker, "/export:ExSizeOfRundownProtectionCacheAware=ExSizeOfRundownProtectionCacheAware_k8")
#endif


//	 ExWaitForRundownProtectionReleaseCacheAware (FASTCALL) (4)
#define _ExWaitForRundownProtectionReleaseCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExWaitForRundownProtectionReleaseCacheAware=@ExWaitForRundownProtectionReleaseCacheAware_k8@4")
#else
 #pragma comment (linker, "/export:ExWaitForRundownProtectionReleaseCacheAware=ExWaitForRundownProtectionReleaseCacheAware_k8")
#endif   
   
//	 ExRundownCompletedCacheAware (FASTCALL) (4)
#define _ExRundownCompletedCacheAware
#ifndef _AMD64_
 #pragma comment (linker, "/export:ExRundownCompletedCacheAware=@ExRundownCompletedCacheAware_k8@4")
#else
 #pragma comment (linker, "/export:ExRundownCompletedCacheAware=ExRundownCompletedCacheAware_k8")
#endif      




#endif
/////////////////////////////////////////
//           Windows 2003              //
#if (NTDDI_VERSION <= NTDDI_WS03SP4)   //
/////////////////////////////////////////

//	 PoDisableSleepStates   (3 * 4)
#define _PoDisableSleepStates
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoDisableSleepStates=_PoDisableSleepStates_k8@12")
#else
 #pragma comment (linker, "/export:PoDisableSleepStates=PoDisableSleepStates_k8")
#endif	


//	 EtwRegister          (4 * 4)
#define _EtwRegister
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwRegister=_EtwRegister_k8@16")
#else
 #pragma comment (linker, "/export:EtwRegister=EtwRegister_k8")
#endif	

//	 EtwUnregister          (2 * 4)
#define _EtwUnregister
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwUnregister=_EtwUnregister_k8@8")
#else
 #pragma comment (linker, "/export:EtwUnregister=EtwUnregister_k8")
#endif	


//	 EtwProviderEnabled     (5 * 4)
#define _EtwProviderEnabled
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwProviderEnabled=_EtwProviderEnabled_k8@20")
#else
 #pragma comment (linker, "/export:EtwProviderEnabled=EtwProviderEnabled_k8")
#endif	


//	 EtwEventEnabled        (3 * 4)
#define _EtwEventEnabled
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwEventEnabled=_EtwEventEnabled_k8@12")
#else
 #pragma comment (linker, "/export:EtwEventEnabled=EtwEventEnabled_k8")
#endif	


//	 EtwWrite          (6 * 4)
#define _EtwWrite
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwWrite=_EtwWrite_k8@24")
#else
 #pragma comment (linker, "/export:EtwWrite=EtwWrite_k8")
#endif	


//	 EtwActivityIdControl  (2 * 4)
#define _EtwActivityIdControl
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwActivityIdControl=_EtwActivityIdControl_k8@8")
#else
 #pragma comment (linker, "/export:EtwActivityIdControl=EtwActivityIdControl_k8")
#endif


//	 EtwWriteTransfer  (7 * 4)
#define _EtwWriteTransfer
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwWriteTransfer=_EtwWriteTransfer_k8@28")
#else
 #pragma comment (linker, "/export:EtwWriteTransfer=EtwWriteTransfer_k8")
#endif	


//	 EtwWriteString  (7 * 4)
#define _EtwWriteString
#ifndef _AMD64_
 #pragma comment (linker, "/export:EtwWriteString=_EtwWriteString_k8@28")
#else
 #pragma comment (linker, "/export:EtwWriteString=EtwWriteString_k8")
#endif	


//	 IoGetDevicePropertyData     (8 * 4)
#define _IoGetDevicePropertyData
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoGetDevicePropertyData=_IoGetDevicePropertyData_k8@32")
#else
 #pragma comment (linker, "/export:IoGetDevicePropertyData=IoGetDevicePropertyData_k8")
#endif

//	 IoSetDevicePropertyData     (8 * 4)
#define _IoSetDevicePropertyData
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoSetDevicePropertyData=_IoSetDevicePropertyData_k8@28")
#else
 #pragma comment (linker, "/export:IoSetDevicePropertyData=IoSetDevicePropertyData_k8")
#endif


//	 ZwAlpcConnectPort     (11 * 4)
#define _ZwAlpcConnectPort
#ifndef _AMD64_
 #pragma comment (linker, "/export:ZwAlpcConnectPort=_ZwAlpcConnectPort_k8@44")
#else
 #pragma comment (linker, "/export:ZwAlpcConnectPort=ZwAlpcConnectPort_k8")
#endif


//	 ZwAlpcSendWaitReceivePort     (8 * 4)
#define _ZwAlpcSendWaitReceivePort
#ifndef _AMD64_
 #pragma comment (linker, "/export:ZwAlpcSendWaitReceivePort=_ZwAlpcSendWaitReceivePort_k8@32")
#else
 #pragma comment (linker, "/export:ZwAlpcSendWaitReceivePort=ZwAlpcSendWaitReceivePort_k8")
#endif


//	 IoQueueWorkItemEx     (4 * 4)
#define _IoQueueWorkItemEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoQueueWorkItemEx=_IoQueueWorkItemEx_k8@16")
#else
 #pragma comment (linker, "/export:IoQueueWorkItemEx=IoQueueWorkItemEx_k8")
#endif


//	 IoInitializeWorkItem     (4 * 4)
#define _IoInitializeWorkItem
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoInitializeWorkItem=_IoInitializeWorkItem_k8@8")
#else
 #pragma comment (linker, "/export:IoInitializeWorkItem=IoInitializeWorkItem_k8")
#endif


//	 IoSizeofWorkItem     ()
#define _IoSizeofWorkItem
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoSizeofWorkItem=_IoSizeofWorkItem_k8@0")
#else
 #pragma comment (linker, "/export:IoSizeofWorkItem=IoSizeofWorkItem_k8")
#endif


//	 IoUninitializeWorkItem     (PIO_WORKITEM)
#define _IoUninitializeWorkItem
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoUninitializeWorkItem=_IoUninitializeWorkItem_k8@4")
#else
 #pragma comment (linker, "/export:IoUninitializeWorkItem=IoUninitializeWorkItem_k8")
#endif


// IoFreeWorkItem (PIO_WORKITEM)
#define _IoFreeWorkItem
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoFreeWorkItem=_IoFreeWorkItem_k8@4")
#else
 #pragma comment (linker, "/export:IoFreeWorkItem=IoFreeWorkItem_k8")
#endif	


//	 KeQueryDpcWatchdogInformation     (PPROCESSOR_NUMBER)
#define _KeQueryDpcWatchdogInformation
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryDpcWatchdogInformation=_KeQueryDpcWatchdogInformation_k8@4")
#else
 #pragma comment (linker, "/export:KeQueryDpcWatchdogInformation=KeQueryDpcWatchdogInformation_k8")
#endif


//	 KeQueryActiveProcessorCount     (PKAFFINITY)
#define _KeQueryActiveProcessorCount
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryActiveProcessorCount=_KeQueryActiveProcessorCount_k8@4")
#else
 #pragma comment (linker, "/export:KeQueryActiveProcessorCount=KeQueryActiveProcessorCount_k8")
#endif


//	 KeQueryMaximumProcessorCount     ()
#define _KeQueryMaximumProcessorCount
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCount=_KeQueryMaximumProcessorCount_k8@0")
#else
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCount=KeQueryMaximumProcessorCount_k8")
#endif


//	 KeSetSystemAffinityThreadEx     (KAFFINITY)
#define _KeSetSystemAffinityThreadEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeSetSystemAffinityThreadEx=_KeSetSystemAffinityThreadEx_k8@4")
#else
 #pragma comment (linker, "/export:KeSetSystemAffinityThreadEx=KeSetSystemAffinityThreadEx_k8")
#endif


//	 KeRevertToUserAffinityThreadEx     (KAFFINITY)
#define _KeRevertToUserAffinityThreadEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeRevertToUserAffinityThreadEx=_KeRevertToUserAffinityThreadEx_k8@4")
#else
 #pragma comment (linker, "/export:KeRevertToUserAffinityThreadEx=KeRevertToUserAffinityThreadEx_k8")
#endif


//	 IoConnectInterruptEx     (PIO_CONNECT_INTERRUPT_PARAMETERS)
#define _IoConnectInterruptEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoConnectInterruptEx=_IoConnectInterruptEx_k8@4")
#else
 #pragma comment (linker, "/export:IoConnectInterruptEx=IoConnectInterruptEx_k8")
#endif


//	 IoDisconnectInterruptEx     (PIO_CONNECT_INTERRUPT_PARAMETERS)
#define _IoDisconnectInterruptEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoDisconnectInterruptEx=_IoDisconnectInterruptEx_k8@4")
#else
 #pragma comment (linker, "/export:IoDisconnectInterruptEx=IoDisconnectInterruptEx_k8")
#endif


//	 PoGetSystemWake     (PIRP)
#define _PoGetSystemWake
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoGetSystemWake=_PoGetSystemWake_k8@4")
#else
 #pragma comment (linker, "/export:PoGetSystemWake=PoGetSystemWake_k8")
#endif


//	 PoSetSystemWake     (PIRP)
#define _PoSetSystemWake
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoSetSystemWake=_PoSetSystemWake_k8@4")
#else
 #pragma comment (linker, "/export:PoSetSystemWake=PoSetSystemWake_k8")
#endif


//	 PoRegisterPowerSettingCallback     (5 * 4)
#define _PoRegisterPowerSettingCallback
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoRegisterPowerSettingCallback=_PoRegisterPowerSettingCallback_k8@20")
#else
 #pragma comment (linker, "/export:PoRegisterPowerSettingCallback=PoRegisterPowerSettingCallback_k8")
#endif


//	 PoUnregisterPowerSettingCallback     (PVOID)
#define _PoUnregisterPowerSettingCallback
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoUnregisterPowerSettingCallback=_PoUnregisterPowerSettingCallback_k8@4")
#else
 #pragma comment (linker, "/export:PoUnregisterPowerSettingCallback=PoUnregisterPowerSettingCallback_k8")
#endif


//	 MmAllocateContiguousMemorySpecifyCacheNode  (9 * 4)
#define _MmAllocateContiguousMemorySpecifyCacheNode
#ifndef _AMD64_
 #pragma comment (linker, "/export:MmAllocateContiguousMemorySpecifyCacheNode=_MmAllocateContiguousMemorySpecifyCacheNode_k8@36")
#else
 #pragma comment (linker, "/export:MmAllocateContiguousMemorySpecifyCacheNode=MmAllocateContiguousMemorySpecifyCacheNode_k8")
#endif


//	 SeSetAuditParameter  (4 * 4)
#define _SeSetAuditParameter
#ifndef _AMD64_
 #pragma comment (linker, "/export:SeSetAuditParameter=_SeSetAuditParameter_k8@16")
#else
 #pragma comment (linker, "/export:SeSetAuditParameter=SeSetAuditParameter_k8")
#endif





#endif
/////////////////////////////////////////
//         Windows Vista/W2008         //
#if (NTDDI_VERSION <= NTDDI_VISTASP4)  //
//                 <= NTDDI_W2008SP4   //
/////////////////////////////////////////

//	 wcsncpy_s     (FASTCALL CDECL)
#define _wcsncpy_s
#ifndef _AMD64_
 #pragma comment (linker, "/export:wcsncpy_s=_wcsncpy_s_k8")
#else
 #pragma comment (linker, "/export:wcsncpy_s=wcsncpy_s_k8")
#endif


//	 KeGetProcessorIndexFromNumber     (PPROCESSOR_NUMBER)
#define _KeGetProcessorIndexFromNumber
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeGetProcessorIndexFromNumber=_KeGetProcessorIndexFromNumber_k8@4")
#else
 #pragma comment (linker, "/export:KeGetProcessorIndexFromNumber=KeGetProcessorIndexFromNumber_k8")
#endif


//	 KeGetProcessorNumberFromIndex   ( 2 * 4)
#define _KeGetProcessorNumberFromIndex
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeGetProcessorNumberFromIndex=_KeGetProcessorNumberFromIndex_k8@8")
#else
 #pragma comment (linker, "/export:KeGetProcessorNumberFromIndex=KeGetProcessorNumberFromIndex_k8")
#endif


//	 KeQueryActiveProcessorCountEx     (USHORT)
#define _KeQueryActiveProcessorCountEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryActiveProcessorCountEx=_KeQueryActiveProcessorCountEx_k8@4")
#else
 #pragma comment (linker, "/export:KeQueryActiveProcessorCountEx=KeQueryActiveProcessorCountEx_k8")
#endif


//	 KeGetCurrentProcessorNumberEx     (PPROCESSOR_NUMBER)
#define _KeGetCurrentProcessorNumberEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeGetCurrentProcessorNumberEx=_KeGetCurrentProcessorNumberEx_k8@4")
#else
 #pragma comment (linker, "/export:KeGetCurrentProcessorNumberEx=KeGetCurrentProcessorNumberEx_k8")
#endif


//	 KeQueryMaximumProcessorCountEx   (USHORT)
#define _KeQueryMaximumProcessorCountEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCountEx=_KeQueryMaximumProcessorCountEx_k8@4")
#else
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCountEx=KeQueryMaximumProcessorCountEx_k8")
#endif


//	 KeQueryActiveGroupCount   ()
#define _KeQueryActiveGroupCount
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryActiveGroupCount=_KeQueryActiveGroupCount_k8@0")
#else
 #pragma comment (linker, "/export:KeQueryActiveGroupCount=KeQueryActiveGroupCount_k8")
#endif


//	 KeQueryMaximumGroupCount   ()
#define _KeQueryMaximumGroupCount
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryMaximumGroupCount=_KeQueryMaximumGroupCount_k8@0")
#else
 #pragma comment (linker, "/export:KeQueryMaximumGroupCount=KeQueryMaximumGroupCount_k8")
#endif


//	 KeSetSystemGroupAffinityThread   ( 2 * 4)
#define _KeSetSystemGroupAffinityThread
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeSetSystemGroupAffinityThread=_KeSetSystemGroupAffinityThread_k8@8")
#else
 #pragma comment (linker, "/export:KeSetSystemGroupAffinityThread=KeSetSystemGroupAffinityThread_k8")
#endif


//	 KeRevertToUserGroupAffinityThread   (PGROUP_AFFINITY)
#define _KeRevertToUserGroupAffinityThread
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeRevertToUserGroupAffinityThread=_KeRevertToUserGroupAffinityThread_k8@4")
#else
 #pragma comment (linker, "/export:KeRevertToUserGroupAffinityThread=KeRevertToUserGroupAffinityThread_k8")
#endif


//	 KeSetTargetProcessorDpcEx   ( 2 * 4)
#define _KeSetTargetProcessorDpcEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeSetTargetProcessorDpcEx=_KeSetTargetProcessorDpcEx_k8@8")
#else
 #pragma comment (linker, "/export:KeSetTargetProcessorDpcEx=KeSetTargetProcessorDpcEx_k8")
#endif


//	 KeGetCurrentNodeNumber   ()
#define _KeGetCurrentNodeNumber
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeGetCurrentNodeNumber=_KeGetCurrentNodeNumber_k8@0")
#else
 #pragma comment (linker, "/export:KeGetCurrentNodeNumber=KeGetCurrentNodeNumber_k8")
#endif


//	 KeQueryHighestNodeNumber   ()
#define _KeQueryHighestNodeNumber
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryHighestNodeNumber=_KeQueryHighestNodeNumber_k8@0")
#else
 #pragma comment (linker, "/export:KeQueryHighestNodeNumber=KeQueryHighestNodeNumber_k8")
#endif


//	 KeQueryNodeActiveAffinity   (3 * 4)
#define _KeQueryNodeActiveAffinity
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryNodeActiveAffinity=_KeQueryNodeActiveAffinity_k8@12")
#else
 #pragma comment (linker, "/export:KeQueryNodeActiveAffinity=KeQueryNodeActiveAffinity_k8")
#endif


//	 KeQueryGroupAffinity   (USHORT)
#define _KeQueryGroupAffinity
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryGroupAffinity=_KeQueryGroupAffinity_k8@4")
#else
 #pragma comment (linker, "/export:KeQueryGroupAffinity=KeQueryGroupAffinity_k8")
#endif


//	 IoGetAffinityInterrupt   (2 * 4)
#define _IoGetAffinityInterrupt
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoGetAffinityInterrupt=_IoGetAffinityInterrupt_k8@8")
#else
 #pragma comment (linker, "/export:IoGetAffinityInterrupt=IoGetAffinityInterrupt_k8")
#endif


//	 KeQueryLogicalProcessorRelationship   (4 * 4)
#define _KeQueryLogicalProcessorRelationship
#ifndef _AMD64_
 #pragma comment (linker, "/export:KeQueryLogicalProcessorRelationship=_KeQueryLogicalProcessorRelationship_k8@16")
#else
 #pragma comment (linker, "/export:KeQueryLogicalProcessorRelationship=KeQueryLogicalProcessorRelationship_k8")
#endif


//	 ObfReferenceObjectWithTag   (FASTCALL) (2 * 4)
#define _ObfReferenceObjectWithTag
#ifndef _AMD64_
 #pragma comment (linker, "/export:ObfReferenceObjectWithTag=@ObfReferenceObjectWithTag_k8@8")
#else
 #pragma comment (linker, "/export:ObfReferenceObjectWithTag=ObfReferenceObjectWithTag_k8")
#endif


//	 ObfDereferenceObjectWithTag   (FASTCALL) (2 * 4)
#define _ObfDereferenceObjectWithTag
#ifndef _AMD64_
 #pragma comment (linker, "/export:ObfDereferenceObjectWithTag=@ObfDereferenceObjectWithTag_k8@8")
#else
 #pragma comment (linker, "/export:ObfDereferenceObjectWithTag=ObfDereferenceObjectWithTag_k8")
#endif


//	 PoStartDeviceBusy   (PULONG)
#define _PoStartDeviceBusy
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoStartDeviceBusy=_PoStartDeviceBusy_k8@4")
#else
 #pragma comment (linker, "/export:PoStartDeviceBusy=PoStartDeviceBusy_k8")
#endif


//	 PoEndDeviceBusy   (PULONG)
#define _PoEndDeviceBusy
#ifndef _AMD64_
 #pragma comment (linker, "/export:PoEndDeviceBusy=_PoEndDeviceBusy_k8@4")
#else
 #pragma comment (linker, "/export:PoEndDeviceBusy=PoEndDeviceBusy_k8")
#endif


//	 LpcReplyWaitReplyPort   (PULONG)
#define _LpcReplyWaitReplyPort
#ifndef _AMD64_
 #pragma comment (linker, "/export:LpcReplyWaitReplyPort=_LpcReplyWaitReplyPort_k8@12")
#else
 #pragma comment (linker, "/export:LpcReplyWaitReplyPort=LpcReplyWaitReplyPort_k8")
#endif



//	 LpcSendWaitReceivePort   (7 * 4)
#define _LpcSendWaitReceivePort
#ifndef _AMD64_
 #pragma comment (linker, "/export:LpcSendWaitReceivePort=_LpcSendWaitReceivePort_k8@28")
#else
 #pragma comment (linker, "/export:LpcSendWaitReceivePort=LpcSendWaitReceivePort_k8")
#endif



//	 SeSetAuthorizationCallbacks   (2 * 4)
#define _SeSetAuthorizationCallbacks
#ifndef _AMD64_
 #pragma comment (linker, "/export:SeSetAuthorizationCallbacks=_SeSetAuthorizationCallbacks_k8@8")
#else
 #pragma comment (linker, "/export:SeSetAuthorizationCallbacks=SeSetAuthorizationCallbacks_k8")
#endif


//	 SeReportSecurityEventWithSubCategory   (5 * 4)
#define _SeReportSecurityEventWithSubCategory
#ifndef _AMD64_
 #pragma comment (linker, "/export:SeReportSecurityEventWithSubCategory=_SeReportSecurityEventWithSubCategory_k8@20")
#else
 #pragma comment (linker, "/export:SeReportSecurityEventWithSubCategory=SeReportSecurityEventWithSubCategory_k8")
#endif


//	 PcwRegister   (2 * 4)
#define _PcwRegister
#ifndef _AMD64_
 #pragma comment (linker, "/export:PcwRegister=_PcwRegister_k8@8")
#else
 #pragma comment (linker, "/export:PcwRegister=PcwRegister_k8")
#endif


//	 PcwUnregister   (4)
#define _PcwUnregister
#ifndef _AMD64_
 #pragma comment (linker, "/export:PcwUnregister=_PcwUnregister_k8@4")
#else
 #pragma comment (linker, "/export:PcwUnregister=PcwUnregister_k8")
#endif


//	 PcwCreateInstance   (5 * 4)
#define _PcwCreateInstance
#ifndef _AMD64_
 #pragma comment (linker, "/export:PcwCreateInstance=_PcwCreateInstance_k8@20")
#else
 #pragma comment (linker, "/export:PcwCreateInstance=PcwCreateInstance_k8")
#endif


//	 PcwCloseInstance   (4)
#define _PcwCloseInstance
#ifndef _AMD64_
 #pragma comment (linker, "/export:PcwCloseInstance=_PcwCloseInstance_k8@4")
#else
 #pragma comment (linker, "/export:PcwCloseInstance=PcwCloseInstance_k8")
#endif


//	 PcwAddInstance   (5 * 4)
#define _PcwAddInstance
#ifndef _AMD64_
 #pragma comment (linker, "/export:PcwAddInstance=_PcwAddInstance_k8@20")
#else
 #pragma comment (linker, "/export:PcwAddInstance=PcwAddInstance_k8")
#endif




#endif
/////////////////////////////////////////
//           Windows 7                 //
#if (NTDDI_VERSION <= NTDDI_WIN7)      //
/////////////////////////////////////////

//	 RtlQueryRegistryValuesEx        (5 * 4)
#define _RtlQueryRegistryValuesEx
#ifndef _AMD64_
 #pragma comment (linker, "/export:RtlQueryRegistryValuesEx=_RtlQueryRegistryValuesEx_k8@20")
#else
 #pragma comment (linker, "/export:RtlQueryRegistryValuesEx=RtlQueryRegistryValuesEx_k8")
#endif

//	 RtlCheckPortableOperatingSystem (PBOOLEAN)
#define _RtlCheckPortableOperatingSystem
#ifndef _AMD64_
 #pragma comment (linker, "/export:RtlCheckPortableOperatingSystem=_RtlCheckPortableOperatingSystem_k8@4")
#else
 #pragma comment (linker, "/export:RtlCheckPortableOperatingSystem=RtlCheckPortableOperatingSystem_k8")
#endif

//	 RtlSetPortableOperatingSystem	 (BOOLEAN)
#define _RtlSetPortableOperatingSystem
#ifndef _AMD64_
 #pragma comment (linker, "/export:RtlSetPortableOperatingSystem=_RtlSetPortableOperatingSystem_k8@4")
#else
 #pragma comment (linker, "/export:RtlSetPortableOperatingSystem=RtlSetPortableOperatingSystem_k8")
#endif

//	 IoSetActivityIdIrp              (2 * 4)
#define _IoSetActivityIdIrp
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoSetActivityIdIrp=_IoSetActivityIdIrp_k8@8")
#else
 #pragma comment (linker, "/export:IoSetActivityIdIrp=IoSetActivityIdIrp_k8")
#endif

//	 IoGetActivityIdIrp              (2 * 4)
#define _IoGetActivityIdIrp
#ifndef _AMD64_
 #pragma comment (linker, "/export:IoGetActivityIdIrp=_IoGetActivityIdIrp_k8@8")
#else
 #pragma comment (linker, "/export:IoGetActivityIdIrp=IoGetActivityIdIrp_k8")
#endif


//	 MmAllocateContiguousNodeMemory              (9 * 4)
#define _MmAllocateContiguousNodeMemory
#ifndef _AMD64_
 #pragma comment (linker, "/export:MmAllocateContiguousNodeMemory=_MmAllocateContiguousNodeMemory_k8@36")
#else
 #pragma comment (linker, "/export:MmAllocateContiguousNodeMemory=MmAllocateContiguousNodeMemory_k8")
#endif



#endif
/////////////////////////////////////////
///     overrides any Windows          //
/////////////////////////////////////////

// MmGetSystemRoutineAddress (PUNICODE_STRING)
#ifndef _AMD64_
 #pragma comment (linker, "/export:MmGetSystemRoutineAddress=_MmGetSystemRoutineAddress_force@4")
#else
 #pragma comment (linker, "/export:MmGetSystemRoutineAddress=MmGetSystemRoutineAddress_force")
#endif	


/////////////////////////////////////////
///      redirect if not yet           //
/////////////////////////////////////////

#ifndef _KeFlushQueuedDpcs
 #pragma comment (linker, "/export:KeFlushQueuedDpcs=ntoskrnl.KeFlushQueuedDpcs")
#endif
#ifndef _ExAcquireRundownProtectionEx
 #pragma comment (linker, "/export:ExAcquireRundownProtectionEx=ntoskrnl.ExAcquireRundownProtectionEx")
#endif
#ifndef _ExReleaseRundownProtectionEx
 #pragma comment (linker, "/export:ExReleaseRundownProtectionEx=ntoskrnl.ExReleaseRundownProtectionEx")
#endif
#ifndef _KdRefreshDebuggerNotPresent
 #pragma comment (linker, "/export:KdRefreshDebuggerNotPresent=ntoskrnl.KdRefreshDebuggerNotPresent")
#endif
#ifndef _KeInitializeGuardedMutex
 #pragma comment (linker, "/export:KeInitializeGuardedMutex=ntoskrnl.KeInitializeGuardedMutex")
#endif
#ifndef _KeAcquireGuardedMutex
 #pragma comment (linker, "/export:KeAcquireGuardedMutex=ntoskrnl.KeAcquireGuardedMutex")
#endif
#ifndef _KeTryToAcquireGuardedMutex
 #pragma comment (linker, "/export:KeTryToAcquireGuardedMutex=ntoskrnl.KeTryToAcquireGuardedMutex")
#endif
#ifndef _KeReleaseGuardedMutex
 #pragma comment (linker, "/export:KeReleaseGuardedMutex=ntoskrnl.KeReleaseGuardedMutex")
#endif
#ifndef _KeAcquireGuardedMutexUnsafe
 #pragma comment (linker, "/export:KeAcquireGuardedMutexUnsafe=ntoskrnl.KeAcquireGuardedMutexUnsafe")
#endif
#ifndef _KeReleaseGuardedMutexUnsafe
 #pragma comment (linker, "/export:KeReleaseGuardedMutexUnsafe=ntoskrnl.KeReleaseGuardedMutexUnsafe")
#endif
#ifndef _KeEnterGuardedRegion
 #pragma comment (linker, "/export:KeEnterGuardedRegion=ntoskrnl.KeEnterGuardedRegion")
#endif
#ifndef _KeLeaveGuardedRegion
 #pragma comment (linker, "/export:KeLeaveGuardedRegion=ntoskrnl.KeLeaveGuardedRegion")
#endif
#ifndef _ExEnterCriticalRegionAndAcquireResourceExclusive
 #pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireResourceExclusive=ntoskrnl.ExEnterCriticalRegionAndAcquireResourceExclusive")
#endif
#ifndef _ExReleaseResourceAndLeaveCriticalRegion
 #pragma comment (linker, "/export:ExReleaseResourceAndLeaveCriticalRegion=ntoskrnl.ExReleaseResourceAndLeaveCriticalRegion")
#endif
#ifndef _ExAllocateCacheAwareRundownProtection
 #pragma comment (linker, "/export:ExAllocateCacheAwareRundownProtection=ntoskrnl.ExAllocateCacheAwareRundownProtection")
#endif
#ifndef _ExInitializeRundownProtectionCacheAware
 #pragma comment (linker, "/export:ExInitializeRundownProtectionCacheAware=ntoskrnl.ExInitializeRundownProtectionCacheAware")
#endif
#ifndef _ExReInitializeRundownProtectionCacheAware
 #pragma comment (linker, "/export:ExReInitializeRundownProtectionCacheAware=ntoskrnl.ExReInitializeRundownProtectionCacheAware")
#endif
#ifndef _ExAcquireRundownProtectionCacheAware
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAware=ntoskrnl.ExAcquireRundownProtectionCacheAware")
#endif
#ifndef _ExAcquireRundownProtectionCacheAwareEx
 #pragma comment (linker, "/export:ExAcquireRundownProtectionCacheAwareEx=ntoskrnl.ExAcquireRundownProtectionCacheAwareEx")
#endif
#ifndef _ExFreeCacheAwareRundownProtection
 #pragma comment (linker, "/export:ExFreeCacheAwareRundownProtection=ntoskrnl.ExFreeCacheAwareRundownProtection")
#endif
#ifndef _ExReleaseRundownProtectionCacheAware
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAware=ntoskrnl.ExReleaseRundownProtectionCacheAware")
#endif
#ifndef _ExReleaseRundownProtectionCacheAwareEx
 #pragma comment (linker, "/export:ExReleaseRundownProtectionCacheAwareEx=ntoskrnl.ExReleaseRundownProtectionCacheAwareEx")
#endif
#ifndef _ExSizeOfRundownProtectionCacheAware
 #pragma comment (linker, "/export:ExSizeOfRundownProtectionCacheAware=ntoskrnl.ExSizeOfRundownProtectionCacheAware")
#endif
#ifndef _ExWaitForRundownProtectionReleaseCacheAware
 #pragma comment (linker, "/export:ExWaitForRundownProtectionReleaseCacheAware=ntoskrnl.ExWaitForRundownProtectionReleaseCacheAware")
#endif
#ifndef _ExRundownCompletedCacheAware
 #pragma comment (linker, "/export:ExRundownCompletedCacheAware=ntoskrnl.ExRundownCompletedCacheAware")
#endif
#ifndef _PoDisableSleepStates
 #pragma comment (linker, "/export:PoDisableSleepStates=ntoskrnl.PoDisableSleepStates")
#endif
#ifndef _EtwRegister
 #pragma comment (linker, "/export:EtwRegister=ntoskrnl.EtwRegister")
#endif
#ifndef _EtwUnregister
 #pragma comment (linker, "/export:EtwUnregister=ntoskrnl.EtwUnregister")
#endif
#ifndef _EtwProviderEnabled
 #pragma comment (linker, "/export:EtwProviderEnabled=ntoskrnl.EtwProviderEnabled")
#endif
#ifndef _EtwEventEnabled
 #pragma comment (linker, "/export:EtwEventEnabled=ntoskrnl.EtwEventEnabled")
#endif
#ifndef _EtwWrite
 #pragma comment (linker, "/export:EtwWrite=ntoskrnl.EtwWrite")
#endif
#ifndef _EtwActivityIdControl
 #pragma comment (linker, "/export:EtwActivityIdControl=ntoskrnl.EtwActivityIdControl")
#endif
#ifndef _EtwWriteTransfer
 #pragma comment (linker, "/export:EtwWriteTransfer=ntoskrnl.EtwWriteTransfer")
#endif
#ifndef _EtwWriteString
 #pragma comment (linker, "/export:EtwWriteString=ntoskrnl.EtwWriteString")
#endif
#ifndef _IoGetDevicePropertyData
 #pragma comment (linker, "/export:IoGetDevicePropertyData=ntoskrnl.IoGetDevicePropertyData")
#endif
#ifndef _IoSetDevicePropertyData
 #pragma comment (linker, "/export:IoSetDevicePropertyData=ntoskrnl.IoSetDevicePropertyData")
#endif
#ifndef _ZwAlpcConnectPort
 #pragma comment (linker, "/export:ZwAlpcConnectPort=ntoskrnl.ZwAlpcConnectPort")
#endif
#ifndef _ZwAlpcSendWaitReceivePort
 #pragma comment (linker, "/export:ZwAlpcSendWaitReceivePort=ntoskrnl.ZwAlpcSendWaitReceivePort")
#endif
#ifndef _IoQueueWorkItemEx
 #pragma comment (linker, "/export:IoQueueWorkItemEx=ntoskrnl.IoQueueWorkItemEx")
#endif
#ifndef _IoInitializeWorkItem
 #pragma comment (linker, "/export:IoInitializeWorkItem=ntoskrnl.IoInitializeWorkItem")
#endif
#ifndef _IoSizeofWorkItem
 #pragma comment (linker, "/export:IoSizeofWorkItem=ntoskrnl.IoSizeofWorkItem")
#endif
#ifndef _IoUninitializeWorkItem
 #pragma comment (linker, "/export:IoUninitializeWorkItem=ntoskrnl.IoUninitializeWorkItem")
#endif
#ifndef _IoFreeWorkItem
 #pragma comment (linker, "/export:IoFreeWorkItem=ntoskrnl.IoFreeWorkItem")
#endif
#ifndef _KeQueryDpcWatchdogInformation
 #pragma comment (linker, "/export:KeQueryDpcWatchdogInformation=ntoskrnl.KeQueryDpcWatchdogInformation")
#endif
#ifndef _KeQueryActiveProcessorCount
 #pragma comment (linker, "/export:KeQueryActiveProcessorCount=ntoskrnl.KeQueryActiveProcessorCount")
#endif
#ifndef _KeQueryMaximumProcessorCount
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCount=ntoskrnl.KeQueryMaximumProcessorCount")
#endif
#ifndef _KeSetSystemAffinityThreadEx
 #pragma comment (linker, "/export:KeSetSystemAffinityThreadEx=ntoskrnl.KeSetSystemAffinityThreadEx")
#endif
#ifndef _KeRevertToUserAffinityThreadEx
 #pragma comment (linker, "/export:KeRevertToUserAffinityThreadEx=ntoskrnl.KeRevertToUserAffinityThreadEx")
#endif
#ifndef _IoConnectInterruptEx
 #pragma comment (linker, "/export:IoConnectInterruptEx=ntoskrnl.IoConnectInterruptEx")
#endif
#ifndef _IoDisconnectInterruptEx
 #pragma comment (linker, "/export:IoDisconnectInterruptEx=ntoskrnl.IoDisconnectInterruptEx")
#endif
#ifndef _PoGetSystemWake
 #pragma comment (linker, "/export:PoGetSystemWake=ntoskrnl.PoGetSystemWake")
#endif
#ifndef _PoSetSystemWake
 #pragma comment (linker, "/export:PoSetSystemWake=ntoskrnl.PoSetSystemWake")
#endif
#ifndef _PoRegisterPowerSettingCallback
 #pragma comment (linker, "/export:PoRegisterPowerSettingCallback=ntoskrnl.PoRegisterPowerSettingCallback")
#endif
#ifndef _PoUnregisterPowerSettingCallback
 #pragma comment (linker, "/export:PoUnregisterPowerSettingCallback=ntoskrnl.PoUnregisterPowerSettingCallback")
#endif
#ifndef _MmAllocateContiguousMemorySpecifyCacheNode
 #pragma comment (linker, "/export:MmAllocateContiguousMemorySpecifyCacheNode=ntoskrnl.MmAllocateContiguousMemorySpecifyCacheNode")
#endif
#ifndef _SeSetAuditParameter
 #pragma comment (linker, "/export:SeSetAuditParameter=ntoskrnl.SeSetAuditParameter")
#endif
#ifndef _wcsncpy_s
 #pragma comment (linker, "/export:wcsncpy_s=ntoskrnl.wcsncpy_s")
#endif
#ifndef _KeGetProcessorIndexFromNumber
 #pragma comment (linker, "/export:KeGetProcessorIndexFromNumber=ntoskrnl.KeGetProcessorIndexFromNumber")
#endif
#ifndef _KeGetProcessorNumberFromIndex
 #pragma comment (linker, "/export:KeGetProcessorNumberFromIndex=ntoskrnl.KeGetProcessorNumberFromIndex")
#endif
#ifndef _KeQueryActiveProcessorCountEx
 #pragma comment (linker, "/export:KeQueryActiveProcessorCountEx=ntoskrnl.KeQueryActiveProcessorCountEx")
#endif
#ifndef _KeGetCurrentProcessorNumberEx
 #pragma comment (linker, "/export:KeGetCurrentProcessorNumberEx=ntoskrnl.KeGetCurrentProcessorNumberEx")
#endif
#ifndef _KeQueryMaximumProcessorCountEx
 #pragma comment (linker, "/export:KeQueryMaximumProcessorCountEx=ntoskrnl.KeQueryMaximumProcessorCountEx")
#endif
#ifndef _KeQueryActiveGroupCount
 #pragma comment (linker, "/export:KeQueryActiveGroupCount=ntoskrnl.KeQueryActiveGroupCount")
#endif
#ifndef _KeQueryMaximumGroupCount
 #pragma comment (linker, "/export:KeQueryMaximumGroupCount=ntoskrnl.KeQueryMaximumGroupCount")
#endif
#ifndef _KeSetSystemGroupAffinityThread
 #pragma comment (linker, "/export:KeSetSystemGroupAffinityThread=ntoskrnl.KeSetSystemGroupAffinityThread")
#endif
#ifndef _KeRevertToUserGroupAffinityThread
 #pragma comment (linker, "/export:KeRevertToUserGroupAffinityThread=ntoskrnl.KeRevertToUserGroupAffinityThread")
#endif
#ifndef _KeSetTargetProcessorDpcEx
 #pragma comment (linker, "/export:KeSetTargetProcessorDpcEx=ntoskrnl.KeSetTargetProcessorDpcEx")
#endif
#ifndef _KeGetCurrentNodeNumber
 #pragma comment (linker, "/export:KeGetCurrentNodeNumber=ntoskrnl.KeGetCurrentNodeNumber")
#endif
#ifndef _KeQueryHighestNodeNumber
 #pragma comment (linker, "/export:KeQueryHighestNodeNumber=ntoskrnl.KeQueryHighestNodeNumber")
#endif
 #ifndef _KeQueryNodeActiveAffinity
 #pragma comment (linker, "/export:KeQueryNodeActiveAffinity=ntoskrnl.KeQueryNodeActiveAffinity")
#endif
#ifndef _KeQueryGroupAffinity
 #pragma comment (linker, "/export:KeQueryGroupAffinity=ntoskrnl.KeQueryGroupAffinity")
#endif
#ifndef _IoGetAffinityInterrupt
 #pragma comment (linker, "/export:IoGetAffinityInterrupt=ntoskrnl.IoGetAffinityInterrupt")
#endif
#ifndef _KeQueryLogicalProcessorRelationship
 #pragma comment (linker, "/export:KeQueryLogicalProcessorRelationship=ntoskrnl.KeQueryLogicalProcessorRelationship")
#endif
#ifndef _ObfReferenceObjectWithTag
 #pragma comment (linker, "/export:ObfReferenceObjectWithTag=ntoskrnl.ObfReferenceObjectWithTag")
#endif
#ifndef _ObfDereferenceObjectWithTag
 #pragma comment (linker, "/export:ObfDereferenceObjectWithTag=ntoskrnl.ObfDereferenceObjectWithTag")
#endif
#ifndef _PoStartDeviceBusy
 #pragma comment (linker, "/export:PoStartDeviceBusy=ntoskrnl.PoStartDeviceBusy")
#endif
#ifndef _PoEndDeviceBusy
 #pragma comment (linker, "/export:PoEndDeviceBusy=ntoskrnl.PoEndDeviceBusy")
#endif
#ifndef _LpcReplyWaitReplyPort
 #pragma comment (linker, "/export:LpcReplyWaitReplyPort=ntoskrnl.LpcReplyWaitReplyPort")
#endif
#ifndef _LpcSendWaitReceivePort
 #pragma comment (linker, "/export:LpcSendWaitReceivePort=ntoskrnl.LpcSendWaitReceivePort")
#endif
#ifndef _SeSetAuthorizationCallbacks
 #pragma comment (linker, "/export:SeSetAuthorizationCallbacks=ntoskrnl.SeSetAuthorizationCallbacks")
#endif
#ifndef _SeReportSecurityEventWithSubCategory
 #pragma comment (linker, "/export:SeReportSecurityEventWithSubCategory=ntoskrnl.SeReportSecurityEventWithSubCategory")
#endif
#ifndef _PcwRegister
 #pragma comment (linker, "/export:PcwRegister=ntoskrnl.PcwRegister")
#endif
#ifndef _PcwUnregister
 #pragma comment (linker, "/export:PcwUnregister=ntoskrnl.PcwUnregister")
#endif
#ifndef _PcwCreateInstance
 #pragma comment (linker, "/export:PcwCreateInstance=ntoskrnl.PcwCreateInstance")
#endif
#ifndef _PcwCloseInstance
 #pragma comment (linker, "/export:PcwCloseInstance=ntoskrnl.PcwCloseInstance")
#endif
#ifndef _PcwAddInstance
 #pragma comment (linker, "/export:PcwAddInstance=ntoskrnl.PcwAddInstance")
#endif
#ifndef _RtlQueryRegistryValuesEx
 #pragma comment (linker, "/export:RtlQueryRegistryValuesEx=ntoskrnl.RtlQueryRegistryValuesEx")
#endif
#ifndef _RtlCheckPortableOperatingSystem
 #pragma comment (linker, "/export:RtlCheckPortableOperatingSystem=ntoskrnl.RtlCheckPortableOperatingSystem")
#endif
#ifndef _RtlSetPortableOperatingSystem
 #pragma comment (linker, "/export:RtlSetPortableOperatingSystem=ntoskrnl.RtlSetPortableOperatingSystem")
#endif
#ifndef _IoSetActivityIdIrp
 #pragma comment (linker, "/export:IoSetActivityIdIrp=ntoskrnl.IoSetActivityIdIrp")
#endif
#ifndef _IoGetActivityIdIrp
 #pragma comment (linker, "/export:IoGetActivityIdIrp=ntoskrnl.IoGetActivityIdIrp")
#endif
#ifndef _MmAllocateContiguousNodeMemory
 #pragma comment (linker, "/export:MmAllocateContiguousNodeMemory=ntoskrnl.MmAllocateContiguousNodeMemory")
#endif



/////////////////////////////////////////
///        default redirect            //
/////////////////////////////////////////

/////////////////////////////////////////
//           Windows 2000              //
#if (NTDDI_VERSION >= NTDDI_NTDDI_WIN2K)
//TODO
#endif


/////////////////////////////////////////
//          Windows XP RTM             //
#if (NTDDI_VERSION >= NTDDI_WINXP)
#pragma comment (linker, "/export:__abnormal_termination=ntoskrnl._abnormal_termination")
#pragma comment (linker, "/export:__alldiv=ntoskrnl._alldiv")
#pragma comment (linker, "/export:__alldvrm=ntoskrnl._alldvrm")
#pragma comment (linker, "/export:__allmul=ntoskrnl._allmul")
#pragma comment (linker, "/export:__alloca_probe=ntoskrnl._alloca_probe")
#pragma comment (linker, "/export:__allrem=ntoskrnl._allrem")
#pragma comment (linker, "/export:__allshl=ntoskrnl._allshl")
#pragma comment (linker, "/export:__allshr=ntoskrnl._allshr")
#pragma comment (linker, "/export:__aulldiv=ntoskrnl._aulldiv")
#pragma comment (linker, "/export:__aulldvrm=ntoskrnl._aulldvrm")
#pragma comment (linker, "/export:__aullrem=ntoskrnl._aullrem")
#pragma comment (linker, "/export:__aullshr=ntoskrnl._aullshr")
#pragma comment (linker, "/export:__CIcos=ntoskrnl._CIcos")
#pragma comment (linker, "/export:__CIsin=ntoskrnl._CIsin")
#pragma comment (linker, "/export:__CIsqrt=ntoskrnl._CIsqrt")
#pragma comment (linker, "/export:__except_handler2=ntoskrnl._except_handler2")
#pragma comment (linker, "/export:__except_handler3=ntoskrnl._except_handler3")
#pragma comment (linker, "/export:__global_unwind2=ntoskrnl._global_unwind2")
#pragma comment (linker, "/export:__itoa=ntoskrnl._itoa")
#pragma comment (linker, "/export:__itow=ntoskrnl._itow")
#pragma comment (linker, "/export:__local_unwind2=ntoskrnl._local_unwind2")
#pragma comment (linker, "/export:__purecall=ntoskrnl._purecall")
#pragma comment (linker, "/export:__snprintf=ntoskrnl._snprintf")
#pragma comment (linker, "/export:__snwprintf=ntoskrnl._snwprintf")
#pragma comment (linker, "/export:__stricmp=ntoskrnl._stricmp")
#pragma comment (linker, "/export:__strlwr=ntoskrnl._strlwr")
#pragma comment (linker, "/export:__strnicmp=ntoskrnl._strnicmp")
#pragma comment (linker, "/export:__strnset=ntoskrnl._strnset")
#pragma comment (linker, "/export:__strrev=ntoskrnl._strrev")
#pragma comment (linker, "/export:__strset=ntoskrnl._strset")
#pragma comment (linker, "/export:__strupr=ntoskrnl._strupr")
#pragma comment (linker, "/export:__vsnprintf=ntoskrnl._vsnprintf")
#pragma comment (linker, "/export:__vsnwprintf=ntoskrnl._vsnwprintf")
#pragma comment (linker, "/export:__wcsicmp=ntoskrnl._wcsicmp")
#pragma comment (linker, "/export:__wcslwr=ntoskrnl._wcslwr")
#pragma comment (linker, "/export:__wcsnicmp=ntoskrnl._wcsnicmp")
#pragma comment (linker, "/export:__wcsnset=ntoskrnl._wcsnset")
#pragma comment (linker, "/export:__wcsrev=ntoskrnl._wcsrev")
#pragma comment (linker, "/export:__wcsupr=ntoskrnl._wcsupr")
#pragma comment (linker, "/export:atoi=ntoskrnl.atoi")
#pragma comment (linker, "/export:atol=ntoskrnl.atol")
#pragma comment (linker, "/export:CcCanIWrite=ntoskrnl.CcCanIWrite")
#pragma comment (linker, "/export:CcCopyRead=ntoskrnl.CcCopyRead")
#pragma comment (linker, "/export:CcCopyWrite=ntoskrnl.CcCopyWrite")
#pragma comment (linker, "/export:CcDeferWrite=ntoskrnl.CcDeferWrite")
#pragma comment (linker, "/export:CcFastCopyRead=ntoskrnl.CcFastCopyRead")
#pragma comment (linker, "/export:CcFastCopyWrite=ntoskrnl.CcFastCopyWrite")
#pragma comment (linker, "/export:CcFastMdlReadWait=ntoskrnl.CcFastMdlReadWait")
#pragma comment (linker, "/export:CcFastReadNotPossible=ntoskrnl.CcFastReadNotPossible")
#pragma comment (linker, "/export:CcFastReadWait=ntoskrnl.CcFastReadWait")
#pragma comment (linker, "/export:CcFlushCache=ntoskrnl.CcFlushCache")
#pragma comment (linker, "/export:CcGetDirtyPages=ntoskrnl.CcGetDirtyPages")
#pragma comment (linker, "/export:CcGetFileObjectFromBcb=ntoskrnl.CcGetFileObjectFromBcb")
#pragma comment (linker, "/export:CcGetFileObjectFromSectionPtrs=ntoskrnl.CcGetFileObjectFromSectionPtrs")
#pragma comment (linker, "/export:CcGetFlushedValidData=ntoskrnl.CcGetFlushedValidData")
#pragma comment (linker, "/export:CcGetLsnForFileObject=ntoskrnl.CcGetLsnForFileObject")
#pragma comment (linker, "/export:CcInitializeCacheMap=ntoskrnl.CcInitializeCacheMap")
#pragma comment (linker, "/export:CcIsThereDirtyData=ntoskrnl.CcIsThereDirtyData")
#pragma comment (linker, "/export:CcMapData=ntoskrnl.CcMapData")
#pragma comment (linker, "/export:CcMdlRead=ntoskrnl.CcMdlRead")
#pragma comment (linker, "/export:CcMdlReadComplete=ntoskrnl.CcMdlReadComplete")
#pragma comment (linker, "/export:CcMdlWriteAbort=ntoskrnl.CcMdlWriteAbort")
#pragma comment (linker, "/export:CcMdlWriteComplete=ntoskrnl.CcMdlWriteComplete")
#pragma comment (linker, "/export:CcPinMappedData=ntoskrnl.CcPinMappedData")
#pragma comment (linker, "/export:CcPinRead=ntoskrnl.CcPinRead")
#pragma comment (linker, "/export:CcPrepareMdlWrite=ntoskrnl.CcPrepareMdlWrite")
#pragma comment (linker, "/export:CcPreparePinWrite=ntoskrnl.CcPreparePinWrite")
#pragma comment (linker, "/export:CcPurgeCacheSection=ntoskrnl.CcPurgeCacheSection")
#pragma comment (linker, "/export:CcRemapBcb=ntoskrnl.CcRemapBcb")
#pragma comment (linker, "/export:CcRepinBcb=ntoskrnl.CcRepinBcb")
#pragma comment (linker, "/export:CcScheduleReadAhead=ntoskrnl.CcScheduleReadAhead")
#pragma comment (linker, "/export:CcSetAdditionalCacheAttributes=ntoskrnl.CcSetAdditionalCacheAttributes")
#pragma comment (linker, "/export:CcSetBcbOwnerPointer=ntoskrnl.CcSetBcbOwnerPointer")
#pragma comment (linker, "/export:CcSetDirtyPageThreshold=ntoskrnl.CcSetDirtyPageThreshold")
#pragma comment (linker, "/export:CcSetDirtyPinnedData=ntoskrnl.CcSetDirtyPinnedData")
#pragma comment (linker, "/export:CcSetFileSizes=ntoskrnl.CcSetFileSizes")
#pragma comment (linker, "/export:CcSetLogHandleForFile=ntoskrnl.CcSetLogHandleForFile")
#pragma comment (linker, "/export:CcSetReadAheadGranularity=ntoskrnl.CcSetReadAheadGranularity")
#pragma comment (linker, "/export:CcUninitializeCacheMap=ntoskrnl.CcUninitializeCacheMap")
#pragma comment (linker, "/export:CcUnpinData=ntoskrnl.CcUnpinData")
#pragma comment (linker, "/export:CcUnpinDataForThread=ntoskrnl.CcUnpinDataForThread")
#pragma comment (linker, "/export:CcUnpinRepinnedBcb=ntoskrnl.CcUnpinRepinnedBcb")
#pragma comment (linker, "/export:CcWaitForCurrentLazyWriterActivity=ntoskrnl.CcWaitForCurrentLazyWriterActivity")
#pragma comment (linker, "/export:CcZeroData=ntoskrnl.CcZeroData")
#pragma comment (linker, "/export:CmRegisterCallback=ntoskrnl.CmRegisterCallback")
#pragma comment (linker, "/export:CmUnRegisterCallback=ntoskrnl.CmUnRegisterCallback")
#pragma comment (linker, "/export:DbgBreakPoint=ntoskrnl.DbgBreakPoint")
#pragma comment (linker, "/export:DbgBreakPointWithStatus=ntoskrnl.DbgBreakPointWithStatus")
#pragma comment (linker, "/export:DbgLoadImageSymbols=ntoskrnl.DbgLoadImageSymbols")
#pragma comment (linker, "/export:DbgPrint=ntoskrnl.DbgPrint")
#pragma comment (linker, "/export:DbgPrintEx=ntoskrnl.DbgPrintEx")
#pragma comment (linker, "/export:DbgPrintReturnControlC=ntoskrnl.DbgPrintReturnControlC")
#pragma comment (linker, "/export:DbgPrompt=ntoskrnl.DbgPrompt")
#pragma comment (linker, "/export:DbgQueryDebugFilterState=ntoskrnl.DbgQueryDebugFilterState")
#pragma comment (linker, "/export:DbgSetDebugFilterState=ntoskrnl.DbgSetDebugFilterState")
#pragma comment (linker, "/export:ExAcquireFastMutexUnsafe=ntoskrnl.ExAcquireFastMutexUnsafe")
#pragma comment (linker, "/export:ExAcquireResourceExclusiveLite=ntoskrnl.ExAcquireResourceExclusiveLite")
#pragma comment (linker, "/export:ExAcquireResourceSharedLite=ntoskrnl.ExAcquireResourceSharedLite")
#pragma comment (linker, "/export:ExAcquireRundownProtection=ntoskrnl.ExAcquireRundownProtection")
#pragma comment (linker, "/export:ExAcquireSharedStarveExclusive=ntoskrnl.ExAcquireSharedStarveExclusive")
#pragma comment (linker, "/export:ExAcquireSharedWaitForExclusive=ntoskrnl.ExAcquireSharedWaitForExclusive")
#pragma comment (linker, "/export:ExAllocateFromPagedLookasideList=ntoskrnl.ExAllocateFromPagedLookasideList")
#pragma comment (linker, "/export:ExAllocatePool=ntoskrnl.ExAllocatePool")
#pragma comment (linker, "/export:ExAllocatePoolWithQuota=ntoskrnl.ExAllocatePoolWithQuota")
#pragma comment (linker, "/export:ExAllocatePoolWithQuotaTag=ntoskrnl.ExAllocatePoolWithQuotaTag")
#pragma comment (linker, "/export:ExAllocatePoolWithTag=ntoskrnl.ExAllocatePoolWithTag")
#pragma comment (linker, "/export:ExAllocatePoolWithTagPriority=ntoskrnl.ExAllocatePoolWithTagPriority")
#pragma comment (linker, "/export:ExConvertExclusiveToSharedLite=ntoskrnl.ExConvertExclusiveToSharedLite")
#pragma comment (linker, "/export:ExCreateCallback=ntoskrnl.ExCreateCallback")
#pragma comment (linker, "/export:ExDeleteNPagedLookasideList=ntoskrnl.ExDeleteNPagedLookasideList")
#pragma comment (linker, "/export:ExDeletePagedLookasideList=ntoskrnl.ExDeletePagedLookasideList")
#pragma comment (linker, "/export:ExDeleteResourceLite=ntoskrnl.ExDeleteResourceLite")
#pragma comment (linker, "/export:ExDesktopObjectType=ntoskrnl.ExDesktopObjectType")
#pragma comment (linker, "/export:ExDisableResourceBoostLite=ntoskrnl.ExDisableResourceBoostLite")
#pragma comment (linker, "/export:ExEnumHandleTable=ntoskrnl.ExEnumHandleTable")
#pragma comment (linker, "/export:ExEventObjectType=ntoskrnl.ExEventObjectType")
#pragma comment (linker, "/export:ExExtendZone=ntoskrnl.ExExtendZone")
#pragma comment (linker, "/export:Exfi386InterlockedDecrementLong=ntoskrnl.Exfi386InterlockedDecrementLong")
#pragma comment (linker, "/export:Exfi386InterlockedExchangeUlong=ntoskrnl.Exfi386InterlockedExchangeUlong")
#pragma comment (linker, "/export:Exfi386InterlockedIncrementLong=ntoskrnl.Exfi386InterlockedIncrementLong")
#pragma comment (linker, "/export:ExfInterlockedAddUlong=ntoskrnl.ExfInterlockedAddUlong")
#pragma comment (linker, "/export:ExfInterlockedCompareExchange64=ntoskrnl.ExfInterlockedCompareExchange64")
#pragma comment (linker, "/export:ExfInterlockedInsertHeadList=ntoskrnl.ExfInterlockedInsertHeadList")
#pragma comment (linker, "/export:ExfInterlockedInsertTailList=ntoskrnl.ExfInterlockedInsertTailList")
#pragma comment (linker, "/export:ExfInterlockedPopEntryList=ntoskrnl.ExfInterlockedPopEntryList")
#pragma comment (linker, "/export:ExfInterlockedPushEntryList=ntoskrnl.ExfInterlockedPushEntryList")
#pragma comment (linker, "/export:ExfInterlockedRemoveHeadList=ntoskrnl.ExfInterlockedRemoveHeadList")
#pragma comment (linker, "/export:ExFreePool=ntoskrnl.ExFreePool")
#pragma comment (linker, "/export:ExFreePoolWithTag=ntoskrnl.ExFreePoolWithTag")
#pragma comment (linker, "/export:ExFreeToPagedLookasideList=ntoskrnl.ExFreeToPagedLookasideList")
#pragma comment (linker, "/export:ExGetCurrentProcessorCounts=ntoskrnl.ExGetCurrentProcessorCounts")
#pragma comment (linker, "/export:ExGetCurrentProcessorCpuUsage=ntoskrnl.ExGetCurrentProcessorCpuUsage")
#pragma comment (linker, "/export:ExGetExclusiveWaiterCount=ntoskrnl.ExGetExclusiveWaiterCount")
#pragma comment (linker, "/export:ExGetPreviousMode=ntoskrnl.ExGetPreviousMode")
#pragma comment (linker, "/export:ExGetSharedWaiterCount=ntoskrnl.ExGetSharedWaiterCount")
#pragma comment (linker, "/export:Exi386InterlockedDecrementLong=ntoskrnl.Exi386InterlockedDecrementLong")
#pragma comment (linker, "/export:Exi386InterlockedExchangeUlong=ntoskrnl.Exi386InterlockedExchangeUlong")
#pragma comment (linker, "/export:Exi386InterlockedIncrementLong=ntoskrnl.Exi386InterlockedIncrementLong")
#pragma comment (linker, "/export:ExInitializeNPagedLookasideList=ntoskrnl.ExInitializeNPagedLookasideList")
#pragma comment (linker, "/export:ExInitializePagedLookasideList=ntoskrnl.ExInitializePagedLookasideList")
#pragma comment (linker, "/export:ExInitializeResourceLite=ntoskrnl.ExInitializeResourceLite")
#pragma comment (linker, "/export:ExInitializeRundownProtection=ntoskrnl.ExInitializeRundownProtection")
#pragma comment (linker, "/export:ExInitializeZone=ntoskrnl.ExInitializeZone")
#pragma comment (linker, "/export:ExInterlockedAddLargeInteger=ntoskrnl.ExInterlockedAddLargeInteger")
#pragma comment (linker, "/export:ExInterlockedAddLargeStatistic=ntoskrnl.ExInterlockedAddLargeStatistic")
#pragma comment (linker, "/export:ExInterlockedAddUlong=ntoskrnl.ExInterlockedAddUlong")
#pragma comment (linker, "/export:ExInterlockedCompareExchange64=ntoskrnl.ExInterlockedCompareExchange64")
#pragma comment (linker, "/export:ExInterlockedDecrementLong=ntoskrnl.ExInterlockedDecrementLong")
#pragma comment (linker, "/export:ExInterlockedExchangeUlong=ntoskrnl.ExInterlockedExchangeUlong")
#pragma comment (linker, "/export:ExInterlockedExtendZone=ntoskrnl.ExInterlockedExtendZone")
#pragma comment (linker, "/export:ExInterlockedFlushSList=ntoskrnl.ExInterlockedFlushSList")
#pragma comment (linker, "/export:ExInterlockedIncrementLong=ntoskrnl.ExInterlockedIncrementLong")
#pragma comment (linker, "/export:ExInterlockedInsertHeadList=ntoskrnl.ExInterlockedInsertHeadList")
#pragma comment (linker, "/export:ExInterlockedInsertTailList=ntoskrnl.ExInterlockedInsertTailList")
#pragma comment (linker, "/export:ExInterlockedPopEntryList=ntoskrnl.ExInterlockedPopEntryList")
#pragma comment (linker, "/export:ExInterlockedPopEntrySList=ntoskrnl.ExInterlockedPopEntrySList")
#pragma comment (linker, "/export:ExInterlockedPushEntryList=ntoskrnl.ExInterlockedPushEntryList")
#pragma comment (linker, "/export:ExInterlockedPushEntrySList=ntoskrnl.ExInterlockedPushEntrySList")
#pragma comment (linker, "/export:ExInterlockedRemoveHeadList=ntoskrnl.ExInterlockedRemoveHeadList")
#pragma comment (linker, "/export:ExIsProcessorFeaturePresent=ntoskrnl.ExIsProcessorFeaturePresent")
#pragma comment (linker, "/export:ExIsResourceAcquiredExclusiveLite=ntoskrnl.ExIsResourceAcquiredExclusiveLite")
#pragma comment (linker, "/export:ExIsResourceAcquiredSharedLite=ntoskrnl.ExIsResourceAcquiredSharedLite")
#pragma comment (linker, "/export:ExLocalTimeToSystemTime=ntoskrnl.ExLocalTimeToSystemTime")
#pragma comment (linker, "/export:ExNotifyCallback=ntoskrnl.ExNotifyCallback")
#pragma comment (linker, "/export:ExQueryPoolBlockSize=ntoskrnl.ExQueryPoolBlockSize")
#pragma comment (linker, "/export:ExQueueWorkItem=ntoskrnl.ExQueueWorkItem")
#pragma comment (linker, "/export:ExRaiseAccessViolation=ntoskrnl.ExRaiseAccessViolation")
#pragma comment (linker, "/export:ExRaiseDatatypeMisalignment=ntoskrnl.ExRaiseDatatypeMisalignment")
#pragma comment (linker, "/export:ExRaiseException=ntoskrnl.ExRaiseException")
#pragma comment (linker, "/export:ExRaiseHardError=ntoskrnl.ExRaiseHardError")
#pragma comment (linker, "/export:ExRaiseStatus=ntoskrnl.ExRaiseStatus")
#pragma comment (linker, "/export:ExRegisterCallback=ntoskrnl.ExRegisterCallback")
#pragma comment (linker, "/export:ExReinitializeResourceLite=ntoskrnl.ExReinitializeResourceLite")
#pragma comment (linker, "/export:ExReInitializeRundownProtection=ntoskrnl.ExReInitializeRundownProtection")
#pragma comment (linker, "/export:ExReleaseFastMutexUnsafe=ntoskrnl.ExReleaseFastMutexUnsafe")
#pragma comment (linker, "/export:ExReleaseResourceForThreadLite=ntoskrnl.ExReleaseResourceForThreadLite")
#pragma comment (linker, "/export:ExReleaseResourceLite=ntoskrnl.ExReleaseResourceLite")
#pragma comment (linker, "/export:ExReleaseRundownProtection=ntoskrnl.ExReleaseRundownProtection")
#pragma comment (linker, "/export:ExRundownCompleted=ntoskrnl.ExRundownCompleted")
#pragma comment (linker, "/export:ExSemaphoreObjectType=ntoskrnl.ExSemaphoreObjectType")
#pragma comment (linker, "/export:ExSetResourceOwnerPointer=ntoskrnl.ExSetResourceOwnerPointer")
#pragma comment (linker, "/export:ExSetTimerResolution=ntoskrnl.ExSetTimerResolution")
#pragma comment (linker, "/export:ExSystemExceptionFilter=ntoskrnl.ExSystemExceptionFilter")
#pragma comment (linker, "/export:ExSystemTimeToLocalTime=ntoskrnl.ExSystemTimeToLocalTime")
#pragma comment (linker, "/export:ExUnregisterCallback=ntoskrnl.ExUnregisterCallback")
#pragma comment (linker, "/export:ExUuidCreate=ntoskrnl.ExUuidCreate")
#pragma comment (linker, "/export:ExVerifySuite=ntoskrnl.ExVerifySuite")
#pragma comment (linker, "/export:ExWaitForRundownProtectionRelease=ntoskrnl.ExWaitForRundownProtectionRelease")
#pragma comment (linker, "/export:ExWindowStationObjectType=ntoskrnl.ExWindowStationObjectType")
#pragma comment (linker, "/export:FsRtlAcquireFileExclusive=ntoskrnl.FsRtlAcquireFileExclusive")
#pragma comment (linker, "/export:FsRtlAddLargeMcbEntry=ntoskrnl.FsRtlAddLargeMcbEntry")
#pragma comment (linker, "/export:FsRtlAddMcbEntry=ntoskrnl.FsRtlAddMcbEntry")
#pragma comment (linker, "/export:FsRtlAddToTunnelCache=ntoskrnl.FsRtlAddToTunnelCache")
#pragma comment (linker, "/export:FsRtlAllocateFileLock=ntoskrnl.FsRtlAllocateFileLock")
#pragma comment (linker, "/export:FsRtlAllocatePool=ntoskrnl.FsRtlAllocatePool")
#pragma comment (linker, "/export:FsRtlAllocatePoolWithQuota=ntoskrnl.FsRtlAllocatePoolWithQuota")
#pragma comment (linker, "/export:FsRtlAllocatePoolWithQuotaTag=ntoskrnl.FsRtlAllocatePoolWithQuotaTag")
#pragma comment (linker, "/export:FsRtlAllocatePoolWithTag=ntoskrnl.FsRtlAllocatePoolWithTag")
#pragma comment (linker, "/export:FsRtlAllocateResource=ntoskrnl.FsRtlAllocateResource")
#pragma comment (linker, "/export:FsRtlAreNamesEqual=ntoskrnl.FsRtlAreNamesEqual")
#pragma comment (linker, "/export:FsRtlBalanceReads=ntoskrnl.FsRtlBalanceReads")
#pragma comment (linker, "/export:FsRtlCheckLockForReadAccess=ntoskrnl.FsRtlCheckLockForReadAccess")
#pragma comment (linker, "/export:FsRtlCheckLockForWriteAccess=ntoskrnl.FsRtlCheckLockForWriteAccess")
#pragma comment (linker, "/export:FsRtlCheckOplock=ntoskrnl.FsRtlCheckOplock")
#pragma comment (linker, "/export:FsRtlCopyRead=ntoskrnl.FsRtlCopyRead")
#pragma comment (linker, "/export:FsRtlCopyWrite=ntoskrnl.FsRtlCopyWrite")
#pragma comment (linker, "/export:FsRtlCurrentBatchOplock=ntoskrnl.FsRtlCurrentBatchOplock")
#pragma comment (linker, "/export:FsRtlDeleteKeyFromTunnelCache=ntoskrnl.FsRtlDeleteKeyFromTunnelCache")
#pragma comment (linker, "/export:FsRtlDeleteTunnelCache=ntoskrnl.FsRtlDeleteTunnelCache")
#pragma comment (linker, "/export:FsRtlDeregisterUncProvider=ntoskrnl.FsRtlDeregisterUncProvider")
#pragma comment (linker, "/export:FsRtlDissectDbcs=ntoskrnl.FsRtlDissectDbcs")
#pragma comment (linker, "/export:FsRtlDissectName=ntoskrnl.FsRtlDissectName")
#pragma comment (linker, "/export:FsRtlDoesDbcsContainWildCards=ntoskrnl.FsRtlDoesDbcsContainWildCards")
#pragma comment (linker, "/export:FsRtlDoesNameContainWildCards=ntoskrnl.FsRtlDoesNameContainWildCards")
#pragma comment (linker, "/export:FsRtlFastCheckLockForRead=ntoskrnl.FsRtlFastCheckLockForRead")
#pragma comment (linker, "/export:FsRtlFastCheckLockForWrite=ntoskrnl.FsRtlFastCheckLockForWrite")
#pragma comment (linker, "/export:FsRtlFastUnlockAll=ntoskrnl.FsRtlFastUnlockAll")
#pragma comment (linker, "/export:FsRtlFastUnlockAllByKey=ntoskrnl.FsRtlFastUnlockAllByKey")
#pragma comment (linker, "/export:FsRtlFastUnlockSingle=ntoskrnl.FsRtlFastUnlockSingle")
#pragma comment (linker, "/export:FsRtlFindInTunnelCache=ntoskrnl.FsRtlFindInTunnelCache")
#pragma comment (linker, "/export:FsRtlFreeFileLock=ntoskrnl.FsRtlFreeFileLock")
#pragma comment (linker, "/export:FsRtlGetFileSize=ntoskrnl.FsRtlGetFileSize")
#pragma comment (linker, "/export:FsRtlGetNextFileLock=ntoskrnl.FsRtlGetNextFileLock")
#pragma comment (linker, "/export:FsRtlGetNextLargeMcbEntry=ntoskrnl.FsRtlGetNextLargeMcbEntry")
#pragma comment (linker, "/export:FsRtlGetNextMcbEntry=ntoskrnl.FsRtlGetNextMcbEntry")
#pragma comment (linker, "/export:FsRtlIncrementCcFastReadNotPossible=ntoskrnl.FsRtlIncrementCcFastReadNotPossible")
#pragma comment (linker, "/export:FsRtlIncrementCcFastReadNoWait=ntoskrnl.FsRtlIncrementCcFastReadNoWait")
#pragma comment (linker, "/export:FsRtlIncrementCcFastReadResourceMiss=ntoskrnl.FsRtlIncrementCcFastReadResourceMiss")
#pragma comment (linker, "/export:FsRtlIncrementCcFastReadWait=ntoskrnl.FsRtlIncrementCcFastReadWait")
#pragma comment (linker, "/export:FsRtlInitializeFileLock=ntoskrnl.FsRtlInitializeFileLock")
#pragma comment (linker, "/export:FsRtlInitializeLargeMcb=ntoskrnl.FsRtlInitializeLargeMcb")
#pragma comment (linker, "/export:FsRtlInitializeMcb=ntoskrnl.FsRtlInitializeMcb")
#pragma comment (linker, "/export:FsRtlInitializeOplock=ntoskrnl.FsRtlInitializeOplock")
#pragma comment (linker, "/export:FsRtlInitializeTunnelCache=ntoskrnl.FsRtlInitializeTunnelCache")
#pragma comment (linker, "/export:FsRtlInsertPerFileObjectContext=ntoskrnl.FsRtlInsertPerFileObjectContext")
#pragma comment (linker, "/export:FsRtlInsertPerStreamContext=ntoskrnl.FsRtlInsertPerStreamContext")
#pragma comment (linker, "/export:FsRtlIsDbcsInExpression=ntoskrnl.FsRtlIsDbcsInExpression")
#pragma comment (linker, "/export:FsRtlIsFatDbcsLegal=ntoskrnl.FsRtlIsFatDbcsLegal")
#pragma comment (linker, "/export:FsRtlIsHpfsDbcsLegal=ntoskrnl.FsRtlIsHpfsDbcsLegal")
#pragma comment (linker, "/export:FsRtlIsNameInExpression=ntoskrnl.FsRtlIsNameInExpression")
#pragma comment (linker, "/export:FsRtlIsNtstatusExpected=ntoskrnl.FsRtlIsNtstatusExpected")
#pragma comment (linker, "/export:FsRtlIsPagingFile=ntoskrnl.FsRtlIsPagingFile")
#pragma comment (linker, "/export:FsRtlIsTotalDeviceFailure=ntoskrnl.FsRtlIsTotalDeviceFailure")
#pragma comment (linker, "/export:FsRtlLegalAnsiCharacterArray=ntoskrnl.FsRtlLegalAnsiCharacterArray")
#pragma comment (linker, "/export:FsRtlLookupLargeMcbEntry=ntoskrnl.FsRtlLookupLargeMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupLastLargeMcbEntry=ntoskrnl.FsRtlLookupLastLargeMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupLastLargeMcbEntryAndIndex=ntoskrnl.FsRtlLookupLastLargeMcbEntryAndIndex")
#pragma comment (linker, "/export:FsRtlLookupLastMcbEntry=ntoskrnl.FsRtlLookupLastMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupMcbEntry=ntoskrnl.FsRtlLookupMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupPerFileObjectContext=ntoskrnl.FsRtlLookupPerFileObjectContext")
#pragma comment (linker, "/export:FsRtlLookupPerStreamContextInternal=ntoskrnl.FsRtlLookupPerStreamContextInternal")
#pragma comment (linker, "/export:FsRtlMdlRead=ntoskrnl.FsRtlMdlRead")
#pragma comment (linker, "/export:FsRtlMdlReadComplete=ntoskrnl.FsRtlMdlReadComplete")
#pragma comment (linker, "/export:FsRtlMdlReadCompleteDev=ntoskrnl.FsRtlMdlReadCompleteDev")
#pragma comment (linker, "/export:FsRtlMdlReadDev=ntoskrnl.FsRtlMdlReadDev")
#pragma comment (linker, "/export:FsRtlMdlWriteComplete=ntoskrnl.FsRtlMdlWriteComplete")
#pragma comment (linker, "/export:FsRtlMdlWriteCompleteDev=ntoskrnl.FsRtlMdlWriteCompleteDev")
#pragma comment (linker, "/export:FsRtlNormalizeNtstatus=ntoskrnl.FsRtlNormalizeNtstatus")
#pragma comment (linker, "/export:FsRtlNotifyChangeDirectory=ntoskrnl.FsRtlNotifyChangeDirectory")
#pragma comment (linker, "/export:FsRtlNotifyCleanup=ntoskrnl.FsRtlNotifyCleanup")
#pragma comment (linker, "/export:FsRtlNotifyFilterChangeDirectory=ntoskrnl.FsRtlNotifyFilterChangeDirectory")
#pragma comment (linker, "/export:FsRtlNotifyFilterReportChange=ntoskrnl.FsRtlNotifyFilterReportChange")
#pragma comment (linker, "/export:FsRtlNotifyFullChangeDirectory=ntoskrnl.FsRtlNotifyFullChangeDirectory")
#pragma comment (linker, "/export:FsRtlNotifyFullReportChange=ntoskrnl.FsRtlNotifyFullReportChange")
#pragma comment (linker, "/export:FsRtlNotifyInitializeSync=ntoskrnl.FsRtlNotifyInitializeSync")
#pragma comment (linker, "/export:FsRtlNotifyReportChange=ntoskrnl.FsRtlNotifyReportChange")
#pragma comment (linker, "/export:FsRtlNotifyUninitializeSync=ntoskrnl.FsRtlNotifyUninitializeSync")
#pragma comment (linker, "/export:FsRtlNotifyVolumeEvent=ntoskrnl.FsRtlNotifyVolumeEvent")
#pragma comment (linker, "/export:FsRtlNumberOfRunsInLargeMcb=ntoskrnl.FsRtlNumberOfRunsInLargeMcb")
#pragma comment (linker, "/export:FsRtlNumberOfRunsInMcb=ntoskrnl.FsRtlNumberOfRunsInMcb")
#pragma comment (linker, "/export:FsRtlOplockFsctrl=ntoskrnl.FsRtlOplockFsctrl")
#pragma comment (linker, "/export:FsRtlOplockIsFastIoPossible=ntoskrnl.FsRtlOplockIsFastIoPossible")
#pragma comment (linker, "/export:FsRtlPostPagingFileStackOverflow=ntoskrnl.FsRtlPostPagingFileStackOverflow")
#pragma comment (linker, "/export:FsRtlPostStackOverflow=ntoskrnl.FsRtlPostStackOverflow")
#pragma comment (linker, "/export:FsRtlPrepareMdlWrite=ntoskrnl.FsRtlPrepareMdlWrite")
#pragma comment (linker, "/export:FsRtlPrepareMdlWriteDev=ntoskrnl.FsRtlPrepareMdlWriteDev")
#pragma comment (linker, "/export:FsRtlPrivateLock=ntoskrnl.FsRtlPrivateLock")
#pragma comment (linker, "/export:FsRtlProcessFileLock=ntoskrnl.FsRtlProcessFileLock")
#pragma comment (linker, "/export:FsRtlRegisterFileSystemFilterCallbacks=ntoskrnl.FsRtlRegisterFileSystemFilterCallbacks")
#pragma comment (linker, "/export:FsRtlRegisterUncProvider=ntoskrnl.FsRtlRegisterUncProvider")
#pragma comment (linker, "/export:FsRtlReleaseFile=ntoskrnl.FsRtlReleaseFile")
#pragma comment (linker, "/export:FsRtlRemoveLargeMcbEntry=ntoskrnl.FsRtlRemoveLargeMcbEntry")
#pragma comment (linker, "/export:FsRtlRemoveMcbEntry=ntoskrnl.FsRtlRemoveMcbEntry")
#pragma comment (linker, "/export:FsRtlRemovePerFileObjectContext=ntoskrnl.FsRtlRemovePerFileObjectContext")
#pragma comment (linker, "/export:FsRtlRemovePerStreamContext=ntoskrnl.FsRtlRemovePerStreamContext")
#pragma comment (linker, "/export:FsRtlResetLargeMcb=ntoskrnl.FsRtlResetLargeMcb")
#pragma comment (linker, "/export:FsRtlSplitLargeMcb=ntoskrnl.FsRtlSplitLargeMcb")
#pragma comment (linker, "/export:FsRtlSyncVolumes=ntoskrnl.FsRtlSyncVolumes")
#pragma comment (linker, "/export:FsRtlTeardownPerStreamContexts=ntoskrnl.FsRtlTeardownPerStreamContexts")
#pragma comment (linker, "/export:FsRtlTruncateLargeMcb=ntoskrnl.FsRtlTruncateLargeMcb")
#pragma comment (linker, "/export:FsRtlTruncateMcb=ntoskrnl.FsRtlTruncateMcb")
#pragma comment (linker, "/export:FsRtlUninitializeFileLock=ntoskrnl.FsRtlUninitializeFileLock")
#pragma comment (linker, "/export:FsRtlUninitializeLargeMcb=ntoskrnl.FsRtlUninitializeLargeMcb")
#pragma comment (linker, "/export:FsRtlUninitializeMcb=ntoskrnl.FsRtlUninitializeMcb")
#pragma comment (linker, "/export:FsRtlUninitializeOplock=ntoskrnl.FsRtlUninitializeOplock")
#pragma comment (linker, "/export:HalDispatchTable=ntoskrnl.HalDispatchTable")
#pragma comment (linker, "/export:HalExamineMBR=ntoskrnl.HalExamineMBR")
#pragma comment (linker, "/export:HalPrivateDispatchTable=ntoskrnl.HalPrivateDispatchTable")
#pragma comment (linker, "/export:HeadlessDispatch=ntoskrnl.HeadlessDispatch")
#pragma comment (linker, "/export:InbvAcquireDisplayOwnership=ntoskrnl.InbvAcquireDisplayOwnership")
#pragma comment (linker, "/export:InbvCheckDisplayOwnership=ntoskrnl.InbvCheckDisplayOwnership")
#pragma comment (linker, "/export:InbvDisplayString=ntoskrnl.InbvDisplayString")
#pragma comment (linker, "/export:InbvEnableBootDriver=ntoskrnl.InbvEnableBootDriver")
#pragma comment (linker, "/export:InbvEnableDisplayString=ntoskrnl.InbvEnableDisplayString")
#pragma comment (linker, "/export:InbvInstallDisplayStringFilter=ntoskrnl.InbvInstallDisplayStringFilter")
#pragma comment (linker, "/export:InbvIsBootDriverInstalled=ntoskrnl.InbvIsBootDriverInstalled")
#pragma comment (linker, "/export:InbvNotifyDisplayOwnershipLost=ntoskrnl.InbvNotifyDisplayOwnershipLost")
#pragma comment (linker, "/export:InbvResetDisplay=ntoskrnl.InbvResetDisplay")
#pragma comment (linker, "/export:InbvSetScrollRegion=ntoskrnl.InbvSetScrollRegion")
#pragma comment (linker, "/export:InbvSetTextColor=ntoskrnl.InbvSetTextColor")
#pragma comment (linker, "/export:InbvSolidColorFill=ntoskrnl.InbvSolidColorFill")
#pragma comment (linker, "/export:InitSafeBootMode=ntoskrnl.InitSafeBootMode")
#pragma comment (linker, "/export:InterlockedCompareExchange=ntoskrnl.InterlockedCompareExchange")
#pragma comment (linker, "/export:InterlockedDecrement=ntoskrnl.InterlockedDecrement")
#pragma comment (linker, "/export:InterlockedExchange=ntoskrnl.InterlockedExchange")
#pragma comment (linker, "/export:InterlockedExchangeAdd=ntoskrnl.InterlockedExchangeAdd")
#pragma comment (linker, "/export:InterlockedIncrement=ntoskrnl.InterlockedIncrement")
#pragma comment (linker, "/export:InterlockedPopEntrySList=ntoskrnl.InterlockedPopEntrySList")
#pragma comment (linker, "/export:InterlockedPushEntrySList=ntoskrnl.InterlockedPushEntrySList")
#pragma comment (linker, "/export:IoAcquireCancelSpinLock=ntoskrnl.IoAcquireCancelSpinLock")
#pragma comment (linker, "/export:IoAcquireRemoveLockEx=ntoskrnl.IoAcquireRemoveLockEx")
#pragma comment (linker, "/export:IoAcquireVpbSpinLock=ntoskrnl.IoAcquireVpbSpinLock")
#pragma comment (linker, "/export:IoAdapterObjectType=ntoskrnl.IoAdapterObjectType")
#pragma comment (linker, "/export:IoAllocateAdapterChannel=ntoskrnl.IoAllocateAdapterChannel")
#pragma comment (linker, "/export:IoAllocateController=ntoskrnl.IoAllocateController")
#pragma comment (linker, "/export:IoAllocateDriverObjectExtension=ntoskrnl.IoAllocateDriverObjectExtension")
#pragma comment (linker, "/export:IoAllocateErrorLogEntry=ntoskrnl.IoAllocateErrorLogEntry")
#pragma comment (linker, "/export:IoAllocateIrp=ntoskrnl.IoAllocateIrp")
#pragma comment (linker, "/export:IoAllocateMdl=ntoskrnl.IoAllocateMdl")
#pragma comment (linker, "/export:IoAllocateWorkItem=ntoskrnl.IoAllocateWorkItem")
#pragma comment (linker, "/export:IoAssignDriveLetters=ntoskrnl.IoAssignDriveLetters")
#pragma comment (linker, "/export:IoAssignResources=ntoskrnl.IoAssignResources")
#pragma comment (linker, "/export:IoAttachDevice=ntoskrnl.IoAttachDevice")
#pragma comment (linker, "/export:IoAttachDeviceByPointer=ntoskrnl.IoAttachDeviceByPointer")
#pragma comment (linker, "/export:IoAttachDeviceToDeviceStack=ntoskrnl.IoAttachDeviceToDeviceStack")
#pragma comment (linker, "/export:IoAttachDeviceToDeviceStackSafe=ntoskrnl.IoAttachDeviceToDeviceStackSafe")
#pragma comment (linker, "/export:IoBuildAsynchronousFsdRequest=ntoskrnl.IoBuildAsynchronousFsdRequest")
#pragma comment (linker, "/export:IoBuildDeviceIoControlRequest=ntoskrnl.IoBuildDeviceIoControlRequest")
#pragma comment (linker, "/export:IoBuildPartialMdl=ntoskrnl.IoBuildPartialMdl")
#pragma comment (linker, "/export:IoBuildSynchronousFsdRequest=ntoskrnl.IoBuildSynchronousFsdRequest")
#pragma comment (linker, "/export:IoCallDriver=ntoskrnl.IoCallDriver")
#pragma comment (linker, "/export:IoCancelFileOpen=ntoskrnl.IoCancelFileOpen")
#pragma comment (linker, "/export:IoCancelIrp=ntoskrnl.IoCancelIrp")
#pragma comment (linker, "/export:IoCheckDesiredAccess=ntoskrnl.IoCheckDesiredAccess")
#pragma comment (linker, "/export:IoCheckEaBufferValidity=ntoskrnl.IoCheckEaBufferValidity")
#pragma comment (linker, "/export:IoCheckFunctionAccess=ntoskrnl.IoCheckFunctionAccess")
#pragma comment (linker, "/export:IoCheckQuerySetFileInformation=ntoskrnl.IoCheckQuerySetFileInformation")
#pragma comment (linker, "/export:IoCheckQuerySetVolumeInformation=ntoskrnl.IoCheckQuerySetVolumeInformation")
#pragma comment (linker, "/export:IoCheckQuotaBufferValidity=ntoskrnl.IoCheckQuotaBufferValidity")
#pragma comment (linker, "/export:IoCheckShareAccess=ntoskrnl.IoCheckShareAccess")
#pragma comment (linker, "/export:IoCompleteRequest=ntoskrnl.IoCompleteRequest")
#pragma comment (linker, "/export:IoConnectInterrupt=ntoskrnl.IoConnectInterrupt")
#pragma comment (linker, "/export:IoCreateController=ntoskrnl.IoCreateController")
#pragma comment (linker, "/export:IoCreateDevice=ntoskrnl.IoCreateDevice")
#pragma comment (linker, "/export:IoCreateDisk=ntoskrnl.IoCreateDisk")
#pragma comment (linker, "/export:IoCreateDriver=ntoskrnl.IoCreateDriver")
#pragma comment (linker, "/export:IoCreateFile=ntoskrnl.IoCreateFile")
#pragma comment (linker, "/export:IoCreateFileSpecifyDeviceObjectHint=ntoskrnl.IoCreateFileSpecifyDeviceObjectHint")
#pragma comment (linker, "/export:IoCreateNotificationEvent=ntoskrnl.IoCreateNotificationEvent")
#pragma comment (linker, "/export:IoCreateStreamFileObject=ntoskrnl.IoCreateStreamFileObject")
#pragma comment (linker, "/export:IoCreateStreamFileObjectEx=ntoskrnl.IoCreateStreamFileObjectEx")
#pragma comment (linker, "/export:IoCreateStreamFileObjectLite=ntoskrnl.IoCreateStreamFileObjectLite")
#pragma comment (linker, "/export:IoCreateSymbolicLink=ntoskrnl.IoCreateSymbolicLink")
#pragma comment (linker, "/export:IoCreateSynchronizationEvent=ntoskrnl.IoCreateSynchronizationEvent")
#pragma comment (linker, "/export:IoCreateUnprotectedSymbolicLink=ntoskrnl.IoCreateUnprotectedSymbolicLink")
#pragma comment (linker, "/export:IoCsqInitialize=ntoskrnl.IoCsqInitialize")
#pragma comment (linker, "/export:IoCsqInsertIrp=ntoskrnl.IoCsqInsertIrp")
#pragma comment (linker, "/export:IoCsqRemoveIrp=ntoskrnl.IoCsqRemoveIrp")
#pragma comment (linker, "/export:IoCsqRemoveNextIrp=ntoskrnl.IoCsqRemoveNextIrp")
#pragma comment (linker, "/export:IoDeleteController=ntoskrnl.IoDeleteController")
#pragma comment (linker, "/export:IoDeleteDevice=ntoskrnl.IoDeleteDevice")
#pragma comment (linker, "/export:IoDeleteDriver=ntoskrnl.IoDeleteDriver")
#pragma comment (linker, "/export:IoDeleteSymbolicLink=ntoskrnl.IoDeleteSymbolicLink")
#pragma comment (linker, "/export:IoDetachDevice=ntoskrnl.IoDetachDevice")
#pragma comment (linker, "/export:IoDeviceHandlerObjectSize=ntoskrnl.IoDeviceHandlerObjectSize")
#pragma comment (linker, "/export:IoDeviceHandlerObjectType=ntoskrnl.IoDeviceHandlerObjectType")
#pragma comment (linker, "/export:IoDeviceObjectType=ntoskrnl.IoDeviceObjectType")
#pragma comment (linker, "/export:IoDisconnectInterrupt=ntoskrnl.IoDisconnectInterrupt")
#pragma comment (linker, "/export:IoDriverObjectType=ntoskrnl.IoDriverObjectType")
#pragma comment (linker, "/export:IoEnqueueIrp=ntoskrnl.IoEnqueueIrp")
#pragma comment (linker, "/export:IoEnumerateDeviceObjectList=ntoskrnl.IoEnumerateDeviceObjectList")
#pragma comment (linker, "/export:IoFastQueryNetworkAttributes=ntoskrnl.IoFastQueryNetworkAttributes")
#pragma comment (linker, "/export:IofCallDriver=ntoskrnl.IofCallDriver")
#pragma comment (linker, "/export:IofCompleteRequest=ntoskrnl.IofCompleteRequest")
#pragma comment (linker, "/export:IoFileObjectType=ntoskrnl.IoFileObjectType")
#pragma comment (linker, "/export:IoForwardAndCatchIrp=ntoskrnl.IoForwardAndCatchIrp")
#pragma comment (linker, "/export:IoForwardIrpSynchronously=ntoskrnl.IoForwardIrpSynchronously")
#pragma comment (linker, "/export:IoFreeController=ntoskrnl.IoFreeController")
#pragma comment (linker, "/export:IoFreeErrorLogEntry=ntoskrnl.IoFreeErrorLogEntry")
#pragma comment (linker, "/export:IoFreeIrp=ntoskrnl.IoFreeIrp")
#pragma comment (linker, "/export:IoFreeMdl=ntoskrnl.IoFreeMdl")
#pragma comment (linker, "/export:IoGetAttachedDevice=ntoskrnl.IoGetAttachedDevice")
#pragma comment (linker, "/export:IoGetAttachedDeviceReference=ntoskrnl.IoGetAttachedDeviceReference")
#pragma comment (linker, "/export:IoGetBaseFileSystemDeviceObject=ntoskrnl.IoGetBaseFileSystemDeviceObject")
#pragma comment (linker, "/export:IoGetBootDiskInformation=ntoskrnl.IoGetBootDiskInformation")
#pragma comment (linker, "/export:IoGetConfigurationInformation=ntoskrnl.IoGetConfigurationInformation")
#pragma comment (linker, "/export:IoGetCurrentProcess=ntoskrnl.IoGetCurrentProcess")
#pragma comment (linker, "/export:IoGetDeviceAttachmentBaseRef=ntoskrnl.IoGetDeviceAttachmentBaseRef")
#pragma comment (linker, "/export:IoGetDeviceInterfaceAlias=ntoskrnl.IoGetDeviceInterfaceAlias")
#pragma comment (linker, "/export:IoGetDeviceInterfaces=ntoskrnl.IoGetDeviceInterfaces")
#pragma comment (linker, "/export:IoGetDeviceObjectPointer=ntoskrnl.IoGetDeviceObjectPointer")
#pragma comment (linker, "/export:IoGetDeviceProperty=ntoskrnl.IoGetDeviceProperty")
#pragma comment (linker, "/export:IoGetDeviceToVerify=ntoskrnl.IoGetDeviceToVerify")
#pragma comment (linker, "/export:IoGetDiskDeviceObject=ntoskrnl.IoGetDiskDeviceObject")
#pragma comment (linker, "/export:IoGetDmaAdapter=ntoskrnl.IoGetDmaAdapter")
#pragma comment (linker, "/export:IoGetDriverObjectExtension=ntoskrnl.IoGetDriverObjectExtension")
#pragma comment (linker, "/export:IoGetFileObjectGenericMapping=ntoskrnl.IoGetFileObjectGenericMapping")
#pragma comment (linker, "/export:IoGetInitialStack=ntoskrnl.IoGetInitialStack")
#pragma comment (linker, "/export:IoGetLowerDeviceObject=ntoskrnl.IoGetLowerDeviceObject")
#pragma comment (linker, "/export:IoGetRelatedDeviceObject=ntoskrnl.IoGetRelatedDeviceObject")
#pragma comment (linker, "/export:IoGetRequestorProcess=ntoskrnl.IoGetRequestorProcess")
#pragma comment (linker, "/export:IoGetRequestorProcessId=ntoskrnl.IoGetRequestorProcessId")
#pragma comment (linker, "/export:IoGetRequestorSessionId=ntoskrnl.IoGetRequestorSessionId")
#pragma comment (linker, "/export:IoGetStackLimits=ntoskrnl.IoGetStackLimits")
#pragma comment (linker, "/export:IoGetTopLevelIrp=ntoskrnl.IoGetTopLevelIrp")
#pragma comment (linker, "/export:IoInitializeIrp=ntoskrnl.IoInitializeIrp")
#pragma comment (linker, "/export:IoInitializeRemoveLockEx=ntoskrnl.IoInitializeRemoveLockEx")
#pragma comment (linker, "/export:IoInitializeTimer=ntoskrnl.IoInitializeTimer")
#pragma comment (linker, "/export:IoInvalidateDeviceRelations=ntoskrnl.IoInvalidateDeviceRelations")
#pragma comment (linker, "/export:IoInvalidateDeviceState=ntoskrnl.IoInvalidateDeviceState")
#pragma comment (linker, "/export:IoIsFileOriginRemote=ntoskrnl.IoIsFileOriginRemote")
#pragma comment (linker, "/export:IoIsOperationSynchronous=ntoskrnl.IoIsOperationSynchronous")
#pragma comment (linker, "/export:IoIsSystemThread=ntoskrnl.IoIsSystemThread")
#pragma comment (linker, "/export:IoIsValidNameGraftingBuffer=ntoskrnl.IoIsValidNameGraftingBuffer")
#pragma comment (linker, "/export:IoIsWdmVersionAvailable=ntoskrnl.IoIsWdmVersionAvailable")
#pragma comment (linker, "/export:IoMakeAssociatedIrp=ntoskrnl.IoMakeAssociatedIrp")
#pragma comment (linker, "/export:IoOpenDeviceInterfaceRegistryKey=ntoskrnl.IoOpenDeviceInterfaceRegistryKey")
#pragma comment (linker, "/export:IoOpenDeviceRegistryKey=ntoskrnl.IoOpenDeviceRegistryKey")
#pragma comment (linker, "/export:IoPageRead=ntoskrnl.IoPageRead")
#pragma comment (linker, "/export:IoPnPDeliverServicePowerNotification=ntoskrnl.IoPnPDeliverServicePowerNotification")
#pragma comment (linker, "/export:IoQueryDeviceDescription=ntoskrnl.IoQueryDeviceDescription")
#pragma comment (linker, "/export:IoQueryFileDosDeviceName=ntoskrnl.IoQueryFileDosDeviceName")
#pragma comment (linker, "/export:IoQueryFileInformation=ntoskrnl.IoQueryFileInformation")
#pragma comment (linker, "/export:IoQueryVolumeInformation=ntoskrnl.IoQueryVolumeInformation")
#pragma comment (linker, "/export:IoQueueThreadIrp=ntoskrnl.IoQueueThreadIrp")
#pragma comment (linker, "/export:IoQueueWorkItem=ntoskrnl.IoQueueWorkItem")
#pragma comment (linker, "/export:IoRaiseHardError=ntoskrnl.IoRaiseHardError")
#pragma comment (linker, "/export:IoRaiseInformationalHardError=ntoskrnl.IoRaiseInformationalHardError")
#pragma comment (linker, "/export:IoReadDiskSignature=ntoskrnl.IoReadDiskSignature")
#pragma comment (linker, "/export:IoReadOperationCount=ntoskrnl.IoReadOperationCount")
#pragma comment (linker, "/export:IoReadPartitionTable=ntoskrnl.IoReadPartitionTable")
#pragma comment (linker, "/export:IoReadPartitionTableEx=ntoskrnl.IoReadPartitionTableEx")
#pragma comment (linker, "/export:IoReadTransferCount=ntoskrnl.IoReadTransferCount")
#pragma comment (linker, "/export:IoRegisterBootDriverReinitialization=ntoskrnl.IoRegisterBootDriverReinitialization")
#pragma comment (linker, "/export:IoRegisterDeviceInterface=ntoskrnl.IoRegisterDeviceInterface")
#pragma comment (linker, "/export:IoRegisterDriverReinitialization=ntoskrnl.IoRegisterDriverReinitialization")
#pragma comment (linker, "/export:IoRegisterFileSystem=ntoskrnl.IoRegisterFileSystem")
#pragma comment (linker, "/export:IoRegisterFsRegistrationChange=ntoskrnl.IoRegisterFsRegistrationChange")
#pragma comment (linker, "/export:IoRegisterLastChanceShutdownNotification=ntoskrnl.IoRegisterLastChanceShutdownNotification")
#pragma comment (linker, "/export:IoRegisterPlugPlayNotification=ntoskrnl.IoRegisterPlugPlayNotification")
#pragma comment (linker, "/export:IoRegisterShutdownNotification=ntoskrnl.IoRegisterShutdownNotification")
#pragma comment (linker, "/export:IoReleaseCancelSpinLock=ntoskrnl.IoReleaseCancelSpinLock")
#pragma comment (linker, "/export:IoReleaseRemoveLockAndWaitEx=ntoskrnl.IoReleaseRemoveLockAndWaitEx")
#pragma comment (linker, "/export:IoReleaseRemoveLockEx=ntoskrnl.IoReleaseRemoveLockEx")
#pragma comment (linker, "/export:IoReleaseVpbSpinLock=ntoskrnl.IoReleaseVpbSpinLock")
#pragma comment (linker, "/export:IoRemoveShareAccess=ntoskrnl.IoRemoveShareAccess")
#pragma comment (linker, "/export:IoReportDetectedDevice=ntoskrnl.IoReportDetectedDevice")
#pragma comment (linker, "/export:IoReportHalResourceUsage=ntoskrnl.IoReportHalResourceUsage")
#pragma comment (linker, "/export:IoReportResourceForDetection=ntoskrnl.IoReportResourceForDetection")
#pragma comment (linker, "/export:IoReportResourceUsage=ntoskrnl.IoReportResourceUsage")
#pragma comment (linker, "/export:IoReportTargetDeviceChange=ntoskrnl.IoReportTargetDeviceChange")
#pragma comment (linker, "/export:IoReportTargetDeviceChangeAsynchronous=ntoskrnl.IoReportTargetDeviceChangeAsynchronous")
#pragma comment (linker, "/export:IoRequestDeviceEject=ntoskrnl.IoRequestDeviceEject")
#pragma comment (linker, "/export:IoReuseIrp=ntoskrnl.IoReuseIrp")
#pragma comment (linker, "/export:IoSetCompletionRoutineEx=ntoskrnl.IoSetCompletionRoutineEx")
#pragma comment (linker, "/export:IoSetDeviceInterfaceState=ntoskrnl.IoSetDeviceInterfaceState")
#pragma comment (linker, "/export:IoSetDeviceToVerify=ntoskrnl.IoSetDeviceToVerify")
#pragma comment (linker, "/export:IoSetFileOrigin=ntoskrnl.IoSetFileOrigin")
#pragma comment (linker, "/export:IoSetHardErrorOrVerifyDevice=ntoskrnl.IoSetHardErrorOrVerifyDevice")
#pragma comment (linker, "/export:IoSetInformation=ntoskrnl.IoSetInformation")
#pragma comment (linker, "/export:IoSetIoCompletion=ntoskrnl.IoSetIoCompletion")
#pragma comment (linker, "/export:IoSetPartitionInformation=ntoskrnl.IoSetPartitionInformation")
#pragma comment (linker, "/export:IoSetPartitionInformationEx=ntoskrnl.IoSetPartitionInformationEx")
#pragma comment (linker, "/export:IoSetShareAccess=ntoskrnl.IoSetShareAccess")
#pragma comment (linker, "/export:IoSetStartIoAttributes=ntoskrnl.IoSetStartIoAttributes")
#pragma comment (linker, "/export:IoSetSystemPartition=ntoskrnl.IoSetSystemPartition")
#pragma comment (linker, "/export:IoSetThreadHardErrorMode=ntoskrnl.IoSetThreadHardErrorMode")
#pragma comment (linker, "/export:IoSetTopLevelIrp=ntoskrnl.IoSetTopLevelIrp")
#pragma comment (linker, "/export:IoStartNextPacket=ntoskrnl.IoStartNextPacket")
#pragma comment (linker, "/export:IoStartNextPacketByKey=ntoskrnl.IoStartNextPacketByKey")
#pragma comment (linker, "/export:IoStartPacket=ntoskrnl.IoStartPacket")
#pragma comment (linker, "/export:IoStartTimer=ntoskrnl.IoStartTimer")
#pragma comment (linker, "/export:IoStatisticsLock=ntoskrnl.IoStatisticsLock")
#pragma comment (linker, "/export:IoStopTimer=ntoskrnl.IoStopTimer")
#pragma comment (linker, "/export:IoSynchronousInvalidateDeviceRelations=ntoskrnl.IoSynchronousInvalidateDeviceRelations")
#pragma comment (linker, "/export:IoSynchronousPageWrite=ntoskrnl.IoSynchronousPageWrite")
#pragma comment (linker, "/export:IoThreadToProcess=ntoskrnl.IoThreadToProcess")
#pragma comment (linker, "/export:IoUnregisterFileSystem=ntoskrnl.IoUnregisterFileSystem")
#pragma comment (linker, "/export:IoUnregisterFsRegistrationChange=ntoskrnl.IoUnregisterFsRegistrationChange")
#pragma comment (linker, "/export:IoUnregisterPlugPlayNotification=ntoskrnl.IoUnregisterPlugPlayNotification")
#pragma comment (linker, "/export:IoUnregisterShutdownNotification=ntoskrnl.IoUnregisterShutdownNotification")
#pragma comment (linker, "/export:IoUpdateShareAccess=ntoskrnl.IoUpdateShareAccess")
#pragma comment (linker, "/export:IoVerifyPartitionTable=ntoskrnl.IoVerifyPartitionTable")
#pragma comment (linker, "/export:IoVerifyVolume=ntoskrnl.IoVerifyVolume")
#pragma comment (linker, "/export:IoVolumeDeviceToDosName=ntoskrnl.IoVolumeDeviceToDosName")
#pragma comment (linker, "/export:IoWMIAllocateInstanceIds=ntoskrnl.IoWMIAllocateInstanceIds")
#pragma comment (linker, "/export:IoWMIDeviceObjectToInstanceName=ntoskrnl.IoWMIDeviceObjectToInstanceName")
#pragma comment (linker, "/export:IoWMIExecuteMethod=ntoskrnl.IoWMIExecuteMethod")
#pragma comment (linker, "/export:IoWMIHandleToInstanceName=ntoskrnl.IoWMIHandleToInstanceName")
#pragma comment (linker, "/export:IoWMIOpenBlock=ntoskrnl.IoWMIOpenBlock")
#pragma comment (linker, "/export:IoWMIQueryAllData=ntoskrnl.IoWMIQueryAllData")
#pragma comment (linker, "/export:IoWMIQueryAllDataMultiple=ntoskrnl.IoWMIQueryAllDataMultiple")
#pragma comment (linker, "/export:IoWMIQuerySingleInstance=ntoskrnl.IoWMIQuerySingleInstance")
#pragma comment (linker, "/export:IoWMIQuerySingleInstanceMultiple=ntoskrnl.IoWMIQuerySingleInstanceMultiple")
#pragma comment (linker, "/export:IoWMIRegistrationControl=ntoskrnl.IoWMIRegistrationControl")
#pragma comment (linker, "/export:IoWMISetNotificationCallback=ntoskrnl.IoWMISetNotificationCallback")
#pragma comment (linker, "/export:IoWMISetSingleInstance=ntoskrnl.IoWMISetSingleInstance")
#pragma comment (linker, "/export:IoWMISetSingleItem=ntoskrnl.IoWMISetSingleItem")
#pragma comment (linker, "/export:IoWMISuggestInstanceName=ntoskrnl.IoWMISuggestInstanceName")
#pragma comment (linker, "/export:IoWMIWriteEvent=ntoskrnl.IoWMIWriteEvent")
#pragma comment (linker, "/export:IoWriteErrorLogEntry=ntoskrnl.IoWriteErrorLogEntry")
#pragma comment (linker, "/export:IoWriteOperationCount=ntoskrnl.IoWriteOperationCount")
#pragma comment (linker, "/export:IoWritePartitionTable=ntoskrnl.IoWritePartitionTable")
#pragma comment (linker, "/export:IoWritePartitionTableEx=ntoskrnl.IoWritePartitionTableEx")
#pragma comment (linker, "/export:IoWriteTransferCount=ntoskrnl.IoWriteTransferCount")
#pragma comment (linker, "/export:isdigit=ntoskrnl.isdigit")
#pragma comment (linker, "/export:islower=ntoskrnl.islower")
#pragma comment (linker, "/export:isprint=ntoskrnl.isprint")
#pragma comment (linker, "/export:isspace=ntoskrnl.isspace")
#pragma comment (linker, "/export:isupper=ntoskrnl.isupper")
#pragma comment (linker, "/export:isxdigit=ntoskrnl.isxdigit")
#pragma comment (linker, "/export:KdDebuggerEnabled=ntoskrnl.KdDebuggerEnabled")
#pragma comment (linker, "/export:KdDebuggerNotPresent=ntoskrnl.KdDebuggerNotPresent")
#pragma comment (linker, "/export:KdDisableDebugger=ntoskrnl.KdDisableDebugger")
#pragma comment (linker, "/export:KdEnableDebugger=ntoskrnl.KdEnableDebugger")
#pragma comment (linker, "/export:KdEnteredDebugger=ntoskrnl.KdEnteredDebugger")
#pragma comment (linker, "/export:KdPollBreakIn=ntoskrnl.KdPollBreakIn")
#pragma comment (linker, "/export:KdPowerTransition=ntoskrnl.KdPowerTransition")
#pragma comment (linker, "/export:Ke386CallBios=ntoskrnl.Ke386CallBios")
#pragma comment (linker, "/export:Ke386IoSetAccessProcess=ntoskrnl.Ke386IoSetAccessProcess")
#pragma comment (linker, "/export:Ke386QueryIoAccessMap=ntoskrnl.Ke386QueryIoAccessMap")
#pragma comment (linker, "/export:Ke386SetIoAccessMap=ntoskrnl.Ke386SetIoAccessMap")
#pragma comment (linker, "/export:KeAcquireInStackQueuedSpinLockAtDpcLevel=ntoskrnl.KeAcquireInStackQueuedSpinLockAtDpcLevel")
#pragma comment (linker, "/export:KeAcquireInterruptSpinLock=ntoskrnl.KeAcquireInterruptSpinLock")
#pragma comment (linker, "/export:KeAcquireSpinLockAtDpcLevel=ntoskrnl.KeAcquireSpinLockAtDpcLevel")
#pragma comment (linker, "/export:KeAddSystemServiceTable=ntoskrnl.KeAddSystemServiceTable")
#pragma comment (linker, "/export:KeAreApcsDisabled=ntoskrnl.KeAreApcsDisabled")
#pragma comment (linker, "/export:KeAttachProcess=ntoskrnl.KeAttachProcess")
#pragma comment (linker, "/export:KeBugCheck=ntoskrnl.KeBugCheck")
#pragma comment (linker, "/export:KeBugCheckEx=ntoskrnl.KeBugCheckEx")
#pragma comment (linker, "/export:KeCancelTimer=ntoskrnl.KeCancelTimer")
#pragma comment (linker, "/export:KeClearEvent=ntoskrnl.KeClearEvent")
#pragma comment (linker, "/export:KeConnectInterrupt=ntoskrnl.KeConnectInterrupt")
#pragma comment (linker, "/export:KeDcacheFlushCount=ntoskrnl.KeDcacheFlushCount")
#pragma comment (linker, "/export:KeDelayExecutionThread=ntoskrnl.KeDelayExecutionThread")
#pragma comment (linker, "/export:KeDeregisterBugCheckCallback=ntoskrnl.KeDeregisterBugCheckCallback")
#pragma comment (linker, "/export:KeDetachProcess=ntoskrnl.KeDetachProcess")
#pragma comment (linker, "/export:KeDisconnectInterrupt=ntoskrnl.KeDisconnectInterrupt")
#pragma comment (linker, "/export:KeEnterCriticalRegion=ntoskrnl.KeEnterCriticalRegion")
#pragma comment (linker, "/export:KeEnterKernelDebugger=ntoskrnl.KeEnterKernelDebugger")
#pragma comment (linker, "/export:KefAcquireSpinLockAtDpcLevel=ntoskrnl.KefAcquireSpinLockAtDpcLevel")
#pragma comment (linker, "/export:KeFindConfigurationEntry=ntoskrnl.KeFindConfigurationEntry")
#pragma comment (linker, "/export:KeFindConfigurationNextEntry=ntoskrnl.KeFindConfigurationNextEntry")
#pragma comment (linker, "/export:KeFlushEntireTb=ntoskrnl.KeFlushEntireTb")
#pragma comment (linker, "/export:KefReleaseSpinLockFromDpcLevel=ntoskrnl.KefReleaseSpinLockFromDpcLevel")
#pragma comment (linker, "/export:KeGetCurrentThread=ntoskrnl.KeGetCurrentThread")
#pragma comment (linker, "/export:KeGetPreviousMode=ntoskrnl.KeGetPreviousMode")
#pragma comment (linker, "/export:KeGetRecommendedSharedDataAlignment=ntoskrnl.KeGetRecommendedSharedDataAlignment")
#pragma comment (linker, "/export:KeI386AbiosCall=ntoskrnl.KeI386AbiosCall")
#pragma comment (linker, "/export:KeI386AllocateGdtSelectors=ntoskrnl.KeI386AllocateGdtSelectors")
#pragma comment (linker, "/export:KeI386Call16BitCStyleFunction=ntoskrnl.KeI386Call16BitCStyleFunction")
#pragma comment (linker, "/export:KeI386Call16BitFunction=ntoskrnl.KeI386Call16BitFunction")
#pragma comment (linker, "/export:Kei386EoiHelper=ntoskrnl.Kei386EoiHelper")
#pragma comment (linker, "/export:KeI386FlatToGdtSelector=ntoskrnl.KeI386FlatToGdtSelector")
#pragma comment (linker, "/export:KeI386GetLid=ntoskrnl.KeI386GetLid")
#pragma comment (linker, "/export:KeI386MachineType=ntoskrnl.KeI386MachineType")
#pragma comment (linker, "/export:KeI386ReleaseGdtSelectors=ntoskrnl.KeI386ReleaseGdtSelectors")
#pragma comment (linker, "/export:KeI386ReleaseLid=ntoskrnl.KeI386ReleaseLid")
#pragma comment (linker, "/export:KeI386SetGdtSelector=ntoskrnl.KeI386SetGdtSelector")
#pragma comment (linker, "/export:KeIcacheFlushCount=ntoskrnl.KeIcacheFlushCount")
#pragma comment (linker, "/export:KeInitializeApc=ntoskrnl.KeInitializeApc")
#pragma comment (linker, "/export:KeInitializeDeviceQueue=ntoskrnl.KeInitializeDeviceQueue")
#pragma comment (linker, "/export:KeInitializeDpc=ntoskrnl.KeInitializeDpc")
#pragma comment (linker, "/export:KeInitializeEvent=ntoskrnl.KeInitializeEvent")
#pragma comment (linker, "/export:KeInitializeInterrupt=ntoskrnl.KeInitializeInterrupt")
#pragma comment (linker, "/export:KeInitializeMutant=ntoskrnl.KeInitializeMutant")
#pragma comment (linker, "/export:KeInitializeMutex=ntoskrnl.KeInitializeMutex")
#pragma comment (linker, "/export:KeInitializeQueue=ntoskrnl.KeInitializeQueue")
#pragma comment (linker, "/export:KeInitializeSemaphore=ntoskrnl.KeInitializeSemaphore")
#pragma comment (linker, "/export:KeInitializeSpinLock=ntoskrnl.KeInitializeSpinLock")
#pragma comment (linker, "/export:KeInitializeTimer=ntoskrnl.KeInitializeTimer")
#pragma comment (linker, "/export:KeInitializeTimerEx=ntoskrnl.KeInitializeTimerEx")
#pragma comment (linker, "/export:KeInsertByKeyDeviceQueue=ntoskrnl.KeInsertByKeyDeviceQueue")
#pragma comment (linker, "/export:KeInsertDeviceQueue=ntoskrnl.KeInsertDeviceQueue")
#pragma comment (linker, "/export:KeInsertHeadQueue=ntoskrnl.KeInsertHeadQueue")
#pragma comment (linker, "/export:KeInsertQueue=ntoskrnl.KeInsertQueue")
#pragma comment (linker, "/export:KeInsertQueueApc=ntoskrnl.KeInsertQueueApc")
#pragma comment (linker, "/export:KeInsertQueueDpc=ntoskrnl.KeInsertQueueDpc")
#pragma comment (linker, "/export:KeIsAttachedProcess=ntoskrnl.KeIsAttachedProcess")
#pragma comment (linker, "/export:KeIsExecutingDpc=ntoskrnl.KeIsExecutingDpc")
#pragma comment (linker, "/export:KeLeaveCriticalRegion=ntoskrnl.KeLeaveCriticalRegion")
#pragma comment (linker, "/export:KeLoaderBlock=ntoskrnl.KeLoaderBlock")
#pragma comment (linker, "/export:KeNumberProcessors=ntoskrnl.KeNumberProcessors")
#pragma comment (linker, "/export:KeProfileInterrupt=ntoskrnl.KeProfileInterrupt")
#pragma comment (linker, "/export:KeProfileInterruptWithSource=ntoskrnl.KeProfileInterruptWithSource")
#pragma comment (linker, "/export:KePulseEvent=ntoskrnl.KePulseEvent")
#pragma comment (linker, "/export:KeQueryActiveProcessors=ntoskrnl.KeQueryActiveProcessors")
#pragma comment (linker, "/export:KeQueryInterruptTime=ntoskrnl.KeQueryInterruptTime")
#pragma comment (linker, "/export:KeQueryPriorityThread=ntoskrnl.KeQueryPriorityThread")
#pragma comment (linker, "/export:KeQueryRuntimeThread=ntoskrnl.KeQueryRuntimeThread")
#pragma comment (linker, "/export:KeQuerySystemTime=ntoskrnl.KeQuerySystemTime")
#pragma comment (linker, "/export:KeQueryTickCount=ntoskrnl.KeQueryTickCount")
#pragma comment (linker, "/export:KeQueryTimeIncrement=ntoskrnl.KeQueryTimeIncrement")
#pragma comment (linker, "/export:KeRaiseUserException=ntoskrnl.KeRaiseUserException")
#pragma comment (linker, "/export:KeReadStateEvent=ntoskrnl.KeReadStateEvent")
#pragma comment (linker, "/export:KeReadStateMutant=ntoskrnl.KeReadStateMutant")
#pragma comment (linker, "/export:KeReadStateMutex=ntoskrnl.KeReadStateMutex")
#pragma comment (linker, "/export:KeReadStateQueue=ntoskrnl.KeReadStateQueue")
#pragma comment (linker, "/export:KeReadStateSemaphore=ntoskrnl.KeReadStateSemaphore")
#pragma comment (linker, "/export:KeReadStateTimer=ntoskrnl.KeReadStateTimer")
#pragma comment (linker, "/export:KeRegisterBugCheckCallback=ntoskrnl.KeRegisterBugCheckCallback")
#pragma comment (linker, "/export:KeReleaseInStackQueuedSpinLockFromDpcLevel=ntoskrnl.KeReleaseInStackQueuedSpinLockFromDpcLevel")
#pragma comment (linker, "/export:KeReleaseInterruptSpinLock=ntoskrnl.KeReleaseInterruptSpinLock")
#pragma comment (linker, "/export:KeReleaseMutant=ntoskrnl.KeReleaseMutant")
#pragma comment (linker, "/export:KeReleaseMutex=ntoskrnl.KeReleaseMutex")
#pragma comment (linker, "/export:KeReleaseSemaphore=ntoskrnl.KeReleaseSemaphore")
#pragma comment (linker, "/export:KeReleaseSpinLockFromDpcLevel=ntoskrnl.KeReleaseSpinLockFromDpcLevel")
#pragma comment (linker, "/export:KeRemoveByKeyDeviceQueue=ntoskrnl.KeRemoveByKeyDeviceQueue")
#pragma comment (linker, "/export:KeRemoveByKeyDeviceQueueIfBusy=ntoskrnl.KeRemoveByKeyDeviceQueueIfBusy")
#pragma comment (linker, "/export:KeRemoveDeviceQueue=ntoskrnl.KeRemoveDeviceQueue")
#pragma comment (linker, "/export:KeRemoveEntryDeviceQueue=ntoskrnl.KeRemoveEntryDeviceQueue")
#pragma comment (linker, "/export:KeRemoveQueue=ntoskrnl.KeRemoveQueue")
#pragma comment (linker, "/export:KeRemoveQueueDpc=ntoskrnl.KeRemoveQueueDpc")
#pragma comment (linker, "/export:KeRemoveSystemServiceTable=ntoskrnl.KeRemoveSystemServiceTable")
#pragma comment (linker, "/export:KeResetEvent=ntoskrnl.KeResetEvent")
#pragma comment (linker, "/export:KeRestoreFloatingPointState=ntoskrnl.KeRestoreFloatingPointState")
#pragma comment (linker, "/export:KeRevertToUserAffinityThread=ntoskrnl.KeRevertToUserAffinityThread")
#pragma comment (linker, "/export:KeRundownQueue=ntoskrnl.KeRundownQueue")
#pragma comment (linker, "/export:KeSaveFloatingPointState=ntoskrnl.KeSaveFloatingPointState")
#pragma comment (linker, "/export:KeSaveStateForHibernate=ntoskrnl.KeSaveStateForHibernate")
#pragma comment (linker, "/export:KeServiceDescriptorTable=ntoskrnl.KeServiceDescriptorTable")
#pragma comment (linker, "/export:KeSetAffinityThread=ntoskrnl.KeSetAffinityThread")
#pragma comment (linker, "/export:KeSetBasePriorityThread=ntoskrnl.KeSetBasePriorityThread")
#pragma comment (linker, "/export:KeSetDmaIoCoherency=ntoskrnl.KeSetDmaIoCoherency")
#pragma comment (linker, "/export:KeSetEvent=ntoskrnl.KeSetEvent")
#pragma comment (linker, "/export:KeSetEventBoostPriority=ntoskrnl.KeSetEventBoostPriority")
#pragma comment (linker, "/export:KeSetIdealProcessorThread=ntoskrnl.KeSetIdealProcessorThread")
#pragma comment (linker, "/export:KeSetImportanceDpc=ntoskrnl.KeSetImportanceDpc")
#pragma comment (linker, "/export:KeSetKernelStackSwapEnable=ntoskrnl.KeSetKernelStackSwapEnable")
#pragma comment (linker, "/export:KeSetPriorityThread=ntoskrnl.KeSetPriorityThread")
#pragma comment (linker, "/export:KeSetProfileIrql=ntoskrnl.KeSetProfileIrql")
#pragma comment (linker, "/export:KeSetSystemAffinityThread=ntoskrnl.KeSetSystemAffinityThread")
#pragma comment (linker, "/export:KeSetTargetProcessorDpc=ntoskrnl.KeSetTargetProcessorDpc")
#pragma comment (linker, "/export:KeSetTimeIncrement=ntoskrnl.KeSetTimeIncrement")
#pragma comment (linker, "/export:KeSetTimer=ntoskrnl.KeSetTimer")
#pragma comment (linker, "/export:KeSetTimerEx=ntoskrnl.KeSetTimerEx")
#pragma comment (linker, "/export:KeSetTimeUpdateNotifyRoutine=ntoskrnl.KeSetTimeUpdateNotifyRoutine")
#pragma comment (linker, "/export:KeStackAttachProcess=ntoskrnl.KeStackAttachProcess")
#pragma comment (linker, "/export:KeSynchronizeExecution=ntoskrnl.KeSynchronizeExecution")
#pragma comment (linker, "/export:KeTerminateThread=ntoskrnl.KeTerminateThread")
#pragma comment (linker, "/export:KeTickCount=ntoskrnl.KeTickCount")
#pragma comment (linker, "/export:KeUnstackDetachProcess=ntoskrnl.KeUnstackDetachProcess")
#pragma comment (linker, "/export:KeUpdateRunTime=ntoskrnl.KeUpdateRunTime")
#pragma comment (linker, "/export:KeUpdateSystemTime=ntoskrnl.KeUpdateSystemTime")
#pragma comment (linker, "/export:KeUserModeCallback=ntoskrnl.KeUserModeCallback")
#pragma comment (linker, "/export:KeWaitForMultipleObjects=ntoskrnl.KeWaitForMultipleObjects")
#pragma comment (linker, "/export:KeWaitForMutexObject=ntoskrnl.KeWaitForMutexObject")
#pragma comment (linker, "/export:KeWaitForSingleObject=ntoskrnl.KeWaitForSingleObject")
#pragma comment (linker, "/export:KiAcquireSpinLock=ntoskrnl.KiAcquireSpinLock")
#pragma comment (linker, "/export:KiBugCheckData=ntoskrnl.KiBugCheckData")
#pragma comment (linker, "/export:KiCoprocessorError=ntoskrnl.KiCoprocessorError")
#pragma comment (linker, "/export:KiDeliverApc=ntoskrnl.KiDeliverApc")
#pragma comment (linker, "/export:KiDispatchInterrupt=ntoskrnl.KiDispatchInterrupt")
#pragma comment (linker, "/export:KiEnableTimerWatchdog=ntoskrnl.KiEnableTimerWatchdog")
#pragma comment (linker, "/export:Kii386SpinOnSpinLock=ntoskrnl.Kii386SpinOnSpinLock")
#pragma comment (linker, "/export:KiIpiServiceRoutine=ntoskrnl.KiIpiServiceRoutine")
#pragma comment (linker, "/export:KiReleaseSpinLock=ntoskrnl.KiReleaseSpinLock")
#pragma comment (linker, "/export:KiUnexpectedInterrupt=ntoskrnl.KiUnexpectedInterrupt")
#pragma comment (linker, "/export:LdrAccessResource=ntoskrnl.LdrAccessResource")
#pragma comment (linker, "/export:LdrEnumResources=ntoskrnl.LdrEnumResources")
#pragma comment (linker, "/export:LdrFindResource_U=ntoskrnl.LdrFindResource_U")
#pragma comment (linker, "/export:LdrFindResourceDirectory_U=ntoskrnl.LdrFindResourceDirectory_U")
#pragma comment (linker, "/export:LpcPortObjectType=ntoskrnl.LpcPortObjectType")
#pragma comment (linker, "/export:LpcRequestPort=ntoskrnl.LpcRequestPort")
#pragma comment (linker, "/export:LpcRequestWaitReplyPort=ntoskrnl.LpcRequestWaitReplyPort")
#pragma comment (linker, "/export:LsaCallAuthenticationPackage=ntoskrnl.LsaCallAuthenticationPackage")
#pragma comment (linker, "/export:LsaDeregisterLogonProcess=ntoskrnl.LsaDeregisterLogonProcess")
#pragma comment (linker, "/export:LsaFreeReturnBuffer=ntoskrnl.LsaFreeReturnBuffer")
#pragma comment (linker, "/export:LsaLogonUser=ntoskrnl.LsaLogonUser")
#pragma comment (linker, "/export:LsaLookupAuthenticationPackage=ntoskrnl.LsaLookupAuthenticationPackage")
#pragma comment (linker, "/export:LsaRegisterLogonProcess=ntoskrnl.LsaRegisterLogonProcess")
#pragma comment (linker, "/export:mbstowcs=ntoskrnl.mbstowcs")
#pragma comment (linker, "/export:mbtowc=ntoskrnl.mbtowc")
#pragma comment (linker, "/export:memchr=ntoskrnl.memchr")
#pragma comment (linker, "/export:memcpy=ntoskrnl.memcpy")
#pragma comment (linker, "/export:memmove=ntoskrnl.memmove")
#pragma comment (linker, "/export:memset=ntoskrnl.memset")
#pragma comment (linker, "/export:Mm64BitPhysicalAddress=ntoskrnl.Mm64BitPhysicalAddress")
#pragma comment (linker, "/export:MmAddPhysicalMemory=ntoskrnl.MmAddPhysicalMemory")
#pragma comment (linker, "/export:MmAddVerifierThunks=ntoskrnl.MmAddVerifierThunks")
#pragma comment (linker, "/export:MmAdjustWorkingSetSize=ntoskrnl.MmAdjustWorkingSetSize")
#pragma comment (linker, "/export:MmAdvanceMdl=ntoskrnl.MmAdvanceMdl")
#pragma comment (linker, "/export:MmAllocateContiguousMemory=ntoskrnl.MmAllocateContiguousMemory")
#pragma comment (linker, "/export:MmAllocateContiguousMemorySpecifyCache=ntoskrnl.MmAllocateContiguousMemorySpecifyCache")
#pragma comment (linker, "/export:MmAllocateMappingAddress=ntoskrnl.MmAllocateMappingAddress")
#pragma comment (linker, "/export:MmAllocateNonCachedMemory=ntoskrnl.MmAllocateNonCachedMemory")
#pragma comment (linker, "/export:MmAllocatePagesForMdl=ntoskrnl.MmAllocatePagesForMdl")
#pragma comment (linker, "/export:MmBuildMdlForNonPagedPool=ntoskrnl.MmBuildMdlForNonPagedPool")
#pragma comment (linker, "/export:MmCanFileBeTruncated=ntoskrnl.MmCanFileBeTruncated")
#pragma comment (linker, "/export:MmCreateMdl=ntoskrnl.MmCreateMdl")
#pragma comment (linker, "/export:MmCreateSection=ntoskrnl.MmCreateSection")
#pragma comment (linker, "/export:MmDisableModifiedWriteOfSection=ntoskrnl.MmDisableModifiedWriteOfSection")
#pragma comment (linker, "/export:MmFlushImageSection=ntoskrnl.MmFlushImageSection")
#pragma comment (linker, "/export:MmForceSectionClosed=ntoskrnl.MmForceSectionClosed")
#pragma comment (linker, "/export:MmFreeContiguousMemory=ntoskrnl.MmFreeContiguousMemory")
#pragma comment (linker, "/export:MmFreeContiguousMemorySpecifyCache=ntoskrnl.MmFreeContiguousMemorySpecifyCache")
#pragma comment (linker, "/export:MmFreeMappingAddress=ntoskrnl.MmFreeMappingAddress")
#pragma comment (linker, "/export:MmFreeNonCachedMemory=ntoskrnl.MmFreeNonCachedMemory")
#pragma comment (linker, "/export:MmFreePagesFromMdl=ntoskrnl.MmFreePagesFromMdl")
#pragma comment (linker, "/export:MmGetPhysicalAddress=ntoskrnl.MmGetPhysicalAddress")
#pragma comment (linker, "/export:MmGetPhysicalMemoryRanges=ntoskrnl.MmGetPhysicalMemoryRanges")
#pragma comment (linker, "/export:MmGetVirtualForPhysical=ntoskrnl.MmGetVirtualForPhysical")
#pragma comment (linker, "/export:MmGrowKernelStack=ntoskrnl.MmGrowKernelStack")
#pragma comment (linker, "/export:MmHighestUserAddress=ntoskrnl.MmHighestUserAddress")
#pragma comment (linker, "/export:MmIsAddressValid=ntoskrnl.MmIsAddressValid")
#pragma comment (linker, "/export:MmIsDriverVerifying=ntoskrnl.MmIsDriverVerifying")
#pragma comment (linker, "/export:MmIsNonPagedSystemAddressValid=ntoskrnl.MmIsNonPagedSystemAddressValid")
#pragma comment (linker, "/export:MmIsRecursiveIoFault=ntoskrnl.MmIsRecursiveIoFault")
#pragma comment (linker, "/export:MmIsThisAnNtAsSystem=ntoskrnl.MmIsThisAnNtAsSystem")
#pragma comment (linker, "/export:MmIsVerifierEnabled=ntoskrnl.MmIsVerifierEnabled")
#pragma comment (linker, "/export:MmLockPagableDataSection=ntoskrnl.MmLockPagableDataSection")
#pragma comment (linker, "/export:MmLockPagableImageSection=ntoskrnl.MmLockPagableImageSection")
#pragma comment (linker, "/export:MmLockPagableSectionByHandle=ntoskrnl.MmLockPagableSectionByHandle")
#pragma comment (linker, "/export:MmMapIoSpace=ntoskrnl.MmMapIoSpace")
#pragma comment (linker, "/export:MmMapLockedPages=ntoskrnl.MmMapLockedPages")
#pragma comment (linker, "/export:MmMapLockedPagesSpecifyCache=ntoskrnl.MmMapLockedPagesSpecifyCache")
#pragma comment (linker, "/export:MmMapLockedPagesWithReservedMapping=ntoskrnl.MmMapLockedPagesWithReservedMapping")
#pragma comment (linker, "/export:MmMapMemoryDumpMdl=ntoskrnl.MmMapMemoryDumpMdl")
#pragma comment (linker, "/export:MmMapUserAddressesToPage=ntoskrnl.MmMapUserAddressesToPage")
#pragma comment (linker, "/export:MmMapVideoDisplay=ntoskrnl.MmMapVideoDisplay")
#pragma comment (linker, "/export:MmMapViewInSessionSpace=ntoskrnl.MmMapViewInSessionSpace")
#pragma comment (linker, "/export:MmMapViewInSystemSpace=ntoskrnl.MmMapViewInSystemSpace")
#pragma comment (linker, "/export:MmMapViewOfSection=ntoskrnl.MmMapViewOfSection")
#pragma comment (linker, "/export:MmMarkPhysicalMemoryAsBad=ntoskrnl.MmMarkPhysicalMemoryAsBad")
#pragma comment (linker, "/export:MmMarkPhysicalMemoryAsGood=ntoskrnl.MmMarkPhysicalMemoryAsGood")
#pragma comment (linker, "/export:MmPageEntireDriver=ntoskrnl.MmPageEntireDriver")
#pragma comment (linker, "/export:MmPrefetchPages=ntoskrnl.MmPrefetchPages")
#pragma comment (linker, "/export:MmProbeAndLockPages=ntoskrnl.MmProbeAndLockPages")
#pragma comment (linker, "/export:MmProbeAndLockProcessPages=ntoskrnl.MmProbeAndLockProcessPages")
#pragma comment (linker, "/export:MmProbeAndLockSelectedPages=ntoskrnl.MmProbeAndLockSelectedPages")
#pragma comment (linker, "/export:MmProtectMdlSystemAddress=ntoskrnl.MmProtectMdlSystemAddress")
#pragma comment (linker, "/export:MmQuerySystemSize=ntoskrnl.MmQuerySystemSize")
#pragma comment (linker, "/export:MmRemovePhysicalMemory=ntoskrnl.MmRemovePhysicalMemory")
#pragma comment (linker, "/export:MmResetDriverPaging=ntoskrnl.MmResetDriverPaging")
#pragma comment (linker, "/export:MmSectionObjectType=ntoskrnl.MmSectionObjectType")
#pragma comment (linker, "/export:MmSecureVirtualMemory=ntoskrnl.MmSecureVirtualMemory")
#pragma comment (linker, "/export:MmSetAddressRangeModified=ntoskrnl.MmSetAddressRangeModified")
#pragma comment (linker, "/export:MmSetBankedSection=ntoskrnl.MmSetBankedSection")
#pragma comment (linker, "/export:MmSizeOfMdl=ntoskrnl.MmSizeOfMdl")
#pragma comment (linker, "/export:MmSystemRangeStart=ntoskrnl.MmSystemRangeStart")
#pragma comment (linker, "/export:MmTrimAllSystemPagableMemory=ntoskrnl.MmTrimAllSystemPagableMemory")
#pragma comment (linker, "/export:MmUnlockPagableImageSection=ntoskrnl.MmUnlockPagableImageSection")
#pragma comment (linker, "/export:MmUnlockPages=ntoskrnl.MmUnlockPages")
#pragma comment (linker, "/export:MmUnmapIoSpace=ntoskrnl.MmUnmapIoSpace")
#pragma comment (linker, "/export:MmUnmapLockedPages=ntoskrnl.MmUnmapLockedPages")
#pragma comment (linker, "/export:MmUnmapReservedMapping=ntoskrnl.MmUnmapReservedMapping")
#pragma comment (linker, "/export:MmUnmapVideoDisplay=ntoskrnl.MmUnmapVideoDisplay")
#pragma comment (linker, "/export:MmUnmapViewInSessionSpace=ntoskrnl.MmUnmapViewInSessionSpace")
#pragma comment (linker, "/export:MmUnmapViewInSystemSpace=ntoskrnl.MmUnmapViewInSystemSpace")
#pragma comment (linker, "/export:MmUnmapViewOfSection=ntoskrnl.MmUnmapViewOfSection")
#pragma comment (linker, "/export:MmUnsecureVirtualMemory=ntoskrnl.MmUnsecureVirtualMemory")
#pragma comment (linker, "/export:MmUserProbeAddress=ntoskrnl.MmUserProbeAddress")
#pragma comment (linker, "/export:NlsAnsiCodePage=ntoskrnl.NlsAnsiCodePage")
#pragma comment (linker, "/export:NlsLeadByteInfo=ntoskrnl.NlsLeadByteInfo")
#pragma comment (linker, "/export:NlsMbCodePageTag=ntoskrnl.NlsMbCodePageTag")
#pragma comment (linker, "/export:NlsMbOemCodePageTag=ntoskrnl.NlsMbOemCodePageTag")
#pragma comment (linker, "/export:NlsOemCodePage=ntoskrnl.NlsOemCodePage")
#pragma comment (linker, "/export:NlsOemLeadByteInfo=ntoskrnl.NlsOemLeadByteInfo")
#pragma comment (linker, "/export:NtAddAtom=ntoskrnl.NtAddAtom")
#pragma comment (linker, "/export:NtAdjustPrivilegesToken=ntoskrnl.NtAdjustPrivilegesToken")
#pragma comment (linker, "/export:NtAllocateLocallyUniqueId=ntoskrnl.NtAllocateLocallyUniqueId")
#pragma comment (linker, "/export:NtAllocateUuids=ntoskrnl.NtAllocateUuids")
#pragma comment (linker, "/export:NtAllocateVirtualMemory=ntoskrnl.NtAllocateVirtualMemory")
#pragma comment (linker, "/export:NtBuildNumber=ntoskrnl.NtBuildNumber")
#pragma comment (linker, "/export:NtClose=ntoskrnl.NtClose")
#pragma comment (linker, "/export:NtConnectPort=ntoskrnl.NtConnectPort")
#pragma comment (linker, "/export:NtCreateEvent=ntoskrnl.NtCreateEvent")
#pragma comment (linker, "/export:NtCreateFile=ntoskrnl.NtCreateFile")
#pragma comment (linker, "/export:NtCreateSection=ntoskrnl.NtCreateSection")
#pragma comment (linker, "/export:NtDeleteAtom=ntoskrnl.NtDeleteAtom")
#pragma comment (linker, "/export:NtDeleteFile=ntoskrnl.NtDeleteFile")
#pragma comment (linker, "/export:NtDeviceIoControlFile=ntoskrnl.NtDeviceIoControlFile")
#pragma comment (linker, "/export:NtDuplicateObject=ntoskrnl.NtDuplicateObject")
#pragma comment (linker, "/export:NtDuplicateToken=ntoskrnl.NtDuplicateToken")
#pragma comment (linker, "/export:NtFindAtom=ntoskrnl.NtFindAtom")
#pragma comment (linker, "/export:NtFreeVirtualMemory=ntoskrnl.NtFreeVirtualMemory")
#pragma comment (linker, "/export:NtFsControlFile=ntoskrnl.NtFsControlFile")
#pragma comment (linker, "/export:NtGlobalFlag=ntoskrnl.NtGlobalFlag")
#pragma comment (linker, "/export:NtLockFile=ntoskrnl.NtLockFile")
#pragma comment (linker, "/export:NtMakePermanentObject=ntoskrnl.NtMakePermanentObject")
#pragma comment (linker, "/export:NtMapViewOfSection=ntoskrnl.NtMapViewOfSection")
#pragma comment (linker, "/export:NtNotifyChangeDirectoryFile=ntoskrnl.NtNotifyChangeDirectoryFile")
#pragma comment (linker, "/export:NtOpenFile=ntoskrnl.NtOpenFile")
#pragma comment (linker, "/export:NtOpenProcess=ntoskrnl.NtOpenProcess")
#pragma comment (linker, "/export:NtOpenProcessToken=ntoskrnl.NtOpenProcessToken")
#pragma comment (linker, "/export:NtOpenProcessTokenEx=ntoskrnl.NtOpenProcessTokenEx")
#pragma comment (linker, "/export:NtOpenThread=ntoskrnl.NtOpenThread")
#pragma comment (linker, "/export:NtOpenThreadToken=ntoskrnl.NtOpenThreadToken")
#pragma comment (linker, "/export:NtOpenThreadTokenEx=ntoskrnl.NtOpenThreadTokenEx")
#pragma comment (linker, "/export:NtQueryDirectoryFile=ntoskrnl.NtQueryDirectoryFile")
#pragma comment (linker, "/export:NtQueryEaFile=ntoskrnl.NtQueryEaFile")
#pragma comment (linker, "/export:NtQueryInformationAtom=ntoskrnl.NtQueryInformationAtom")
#pragma comment (linker, "/export:NtQueryInformationFile=ntoskrnl.NtQueryInformationFile")
#pragma comment (linker, "/export:NtQueryInformationProcess=ntoskrnl.NtQueryInformationProcess")
#pragma comment (linker, "/export:NtQueryInformationThread=ntoskrnl.NtQueryInformationThread")
#pragma comment (linker, "/export:NtQueryInformationToken=ntoskrnl.NtQueryInformationToken")
#pragma comment (linker, "/export:NtQueryQuotaInformationFile=ntoskrnl.NtQueryQuotaInformationFile")
#pragma comment (linker, "/export:NtQuerySecurityObject=ntoskrnl.NtQuerySecurityObject")
#pragma comment (linker, "/export:NtQuerySystemInformation=ntoskrnl.NtQuerySystemInformation")
#pragma comment (linker, "/export:NtQueryVolumeInformationFile=ntoskrnl.NtQueryVolumeInformationFile")
#pragma comment (linker, "/export:NtReadFile=ntoskrnl.NtReadFile")
#pragma comment (linker, "/export:NtRequestPort=ntoskrnl.NtRequestPort")
#pragma comment (linker, "/export:NtRequestWaitReplyPort=ntoskrnl.NtRequestWaitReplyPort")
#pragma comment (linker, "/export:NtSetEaFile=ntoskrnl.NtSetEaFile")
#pragma comment (linker, "/export:NtSetEvent=ntoskrnl.NtSetEvent")
#pragma comment (linker, "/export:NtSetInformationFile=ntoskrnl.NtSetInformationFile")
#pragma comment (linker, "/export:NtSetInformationProcess=ntoskrnl.NtSetInformationProcess")
#pragma comment (linker, "/export:NtSetInformationThread=ntoskrnl.NtSetInformationThread")
#pragma comment (linker, "/export:NtSetQuotaInformationFile=ntoskrnl.NtSetQuotaInformationFile")
#pragma comment (linker, "/export:NtSetSecurityObject=ntoskrnl.NtSetSecurityObject")
#pragma comment (linker, "/export:NtSetVolumeInformationFile=ntoskrnl.NtSetVolumeInformationFile")
#pragma comment (linker, "/export:NtShutdownSystem=ntoskrnl.NtShutdownSystem")
#pragma comment (linker, "/export:NtTraceEvent=ntoskrnl.NtTraceEvent")
#pragma comment (linker, "/export:NtUnlockFile=ntoskrnl.NtUnlockFile")
#pragma comment (linker, "/export:NtVdmControl=ntoskrnl.NtVdmControl")
#pragma comment (linker, "/export:NtWaitForSingleObject=ntoskrnl.NtWaitForSingleObject")
#pragma comment (linker, "/export:NtWriteFile=ntoskrnl.NtWriteFile")
#pragma comment (linker, "/export:ObAssignSecurity=ntoskrnl.ObAssignSecurity")
#pragma comment (linker, "/export:ObCheckCreateObjectAccess=ntoskrnl.ObCheckCreateObjectAccess")
#pragma comment (linker, "/export:ObCheckObjectAccess=ntoskrnl.ObCheckObjectAccess")
#pragma comment (linker, "/export:ObCloseHandle=ntoskrnl.ObCloseHandle")
#pragma comment (linker, "/export:ObCreateObject=ntoskrnl.ObCreateObject")
#pragma comment (linker, "/export:ObCreateObjectType=ntoskrnl.ObCreateObjectType")
#pragma comment (linker, "/export:ObDereferenceObject=ntoskrnl.ObDereferenceObject")
#pragma comment (linker, "/export:ObDereferenceSecurityDescriptor=ntoskrnl.ObDereferenceSecurityDescriptor")
#pragma comment (linker, "/export:ObfDereferenceObject=ntoskrnl.ObfDereferenceObject")
#pragma comment (linker, "/export:ObFindHandleForObject=ntoskrnl.ObFindHandleForObject")
#pragma comment (linker, "/export:ObfReferenceObject=ntoskrnl.ObfReferenceObject")
#pragma comment (linker, "/export:ObGetObjectSecurity=ntoskrnl.ObGetObjectSecurity")
#pragma comment (linker, "/export:ObInsertObject=ntoskrnl.ObInsertObject")
#pragma comment (linker, "/export:ObLogSecurityDescriptor=ntoskrnl.ObLogSecurityDescriptor")
#pragma comment (linker, "/export:ObMakeTemporaryObject=ntoskrnl.ObMakeTemporaryObject")
#pragma comment (linker, "/export:ObOpenObjectByName=ntoskrnl.ObOpenObjectByName")
#pragma comment (linker, "/export:ObOpenObjectByPointer=ntoskrnl.ObOpenObjectByPointer")
#pragma comment (linker, "/export:ObQueryNameString=ntoskrnl.ObQueryNameString")
#pragma comment (linker, "/export:ObQueryObjectAuditingByHandle=ntoskrnl.ObQueryObjectAuditingByHandle")
#pragma comment (linker, "/export:ObReferenceObjectByHandle=ntoskrnl.ObReferenceObjectByHandle")
#pragma comment (linker, "/export:ObReferenceObjectByName=ntoskrnl.ObReferenceObjectByName")
#pragma comment (linker, "/export:ObReferenceObjectByPointer=ntoskrnl.ObReferenceObjectByPointer")
#pragma comment (linker, "/export:ObReferenceSecurityDescriptor=ntoskrnl.ObReferenceSecurityDescriptor")
#pragma comment (linker, "/export:ObReleaseObjectSecurity=ntoskrnl.ObReleaseObjectSecurity")
#pragma comment (linker, "/export:ObSetHandleAttributes=ntoskrnl.ObSetHandleAttributes")
#pragma comment (linker, "/export:ObSetSecurityDescriptorInfo=ntoskrnl.ObSetSecurityDescriptorInfo")
#pragma comment (linker, "/export:ObSetSecurityObjectByPointer=ntoskrnl.ObSetSecurityObjectByPointer")
#pragma comment (linker, "/export:PfxFindPrefix=ntoskrnl.PfxFindPrefix")
#pragma comment (linker, "/export:PfxInitialize=ntoskrnl.PfxInitialize")
#pragma comment (linker, "/export:PfxInsertPrefix=ntoskrnl.PfxInsertPrefix")
#pragma comment (linker, "/export:PfxRemovePrefix=ntoskrnl.PfxRemovePrefix")
#pragma comment (linker, "/export:PoCallDriver=ntoskrnl.PoCallDriver")
#pragma comment (linker, "/export:PoCancelDeviceNotify=ntoskrnl.PoCancelDeviceNotify")
#pragma comment (linker, "/export:PoQueueShutdownWorkItem=ntoskrnl.PoQueueShutdownWorkItem")
#pragma comment (linker, "/export:PoRegisterDeviceForIdleDetection=ntoskrnl.PoRegisterDeviceForIdleDetection")
#pragma comment (linker, "/export:PoRegisterDeviceNotify=ntoskrnl.PoRegisterDeviceNotify")
#pragma comment (linker, "/export:PoRegisterSystemState=ntoskrnl.PoRegisterSystemState")
#pragma comment (linker, "/export:PoRequestPowerIrp=ntoskrnl.PoRequestPowerIrp")
#pragma comment (linker, "/export:PoRequestShutdownEvent=ntoskrnl.PoRequestShutdownEvent")
#pragma comment (linker, "/export:PoSetHiberRange=ntoskrnl.PoSetHiberRange")
#pragma comment (linker, "/export:PoSetPowerState=ntoskrnl.PoSetPowerState")
#pragma comment (linker, "/export:PoSetSystemState=ntoskrnl.PoSetSystemState")
#pragma comment (linker, "/export:PoShutdownBugCheck=ntoskrnl.PoShutdownBugCheck")
#pragma comment (linker, "/export:PoStartNextPowerIrp=ntoskrnl.PoStartNextPowerIrp")
#pragma comment (linker, "/export:PoUnregisterSystemState=ntoskrnl.PoUnregisterSystemState")
#pragma comment (linker, "/export:ProbeForRead=ntoskrnl.ProbeForRead")
#pragma comment (linker, "/export:ProbeForWrite=ntoskrnl.ProbeForWrite")
#pragma comment (linker, "/export:PsAssignImpersonationToken=ntoskrnl.PsAssignImpersonationToken")
#pragma comment (linker, "/export:PsChargePoolQuota=ntoskrnl.PsChargePoolQuota")
#pragma comment (linker, "/export:PsChargeProcessNonPagedPoolQuota=ntoskrnl.PsChargeProcessNonPagedPoolQuota")
#pragma comment (linker, "/export:PsChargeProcessPagedPoolQuota=ntoskrnl.PsChargeProcessPagedPoolQuota")
#pragma comment (linker, "/export:PsChargeProcessPoolQuota=ntoskrnl.PsChargeProcessPoolQuota")
#pragma comment (linker, "/export:PsCreateSystemProcess=ntoskrnl.PsCreateSystemProcess")
#pragma comment (linker, "/export:PsCreateSystemThread=ntoskrnl.PsCreateSystemThread")
#pragma comment (linker, "/export:PsDereferenceImpersonationToken=ntoskrnl.PsDereferenceImpersonationToken")
#pragma comment (linker, "/export:PsDereferencePrimaryToken=ntoskrnl.PsDereferencePrimaryToken")
#pragma comment (linker, "/export:PsDisableImpersonation=ntoskrnl.PsDisableImpersonation")
#pragma comment (linker, "/export:PsEstablishWin32Callouts=ntoskrnl.PsEstablishWin32Callouts")
#pragma comment (linker, "/export:PsGetCurrentProcess=ntoskrnl.PsGetCurrentProcess")
#pragma comment (linker, "/export:PsGetCurrentProcessId=ntoskrnl.PsGetCurrentProcessId")
#pragma comment (linker, "/export:PsGetCurrentProcessSessionId=ntoskrnl.PsGetCurrentProcessSessionId")
#pragma comment (linker, "/export:PsGetCurrentThread=ntoskrnl.PsGetCurrentThread")
#pragma comment (linker, "/export:PsGetCurrentThreadId=ntoskrnl.PsGetCurrentThreadId")
#pragma comment (linker, "/export:PsGetCurrentThreadPreviousMode=ntoskrnl.PsGetCurrentThreadPreviousMode")
#pragma comment (linker, "/export:PsGetCurrentThreadStackBase=ntoskrnl.PsGetCurrentThreadStackBase")
#pragma comment (linker, "/export:PsGetCurrentThreadStackLimit=ntoskrnl.PsGetCurrentThreadStackLimit")
#pragma comment (linker, "/export:PsGetJobLock=ntoskrnl.PsGetJobLock")
#pragma comment (linker, "/export:PsGetJobSessionId=ntoskrnl.PsGetJobSessionId")
#pragma comment (linker, "/export:PsGetJobUIRestrictionsClass=ntoskrnl.PsGetJobUIRestrictionsClass")
#pragma comment (linker, "/export:PsGetProcessCreateTimeQuadPart=ntoskrnl.PsGetProcessCreateTimeQuadPart")
#pragma comment (linker, "/export:PsGetProcessDebugPort=ntoskrnl.PsGetProcessDebugPort")
#pragma comment (linker, "/export:PsGetProcessExitProcessCalled=ntoskrnl.PsGetProcessExitProcessCalled")
#pragma comment (linker, "/export:PsGetProcessExitStatus=ntoskrnl.PsGetProcessExitStatus")
#pragma comment (linker, "/export:PsGetProcessExitTime=ntoskrnl.PsGetProcessExitTime")
#pragma comment (linker, "/export:PsGetProcessId=ntoskrnl.PsGetProcessId")
#pragma comment (linker, "/export:PsGetProcessImageFileName=ntoskrnl.PsGetProcessImageFileName")
#pragma comment (linker, "/export:PsGetProcessInheritedFromUniqueProcessId=ntoskrnl.PsGetProcessInheritedFromUniqueProcessId")
#pragma comment (linker, "/export:PsGetProcessJob=ntoskrnl.PsGetProcessJob")
#pragma comment (linker, "/export:PsGetProcessPeb=ntoskrnl.PsGetProcessPeb")
#pragma comment (linker, "/export:PsGetProcessPriorityClass=ntoskrnl.PsGetProcessPriorityClass")
#pragma comment (linker, "/export:PsGetProcessSectionBaseAddress=ntoskrnl.PsGetProcessSectionBaseAddress")
#pragma comment (linker, "/export:PsGetProcessSecurityPort=ntoskrnl.PsGetProcessSecurityPort")
#pragma comment (linker, "/export:PsGetProcessSessionId=ntoskrnl.PsGetProcessSessionId")
#pragma comment (linker, "/export:PsGetProcessWin32Process=ntoskrnl.PsGetProcessWin32Process")
#pragma comment (linker, "/export:PsGetProcessWin32WindowStation=ntoskrnl.PsGetProcessWin32WindowStation")
#pragma comment (linker, "/export:PsGetThreadFreezeCount=ntoskrnl.PsGetThreadFreezeCount")
#pragma comment (linker, "/export:PsGetThreadHardErrorsAreDisabled=ntoskrnl.PsGetThreadHardErrorsAreDisabled")
#pragma comment (linker, "/export:PsGetThreadId=ntoskrnl.PsGetThreadId")
#pragma comment (linker, "/export:PsGetThreadProcess=ntoskrnl.PsGetThreadProcess")
#pragma comment (linker, "/export:PsGetThreadProcessId=ntoskrnl.PsGetThreadProcessId")
#pragma comment (linker, "/export:PsGetThreadSessionId=ntoskrnl.PsGetThreadSessionId")
#pragma comment (linker, "/export:PsGetThreadTeb=ntoskrnl.PsGetThreadTeb")
#pragma comment (linker, "/export:PsGetThreadWin32Thread=ntoskrnl.PsGetThreadWin32Thread")
#pragma comment (linker, "/export:PsGetVersion=ntoskrnl.PsGetVersion")
#pragma comment (linker, "/export:PsImpersonateClient=ntoskrnl.PsImpersonateClient")
#pragma comment (linker, "/export:PsInitialSystemProcess=ntoskrnl.PsInitialSystemProcess")
#pragma comment (linker, "/export:PsIsProcessBeingDebugged=ntoskrnl.PsIsProcessBeingDebugged")
#pragma comment (linker, "/export:PsIsSystemThread=ntoskrnl.PsIsSystemThread")
#pragma comment (linker, "/export:PsIsThreadImpersonating=ntoskrnl.PsIsThreadImpersonating")
#pragma comment (linker, "/export:PsIsThreadTerminating=ntoskrnl.PsIsThreadTerminating")
#pragma comment (linker, "/export:PsJobType=ntoskrnl.PsJobType")
#pragma comment (linker, "/export:PsLookupProcessByProcessId=ntoskrnl.PsLookupProcessByProcessId")
#pragma comment (linker, "/export:PsLookupProcessThreadByCid=ntoskrnl.PsLookupProcessThreadByCid")
#pragma comment (linker, "/export:PsLookupThreadByThreadId=ntoskrnl.PsLookupThreadByThreadId")
#pragma comment (linker, "/export:PsProcessType=ntoskrnl.PsProcessType")
#pragma comment (linker, "/export:PsReferenceImpersonationToken=ntoskrnl.PsReferenceImpersonationToken")
#pragma comment (linker, "/export:PsReferencePrimaryToken=ntoskrnl.PsReferencePrimaryToken")
#pragma comment (linker, "/export:PsRemoveCreateThreadNotifyRoutine=ntoskrnl.PsRemoveCreateThreadNotifyRoutine")
#pragma comment (linker, "/export:PsRemoveLoadImageNotifyRoutine=ntoskrnl.PsRemoveLoadImageNotifyRoutine")
#pragma comment (linker, "/export:PsRestoreImpersonation=ntoskrnl.PsRestoreImpersonation")
#pragma comment (linker, "/export:PsReturnPoolQuota=ntoskrnl.PsReturnPoolQuota")
#pragma comment (linker, "/export:PsReturnProcessNonPagedPoolQuota=ntoskrnl.PsReturnProcessNonPagedPoolQuota")
#pragma comment (linker, "/export:PsReturnProcessPagedPoolQuota=ntoskrnl.PsReturnProcessPagedPoolQuota")
#pragma comment (linker, "/export:PsRevertThreadToSelf=ntoskrnl.PsRevertThreadToSelf")
#pragma comment (linker, "/export:PsRevertToSelf=ntoskrnl.PsRevertToSelf")
#pragma comment (linker, "/export:PsSetCreateProcessNotifyRoutine=ntoskrnl.PsSetCreateProcessNotifyRoutine")
#pragma comment (linker, "/export:PsSetCreateThreadNotifyRoutine=ntoskrnl.PsSetCreateThreadNotifyRoutine")
#pragma comment (linker, "/export:PsSetJobUIRestrictionsClass=ntoskrnl.PsSetJobUIRestrictionsClass")
#pragma comment (linker, "/export:PsSetLegoNotifyRoutine=ntoskrnl.PsSetLegoNotifyRoutine")
#pragma comment (linker, "/export:PsSetLoadImageNotifyRoutine=ntoskrnl.PsSetLoadImageNotifyRoutine")
#pragma comment (linker, "/export:PsSetProcessPriorityByClass=ntoskrnl.PsSetProcessPriorityByClass")
#pragma comment (linker, "/export:PsSetProcessPriorityClass=ntoskrnl.PsSetProcessPriorityClass")
#pragma comment (linker, "/export:PsSetProcessSecurityPort=ntoskrnl.PsSetProcessSecurityPort")
#pragma comment (linker, "/export:PsSetProcessWin32Process=ntoskrnl.PsSetProcessWin32Process")
#pragma comment (linker, "/export:PsSetProcessWindowStation=ntoskrnl.PsSetProcessWindowStation")
#pragma comment (linker, "/export:PsSetThreadHardErrorsAreDisabled=ntoskrnl.PsSetThreadHardErrorsAreDisabled")
#pragma comment (linker, "/export:PsSetThreadWin32Thread=ntoskrnl.PsSetThreadWin32Thread")
#pragma comment (linker, "/export:PsTerminateSystemThread=ntoskrnl.PsTerminateSystemThread")
#pragma comment (linker, "/export:PsThreadType=ntoskrnl.PsThreadType")
#pragma comment (linker, "/export:qsort=ntoskrnl.qsort")
#pragma comment (linker, "/export:rand=ntoskrnl.rand")
#pragma comment (linker, "/export:READ_REGISTER_BUFFER_UCHAR=ntoskrnl.READ_REGISTER_BUFFER_UCHAR")
#pragma comment (linker, "/export:READ_REGISTER_BUFFER_ULONG=ntoskrnl.READ_REGISTER_BUFFER_ULONG")
#pragma comment (linker, "/export:READ_REGISTER_BUFFER_USHORT=ntoskrnl.READ_REGISTER_BUFFER_USHORT")
#pragma comment (linker, "/export:READ_REGISTER_UCHAR=ntoskrnl.READ_REGISTER_UCHAR")
#pragma comment (linker, "/export:READ_REGISTER_ULONG=ntoskrnl.READ_REGISTER_ULONG")
#pragma comment (linker, "/export:READ_REGISTER_USHORT=ntoskrnl.READ_REGISTER_USHORT")
#pragma comment (linker, "/export:RtlAbsoluteToSelfRelativeSD=ntoskrnl.RtlAbsoluteToSelfRelativeSD")
#pragma comment (linker, "/export:RtlAddAccessAllowedAce=ntoskrnl.RtlAddAccessAllowedAce")
#pragma comment (linker, "/export:RtlAddAce=ntoskrnl.RtlAddAce")
#pragma comment (linker, "/export:RtlAddAtomToAtomTable=ntoskrnl.RtlAddAtomToAtomTable")
#pragma comment (linker, "/export:RtlAddRange=ntoskrnl.RtlAddRange")
#pragma comment (linker, "/export:RtlAllocateHeap=ntoskrnl.RtlAllocateHeap")
#pragma comment (linker, "/export:RtlAnsiCharToUnicodeChar=ntoskrnl.RtlAnsiCharToUnicodeChar")
#pragma comment (linker, "/export:RtlAnsiStringToUnicodeSize=ntoskrnl.RtlAnsiStringToUnicodeSize")
#pragma comment (linker, "/export:RtlAnsiStringToUnicodeString=ntoskrnl.RtlAnsiStringToUnicodeString")
#pragma comment (linker, "/export:RtlAppendAsciizToString=ntoskrnl.RtlAppendAsciizToString")
#pragma comment (linker, "/export:RtlAppendStringToString=ntoskrnl.RtlAppendStringToString")
#pragma comment (linker, "/export:RtlAppendUnicodeStringToString=ntoskrnl.RtlAppendUnicodeStringToString")
#pragma comment (linker, "/export:RtlAppendUnicodeToString=ntoskrnl.RtlAppendUnicodeToString")
#pragma comment (linker, "/export:RtlAreAllAccessesGranted=ntoskrnl.RtlAreAllAccessesGranted")
#pragma comment (linker, "/export:RtlAreAnyAccessesGranted=ntoskrnl.RtlAreAnyAccessesGranted")
#pragma comment (linker, "/export:RtlAreBitsClear=ntoskrnl.RtlAreBitsClear")
#pragma comment (linker, "/export:RtlAreBitsSet=ntoskrnl.RtlAreBitsSet")
#pragma comment (linker, "/export:RtlAssert=ntoskrnl.RtlAssert")
#pragma comment (linker, "/export:RtlCaptureContext=ntoskrnl.RtlCaptureContext")
#pragma comment (linker, "/export:RtlCaptureStackBackTrace=ntoskrnl.RtlCaptureStackBackTrace")
#pragma comment (linker, "/export:RtlCharToInteger=ntoskrnl.RtlCharToInteger")
#pragma comment (linker, "/export:RtlCheckRegistryKey=ntoskrnl.RtlCheckRegistryKey")
#pragma comment (linker, "/export:RtlClearAllBits=ntoskrnl.RtlClearAllBits")
#pragma comment (linker, "/export:RtlClearBit=ntoskrnl.RtlClearBit")
#pragma comment (linker, "/export:RtlClearBits=ntoskrnl.RtlClearBits")
#pragma comment (linker, "/export:RtlCompareMemory=ntoskrnl.RtlCompareMemory")
#pragma comment (linker, "/export:RtlCompareMemoryUlong=ntoskrnl.RtlCompareMemoryUlong")
#pragma comment (linker, "/export:RtlCompareString=ntoskrnl.RtlCompareString")
#pragma comment (linker, "/export:RtlCompareUnicodeString=ntoskrnl.RtlCompareUnicodeString")
#pragma comment (linker, "/export:RtlCompressBuffer=ntoskrnl.RtlCompressBuffer")
#pragma comment (linker, "/export:RtlCompressChunks=ntoskrnl.RtlCompressChunks")
#pragma comment (linker, "/export:RtlConvertLongToLargeInteger=ntoskrnl.RtlConvertLongToLargeInteger")
#pragma comment (linker, "/export:RtlConvertSidToUnicodeString=ntoskrnl.RtlConvertSidToUnicodeString")
#pragma comment (linker, "/export:RtlConvertUlongToLargeInteger=ntoskrnl.RtlConvertUlongToLargeInteger")
#pragma comment (linker, "/export:RtlCopyLuid=ntoskrnl.RtlCopyLuid")
#pragma comment (linker, "/export:RtlCopyRangeList=ntoskrnl.RtlCopyRangeList")
#pragma comment (linker, "/export:RtlCopySid=ntoskrnl.RtlCopySid")
#pragma comment (linker, "/export:RtlCopyString=ntoskrnl.RtlCopyString")
#pragma comment (linker, "/export:RtlCopyUnicodeString=ntoskrnl.RtlCopyUnicodeString")
#pragma comment (linker, "/export:RtlCreateAcl=ntoskrnl.RtlCreateAcl")
#pragma comment (linker, "/export:RtlCreateAtomTable=ntoskrnl.RtlCreateAtomTable")
#pragma comment (linker, "/export:RtlCreateHeap=ntoskrnl.RtlCreateHeap")
#pragma comment (linker, "/export:RtlCreateRegistryKey=ntoskrnl.RtlCreateRegistryKey")
#pragma comment (linker, "/export:RtlCreateSecurityDescriptor=ntoskrnl.RtlCreateSecurityDescriptor")
#pragma comment (linker, "/export:RtlCreateSystemVolumeInformationFolder=ntoskrnl.RtlCreateSystemVolumeInformationFolder")
#pragma comment (linker, "/export:RtlCreateUnicodeString=ntoskrnl.RtlCreateUnicodeString")
#pragma comment (linker, "/export:RtlCustomCPToUnicodeN=ntoskrnl.RtlCustomCPToUnicodeN")
#pragma comment (linker, "/export:RtlDecompressBuffer=ntoskrnl.RtlDecompressBuffer")
#pragma comment (linker, "/export:RtlDecompressChunks=ntoskrnl.RtlDecompressChunks")
#pragma comment (linker, "/export:RtlDecompressFragment=ntoskrnl.RtlDecompressFragment")
#pragma comment (linker, "/export:RtlDelete=ntoskrnl.RtlDelete")
#pragma comment (linker, "/export:RtlDeleteAce=ntoskrnl.RtlDeleteAce")
#pragma comment (linker, "/export:RtlDeleteAtomFromAtomTable=ntoskrnl.RtlDeleteAtomFromAtomTable")
#pragma comment (linker, "/export:RtlDeleteElementGenericTable=ntoskrnl.RtlDeleteElementGenericTable")
#pragma comment (linker, "/export:RtlDeleteElementGenericTableAvl=ntoskrnl.RtlDeleteElementGenericTableAvl")
#pragma comment (linker, "/export:RtlDeleteNoSplay=ntoskrnl.RtlDeleteNoSplay")
#pragma comment (linker, "/export:RtlDeleteOwnersRanges=ntoskrnl.RtlDeleteOwnersRanges")
#pragma comment (linker, "/export:RtlDeleteRange=ntoskrnl.RtlDeleteRange")
#pragma comment (linker, "/export:RtlDeleteRegistryValue=ntoskrnl.RtlDeleteRegistryValue")
#pragma comment (linker, "/export:RtlDescribeChunk=ntoskrnl.RtlDescribeChunk")
#pragma comment (linker, "/export:RtlDestroyAtomTable=ntoskrnl.RtlDestroyAtomTable")
#pragma comment (linker, "/export:RtlDestroyHeap=ntoskrnl.RtlDestroyHeap")
#pragma comment (linker, "/export:RtlDowncaseUnicodeString=ntoskrnl.RtlDowncaseUnicodeString")
#pragma comment (linker, "/export:RtlEmptyAtomTable=ntoskrnl.RtlEmptyAtomTable")
#pragma comment (linker, "/export:RtlEnlargedIntegerMultiply=ntoskrnl.RtlEnlargedIntegerMultiply")
#pragma comment (linker, "/export:RtlEnlargedUnsignedDivide=ntoskrnl.RtlEnlargedUnsignedDivide")
#pragma comment (linker, "/export:RtlEnlargedUnsignedMultiply=ntoskrnl.RtlEnlargedUnsignedMultiply")
#pragma comment (linker, "/export:RtlEnumerateGenericTable=ntoskrnl.RtlEnumerateGenericTable")
#pragma comment (linker, "/export:RtlEnumerateGenericTableAvl=ntoskrnl.RtlEnumerateGenericTableAvl")
#pragma comment (linker, "/export:RtlEnumerateGenericTableLikeADirectory=ntoskrnl.RtlEnumerateGenericTableLikeADirectory")
#pragma comment (linker, "/export:RtlEnumerateGenericTableWithoutSplaying=ntoskrnl.RtlEnumerateGenericTableWithoutSplaying")
#pragma comment (linker, "/export:RtlEnumerateGenericTableWithoutSplayingAvl=ntoskrnl.RtlEnumerateGenericTableWithoutSplayingAvl")
#pragma comment (linker, "/export:RtlEqualLuid=ntoskrnl.RtlEqualLuid")
#pragma comment (linker, "/export:RtlEqualSid=ntoskrnl.RtlEqualSid")
#pragma comment (linker, "/export:RtlEqualString=ntoskrnl.RtlEqualString")
#pragma comment (linker, "/export:RtlEqualUnicodeString=ntoskrnl.RtlEqualUnicodeString")
#pragma comment (linker, "/export:RtlExtendedIntegerMultiply=ntoskrnl.RtlExtendedIntegerMultiply")
#pragma comment (linker, "/export:RtlExtendedLargeIntegerDivide=ntoskrnl.RtlExtendedLargeIntegerDivide")
#pragma comment (linker, "/export:RtlExtendedMagicDivide=ntoskrnl.RtlExtendedMagicDivide")
#pragma comment (linker, "/export:RtlFillMemory=ntoskrnl.RtlFillMemory")
#pragma comment (linker, "/export:RtlFillMemoryUlong=ntoskrnl.RtlFillMemoryUlong")
#pragma comment (linker, "/export:RtlFindClearBits=ntoskrnl.RtlFindClearBits")
#pragma comment (linker, "/export:RtlFindClearBitsAndSet=ntoskrnl.RtlFindClearBitsAndSet")
#pragma comment (linker, "/export:RtlFindClearRuns=ntoskrnl.RtlFindClearRuns")
#pragma comment (linker, "/export:RtlFindFirstRunClear=ntoskrnl.RtlFindFirstRunClear")
#pragma comment (linker, "/export:RtlFindLastBackwardRunClear=ntoskrnl.RtlFindLastBackwardRunClear")
#pragma comment (linker, "/export:RtlFindLeastSignificantBit=ntoskrnl.RtlFindLeastSignificantBit")
#pragma comment (linker, "/export:RtlFindLongestRunClear=ntoskrnl.RtlFindLongestRunClear")
#pragma comment (linker, "/export:RtlFindMessage=ntoskrnl.RtlFindMessage")
#pragma comment (linker, "/export:RtlFindMostSignificantBit=ntoskrnl.RtlFindMostSignificantBit")
#pragma comment (linker, "/export:RtlFindNextForwardRunClear=ntoskrnl.RtlFindNextForwardRunClear")
#pragma comment (linker, "/export:RtlFindRange=ntoskrnl.RtlFindRange")
#pragma comment (linker, "/export:RtlFindSetBits=ntoskrnl.RtlFindSetBits")
#pragma comment (linker, "/export:RtlFindSetBitsAndClear=ntoskrnl.RtlFindSetBitsAndClear")
#pragma comment (linker, "/export:RtlFindUnicodePrefix=ntoskrnl.RtlFindUnicodePrefix")
#pragma comment (linker, "/export:RtlFormatCurrentUserKeyPath=ntoskrnl.RtlFormatCurrentUserKeyPath")
#pragma comment (linker, "/export:RtlFreeAnsiString=ntoskrnl.RtlFreeAnsiString")
#pragma comment (linker, "/export:RtlFreeHeap=ntoskrnl.RtlFreeHeap")
#pragma comment (linker, "/export:RtlFreeOemString=ntoskrnl.RtlFreeOemString")
#pragma comment (linker, "/export:RtlFreeRangeList=ntoskrnl.RtlFreeRangeList")
#pragma comment (linker, "/export:RtlFreeUnicodeString=ntoskrnl.RtlFreeUnicodeString")
#pragma comment (linker, "/export:RtlGenerate8dot3Name=ntoskrnl.RtlGenerate8dot3Name")
#pragma comment (linker, "/export:RtlGetAce=ntoskrnl.RtlGetAce")
#pragma comment (linker, "/export:RtlGetCallersAddress=ntoskrnl.RtlGetCallersAddress")
#pragma comment (linker, "/export:RtlGetCompressionWorkSpaceSize=ntoskrnl.RtlGetCompressionWorkSpaceSize")
#pragma comment (linker, "/export:RtlGetDaclSecurityDescriptor=ntoskrnl.RtlGetDaclSecurityDescriptor")
#pragma comment (linker, "/export:RtlGetDefaultCodePage=ntoskrnl.RtlGetDefaultCodePage")
#pragma comment (linker, "/export:RtlGetElementGenericTable=ntoskrnl.RtlGetElementGenericTable")
#pragma comment (linker, "/export:RtlGetElementGenericTableAvl=ntoskrnl.RtlGetElementGenericTableAvl")
#pragma comment (linker, "/export:RtlGetFirstRange=ntoskrnl.RtlGetFirstRange")
#pragma comment (linker, "/export:RtlGetGroupSecurityDescriptor=ntoskrnl.RtlGetGroupSecurityDescriptor")
#pragma comment (linker, "/export:RtlGetNextRange=ntoskrnl.RtlGetNextRange")
#pragma comment (linker, "/export:RtlGetNtGlobalFlags=ntoskrnl.RtlGetNtGlobalFlags")
#pragma comment (linker, "/export:RtlGetOwnerSecurityDescriptor=ntoskrnl.RtlGetOwnerSecurityDescriptor")
#pragma comment (linker, "/export:RtlGetSaclSecurityDescriptor=ntoskrnl.RtlGetSaclSecurityDescriptor")
#pragma comment (linker, "/export:RtlGetSetBootStatusData=ntoskrnl.RtlGetSetBootStatusData")
#pragma comment (linker, "/export:RtlGetVersion=ntoskrnl.RtlGetVersion")
#pragma comment (linker, "/export:RtlGUIDFromString=ntoskrnl.RtlGUIDFromString")
#pragma comment (linker, "/export:RtlHashUnicodeString=ntoskrnl.RtlHashUnicodeString")
#pragma comment (linker, "/export:RtlImageDirectoryEntryToData=ntoskrnl.RtlImageDirectoryEntryToData")
#pragma comment (linker, "/export:RtlImageNtHeader=ntoskrnl.RtlImageNtHeader")
#pragma comment (linker, "/export:RtlInitAnsiString=ntoskrnl.RtlInitAnsiString")
#pragma comment (linker, "/export:RtlInitCodePageTable=ntoskrnl.RtlInitCodePageTable")
#pragma comment (linker, "/export:RtlInitializeBitMap=ntoskrnl.RtlInitializeBitMap")
#pragma comment (linker, "/export:RtlInitializeGenericTable=ntoskrnl.RtlInitializeGenericTable")
#pragma comment (linker, "/export:RtlInitializeGenericTableAvl=ntoskrnl.RtlInitializeGenericTableAvl")
#pragma comment (linker, "/export:RtlInitializeRangeList=ntoskrnl.RtlInitializeRangeList")
#pragma comment (linker, "/export:RtlInitializeSid=ntoskrnl.RtlInitializeSid")
#pragma comment (linker, "/export:RtlInitializeUnicodePrefix=ntoskrnl.RtlInitializeUnicodePrefix")
#pragma comment (linker, "/export:RtlInitString=ntoskrnl.RtlInitString")
#pragma comment (linker, "/export:RtlInitUnicodeString=ntoskrnl.RtlInitUnicodeString")
#pragma comment (linker, "/export:RtlInsertElementGenericTable=ntoskrnl.RtlInsertElementGenericTable")
#pragma comment (linker, "/export:RtlInsertElementGenericTableAvl=ntoskrnl.RtlInsertElementGenericTableAvl")
#pragma comment (linker, "/export:RtlInsertElementGenericTableFull=ntoskrnl.RtlInsertElementGenericTableFull")
#pragma comment (linker, "/export:RtlInsertElementGenericTableFullAvl=ntoskrnl.RtlInsertElementGenericTableFullAvl")
#pragma comment (linker, "/export:RtlInsertUnicodePrefix=ntoskrnl.RtlInsertUnicodePrefix")
#pragma comment (linker, "/export:RtlInt64ToUnicodeString=ntoskrnl.RtlInt64ToUnicodeString")
#pragma comment (linker, "/export:RtlIntegerToChar=ntoskrnl.RtlIntegerToChar")
#pragma comment (linker, "/export:RtlIntegerToUnicode=ntoskrnl.RtlIntegerToUnicode")
#pragma comment (linker, "/export:RtlIntegerToUnicodeString=ntoskrnl.RtlIntegerToUnicodeString")
#pragma comment (linker, "/export:RtlInvertRangeList=ntoskrnl.RtlInvertRangeList")
#pragma comment (linker, "/export:RtlIpv4AddressToStringA=ntoskrnl.RtlIpv4AddressToStringA")
#pragma comment (linker, "/export:RtlIpv4AddressToStringW=ntoskrnl.RtlIpv4AddressToStringW")
#pragma comment (linker, "/export:RtlIpv4StringToAddressA=ntoskrnl.RtlIpv4StringToAddressA")
#pragma comment (linker, "/export:RtlIpv4StringToAddressW=ntoskrnl.RtlIpv4StringToAddressW")
#pragma comment (linker, "/export:RtlIpv6AddressToStringA=ntoskrnl.RtlIpv6AddressToStringA")
#pragma comment (linker, "/export:RtlIpv6AddressToStringW=ntoskrnl.RtlIpv6AddressToStringW")
#pragma comment (linker, "/export:RtlIpv6StringToAddressA=ntoskrnl.RtlIpv6StringToAddressA")
#pragma comment (linker, "/export:RtlIpv6StringToAddressW=ntoskrnl.RtlIpv6StringToAddressW")
#pragma comment (linker, "/export:RtlIsGenericTableEmpty=ntoskrnl.RtlIsGenericTableEmpty")
#pragma comment (linker, "/export:RtlIsGenericTableEmptyAvl=ntoskrnl.RtlIsGenericTableEmptyAvl")
#pragma comment (linker, "/export:RtlIsNameLegalDOS8Dot3=ntoskrnl.RtlIsNameLegalDOS8Dot3")
#pragma comment (linker, "/export:RtlIsRangeAvailable=ntoskrnl.RtlIsRangeAvailable")
#pragma comment (linker, "/export:RtlIsValidOemCharacter=ntoskrnl.RtlIsValidOemCharacter")
#pragma comment (linker, "/export:RtlLargeIntegerAdd=ntoskrnl.RtlLargeIntegerAdd")
#pragma comment (linker, "/export:RtlLargeIntegerArithmeticShift=ntoskrnl.RtlLargeIntegerArithmeticShift")
#pragma comment (linker, "/export:RtlLargeIntegerDivide=ntoskrnl.RtlLargeIntegerDivide")
#pragma comment (linker, "/export:RtlLargeIntegerNegate=ntoskrnl.RtlLargeIntegerNegate")
#pragma comment (linker, "/export:RtlLargeIntegerShiftLeft=ntoskrnl.RtlLargeIntegerShiftLeft")
#pragma comment (linker, "/export:RtlLargeIntegerShiftRight=ntoskrnl.RtlLargeIntegerShiftRight")
#pragma comment (linker, "/export:RtlLargeIntegerSubtract=ntoskrnl.RtlLargeIntegerSubtract")
#pragma comment (linker, "/export:RtlLengthRequiredSid=ntoskrnl.RtlLengthRequiredSid")
#pragma comment (linker, "/export:RtlLengthSecurityDescriptor=ntoskrnl.RtlLengthSecurityDescriptor")
#pragma comment (linker, "/export:RtlLengthSid=ntoskrnl.RtlLengthSid")
#pragma comment (linker, "/export:RtlLockBootStatusData=ntoskrnl.RtlLockBootStatusData")
#pragma comment (linker, "/export:RtlLookupAtomInAtomTable=ntoskrnl.RtlLookupAtomInAtomTable")
#pragma comment (linker, "/export:RtlLookupElementGenericTable=ntoskrnl.RtlLookupElementGenericTable")
#pragma comment (linker, "/export:RtlLookupElementGenericTableAvl=ntoskrnl.RtlLookupElementGenericTableAvl")
#pragma comment (linker, "/export:RtlLookupElementGenericTableFull=ntoskrnl.RtlLookupElementGenericTableFull")
#pragma comment (linker, "/export:RtlLookupElementGenericTableFullAvl=ntoskrnl.RtlLookupElementGenericTableFullAvl")
#pragma comment (linker, "/export:RtlMapGenericMask=ntoskrnl.RtlMapGenericMask")
#pragma comment (linker, "/export:RtlMapSecurityErrorToNtStatus=ntoskrnl.RtlMapSecurityErrorToNtStatus")
#pragma comment (linker, "/export:RtlMergeRangeLists=ntoskrnl.RtlMergeRangeLists")
#pragma comment (linker, "/export:RtlMoveMemory=ntoskrnl.RtlMoveMemory")
#pragma comment (linker, "/export:RtlMultiByteToUnicodeN=ntoskrnl.RtlMultiByteToUnicodeN")
#pragma comment (linker, "/export:RtlMultiByteToUnicodeSize=ntoskrnl.RtlMultiByteToUnicodeSize")
#pragma comment (linker, "/export:RtlNextUnicodePrefix=ntoskrnl.RtlNextUnicodePrefix")
#pragma comment (linker, "/export:RtlNtStatusToDosError=ntoskrnl.RtlNtStatusToDosError")
#pragma comment (linker, "/export:RtlNtStatusToDosErrorNoTeb=ntoskrnl.RtlNtStatusToDosErrorNoTeb")
#pragma comment (linker, "/export:RtlNumberGenericTableElements=ntoskrnl.RtlNumberGenericTableElements")
#pragma comment (linker, "/export:RtlNumberGenericTableElementsAvl=ntoskrnl.RtlNumberGenericTableElementsAvl")
#pragma comment (linker, "/export:RtlNumberOfClearBits=ntoskrnl.RtlNumberOfClearBits")
#pragma comment (linker, "/export:RtlNumberOfSetBits=ntoskrnl.RtlNumberOfSetBits")
#pragma comment (linker, "/export:RtlOemStringToCountedUnicodeString=ntoskrnl.RtlOemStringToCountedUnicodeString")
#pragma comment (linker, "/export:RtlOemStringToUnicodeSize=ntoskrnl.RtlOemStringToUnicodeSize")
#pragma comment (linker, "/export:RtlOemStringToUnicodeString=ntoskrnl.RtlOemStringToUnicodeString")
#pragma comment (linker, "/export:RtlOemToUnicodeN=ntoskrnl.RtlOemToUnicodeN")
#pragma comment (linker, "/export:RtlPinAtomInAtomTable=ntoskrnl.RtlPinAtomInAtomTable")
#pragma comment (linker, "/export:RtlPrefetchMemoryNonTemporal=ntoskrnl.RtlPrefetchMemoryNonTemporal")
#pragma comment (linker, "/export:RtlPrefixString=ntoskrnl.RtlPrefixString")
#pragma comment (linker, "/export:RtlPrefixUnicodeString=ntoskrnl.RtlPrefixUnicodeString")
#pragma comment (linker, "/export:RtlQueryAtomInAtomTable=ntoskrnl.RtlQueryAtomInAtomTable")
#pragma comment (linker, "/export:RtlQueryRegistryValues=ntoskrnl.RtlQueryRegistryValues")
#pragma comment (linker, "/export:RtlQueryTimeZoneInformation=ntoskrnl.RtlQueryTimeZoneInformation")
#pragma comment (linker, "/export:RtlRaiseException=ntoskrnl.RtlRaiseException")
#pragma comment (linker, "/export:RtlRandom=ntoskrnl.RtlRandom")
#pragma comment (linker, "/export:RtlRandomEx=ntoskrnl.RtlRandomEx")
#pragma comment (linker, "/export:RtlRealPredecessor=ntoskrnl.RtlRealPredecessor")
#pragma comment (linker, "/export:RtlRealSuccessor=ntoskrnl.RtlRealSuccessor")
#pragma comment (linker, "/export:RtlRemoveUnicodePrefix=ntoskrnl.RtlRemoveUnicodePrefix")
#pragma comment (linker, "/export:RtlReserveChunk=ntoskrnl.RtlReserveChunk")
#pragma comment (linker, "/export:RtlSecondsSince1970ToTime=ntoskrnl.RtlSecondsSince1970ToTime")
#pragma comment (linker, "/export:RtlSecondsSince1980ToTime=ntoskrnl.RtlSecondsSince1980ToTime")
#pragma comment (linker, "/export:RtlSelfRelativeToAbsoluteSD=ntoskrnl.RtlSelfRelativeToAbsoluteSD")
#pragma comment (linker, "/export:RtlSelfRelativeToAbsoluteSD2=ntoskrnl.RtlSelfRelativeToAbsoluteSD2")
#pragma comment (linker, "/export:RtlSetAllBits=ntoskrnl.RtlSetAllBits")
#pragma comment (linker, "/export:RtlSetBit=ntoskrnl.RtlSetBit")
#pragma comment (linker, "/export:RtlSetBits=ntoskrnl.RtlSetBits")
#pragma comment (linker, "/export:RtlSetDaclSecurityDescriptor=ntoskrnl.RtlSetDaclSecurityDescriptor")
#pragma comment (linker, "/export:RtlSetGroupSecurityDescriptor=ntoskrnl.RtlSetGroupSecurityDescriptor")
#pragma comment (linker, "/export:RtlSetOwnerSecurityDescriptor=ntoskrnl.RtlSetOwnerSecurityDescriptor")
#pragma comment (linker, "/export:RtlSetSaclSecurityDescriptor=ntoskrnl.RtlSetSaclSecurityDescriptor")
#pragma comment (linker, "/export:RtlSetTimeZoneInformation=ntoskrnl.RtlSetTimeZoneInformation")
#pragma comment (linker, "/export:RtlSizeHeap=ntoskrnl.RtlSizeHeap")
#pragma comment (linker, "/export:RtlSplay=ntoskrnl.RtlSplay")
#pragma comment (linker, "/export:RtlStringFromGUID=ntoskrnl.RtlStringFromGUID")
#pragma comment (linker, "/export:RtlSubAuthorityCountSid=ntoskrnl.RtlSubAuthorityCountSid")
#pragma comment (linker, "/export:RtlSubAuthoritySid=ntoskrnl.RtlSubAuthoritySid")
#pragma comment (linker, "/export:RtlSubtreePredecessor=ntoskrnl.RtlSubtreePredecessor")
#pragma comment (linker, "/export:RtlSubtreeSuccessor=ntoskrnl.RtlSubtreeSuccessor")
#pragma comment (linker, "/export:RtlTestBit=ntoskrnl.RtlTestBit")
#pragma comment (linker, "/export:RtlTimeFieldsToTime=ntoskrnl.RtlTimeFieldsToTime")
#pragma comment (linker, "/export:RtlTimeToElapsedTimeFields=ntoskrnl.RtlTimeToElapsedTimeFields")
#pragma comment (linker, "/export:RtlTimeToSecondsSince1970=ntoskrnl.RtlTimeToSecondsSince1970")
#pragma comment (linker, "/export:RtlTimeToSecondsSince1980=ntoskrnl.RtlTimeToSecondsSince1980")
#pragma comment (linker, "/export:RtlTimeToTimeFields=ntoskrnl.RtlTimeToTimeFields")
#pragma comment (linker, "/export:RtlTraceDatabaseAdd=ntoskrnl.RtlTraceDatabaseAdd")
#pragma comment (linker, "/export:RtlTraceDatabaseCreate=ntoskrnl.RtlTraceDatabaseCreate")
#pragma comment (linker, "/export:RtlTraceDatabaseDestroy=ntoskrnl.RtlTraceDatabaseDestroy")
#pragma comment (linker, "/export:RtlTraceDatabaseEnumerate=ntoskrnl.RtlTraceDatabaseEnumerate")
#pragma comment (linker, "/export:RtlTraceDatabaseFind=ntoskrnl.RtlTraceDatabaseFind")
#pragma comment (linker, "/export:RtlTraceDatabaseLock=ntoskrnl.RtlTraceDatabaseLock")
#pragma comment (linker, "/export:RtlTraceDatabaseUnlock=ntoskrnl.RtlTraceDatabaseUnlock")
#pragma comment (linker, "/export:RtlTraceDatabaseValidate=ntoskrnl.RtlTraceDatabaseValidate")
#pragma comment (linker, "/export:RtlUlongByteSwap=ntoskrnl.RtlUlongByteSwap")
#pragma comment (linker, "/export:RtlUlonglongByteSwap=ntoskrnl.RtlUlonglongByteSwap")
#pragma comment (linker, "/export:RtlUnicodeStringToAnsiSize=ntoskrnl.RtlUnicodeStringToAnsiSize")
#pragma comment (linker, "/export:RtlUnicodeStringToAnsiString=ntoskrnl.RtlUnicodeStringToAnsiString")
#pragma comment (linker, "/export:RtlUnicodeStringToCountedOemString=ntoskrnl.RtlUnicodeStringToCountedOemString")
#pragma comment (linker, "/export:RtlUnicodeStringToInteger=ntoskrnl.RtlUnicodeStringToInteger")
#pragma comment (linker, "/export:RtlUnicodeStringToOemSize=ntoskrnl.RtlUnicodeStringToOemSize")
#pragma comment (linker, "/export:RtlUnicodeStringToOemString=ntoskrnl.RtlUnicodeStringToOemString")
#pragma comment (linker, "/export:RtlUnicodeToCustomCPN=ntoskrnl.RtlUnicodeToCustomCPN")
#pragma comment (linker, "/export:RtlUnicodeToMultiByteN=ntoskrnl.RtlUnicodeToMultiByteN")
#pragma comment (linker, "/export:RtlUnicodeToMultiByteSize=ntoskrnl.RtlUnicodeToMultiByteSize")
#pragma comment (linker, "/export:RtlUnicodeToOemN=ntoskrnl.RtlUnicodeToOemN")
#pragma comment (linker, "/export:RtlUnlockBootStatusData=ntoskrnl.RtlUnlockBootStatusData")
#pragma comment (linker, "/export:RtlUnwind=ntoskrnl.RtlUnwind")
#pragma comment (linker, "/export:RtlUpcaseUnicodeChar=ntoskrnl.RtlUpcaseUnicodeChar")
#pragma comment (linker, "/export:RtlUpcaseUnicodeString=ntoskrnl.RtlUpcaseUnicodeString")
#pragma comment (linker, "/export:RtlUpcaseUnicodeStringToAnsiString=ntoskrnl.RtlUpcaseUnicodeStringToAnsiString")
#pragma comment (linker, "/export:RtlUpcaseUnicodeStringToCountedOemString=ntoskrnl.RtlUpcaseUnicodeStringToCountedOemString")
#pragma comment (linker, "/export:RtlUpcaseUnicodeStringToOemString=ntoskrnl.RtlUpcaseUnicodeStringToOemString")
#pragma comment (linker, "/export:RtlUpcaseUnicodeToCustomCPN=ntoskrnl.RtlUpcaseUnicodeToCustomCPN")
#pragma comment (linker, "/export:RtlUpcaseUnicodeToMultiByteN=ntoskrnl.RtlUpcaseUnicodeToMultiByteN")
#pragma comment (linker, "/export:RtlUpcaseUnicodeToOemN=ntoskrnl.RtlUpcaseUnicodeToOemN")
#pragma comment (linker, "/export:RtlUpperChar=ntoskrnl.RtlUpperChar")
#pragma comment (linker, "/export:RtlUpperString=ntoskrnl.RtlUpperString")
#pragma comment (linker, "/export:RtlUshortByteSwap=ntoskrnl.RtlUshortByteSwap")
#pragma comment (linker, "/export:RtlValidRelativeSecurityDescriptor=ntoskrnl.RtlValidRelativeSecurityDescriptor")
#pragma comment (linker, "/export:RtlValidSecurityDescriptor=ntoskrnl.RtlValidSecurityDescriptor")
#pragma comment (linker, "/export:RtlValidSid=ntoskrnl.RtlValidSid")
#pragma comment (linker, "/export:RtlVerifyVersionInfo=ntoskrnl.RtlVerifyVersionInfo")
#pragma comment (linker, "/export:RtlVolumeDeviceToDosName=ntoskrnl.RtlVolumeDeviceToDosName")
#pragma comment (linker, "/export:RtlWalkFrameChain=ntoskrnl.RtlWalkFrameChain")
#pragma comment (linker, "/export:RtlWriteRegistryValue=ntoskrnl.RtlWriteRegistryValue")
#pragma comment (linker, "/export:RtlxAnsiStringToUnicodeSize=ntoskrnl.RtlxAnsiStringToUnicodeSize")
#pragma comment (linker, "/export:RtlxOemStringToUnicodeSize=ntoskrnl.RtlxOemStringToUnicodeSize")
#pragma comment (linker, "/export:RtlxUnicodeStringToAnsiSize=ntoskrnl.RtlxUnicodeStringToAnsiSize")
#pragma comment (linker, "/export:RtlxUnicodeStringToOemSize=ntoskrnl.RtlxUnicodeStringToOemSize")
#pragma comment (linker, "/export:RtlZeroHeap=ntoskrnl.RtlZeroHeap")
#pragma comment (linker, "/export:RtlZeroMemory=ntoskrnl.RtlZeroMemory")
#pragma comment (linker, "/export:SeAccessCheck=ntoskrnl.SeAccessCheck")
#pragma comment (linker, "/export:SeAppendPrivileges=ntoskrnl.SeAppendPrivileges")
#pragma comment (linker, "/export:SeAssignSecurity=ntoskrnl.SeAssignSecurity")
#pragma comment (linker, "/export:SeAssignSecurityEx=ntoskrnl.SeAssignSecurityEx")
#pragma comment (linker, "/export:SeAuditHardLinkCreation=ntoskrnl.SeAuditHardLinkCreation")
#pragma comment (linker, "/export:SeAuditingFileEvents=ntoskrnl.SeAuditingFileEvents")
#pragma comment (linker, "/export:SeAuditingFileOrGlobalEvents=ntoskrnl.SeAuditingFileOrGlobalEvents")
#pragma comment (linker, "/export:SeAuditingHardLinkEvents=ntoskrnl.SeAuditingHardLinkEvents")
#pragma comment (linker, "/export:SeCaptureSecurityDescriptor=ntoskrnl.SeCaptureSecurityDescriptor")
#pragma comment (linker, "/export:SeCaptureSubjectContext=ntoskrnl.SeCaptureSubjectContext")
#pragma comment (linker, "/export:SeCloseObjectAuditAlarm=ntoskrnl.SeCloseObjectAuditAlarm")
#pragma comment (linker, "/export:SeCreateAccessState=ntoskrnl.SeCreateAccessState")
#pragma comment (linker, "/export:SeCreateClientSecurity=ntoskrnl.SeCreateClientSecurity")
#pragma comment (linker, "/export:SeCreateClientSecurityFromSubjectContext=ntoskrnl.SeCreateClientSecurityFromSubjectContext")
#pragma comment (linker, "/export:SeDeassignSecurity=ntoskrnl.SeDeassignSecurity")
#pragma comment (linker, "/export:SeDeleteAccessState=ntoskrnl.SeDeleteAccessState")
#pragma comment (linker, "/export:SeDeleteObjectAuditAlarm=ntoskrnl.SeDeleteObjectAuditAlarm")
#pragma comment (linker, "/export:SeExports=ntoskrnl.SeExports")
#pragma comment (linker, "/export:SeFilterToken=ntoskrnl.SeFilterToken")
#pragma comment (linker, "/export:SeFreePrivileges=ntoskrnl.SeFreePrivileges")
#pragma comment (linker, "/export:SeImpersonateClient=ntoskrnl.SeImpersonateClient")
#pragma comment (linker, "/export:SeImpersonateClientEx=ntoskrnl.SeImpersonateClientEx")
#pragma comment (linker, "/export:SeLockSubjectContext=ntoskrnl.SeLockSubjectContext")
#pragma comment (linker, "/export:SeMarkLogonSessionForTerminationNotification=ntoskrnl.SeMarkLogonSessionForTerminationNotification")
#pragma comment (linker, "/export:SeOpenObjectAuditAlarm=ntoskrnl.SeOpenObjectAuditAlarm")
#pragma comment (linker, "/export:SeOpenObjectForDeleteAuditAlarm=ntoskrnl.SeOpenObjectForDeleteAuditAlarm")
#pragma comment (linker, "/export:SePrivilegeCheck=ntoskrnl.SePrivilegeCheck")
#pragma comment (linker, "/export:SePrivilegeObjectAuditAlarm=ntoskrnl.SePrivilegeObjectAuditAlarm")
#pragma comment (linker, "/export:SePublicDefaultDacl=ntoskrnl.SePublicDefaultDacl")
#pragma comment (linker, "/export:SeQueryAuthenticationIdToken=ntoskrnl.SeQueryAuthenticationIdToken")
#pragma comment (linker, "/export:SeQueryInformationToken=ntoskrnl.SeQueryInformationToken")
#pragma comment (linker, "/export:SeQuerySecurityDescriptorInfo=ntoskrnl.SeQuerySecurityDescriptorInfo")
#pragma comment (linker, "/export:SeQuerySessionIdToken=ntoskrnl.SeQuerySessionIdToken")
#pragma comment (linker, "/export:SeRegisterLogonSessionTerminatedRoutine=ntoskrnl.SeRegisterLogonSessionTerminatedRoutine")
#pragma comment (linker, "/export:SeReleaseSecurityDescriptor=ntoskrnl.SeReleaseSecurityDescriptor")
#pragma comment (linker, "/export:SeReleaseSubjectContext=ntoskrnl.SeReleaseSubjectContext")
#pragma comment (linker, "/export:SeSetAccessStateGenericMapping=ntoskrnl.SeSetAccessStateGenericMapping")
#pragma comment (linker, "/export:SeSetSecurityDescriptorInfo=ntoskrnl.SeSetSecurityDescriptorInfo")
#pragma comment (linker, "/export:SeSetSecurityDescriptorInfoEx=ntoskrnl.SeSetSecurityDescriptorInfoEx")
#pragma comment (linker, "/export:SeSinglePrivilegeCheck=ntoskrnl.SeSinglePrivilegeCheck")
#pragma comment (linker, "/export:SeSystemDefaultDacl=ntoskrnl.SeSystemDefaultDacl")
#pragma comment (linker, "/export:SeTokenImpersonationLevel=ntoskrnl.SeTokenImpersonationLevel")
#pragma comment (linker, "/export:SeTokenIsAdmin=ntoskrnl.SeTokenIsAdmin")
#pragma comment (linker, "/export:SeTokenIsRestricted=ntoskrnl.SeTokenIsRestricted")
#pragma comment (linker, "/export:SeTokenObjectType=ntoskrnl.SeTokenObjectType")
#pragma comment (linker, "/export:SeTokenType=ntoskrnl.SeTokenType")
#pragma comment (linker, "/export:SeUnlockSubjectContext=ntoskrnl.SeUnlockSubjectContext")
#pragma comment (linker, "/export:SeUnregisterLogonSessionTerminatedRoutine=ntoskrnl.SeUnregisterLogonSessionTerminatedRoutine")
#pragma comment (linker, "/export:SeValidSecurityDescriptor=ntoskrnl.SeValidSecurityDescriptor")
#pragma comment (linker, "/export:sprintf=ntoskrnl.sprintf")
#pragma comment (linker, "/export:srand=ntoskrnl.srand")
#pragma comment (linker, "/export:strcat=ntoskrnl.strcat")
#pragma comment (linker, "/export:strchr=ntoskrnl.strchr")
#pragma comment (linker, "/export:strcmp=ntoskrnl.strcmp")
#pragma comment (linker, "/export:strcpy=ntoskrnl.strcpy")
#pragma comment (linker, "/export:strlen=ntoskrnl.strlen")
#pragma comment (linker, "/export:strncat=ntoskrnl.strncat")
#pragma comment (linker, "/export:strncmp=ntoskrnl.strncmp")
#pragma comment (linker, "/export:strncpy=ntoskrnl.strncpy")
#pragma comment (linker, "/export:strrchr=ntoskrnl.strrchr")
#pragma comment (linker, "/export:strspn=ntoskrnl.strspn")
#pragma comment (linker, "/export:strstr=ntoskrnl.strstr")
#pragma comment (linker, "/export:swprintf=ntoskrnl.swprintf")
#pragma comment (linker, "/export:tolower=ntoskrnl.tolower")
#pragma comment (linker, "/export:toupper=ntoskrnl.toupper")
#pragma comment (linker, "/export:towlower=ntoskrnl.towlower")
#pragma comment (linker, "/export:towupper=ntoskrnl.towupper")
#pragma comment (linker, "/export:vDbgPrintEx=ntoskrnl.vDbgPrintEx")
#pragma comment (linker, "/export:vDbgPrintExWithPrefix=ntoskrnl.vDbgPrintExWithPrefix")
#pragma comment (linker, "/export:VerSetConditionMask=ntoskrnl.VerSetConditionMask")
#pragma comment (linker, "/export:VfFailDeviceNode=ntoskrnl.VfFailDeviceNode")
#pragma comment (linker, "/export:VfFailDriver=ntoskrnl.VfFailDriver")
#pragma comment (linker, "/export:VfFailSystemBIOS=ntoskrnl.VfFailSystemBIOS")
#pragma comment (linker, "/export:VfIsVerificationEnabled=ntoskrnl.VfIsVerificationEnabled")
#pragma comment (linker, "/export:vsprintf=ntoskrnl.vsprintf")
#pragma comment (linker, "/export:wcscat=ntoskrnl.wcscat")
#pragma comment (linker, "/export:wcschr=ntoskrnl.wcschr")
#pragma comment (linker, "/export:wcscmp=ntoskrnl.wcscmp")
#pragma comment (linker, "/export:wcscpy=ntoskrnl.wcscpy")
#pragma comment (linker, "/export:wcscspn=ntoskrnl.wcscspn")
#pragma comment (linker, "/export:wcslen=ntoskrnl.wcslen")
#pragma comment (linker, "/export:wcsncat=ntoskrnl.wcsncat")
#pragma comment (linker, "/export:wcsncmp=ntoskrnl.wcsncmp")
#pragma comment (linker, "/export:wcsncpy=ntoskrnl.wcsncpy")
#pragma comment (linker, "/export:wcsrchr=ntoskrnl.wcsrchr")
#pragma comment (linker, "/export:wcsspn=ntoskrnl.wcsspn")
#pragma comment (linker, "/export:wcsstr=ntoskrnl.wcsstr")
#pragma comment (linker, "/export:wcstombs=ntoskrnl.wcstombs")
#pragma comment (linker, "/export:wctomb=ntoskrnl.wctomb")
#pragma comment (linker, "/export:WmiFlushTrace=ntoskrnl.WmiFlushTrace")
#pragma comment (linker, "/export:WmiGetClock=ntoskrnl.WmiGetClock")
#pragma comment (linker, "/export:WmiQueryTrace=ntoskrnl.WmiQueryTrace")
#pragma comment (linker, "/export:WmiQueryTraceInformation=ntoskrnl.WmiQueryTraceInformation")
#pragma comment (linker, "/export:WmiStartTrace=ntoskrnl.WmiStartTrace")
#pragma comment (linker, "/export:WmiStopTrace=ntoskrnl.WmiStopTrace")
#pragma comment (linker, "/export:WmiTraceMessage=ntoskrnl.WmiTraceMessage")
#pragma comment (linker, "/export:WmiTraceMessageVa=ntoskrnl.WmiTraceMessageVa")
#pragma comment (linker, "/export:WmiUpdateTrace=ntoskrnl.WmiUpdateTrace")
#pragma comment (linker, "/export:WRITE_REGISTER_BUFFER_UCHAR=ntoskrnl.WRITE_REGISTER_BUFFER_UCHAR")
#pragma comment (linker, "/export:WRITE_REGISTER_BUFFER_ULONG=ntoskrnl.WRITE_REGISTER_BUFFER_ULONG")
#pragma comment (linker, "/export:WRITE_REGISTER_BUFFER_USHORT=ntoskrnl.WRITE_REGISTER_BUFFER_USHORT")
#pragma comment (linker, "/export:WRITE_REGISTER_UCHAR=ntoskrnl.WRITE_REGISTER_UCHAR")
#pragma comment (linker, "/export:WRITE_REGISTER_ULONG=ntoskrnl.WRITE_REGISTER_ULONG")
#pragma comment (linker, "/export:WRITE_REGISTER_USHORT=ntoskrnl.WRITE_REGISTER_USHORT")
#pragma comment (linker, "/export:XIPDispatch=ntoskrnl.XIPDispatch")
#pragma comment (linker, "/export:ZwAccessCheckAndAuditAlarm=ntoskrnl.ZwAccessCheckAndAuditAlarm")
#pragma comment (linker, "/export:ZwAddBootEntry=ntoskrnl.ZwAddBootEntry")
#pragma comment (linker, "/export:ZwAdjustPrivilegesToken=ntoskrnl.ZwAdjustPrivilegesToken")
#pragma comment (linker, "/export:ZwAlertThread=ntoskrnl.ZwAlertThread")
#pragma comment (linker, "/export:ZwAllocateVirtualMemory=ntoskrnl.ZwAllocateVirtualMemory")
#pragma comment (linker, "/export:ZwAssignProcessToJobObject=ntoskrnl.ZwAssignProcessToJobObject")
#pragma comment (linker, "/export:ZwCancelIoFile=ntoskrnl.ZwCancelIoFile")
#pragma comment (linker, "/export:ZwCancelTimer=ntoskrnl.ZwCancelTimer")
#pragma comment (linker, "/export:ZwClearEvent=ntoskrnl.ZwClearEvent")
#pragma comment (linker, "/export:ZwClose=ntoskrnl.ZwClose")
#pragma comment (linker, "/export:ZwCloseObjectAuditAlarm=ntoskrnl.ZwCloseObjectAuditAlarm")
#pragma comment (linker, "/export:ZwConnectPort=ntoskrnl.ZwConnectPort")
#pragma comment (linker, "/export:ZwCreateDirectoryObject=ntoskrnl.ZwCreateDirectoryObject")
#pragma comment (linker, "/export:ZwCreateEvent=ntoskrnl.ZwCreateEvent")
#pragma comment (linker, "/export:ZwCreateFile=ntoskrnl.ZwCreateFile")
#pragma comment (linker, "/export:ZwCreateJobObject=ntoskrnl.ZwCreateJobObject")
#pragma comment (linker, "/export:ZwCreateKey=ntoskrnl.ZwCreateKey")
#pragma comment (linker, "/export:ZwCreateSection=ntoskrnl.ZwCreateSection")
#pragma comment (linker, "/export:ZwCreateSymbolicLinkObject=ntoskrnl.ZwCreateSymbolicLinkObject")
#pragma comment (linker, "/export:ZwCreateTimer=ntoskrnl.ZwCreateTimer")
#pragma comment (linker, "/export:ZwDeleteBootEntry=ntoskrnl.ZwDeleteBootEntry")
#pragma comment (linker, "/export:ZwDeleteFile=ntoskrnl.ZwDeleteFile")
#pragma comment (linker, "/export:ZwDeleteKey=ntoskrnl.ZwDeleteKey")
#pragma comment (linker, "/export:ZwDeleteValueKey=ntoskrnl.ZwDeleteValueKey")
#pragma comment (linker, "/export:ZwDeviceIoControlFile=ntoskrnl.ZwDeviceIoControlFile")
#pragma comment (linker, "/export:ZwDisplayString=ntoskrnl.ZwDisplayString")
#pragma comment (linker, "/export:ZwDuplicateObject=ntoskrnl.ZwDuplicateObject")
#pragma comment (linker, "/export:ZwDuplicateToken=ntoskrnl.ZwDuplicateToken")
#pragma comment (linker, "/export:ZwEnumerateBootEntries=ntoskrnl.ZwEnumerateBootEntries")
#pragma comment (linker, "/export:ZwEnumerateKey=ntoskrnl.ZwEnumerateKey")
#pragma comment (linker, "/export:ZwEnumerateValueKey=ntoskrnl.ZwEnumerateValueKey")
#pragma comment (linker, "/export:ZwFlushInstructionCache=ntoskrnl.ZwFlushInstructionCache")
#pragma comment (linker, "/export:ZwFlushKey=ntoskrnl.ZwFlushKey")
#pragma comment (linker, "/export:ZwFlushVirtualMemory=ntoskrnl.ZwFlushVirtualMemory")
#pragma comment (linker, "/export:ZwFreeVirtualMemory=ntoskrnl.ZwFreeVirtualMemory")
#pragma comment (linker, "/export:ZwFsControlFile=ntoskrnl.ZwFsControlFile")
#pragma comment (linker, "/export:ZwInitiatePowerAction=ntoskrnl.ZwInitiatePowerAction")
#pragma comment (linker, "/export:ZwIsProcessInJob=ntoskrnl.ZwIsProcessInJob")
#pragma comment (linker, "/export:ZwLoadDriver=ntoskrnl.ZwLoadDriver")
#pragma comment (linker, "/export:ZwLoadKey=ntoskrnl.ZwLoadKey")
#pragma comment (linker, "/export:ZwMakeTemporaryObject=ntoskrnl.ZwMakeTemporaryObject")
#pragma comment (linker, "/export:ZwMapViewOfSection=ntoskrnl.ZwMapViewOfSection")
#pragma comment (linker, "/export:ZwNotifyChangeKey=ntoskrnl.ZwNotifyChangeKey")
#pragma comment (linker, "/export:ZwOpenDirectoryObject=ntoskrnl.ZwOpenDirectoryObject")
#pragma comment (linker, "/export:ZwOpenEvent=ntoskrnl.ZwOpenEvent")
#pragma comment (linker, "/export:ZwOpenFile=ntoskrnl.ZwOpenFile")
#pragma comment (linker, "/export:ZwOpenJobObject=ntoskrnl.ZwOpenJobObject")
#pragma comment (linker, "/export:ZwOpenKey=ntoskrnl.ZwOpenKey")
#pragma comment (linker, "/export:ZwOpenProcess=ntoskrnl.ZwOpenProcess")
#pragma comment (linker, "/export:ZwOpenProcessToken=ntoskrnl.ZwOpenProcessToken")
#pragma comment (linker, "/export:ZwOpenProcessTokenEx=ntoskrnl.ZwOpenProcessTokenEx")
#pragma comment (linker, "/export:ZwOpenSection=ntoskrnl.ZwOpenSection")
#pragma comment (linker, "/export:ZwOpenSymbolicLinkObject=ntoskrnl.ZwOpenSymbolicLinkObject")
#pragma comment (linker, "/export:ZwOpenThread=ntoskrnl.ZwOpenThread")
#pragma comment (linker, "/export:ZwOpenThreadToken=ntoskrnl.ZwOpenThreadToken")
#pragma comment (linker, "/export:ZwOpenThreadTokenEx=ntoskrnl.ZwOpenThreadTokenEx")
#pragma comment (linker, "/export:ZwOpenTimer=ntoskrnl.ZwOpenTimer")
#pragma comment (linker, "/export:ZwPowerInformation=ntoskrnl.ZwPowerInformation")
#pragma comment (linker, "/export:ZwPulseEvent=ntoskrnl.ZwPulseEvent")
#pragma comment (linker, "/export:ZwQueryBootEntryOrder=ntoskrnl.ZwQueryBootEntryOrder")
#pragma comment (linker, "/export:ZwQueryBootOptions=ntoskrnl.ZwQueryBootOptions")
#pragma comment (linker, "/export:ZwQueryDefaultLocale=ntoskrnl.ZwQueryDefaultLocale")
#pragma comment (linker, "/export:ZwQueryDefaultUILanguage=ntoskrnl.ZwQueryDefaultUILanguage")
#pragma comment (linker, "/export:ZwQueryDirectoryFile=ntoskrnl.ZwQueryDirectoryFile")
#pragma comment (linker, "/export:ZwQueryDirectoryObject=ntoskrnl.ZwQueryDirectoryObject")
#pragma comment (linker, "/export:ZwQueryEaFile=ntoskrnl.ZwQueryEaFile")
#pragma comment (linker, "/export:ZwQueryFullAttributesFile=ntoskrnl.ZwQueryFullAttributesFile")
#pragma comment (linker, "/export:ZwQueryInformationFile=ntoskrnl.ZwQueryInformationFile")
#pragma comment (linker, "/export:ZwQueryInformationJobObject=ntoskrnl.ZwQueryInformationJobObject")
#pragma comment (linker, "/export:ZwQueryInformationProcess=ntoskrnl.ZwQueryInformationProcess")
#pragma comment (linker, "/export:ZwQueryInformationThread=ntoskrnl.ZwQueryInformationThread")
#pragma comment (linker, "/export:ZwQueryInformationToken=ntoskrnl.ZwQueryInformationToken")
#pragma comment (linker, "/export:ZwQueryInstallUILanguage=ntoskrnl.ZwQueryInstallUILanguage")
#pragma comment (linker, "/export:ZwQueryKey=ntoskrnl.ZwQueryKey")
#pragma comment (linker, "/export:ZwQueryObject=ntoskrnl.ZwQueryObject")
#pragma comment (linker, "/export:ZwQuerySection=ntoskrnl.ZwQuerySection")
#pragma comment (linker, "/export:ZwQuerySecurityObject=ntoskrnl.ZwQuerySecurityObject")
#pragma comment (linker, "/export:ZwQuerySymbolicLinkObject=ntoskrnl.ZwQuerySymbolicLinkObject")
#pragma comment (linker, "/export:ZwQuerySystemInformation=ntoskrnl.ZwQuerySystemInformation")
#pragma comment (linker, "/export:ZwQueryValueKey=ntoskrnl.ZwQueryValueKey")
#pragma comment (linker, "/export:ZwQueryVolumeInformationFile=ntoskrnl.ZwQueryVolumeInformationFile")
#pragma comment (linker, "/export:ZwReadFile=ntoskrnl.ZwReadFile")
#pragma comment (linker, "/export:ZwReplaceKey=ntoskrnl.ZwReplaceKey")
#pragma comment (linker, "/export:ZwRequestWaitReplyPort=ntoskrnl.ZwRequestWaitReplyPort")
#pragma comment (linker, "/export:ZwResetEvent=ntoskrnl.ZwResetEvent")
#pragma comment (linker, "/export:ZwRestoreKey=ntoskrnl.ZwRestoreKey")
#pragma comment (linker, "/export:ZwSaveKey=ntoskrnl.ZwSaveKey")
#pragma comment (linker, "/export:ZwSaveKeyEx=ntoskrnl.ZwSaveKeyEx")
#pragma comment (linker, "/export:ZwSetBootEntryOrder=ntoskrnl.ZwSetBootEntryOrder")
#pragma comment (linker, "/export:ZwSetBootOptions=ntoskrnl.ZwSetBootOptions")
#pragma comment (linker, "/export:ZwSetDefaultLocale=ntoskrnl.ZwSetDefaultLocale")
#pragma comment (linker, "/export:ZwSetDefaultUILanguage=ntoskrnl.ZwSetDefaultUILanguage")
#pragma comment (linker, "/export:ZwSetEaFile=ntoskrnl.ZwSetEaFile")
#pragma comment (linker, "/export:ZwSetEvent=ntoskrnl.ZwSetEvent")
#pragma comment (linker, "/export:ZwSetInformationFile=ntoskrnl.ZwSetInformationFile")
#pragma comment (linker, "/export:ZwSetInformationJobObject=ntoskrnl.ZwSetInformationJobObject")
#pragma comment (linker, "/export:ZwSetInformationObject=ntoskrnl.ZwSetInformationObject")
#pragma comment (linker, "/export:ZwSetInformationProcess=ntoskrnl.ZwSetInformationProcess")
#pragma comment (linker, "/export:ZwSetInformationThread=ntoskrnl.ZwSetInformationThread")
#pragma comment (linker, "/export:ZwSetSecurityObject=ntoskrnl.ZwSetSecurityObject")
#pragma comment (linker, "/export:ZwSetSystemInformation=ntoskrnl.ZwSetSystemInformation")
#pragma comment (linker, "/export:ZwSetSystemTime=ntoskrnl.ZwSetSystemTime")
#pragma comment (linker, "/export:ZwSetTimer=ntoskrnl.ZwSetTimer")
#pragma comment (linker, "/export:ZwSetValueKey=ntoskrnl.ZwSetValueKey")
#pragma comment (linker, "/export:ZwSetVolumeInformationFile=ntoskrnl.ZwSetVolumeInformationFile")
#pragma comment (linker, "/export:ZwTerminateJobObject=ntoskrnl.ZwTerminateJobObject")
#pragma comment (linker, "/export:ZwTerminateProcess=ntoskrnl.ZwTerminateProcess")
#pragma comment (linker, "/export:ZwTranslateFilePath=ntoskrnl.ZwTranslateFilePath")
#pragma comment (linker, "/export:ZwUnloadDriver=ntoskrnl.ZwUnloadDriver")
#pragma comment (linker, "/export:ZwUnloadKey=ntoskrnl.ZwUnloadKey")
#pragma comment (linker, "/export:ZwUnmapViewOfSection=ntoskrnl.ZwUnmapViewOfSection")
#pragma comment (linker, "/export:ZwWaitForMultipleObjects=ntoskrnl.ZwWaitForMultipleObjects")
#pragma comment (linker, "/export:ZwWaitForSingleObject=ntoskrnl.ZwWaitForSingleObject")
#pragma comment (linker, "/export:ZwWriteFile=ntoskrnl.ZwWriteFile")
#pragma comment (linker, "/export:ZwYieldExecution=ntoskrnl.ZwYieldExecution")
// #pragma comment (linker, "/export:ExAcquireRundownProtectionEx=ntoskrnl.ExAcquireRundownProtectionEx")
// #pragma comment (linker, "/export:ExReleaseRundownProtectionEx=ntoskrnl.ExReleaseRundownProtectionEx")
// #pragma comment (linker, "/export:IoFreeWorkItem=ntoskrnl.IoFreeWorkItem")
// #pragma comment (linker, "/export:KeFlushQueuedDpcs=ntoskrnl.KeFlushQueuedDpcs")
// #pragma comment (linker, "/export:MmGetSystemRoutineAddress=ntoskrnl.MmGetSystemRoutineAddress")
#endif


/////////////////////////////////////////
//         Windows XP SP2+SP3          //
#if (NTDDI_VERSION >= NTDDI_WINXPSP2)
#pragma comment (linker, "/export:ExfAcquirePushLockExclusive=ntoskrnl.ExfAcquirePushLockExclusive")
#pragma comment (linker, "/export:ExfAcquirePushLockShared=ntoskrnl.ExfAcquirePushLockShared")
#pragma comment (linker, "/export:ExfReleasePushLock=ntoskrnl.ExfReleasePushLock")
#pragma comment (linker, "/export:FsRtlCreateSectionForDataScan=ntoskrnl.FsRtlCreateSectionForDataScan")
#pragma comment (linker, "/export:IoEnumerateRegisteredFiltersList=ntoskrnl.IoEnumerateRegisteredFiltersList")
#pragma comment (linker, "/export:IoInitializeCrashDump=ntoskrnl.IoInitializeCrashDump")
#pragma comment (linker, "/export:IoValidateDeviceIoControlAccess=ntoskrnl.IoValidateDeviceIoControlAccess")
#pragma comment (linker, "/export:KeCapturePersistentThreadState=ntoskrnl.KeCapturePersistentThreadState")
#pragma comment (linker, "/export:KeDeregisterBugCheckReasonCallback=ntoskrnl.KeDeregisterBugCheckReasonCallback")
#pragma comment (linker, "/export:KeRegisterBugCheckReasonCallback=ntoskrnl.KeRegisterBugCheckReasonCallback")
#pragma comment (linker, "/export:MmCommitSessionMappedView=ntoskrnl.MmCommitSessionMappedView")
#pragma comment (linker, "/export:ObIsDosDeviceLocallyMapped=ntoskrnl.ObIsDosDeviceLocallyMapped")
#pragma comment (linker, "/export:PsDereferenceKernelStack=ntoskrnl.PsDereferenceKernelStack")
#pragma comment (linker, "/export:PsGetContextThread=ntoskrnl.PsGetContextThread")
#pragma comment (linker, "/export:PsReferenceKernelStack=ntoskrnl.PsReferenceKernelStack")
#pragma comment (linker, "/export:PsSetContextThread=ntoskrnl.PsSetContextThread")
#pragma comment (linker, "/export:RtlAddAccessAllowedAceEx=ntoskrnl.RtlAddAccessAllowedAceEx")
#pragma comment (linker, "/export:RtlIpv4AddressToStringExA=ntoskrnl.RtlIpv4AddressToStringExA")
#pragma comment (linker, "/export:RtlIpv4AddressToStringExW=ntoskrnl.RtlIpv4AddressToStringExW")
#pragma comment (linker, "/export:RtlIpv4StringToAddressExA=ntoskrnl.RtlIpv4StringToAddressExA")
#pragma comment (linker, "/export:RtlIpv4StringToAddressExW=ntoskrnl.RtlIpv4StringToAddressExW")
#pragma comment (linker, "/export:RtlIpv6AddressToStringExA=ntoskrnl.RtlIpv6AddressToStringExA")
#pragma comment (linker, "/export:RtlIpv6AddressToStringExW=ntoskrnl.RtlIpv6AddressToStringExW")
#pragma comment (linker, "/export:RtlIpv6StringToAddressExA=ntoskrnl.RtlIpv6StringToAddressExA")
#pragma comment (linker, "/export:RtlIpv6StringToAddressExW=ntoskrnl.RtlIpv6StringToAddressExW")
#pragma comment (linker, "/export:SeAuditingFileEventsWithContext=ntoskrnl.SeAuditingFileEventsWithContext")
#pragma comment (linker, "/export:SeAuditingHardLinkEventsWithContext=ntoskrnl.SeAuditingHardLinkEventsWithContext")
#pragma comment (linker, "/export:SeTokenIsWriteRestricted=ntoskrnl.SeTokenIsWriteRestricted")
#endif

/////////////////////////////////////////
//           Windows 2003              //
#if (NTDDI_VERSION >= NTDDI_WS03)      //
#pragma comment (linker, "/export:DbgCommandString=ntoskrnl.DbgCommandString")
#pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireFastMutexUnsafe=ntoskrnl.ExEnterCriticalRegionAndAcquireFastMutexUnsafe")
#pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireResourceExclusive=ntoskrnl.ExEnterCriticalRegionAndAcquireResourceExclusive")
#pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireResourceShared=ntoskrnl.ExEnterCriticalRegionAndAcquireResourceShared")
#pragma comment (linker, "/export:ExEnterCriticalRegionAndAcquireSharedWaitForExclusive=ntoskrnl.ExEnterCriticalRegionAndAcquireSharedWaitForExclusive")
#pragma comment (linker, "/export:ExfReleasePushLockExclusive=ntoskrnl.ExfReleasePushLockExclusive")
#pragma comment (linker, "/export:ExfReleasePushLockShared=ntoskrnl.ExfReleasePushLockShared")
#pragma comment (linker, "/export:ExfTryToWakePushLock=ntoskrnl.ExfTryToWakePushLock")
#pragma comment (linker, "/export:ExfUnblockPushLock=ntoskrnl.ExfUnblockPushLock")
#pragma comment (linker, "/export:ExiAcquireFastMutex=ntoskrnl.ExiAcquireFastMutex")
#pragma comment (linker, "/export:ExiReleaseFastMutex=ntoskrnl.ExiReleaseFastMutex")
#pragma comment (linker, "/export:ExiTryToAcquireFastMutex=ntoskrnl.ExiTryToAcquireFastMutex")
#pragma comment (linker, "/export:ExReleaseFastMutexUnsafeAndLeaveCriticalRegion=ntoskrnl.ExReleaseFastMutexUnsafeAndLeaveCriticalRegion")
#pragma comment (linker, "/export:FsRtlAddBaseMcbEntry=ntoskrnl.FsRtlAddBaseMcbEntry")
#pragma comment (linker, "/export:FsRtlGetNextBaseMcbEntry=ntoskrnl.FsRtlGetNextBaseMcbEntry")
#pragma comment (linker, "/export:FsRtlInitializeBaseMcb=ntoskrnl.FsRtlInitializeBaseMcb")
#pragma comment (linker, "/export:FsRtlLookupBaseMcbEntry=ntoskrnl.FsRtlLookupBaseMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupLastBaseMcbEntry=ntoskrnl.FsRtlLookupLastBaseMcbEntry")
#pragma comment (linker, "/export:FsRtlLookupLastBaseMcbEntryAndIndex=ntoskrnl.FsRtlLookupLastBaseMcbEntryAndIndex")
#pragma comment (linker, "/export:FsRtlNumberOfRunsInBaseMcb=ntoskrnl.FsRtlNumberOfRunsInBaseMcb")
#pragma comment (linker, "/export:FsRtlRemoveBaseMcbEntry=ntoskrnl.FsRtlRemoveBaseMcbEntry")
#pragma comment (linker, "/export:FsRtlResetBaseMcb=ntoskrnl.FsRtlResetBaseMcb")
#pragma comment (linker, "/export:FsRtlSplitBaseMcb=ntoskrnl.FsRtlSplitBaseMcb")
#pragma comment (linker, "/export:FsRtlTruncateBaseMcb=ntoskrnl.FsRtlTruncateBaseMcb")
#pragma comment (linker, "/export:FsRtlUninitializeBaseMcb=ntoskrnl.FsRtlUninitializeBaseMcb")
#pragma comment (linker, "/export:IoCsqInitializeEx=ntoskrnl.IoCsqInitializeEx")
#pragma comment (linker, "/export:IoCsqInsertIrpEx=ntoskrnl.IoCsqInsertIrpEx")
#pragma comment (linker, "/export:IoGetPagingIoPriority=ntoskrnl.IoGetPagingIoPriority")
#pragma comment (linker, "/export:IoTranslateBusAddress=ntoskrnl.IoTranslateBusAddress")
#pragma comment (linker, "/export:KdChangeOption=ntoskrnl.KdChangeOption")
#pragma comment (linker, "/export:KdSystemDebugControl=ntoskrnl.KdSystemDebugControl")
#pragma comment (linker, "/export:KeAcquireInStackQueuedSpinLockForDpc=ntoskrnl.KeAcquireInStackQueuedSpinLockForDpc")
#pragma comment (linker, "/export:KeAcquireSpinLockForDpc=ntoskrnl.KeAcquireSpinLockForDpc")
#pragma comment (linker, "/export:KeAreAllApcsDisabled=ntoskrnl.KeAreAllApcsDisabled")
#pragma comment (linker, "/export:KeDeregisterNmiCallback=ntoskrnl.KeDeregisterNmiCallback")
#pragma comment (linker, "/export:KeGenericCallDpc=ntoskrnl.KeGenericCallDpc")
#pragma comment (linker, "/export:KeInitializeCrashDumpHeader=ntoskrnl.KeInitializeCrashDumpHeader")
#pragma comment (linker, "/export:KeInitializeThreadedDpc=ntoskrnl.KeInitializeThreadedDpc")
#pragma comment (linker, "/export:KeInvalidateAllCaches=ntoskrnl.KeInvalidateAllCaches")
#pragma comment (linker, "/export:KeIpiGenericCall=ntoskrnl.KeIpiGenericCall")
#pragma comment (linker, "/export:KeIsWaitListEmpty=ntoskrnl.KeIsWaitListEmpty")
#pragma comment (linker, "/export:KeRegisterNmiCallback=ntoskrnl.KeRegisterNmiCallback")
#pragma comment (linker, "/export:KeReleaseInStackQueuedSpinLockForDpc=ntoskrnl.KeReleaseInStackQueuedSpinLockForDpc")
#pragma comment (linker, "/export:KeReleaseSpinLockForDpc=ntoskrnl.KeReleaseSpinLockForDpc")
#pragma comment (linker, "/export:KeSignalCallDpcDone=ntoskrnl.KeSignalCallDpcDone")
#pragma comment (linker, "/export:KeSignalCallDpcSynchronize=ntoskrnl.KeSignalCallDpcSynchronize")
#pragma comment (linker, "/export:KeTestSpinLock=ntoskrnl.KeTestSpinLock")
#pragma comment (linker, "/export:KeTryToAcquireSpinLockAtDpcLevel=ntoskrnl.KeTryToAcquireSpinLockAtDpcLevel")
#pragma comment (linker, "/export:KiCheckForKernelApcDelivery=ntoskrnl.KiCheckForKernelApcDelivery")
#pragma comment (linker, "/export:KiCheckForSListAddress=ntoskrnl.KiCheckForSListAddress")
#pragma comment (linker, "/export:MmAllocatePagesForMdlEx=ntoskrnl.MmAllocatePagesForMdlEx")
#pragma comment (linker, "/export:MmCreateMirror=ntoskrnl.MmCreateMirror")
#pragma comment (linker, "/export:MmDoesFileHaveUserWritableReferences=ntoskrnl.MmDoesFileHaveUserWritableReferences")
#pragma comment (linker, "/export:MmIsIoSpaceActive=ntoskrnl.MmIsIoSpaceActive")
#pragma comment (linker, "/export:ObDeleteCapturedInsertInfo=ntoskrnl.ObDeleteCapturedInsertInfo")
#pragma comment (linker, "/export:PsGetCurrentProcessWin32Process=ntoskrnl.PsGetCurrentProcessWin32Process")
#pragma comment (linker, "/export:PsGetCurrentThreadProcess=ntoskrnl.PsGetCurrentThreadProcess")
#pragma comment (linker, "/export:PsGetCurrentThreadProcessId=ntoskrnl.PsGetCurrentThreadProcessId")
#pragma comment (linker, "/export:PsGetCurrentThreadTeb=ntoskrnl.PsGetCurrentThreadTeb")
#pragma comment (linker, "/export:PsGetCurrentThreadWin32Thread=ntoskrnl.PsGetCurrentThreadWin32Thread")
#pragma comment (linker, "/export:PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion=ntoskrnl.PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion")
#pragma comment (linker, "/export:PsGetProcessSessionIdEx=ntoskrnl.PsGetProcessSessionIdEx")
#pragma comment (linker, "/export:PsIsSystemProcess=ntoskrnl.PsIsSystemProcess")
#pragma comment (linker, "/export:PsWrapApcWow64Thread=ntoskrnl.PsWrapApcWow64Thread")
#pragma comment (linker, "/export:RtlInitAnsiStringEx=ntoskrnl.RtlInitAnsiStringEx")
#pragma comment (linker, "/export:RtlInitUnicodeStringEx=ntoskrnl.RtlInitUnicodeStringEx")
#pragma comment (linker, "/export:SeReportSecurityEvent=ntoskrnl.SeReportSecurityEvent")
#pragma comment (linker, "/export:WmiTraceFastEvent=ntoskrnl.WmiTraceFastEvent")
#pragma comment (linker, "/export:ZwAddDriverEntry=ntoskrnl.ZwAddDriverEntry")
#pragma comment (linker, "/export:ZwDeleteDriverEntry=ntoskrnl.ZwDeleteDriverEntry")
#pragma comment (linker, "/export:ZwEnumerateDriverEntries=ntoskrnl.ZwEnumerateDriverEntries")
#pragma comment (linker, "/export:ZwModifyBootEntry=ntoskrnl.ZwModifyBootEntry")
#pragma comment (linker, "/export:ZwModifyDriverEntry=ntoskrnl.ZwModifyDriverEntry")
#pragma comment (linker, "/export:ZwQueryDriverEntryOrder=ntoskrnl.ZwQueryDriverEntryOrder")
#pragma comment (linker, "/export:ZwSecureConnectPort=ntoskrnl.ZwSecureConnectPort")
#pragma comment (linker, "/export:ZwSetDriverEntryOrder=ntoskrnl.ZwSetDriverEntryOrder")
#endif

/////////////////////////////////////////
//           Windows Vista             //
#if (NTDDI_VERSION >= NTDDI_VISTA)     //
//TODO
#endif

/////////////////////////////////////////
//             Windows 7               //
#if (NTDDI_VERSION >= NTDDI_WIN7)      //
//TODO
#endif


//////////////////////////////////////////////////////////
//                Windows XP/2003 x64                   //
#if (NTDDI_VERSION >= NTDDI_WS03) && defined(_AMD64_)   //
//TODO
//__C_specific_handler
//__chkstk
//__misaligned_access
//....
#endif  


//////////////////////////////////////////////////////////
//                 Windows Vista x64                    //
#if (NTDDI_VERSION >= NTDDI_VISTA) && defined(_AMD64_)  //
//TODO
#endif

//////////////////////////////////////////////////////////
//                   Windows 7 x64                      //
#if (NTDDI_VERSION >= NTDDI_WIN7) && defined(_AMD64_)   //
//TODO
#endif