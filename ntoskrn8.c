#undef    __STDC_WANT_SECURE_LIB__
#define   __STDC_WANT_SECURE_LIB__ 0

#include <ntddk.h> 
#include "ntifs_ddk.h"
#include <Initguid.h>
#include <devpkey.h>

#include "common.h"
#include "wrk2003.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _ArrayType_CreateProcessNotifyEx {
    PCREATE_PROCESS_NOTIFY_ROUTINE_EX  Routine;
} ArrayType_CreateProcessNotifyEx;


typedef struct _ArrayType_LookasideListEx {
    PALLOCATE_FUNCTION_EX   AllocateEx;
    PFREE_FUNCTION_EX       FreeEx;
    PLOOKASIDE_LIST_EX      LookasideEx;
} ArrayType_LookasideListEx;


typedef struct _RoutineAddress {
    UNICODE_STRING  RoutineName;
    PVOID           CallAddress;
} RoutineAddressType;


#define ADDENTRY_FULLPROLOG(Array,Field,g_Spin,MAX,LastUsed_Entry)  \
for (ULONG iii = 0; iii < MAX; iii++) {                                   \
    if (Array[iii].Field == NULL) {                                 \
        if (iii > LastUsed_Entry)                                   \
            LastUsed_Entry = iii;
            
#define ADDENTRY_BEGIN(Name,Field)                                  \
for (ULONG iii = 0; iii < MAX_##Name; iii++) {                            \
    if (Array_##Name[iii].Field == NULL) {                          \
        if (iii > gLastEntry_##Name)                                \
            gLastEntry_##Name = iii;            

// ADDENTRY BODY ...

#define ADDENTRY_END(Name,BugNum1,BugNum2,BugNum3)  \
             foundFreeEntry = TRUE;                 \
             break;                                 \
            }                                       \
        }                                           \
                                                    \
if (foundFreeEntry == FALSE) {                      \
    DbgPrint("ntoskrn8: " # Name " overflow");      \
    KeBugCheckEx(0xDEADBEEFL,                       \
                (ULONG_PTR)BugNum1,                 \
                (ULONG_PTR)BugNum2,                 \
                (ULONG_PTR)BugNum3,                 \
                0x0);                               \
}


#define FIND_DOUBLE_ENTRY_BEGIN(Name,Field,CompareValue)    \
for (ULONG iii = 0; iii <= gLastEntry_##Name; iii++) {                    \
    if (Array_##Name[iii].Field == ##CompareValue) {

// FIND_DOUBLE_ENTRY BODY ...

#define FIND_DOUBLE_ENTRY_END()                 \
             break;                             \
            }                                   \
        }



//for (ULONG i = 0; i <= gLastEntry_CreateProcessNotifyEx; i++) {
            //    if (Array_CreateProcessNotifyEx[i].Routine == NotifyRoutine) { // double
            //        status = STATUS_INVALID_PARAMETER;
            //        break;
            //    }
            //}


#define CLEANUP_ENTRY(Array,Field,ComparedValue,LastUsed_Entry)                \
for (ULONG CLEANUP_Index = 0; CLEANUP_Index <= LastUsed_Entry; CLEANUP_Index++) {    \
    if (Array[CLEANUP_Index].Field == ComparedValue)                           \
    {                                                                          \
        Array[CLEANUP_Index].Field = NULL;                                     \
        if (CLEANUP_Index==LastUsed_Entry && LastUsed_Entry > 0) {             \
            LastUsed_Entry--;                                                  \
            for (ULONG CLEANUP_RevIndex=LastUsed_Entry; CLEANUP_RevIndex > 0; CLEANUP_RevIndex--) {  \
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
0xDEADBEEFL, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

PWSTR static const
dummy_pci_id = L"PCI\\CC_0C0330";

PWSTR static const
XhciCompatibleIdList[] = {
    L"PCI\\CC_0C0330"
};


//////////////////////////////////////////////////////////
// static writable variables

#define MAX_CreateProcessNotifyEx   256
static ArrayType_CreateProcessNotifyEx Array_CreateProcessNotifyEx [MAX_CreateProcessNotifyEx];

//#define MAX_LookasideListEx        1024
//ArrayType_LookasideListEx       Array_LookasideListEx       [MAX_LookasideListEx];

static ULONG           gLastEntry_CreateProcessNotifyEx;
static ULONG           gLastEntry_LookasideListEx;

static KSPIN_LOCK      gSpin_CreateProcessNotifyEx;
static KSPIN_LOCK      gSpin_LookasideListEx;


// static writable
//////////////////////////////////////////////////////////


void
Initialize (PUNICODE_STRING  RegistryPath)
{
    KLOCK_QUEUE_HANDLE  LockHandle;

    gGuardedRegionCounter  = 0;
    gGuardedRegion_OldIrql = APC_LEVEL; // initial MAX irql

    KeInitializeSpinLock(&gSpin_CreateProcessNotifyEx);
    KeInitializeSpinLock(&gSpin_LookasideListEx);
    
    gLastEntry_CreateProcessNotifyEx = 0;
    gLastEntry_LookasideListEx = 0;
    
    RtlZeroMemory(Array_CreateProcessNotifyEx,  sizeof(Array_CreateProcessNotifyEx));
    //RtlZeroMemory(Array_LookasideListEx,        sizeof(Array_LookasideListEx));
    
}


void
KeFlushQueuedDpcs_k8 (void)    // WinXP RTM, SP1
{
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
RtlCheckPortableOperatingSystem_k8 (PBOOLEAN IsPortable)    // RE win8_ntoskrnl
{
    if (RtlCheckRegistryKey(RTL_REGISTRY_CONTROL, L"MiniNT") == STATUS_SUCCESS)
        *IsPortable = TRUE;
    else   
        *IsPortable = FALSE;
    
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
    *RegHandle = (REGHANDLE) 0; // is RegHandle=0 OK ?
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
EtwRegisterClassicProvider_k8 (   // undocumented
    PVOID Ptr,
    ULONG Flags,
    PVOID Callback,
    PVOID a4,
    PVOID a5)
{
    return STATUS_SUCCESS;
}


NTSTATUS
ZwAlpcConnectPort_k8 (                          // undocumented
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
ZwAlpcSendWaitReceivePort_k8 (                  // undocumented
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
        *Type = DEVPROP_TYPE_STRING;
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


VOID
IopProcessWorkItem_k8 (
    PIO_WORKITEM_EX ioWorkItem
    )
{
    PDEVICE_OBJECT deviceObject = ioWorkItem->IoObject;

    if (ioWorkItem->Type) // routine_ex
        ioWorkItem->Routine( deviceObject,
                             ioWorkItem->Context,
                             ioWorkItem );
    else {
        PIO_WORKITEM_ROUTINE ClassicCall = (PIO_WORKITEM_ROUTINE) ioWorkItem->Routine;
        ClassicCall( deviceObject, ioWorkItem->Context );
    }

    ObDereferenceObject( deviceObject );
}


PIO_WORKITEM_EX
IoAllocateWorkItem_inject (
    PDEVICE_OBJECT DeviceObject
    )
{
    PIO_WORKITEM_EX ioWorkItem;
    PWORK_QUEUE_ITEM exWorkItem;

    ioWorkItem = (PIO_WORKITEM_EX) ExAllocatePoolWithTag( NonPagedPool, sizeof( IO_WORKITEM_EX ), '  oI');
    if (ioWorkItem != NULL) {
        ioWorkItem->IoObject = DeviceObject;
        ioWorkItem->Type = 0;               // standart workitem

        exWorkItem = &ioWorkItem->WorkItem;
        exWorkItem->WorkerRoutine = (PWORKER_THREAD_ROUTINE) IopProcessWorkItem_k8;
        exWorkItem->Parameter = ioWorkItem;
        exWorkItem->List.Flink = NULL;
    }

    return ioWorkItem;
}


#pragma warning(push)
#pragma warning(disable : 4996) // 'ExQueueWorkItem': was declared deprecated

void
IoQueueWorkItemEx_k8 (
    PIO_WORKITEM_EX             IoWorkItem,
    PIO_WORKITEM_ROUTINE_EX_k8  WorkerRoutineEx,
    WORK_QUEUE_TYPE             QueueType,
    PVOID                       Context )
{
    ObReferenceObject( IoWorkItem->IoObject );

    IoWorkItem->Routine = WorkerRoutineEx;
    IoWorkItem->Context = Context;
    IoWorkItem->Type    = 1;            // WorkitemEx

    ExQueueWorkItem( &IoWorkItem->WorkItem, QueueType );
}

void
IoQueueWorkItem_inject (
    PIO_WORKITEM_EX      IoWorkItem,
    PIO_WORKITEM_ROUTINE WorkerRoutine,
    WORK_QUEUE_TYPE      QueueType,
    PVOID                Context )
{
    ObReferenceObject( IoWorkItem->IoObject );

    IoWorkItem->Routine = (PIO_WORKITEM_ROUTINE_EX_k8) WorkerRoutine;
    IoWorkItem->Context = Context;
    IoWorkItem->Type    = 0;            // standart workitem

    ExQueueWorkItem( &IoWorkItem->WorkItem, QueueType );
}

#pragma warning(pop)


ULONG
IoSizeofWorkItem_k8 (void)
{
    return sizeof(IO_WORKITEM_EX);
}
 
 
void
IoInitializeWorkItem_k8 (
   PVOID           IoObject,
   PIO_WORKITEM_EX IoWorkItem )
{  
    IoWorkItem->WorkItem.List.Flink = 0;
    IoWorkItem->Type = 0;                   // standart workitem
    IoWorkItem->IoObject = (PDEVICE_OBJECT) IoObject;
    IoWorkItem->WorkItem.WorkerRoutine = (PWORKER_THREAD_ROUTINE) IopProcessWorkItem_k8;
    IoWorkItem->WorkItem.Parameter = IoWorkItem;
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
        
    ProcNumber->Group    =  0;
    ProcNumber->Number   =  (UCHAR) ProcIndex;
    ProcNumber->Reserved =  0;
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
    
    CurrentProcessorNumber = KeGetCurrentProcessorNumber();
    if (ProcNumber != NULL) {
            ProcNumber->Group    =  0;
            ProcNumber->Number   =  (UCHAR) CurrentProcessorNumber;
            ProcNumber->Reserved =  0;
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


VOID
KeSetSystemAffinityThread_inject (
    KAFFINITY Affinity
    )

// Windows XP KeSetSystemAffinityThread don't check Affinity for match with actual processors
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


KAFFINITY
KeSetSystemAffinityThreadEx_k8 (                // RE procgrp.lib
    KAFFINITY Affinity )
{
    KeSetSystemAffinityThread_inject(Affinity);
    return (KAFFINITY) 0;
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
    //ULONG       ProcessorCore;
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

    Buffer = (UCHAR *) ExAllocatePoolWithTag( NonPagedPool, 65536 * sizeof(UCHAR), 200);

    //TODO: Bug in NtQuerySystemInformation, it never report SMT detection
    Status = NtQuerySystemInformation( SystemLogicalProcessorInformation,
                                     Buffer,
                                     65536 * sizeof(UCHAR),
                                     &Length);
    if (!Status) {
        for (ULONG i = 0; i < Length/(ULONG)sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++)
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
    for (ULONG i = 0; i < TotalProcessors; i++) 
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

        for (ULONG ProcessorCore = 0; ProcessorCore <= SMT; ProcessorCore++)
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
    for (ULONG i = 0; i < TotalCores; i++)
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


NTSTATUS
PoDisableSleepStates_k8 (ULONG a, ULONG b, ULONG c)         // undocumented
{
    return STATUS_SUCCESS;
}


void
PoReenableSleepStates_k8 (ULONG a)                          // undocumented
{
    ;
}


// mark some routines for dynamic availablity
const RoutineAddressType RoutineAddressEnabled[] = {    
    { CONSTANT_STR(L"IoConnectInterruptEx"),     &IoConnectInterruptEx_k8    }, // wdf01000.sys 1.11.9200
    { CONSTANT_STR(L"IoDisconnectInterruptEx"),  &IoDisconnectInterruptEx_k8 }, // wdf01000.sys 1.11.9200
};


PVOID
MmGetSystemRoutineAddress_inject (
    PUNICODE_STRING SystemRoutineName )
{
    PVOID    Address = NULL;

    Address = MmGetSystemRoutineAddress(SystemRoutineName);
    if (Address == NULL) {
        for (ULONG i = 0; i < ARRAYSIZE(RoutineAddressEnabled); i++) {
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


#if (NTDDI_VERSION < NTDDI_VISTA)

#if defined(_X86_)
void __stdcall     // _stdcall - CREATE_PROCESS_NOTIFY_ROUTINE_asm accept this calling convection
#else
void
#endif
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

    PS_CREATE_NOTIFY_INFO   CreateInfo;
    PFILE_OBJECT            FileObject;
    PCUNICODE_STRING        CommandLine;        
    PCUNICODE_STRING        ImageFileName;

    for (ULONG i = 0; i <= gLastEntry_CreateProcessNotifyEx; i++) {
        if (Array_CreateProcessNotifyEx[i].Routine ==  NULL) // skip empty entry
                continue;

        if (Create == FALSE)                                // exit process
                Array_CreateProcessNotifyEx[i].Routine(
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

            Array_CreateProcessNotifyEx[i].Routine(
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
    KLOCK_QUEUE_HANDLE  LockHandle;
    NTSTATUS            status = STATUS_SUCCESS;
    BOOLEAN             foundFreeEntry = FALSE;
    
    KeAcquireInStackQueuedSpinLock(&gSpin_CreateProcessNotifyEx, &LockHandle);
    {
     if (Remove ==  FALSE) { // Add new callback
     
            // check for double
            FIND_DOUBLE_ENTRY_BEGIN (CreateProcessNotifyEx, Routine, NotifyRoutine) {
                status = STATUS_INVALID_PARAMETER;
            }
            FIND_DOUBLE_ENTRY_END()
            
            if (status != STATUS_INVALID_PARAMETER) {
                ADDENTRY_BEGIN (CreateProcessNotifyEx, Routine) {
                    Array_CreateProcessNotifyEx[iii].Routine = NotifyRoutine;
                }
                ADDENTRY_END(CreateProcessNotifyEx, 0x3, NotifyRoutine, Remove)
            }
     }
     else                   // remove callback & shrink CreateProcessNotifyExArray
         CLEANUP_ENTRY(Array_CreateProcessNotifyEx, Routine, NotifyRoutine, gLastEntry_CreateProcessNotifyEx)
    }
    KeReleaseInStackQueuedSpinLock(&LockHandle);
    
    return status; 
}

#else
#if defined(_X86_)
void __stdcall     // _stdcall - CREATE_PROCESS_NOTIFY_ROUTINE_asm accept this calling convection
#else
void
#endif
Create_Process_Notify_Routine_XP (
    HANDLE      ParentId,
    HANDLE      ProcessId,
    BOOLEAN     Create,
    PEPROCESS   ProcessExit,      // EDI
    PEPROCESS   ProcessCreate,    // EBX
    PETHREAD    Thread )          // ESI
{
    // dummy for Vista+
}

#endif          // (NTDDI_VERSION < NTDDI_VISTA)

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



struct LicenseValueType {
    UNICODE_STRING  Name;
    ULONG           Type;
    ULONG           Length;
    UCHAR           Value[16];
};


#define ULONG_MAX   0xffffffffUL
#define TYPE_DWORD  4
#define TYPE_SZ     1
#define LEN_0       0
#define LEN_DWORD   4

const struct LicenseValueType LicenseValueArray[] = {
 //dxgkrnl.sys   
 {CONSTANT_STR(L"Microsoft-Windows-Core-MaxHRes"),        TYPE_DWORD, LEN_DWORD, 0xFF, 0xFF, 0xFF, 0xFF },
 {CONSTANT_STR(L"Microsoft-Windows-Core-MaxVRes"),        TYPE_DWORD, LEN_DWORD, 0xFF, 0xFF, 0xFF, 0xFF },
 {CONSTANT_STR(L"Microsoft-Windows-Core-AllowMultiMon"),  TYPE_DWORD, LEN_DWORD, 0x01, 0x00, 0x00, 0x00 },
  
 //win32k.sys
 {CONSTANT_STR(L"Security-SPP-GenuineLocalStatus"),            TYPE_DWORD, LEN_DWORD, 0x01, 0x00, 0x00, 0x00 },
 {CONSTANT_STR(L"Security-SPP-Reserved-TBLProductKeyType"),    TYPE_DWORD, LEN_DWORD, 0x00, 0x00, 0x00, 0x00 },
 {CONSTANT_STR(L"Security-SPP-Reserved-TBLRemainingTime"),     TYPE_DWORD, LEN_DWORD, 0x00, 0x00, 0x00, 0x00 },
 {CONSTANT_STR(L"Security-SPP-TokenActivation-AdditionalInfo"),TYPE_SZ,    LEN_0,     0x00                   },
 {CONSTANT_STR(L"Security-SPP-Reserved-TBLState"),             TYPE_DWORD, LEN_DWORD, 0x00, 0x00, 0x00, 0x00 },

 //Microsoft-Windows-Core-MaximizeAlways
 {CONSTANT_STR(L"Microsoft-Windows-Core-MaxTopLevelWinPerApp"),     TYPE_DWORD, LEN_DWORD, 0x00, 0x00, 0x00, 0x00 },
 {CONSTANT_STR(L"Microsoft-Windows-Core-MaxConcurrentIApp"),        TYPE_DWORD, LEN_DWORD, 0x00, 0x00, 0x00, 0x00 },
 {CONSTANT_STR(L"Microsoft-Windows-Core-ParentProcessDenyList"),    TYPE_SZ,    LEN_0,     0x00                   },
 {CONSTANT_STR(L"Microsoft-Windows-Core-InstanceLimitExemptedApps"),TYPE_SZ,    LEN_0,     0x00                   },
};


NTSTATUS
ZwQueryLicenseValue_k8 (
    UNICODE_STRING *name,
    ULONG *result_type,
    PVOID data,
    ULONG length,
    ULONG *result_len)
{
    BOOLEAN found=FALSE;
    ULONG   Type, Length ;
    UCHAR   *Value;
    
    if (!name || !name->Buffer || !name->Length || !result_len)
        return STATUS_INVALID_PARAMETER;
    
    for (ULONG i = 0; i < ARRAYSIZE(LicenseValueArray); i++) {
        if (RtlEqualUnicodeString( &LicenseValueArray[i].Name, name, FALSE)) {
            found=  TRUE;
            Type=            LicenseValueArray[i].Type;
            Length=          LicenseValueArray[i].Length;
            Value=  (UCHAR *)LicenseValueArray[i].Value;
            break;
        }
    } 
    
    if (found == FALSE)
        return STATUS_OBJECT_NAME_NOT_FOUND;
    
    if (result_type)
        *result_type = Type;
            
    *result_len = Length;              
 
    if (length < Length)
        return STATUS_BUFFER_TOO_SMALL;
    
    if (data)
        RtlCopyMemory(data, Value, Length); 
        
    return STATUS_SUCCESS;
}


BOOLEAN
PsIsProtectedProcess_k8 (PEPROCESS Process)
{
    const char *pszImageFile = (const char *)Process->ImageFileName;
    if (!pszImageFile)
        return FALSE;

    if (!_stricmp(pszImageFile, "CSRSS.EXE"))
        return TRUE;
    
    return FALSE;
}


NTSTATUS
ZwAllocateLocallyUniqueId_k8 (PLUID Luid)
{
    return NtAllocateLocallyUniqueId(Luid);
}


KPRIORITY
KeSetActualBasePriorityThread_k8 (
    PKTHREAD Thread,
    KPRIORITY Priority )
{
    KPRIORITY PrevBasePriority= Thread->BasePriority;
    
    Thread->BasePriority=(SCHAR)Priority;
    
    if (Thread->Priority != (SCHAR)Priority) {
        KeSetPriorityThread(Thread, Priority);
    }
    
    return PrevBasePriority;
}


VOID FASTCALL
KeInvalidateRangeAllCaches_k8 (
    PCHAR BaseAddress,
    ULONG Length )
{
    CHAR  *Addr;
    CHAR  *EndAddr;
    ULONG  IncAddr;
    // IncAddr = [PEB+3BCh];
    // cache line size=  (bits 8-15 of EBX CPUID(EAX=1)
    // shr     ebx, 5
    // and     ebx, 11111111000b
    
    ULONG OutEax,OutEbx,OutEcx,OutEdx;
    CPU_INFO CPU_Info;
    
    #if defined(_X86_)  // x32
        CPUID(1, &OutEax, &OutEbx, &OutEcx, &OutEdx);
    #else               // x64
        KiCpuId(0x1, 0, &CPU_Info);
        OutEax=CPU_Info.Eax;
        OutEbx=CPU_Info.Ebx;
        OutEcx=CPU_Info.Ecx;
        OutEdx=CPU_Info.Edx;
    #endif
    
    if ( (OutEdx & (1L < 19)) == 1) {    //clflush features if CPUID.01H:EDX[bit 19]=1
        Addr    = BaseAddress;
        IncAddr = (OutEbx >> 5) & 0x07F8UL;  // OutEbx= CacheLine Size, 11111111000b
        EndAddr = BaseAddress + Length - 1;
        while (Addr <= EndAddr )
        {
            _mm_clflush(Addr);
            Addr += IncAddr;
        }
    }
    else {
        KeInvalidateAllCaches_k8();
    }
    
}

 
PVOID
ExAllocatePoolEx (
    POOL_TYPE PoolType,
    SIZE_T    NumberOfBytes,
    ULONG     Tag,
    PLOOKASIDE_LIST_EX Lookaside )
{
    return ExAllocatePoolWithTag(PoolType, NumberOfBytes, Tag);
}


void
ExFreePoolEx(
    PVOID  Buffer,
    PLOOKASIDE_LIST_EX  Lookaside )
{
    ExFreePoolWithTag(Buffer, 0);
}


NTSTATUS
ExInitializeLookasideListEx_k8 (
    PLOOKASIDE_LIST_EX      LookasideEx,
    PALLOCATE_FUNCTION_EX   AllocateEx,
    PFREE_FUNCTION_EX       FreeEx,
    POOL_TYPE               PoolType,
    ULONG                   Flags,
    SIZE_T                  Size,
    ULONG                   Tag,
    USHORT                  Depth )
{
    PALLOCATE_FUNCTION  AllocateExFunc = (PALLOCATE_FUNCTION) AllocateEx;
    PFREE_FUNCTION      FreeExFunc     = (PFREE_FUNCTION)     FreeEx;
    KLOCK_QUEUE_HANDLE  LockHandle;
    BOOLEAN             foundFreeEntry = FALSE;
        
    if (PoolType & 0xFFFFFE18L || (PoolType & 3) == 3 ) // 11111111111111111111111000011000
          return STATUS_INVALID_PARAMETER_4;
    
    if (Size <= 4)
            Size = 4;
    
    if (AllocateExFunc == NULL)
            AllocateExFunc= (PALLOCATE_FUNCTION) ExAllocatePoolEx;
          
    if (FreeExFunc == NULL)
            FreeExFunc= (PFREE_FUNCTION) ExFreePoolEx;

/*
        Flags &= 1; // XP support only pool allocation flag
        ADDENTRY_BEGIN (LookasideListEx, LookasideEx) {
            Array_LookasideListEx[iii].AllocateEx= AllocateEx;
            Array_LookasideListEx[iii].FreeEx =    FreeEx;
            Array_LookasideListEx[iii].LookasideEx = LookasideEx;
        }
        ADDENTRY_END(LookasideListEx, 0x4, LookasideEx, Size)
            
    }
*/
    
    if (PoolType & 1) {                         // PagedPool
        ExInitializePagedLookasideList(
           (PPAGED_LOOKASIDE_LIST)  LookasideEx,
                                    AllocateExFunc,
                                    FreeExFunc,
                                    Flags,
                                    Size,
                                    Tag,
                                    Depth );
    }
    else {                                      // NonPagedPool
        ExInitializeNPagedLookasideList(
           (PNPAGED_LOOKASIDE_LIST) LookasideEx,
                                    AllocateExFunc,
                                    FreeExFunc,
                                    Flags,
                                    Size,
                                    Tag,
                                    Depth );
    }
    
    return STATUS_SUCCESS;
}


void
ExDeleteLookasideListEx_k8 (
    PLOOKASIDE_LIST_EX Lookaside )
{
    if (Lookaside->L.Type & 1) {                  // PagedPool
      ExDeletePagedLookasideList( (PPAGED_LOOKASIDE_LIST) Lookaside);
    }
    else {                                      // NonPagedPool     
      ExDeleteNPagedLookasideList((PNPAGED_LOOKASIDE_LIST) Lookaside);
    }
        
}


NTSTATUS
RtlQueryElevationFlags_k8 (ULONG *Result)
{
    *Result=0;
    return STATUS_SUCCESS;
}


void
PsEnterPriorityRegion_k8 (void)
{
    ;
}


void
PsLeavePriorityRegion_k8 (void)
{
    ;
}


PVOID
ExEnterPriorityRegionAndAcquireResourceExclusive_k8 (PERESOURCE Resource)
{
    return ExEnterCriticalRegionAndAcquireResourceExclusive_k8(Resource);
}


PVOID
ExEnterPriorityRegionAndAcquireResourceShared_k8 (PERESOURCE Resource)
{
    return ExEnterCriticalRegionAndAcquireResourceShared_k8(Resource);
}


VOID FASTCALL
ExReleaseResourceAndLeavePriorityRegion_k8 (PERESOURCE Resource)
{
    ExReleaseResourceAndLeaveCriticalRegion_k8(Resource);
}


NTSTATUS
PsAcquireProcessExitSynchronization_k8 (PEPROCESS Process)
{
    ULONG_PTR       Value, NewValue;
    PEX_RUNDOWN_REF RundownProtect;
    
    RundownProtect=&Process->RundownProtect;
    Value = RundownProtect->Count & 0xFFFFFFFEL;
    NewValue = Value + EX_RUNDOWN_COUNT_INC;

    #if !defined(_WIN64)
        NewValue=(ULONG_PTR) InterlockedCompareExchange(
                            (LONG *)RundownProtect,
                            NewValue,
                            Value);
    #else
        NewValue=(ULONG_PTR) InterlockedCompareExchange64(
                            (LONG64 *)RundownProtect,
                            NewValue,
                            Value);
    #endif
    
    if (Value==NewValue) {
        return STATUS_SUCCESS;
    } else {
    
        if (ExAcquireRundownProtection(RundownProtect) == FALSE) {
            return STATUS_PROCESS_IS_TERMINATING;
        }
        else {
            return STATUS_SUCCESS;
        }
    }
}


void
PsReleaseProcessExitSynchronization_k8 (PEPROCESS Process)
{
    ULONG_PTR       Value, NewValue;
    PEX_RUNDOWN_REF RundownProtect;
    
    RundownProtect=&Process->RundownProtect;
    Value = RundownProtect->Count & 0xFFFFFFFEL;
    NewValue = Value - EX_RUNDOWN_COUNT_INC;

    #if !defined(_WIN64)
        NewValue=(ULONG_PTR) InterlockedCompareExchange(
                            (LONG *)RundownProtect,
                            NewValue,
                            Value);
    #else
        NewValue=(ULONG_PTR) InterlockedCompareExchange64(
                            (LONG64 *)RundownProtect,
                            NewValue,
                            Value);
    #endif

    if (Value != NewValue)
        ExReleaseRundownProtection(RundownProtect);
}


// typedef struct _DUMMY_FILE_OBJECT {
//  OBJECT_HEADER ObjectHeader;
//  CHAR FileObjectBody[ sizeof( FILE_OBJECT ) ];
// } DUMMY_FILE_OBJECT, *PDUMMY_FILE_OBJECT;

#if (NTDDI_VERSION < NTDDI_WIN7)
POBJECT_TYPE
ObGetObjectType_k8 (PVOID Object) {
    POBJECT_HEADER  ObjectHeader;
    
    ObjectHeader = OBJECT_TO_OBJECT_HEADER(Object);
    return ObjectHeader->Type;
}


FORCEINLINE POBJECT_HEADER_NAME_INFO
OBJECT_HEADER_TO_NAME_INFO_EXISTS (
    IN POBJECT_HEADER ObjectHeader
    )
{
    return (POBJECT_HEADER_NAME_INFO)((PUCHAR)ObjectHeader -
                                      ObjectHeader->NameInfoOffset);
}


FORCEINLINE POBJECT_HEADER_NAME_INFO
OBJECT_HEADER_TO_NAME_INFO (
    IN POBJECT_HEADER ObjectHeader )
{
    POBJECT_HEADER_NAME_INFO nameInfo;

    if (ObjectHeader->NameInfoOffset != 0) {
        nameInfo = OBJECT_HEADER_TO_NAME_INFO_EXISTS(ObjectHeader);
    } else {
        nameInfo = NULL;
    }

    return nameInfo;
}


POBJECT_HEADER_NAME_INFO
ObQueryNameInfo_k8 (PVOID Object) {
    POBJECT_HEADER  ObjectHeader;
    POBJECT_HEADER_NAME_INFO NameInfo;
        
    ObjectHeader = OBJECT_TO_OBJECT_HEADER(Object);
    NameInfo =     OBJECT_HEADER_TO_NAME_INFO(ObjectHeader);
    
    return NameInfo;
}
#endif // NTDDI_VERSION < NTDDI_WIN7


void
PoUserShutdownInitiated_k8 (void)
{
    ;
}


//TODO: check x64 args 
NTSTATUS 
LdrResFindResource_k8 (
    PVOID                       DllHandle,
    ULONG_PTR                   Type,
    ULONG_PTR                   Name,
    ULONG_PTR                   Lang,
    ULONG_PTR                  *ResourcePtr,
    ULONG                      *Size,
    ULONG                       a7,
    ULONG                       a8,
    ULONG                       Flags)
{
    NTSTATUS                    Status; 
    ULONG_PTR                   IdPath[3];
    PIMAGE_RESOURCE_DATA_ENTRY  ResourceDataEntry;
    
    IdPath[0] = Type;
    IdPath[1] = Name;
    IdPath[2] = Lang;
    
    Status = LdrFindResource_U(
                DllHandle,
                IdPath,
                3,
                &ResourceDataEntry );
                
    if (Status != STATUS_SUCCESS)
        return STATUS_RESOURCE_DATA_NOT_FOUND;
    
    *ResourcePtr=0;
    Status = LdrAccessResource(
                    DllHandle,
                    ResourceDataEntry,
          (PVOID *) ResourcePtr,
                    Size );
                
    return Status;
                
}


//TODO: check x64 args 
NTSTATUS 
LdrResFindResourceDirectory_k8 (
    PVOID                       DllHandle,
    ULONG_PTR                   Type,
    ULONG_PTR                   Lang,
    PIMAGE_RESOURCE_DIRECTORY  *ResourceDirectory,
    ULONG                       a6,
    ULONG                       a7,
    ULONG                       a8 )
{

    NTSTATUS                  Status; 
    ULONG_PTR                 IdPath[1];
        
    IdPath[0] = Type;
    
    Status = LdrFindResourceDirectory_U(
                DllHandle,
                IdPath,
                1,
                ResourceDirectory);
                
    return Status;
}


NTSTATUS
DbgkLkmdRegisterCallback_k8 (
                PVOID CallBack,
                PVOID Context,
                ULONG Mode)
{
    return STATUS_SUCCESS;
}




////////////////////////////////////////////////////////////
// msrpc.sys stubs, please ignore

#define RPC_VAR_ENTRY   __cdecl
#define RPC_ENTRY       __stdcall
#define __RPC_FAR


typedef struct _MIDL_STUB_DESC
{
    void  *    RpcInterfaceInformation;
    //
} MIDL_STUB_DESC;


typedef const MIDL_STUB_DESC  * PMIDL_STUB_DESC;

typedef long RPC_STATUS;

typedef const unsigned char  * PFORMAT_STRING;


// hack
typedef void  * CLIENT_CALL_RETURN;


typedef struct _RPC_ASYNC_STATE {
    unsigned int    Size; // size of this structure
    //
} RPC_ASYNC_STATE, *PRPC_ASYNC_STATE;

typedef void * I_RPC_HANDLE;
typedef I_RPC_HANDLE RPC_BINDING_HANDLE;
typedef void __RPC_FAR * RPC_IF_HANDLE;


typedef struct _RPC_BINDING_HANDLE_TEMPLATE_V1_W {
    unsigned long Version;
    unsigned long Flags;
    unsigned long ProtocolSequence;
    unsigned short *NetworkAddress;
    unsigned short *StringEndpoint;
    union
    {
        unsigned short *Reserved;
    } u1;
    UUID ObjectUuid;
} RPC_BINDING_HANDLE_TEMPLATE_V1_W, *PRPC_BINDING_HANDLE_TEMPLATE_V1_W;


typedef struct _RPC_BINDING_HANDLE_SECURITY_V1_W {
    unsigned long Version;
    //
} RPC_BINDING_HANDLE_SECURITY_V1_W, *PRPC_BINDING_HANDLE_SECURITY_V1_W;


typedef struct _RPC_BINDING_HANDLE_OPTIONS_V1 {
    unsigned long Version;
    unsigned long Flags;
    unsigned long ComTimeout;
    unsigned long CallTimeout;
} RPC_BINDING_HANDLE_OPTIONS_V1, *PRPC_BINDING_HANDLE_OPTIONS_V1;


RPC_STATUS 
RpcBindingBind (
    PRPC_ASYNC_STATE pAsync,
    RPC_BINDING_HANDLE Binding,
    RPC_IF_HANDLE IfSpec )
{
    return STATUS_INVALID_PARAMETER;
}


RPC_STATUS
RpcBindingCreateW (
    RPC_BINDING_HANDLE_TEMPLATE_V1_W * Template,
    RPC_BINDING_HANDLE_SECURITY_V1_W * Security,
    RPC_BINDING_HANDLE_OPTIONS_V1 * Options,
    RPC_BINDING_HANDLE * Binding )
{
    return STATUS_INVALID_PARAMETER;
}


RPC_STATUS
RpcBindingCopy (
    RPC_BINDING_HANDLE SourceBinding,
    RPC_BINDING_HANDLE __RPC_FAR * DestinationBinding )
{
    return STATUS_INVALID_PARAMETER;
}


RPC_STATUS
RpcBindingFree (
    RPC_BINDING_HANDLE __RPC_FAR * Binding )
{
    return STATUS_SUCCESS;
}


RPC_STATUS 
RpcBindingUnbind (
    RPC_BINDING_HANDLE Binding )
{
    return STATUS_SUCCESS;
}


RPC_STATUS
RpcAsyncInitializeHandle (
    PRPC_ASYNC_STATE pAsync,
    unsigned int     Size )
{
    return STATUS_INVALID_PARAMETER;
}


RPC_STATUS
RpcAsyncCompleteCall (
    PRPC_ASYNC_STATE pAsync,
    void *Reply )
{
    return STATUS_INVALID_PARAMETER;
}


RPC_STATUS
RpcAsyncCancelCall (
    PRPC_ASYNC_STATE pAsync,
    BOOLEAN fAbort )
{
    return STATUS_INVALID_PARAMETER;
}


CLIENT_CALL_RETURN RPC_VAR_ENTRY
NdrAsyncClientCall(
    PMIDL_STUB_DESC         pStubDescriptor,
    PFORMAT_STRING          pFormat,
    ...
    )
{
    return (CLIENT_CALL_RETURN)NULL;
}


PVOID
I_RpcGetCompleteAndFreeRoutine ()
{
    return NULL;
}


int 
RPC_ENTRY
I_RpcExceptionFilter (
    unsigned long ExceptionCode )
{
    return 0;
}



NTSTATUS
MmRotatePhysicalView_k8 (
    PVOID               VirtualAddress,
    PSIZE_T             NumberOfBytes,
    PMDLX               NewMdl,
    MM_ROTATE_DIRECTION Direction,
    PMM_ROTATE_COPY_CALLBACK_FUNCTION CopyFunction,
    PVOID               Context )
{
    return STATUS_SUCCESS;
}


/*
#define gWin32KServiceTblMAX 825     // win7 win32k.sys W32pServiceTable size=825 
ULONG_PTR  gWin32KServiceTbl [gWin32KServiceTblMAX];
UCHAR      gWin32KServiceArgs[gWin32KServiceTblMAX];


BOOLEAN
KeAddSystemServiceTable_win32k (        // win7 win32k.sys W32pServiceTable patcher
    PULONG_PTR  Base,
    PULONG      Count,
    ULONG       Limit,
    PUCHAR      Arguments,
    ULONG       Index )
{
    
    for (ULONG i=0; i<= gWin32KServiceTblMAX; i++) {
        gWin32KServiceTbl[i]  = Base[i];
        gWin32KServiceArgs[i] = Arguments[i];
    }
       
    
    return KeAddSystemServiceTable (
                gWin32KServiceTbl,
                Count,
                Limit,
                gWin32KServiceArgs,
                Index );
}
*/


NTSTATUS
ZwPowerInformation_inject (
    POWER_INFORMATION_LEVEL InformationLevel,
    PVOID                   InputBuffer,
    ULONG                   InputBufferLength,
    PVOID                   OutputBuffer,
    ULONG                   OutputBufferLength )
{
    switch (InformationLevel) {
    case 0x29:                                  // Sleep TimeOuts
        RtlZeroMemory(OutputBuffer, 0x1C);
        return STATUS_SUCCESS;
        break;
    case 0x1D:                                  // PopConsoleDisplayState
        RtlZeroMemory(OutputBuffer, 0x04);
        return STATUS_SUCCESS;
        break;
        
    case 0x1E:
        return STATUS_SUCCESS;
        break;
        
    case 0x1F:
        return STATUS_SUCCESS;
        break;
        
    case 0x2A:
        return STATUS_SUCCESS;
        break;
        
    case 0x2F:
        return STATUS_SUCCESS;
        break;
        
    default:
        return ZwPowerInformation(
                    InformationLevel,
                    InputBuffer,
                    InputBufferLength,
                    OutputBuffer,
                    OutputBufferLength );
    }
}

#define SE_GROUP_MANDATORY          0x00000001
#define SE_GROUP_ENABLED_BY_DEFAULT 0x00000002
#define SE_GROUP_ENABLED            0x00000004
#define SE_GROUP_OWNER              0x00000008
#define SE_GROUP_USE_FOR_DENY_ONLY  0x00000010
#define SE_GROUP_INTEGRITY          0x00000020
#define SE_GROUP_INTEGRITY_ENABLED  0x00000040
#define SE_GROUP_LOGON_ID           0xC0000000
#define SE_GROUP_RESOURCE           0x20000000
#define SE_GROUP_VALID_ATTRIBUTES   0xE000007F

static const SID high_level = {SID_REVISION, 1, {SECURITY_MANDATORY_LABEL_AUTHORITY},
                                                            {SECURITY_MANDATORY_HIGH_RID}};

NTSTATUS
SeQueryInformationToken_inject (                                // wine
    PACCESS_TOKEN           Token,
    TOKEN_INFORMATION_CLASS TokenInformationClass,
    PVOID                  *TokenInformation )
{
    TOKEN_MANDATORY_LABEL  *tml;
    PSID                    psid;
    
    switch (TokenInformationClass) {
    case TokenIntegrityLevel:
         /* report always "S-1-16-12288" (high mandatory level) for now */
            tml = (TOKEN_MANDATORY_LABEL *) Token;
            psid = tml + 1;

            tml->Label.Sid = psid;
            tml->Label.Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED;
            memcpy(psid, &high_level, sizeof(SID)); 
            *TokenInformation=psid;
        return STATUS_SUCCESS;
        break;
    case TokenUIAccess:
        *TokenInformation= NULL;
        return STATUS_SUCCESS;
        break;
    default:
        return SeQueryInformationToken(
                    Token,
                    TokenInformationClass,
                    TokenInformation );
    }
}



// https://github.com/pappyN4/NTOSKRNL_Emu merged with latest pappyN4 mods

NTSTATUS
RtlInitAnsiStringEx_k8 (
    OUT PANSI_STRING DestinationString,
    IN PCSZ SourceString OPTIONAL )
{
    ULONG Length;

    if (ARGUMENT_PRESENT( SourceString )) {
        Length = strlen(SourceString);

        if (Length > (MAXUSHORT - 1)) {
            return STATUS_NAME_TOO_LONG;
        }

        DestinationString->Length = (USHORT)Length;
        DestinationString->MaximumLength = (USHORT)(Length+1);
    } else {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
    }
    DestinationString->Buffer = (PCHAR)SourceString;

    return STATUS_SUCCESS;
}


NTSTATUS
RtlInitUnicodeStringEx_k8 (
    OUT PUNICODE_STRING DestinationString,
    IN PCWSTR SourceString OPTIONAL )
{
    if (SourceString != NULL) {
        SIZE_T Length = wcslen(SourceString);

        if (Length > (UNICODE_STRING_MAX_CHARS - 1)) {
            return STATUS_NAME_TOO_LONG;
        }

        Length *= sizeof(WCHAR);

        DestinationString->Length = (USHORT) Length;
        DestinationString->MaximumLength = (USHORT) (Length + sizeof(WCHAR));
        DestinationString->Buffer = (PWSTR) SourceString;
    } else {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
        DestinationString->Buffer = NULL;
    }

    return STATUS_SUCCESS;
}


// https://stackoverflow.com/questions/5017659/implementing-memcmp

INT
memcmp_k8(
	const void* buf1,
	const void* buf2,
    size_t count
){
    if(!count)
        return(0);

    while(--count && *(char*)buf1 == *(char*)buf2 ) {
        buf1 = (char*)buf1 + 1;
        buf2 = (char*)buf2 + 1;
    }

    return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}




// https://github.com/microsoft/Windows-driver-samples/blob/main/general/pcidrv/kmdf/HW/nic_init.c

PVOID 
MmMapIoSpaceEx_k8(
    PHYSICAL_ADDRESS PhysicalAddress,
    SIZE_T NumberOfBytes,
	ULONG Protect
    )
{
    return MmMapIoSpace(PhysicalAddress, NumberOfBytes, MmNonCached); 
}



WCHAR
RtlDowncaseUnicodeChar_k8(
    IN WCHAR SourceCharacter
    )
{
	return (WCHAR)tolower(SourceCharacter);
}

//
// Runtime Power Management Framework
//



/* START NEW section*/

//typedef PO_FX_DEVICE, *PPO_FX_DEVICE;

NTSTATUS
SmKmGenericCompletion(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp,
    PVOID Context)
{
  KeSetEvent((PRKEVENT) Context, 0, FALSE);
  return STATUS_MORE_PROCESSING_REQUIRED;
}


NTSTATUS
IoSynchronousCallDriver_k8(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
    )
{
    PIO_STACK_LOCATION IrpSp;
    NTSTATUS Status;
    KEVENT Event;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);
    IrpSp = IoGetNextIrpStackLocation(Irp);
    IrpSp->Context           = &Event;
    IrpSp->CompletionRoutine = (PIO_COMPLETION_ROUTINE) SmKmGenericCompletion;
    IrpSp->Control           = SL_INVOKE_ON_ERROR | SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_CANCEL; // 0xE0

    Status = IofCallDriver(DeviceObject, Irp);
    if (Status == STATUS_PENDING) {
        KeWaitForSingleObject(&Event, Suspended, KernelMode, FALSE, NULL);
        Status = Irp->IoStatus.Status;
    }

    return Status;
}





	
typedef PO_FX_COMPONENT_ACTIVE_CONDITION_CALLBACK, *PPO_FX_COMPONENT_ACTIVE_CONDITION_CALLBACK;
typedef PO_FX_COMPONENT_IDLE_CONDITION_CALLBACK, *PPO_FX_COMPONENT_IDLE_CONDITION_CALLBACK;
typedef PO_FX_COMPONENT_IDLE_STATE_CALLBACK, *PPO_FX_COMPONENT_IDLE_STATE_CALLBACK;
typedef PO_FX_DEVICE_POWER_REQUIRED_CALLBACK, *PPO_FX_DEVICE_POWER_REQUIRED_CALLBACK;
typedef PO_FX_DEVICE_POWER_NOT_REQUIRED_CALLBACK, *PPO_FX_DEVICE_POWER_NOT_REQUIRED_CALLBACK;
typedef PO_FX_POWER_CONTROL_CALLBACK, *PPO_FX_POWER_CONTROL_CALLBACK;

typedef struct _PO_FX_COMPONENT_IDLE_STATE {
    ULONGLONG TransitionLatency;
    ULONGLONG ResidencyRequirement;
    ULONG NominalPower;
} PO_FX_COMPONENT_IDLE_STATE, *PPO_FX_COMPONENT_IDLE_STATE;



typedef struct _PO_FX_COMPONENT_V1 {
    GUID Id;
    ULONG IdleStateCount;
    ULONG DeepestWakeableIdleState;
    PPO_FX_COMPONENT_IDLE_STATE IdleStates;
} PO_FX_COMPONENT_V1, *PPO_FX_COMPONENT_V1;

typedef struct _PO_FX_DEVICE_V1 {
  ULONG                                      Version;
  ULONG                                      ComponentCount;
  PPO_FX_COMPONENT_ACTIVE_CONDITION_CALLBACK ComponentActiveConditionCallback;
  PPO_FX_COMPONENT_IDLE_CONDITION_CALLBACK   ComponentIdleConditionCallback;
  PPO_FX_COMPONENT_IDLE_STATE_CALLBACK       ComponentIdleStateCallback;
  PPO_FX_DEVICE_POWER_REQUIRED_CALLBACK      DevicePowerRequiredCallback;
  PPO_FX_DEVICE_POWER_NOT_REQUIRED_CALLBACK  DevicePowerNotRequiredCallback;
  PPO_FX_POWER_CONTROL_CALLBACK              PowerControlCallback;
  PVOID                                      DeviceContext;
  PO_FX_COMPONENT_V1                         Components[ANYSIZE_ARRAY];
} PO_FX_DEVICE, *PPO_FX_DEVICE;


//
// Runtime Power Management Framework
//



DECLARE_HANDLE(POHANDLE);


/* 8.0 8056 */

NTSTATUS
PoFxRegisterDevice_k8 (
    PDEVICE_OBJECT Pdo,
    PPO_FX_DEVICE Device,
    POHANDLE *Handle
    )
	
	{
    return STATUS_SUCCESS;		
	}
	


VOID
PoFxUnregisterDevice_k8 (
    POHANDLE Handle
	){
		
	}
	
VOID
PoFxSetComponentLatency_k8 (
    POHANDLE Handle,
    ULONG Component,
    ULONGLONG Latency
	){
		
	}
	
VOID
PoFxSetComponentResidency_k8 (
    POHANDLE Handle,
    ULONG Component,
    ULONGLONG Residency
    ){
		
	}	
	
VOID
PoFxStartDevicePowerManagement_k8 (
    POHANDLE Handle
    ){
		
	}

VOID
PoFxCompleteIdleState_k8 (
    POHANDLE Handle,
    ULONG Component
    ){
		
	}


VOID
PoFxCompleteIdleCondition_k8 (
    POHANDLE Handle,
    ULONG Component
    ){
		
	}

VOID
PoFxReportDevicePoweredOn_k8 (
    POHANDLE Handle
    ){
		
	}
	
VOID
PoFxCompleteDevicePowerNotRequired_k8 (
    POHANDLE Handle
    ){
		
	}



VOID
PoFxActivateComponent_k8 (
    POHANDLE Handle,
    ULONG Component,
    ULONG Flags
    ){
		
	}

VOID
PoFxIdleComponent_k8 (
    POHANDLE Handle,
    ULONG Component,
    ULONG Flags
    ){
		
	}
	
	
/* 8.0 RTM */
	
	
VOID
PoFxSetDeviceIdleTimeout (
    POHANDLE Handle,
    ULONGLONG IdleTimeout
    ){
		
	}



typedef struct _IO_REPORT_INTERRUPT_ACTIVE_STATE_PARAMETERS {

    //
    // Version - Supplies the type of interrupt reporting operation
    //     requested by this structure.  This field must match the connection
    //     type returned by a corresponding successful call to
    //     IoConnectInterruptEx.
    //

    ULONG Version;

    //
    // ConnectionContext - Supplies a union containing the connection context
    //     associated with the interrupt being reported.  When
    //     referring to fully specified or line based interrupts, this
    //     parameter supplies the interrupt object pointer that was returned
    //     when the interrupt was initially connected.  When reporting a
    //     set of interrupt messages, this parameter supplies the interrupt
    //     message information table pointer that was returned when the
    //     interrupt messages were initially connected.
    //

    union {
        PVOID Generic;
        PKINTERRUPT InterruptObject;
        PIO_INTERRUPT_MESSAGE_INFO InterruptMessageTable;
    } ConnectionContext;

} IO_REPORT_INTERRUPT_ACTIVE_STATE_PARAMETERS,
  *PIO_REPORT_INTERRUPT_ACTIVE_STATE_PARAMETERS;
  
VOID
IoReportInterruptActive (
    PIO_REPORT_INTERRUPT_ACTIVE_STATE_PARAMETERS Parameters
    ){
		
	}

VOID
IoReportInterruptInactive (
    PIO_REPORT_INTERRUPT_ACTIVE_STATE_PARAMETERS Parameters
    ){
		
	}
	

VOID
PoFxSetComponentWake (
    POHANDLE Handle,
    ULONG Component,
    BOOLEAN WakeHint
    ){
		
	}

NTSTATUS
PoFxPowerControl (
    POHANDLE Handle,
    LPCGUID PowerControlCode,
    PVOID InBuffer,
    SIZE_T InBufferSize,
    PVOID OutBuffer,
    SIZE_T OutBufferSize,
    PSIZE_T BytesReturned
    ){
		return STATUS_SUCCESS;
		
	}
	
	
	
	
	
/* END NEW section*/






 


NTSTATUS
DriverEntry (                   // Dummy entry
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath )
{
    return STATUS_SUCCESS;
}


#ifdef DEBUG_TESTS
    #include "tests.c"
#endif    


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
    WRK2003_Init();
    //InitTramplineArray();


#ifdef DEBUG_TESTS
    RunTests();
#endif

    return STATUS_SUCCESS;
}


NTSTATUS
DllUnload (void)
{
    PsSetCreateProcessNotifyRoutine((PCREATE_PROCESS_NOTIFY_ROUTINE)&CREATE_PROCESS_NOTIFY_ROUTINE_asm, TRUE);

    return STATUS_SUCCESS;
}



#ifdef __cplusplus
}
#endif

#include "ntoskrnl_redirects.h"
