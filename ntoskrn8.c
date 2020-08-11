#undef    __STDC_WANT_SECURE_LIB__
#define   __STDC_WANT_SECURE_LIB__ 0

#include <ntddk.h> 
#include <Initguid.h>
#include <devpkey.h>

#include "common.h"
#include "wrk2003.h"


typedef void                (*pVOID__VOID)            (void);
typedef void                (*pVOID__ULONG)           (ULONG);
typedef void                (*pVOID__ULONG_ULONG)     (ULONG,ULONG);
typedef void    (__stdcall  *pfVOID__ULONG_ULONG)     (ULONG,ULONG);
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

typedef void IO_WORKITEM_ROUTINE_EX (
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


typedef struct _CreateProcessNotifyExArray {
    PCREATE_PROCESS_NOTIFY_ROUTINE_EX  Routine;
} CreateProcessNotifyExArrayType;

typedef struct _RoutineAddress {
    UNICODE_STRING  RoutineName;
    PVOID           CallAddress;
} RoutineAddressType;


#define CLEANUP_ENTRY(Array,Field,ComparedValue,LastUsed_Entry)                \
for (CLEANUP_Index = 0; CLEANUP_Index <= LastUsed_Entry; CLEANUP_Index++) {    \
    if (Array[CLEANUP_Index].Field == ComparedValue)                           \
    {                                                                          \
        Array[CLEANUP_Index].Field = NULL;                                     \
	    if (CLEANUP_Index==LastUsed_Entry && LastUsed_Entry > 0) {             \
            LastUsed_Entry--;                                                  \
            for (CLEANUP_RevIndex=LastUsed_Entry; CLEANUP_RevIndex > 0; CLEANUP_RevIndex--) {  \
                if (Array[CLEANUP_RevIndex].Field == NULL)                     \
                    LastUsed_Entry= CLEANUP_RevIndex-1;                        \
                else                                                           \
                    break;                                                     \
            }                                                                  \
        }                                                                      \
                                                                               \
        break;                                                                 \
     }                                                                         \
}



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

#define                         MAX_CreateProcessNotifyEx  256
CreateProcessNotifyExArrayType  CreateProcessNotifyExArray [MAX_CreateProcessNotifyEx];

//pVOID                     g_pKeFlushQueuedDpcs            = NULL;
//pfBOOLEAN__ULONG_ULONG    g_pExAcquireRundownProtectionEx = NULL;
//pfULONG_ULONG             g_pExReleaseRundownProtectionEx = NULL;
pVOID__VOID                 g_pKeRevertToUserAffinityThread = NULL;

ULONG           g_LastUsedWorkerRoutineArray_Entry;
ULONG           g_LastUsedCreateProcessNotifyExArray_Entry;
KSPIN_LOCK      g_SpinWorkerRoutineArray;
KSPIN_LOCK      g_SpinCreateProcessNotifyExArray;
KIRQL           g_GuardedRegion_OldIrql;
LONG            g_GuardedRegionCounter;


// static writable
//////////////////////////////////////////////////////////


void
Initialize (PUNICODE_STRING  RegistryPath)
{
    KLOCK_QUEUE_HANDLE  LockHandle;

    g_GuardedRegionCounter = 0;

    KeInitializeSpinLock(&g_SpinWorkerRoutineArray);
    KeAcquireInStackQueuedSpinLock(&g_SpinWorkerRoutineArray, &LockHandle);
    {
        g_LastUsedWorkerRoutineArray_Entry = 0;
        RtlZeroMemory(WorkerRoutineArray, sizeof(WorkerRoutineArray));
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);

    KeInitializeSpinLock(&g_SpinCreateProcessNotifyExArray);
    KeAcquireInStackQueuedSpinLock(&g_SpinCreateProcessNotifyExArray, &LockHandle);
    {
        g_LastUsedCreateProcessNotifyExArray_Entry = 0;
        RtlZeroMemory(CreateProcessNotifyExArray, sizeof(CreateProcessNotifyExArray));
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);

}


void
KeFlushQueuedDpcs_k8 (void)    // WinXP RTM, SP1
{
    /* TODO: find KeRevertToUserAffinityThread in ntoskrnl, g_pKeRevertToUserAffinityThread=
    
    PKTHREAD CurrentThread =     KeGetCurrentThread();
    KPRIORITY PrevPriority =     KeSetPriorityThread(CurrentThread, HIGH_PRIORITY);

    KAFFINITY ActiveProcessors = KeQueryActiveProcessors();
    ULONG CurrentProcessor =     KeGetCurrentProcessorNumber();
    KAFFINITY CurrentAffinity =  ActiveProcessors & ~((KAFFINITY) 1 << CurrentProcessor);
    if (CurrentAffinity) {
        do {
            KeSetSystemAffinityThread_k8(CurrentAffinity);
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
    if (RtlCheckRegistryKey(RTL_REGISTRY_CONTROL, L"MiniNT") == STATUS_SUCCESS)
        *IsPortable=TRUE;
    else   
        *IsPortable=FALSE;
    
    return STATUS_SUCCESS;
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
            break;
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



BOOLEAN
KdRefreshDebuggerNotPresent_k8 (void)
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


void
IoQueueWorkItemCompatible (
    PDEVICE_OBJECT  DeviceObject,
    PVOID           Context )
{
    ULONG                   i;
    BOOLEAN                 foundContext = FALSE;
    PIO_WORKITEM            IoWorkItem = NULL;
    KLOCK_QUEUE_HANDLE      LockHandle;
    PIO_WORKITEM_ROUTINE_EX WorkerRoutineEx;

    KeAcquireInStackQueuedSpinLock(&g_SpinWorkerRoutineArray, &LockHandle);
    {
        for (i = 0; i <= g_LastUsedWorkerRoutineArray_Entry; i++) {
            if (WorkerRoutineArray[i].IoWorkItem != NULL &&
                WorkerRoutineArray[i].Context == Context ) {         // match by Context
                    foundContext    = TRUE;
                    IoWorkItem      = WorkerRoutineArray[i].IoWorkItem;
                    WorkerRoutineEx = WorkerRoutineArray[i].WorkerRoutineEx;

                    WorkerRoutineArray[i].IoWorkItem = NULL;        // free entry
                    
                    // shrink WorkerRoutineArray if possible
                    if (i == g_LastUsedWorkerRoutineArray_Entry && g_LastUsedWorkerRoutineArray_Entry > 0)
                            g_LastUsedWorkerRoutineArray_Entry--;
                    break;
            }
        }
    }    
    KeReleaseInStackQueuedSpinLock(&LockHandle); 

    if (foundContext)
        WorkerRoutineEx(DeviceObject, Context, IoWorkItem);
    else {
        DbgPrint("ntoskrn8: IoQueueWork Context not found");
        KeBugCheckEx(0xDEADBEEF, 0x0, (ULONG_PTR)Context, (ULONG_PTR)DeviceObject,  0x0);
    }
    
}


void
IoFreeWorkItem_k8 (
    PIO_WORKITEM IoWorkItem )
{
    ULONG               CLEANUP_Index, CLEANUP_RevIndex;
    KLOCK_QUEUE_HANDLE  LockHandle;

    KeAcquireInStackQueuedSpinLock(&g_SpinWorkerRoutineArray, &LockHandle);
    {
     // 1) remove prepared entry from WorkerRoutineArray if IoFreeWorkItem called before IoQueueWorkItemCompatible
     // 2) shrink WorkerRoutineArray
     CLEANUP_ENTRY(WorkerRoutineArray, IoWorkItem, IoWorkItem, g_LastUsedWorkerRoutineArray_Entry)
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);    

    IoFreeWorkItem(IoWorkItem);
}


void
IoQueueWorkItemEx_k8 (
    PIO_WORKITEM            IoWorkItem,
    PIO_WORKITEM_ROUTINE_EX WorkerRoutineEx,
    WORK_QUEUE_TYPE         QueueType,
    PVOID                   Context )
{
    ULONG               i;
    BOOLEAN             foundFreeEntry = FALSE;
    KLOCK_QUEUE_HANDLE  LockHandle;

    KeAcquireInStackQueuedSpinLock(&g_SpinWorkerRoutineArray, &LockHandle);
    {
        // check for double
        for (i = 0; i <= g_LastUsedWorkerRoutineArray_Entry; i++) {
            if (WorkerRoutineArray[i].IoWorkItem != NULL &&
                WorkerRoutineArray[i].Context == Context) { // double => bugcheck
                    KeReleaseInStackQueuedSpinLock(&LockHandle);
                    DbgPrint("ntoskrn8: WorkerRoutineArray double Context");
                    KeBugCheckEx(0xDEADBEEF, 0x2, (ULONG_PTR)Context, (ULONG_PTR)IoWorkItem, 0x0);
            }
        }
        
        for (i = 0; i < MAX_WorkerRoutineIndex; i++) {
            if (WorkerRoutineArray[i].IoWorkItem == NULL) {
                foundFreeEntry = TRUE;
                if (i > g_LastUsedWorkerRoutineArray_Entry)
                            g_LastUsedWorkerRoutineArray_Entry = i;
                WorkerRoutineArray[i].WorkerRoutineEx = WorkerRoutineEx;
                WorkerRoutineArray[i].IoWorkItem      = IoWorkItem;
                WorkerRoutineArray[i].Context         = Context;
                break;
            }
        }
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);

    if (foundFreeEntry) {
        IoQueueWorkItem( IoWorkItem,
                         (PIO_WORKITEM_ROUTINE) IoQueueWorkItemCompatible,
                         QueueType,
                         Context);
    }
    else {
        DbgPrint("ntoskrn8: WorkerRoutineArray overflow");
        KeBugCheckEx(0xDEADBEEF, 0x1, (ULONG_PTR)Context, (ULONG_PTR)IoWorkItem, 0x0);
    }

}


ULONG
IoSizeofWorkItem_k8 (void)
{
    return sizeof(IO_WORKITEM);
}
 
 
void
IoInitializeWorkItem_k8 (
   PVOID        IoObject,
   PIO_WORKITEM IoWorkItem )
{
    PIO_WORKITEM tmpWorkItem = IoAllocateWorkItem(IoObject);
    RtlCopyMemory(IoWorkItem, tmpWorkItem, sizeof(IO_WORKITEM));
    IoFreeWorkItem(tmpWorkItem);
}


void
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
KeQueryMaximumProcessorCount_k8 (void)          // RE procgrp.lib
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
KeQueryActiveGroupCount_k8 (void)
{
    return 1;  // active groups = 1 
}


USHORT
KeQueryMaximumGroupCount_k8 (void)
{
    return 1;  // total groups = 1 
}


KAFFINITY
KeSetSystemAffinityThreadEx_k8 (                // RE procgrp.lib
    KAFFINITY Affinity )
{
    KeSetSystemAffinityThread_k8(Affinity);
    return (KAFFINITY)0;
}


void
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


void
KeRevertToUserAffinityThreadEx_k8 (             // RE procgrp.lib
    KAFFINITY Affinity )
{
    if ( Affinity == 0)
        KeRevertToUserAffinityThread();
}


void
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
KeGetCurrentNodeNumber_k8 (void)
{
    return 0; // current node = 0
}


USHORT
KeQueryHighestNodeNumber_k8 (void)
{
    return 0; // total nodes = 0
}


void
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
        for (i = 0; i < Length/(ULONG)sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++)
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


void
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


void
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

typedef ULONG NODE_REQUIREMENT; 

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
    ExAcquireFastMutex((PFAST_MUTEX)Mutex);
}


BOOLEAN FASTCALL
KeTryToAcquireGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    return ExTryToAcquireFastMutex((PFAST_MUTEX)Mutex);
}


void FASTCALL
KeReleaseGuardedMutex_k8 (
    PKGUARDED_MUTEX Mutex )
{
    ExReleaseFastMutex((PFAST_MUTEX)Mutex);
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


// TODO trace OldIrql depend on caller
void
KeEnterGuardedRegion_k8 (void)
{
    if (g_GuardedRegionCounter < 1)
        KeRaiseIrql(APC_LEVEL, &g_GuardedRegion_OldIrql);

    KeEnterCriticalRegion();
    InterlockedIncrement(&g_GuardedRegionCounter);
}


void
KeLeaveGuardedRegion_k8 (void)
{
    KeLeaveCriticalRegion();
    InterlockedDecrement(&g_GuardedRegionCounter);

    if (g_GuardedRegionCounter < 1)
        KeLowerIrql(g_GuardedRegion_OldIrql);

    if (g_GuardedRegionCounter < 0)
        InterlockedExchange(&g_GuardedRegionCounter, 0);    
}


NTSTATUS
DummyProc (void)
{
    return STATUS_SUCCESS;
}


void
PoStartDeviceBusy_k8 (
    PULONG IdlePointer )
{
    PoSetDeviceBusy(IdlePointer);
}


void
PoEndDeviceBusy_k8 (
    PULONG IdlePointer )

{
    return;
}


// mark some routines for dynamic availablity
const RoutineAddressType RoutineAddressEnabled[] = {    
    { RTL_CONSTANT_STRING_k8(L"IoConnectInterruptEx"),     &IoConnectInterruptEx_k8    }, // wdf01000.sys 1.11.9200
    { RTL_CONSTANT_STRING_k8(L"IoDisconnectInterruptEx"),  &IoDisconnectInterruptEx_k8 }, // wdf01000.sys 1.11.9200
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
    *Registration = NULL;
    return STATUS_SUCCESS;
}


void
PcwUnregister_k8 (
    PPCW_REGISTRATION Registration )
{
    return;
}


NTSTATUS
PcwCreateInstance_k8 (
     PPCW_INSTANCE     *Instance,
     PPCW_REGISTRATION  Registration,
     PCUNICODE_STRING   Name,
     ULONG              Count,
     PPCW_DATA          Data )
{
    *Instance = NULL;
    return STATUS_SUCCESS;
}


void
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


NTSTATUS
IoUnregisterPlugPlayNotificationEx_k8(
    PVOID NotificationEntry )
{
    return IoUnregisterPlugPlayNotification(NotificationEntry);
}



NTSTATUS
EmClientQueryRuleState_k8 (
    LPCGUID RuleId,
    PEM_RULE_STATE State )
{   
    *State = STATE_UNKNOWN;
    return STATUS_SUCCESS;
}


VOID
PoSetDeviceBusyEx_k8 (
    PULONG IdlePointer )
{
    PoSetDeviceBusy(IdlePointer);
}


IO_PRIORITY_HINT
IoGetIoPriorityHint_k8 (
    PIRP Irp )
{
    return IoPriorityNormal;
}


NTSTATUS
IoAllocateSfioStreamIdentifier_k8 (
     PFILE_OBJECT  FileObject,
     ULONG         Length,
     PVOID         Signature,
     PVOID         *StreamIdentifier )
{
    // PVOID   *Buffer;
    // Buffer    = ExAllocatePoolWithTag(NonPagedPool, Length + 4*sizeof(ULONG_PTR), 'tSoI');
    // Buffer[0] = NULL;
    // Buffer[1] = NULL;
    // Buffer[2] = &Buffer[4];
    // Buffer[3] = Signature;
    // *StreamIdentifier = &Buffer[4];
    // return STATUS_SUCCESS;
    
    return STATUS_UNSUCCESSFUL;
}


PVOID
IoGetSfioStreamIdentifier_k8 (
     PFILE_OBJECT   FileObject,
     PVOID          Signature )
{
    return NULL;
}


NTSTATUS
IoFreeSfioStreamIdentifier_k8 (
    PFILE_OBJECT   FileObject,
    PVOID          Signature )
{
    return STATUS_SUCCESS;
}


void
Create_Process_Notify_Routine_XP (
    HANDLE      ParentId,
    HANDLE      ProcessId,
    BOOLEAN     Create,
    PEPROCESS   ProcessExit,      // EDI
    PEPROCESS   ProcessCreate,    // EBX
    PETHREAD    Thread )          // ESI
{
//                PspCreateThread x32 XP/2003
// EBX - ProcessCreate
// ESI - Thread

//                PspExitProcess  x32 XP/2003
// EDI - ProcessExit

//                PspCreateThread x64 XP
// R12 - Process
// [rsp+80h] - Thread

//                PspExitProcess  x64 XP
// RSI - ProcessExit

//                PspCreateThread x64 2003
// R12 - Process
// [rsp+80h] - Thread

//                PspExitProcess  x64 2003
// RSI - ProcessExit

    ULONG                   i;
    PS_CREATE_NOTIFY_INFO   CreateInfo;
    PFILE_OBJECT            FileObject;
    PCUNICODE_STRING        CommandLine;        
    PCUNICODE_STRING        ImageFileName;

    for (i = 0; i <= g_LastUsedCreateProcessNotifyExArray_Entry; i++) {
        if (CreateProcessNotifyExArray[i].Routine ==  NULL) // skip empty entry
                continue;

        if (Create == FALSE)                                // exit process
                CreateProcessNotifyExArray[i].Routine(
                                                ProcessExit,
                                                ProcessId,
                                                NULL);
        else {                                              // create thread/process
            CreateInfo.Size = sizeof(PS_CREATE_NOTIFY_INFO);
            CreateInfo.CreationStatus = 0;
            CreateInfo.Flags = 0;
            CreateInfo.ParentProcessId = ParentId;
            CreateInfo.CreatingThreadId.UniqueProcess = ProcessId;
            CreateInfo.CreatingThreadId.UniqueThread = Thread->Cid.UniqueThread;

            FileObject = ((PSECTION)ProcessCreate->SectionObject)->Segment->ControlArea->FilePointer;
            CreateInfo.FileObject = FileObject;
            CreateInfo.ImageFileName = &FileObject->FileName;
            CreateInfo.FileOpenNameAvailable = FALSE;   // or TRUE ?
            CreateInfo.CommandLine = NULL;

            CreateProcessNotifyExArray[i].Routine(
                                            ProcessCreate,
                                            ProcessId,
                                            &CreateInfo);

            if (CreateInfo.CreationStatus < 0)
            {
                //TODO: PsTerminateProcess(ProcessCreate);
                ;
            }

        }
    }
}


NTSTATUS
PsSetCreateProcessNotifyRoutineEx_k8 (
    PCREATE_PROCESS_NOTIFY_ROUTINE_EX NotifyRoutine,
    BOOLEAN Remove )
{
    ULONG               i, CLEANUP_Index, CLEANUP_RevIndex;
    KLOCK_QUEUE_HANDLE  LockHandle;
    NTSTATUS            status = STATUS_SUCCESS;

    KeAcquireInStackQueuedSpinLock(&g_SpinCreateProcessNotifyExArray, &LockHandle);
    {
     if (Remove ==  FALSE) { // Add new callback
     
            // check for double
            for (i = 0; i <= g_LastUsedCreateProcessNotifyExArray_Entry; i++) {
                if (CreateProcessNotifyExArray[i].Routine == NotifyRoutine) { // double
                    status = STATUS_INVALID_PARAMETER;
                    break;
                }
            }

            if (status != STATUS_INVALID_PARAMETER) {
                for (i = 0; i < MAX_CreateProcessNotifyEx; i++) {
                    if (CreateProcessNotifyExArray[i].Routine == NULL) {
                        if (i > g_LastUsedCreateProcessNotifyExArray_Entry)
                                    g_LastUsedCreateProcessNotifyExArray_Entry = i;

                        CreateProcessNotifyExArray[i].Routine = NotifyRoutine;
                        break;
                    }
                }
            }
     }
     else                   // remove callback & shrink CreateProcessNotifyExArray
         CLEANUP_ENTRY(CreateProcessNotifyExArray, Routine, NotifyRoutine, g_LastUsedCreateProcessNotifyExArray_Entry)
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);
    
    return status; 
}


BOOLEAN
RtlIsNtDdiVersionAvailable_k8 (
    ULONG Version )
{
    ULONG MajorVersion;
    ULONG MinorVersion;

    if ((USHORT)Version != 0)
        return FALSE;
    else
    {
        PsGetVersion(
            &MajorVersion,
            &MinorVersion,
            0,              // BuildNumber,
            0);             // CSDVersion
           
        if (((MinorVersion + (MajorVersion << 8)) << 16) >= Version)
            return TRUE;
        else
            return FALSE;
    }
}


NTSTATUS
ExGetFirmwareEnvironmentVariable_k8 (
    PUNICODE_STRING VariableName,
    LPGUID          VendorGuid,
    PVOID           Value,
    PULONG          ValueLength,
    PULONG          Attributes )
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
ExSetFirmwareEnvironmentVariable_k8 (
    PUNICODE_STRING VariableName,
    LPGUID VendorGuid,
    PVOID Value,
    ULONG ValueLength,
    ULONG Attributes )
{
    return STATUS_NOT_IMPLEMENTED;
}    


void
KeSetSystemAffinityThread_k8 (
    KAFFINITY Affinity )

// Windows XP KeSetSystemAffinityThread dont check Affinity for match with actual processors
// Intel AHCI driver set affinity 0x111111110 even if OS have one-core CPU
// TODO: check w2003 & x64 kernels   
{
    KAFFINITY AvailableAffinity;

    AvailableAffinity = KeQueryActiveProcessors();
    if ((Affinity & AvailableAffinity) == 0) { // bad requested affinity
        Affinity = AvailableAffinity;
    }

    KeSetSystemAffinityThread(Affinity);
}    




NTSTATUS
DriverEntry (                   // Dummy entry
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath )
{
    return STATUS_SUCCESS;
}


NTSTATUS
DllInitialize (                // Main entry
    PUNICODE_STRING  RegistryPath )
{
     /*     // debug loop
    __asm {
        L1:
        jmp L1
    }
     */
    
    Initialize(RegistryPath);
    PsSetCreateProcessNotifyRoutine((PCREATE_PROCESS_NOTIFY_ROUTINE)&CREATE_PROCESS_NOTIFY_ROUTINE_asm, FALSE);

    return STATUS_SUCCESS;
}


NTSTATUS
DllUnload (void)
{
    PsSetCreateProcessNotifyRoutine((PCREATE_PROCESS_NOTIFY_ROUTINE)&CREATE_PROCESS_NOTIFY_ROUTINE_asm, TRUE);

    return STATUS_SUCCESS;
}


#include "ntoskrnl_redirects.h"
