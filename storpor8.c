#undef    __STDC_WANT_SECURE_LIB__
#define   __STDC_WANT_SECURE_LIB__ 0


//#define STORPORT_W8

#include <ntddk.h> 
#include "common.h"
#include "wrk2003.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    ExtFunctionGetD3ColdSupport,                    // 67
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


#define STOR_STATUS_SUCCESS                 (0x00000000L)
#define STOR_STATUS_UNSUCCESSFUL            (0xC1000001L)
#define STOR_STATUS_NOT_IMPLEMENTED         (0xC1000002L)
#define STOR_STATUS_INSUFFICIENT_RESOURCES  (0xC1000003L)
#define STOR_STATUS_BUFFER_TOO_SMALL        (0xC1000004L)
#define STOR_STATUS_ACCESS_DENIED           (0xC1000005L)
#define STOR_STATUS_INVALID_PARAMETER       (0xC1000006L)
#define STOR_STATUS_INVALID_DEVICE_REQUEST  (0xC1000007L)
#define STOR_STATUS_INVALID_IRQL            (0xC1000008L)
#define STOR_STATUS_INVALID_DEVICE_STATE    (0xC1000009L)
#define STOR_STATUS_INVALID_BUFFER_SIZE     (0xC100000AL)
#define STOR_STATUS_UNSUPPORTED_VERSION     (0xC100000BL)
#define STOR_STATUS_BUSY                    (0xC100000CL)


#define IOCTL_ACPI_ASYNC_EVAL_METHOD \
        CTL_CODE(FILE_DEVICE_ACPI, 0, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#if defined(_WIN64) || defined(_M_ALPHA)
#define STOR_ADDRESS_ALIGN           DECLSPEC_ALIGN(8)
#else
#define STOR_ADDRESS_ALIGN
#endif


///////////////////////////////////////////
// storport.lib
ULONG
StorPortInitialize (
    PVOID Argument1,
    PVOID Argument2,
    struct _HW_INITIALIZATION_DATA *HwInitializationData,
    PVOID HwContext );

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

// storport.lib
///////////////////////////////////////////


enum _STOR_SYNCHRONIZATION_MODEL
{
    StorSynchronizeHalfDuplex = 0,
    StorSynchronizeFullDuplex = 1
};

enum _INTERRUPT_SYNCHRONIZATION_MODE
{
    InterruptSupportNone = 0,
    InterruptSynchronizeAll = 1,
    InterruptSynchronizePerMessage = 2
};

enum _RAID_OBJECT_TYPE
{
    RaidUnknownObject = -1,
    RaidAdapterObject = 0,
    RaidUnitObject = 1,
    RaidDriverObject =  2
};

enum _DEVICE_STATE
{
    DeviceStateNotPresent = 0,
    DeviceStateWorking = 1,
    DeviceStateStopped = 2,
    DeviceStatePendingStop = 3,
    DeviceStatePendingRemove = 4,
    DeviceStateSurpriseRemoval = 5,
    DeviceStateDeleted = 6,
    DeviceStateDisabled = 7,
    DeviceStateNoResource = 8
};


typedef struct _STOR_LOCKED_LIST            // Size=0x10
{
    KSPIN_LOCK          Lock;   // Offset=0x0 Size=0x4
    struct _LIST_ENTRY  List;   // Offset=0x4 Size=0x8
    unsigned long       Count;  // Offset=0xc Size=0x4
} STOR_LOCKED_LIST;


typedef struct _STOR_DICTIONARY             // Size=0x1c
{
    unsigned long        EntryCount;                            // Offset=0x0 Size=0x4
    unsigned long        MaxEntryCount;                         // Offset=0x4 Size=0x4
    enum _POOL_TYPE      PoolType;                              // Offset=0x8 Size=0x4
    struct _LIST_ENTRY  *Entries;                               // Offset=0xc Size=0x4
    void                *(*GetKeyRoutine)(struct _LIST_ENTRY *);// Offset=0x10 Size=0x4
    long                (*CompareKeyRoutine)(void *, void *);   // Offset=0x14 Size=0x4
    unsigned long       (*HashKeyRoutine)(void *);              // Offset=0x18 Size=0x4
} STOR_DICTIONARY;


typedef struct _RAID_RESOURCE_LIST          // Size=0x8
{
    struct _CM_RESOURCE_LIST *AllocatedResources;   // Offset=0x0 Size=0x4
    struct _CM_RESOURCE_LIST *TranslatedResources;  // Offset=0x4 Size=0x4
} RAID_RESOURCE_LIST;


typedef struct _ACCESS_RANGE                // Size=0x10
{
    union _LARGE_INTEGER RangeStart;    // Offset=0x0 Size=0x8
    unsigned long        RangeLength;   // Offset=0x8 Size=0x4
    unsigned char        RangeInMemory; // Offset=0xc Size=0x1
} ACCESS_RANGE;


typedef struct _MEMORY_REGION               // Size=0x18
{
    unsigned char       *VirtualBase;   // Offset=0x0 Size=0x4
    unsigned char __align0[4];          // Offset=0x4 Size=0x4
    PHYSICAL_ADDRESS     PhysicalBase;  // Offset=0x8 Size=0x8
    unsigned long        Length;        // Offset=0x10 Size=0x4
} MEMORY_REGION;


typedef struct _RAID_BUS_INTERFACE          // Size=0x24
{
    unsigned char                   Initialized; // Offset=0x0 Size=0x1
    struct _BUS_INTERFACE_STANDARD  Interface;   // Offset=0x4 Size=0x20
} RAID_BUS_INTERFACE;


typedef struct _STOR_SCSI_ADDRESS           // Size=0x4
{
    unsigned char PathId;               // Offset=0x0 Size=0x1
    unsigned char TargetId;             // Offset=0x1 Size=0x1
    unsigned char Lun;                  // Offset=0x2 Size=0x1
    unsigned char Reserved;             // Offset=0x3 Size=0x1
} STOR_SCSI_ADDRESS;


typedef struct STOR_ADDRESS_ALIGN _STOR_ADDR_BTL8 { // Size=0xc
    unsigned short Type;          // Offset=0x0 Size=0x2
    unsigned short Port;          // Offset=0x2 Size=0x2
    unsigned long  AddressLength; // Offset=0x4 Size=0x4
    unsigned char  Path;          // Offset=0x8 Size=0x1
    unsigned char  Target;        // Offset=0x9 Size=0x1
    unsigned char  Lun;           // Offset=0xa Size=0x1
    unsigned char  Reserved;      // Offset=0xb Size=0x1
} STOR_ADDR_BTL8, *PSTOR_ADDR_BTL8;


typedef struct STOR_ADDRESS_ALIGN _STOR_ADDRESS {
    USHORT Type;
    USHORT Port;
    ULONG  AddressLength;
    UCHAR  AddressData[4];
} STOR_ADDRESS, *PSTOR_ADDRESS;


#ifndef STORPORT_W8
///////////////////////////////////////////////////////////////////
//                         Win7 storport                         //

typedef struct _RAID_POWER_STATE    // Size=0xc
{
    enum _SYSTEM_POWER_STATE SystemState;           // Offset=0x0 Size=0x4
    enum _DEVICE_POWER_STATE DeviceState;           // Offset=0x4 Size=0x4
    unsigned char            SystemPoweringDown;    // Offset=0x8 Size=0x1
} RAID_POWER_STATE;


typedef struct _PORT_CONFIGURATION_INFORMATION  // Size=0xc0
{
    unsigned long Length;                                              // Offset=0x0 Size=0x4
    unsigned long SystemIoBusNumber;                                   // Offset=0x4 Size=0x4
    enum _INTERFACE_TYPE AdapterInterfaceType;                         // Offset=0x8 Size=0x4
    unsigned long BusInterruptLevel;                                   // Offset=0xc Size=0x4
    unsigned long BusInterruptVector;                                  // Offset=0x10 Size=0x4
    enum _KINTERRUPT_MODE InterruptMode;                               // Offset=0x14 Size=0x4
    unsigned long MaximumTransferLength;                               // Offset=0x18 Size=0x4
    unsigned long NumberOfPhysicalBreaks;                              // Offset=0x1c Size=0x4
    unsigned long DmaChannel;                                          // Offset=0x20 Size=0x4
    unsigned long DmaPort;                                             // Offset=0x24 Size=0x4
    enum _DMA_WIDTH DmaWidth;                                          // Offset=0x28 Size=0x4
    enum _DMA_SPEED DmaSpeed;                                          // Offset=0x2c Size=0x4
    unsigned long AlignmentMask;                                       // Offset=0x30 Size=0x4
    unsigned long NumberOfAccessRanges;                                // Offset=0x34 Size=0x4
    struct _ACCESS_RANGE *AccessRanges;                                // Offset=0x38 Size=0x4
    void *Reserved;                                                    // Offset=0x3c Size=0x4
    unsigned char NumberOfBuses;                                       // Offset=0x40 Size=0x1
    char InitiatorBusId[8];                                            // Offset=0x41 Size=0x8
    unsigned char ScatterGather;                                       // Offset=0x49 Size=0x1
    unsigned char Master;                                              // Offset=0x4a Size=0x1
    unsigned char CachesData;                                          // Offset=0x4b Size=0x1
    unsigned char AdapterScansDown;                                    // Offset=0x4c Size=0x1
    unsigned char AtdiskPrimaryClaimed;                                // Offset=0x4d Size=0x1
    unsigned char AtdiskSecondaryClaimed;                              // Offset=0x4e Size=0x1
    unsigned char Dma32BitAddresses;                                   // Offset=0x4f Size=0x1
    unsigned char DemandMode;                                          // Offset=0x50 Size=0x1
    unsigned char MapBuffers;                                          // Offset=0x51 Size=0x1
    unsigned char NeedPhysicalAddresses;                               // Offset=0x52 Size=0x1
    unsigned char TaggedQueuing;                                       // Offset=0x53 Size=0x1
    unsigned char AutoRequestSense;                                    // Offset=0x54 Size=0x1
    unsigned char MultipleRequestPerLu;                                // Offset=0x55 Size=0x1
    unsigned char ReceiveEvent;                                        // Offset=0x56 Size=0x1
    unsigned char RealModeInitialized;                                 // Offset=0x57 Size=0x1
    unsigned char BufferAccessScsiPortControlled;                      // Offset=0x58 Size=0x1
    unsigned char MaximumNumberOfTargets;                              // Offset=0x59 Size=0x1
    unsigned char ReservedUchars[2];                                   // Offset=0x5a Size=0x2
    unsigned long SlotNumber;                                          // Offset=0x5c Size=0x4
    unsigned long BusInterruptLevel2;                                  // Offset=0x60 Size=0x4
    unsigned long BusInterruptVector2;                                 // Offset=0x64 Size=0x4
    enum _KINTERRUPT_MODE InterruptMode2;                              // Offset=0x68 Size=0x4
    unsigned long DmaChannel2;                                         // Offset=0x6c Size=0x4
    unsigned long DmaPort2;                                            // Offset=0x70 Size=0x4
    enum _DMA_WIDTH DmaWidth2;                                         // Offset=0x74 Size=0x4
    enum _DMA_SPEED DmaSpeed2;                                         // Offset=0x78 Size=0x4
    unsigned long DeviceExtensionSize;                                 // Offset=0x7c Size=0x4
    unsigned long SpecificLuExtensionSize;                             // Offset=0x80 Size=0x4
    unsigned long SrbExtensionSize;                                    // Offset=0x84 Size=0x4
    unsigned char Dma64BitAddresses;                                   // Offset=0x88 Size=0x1
    unsigned char ResetTargetSupported;                                // Offset=0x89 Size=0x1
    unsigned char MaximumNumberOfLogicalUnits;                         // Offset=0x8a Size=0x1
    unsigned char WmiDataProvider;                                     // Offset=0x8b Size=0x1
    enum _STOR_SYNCHRONIZATION_MODEL SynchronizationModel;             // Offset=0x8c Size=0x4
    unsigned char (*HwMSInterruptRoutine)(void *, unsigned long);      // Offset=0x90 Size=0x4
    enum _INTERRUPT_SYNCHRONIZATION_MODE InterruptSynchronizationMode; // Offset=0x94 Size=0x4
    struct _MEMORY_REGION DumpRegion;                                  // Offset=0x98 Size=0x18
    unsigned long RequestedDumpBufferSize;                             // Offset=0xb0 Size=0x4
    unsigned char VirtualDevice;                                       // Offset=0xb4 Size=0x1
    //unsigned char __align0[3];                                       // Offset=0xb5 Size=0x3
    unsigned long ExtendedFlags1;                                      // Offset=0xb8 Size=0x4
    unsigned long MaxNumberOfIO;                                       // Offset=0xbc Size=0x4
} PORT_CONFIGURATION_INFORMATION;


typedef struct _RAID_MINIPORT   // Size=0xd8
{
    struct _RAID_ADAPTER_EXTENSION *Adapter;                  // Offset=0x0 Size=0x4
    unsigned char __align0[4];                                // Offset=0x4 Size=0x4
    struct _PORT_CONFIGURATION_INFORMATION PortConfiguration; // Offset=0x8 Size=0xc0
    struct _HW_INITIALIZATION_DATA *HwInitializationData;     // Offset=0xc8 Size=0x4
    struct _RAID_HW_DEVICE_EXT *PrivateDeviceExt;             // Offset=0xcc Size=0x4
    unsigned char Flags[8];                                   // Offset=0xd0 Size=0x8
} RAID_MINIPORT;


typedef struct _RAID_ADAPTER_EXTENSION  // Size=0x5e0
{
    enum _RAID_OBJECT_TYPE ObjectType;              // Offset=0x0 Size=0x4
    struct _DEVICE_OBJECT *DeviceObject;            // Offset=0x4 Size=0x4
    struct _RAID_DRIVER_EXTENSION *Driver;          // Offset=0x8 Size=0x4
    struct _DEVICE_OBJECT *LowerDeviceObject;       // Offset=0xc Size=0x4
    struct _DEVICE_OBJECT *PhysicalDeviceObject;    // Offset=0x10 Size=0x4
    struct _UNICODE_STRING DeviceName;              // Offset=0x14 Size=0x8
    unsigned long PortNumber;                       // Offset=0x1c Size=0x4
    struct _LIST_ENTRY NextAdapter;                 // Offset=0x20 Size=0x8
    unsigned long SlowLock;                         // Offset=0x28 Size=0x4
    enum _DEVICE_STATE DeviceState;                 // Offset=0x2c Size=0x4
    unsigned long PagingPathCount;                  // Offset=0x30 Size=0x4
    unsigned long CrashDumpPathCount;               // Offset=0x34 Size=0x4
    unsigned long HiberPathCount;                   // Offset=0x38 Size=0x4
    unsigned long Flags;                            // Offset=0x3c Size=0x4
    struct _STOR_LOCKED_LIST UnitList;              // Offset=0x40 Size=0x10
    struct _STOR_DICTIONARY UnitDictionary;         // Offset=0x50 Size=0x1c
    struct _STOR_LOCKED_LIST ZombieList;            // Offset=0x6c Size=0x10
    unsigned char __align0[4];                      // Offset=0x7c Size=0x4
    union _SLIST_HEADER CompletedList;              // Offset=0x80 Size=0x8
    struct _EX_RUNDOWN_REF_CACHE_AWARE *RemoveLock; // Offset=0x88 Size=0x4
    struct _RAID_POWER_STATE Power;                 // Offset=0x8c Size=0xc
    struct _RAID_RESOURCE_LIST ResourceList;        // Offset=0x98 Size=0x8
    struct _RAID_MINIPORT Miniport;                 // Offset=0xa0 Size=0xd8
    struct _RAID_BUS_INTERFACE Bus;                 // Offset=0x178 Size=0x24
    struct _KINTERRUPT *Interrupt;                  // Offset=0x19c Size=0x4
    unsigned long InterruptIrql;                    // Offset=0x1a0 Size=0x4
    unsigned long InterruptVersion;                 // Offset=0x1a4 Size=0x4
    KSPIN_LOCK StartIoLock;                         // Offset=0x1a8 Size=0x4
    enum _STOR_SYNCHRONIZATION_MODEL IoModel;       // Offset=0x1ac Size=0x4
    // ...
} RAID_ADAPTER_EXTENSION, *PRAID_ADAPTER_EXTENSION;


#pragma warning(push)
#pragma warning(disable:4200)               // HwDeviceExtension[] warning
typedef struct _RAID_HW_DEVICE_EXT          // Size=0x4
{
    struct _RAID_MINIPORT *Miniport;        // Offset=0x0 Size=0x4
    unsigned char HwDeviceExtension[];      // Offset=0x4
} RAID_HW_DEVICE_EXT;
#pragma warning(pop)


typedef struct _RAID_UNIT_EXTENSION             // Size=0x2a0
{
    enum _RAID_OBJECT_TYPE ObjectType;       // Offset=0x0 Size=0x4
    struct _DEVICE_OBJECT *DeviceObject;     // Offset=0x4 Size=0x4
    struct _RAID_ADAPTER_EXTENSION *Adapter; // Offset=0x8 Size=0x4
    unsigned long SlowLock;                  // Offset=0xc Size=0x4
    enum _DEVICE_STATE DeviceState;          // Offset=0x10 Size=0x4
    struct _LIST_ENTRY NextUnit;             // Offset=0x14 Size=0x8
    struct _LIST_ENTRY UnitTableLink;        // Offset=0x1c Size=0x8
    struct _STOR_SCSI_ADDRESS Address;       // Offset=0x24 Size=0x4
    //...
} RAID_UNIT_EXTENSION, *PRAID_UNIT_EXTENSION;


#else 
///////////////////////////////////////////////////////////////////
//                       Win8+ storport                          //

typedef struct _RAID_POWER_STATE// Size=0x10
{
    enum _SYSTEM_POWER_STATE SystemState; // Offset=0x0 Size=0x4
    enum _DEVICE_POWER_STATE DeviceState; // Offset=0x4 Size=0x4
    enum _POWER_ACTION PowerAction;        // Offset=0x8 Size=0x4
    unsigned char SystemPoweringDown;     // Offset=0xc Size=0x1
} RAID_POWER_STATE;


typedef struct _PORT_CONFIGURATION_INFORMATION  // Size=0xd0
{
    unsigned long Length;                                              // Offset=0x0 Size=0x4
    unsigned long SystemIoBusNumber;                                   // Offset=0x4 Size=0x4
    enum _INTERFACE_TYPE AdapterInterfaceType;                         // Offset=0x8 Size=0x4
    unsigned long BusInterruptLevel;                                   // Offset=0xc Size=0x4
    unsigned long BusInterruptVector;                                  // Offset=0x10 Size=0x4
    enum _KINTERRUPT_MODE InterruptMode;                               // Offset=0x14 Size=0x4
    unsigned long MaximumTransferLength;                               // Offset=0x18 Size=0x4
    unsigned long NumberOfPhysicalBreaks;                              // Offset=0x1c Size=0x4
    unsigned long DmaChannel;                                          // Offset=0x20 Size=0x4
    unsigned long DmaPort;                                             // Offset=0x24 Size=0x4
    enum _DMA_WIDTH DmaWidth;                                          // Offset=0x28 Size=0x4
    enum _DMA_SPEED DmaSpeed;                                          // Offset=0x2c Size=0x4
    unsigned long AlignmentMask;                                       // Offset=0x30 Size=0x4
    unsigned long NumberOfAccessRanges;                                // Offset=0x34 Size=0x4
    struct _ACCESS_RANGE *AccessRanges;                                // Offset=0x38 Size=0x4
    void *MiniportDumpData;                                            // Offset=0x3c Size=0x4
    unsigned char NumberOfBuses;                                       // Offset=0x40 Size=0x1
    char InitiatorBusId[8];                                            // Offset=0x41 Size=0x8
    unsigned char ScatterGather;                                       // Offset=0x49 Size=0x1
    unsigned char Master;                                              // Offset=0x4a Size=0x1
    unsigned char CachesData;                                          // Offset=0x4b Size=0x1
    unsigned char AdapterScansDown;                                    // Offset=0x4c Size=0x1
    unsigned char AtdiskPrimaryClaimed;                                // Offset=0x4d Size=0x1
    unsigned char AtdiskSecondaryClaimed;                              // Offset=0x4e Size=0x1
    unsigned char Dma32BitAddresses;                                   // Offset=0x4f Size=0x1
    unsigned char DemandMode;                                          // Offset=0x50 Size=0x1
    unsigned char MapBuffers;                                          // Offset=0x51 Size=0x1
    unsigned char NeedPhysicalAddresses;                               // Offset=0x52 Size=0x1
    unsigned char TaggedQueuing;                                       // Offset=0x53 Size=0x1
    unsigned char AutoRequestSense;                                    // Offset=0x54 Size=0x1
    unsigned char MultipleRequestPerLu;                                // Offset=0x55 Size=0x1
    unsigned char ReceiveEvent;                                        // Offset=0x56 Size=0x1
    unsigned char RealModeInitialized;                                 // Offset=0x57 Size=0x1
    unsigned char BufferAccessScsiPortControlled;                      // Offset=0x58 Size=0x1
    unsigned char MaximumNumberOfTargets;                              // Offset=0x59 Size=0x1
    unsigned char SrbType;                                             // Offset=0x5a Size=0x1
    unsigned char AddressType;                                         // Offset=0x5b Size=0x1
    unsigned long SlotNumber;                                          // Offset=0x5c Size=0x4
    unsigned long BusInterruptLevel2;                                  // Offset=0x60 Size=0x4
    unsigned long BusInterruptVector2;                                 // Offset=0x64 Size=0x4
    enum _KINTERRUPT_MODE InterruptMode2;                              // Offset=0x68 Size=0x4
    unsigned long DmaChannel2;                                         // Offset=0x6c Size=0x4
    unsigned long DmaPort2;                                            // Offset=0x70 Size=0x4
    enum _DMA_WIDTH DmaWidth2;                                         // Offset=0x74 Size=0x4
    enum _DMA_SPEED DmaSpeed2;                                         // Offset=0x78 Size=0x4
    unsigned long DeviceExtensionSize;                                 // Offset=0x7c Size=0x4
    unsigned long SpecificLuExtensionSize;                             // Offset=0x80 Size=0x4
    unsigned long SrbExtensionSize;                                    // Offset=0x84 Size=0x4
    unsigned char Dma64BitAddresses;                                   // Offset=0x88 Size=0x1
    unsigned char ResetTargetSupported;                                // Offset=0x89 Size=0x1
    unsigned char MaximumNumberOfLogicalUnits;                         // Offset=0x8a Size=0x1
    unsigned char WmiDataProvider;                                     // Offset=0x8b Size=0x1
    enum _STOR_SYNCHRONIZATION_MODEL SynchronizationModel;             // Offset=0x8c Size=0x4
    unsigned char (*HwMSInterruptRoutine)(void *, unsigned long);      // Offset=0x90 Size=0x4
    enum _INTERRUPT_SYNCHRONIZATION_MODE InterruptSynchronizationMode; // Offset=0x94 Size=0x4
    struct _MEMORY_REGION DumpRegion;                                  // Offset=0x98 Size=0x18
    unsigned long RequestedDumpBufferSize;                             // Offset=0xb0 Size=0x4
    unsigned char VirtualDevice;                                       // Offset=0xb4 Size=0x1
    unsigned char DumpMode;                                            // Offset=0xb5 Size=0x1
    //unsigned char __align0[2];                                         // Offset=0xb6 Size=0x2
    unsigned long ExtendedFlags1;                                      // Offset=0xb8 Size=0x4
    unsigned long MaxNumberOfIO;                                       // Offset=0xbc Size=0x4
    unsigned long MaxIOsPerLun;                                        // Offset=0xc0 Size=0x4
    unsigned long InitialLunQueueDepth;                                // Offset=0xc4 Size=0x4
    unsigned long BusResetHoldTime;                                    // Offset=0xc8 Size=0x4
    unsigned long FeatureSupport;                                      // Offset=0xcc Size=0x4
} PORT_CONFIGURATION_INFORMATION;


struct _UNIT_CONTROL_LIST           // Size=0x10
{
    unsigned long MaxControlType;       // Offset=0x0 Size=0x4
    unsigned char SupportedTypeList[9]; // Offset=0x4 Size=0x9
};

struct _ADAPTER_CONTROL_LIST        // Size=0x14
{
    unsigned long MaxControlType;        // Offset=0x0 Size=0x4
    unsigned char SupportedTypeList[14]; // Offset=0x4 Size=0xe
};

typedef struct _RAID_MINIPORT       // Size=0x108
{
    struct _RAID_ADAPTER_EXTENSION *Adapter;                  // Offset=0x0 Size=0x4
    unsigned char __align0[4];                                // Offset=0x4 Size=0x4
    struct _PORT_CONFIGURATION_INFORMATION PortConfiguration; // Offset=0x8 Size=0xd0
    struct _HW_INITIALIZATION_DATA *HwInitializationData;     // Offset=0xd8 Size=0x4
    struct _RAID_HW_DEVICE_EXT *PrivateDeviceExt;             // Offset=0xdc Size=0x4
    unsigned char Flags;                                      // Offset=0xe0 Size=0x1
    //unsigned char __align1[3];// Offset=0xe1 Size=0x3
    struct _UNIT_CONTROL_LIST UnitControlList;                // Offset=0xe4 Size=0x10
    struct _ADAPTER_CONTROL_LIST AdapterControlList;          // Offset=0xf4 Size=0x14
} RAID_MINIPORT;


typedef struct _RAID_ADAPTER_EXTENSION  // Size=0xbc0
{
    enum _RAID_OBJECT_TYPE ObjectType;              // Offset=0x0 Size=0x4
    struct _DEVICE_OBJECT *DeviceObject;            // Offset=0x4 Size=0x4
    struct _RAID_DRIVER_EXTENSION *Driver;          // Offset=0x8 Size=0x4
    struct _DEVICE_OBJECT *LowerDeviceObject;       // Offset=0xc Size=0x4
    struct _DEVICE_OBJECT *PhysicalDeviceObject;    // Offset=0x10 Size=0x4
    struct _UNICODE_STRING DeviceName;              // Offset=0x14 Size=0x8
    unsigned long PortNumber;                       // Offset=0x1c Size=0x4
    struct _LIST_ENTRY NextAdapter;                 // Offset=0x20 Size=0x8
    unsigned long SlowLock;                         // Offset=0x28 Size=0x4
    enum _DEVICE_STATE DeviceState;                 // Offset=0x2c Size=0x4
    unsigned long PagingPathCount;                  // Offset=0x30 Size=0x4
    unsigned long CrashDumpPathCount;               // Offset=0x34 Size=0x4
    unsigned long HiberPathCount;                   // Offset=0x38 Size=0x4
    unsigned char Flags[8];                         // Offset=0x3c Size=0x8
    struct _STOR_LOCKED_LIST UnitList;              // Offset=0x44 Size=0x10
    struct _STOR_DICTIONARY UnitDictionary;         // Offset=0x54 Size=0x1c
    struct _STOR_LOCKED_LIST ZombieList;            // Offset=0x70 Size=0x10
    union _SLIST_HEADER CompletedList;              // Offset=0x80 Size=0x8
    struct _SINGLE_LIST_ENTRY *SavedXrbList;        // Offset=0x88 Size=0x4
    long AdapterDpcRoutineLock;                     // Offset=0x8c Size=0x4
    struct _EX_RUNDOWN_REF_CACHE_AWARE *RemoveLock; // Offset=0x90 Size=0x4
    struct _RAID_POWER_STATE Power;                 // Offset=0x94 Size=0x10
    struct _RAID_RESOURCE_LIST ResourceList;        // Offset=0xa4 Size=0x8
    unsigned char __align1[4];                      // Offset=0xac Size=0x4
    struct _RAID_MINIPORT Miniport;                 // Offset=0xb0 Size=0x108
    struct _RAID_BUS_INTERFACE Bus;                 // Offset=0x1b8 Size=0x24
    struct _KINTERRUPT *Interrupt;                  // Offset=0x1dc Size=0x4
    unsigned long InterruptIrql;                    // Offset=0x1e0 Size=0x4
    unsigned long InterruptVersion;                 // Offset=0x1e4 Size=0x4
    unsigned long StartIoLock;                      // Offset=0x1e8 Size=0x4
    enum _STOR_SYNCHRONIZATION_MODEL IoModel;       // Offset=0x1ec Size=0x4
    //...
} RAID_ADAPTER_EXTENSION, *PRAID_ADAPTER_EXTENSION;


typedef struct _RAID_HW_DEVICE_EXT          // Size=0x8
{
    struct _RAID_MINIPORT *Miniport;        // Offset=0x0 Size=0x4
    void *Reserved;                         // Offset=0x4 Size=0x4
    unsigned char HwDeviceExtension[];      // Offset=0x8
} RAID_HW_DEVICE_EXT;


typedef struct _RAID_UNIT_EXTENSION          // Size=0x2a0
{
    enum _RAID_OBJECT_TYPE ObjectType;       // Offset=0x0 Size=0x4
    struct _DEVICE_OBJECT *DeviceObject;     // Offset=0x4 Size=0x4
    struct _RAID_ADAPTER_EXTENSION *Adapter; // Offset=0x8 Size=0x4
    unsigned long SlowLock;                  // Offset=0xc Size=0x4
    enum _DEVICE_STATE DeviceState;          // Offset=0x10 Size=0x4
    struct _LIST_ENTRY NextUnit;             // Offset=0x14 Size=0x8
    struct _LIST_ENTRY UnitTableLink;        // Offset=0x1c Size=0x8
    union                                    // Size=0xc
    {
        struct _STOR_ADDR_BTL8 StorAddressBTL8; // Offset=0x24 Size=0xc
        struct                                  // Size=0xc
        {
            unsigned short AddressType;        // Offset=0x24 Size=0x2
            unsigned short AddressPort;        // Offset=0x26 Size=0x2
            unsigned long AddressLength;       // Offset=0x28 Size=0x4
            struct _STOR_SCSI_ADDRESS Address; // Offset=0x2c Size=0x4
        };
    };
    //...
} RAID_UNIT_EXTENSION, *PRAID_UNIT_EXTENSION;

#endif
//                       Win8+ storport                          //
///////////////////////////////////////////////////////////////////




typedef struct _STOR_TIMER_CONTEXT                 // Size=0x60
{
    RAID_ADAPTER_EXTENSION *Adapter;               // Offset=0x0 Size=0x4
    unsigned char __align0[4];                     // Offset=0x4 Size=0x4
    KTIMER                  Timer;                 // Offset=0x8 Size=0x28
    KDPC                    TimerDpc;              // Offset=0x30 Size=0x20
    void         (*Callback)(void *, void *);      // Offset=0x50 Size=0x4
    void                   *CallbackContext;       // Offset=0x54 Size=0x4
    IO_WORKITEM            *FreeTimerWorkItem;     // Offset=0x58 Size=0x4
} STOR_TIMER_CONTEXT;



/////////////////////////////////////////////////////////
// Static variables

//UNICODE_STRING StorPortInitializeName       = RTL_CONSTANT_STRING(L"StorPortInitialize");
//UNICODE_STRING StorPortRegistryReadName     = RTL_CONSTANT_STRING(L"StorPortRegistryRead");
//UNICODE_STRING StorPortExtendedFunctionName = RTL_CONSTANT_STRING(L"StorPortExtendedFunction");
// LIST_ENTRY *gPsLoadedModuleList;

static const char* DUMPPREFIX = "dump";


static BOOLEAN  gIsWeInitialized = FALSE;
       BOOLEAN  gDumpMode;

PVOID    gScsiPortMoveMemory;
PVOID    gScsiPortNotification;
PVOID    gStorPortAllocateRegistryBuffer;
PVOID    gStorPortBusy;
PVOID    gStorPortCompleteRequest;
PVOID    gStorPortConvertUlongToPhysicalAddress;
PVOID    gStorPortDebugPrint;
PVOID    gStorPortDeviceBusy;
PVOID    gStorPortDeviceReady;
PVOID    gStorPortFreeDeviceBase;
PVOID    gStorPortFreeRegistryBuffer;
PVOID    gStorPortGetBusData;
PVOID    gStorPortGetDeviceBase;
PVOID    gStorPortGetLogicalUnit;
PVOID    gStorPortGetPhysicalAddress;
PVOID    gStorPortGetScatterGatherList;
PVOID    gStorPortGetSrb;
PVOID    gStorPortGetUncachedExtension;
PVOID    gStorPortGetVirtualAddress;
PVOID    gStorPortLogError;
PVOID    gStorPortMoveMemory;
PVOID    gStorPortNotification;
PVOID    gStorPortPause;
PVOID    gStorPortPauseDevice;
PVOID    gStorPortQuerySystemTime;
PVOID    gStorPortReadPortBufferUchar;
PVOID    gStorPortReadPortBufferUlong;
PVOID    gStorPortReadPortBufferUshort;
PVOID    gStorPortReadPortUchar;
PVOID    gStorPortReadPortUlong;
PVOID    gStorPortReadPortUshort;
PVOID    gStorPortReadRegisterBufferUchar;
PVOID    gStorPortReadRegisterBufferUlong;
PVOID    gStorPortReadRegisterBufferUshort;
PVOID    gStorPortReadRegisterUchar;
PVOID    gStorPortReadRegisterUlong;
PVOID    gStorPortReadRegisterUshort;
PVOID    gStorPortReady;
PVOID    gStorPortRegistryWrite;
PVOID    gStorPortResume;
PVOID    gStorPortResumeDevice;
PVOID    gStorPortSetBusDataByOffset;
PVOID    gStorPortSetDeviceQueueDepth;
PVOID    gStorPortStallExecution;
PVOID    gStorPortSynchronizeAccess;
PVOID    gStorPortValidateRange;
PVOID    gStorPortWritePortBufferUchar;
PVOID    gStorPortWritePortBufferUlong;
PVOID    gStorPortWritePortBufferUshort;
PVOID    gStorPortWritePortUchar;
PVOID    gStorPortWritePortUlong;
PVOID    gStorPortWritePortUshort;
PVOID    gStorPortWriteRegisterBufferUchar;
PVOID    gStorPortWriteRegisterBufferUlong;
PVOID    gStorPortWriteRegisterBufferUshort;
PVOID    gStorPortWriteRegisterUchar;
PVOID    gStorPortWriteRegisterUlong;
PVOID    gStorPortWriteRegisterUshort;


ULONG
(*gStorPortInitialize) (
    PVOID Argument1,
    PVOID Argument2,
    struct _HW_INITIALIZATION_DATA *HwInitializationData,
    PVOID HwContext );

BOOLEAN
(*gStorPortRegistryRead) (
    PVOID HwDeviceExtension,
    PUCHAR ValueName,
    ULONG Global,
    ULONG Type,
    PUCHAR Buffer,
    PULONG BufferLength );

ULONG
(*gStorPortExtendedFunction) (
    STORPORT_FUNCTION_CODE FunctionCode,
    PVOID HwDeviceExtension,
    ULONG_PTR va1, ULONG_PTR va2, ULONG_PTR va3, ULONG_PTR va4, ULONG_PTR va5, ULONG_PTR va6, ULONG_PTR va7,
    ULONG_PTR va8, ULONG_PTR va9, ULONG_PTR va10, ULONG_PTR va11, ULONG_PTR va12, ULONG_PTR va13);

// Static variables
/////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////
//                Functions


BOOLEAN
isDumpMode(PUNICODE_STRING RegistryPath)
{
    ANSI_STRING AnsiString;
    PCHAR       s;
    NTSTATUS    Status;

    if (RegistryPath == NULL)
        return TRUE;

    do {
        Status = RtlUnicodeStringToAnsiString(&AnsiString, RegistryPath, TRUE);

        if (NT_SUCCESS (Status)) {
            break;
        }

        KeDelayExecutionThread (KernelMode, FALSE, (PLARGE_INTEGER)&MmShortTime);

    } while (TRUE);

    s = AnsiString.Buffer + AnsiString.Length;
    while (s > AnsiString.Buffer && *--s)
    {
        if (*s == (UCHAR)OBJ_NAME_PATH_SEPARATOR)
        {
            s += 1;
            break;
        }
    }

    if (strstr(s, DUMPPREFIX) == s)
        return TRUE;
    else
        return FALSE;
    
}


#define GETROUTINE(x)                                                   \
           RtlInitAnsiString(&AnsiString, #x);                        \
           g##x = MiFindExportedRoutineByName_k8(BaseAdress, &AnsiString);


#define GETROUTINETYPE(x, y)                                                   \
           RtlInitAnsiString(&AnsiString, #x);                        \
           g##x = (y) MiFindExportedRoutineByName_k8(BaseAdress, &AnsiString);


void
StorportInit(PDEVICE_OBJECT DeviceObject, PUNICODE_STRING  RegistryPath) 
{
    ANSI_STRING     AnsiString;
    PVOID           BaseAdress;

    gIsWeInitialized = TRUE;
    gDumpMode = isDumpMode(RegistryPath);

    if (gDumpMode) {
        BaseAdress = GetModuleBaseAddress_k8("dump_diskdump.sys");
        if (BaseAdress == NULL) {
            gDumpMode = FALSE;      // error,  dump_diskdump.sys not found
            return;
        }

        GETROUTINE(ScsiPortMoveMemory)
        GETROUTINE(ScsiPortNotification)
        GETROUTINE(StorPortAllocateRegistryBuffer)
        GETROUTINE(StorPortBusy)
        GETROUTINE(StorPortCompleteRequest)
        GETROUTINE(StorPortConvertUlongToPhysicalAddress)
        GETROUTINE(StorPortDebugPrint)
        GETROUTINE(StorPortDeviceBusy)
        GETROUTINE(StorPortDeviceReady)
        GETROUTINETYPE(StorPortExtendedFunction, ULONG (__stdcall *)(STORPORT_FUNCTION_CODE,PVOID,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR,ULONG_PTR))
        GETROUTINE(StorPortFreeDeviceBase)
        GETROUTINE(StorPortFreeRegistryBuffer)
        GETROUTINE(StorPortGetBusData)
        GETROUTINE(StorPortGetDeviceBase)
        GETROUTINE(StorPortGetLogicalUnit)
        GETROUTINE(StorPortGetPhysicalAddress)
        GETROUTINE(StorPortGetScatterGatherList)
        GETROUTINE(StorPortGetSrb)
        GETROUTINE(StorPortGetUncachedExtension)
        GETROUTINE(StorPortGetVirtualAddress)
        GETROUTINE(StorPortLogError)
        GETROUTINETYPE(StorPortInitialize, ULONG (__stdcall *)(PVOID,PVOID,_HW_INITIALIZATION_DATA *,PVOID))
        GETROUTINE(StorPortMoveMemory)
        GETROUTINE(StorPortNotification)
        GETROUTINE(StorPortPause)
        GETROUTINE(StorPortPauseDevice)
        GETROUTINE(StorPortQuerySystemTime)
        GETROUTINE(StorPortReadPortBufferUchar)
        GETROUTINE(StorPortReadPortBufferUlong)
        GETROUTINE(StorPortReadPortBufferUshort)
        GETROUTINE(StorPortReadPortUchar)
        GETROUTINE(StorPortReadPortUlong)
        GETROUTINE(StorPortReadPortUshort)
        GETROUTINE(StorPortReadRegisterBufferUchar)
        GETROUTINE(StorPortReadRegisterBufferUlong)
        GETROUTINE(StorPortReadRegisterBufferUshort)
        GETROUTINE(StorPortReadRegisterUchar)
        GETROUTINE(StorPortReadRegisterUlong)
        GETROUTINE(StorPortReadRegisterUshort)
        GETROUTINE(StorPortReady)
        GETROUTINETYPE(StorPortRegistryRead, BOOLEAN (__stdcall *)(PVOID,PUCHAR,ULONG,ULONG,PUCHAR,PULONG))
        GETROUTINE(StorPortRegistryWrite)
        GETROUTINE(StorPortResume)
        GETROUTINE(StorPortResumeDevice)
        GETROUTINE(StorPortSetBusDataByOffset)
        GETROUTINE(StorPortSetDeviceQueueDepth)
        GETROUTINE(StorPortStallExecution)
        GETROUTINE(StorPortSynchronizeAccess)
        GETROUTINE(StorPortValidateRange)
        GETROUTINE(StorPortWritePortBufferUchar)
        GETROUTINE(StorPortWritePortBufferUlong)
        GETROUTINE(StorPortWritePortBufferUshort)
        GETROUTINE(StorPortWritePortUchar)
        GETROUTINE(StorPortWritePortUlong)
        GETROUTINE(StorPortWritePortUshort)
        GETROUTINE(StorPortWriteRegisterBufferUchar)
        GETROUTINE(StorPortWriteRegisterBufferUlong)
        GETROUTINE(StorPortWriteRegisterBufferUshort)
        GETROUTINE(StorPortWriteRegisterUchar)
        GETROUTINE(StorPortWriteRegisterUlong)
        GETROUTINE(StorPortWriteRegisterUshort)
    }
}


NTSTATUS NtStatusToStorStatus(NTSTATUS NTStatus)
{
    if (NTStatus == STATUS_INVALID_LEVEL)
        return STOR_STATUS_INVALID_IRQL;
    if (NTStatus == STATUS_INSUFFICIENT_RESOURCES)
        return STOR_STATUS_INSUFFICIENT_RESOURCES;
    if (NTStatus == STATUS_BUFFER_OVERFLOW)
        return STOR_STATUS_BUFFER_TOO_SMALL;
    if (NTStatus == STATUS_INVALID_PARAMETER)
        return STOR_STATUS_INVALID_PARAMETER;
    if (NTStatus == STATUS_SUCCESS)
        return STOR_STATUS_SUCCESS;

    return STOR_STATUS_UNSUCCESSFUL; // default
}


FORCEINLINE
RAID_ADAPTER_EXTENSION *
GetAdapter (PVOID HwDeviceExtension)
{
       return ( CONTAINING_RECORD(
                  HwDeviceExtension,
                  RAID_HW_DEVICE_EXT,                        
                  HwDeviceExtension) )->Miniport->Adapter;
}


ULONG
RaidSyncAcpiEvalMethod (
    PDEVICE_OBJECT  DeviceObject,
    PVOID           InputBuffer,
    ULONG           InputBufferLength,
    PVOID           OutputBuffer,
    ULONG           OutputBufferLength,
    PULONG          BytesReturned )
{
    KEVENT              Event;
    IO_STATUS_BLOCK     IoStatusBlock;
    PDEVICE_OBJECT      UpperDevice;
    PIRP                Irp;
    NTSTATUS            status;

    UpperDevice = NULL;
    IoStatusBlock.Status = 0;
    IoStatusBlock.Information = 0;
    TRY {    
        if (KeGetCurrentIrql() > PASSIVE_LEVEL) {
            status = STATUS_INVALID_LEVEL;
            LEAVE;
        }

        KeInitializeEvent(&Event, NotificationEvent, FALSE);
        UpperDevice = IoGetAttachedDeviceReference(DeviceObject);
        Irp = IoBuildDeviceIoControlRequest(
                IOCTL_ACPI_ASYNC_EVAL_METHOD,                      // IO Control Code
                UpperDevice,
                InputBuffer,
                InputBufferLength,
                OutputBuffer,
                OutputBufferLength,
                FALSE,
                &Event,
                &IoStatusBlock);
     
        if  (Irp == NULL) {
                status = STATUS_INSUFFICIENT_RESOURCES;
                LEAVE;
        }

        Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
        status = IoCallDriver(UpperDevice, Irp);

        if ( status == STATUS_PENDING ) {
            KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
            status = IoStatusBlock.Status;
        }
        
        if ( status >= 0 && IoStatusBlock.Information == 0 && OutputBufferLength > 0 )
            status = STATUS_UNSUCCESSFUL;

        if (BytesReturned != NULL) {
            if (status >= 0)
                *BytesReturned = (ULONG) IoStatusBlock.Information;
        }
    }
    FINALLY {
        if (BytesReturned != NULL && status < 0)
                *BytesReturned = 0;

        if (UpperDevice)
            ObDereferenceObject(UpperDevice);
    }

    return NtStatusToStorStatus(status);

}


KIRQL
RaidAdapterAcquireInterruptLock (
    RAID_ADAPTER_EXTENSION  *Adapter )
{
    if (Adapter->Interrupt)
        return KeAcquireInterruptSpinLock(Adapter->Interrupt);
    else
        return PASSIVE_LEVEL;
}


void
RaidAdapterReleaseInterruptLock (
    RAID_ADAPTER_EXTENSION  *Adapter,
    KIRQL    OldIrq )
{
    if (Adapter->Interrupt)
        KeReleaseInterruptSpinLock(Adapter->Interrupt, OldIrq);
}


void
RaidAdapterAcquireStartIoLock (
    RAID_ADAPTER_EXTENSION  *Adapter,
    KLOCK_QUEUE_HANDLE      *LockHandle )
{
    KIRQL   *LockHandleAsIRQL = (KIRQL *) LockHandle;
    if (Adapter->IoModel == StorSynchronizeFullDuplex)
        KeAcquireInStackQueuedSpinLock(&Adapter->StartIoLock , LockHandle);
    else
        *LockHandleAsIRQL = RaidAdapterAcquireInterruptLock(Adapter);
}


void
RaidAdapterReleaseStartIoLock (
    RAID_ADAPTER_EXTENSION  *Adapter,
    KLOCK_QUEUE_HANDLE      *LockHandle)
{
    KIRQL   *LockHandleAsIRQL = (KIRQL *) LockHandle;
    if (Adapter->IoModel == StorSynchronizeFullDuplex)
        KeReleaseInStackQueuedSpinLock(LockHandle);
    else
        RaidAdapterReleaseInterruptLock(Adapter, *LockHandleAsIRQL);
}


NTSTATUS
StorFindDictionary (
    STOR_DICTIONARY    *UnitDictionary,
    ULONG_PTR           AddressToLong,
    PVOID              *ResultPtr )
{
    void * ( *GetKeyRoutine)     (struct _LIST_ENTRY *);
    long   ( *CompareKeyRoutine) (void *, void *);
    ULONG  ( *HashKeyRoutine)    (void *);

    LIST_ENTRY  *Entries;
    LIST_ENTRY  *ListHead;
    NTSTATUS     status;

    Entries             = UnitDictionary->Entries;
    GetKeyRoutine       = UnitDictionary->GetKeyRoutine;
    CompareKeyRoutine   = UnitDictionary->CompareKeyRoutine;
    HashKeyRoutine      = UnitDictionary->HashKeyRoutine;
    
    ListHead = (LIST_ENTRY  *) ((UCHAR *)Entries +
                 sizeof(LIST_ENTRY) *
                 (HashKeyRoutine((void *)AddressToLong) % UnitDictionary->MaxEntryCount));

    for (LIST_ENTRY* List = ListHead->Flink; List != ListHead; List = List->Flink)
    {
        
        status = CompareKeyRoutine(GetKeyRoutine(List), (void *)AddressToLong);
        if (!status)
        {
            if (ResultPtr)
                *ResultPtr = List;
            return STATUS_SUCCESS;
        }
        if (status < 0)
        {
            if (ResultPtr)
                *ResultPtr = 0;
            return STATUS_NOT_FOUND;
        }
    }

    return STATUS_NOT_FOUND;
}    


RAID_UNIT_EXTENSION *
RaidAdapterFindUnitAtDirql (
   RAID_ADAPTER_EXTENSION   *Adapter,
   STOR_SCSI_ADDRESS         Address )
{
    PVOID   UnitAsList;
    ULONG   AddressToLong;

    AddressToLong = Address.PathId << 16 | Address.TargetId << 8 | Address.Lun ;
    if (StorFindDictionary(&Adapter->UnitDictionary, AddressToLong, &UnitAsList) < 0)
        return NULL;
    else
        return CONTAINING_RECORD(
                  UnitAsList,
                  RAID_UNIT_EXTENSION,                        
                  UnitTableLink);
}


RAID_UNIT_EXTENSION *
RaidAdapterFindUnit (
    RAID_ADAPTER_EXTENSION *Adapter,
    STOR_SCSI_ADDRESS       Address )
{
    LIST_ENTRY             *List;
    LIST_ENTRY             *ListHead;
    KLOCK_QUEUE_HANDLE      LockHandle;
    RAID_UNIT_EXTENSION    *result;
    RAID_UNIT_EXTENSION    *UNIT;
    KIRQL                   CurrentIrql;
    KIRQL                   OldIrql;

    CurrentIrql = KeGetCurrentIrql();

    if (CurrentIrql == PASSIVE_LEVEL) {
        KeAcquireInStackQueuedSpinLock(&Adapter->UnitList.Lock, &LockHandle);

        ListHead = &Adapter->UnitList.List;
        List = ListHead->Flink;

        result = NULL;
        while (List != ListHead) {
            UNIT = CONTAINING_RECORD(List,
                                     RAID_UNIT_EXTENSION,
                                     NextUnit);

            if ( UNIT->Address.PathId   == Address.PathId     &&
                 UNIT->Address.TargetId == Address.TargetId   &&
                 UNIT->Address.Lun      == Address.Lun ) {
                    result = UNIT;
                    break;
            }

            List = List->Flink;
        }

        KeReleaseInStackQueuedSpinLock(&LockHandle);
        return result;
    }
    else
        if (CurrentIrql >= Adapter->InterruptIrql )
            return RaidAdapterFindUnitAtDirql(Adapter, Address);
        else {
            OldIrql = RaidAdapterAcquireInterruptLock(Adapter);
            result = RaidAdapterFindUnitAtDirql(Adapter, Address);
            RaidAdapterReleaseInterruptLock(Adapter, OldIrql);
            return result;
        }
}


typedef void (*TimerCallback)(void *, void *);

void
StorportTimerDpc (
     PKDPC                  Dpc,
     STOR_TIMER_CONTEXT    *TIMER_CONTEXT,
     PVOID                  SystemArgument1,
     PVOID                  SystemArgument2 )
{
    TimerCallback       Callback;
    KLOCK_QUEUE_HANDLE  Lock;

    Callback = (TimerCallback) InterlockedExchangePointer((PVOID *)&TIMER_CONTEXT->Callback, NULL);
    if (Callback) {
        RaidAdapterAcquireStartIoLock(TIMER_CONTEXT->Adapter, &Lock);
        Callback(TIMER_CONTEXT->Adapter->Miniport.PrivateDeviceExt->HwDeviceExtension, TIMER_CONTEXT->CallbackContext);
        RaidAdapterReleaseStartIoLock(TIMER_CONTEXT->Adapter, &Lock);
    }
}


void
StorpFreeTimerWorkItemCallback(
    PDEVICE_OBJECT      DeviceObject,
    STOR_TIMER_CONTEXT  *Timer)
{
  KeFlushQueuedDpcs();
  IoFreeWorkItem(Timer->FreeTimerWorkItem);
  ExFreePoolWithTag(Timer, 'TAaR');
  //InterlockedDecrement(&gAllocatedTimers);
}


typedef void
HW_TIMER_EX (
    PVOID DeviceExtension,
    PVOID Context );

typedef HW_TIMER_EX *PHW_TIMER_EX;


// pattern generator.
#define PATTERN "InTheHeatOfTheNightILooseControl" // (c) Sandra
#define PATTERN_SIZE (sizeof(PATTERN) - 1)
static const UCHAR gPattern[] = PATTERN;


VOID
FillBufferWithPattern (UCHAR *Buffer, ULONG BufferLength )
{
    ULONG Rotor = 0;

    for (ULONG Index = 0; Index < BufferLength; Index ++) {
        Buffer[Index] = gPattern[Rotor] XOR (UCHAR)(0xA1 - Index);
        Rotor ++;
        if (Rotor == PATTERN_SIZE) {
            Rotor = 0;
        }
    }
}


BOOLEAN
isPatternMatch (UCHAR *Buffer, ULONG BufferLength )
{
    ULONG Rotor = 0;

    for (ULONG Index = 0; Index < BufferLength; Index ++) {
        if (Buffer[Index] != (gPattern[Rotor] XOR (UCHAR)(0xA1 - Index)))
            return FALSE;

        Rotor ++;
        if (Rotor == PATTERN_SIZE) {
            Rotor = 0;
        }    
    }

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////
//                     preWin8 STORPORT.SYS Hooks                                //


#ifndef _AMD64_
 #pragma comment (linker, "/export:StorPortInitialize=_StorPortInitialize_k8@16")
#else
 #pragma comment (linker, "/export:StorPortInitialize=StorPortInitialize_k8")
#endif

ULONG
StorPortInitialize_k8 (
    PVOID Argument1,
    PVOID Argument2,
    struct _HW_INITIALIZATION_DATA *HwInitializationData,
    PVOID HwContext )
{
    if (!gIsWeInitialized)                  // dump mode, called from miniport
        StorportInit((PDEVICE_OBJECT) Argument1, (PUNICODE_STRING) Argument2); // both args are null

    if (gDumpMode)          // passthrough
        return gStorPortInitialize(
                    Argument1,
                    Argument2,
                    HwInitializationData,
                    HwContext);
    else
        return StorPortInitialize(
                    Argument1,
                    Argument2,
                    HwInitializationData,
                    HwContext);
}




#define _StorPortRegistryRead
#ifndef _AMD64_
 #pragma comment (linker, "/export:StorPortRegistryRead=_StorPortRegistryRead_k8@24")
#else
 #pragma comment (linker, "/export:StorPortRegistryRead=StorPortRegistryRead_k8")
#endif

// StorPortRegistryRead has BUG, it always return TRUE and skip clearing BufferLength even if ValueName was not found
BOOLEAN
StorPortRegistryRead_k8 (
    PVOID HwDeviceExtension,
    PUCHAR ValueName,
    ULONG Global,
    ULONG Type,
    PUCHAR Buffer,
    PULONG BufferLength )
{
    BOOLEAN status;
    ULONG OldBufferLength;

    if (gDumpMode)           // passthrough
        return gStorPortRegistryRead(
                        HwDeviceExtension,
                        ValueName,
                        Global,
                        Type,
                        Buffer,
                        BufferLength );


    OldBufferLength = *BufferLength;
    FillBufferWithPattern(Buffer, *BufferLength);
    status = StorPortRegistryRead (
                        HwDeviceExtension,
                        ValueName,
                        Global,
                        Type,
                        Buffer,
                        BufferLength );

    if (status == TRUE) {
         // Detect bug:
         //  Len    not changed AND
         //  Buffer not changed
        if (*BufferLength == OldBufferLength && isPatternMatch(Buffer, *BufferLength)) {
                status        = FALSE;
                *BufferLength = 0;
        }
    }

    return status;
}


#ifndef _AMD64_
 #pragma comment (linker, "/export:StorPortExtendedFunction=_StorPortExtendedFunction_k8")
#else
 #pragma comment (linker, "/export:StorPortExtendedFunction=StorPortExtendedFunction_k8")
#endif

ULONG __cdecl StorPortExtendedFunction_k8(
    STORPORT_FUNCTION_CODE FunctionCode,
    PVOID HwDeviceExtension,
    ULONG_PTR va1, ULONG_PTR va2, ULONG_PTR va3, ULONG_PTR va4, ULONG_PTR va5, ULONG_PTR va6, ULONG_PTR va7,
    ULONG_PTR va8, ULONG_PTR va9, ULONG_PTR va10, ULONG_PTR va11, ULONG_PTR va12, ULONG_PTR va13)
{

    if (gDumpMode)          // passthrough
        return gStorPortExtendedFunction(FunctionCode, HwDeviceExtension,
                     va1, va2, va3, va4, va5, va6, va7, va8,
                     va9, va10, va11, va12, va13);

    if (FunctionCode <= ExtFunctionFreeContiguousMemorySpecifyCache ||      // storport_w7 has
        FunctionCode == ExtFunctionGetProcessorIndexFromNumber)             // 1-25, 56 Extended Functions
            return StorPortExtendedFunction(FunctionCode, HwDeviceExtension,
                     va1, va2, va3, va4, va5, va6, va7, va8,
                     va9, va10, va11, va12, va13);


///////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionGetActivityId) {
        /*
        ULONG StorPortGetActivityIdSrb(
            PVOID HwDeviceExtension,
            PSCSI_REQUEST_BLOCK Srb,
            LPGUID ActivityId ); 
        */
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionPoFxSetComponentLatency) {
        /*
        ULONG StorPortPoFxSetComponentLatency(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            ULONG Component,
            ULONGLONG Latency );
        */

        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionPoFxSetComponentResidency) {
        /*
        ULONG StorPortPoFxSetComponentResidency(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            ULONG Component,
            ULONGLONG Residency );
        */
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionPoFxPowerControl) {
        /*
        ULONG StorPortPoFxPowerControl(
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            LPCGUID PowerControlCode,
            PVOID InBuffer,
            SIZE_T InBufferSize,
            PVOID OutBuffer,
            SIZE_T OutBufferSize,
            PSIZE_T BytesReturned );
        */
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionSetPowerSettingNotificationGuids) {
        /*
        ULONG StorPortSetPowerSettingNotificationGuids (
            PVOID  HwDeviceExtension,
            ULONG  GuidCount,
            LPGUID Guid );
        */
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionSetUnitAttributes) {
        /*
        ULONG StorPortRequestTimer(
            PVOID HwDeviceExtension,
            PVOID TimerHandle,
            PHW_TIMER_EX TimerCallback,
            PVOID CallbackContext,
            ULONGLONG TimerValue,
            ULONGLONG TolerableDelay );
        */
        return STOR_STATUS_SUCCESS;
    }

    

///////////////////////////////////////////////////////////////////////////////////////
//                             storahci_w8
   
    if (FunctionCode == ExtFunctionFreeTimer) {
        /*
        ULONG StorPortFreeTimer(
            PVOID HwDeviceExtension,
            PVOID TimerHandle );
        */
        STOR_TIMER_CONTEXT      *TimerHandle = (STOR_TIMER_CONTEXT *)   va1;
        RAID_ADAPTER_EXTENSION  *Adapter;
        IO_WORKITEM             *WorkItem;

        if (HwDeviceExtension == NULL || TimerHandle == NULL)
            return STOR_STATUS_INVALID_PARAMETER;

        if (KeGetCurrentIrql() > DISPATCH_LEVEL)
            return STOR_STATUS_INVALID_IRQL;

        Adapter = GetAdapter(HwDeviceExtension);
        InterlockedExchangePointer((PVOID *)&TimerHandle->Callback, NULL);

        if (KeCancelTimer(&TimerHandle->Timer) ==  FALSE) { // DPC is live
            KeRemoveQueueDpc(&TimerHandle->TimerDpc);

            if (KeGetCurrentIrql() > PASSIVE_LEVEL) {
                WorkItem = IoAllocateWorkItem(Adapter->DeviceObject);
                TimerHandle->FreeTimerWorkItem = WorkItem;
                if (WorkItem == NULL)
                    return STOR_STATUS_INSUFFICIENT_RESOURCES;
                IoQueueWorkItem(WorkItem, (PIO_WORKITEM_ROUTINE) StorpFreeTimerWorkItemCallback, DelayedWorkQueue, TimerHandle);
                return STOR_STATUS_SUCCESS;
            }

            KeFlushQueuedDpcs();
         }
         
        ExFreePoolWithTag(TimerHandle, 'TAaR');
        //InterlockedDecrement(&gAllocatedTimers);
    
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionInitializeTimer) {
        /*
        ULONG StorPortInitializeTimer(
            PVOID HwDeviceExtension,
            PVOID *TimerHandle );
        */
        STOR_TIMER_CONTEXT      *Timer;
        RAID_ADAPTER_EXTENSION  *Adapter;
        PVOID                   *TimerHandle = (PVOID *) va1;

        if (HwDeviceExtension == NULL || TimerHandle == NULL)
            return STOR_STATUS_INVALID_PARAMETER;

        if (KeGetCurrentIrql() > DISPATCH_LEVEL)
            return STOR_STATUS_INVALID_IRQL;

        Adapter = GetAdapter(HwDeviceExtension);    
        Timer   = (STOR_TIMER_CONTEXT *) ExAllocatePoolWithTag(NonPagedPool, sizeof(STOR_TIMER_CONTEXT), 'TAaR');
        if (Timer == NULL)
            return STOR_STATUS_INSUFFICIENT_RESOURCES;

        RtlZeroMemory(Timer, sizeof(STOR_TIMER_CONTEXT));
        Timer->Adapter = Adapter;
        KeInitializeTimer(&Timer->Timer);
        KeInitializeDpc(&Timer->TimerDpc, (PKDEFERRED_ROUTINE) StorportTimerDpc, Timer);
        //InterlockedIncrement(&gAllocatedTimers);
        *TimerHandle = Timer;

        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionInvokeAcpiMethod) {
        /*
        ULONG StorPortInvokeAcpiMethod (
            PVOID HwDeviceExtension,
            PSTOR_ADDRESS Address,
            ULONG MethodName,
            PVOID InputBuffer,
            ULONG InputBufferLength,
            PVOID OutputBuffer,
            ULONG OutputBufferLength,
            PULONG BytesReturned  );
        */

        RAID_ADAPTER_EXTENSION  *Adapter;
        RAID_UNIT_EXTENSION     *Unit;
        PDEVICE_OBJECT          DeviceObject;
        STOR_SCSI_ADDRESS       PackedAddress;

        PSTOR_ADDRESS Address       = (PSTOR_ADDRESS) va1;
        PVOID  InputBuffer          = (PVOID) va3;
        ULONG  InputBufferLength    = (ULONG) va4;
        PVOID  OutputBuffer         = (PVOID) va5;
        ULONG  OutputBufferLength   = (ULONG) va6;
        PULONG BytesReturned        = (PULONG) va7;

        if (KeGetCurrentIrql() > PASSIVE_LEVEL)
            return STOR_STATUS_INVALID_IRQL;

        if (BytesReturned != NULL)
                *BytesReturned = 0;

        Adapter = GetAdapter(HwDeviceExtension);
        if (Adapter == 0)
                return STOR_STATUS_INVALID_PARAMETER;

        if (Address != NULL) {
            if (Address->Type != 1 || Address->AddressLength < 4 ) 
                return STOR_STATUS_INVALID_PARAMETER;

            PackedAddress.PathId   = Address->AddressData[0];
            PackedAddress.TargetId = Address->AddressData[1];
            PackedAddress.Lun      = Address->AddressData[2];
            Unit = RaidAdapterFindUnit(Adapter, PackedAddress);
            if (Unit == NULL)
                return STOR_STATUS_INVALID_PARAMETER;

            DeviceObject = Unit->DeviceObject;
        }
        else {  //  Adapter == 0
            DeviceObject = Adapter->DeviceObject;
        }

        if (DeviceObject != NULL)
            return RaidSyncAcpiEvalMethod (
                DeviceObject,
                InputBuffer,
                InputBufferLength,
                OutputBuffer,
                OutputBufferLength,
                BytesReturned);
        else
            return STOR_STATUS_INVALID_PARAMETER;
 
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionDeviceOperationAllowed) {
        /*
        ULONG StorPortIsDeviceOperationAllowed(
            PVOID           HwDeviceExtension,
            PSTOR_ADDRESS   Address,
            LPCGUID         DeviceOperation,
            ULONG           *AllowedFlag );
        */
        static const GUID STORPORT_DEVICEOPERATION_SECURE_REPROVISION_GUID = { 0xdcaf9c10, 0x895f, 0x481f, { 0xa4, 0x92, 0xd4, 0xce, 0xd2, 0xf5, 0x56, 0x33 } };
        PRAID_ADAPTER_EXTENSION     Adapter;
        STOR_SCSI_ADDRESS           PackedAddress;

        PSTOR_ADDRESS   Address         = (PSTOR_ADDRESS) va1;
        LPCGUID         DeviceOperation = (LPCGUID)      va2;
        ULONG           *AllowedFlag    = (ULONG *)      va3;

        if (KeGetCurrentIrql() > PASSIVE_LEVEL)
            return STOR_STATUS_INVALID_IRQL;

        if (AllowedFlag == NULL)
            return STOR_STATUS_INVALID_PARAMETER;

        *AllowedFlag = FALSE;
        Adapter = GetAdapter(HwDeviceExtension);

        if (Address != NULL) 
            if (Address->Type != 1 || Address->AddressLength < 4 ) {
                return STOR_STATUS_INVALID_PARAMETER;

            PackedAddress.PathId   = Address->AddressData[0];
            PackedAddress.TargetId = Address->AddressData[1];
            PackedAddress.Lun      = Address->AddressData[2];
            if (RaidAdapterFindUnit((RAID_ADAPTER_EXTENSION *) HwDeviceExtension, PackedAddress ) == NULL)
                return STOR_STATUS_INVALID_PARAMETER;
            }

        if (!RtlEqualMemory(DeviceOperation,
                   &STORPORT_DEVICEOPERATION_SECURE_REPROVISION_GUID,
                   sizeof(GUID)))
                        return STOR_STATUS_NOT_IMPLEMENTED;

        if (RtlCheckRegistryKey(RTL_REGISTRY_CONTROL, L"MiniNT") == STATUS_SUCCESS)  // WinPE
                *AllowedFlag = TRUE;

        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionQueryPerformanceCounter) {
        /*
        ULONG StorPortQueryPerformanceCounter(
            PVOID HwDeviceExtension,
            PLARGE_INTEGER PerformanceFrequency,
            PLARGE_INTEGER PerformanceCounter );
        */
        PLARGE_INTEGER PerformanceFrequency = (PLARGE_INTEGER) va1;
        PLARGE_INTEGER PerformanceCounter   = (PLARGE_INTEGER) va2;

        if (PerformanceCounter == NULL)
            return STOR_STATUS_INVALID_PARAMETER;

        if (PerformanceFrequency != NULL) {
           PerformanceFrequency->LowPart  = 1;
           PerformanceFrequency->HighPart = 0;
        }

       *PerformanceCounter = KeQueryPerformanceCounter(PerformanceFrequency);
        return STOR_STATUS_SUCCESS;
    }

///////////////////////////////////////////////////////////////////////////////////////
    if (FunctionCode == ExtFunctionRequestTimer) {
        /*
        ULONG StorPortRequestTimer(
            PVOID HwDeviceExtension,
            PVOID TimerHandle,
            PHW_TIMER_EX TimerCallback,
            PVOID CallbackContext,
            ULONGLONG TimerValue,
            ULONGLONG TolerableDelay );
        */
        RAID_ADAPTER_EXTENSION  *Adapter;
        ULONG                    TolerDelay;
        LARGE_INTEGER            DueTime;


        STOR_TIMER_CONTEXT  *TimerHandle    = (STOR_TIMER_CONTEXT  *)             va1;
        PHW_TIMER_EX        TimerCallback   = (PHW_TIMER_EX)                      va2;
        PVOID               CallbackContext = (PVOID)                             va3;

        #ifdef _AMD64_
            ULONGLONG       TimerValue      = (ULONGLONG)va4; 
            ULONGLONG       TolerableDelay  = (ULONGLONG)va5; 
        #else
            ULONGLONG       TimerValue      = ((ULONGLONG)va5 << 32) + (ULONGLONG)va4; 
            ULONGLONG       TolerableDelay  = ((ULONGLONG)va7 << 32) + (ULONGLONG)va6; 
        #endif

        if (!HwDeviceExtension || !TimerHandle || !TimerCallback )
            return STOR_STATUS_INVALID_PARAMETER;

        Adapter = GetAdapter(HwDeviceExtension);

        if (TimerValue == 0) {
            if (TimerHandle ->Adapter != Adapter)
                return STOR_STATUS_INVALID_PARAMETER;

            if (TimerHandle->Callback != TimerCallback)
                return STOR_STATUS_INVALID_PARAMETER;

            InterlockedExchangePointer((PVOID *)&TimerHandle->Callback, NULL);
            KeCancelTimer(&TimerHandle->Timer);
            return STOR_STATUS_SUCCESS;
        }

        else {                                              // TimerValue > 0
            if (InterlockedCompareExchangePointer((PVOID *)&TimerHandle->Callback, TimerCallback, 0))
                    return STOR_STATUS_BUSY;

            TimerHandle->CallbackContext= CallbackContext;

            DueTime.HighPart = (-10 * TimerValue) >> 32;
            DueTime.LowPart = -10 * (ULONG) TimerValue;

            TolerDelay = (ULONG) (TolerableDelay / 1000);
            if (TolerableDelay / 1000 >= 32) {
                if (TolerableDelay != 1000 * (TolerableDelay / 1000))
                    ++TolerDelay;
            }
            else
                TolerDelay = 0;

            if (KeGetCurrentIrql() > DISPATCH_LEVEL) {
                // TODO:  
                //return RequestAddtionalTimerDeferred(Adapter, TimerHandle, TolerDelay, DueTime);
                return STOR_STATUS_INSUFFICIENT_RESOURCES;
            }
            else {
                KeSetCoalescableTimer(
                                &TimerHandle->Timer,
                                DueTime,
                                0,
                                TolerDelay,
                                &TimerHandle->TimerDpc);
                return STOR_STATUS_SUCCESS;
            }
        }    

        return STOR_STATUS_SUCCESS;
    }


    return STOR_STATUS_NOT_IMPLEMENTED;     // default answer to all unsupported functions
    
}    


NTSTATUS
DriverEntry (                // Dummy entry
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
    
    StorportInit((PDEVICE_OBJECT) -1, RegistryPath);

    return STATUS_SUCCESS;
}


NTSTATUS
DllUnload (void)
{
    return STATUS_SUCCESS;
}


#ifdef __cplusplus
}
#endif
