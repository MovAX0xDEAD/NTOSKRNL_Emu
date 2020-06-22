#undef    __STDC_WANT_SECURE_LIB__
#define   __STDC_WANT_SECURE_LIB__ 0

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <memory.h>

#include <ntddk.h> 
#include <wdm.h>    // ntddk + wdm

#include <Initguid.h>
#include <devpkey.h>

#include "wrk2003.h"

/////////////////////////////////////////////////////////
// early definition

VOID
StorportInitialize();

NTSTATUS
EtwRegister_k8 (
    LPCGUID             ProviderId,
    PETWENABLECALLBACK  EnableCallback,
    PVOID               CallbackContext,
    PREGHANDLE          RegHandle );

void PrintNumaCpuConfiguration();

// early definition
//////////////////////////////////////////////////////////

typedef VOID                (*pVOID__VOID)            (VOID);
typedef VOID                (*pVOID__ULONG)           (ULONG);
typedef VOID                (*pVOID__ULONG_ULONG)     (ULONG,ULONG);
typedef VOID    (__stdcall  *pfVOID__ULONG_ULONG)     (ULONG,ULONG);
typedef BOOLEAN (__stdcall  *pfBOOLEAN__ULONG_ULONG)  (ULONG,ULONG);

typedef struct    _PORT_MESSAGE {
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
} PORT_MESSAGE, *PPORT_MESSAGE;

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

typedef VOID IO_WORKITEM_ROUTINE_EX (
    PVOID           IoObject,
    PVOID           Context,
    PIO_WORKITEM    IoWorkItem
    );
typedef IO_WORKITEM_ROUTINE_EX *PIO_WORKITEM_ROUTINE_EX;

typedef struct _WorkerRoutineArray {
    PIO_WORKITEM_ROUTINE_EX  WorkerRoutineEx;
    PIO_WORKITEM             IoWorkItem;
    PVOID                    Context;
} WorkerRoutineArrayType;

typedef struct _RoutineAddress {
    UNICODE_STRING  RoutineName;
    PVOID           CallAddress;
} RoutineAddressType;


DEFINE_GUID(DUMMYGUID, 
0xDEADBEEF, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

PWSTR const
dummy_pci_id = L"PCI\\CC_0C0330";

PWSTR const
XhciCompatibleIdList[] = {
    L"PCI\\CC_0C0330"
};


//////////////////////////////////////////////////////////
// static writable variables

#define                 MAX_WorkerRoutineIndex  256
WorkerRoutineArrayType  WorkerRoutineArray [MAX_WorkerRoutineIndex];

//pVOID                     g_pKeFlushQueuedDpcs            = NULL;
//pfBOOLEAN__ULONG_ULONG    g_pExAcquireRundownProtectionEx = NULL;
//pfULONG_ULONG             g_pExReleaseRundownProtectionEx = NULL;
pVOID__VOID                 g_pKeRevertToUserAffinityThread = NULL;

ULONG           g_LastUsedArray_Entry;
KSPIN_LOCK      g_SpinWorkerRoutineArray;
KIRQL           g_OldIrql;

// static writable
//////////////////////////////////////////////////////////


VOID
Initialize (VOID)
{
    UNICODE_STRING  funcName;
    ULONG           i;
    KIRQL           irql;

     KeInitializeSpinLock(&g_SpinWorkerRoutineArray);

    KeAcquireSpinLock(&g_SpinWorkerRoutineArray, &irql);
    {
        g_LastUsedArray_Entry = 0;
        RtlZeroMemory(WorkerRoutineArray, sizeof(WorkerRoutineArray));
    }
    KeReleaseSpinLock(&g_SpinWorkerRoutineArray, irql);
}


NTSTATUS
DriverEntry(                // Dummy entry
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath )
{
    return STATUS_SUCCESS;
}


NTSTATUS
DllInitialize(                // Main entry
    PUNICODE_STRING  RegistryPath )
{
     /*     // debug loop
    __asm {
        L1:
        jmp L1
    }
     */
    
    Initialize();
    StorportInitialize();
    return STATUS_SUCCESS;
}


NTSTATUS
DllUnload (VOID)
{
    return STATUS_SUCCESS;
}


VOID
KeFlushQueuedDpcs_k8 (VOID)    // WinXP RTM, SP1
{
    /* TODO: find KeRevertToUserAffinityThread in ntoskrnl, g_pKeRevertToUserAffinityThread=
    
    PKTHREAD CurrentThread =     KeGetCurrentThread();
    KPRIORITY PrevPriority =     KeSetPriorityThread(CurrentThread, HIGH_PRIORITY);

    KAFFINITY ActiveProcessors = KeQueryActiveProcessors();
    ULONG CurrentProcessor =     KeGetCurrentProcessorNumber();
    KAFFINITY CurrentAffinity =  ActiveProcessors & ~((KAFFINITY) 1 << CurrentProcessor);
    if (CurrentAffinity) {
        do {
            KeSetSystemAffinityThread(CurrentAffinity);
            CurrentAffinity &= ~((KAFFINITY) 1 << CurrentProcessor);
        }
        while (CurrentAffinity);
            g_pKeRevertToUserAffinityThread();
    }
    
    KeSetPriorityThread(CurrentThread, PrevPriority);
    */
    
    return; // do nothing as WinXP SP2 does
}


NTSTATUS
RtlQueryRegistryValuesEx_k8 (
    ULONG                       RelativeTo,
    PCWSTR                      Path,
    PRTL_QUERY_REGISTRY_TABLE   QueryTable,
    PVOID                       Context,
    PVOID                       Environment )
{
    return RtlQueryRegistryValues(RelativeTo, Path, QueryTable, Context, Environment);
}


NTSTATUS
PoDisableSleepStates_k8 (ULONG a, ULONG b, ULONG c)        // undocumented
{
    return STATUS_SUCCESS;
}


NTSTATUS
RtlCheckPortableOperatingSystem_k8 (PBOOLEAN IsPortable)    // RE win8_ntoskrnl
{
    
    if (RtlCheckRegistryKey(RTL_REGISTRY_CONTROL, L"MiniNT") == 0) { // SUCCESS
        *IsPortable=TRUE;
        return STATUS_SUCCESS;
    }
    
    return STATUS_NOT_FOUND;
}


NTSTATUS
RtlSetPortableOperatingSystem_k8 (BOOLEAN IsPortable)
{
    return STATUS_SUCCESS;
}


NTSTATUS
IoSetActivityIdIrp_k8 (
    PIRP    Irp,
    LPCGUID Guid )
{
    return STATUS_SUCCESS;
}


NTSTATUS
IoGetActivityIdIrp_k8 (
    PIRP   Irp,
    LPGUID Guid )
{
    RtlCopyMemory(  Guid,
                    &DUMMYGUID,
                    sizeof(GUID));

    return STATUS_SUCCESS;
}


NTSTATUS
EtwRegister_k8 (
    LPCGUID             ProviderId,
    PETWENABLECALLBACK  EnableCallback,
    PVOID               CallbackContext,
    PREGHANDLE          RegHandle )
{
    *RegHandle=(REGHANDLE)0; // is RegHandle=0 OK ?
    return STATUS_SUCCESS;
}


NTSTATUS
EtwUnregister_k8 (
    REGHANDLE RegHandle )
{
    return STATUS_SUCCESS;
}


BOOLEAN
EtwProviderEnabled_k8 (
  REGHANDLE     RegHandle,
  UCHAR         Level,
  ULONGLONG     Keyword )
{
    return FALSE;
}


BOOLEAN
EtwEventEnabled_k8 (
    REGHANDLE           RegHandle,
    PCEVENT_DESCRIPTOR  EventDescriptor )
{
    return FALSE;
}


NTSTATUS
EtwWrite_k8 (
    REGHANDLE               RegHandle,
    PCEVENT_DESCRIPTOR      EventDescriptor,
    LPCGUID                 ActivityId,
    ULONG                   UserDataCount,
    PEVENT_DATA_DESCRIPTOR  UserData )
{
    return STATUS_SUCCESS;
}


NTSTATUS
EtwActivityIdControl_k8 (
    ULONG   ControlCode,
    LPGUID  ActivityId )
{
    switch (ControlCode) {
        case EVENT_ACTIVITY_CTRL_GET_ID:
        case EVENT_ACTIVITY_CTRL_CREATE_ID:
        case EVENT_ACTIVITY_CTRL_GET_SET_ID:
            RtlCopyMemory(  ActivityId,
                            &DUMMYGUID,
                            sizeof(GUID));
        case EVENT_ACTIVITY_CTRL_SET_ID:
        case EVENT_ACTIVITY_CTRL_CREATE_SET_ID:
        default:
        ;
    }

    return STATUS_SUCCESS;
}


NTSTATUS
EtwWriteTransfer_k8 (
     REGHANDLE              RegHandle,
     PCEVENT_DESCRIPTOR     EventDescriptor,
     LPCGUID                ActivityId,
     LPCGUID                RelatedActivityId,
     ULONG                  UserDataCount,
     PEVENT_DATA_DESCRIPTOR UserData )

{
    return STATUS_SUCCESS;    
}


NTSTATUS
EtwWriteString_k8 (
     REGHANDLE  RegHandle,
     UCHAR      Level,
     ULONGLONG  Keyword,
     LPCGUID    ActivityId,
     PCWSTR     String )
{
    return STATUS_SUCCESS;
}


NTSTATUS
ZwAlpcConnectPort_k8 (                        // undocumented
    PHANDLE                     PortHandle,
    PUNICODE_STRING             PortName,
    POBJECT_ATTRIBUTES          ObjectAttributes,
    PALPC_PORT_ATTRIBUTES       PortAttributes,
    ULONG                       Flags,
    PSID                        RequiredServerSid,
    PPORT_MESSAGE               ConnectionMessage,
    PULONG                      BufferLength,
    PALPC_MESSAGE_ATTRIBUTES    OutMessageAttributes,
    PALPC_MESSAGE_ATTRIBUTES    InMessageAttributes,
    PLARGE_INTEGER              Timeout )
{
    return STATUS_SUCCESS;
}


NTSTATUS
ZwAlpcSendWaitReceivePort_k8 (                // undocumented
    HANDLE                      PortHandle,
    ULONG                       Flags,
    PPORT_MESSAGE               SendMessage,
    PALPC_MESSAGE_ATTRIBUTES    SendMessageAttributes,
    PPORT_MESSAGE               ReceiveMessage,
    PSIZE_T                     BufferLength,
    PALPC_MESSAGE_ATTRIBUTES    ReceiveMessageAttributes,
    PLARGE_INTEGER              Timeout )
{
    return STATUS_SUCCESS;
}


errno_t __ALTDECL
wcsncpy_s_k8 (                               // Wine
    wchar_t* wcDest,
    rsize_t numElement,
    const wchar_t *wcSrc,
    rsize_t count )
{
    #define MSVCRT_EINVAL    22
    #define MSVCRT_ERANGE    34
    #define MSVCRT_STRUNCATE 80
    #define MSVCRT__TRUNCATE ((size_t)-1)

    WCHAR *p = wcDest;
    BOOLEAN truncate = (count == MSVCRT__TRUNCATE);

    if(!wcDest && !numElement && !count)
        return 0;

    if (!wcDest || !numElement)
        return MSVCRT_EINVAL;

    if (!wcSrc)
    {
        *wcDest = 0;
        return count ? MSVCRT_EINVAL : 0;
    }

    while (numElement && count && *wcSrc)
    {
        *p++ = *wcSrc++;
        numElement--;
        count--;
    }
    if (!numElement && truncate)
    {
        *(p-1) = 0;
        return MSVCRT_STRUNCATE;
    }
    else if (!numElement)
    {
        *wcDest = 0;
        return MSVCRT_ERANGE;
    }

    *p = 0;
    return 0; 
}


BOOLEAN
KdRefreshDebuggerNotPresent_k8 (VOID)
 {
    return KD_DEBUGGER_NOT_PRESENT;
 }


NTSTATUS
IoGetDevicePropertyData_k8 (
    PDEVICE_OBJECT    Pdo,
    CONST DEVPROPKEY  *PropertyKey,
    LCID              Lcid,
    ULONG             Flags,
    ULONG             Size,
    PVOID             Data,
    PULONG            RequiredSize,
    PDEVPROPTYPE      Type )
{
    if (RtlEqualMemory( PropertyKey,
                        &DEVPKEY_Device_MatchingDeviceId,
                        sizeof(DEVPROPKEY)) ) {
        //TODO: find DeviceId on PDO
        *Type=DEVPROP_TYPE_STRING;
    }

    return STATUS_SUCCESS;
}


NTSTATUS
IoSetDevicePropertyData_k8 (
    PDEVICE_OBJECT     Pdo,
    CONST DEVPROPKEY   *PropertyKey,
    LCID               Lcid,
    ULONG              Flags,
    DEVPROPTYPE        Type,
    ULONG              Size,
    PVOID              Data )
{

    return STATUS_SUCCESS;
}


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


VOID
IoQueueWorkItemCompatible (
    PDEVICE_OBJECT  DeviceObject,
    PVOID           Context )
{
    ULONG                   i;
    BOOLEAN                 foundContext = FALSE;
    PIO_WORKITEM            IoWorkItem;
    KIRQL                   irql;
    PIO_WORKITEM_ROUTINE_EX WorkerRoutineEx;

    KeAcquireSpinLock(&g_SpinWorkerRoutineArray, &irql);
    for (i = 0; i <= g_LastUsedArray_Entry; i++) {
        if (WorkerRoutineArray[i].IoWorkItem != NULL &&
            WorkerRoutineArray[i].Context == Context ) {         // match by context
                foundContext    = TRUE;
                IoWorkItem      = WorkerRoutineArray[i].IoWorkItem;
                WorkerRoutineEx = WorkerRoutineArray[i].WorkerRoutineEx;
                WorkerRoutineArray[i].IoWorkItem = NULL;        // free entry
                if (g_LastUsedArray_Entry == i && i > 0)
                        g_LastUsedArray_Entry--;
                break;
        }
    }
    KeReleaseSpinLock(&g_SpinWorkerRoutineArray, irql); 

    if (foundContext)
        WorkerRoutineEx(DeviceObject, Context, IoWorkItem);
    else {
        DbgPrint("IoQueueWork Context not found");
        KeBugCheckEx(0xDEADBEEF, 0x0, (ULONG_PTR)Context, (ULONG_PTR)DeviceObject,  0x0);
    }
    
}


VOID
IoFreeWorkItem_k8 (
    PIO_WORKITEM IoWorkItem )
{
    ULONG   i;
    KIRQL   irql;

    KeAcquireSpinLock(&g_SpinWorkerRoutineArray, &irql); {
        for (i = 0; i <= g_LastUsedArray_Entry; i++) {
            if (WorkerRoutineArray[i].IoWorkItem == IoWorkItem) {
                    WorkerRoutineArray[i].IoWorkItem = NULL;            // free entry
                    if (g_LastUsedArray_Entry == i && i > 0)
                            g_LastUsedArray_Entry--;
                    break;
            }
        }
    }
    KeReleaseSpinLock(&g_SpinWorkerRoutineArray, irql);    

    IoFreeWorkItem(IoWorkItem);
}


VOID
IoQueueWorkItemEx_k8 (
    PIO_WORKITEM            IoWorkItem,
    PIO_WORKITEM_ROUTINE_EX WorkerRoutineEx,
    WORK_QUEUE_TYPE         QueueType,
    PVOID                   Context )
{
    ULONG    i;
    BOOLEAN  foundFreeEntry = FALSE;
    KIRQL    irql;

    KeAcquireSpinLock(&g_SpinWorkerRoutineArray, &irql); {
        for (i = 0; i < MAX_WorkerRoutineIndex; i++) {
            if (WorkerRoutineArray[i].IoWorkItem == NULL) {
                foundFreeEntry = TRUE;
                if (i > g_LastUsedArray_Entry)
                            g_LastUsedArray_Entry = i;
                WorkerRoutineArray[i].WorkerRoutineEx = WorkerRoutineEx;
                WorkerRoutineArray[i].IoWorkItem      = IoWorkItem;
                WorkerRoutineArray[i].Context         = Context;
                break;
            }
        }
    }
    KeReleaseSpinLock(&g_SpinWorkerRoutineArray, irql);

    if (foundFreeEntry) {
        IoQueueWorkItem( IoWorkItem,
                         (PIO_WORKITEM_ROUTINE) IoQueueWorkItemCompatible,
                         QueueType,
                         Context);
    }
    else {
        DbgPrint("WorkerRoutineArray overflow");
        KeBugCheckEx(0xDEADBEEF, 0x1, (ULONG_PTR)Context, (ULONG_PTR)IoWorkItem, 0x0);
    }

}


ULONG
IoSizeofWorkItem_k8 (VOID)
{
    return sizeof(IO_WORKITEM);
}
 
 
VOID
IoInitializeWorkItem_k8 (
   PVOID        IoObject,
   PIO_WORKITEM IoWorkItem )
{
    PIO_WORKITEM tmpWorkItem = IoAllocateWorkItem(IoObject);
    RtlCopyMemory(IoWorkItem, tmpWorkItem, sizeof(IO_WORKITEM));
    IoFreeWorkItem(tmpWorkItem);
}


VOID
IoUninitializeWorkItem_k8 (
    PIO_WORKITEM IoWorkItem )
{
    ; // nothing to do
}


NTSTATUS
KeQueryDpcWatchdogInformation_k8 (
    PKDPC_WATCHDOG_INFORMATION WatchdogInformation )
{
    WatchdogInformation->DpcTimeLimit       = 0;
    WatchdogInformation->DpcTimeCount       = 0;
    WatchdogInformation->DpcWatchdogLimit   = 0;
    WatchdogInformation->DpcWatchdogCount   = 0;

    return STATUS_SUCCESS;
}


////////////////////////////////////////////
//     proc/node/group

ULONG 
KeGetProcessorIndexFromNumber_k8 (              // RE procgrp.lib
    PPROCESSOR_NUMBER ProcNumber ) 
{
    if (ProcNumber->Group    != 0                  ||
        ProcNumber->Number   >= KeNumberProcessors ||
        ProcNumber->Reserved != 0 )
        return INVALID_PROCESSOR_INDEX;

    return ProcNumber->Number;
}


NTSTATUS
KeGetProcessorNumberFromIndex_k8 (              // RE procgrp.lib
    ULONG               ProcIndex,
    PPROCESSOR_NUMBER   ProcNumber )
{
    if (ProcIndex >= (ULONG)KeNumberProcessors)
        return STATUS_INVALID_PARAMETER;
        
    ProcNumber->Group=          0;
    ProcNumber->Number= (UCHAR) ProcIndex;
    ProcNumber->Reserved=       0;
    return STATUS_SUCCESS;
}



ULONG
KeQueryActiveProcessorCount_k8 (
    PKAFFINITY ActiveProcessors )
{
    if (ActiveProcessors != NULL)
            *ActiveProcessors = KeQueryActiveProcessors();
    
    return KeNumberProcessors;
}


ULONG 
KeQueryActiveProcessorCountEx_k8 (              // RE procgrp.lib
    USHORT GroupNumber )
{
    if ( GroupNumber != 0 && GroupNumber != ALL_PROCESSOR_GROUPS )
        return 0;

    return KeNumberProcessors;
}


ULONG
KeGetCurrentProcessorNumberEx_k8 (              // RE procgrp.lib
    PPROCESSOR_NUMBER ProcNumber )
{
    ULONG CurrentProcessorNumber;
    
    CurrentProcessorNumber= KeGetCurrentProcessorNumber();
    if (ProcNumber != NULL) {
            ProcNumber->Group=           0;
            ProcNumber->Number=  (UCHAR) CurrentProcessorNumber;
            ProcNumber->Reserved=        0;
    }

    return CurrentProcessorNumber;
}


ULONG
KeQueryMaximumProcessorCount_k8 (VOID)          // RE procgrp.lib
{
    return KeNumberProcessors;
}


ULONG
KeQueryMaximumProcessorCountEx_k8 (             // RE procgrp.lib
    USHORT GroupNumber )
{
    if (GroupNumber == 0 || GroupNumber == ALL_PROCESSOR_GROUPS)
        return KeNumberProcessors;

    return 0; // error
}


USHORT
KeQueryActiveGroupCount_k8 (VOID)
{
    return 1;  // active groups = 1 
}


USHORT
KeQueryMaximumGroupCount_k8 (VOID)
{
    return 1;  // total groups = 1 
}


KAFFINITY
KeSetSystemAffinityThreadEx_k8 (                // RE procgrp.lib
    KAFFINITY Affinity )
{
    KeSetSystemAffinityThread(Affinity);
    return (KAFFINITY)0;
}


VOID
KeSetSystemGroupAffinityThread_k8 (             // RE procgrp.lib
    PGROUP_AFFINITY Affinity,
    PGROUP_AFFINITY PreviousAffinity )
{
    KAFFINITY PrevAffinity;
    KAFFINITY NewAffinity = 0;
    
    if (Affinity->Group       == 0  &&
        Affinity->Reserved[0] == 0  &&
        Affinity->Reserved[1] == 0  &&
        Affinity->Reserved[2] == 0)
            NewAffinity= Affinity->Mask;
    
    
    PrevAffinity=KeSetSystemAffinityThreadEx_k8(NewAffinity);
    
    if (PreviousAffinity != NULL)
    {
        PreviousAffinity->Mask        = PrevAffinity;
        PreviousAffinity->Group       = 0;
        PreviousAffinity->Reserved[0] = 0;
        PreviousAffinity->Reserved[1] = 0;
        PreviousAffinity->Reserved[2] = 0;
    }
}


VOID
KeRevertToUserAffinityThreadEx_k8 (             // RE procgrp.lib
    KAFFINITY Affinity )
{
    if ( Affinity == 0)
        KeRevertToUserAffinityThread();
}


VOID
KeRevertToUserGroupAffinityThread_k8 (          // RE procgrp.lib
    PGROUP_AFFINITY PreviousAffinity )
{
    if (PreviousAffinity->Reserved[0] != 0 ||
        PreviousAffinity->Reserved[1] != 0 ||
        PreviousAffinity->Reserved[2] != 0 )
        return;
        
    if (PreviousAffinity->Mask == 0)
        KeRevertToUserAffinityThread();
    else {
        if (PreviousAffinity->Group == 0)
            KeRevertToUserAffinityThreadEx_k8(PreviousAffinity->Mask);
    }
}


NTSTATUS
KeSetTargetProcessorDpcEx_k8 (                  // RE procgrp.lib
    PKDPC               Dpc,
    PPROCESSOR_NUMBER   ProcNumber )
{
    if (ProcNumber->Group    != 0                  ||
        ProcNumber->Number   >= KeNumberProcessors ||
        ProcNumber->Reserved != 0)
            return STATUS_INVALID_PARAMETER;
        
    KeSetTargetProcessorDpc(Dpc, ProcNumber->Number);
    return STATUS_SUCCESS;    
}


USHORT
KeGetCurrentNodeNumber_k8 (VOID)
{
    return 0; // current node = 0
}


USHORT
KeQueryHighestNodeNumber_k8 (VOID)
{
    return 0; // total nodes = 0
}


VOID
KeQueryNodeActiveAffinity_k8 (
    USHORT          NodeNumber,
    PGROUP_AFFINITY Affinity,
    PUSHORT         Count )
{
    if (NodeNumber > 0) {
        if (Affinity != NULL) { 
            Affinity->Group         = 0;
            Affinity->Mask          = 0;
            Affinity->Reserved[0]   = 0;
            Affinity->Reserved[1]   = 0;
            Affinity->Reserved[2]   = 0;
        }
        
        if (Count != NULL)
            *Count= 0;    
    }

    else {
        if (Affinity != NULL) { 
            Affinity->Group         = 0;
            Affinity->Mask          = KeQueryActiveProcessors();
            Affinity->Reserved[0]   = 0;
            Affinity->Reserved[1]   = 0;
            Affinity->Reserved[2]   = 0;
        }
        
        if (Count != NULL)
            *Count= KeNumberProcessors;
    }
}


KAFFINITY
KeQueryGroupAffinity_k8 (                       // RE procgrp.lib
    USHORT  GroupNumber )
{
    if (GroupNumber != 0)
        return (KAFFINITY)0;
    
    return KeQueryActiveProcessors();    
}


NTSTATUS
IoGetAffinityInterrupt_k8 (                     // RE iointex.lib
    PKINTERRUPT     InterruptObject,
    PGROUP_AFFINITY GroupAffinity )
{
    GroupAffinity->Mask         = KeQueryActiveProcessors();
    GroupAffinity->Group        = 0;
    GroupAffinity->Reserved[0]  = 0;
    GroupAffinity->Reserved[1]  = 0;
    GroupAffinity->Reserved[2]  = 0;
    
    return STATUS_SUCCESS;    
}

#define GetGroupAffinityAffinity(x)         ( \
        IoGetAffinityInterrupt_k8(0,x)      )


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
NtQuerySystemInformation (
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
    );


NTSTATUS
KeQueryLogicalProcessorRelationship_k8 (
    PPROCESSOR_NUMBER                           pProcessorNumber,
    LOGICAL_PROCESSOR_RELATIONSHIP              RelationshipType,
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX    Information,
    PULONG                                      pLength
    )
{
    #define     SizeofHeader    sizeof(LOGICAL_PROCESSOR_RELATIONSHIP) + \
                                sizeof(ULONG)

    ULONG       ProcessorFilter   = 0;
    ULONG       CollectedLength   = 0;
    BOOLEAN     LengthMode        = FALSE;
    UCHAR       *Buffer;
    ULONG       Length            = 0;
    ULONG       Status;
    ULONG       i;
    ULONG       ProcessorCore;
    UCHAR       *pInformation;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX  NewEntry;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION     *CurrentInfo;
    KAFFINITY   Mask;
    UCHAR       Flags;
    UCHAR       TotalProcessors   = 0;
    UCHAR       SMT               = 0;
    UCHAR       TotalCores;
    KAFFINITY   PackageMask;
    KAFFINITY   ProcessorCoreMask = 1;
    KAFFINITY   TotalCoresMask    = 0;

    Buffer = ExAllocatePoolWithTag( NonPagedPool, 65536 * sizeof(UCHAR), 200);

    //TODO: Bug in NtQuerySystemInformation, it never report SMT detection
    Status = NtQuerySystemInformation( SystemLogicalProcessorInformation,
                                     Buffer,
                                     65536 * sizeof(UCHAR),
                                     &Length);
    if (!Status) {
        for (i = 0; i < (ULONG) Length/sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++)
        {
            CurrentInfo=(SYSTEM_LOGICAL_PROCESSOR_INFORMATION *) &Buffer[i * sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION)];
            if (CurrentInfo->Relationship == RelationProcessorCore) {
                Mask  = CurrentInfo->ProcessorMask;
                Flags = CurrentInfo->ProcessorCore.Flags;

                if (Flags)
                        SMT++;

                if (Mask)
                        TotalProcessors++;

            }
        }
        
    }
    ExFreePoolWithTag(Buffer, 200);
                
    if (SMT)
        PackageMask= (KAFFINITY)3;
    else
        PackageMask= (KAFFINITY)1;


    if (Information == NULL) 
        LengthMode = TRUE;

    if (pProcessorNumber != NULL) {
        if (pProcessorNumber->Reserved != 0 ||
            pProcessorNumber->Group    != 0 ||
            pProcessorNumber->Number   >= 32 )
            ProcessorFilter=INVALID_PROCESSOR_INDEX;
        else
            ProcessorFilter=pProcessorNumber->Number;

        if (ProcessorFilter >= (ULONG)KeNumberProcessors)
            return STATUS_INVALID_PARAMETER;
    }
 

    //TODO: Filter PROCESSOR_NUMBER
    for (i = 0; i < TotalProcessors; i++) 
    {
        if (i != 0) {    // skip first time
            if (SMT)
                PackageMask= PackageMask << 2;
            else
                PackageMask= PackageMask << 1;
        }

        if (
             (RelationshipType == RelationProcessorPackage || RelationshipType == RelationAll) &&
            ((ProcessorFilter  && ProcessorFilter == i)    || ProcessorFilter  == 0)    // filter anything but requested processor
           )
        {
             /* {
                    BYTE            Flags;
                    BYTE            Reserved[21];
                    WORD            GroupCount;
                    GROUP_AFFINITY  GroupMask[ANYSIZE_ARRAY];
                } PROCESSOR_RELATIONSHIP    */

            if (!LengthMode) {
                #define sizeof_ProcessorPackage (sizeof(PROCESSOR_RELATIONSHIP) + SizeofHeader)
                NewEntry = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
                          ((UCHAR *)Information + CollectedLength);
                RtlZeroMemory(NewEntry, sizeof_ProcessorPackage);

                NewEntry->Relationship          = RelationProcessorPackage;
                NewEntry->Size                  = sizeof_ProcessorPackage;
                NewEntry->Processor.Flags       = 0;
                NewEntry->Processor.Reserved[0] = 0;    // EfficiencyClass, win10 only
                NewEntry->Processor.GroupCount  = 1;

                NewEntry->Processor.GroupMask[0].Mask        = PackageMask;
                NewEntry->Processor.GroupMask[0].Group       = 0;
                NewEntry->Processor.GroupMask[0].Reserved[0] = 0;
            }
            
            CollectedLength += sizeof_ProcessorPackage;
        }

        for (ProcessorCore = 0; ProcessorCore <= SMT; ProcessorCore++)
        {
            if (i != 0)             // skip first time
             ProcessorCoreMask = ProcessorCoreMask << 1;

            if (ProcessorFilter && ProcessorFilter != i)
            {
                continue;
            }

            if (RelationshipType == RelationProcessorCore    || RelationshipType == RelationAll )
            {
                if (!LengthMode) {
                    #define sizeof_ProcessorCore (sizeof(PROCESSOR_RELATIONSHIP) + SizeofHeader)
                    NewEntry = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
                              ((UCHAR *)Information + CollectedLength);
                    RtlZeroMemory(NewEntry, sizeof_ProcessorCore);

                    NewEntry->Relationship    = RelationProcessorCore;
                    NewEntry->Size            = sizeof_ProcessorCore;
                    // TODO: set Flags=LTP_PC_SMT if core has more than one logical processor
                    NewEntry->Processor.Flags       = 0;
                    NewEntry->Processor.Reserved[0] = 0;    // EfficiencyClass, win10 only
                    NewEntry->Processor.GroupCount  = 1;

                    NewEntry->Processor.GroupMask[0].Mask        = ProcessorCoreMask;
                    NewEntry->Processor.GroupMask[0].Group       = 0;
                    NewEntry->Processor.GroupMask[0].Reserved[0] = 0;
                }
                
                CollectedLength += sizeof_ProcessorCore; 
            }

            if (RelationshipType == RelationCache ||
                RelationshipType == RelationAll   ||
                ProcessorCore    == 0)   // show cache only on first thread
            {
                /* {
                    BYTE                    Level;
                    BYTE                    Associativity;
                    WORD                    LineSize;
                    DWORD                   CacheSize;
                    PROCESSOR_CACHE_TYPE    Type;
                    BYTE                    Reserved[20];
                    GROUP_AFFINITY          GroupMask;
                } CACHE_RELATIONSHIP */

                if (!LengthMode) {
                    #define sizeof_Cache (sizeof(CACHE_RELATIONSHIP) + SizeofHeader)
                    NewEntry = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
                              ((UCHAR *)Information + CollectedLength);
                    RtlZeroMemory(NewEntry, sizeof_Cache);

                    NewEntry->Relationship          = RelationCache;
                    NewEntry->Size                  = sizeof_Cache;
                    NewEntry->Cache.Level           = 1;  // L1 
                    NewEntry->Cache.Associativity   = 8;  // L1
                    NewEntry->Cache.LineSize        = 64;
                    NewEntry->Cache.CacheSize       = 16384;
                    NewEntry->Cache.Type            = CacheUnified;
                    NewEntry->Cache.Reserved[0]     = 0;

                    NewEntry->Cache.GroupMask.Mask          = PackageMask;
                    NewEntry->Cache.GroupMask.Group         = 0;
                    NewEntry->Cache.GroupMask.Reserved[0]   = 0;
                }
                
                CollectedLength += sizeof_Cache;
            }
        }
    }

    TotalCores = TotalProcessors + TotalProcessors*SMT;
    for (i = 0; i < TotalCores; i++)
    {
        TotalCoresMask =  TotalCoresMask | ((KAFFINITY)1 << i);
    }

    if (RelationshipType == RelationNumaNode || RelationshipType == RelationAll )
    {
        /* {
            ULONG        NodeNumber;
            UCHAR        Reserved[20];
            GROUP_AFFINITY    GroupMask;
           } NUMA_NODE_RELATIONSHIP
        */
        if (!LengthMode) {
            #define sizeof_Numa (sizeof(NUMA_NODE_RELATIONSHIP) + SizeofHeader)
            NewEntry = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
                      ((UCHAR *)Information + CollectedLength);
            RtlZeroMemory(NewEntry, sizeof_Numa);

            NewEntry->Relationship          = RelationNumaNode;
            NewEntry->Size                  = sizeof_Numa;
            NewEntry->NumaNode.NodeNumber   = 0;
            NewEntry->NumaNode.Reserved[0]  = 0;

            NewEntry->NumaNode.GroupMask.Mask         = TotalCoresMask; 
            NewEntry->NumaNode.GroupMask.Group        = 0;
            NewEntry->NumaNode.GroupMask.Reserved[0]  = 0;
        }
        
        CollectedLength += sizeof_Numa;
    }

    if ((RelationshipType == RelationGroup ||
         RelationshipType == RelationAll)  &&
        ProcessorFilter == 0) // skip groups if processor was filtered
    {
        /* {
            WORD                 MaximumGroupCount;
            WORD                 ActiveGroupCount;
            BYTE                 Reserved[20];
            PROCESSOR_GROUP_INFO GroupInfo[ANYSIZE_ARRAY];
        } GROUP_RELATIONSHIP,
        
          {
            BYTE  MaximumProcessorCount;
            BYTE  ActiveProcessorCount;
            BYTE  Reserved[38];
            KAFFINITY ActiveProcessorMask;
        } PROCESSOR_GROUP_INFO  */

        if (!LengthMode) {
            #define sizeof_Group (sizeof(GROUP_RELATIONSHIP) + SizeofHeader)
            NewEntry = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)
                      ((UCHAR *)Information + CollectedLength);
            RtlZeroMemory(NewEntry, sizeof_Group);
    
            NewEntry->Relationship              = RelationGroup;
            NewEntry->Size                      = sizeof_Group;
            NewEntry->Group.MaximumGroupCount   = 1;
            NewEntry->Group.ActiveGroupCount    = 1;
            NewEntry->Group.Reserved[0]         = 0;
            
            NewEntry->Group.GroupInfo[0].ActiveProcessorCount   = TotalCores;
            NewEntry->Group.GroupInfo[0].MaximumProcessorCount  = TotalCores;
            NewEntry->Group.GroupInfo[0].Reserved[0]            = 0;
            NewEntry->Group.GroupInfo[0].ActiveProcessorMask    = TotalCoresMask;
        }

        CollectedLength += sizeof_Group;
    }
    
    if (LengthMode) {
        *pLength= CollectedLength;
        return STATUS_INFO_LENGTH_MISMATCH; // Length Mode Status
    }

    *pLength= CollectedLength;
    return STATUS_SUCCESS;    
}

//     proc/node/group
////////////////////////////////////////////


/*
///////////////////////////////////////////
//  debug KeQueryLogicalProcessorRelationship

#define TEMP_BUFFER_SIZE        128
#define TRACE_LEVEL_CRITICAL    DPFLTR_ERROR_LEVEL        
#define TRACE_LEVEL_FATAL       DPFLTR_ERROR_LEVEL        
#define TRACE_LEVEL_ERROR       DPFLTR_ERROR_LEVEL       
#define TRACE_LEVEL_WARNING     DPFLTR_WARNING_LEVEL      
#define TRACE_LEVEL_INFORMATION DPFLTR_TRACE_LEVEL   
#define TRACE_LEVEL_VERBOSE     DPFLTR_INFO_LEVEL   

#include <ntstrsafe.h>

VOID
TraceGUMessage(
    IN PCCHAR  func,
    IN PCCHAR  file,
    IN ULONG   line,
    IN ULONG   level,
    IN PCCHAR  format,
    ...
    )

 {
    va_list    list;
    NTSTATUS   status;
    char psPrefix [TEMP_BUFFER_SIZE];
    PCCHAR  fileName;

    va_start(list, format);

    fileName = strrchr(file, '\\');

    if (fileName != NULL)
    {
        fileName++;
    }
    
    
    if(level == TRACE_LEVEL_ERROR) 
    {
        status = RtlStringCchPrintfA(psPrefix, TEMP_BUFFER_SIZE, "%s", " ");
    }
    else
    {
        status = RtlStringCchPrintfA(psPrefix, TEMP_BUFFER_SIZE, "%s", " ");
        level = TRACE_LEVEL_ERROR;
    }
    

    ASSERT(NT_SUCCESS(status));
    vDbgPrintExWithPrefix(psPrefix , DPFLTR_IHVNETWORK_ID, level, format, list);

    va_end(list);

}


#define GU_PRINT(_level_,_flag_, _format_, ...)                               \
        TraceGUMessage(__FUNCTION__, __FILE__, __LINE__, _level_, _format_, __VA_ARGS__);  \

#if (NTDDI_VERSION <= NTDDI_VISTASP4)
    #define KeQueryLogicalProcessorRelationship     \
            KeQueryLogicalProcessorRelationship_k8
#endif

void PrintNumaCpuConfiguration()
{
    USHORT NodeNumber;
    NTSTATUS Status;
    GROUP_AFFINITY GroupAffinity;
    USHORT NodeCount, HighestNodeNumber;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Info = NULL;
    ULONG BufferSize = 0;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pCurrInfo;  
    USHORT i;

    //
    // Get required buffer size.
    //
    PROCESSOR_NUMBER Filter;

    Filter.Group    = 0;
    Filter.Number   = 3;
    Filter.Reserved = 0;

    Status = KeQueryLogicalProcessorRelationship(NULL, RelationAll, NULL, &BufferSize);

    ASSERT(Status == STATUS_INFO_LENGTH_MISMATCH && BufferSize > 0);

    //
    // Allocate buffer (assume IRQL <= APC_LEVEL).
    //
    Info = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX) ExAllocatePoolWithTag(PagedPool, BufferSize, ' gaT');
    if (Info == NULL)
    {
        return;
    }

    //
    // Get processor relationship information.
    //
    Status = KeQueryLogicalProcessorRelationship(NULL, RelationAll, Info, &BufferSize);

    if(Status != STATUS_SUCCESS)
    {
        return;
    }

    GU_PRINT(TRACE_LEVEL_ERROR, GU, "\n\nSystem processor information:\n");


    for(pCurrInfo = Info;
        (char *) pCurrInfo < (char *) Info + BufferSize;
        pCurrInfo = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX) ((char *) pCurrInfo + pCurrInfo->Size))
    {
        switch(pCurrInfo->Relationship)
        {
        case RelationProcessorCore:
            GU_PRINT(TRACE_LEVEL_ERROR, GU, "--Processor core-- Group=%d Mask=0x%p\n", 
                pCurrInfo->Processor.GroupMask[0].Group, 
                (void *) pCurrInfo->Processor.GroupMask[0].Mask );
            break;

        case RelationProcessorPackage:
            GU_PRINT(TRACE_LEVEL_ERROR, GU, "--Processor package-- Group count %d\n", pCurrInfo->Processor.GroupCount);
            for(i = 0; i < pCurrInfo->Processor.GroupCount; i++)
            {
                GU_PRINT(TRACE_LEVEL_ERROR, GU, "Group #%d: Group=%d Mask=0x%p\n", i, 
                    pCurrInfo->Processor.GroupMask[i].Group,
                    (void *) pCurrInfo->Processor.GroupMask[i].Mask);
            }
            break;

        case RelationNumaNode:
            GU_PRINT(TRACE_LEVEL_ERROR, GU, "--Numa node-- Node #%d ProcessorGroup=%d ProcessorMask=0x%p\n", 
                pCurrInfo->NumaNode.NodeNumber, pCurrInfo->NumaNode.GroupMask.Group, (void *) pCurrInfo->NumaNode.GroupMask.Mask);
            break;

        case RelationGroup:
            GU_PRINT(TRACE_LEVEL_ERROR, GU, "--Groups info-- MaxGroupCount=%d ActiveGroupCount=%d\n", 
                pCurrInfo->Group.MaximumGroupCount, pCurrInfo->Group.ActiveGroupCount);
            for(i = 0; i < pCurrInfo->Group.ActiveGroupCount; i++)
            {
                GU_PRINT(TRACE_LEVEL_ERROR, GU, "Group #%d: MaxProcCount=%d ActiveProcCount=%d ActiveProcMask=0x%p\n", i, 
                    pCurrInfo->Group.GroupInfo[i].MaximumProcessorCount,
                    pCurrInfo->Group.GroupInfo[i].ActiveProcessorCount,
                    (void *) pCurrInfo->Group.GroupInfo[i].ActiveProcessorMask);
            }
            break;

        case RelationCache:
            GU_PRINT(TRACE_LEVEL_ERROR, GU, "--Cache info-- Level=L%d, Associativity=%d, LineSize=%d bytes, \
                CacheSize=%d bytes, Type=%d, ProcGroup=%d, ProcMask=0x%p\n", 
                pCurrInfo->Cache.Level, 
                pCurrInfo->Cache.Associativity,
                pCurrInfo->Cache.LineSize,
                pCurrInfo->Cache.CacheSize,
                pCurrInfo->Cache.Type,
                pCurrInfo->Cache.GroupMask.Group,
                (void *) pCurrInfo->Cache.GroupMask.Mask);
            break;

        default:
            ASSERT(FALSE);
            break;
        }
    }

}

//  debug
/////////////////////////////////
*/

NTSTATUS
IoConnectInterruptEx_k8 (                       // RE iointex.lib
    PIO_CONNECT_INTERRUPT_PARAMETERS  Parameters )
{
    if (Parameters->Version == CONNECT_FULLY_SPECIFIED  ||
        Parameters->Version == CONNECT_FULLY_SPECIFIED_GROUP) {
        
        if (Parameters->FullySpecified.PhysicalDeviceObject == NULL     ||
            Parameters->FullySpecified.ServiceRoutine       == NULL     ||
            Parameters->FullySpecified.SynchronizeIrql      < Parameters->FullySpecified.Irql)
            return STATUS_INVALID_PARAMETER;        

        return IoConnectInterrupt(
                Parameters->FullySpecified.InterruptObject,
                Parameters->FullySpecified.ServiceRoutine,
                Parameters->FullySpecified.ServiceContext,
                Parameters->FullySpecified.SpinLock,
                Parameters->FullySpecified.Vector,
                Parameters->FullySpecified.Irql,
                Parameters->FullySpecified.SynchronizeIrql,
                Parameters->FullySpecified.InterruptMode,
                Parameters->FullySpecified.ShareVector,
                Parameters->FullySpecified.ProcessorEnableMask,
                Parameters->FullySpecified.FloatingSave);
    }
    else {
        Parameters->Version = CONNECT_FULLY_SPECIFIED;
        return STATUS_NOT_SUPPORTED;
        
    }
}


VOID
IoDisconnectInterruptEx_k8 (                    // RE iointex.lib
    PIO_DISCONNECT_INTERRUPT_PARAMETERS Parameters )
{
    if (Parameters->Version == CONNECT_FULLY_SPECIFIED  ||
        Parameters->Version == CONNECT_FULLY_SPECIFIED_GROUP) 
        IoDisconnectInterrupt(Parameters->ConnectionContext.InterruptObject);
}


BOOLEAN
PoGetSystemWake_k8 (
    PIRP  Irp )
{
    return FALSE;
}


VOID
PoSetSystemWake_k8 (
    PIRP  Irp )
{
}


LONG_PTR FASTCALL
ObfReferenceObjectWithTag_k8 (
    PVOID Object,
    ULONG Tag )
{
    return ObfReferenceObject(Object);
}


LONG_PTR FASTCALL
ObfDereferenceObjectWithTag_k8 (
    PVOID Object,
    ULONG Tag )
{
    return ObfDereferenceObject(Object);
}


NTSTATUS
PoRegisterPowerSettingCallback_k8 (
    PDEVICE_OBJECT          DeviceObject,
    LPCGUID                 SettingGuid,
    PPOWER_SETTING_CALLBACK Callback,
    PVOID                   Context,
    PVOID                   *Handle )
{
    return STATUS_SUCCESS;    
}
    
    
NTSTATUS
PoUnregisterPowerSettingCallback_k8 (
    PVOID Handle )
{
    return STATUS_SUCCESS;    
}
 

PVOID
MmAllocateContiguousMemorySpecifyCacheNode_k8 (
    SIZE_T              NumberOfBytes,
    PHYSICAL_ADDRESS    LowestAcceptableAddress,
    PHYSICAL_ADDRESS    HighestAcceptableAddress,
    PHYSICAL_ADDRESS    BoundaryAddressMultiple,
    MEMORY_CACHING_TYPE CacheType,
    NODE_REQUIREMENT    PreferredNode )
{
    return MmAllocateContiguousMemorySpecifyCache (
                NumberOfBytes,
                LowestAcceptableAddress,
                HighestAcceptableAddress,
                BoundaryAddressMultiple,
                CacheType );
}


PVOID
MmAllocateContiguousNodeMemory_k8 (
    SIZE_T            NumberOfBytes,
    PHYSICAL_ADDRESS  LowestAcceptableAddress,
    PHYSICAL_ADDRESS  HighestAcceptableAddress,
    PHYSICAL_ADDRESS  BoundaryAddressMultiple,
    ULONG             Protect,
    NODE_REQUIREMENT  PreferredNode )
{
    /*
    #define PAGE_NOACCESS            0x01
    #define PAGE_READONLY            0x02
    #define PAGE_READWRITE           0x04
    #define PAGE_WRITECOPY           0x08
    #define PAGE_EXECUTE             0x10
    #define PAGE_EXECUTE_READ        0x20
    #define PAGE_EXECUTE_READWRITE   0x40
    #define PAGE_EXECUTE_WRITECOPY   0x80
    #define PAGE_GUARD              0x100
    #define PAGE_NOCACHE            0x200
    #define PAGE_WRITECOMBINE       0x400

    0 MmNonCached = FALSE,
    1 MmCached = TRUE,
    2 MmWriteCombined = MmFrameBufferCached,
    3 MmHardwareCoherentCached,
    4 MmNonCachedUnordered,       // IA64
    5 MmUSWCCached,
    6 MmMaximumCacheType */
    
    MEMORY_CACHING_TYPE CacheType = MmCached;

    if ( (Protect & PAGE_WRITECOMBINE) == PAGE_WRITECOMBINE )
        CacheType = MmWriteCombined;
    
    if ( (Protect & PAGE_NOCACHE)      == PAGE_NOCACHE )
        CacheType = MmNonCached;
    
    return MmAllocateContiguousMemorySpecifyCache (
                NumberOfBytes,
                LowestAcceptableAddress,
                HighestAcceptableAddress,
                BoundaryAddressMultiple,
                CacheType );
    
}


VOID FASTCALL
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


VOID FASTCALL
KeAcquireGuardedMutex_k8 (
  PKGUARDED_MUTEX  Mutex )
{
    ExAcquireFastMutex((PFAST_MUTEX)Mutex);
}


BOOLEAN FASTCALL
KeTryToAcquireGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    return ExTryToAcquireFastMutex((PFAST_MUTEX)Mutex);
}


VOID FASTCALL
KeReleaseGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    ExReleaseFastMutex((PFAST_MUTEX)Mutex);
}


VOID FASTCALL
KeAcquireGuardedMutexUnsafe_k8 (
    PKGUARDED_MUTEX FastMutex )
{
    ExAcquireFastMutexUnsafe((PFAST_MUTEX)FastMutex);
}


VOID FASTCALL
KeReleaseGuardedMutexUnsafe_k8 (
    PKGUARDED_MUTEX FastMutex )
{
    ExReleaseFastMutexUnsafe((PFAST_MUTEX)FastMutex);
}


// TODO trace OldIrql depend on caller
VOID
KeEnterGuardedRegion_k8 (VOID)
{
    KeEnterCriticalRegion();
    KeRaiseIrql(APC_LEVEL, &g_OldIrql);
}


VOID
KeLeaveGuardedRegion_k8 (VOID)
{
    KeLeaveCriticalRegion();
    KeLowerIrql(g_OldIrql);
}


NTSTATUS
DummyProc (VOID)
{
    return STATUS_SUCCESS;
}


VOID
PoStartDeviceBusy_k8 (
    PULONG IdlePointer )
{
    PoSetDeviceBusy(IdlePointer);
}


VOID
PoEndDeviceBusy_k8 (
    PULONG IdlePointer )

{
    return;
}


// mark some routines for dynamic availablity
RoutineAddressType const RoutineAddressEnabled[] = {    
    { RTL_CONSTANT_STRING(L"IoConnectInterruptEx"),     &IoConnectInterruptEx_k8    }, // wdf01000.sys 1.11.9200
    { RTL_CONSTANT_STRING(L"IoDisconnectInterruptEx"),  &IoDisconnectInterruptEx_k8 }, // wdf01000.sys 1.11.9200
};


PVOID
MmGetSystemRoutineAddress_force (
    PUNICODE_STRING SystemRoutineName )
{
    ULONG    i;
    PVOID    Address = NULL;

    Address = MmGetSystemRoutineAddress(SystemRoutineName);
    if (Address == NULL) {
        for (i = 0; i < ARRAYSIZE(RoutineAddressEnabled); i++) {
            if (RtlEqualUnicodeString(
                    &RoutineAddressEnabled[i].RoutineName,
                    SystemRoutineName, FALSE)) {
                        Address = RoutineAddressEnabled[i].CallAddress;
                        break;
                    }
        }
    }

    return Address;
}


NTSTATUS
LpcReplyWaitReplyPort_k8 (                      // undocumented
    ULONG arg0, ULONG arg1, ULONG arg2 ) 
{
    return STATUS_PORT_DISCONNECTED;
}


NTSTATUS
LpcSendWaitReceivePort_k8 (                     // undocumented
    ULONG arg0, ULONG arg1, ULONG arg2, ULONG arg3,
    ULONG arg4, ULONG arg5, ULONG arg6 ) 
{
    return STATUS_PORT_DISCONNECTED;
}



NTSTATUS
SeSetAuthorizationCallbacks_k8 (                // undocumented, exist only in Vista
    PVOID    Callback,
    ULONG    Mode )
{
    return STATUS_SUCCESS;    
}


NTSTATUS
SeSetAuditParameter_k8 (
     PSE_ADT_PARAMETER_ARRAY AuditParameters,
     SE_ADT_PARAMETER_TYPE   Type,
     ULONG                   Index,
     PVOID                   Data )
{
    return STATUS_SUCCESS;
}


NTSTATUS
SeReportSecurityEventWithSubCategory_k8 (
     ULONG                   Flags,
     PUNICODE_STRING         SourceName,
     PSID                    UserSid,
     PSE_ADT_PARAMETER_ARRAY AuditParameters,
     ULONG                   AuditSubcategoryId )
{
    return STATUS_SUCCESS;
}


NTSTATUS
PcwRegister_k8 (
     PPCW_REGISTRATION              *Registration,
     PPCW_REGISTRATION_INFORMATION  Info )
{
    return STATUS_SUCCESS;
}


VOID
PcwUnregister_k8 (
    PPCW_REGISTRATION Registration )
{
    return;
}


NTSTATUS
PcwCreateInstance_k8 (
     PPCW_INSTANCE      *Instance,
     PPCW_REGISTRATION  Registration,
     PCUNICODE_STRING   Name,
     ULONG              Count,
     PPCW_DATA          Data )
{
    return STATUS_SUCCESS;
}


VOID
PcwCloseInstance_k8 (
    PPCW_INSTANCE Instance )
{
    return;
}


NTSTATUS
PcwAddInstance_k8 (
    PPCW_BUFFER        Buffer,
    PCUNICODE_STRING   Name,
    ULONG              Id,
    ULONG              Count,
    PPCW_DATA          Data )
{
    return STATUS_SUCCESS;
}



#include "ntoskrnl_redirects.h"
