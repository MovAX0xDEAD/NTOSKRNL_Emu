    .686P
    .model  flat

EXTRN   _gDumpMode:BYTE

EXTRN   _gScsiPortMoveMemory:DWORD
EXTRN   _gScsiPortNotification:DWORD
EXTRN   _gStorPortAllocateRegistryBuffer:DWORD
EXTRN   _gStorPortBusy:DWORD
EXTRN   _gStorPortCompleteRequest:DWORD
EXTRN   _gStorPortConvertUlongToPhysicalAddress:DWORD
EXTRN   _gStorPortDebugPrint:DWORD
EXTRN   _gStorPortDeviceBusy:DWORD
EXTRN   _gStorPortDeviceReady:DWORD
EXTRN   _gStorPortFreeDeviceBase:DWORD
EXTRN   _gStorPortFreeRegistryBuffer:DWORD
EXTRN   _gStorPortGetBusData:DWORD
EXTRN   _gStorPortGetDeviceBase:DWORD
EXTRN   _gStorPortGetLogicalUnit:DWORD
EXTRN   _gStorPortGetPhysicalAddress:DWORD
EXTRN   _gStorPortGetScatterGatherList:DWORD
EXTRN   _gStorPortGetSrb:DWORD
EXTRN   _gStorPortGetUncachedExtension:DWORD
EXTRN   _gStorPortGetVirtualAddress:DWORD
EXTRN   _gStorPortLogError:DWORD
EXTRN   _gStorPortMoveMemory:DWORD
EXTRN   _gStorPortNotification:DWORD
EXTRN   _gStorPortPause:DWORD
EXTRN   _gStorPortPauseDevice:DWORD
EXTRN   _gStorPortQuerySystemTime:DWORD
EXTRN   _gStorPortReadPortBufferUchar:DWORD
EXTRN   _gStorPortReadPortBufferUlong:DWORD
EXTRN   _gStorPortReadPortBufferUshort:DWORD
EXTRN   _gStorPortReadPortUchar:DWORD
EXTRN   _gStorPortReadPortUlong:DWORD
EXTRN   _gStorPortReadPortUshort:DWORD
EXTRN   _gStorPortReadRegisterBufferUchar:DWORD
EXTRN   _gStorPortReadRegisterBufferUlong:DWORD
EXTRN   _gStorPortReadRegisterBufferUshort:DWORD
EXTRN   _gStorPortReadRegisterUchar:DWORD
EXTRN   _gStorPortReadRegisterUlong:DWORD
EXTRN   _gStorPortReadRegisterUshort:DWORD
EXTRN   _gStorPortReady:DWORD
EXTRN   _gStorPortRegistryWrite:DWORD
EXTRN   _gStorPortResume:DWORD
EXTRN   _gStorPortResumeDevice:DWORD
EXTRN   _gStorPortSetBusDataByOffset:DWORD
EXTRN   _gStorPortSetDeviceQueueDepth:DWORD
EXTRN   _gStorPortStallExecution:DWORD
EXTRN   _gStorPortSynchronizeAccess:DWORD
EXTRN   _gStorPortValidateRange:DWORD
EXTRN   _gStorPortWritePortBufferUchar:DWORD
EXTRN   _gStorPortWritePortBufferUlong:DWORD
EXTRN   _gStorPortWritePortBufferUshort:DWORD
EXTRN   _gStorPortWritePortUchar:DWORD
EXTRN   _gStorPortWritePortUlong:DWORD
EXTRN   _gStorPortWritePortUshort:DWORD
EXTRN   _gStorPortWriteRegisterBufferUchar:DWORD
EXTRN   _gStorPortWriteRegisterBufferUlong:DWORD
EXTRN   _gStorPortWriteRegisterBufferUshort:DWORD
EXTRN   _gStorPortWriteRegisterUchar:DWORD
EXTRN   _gStorPortWriteRegisterUlong:DWORD
EXTRN   _gStorPortWriteRegisterUshort:DWORD

EXTRN   __imp__ScsiPortMoveMemory@12:PROC
EXTRN   __imp__ScsiPortNotification:PROC
EXTRN   __imp__StorPortAllocateRegistryBuffer@8:PROC
EXTRN   __imp__StorPortBusy@8:PROC
EXTRN   __imp__StorPortCompleteRequest@20:PROC
EXTRN   __imp__StorPortConvertUlongToPhysicalAddress@4:PROC
EXTRN   __imp__StorPortDebugPrint:PROC
EXTRN   __imp__StorPortDeviceBusy@20:PROC
EXTRN   __imp__StorPortDeviceReady@16:PROC
EXTRN   __imp__StorPortFreeDeviceBase@8:PROC
EXTRN   __imp__StorPortFreeRegistryBuffer@8:PROC
EXTRN   __imp__StorPortGetBusData@24:PROC
EXTRN   __imp__StorPortGetDeviceBase@28:PROC
EXTRN   __imp__StorPortGetLogicalUnit@16:PROC
EXTRN   __imp__StorPortGetPhysicalAddress@16:PROC
EXTRN   __imp__StorPortGetScatterGatherList@8:PROC
EXTRN   __imp__StorPortGetSrb@20:PROC
EXTRN   __imp__StorPortGetUncachedExtension@12:PROC
EXTRN   __imp__StorPortGetVirtualAddress@12:PROC
EXTRN   __imp__StorPortLogError@28:PROC
EXTRN   __imp__StorPortMoveMemory@12:PROC
EXTRN   __imp__StorPortNotification:PROC
EXTRN   __imp__StorPortPause@8:PROC
EXTRN   __imp__StorPortPauseDevice@20:PROC
EXTRN   __imp__StorPortQuerySystemTime@4:PROC
EXTRN   __imp__StorPortReadPortBufferUchar@16:PROC
EXTRN   __imp__StorPortReadPortBufferUlong@16:PROC
EXTRN   __imp__StorPortReadPortBufferUshort@16:PROC
EXTRN   __imp__StorPortReadPortUchar@8:PROC
EXTRN   __imp__StorPortReadPortUlong@8:PROC
EXTRN   __imp__StorPortReadPortUshort@8:PROC
EXTRN   __imp__StorPortReadRegisterBufferUchar@16:PROC
EXTRN   __imp__StorPortReadRegisterBufferUlong@16:PROC
EXTRN   __imp__StorPortReadRegisterBufferUshort@16:PROC
EXTRN   __imp__StorPortReadRegisterUchar@8:PROC
EXTRN   __imp__StorPortReadRegisterUlong@8:PROC
EXTRN   __imp__StorPortReadRegisterUshort@8:PROC
EXTRN   __imp__StorPortReady@4:PROC
EXTRN   __imp__StorPortRegistryWrite@24:PROC
EXTRN   __imp__StorPortResume@4:PROC
EXTRN   __imp__StorPortResumeDevice@16:PROC
EXTRN   __imp__StorPortSetBusDataByOffset@28:PROC
EXTRN   __imp__StorPortSetDeviceQueueDepth@20:PROC
EXTRN   __imp__StorPortStallExecution@4:PROC
EXTRN   __imp__StorPortSynchronizeAccess@12:PROC
EXTRN   __imp__StorPortValidateRange@28:PROC
EXTRN   __imp__StorPortWritePortBufferUchar@16:PROC
EXTRN   __imp__StorPortWritePortBufferUlong@16:PROC
EXTRN   __imp__StorPortWritePortBufferUshort@16:PROC
EXTRN   __imp__StorPortWritePortUchar@12:PROC
EXTRN   __imp__StorPortWritePortUlong@12:PROC
EXTRN   __imp__StorPortWritePortUshort@12:PROC
EXTRN   __imp__StorPortWriteRegisterBufferUchar@16:PROC
EXTRN   __imp__StorPortWriteRegisterBufferUlong@16:PROC
EXTRN   __imp__StorPortWriteRegisterBufferUshort@16:PROC
EXTRN   __imp__StorPortWriteRegisterUchar@12:PROC
EXTRN   __imp__StorPortWriteRegisterUlong@12:PROC
EXTRN   __imp__StorPortWriteRegisterUshort@12:PROC


STUB MACRO ExportName, ImportName, gDiskDumpImportAdress
LOCAL   DiskDump
PUBLIC  ExportName

    ExportName PROC
    
        cmp     [_gDumpMode], 0
        jnz     DiskDump
        jmp     DWORD PTR [ImportName]
        
    DiskDump:
        jmp     DWORD PTR [gDiskDumpImportAdress]
        
    ExportName ENDP

ENDM


_TEXT   SEGMENT

    STUB ScsiPortMoveMemory,            __imp__ScsiPortMoveMemory@12, _gScsiPortMoveMemory
    STUB ScsiPortNotification,          __imp__ScsiPortNotification, _gScsiPortNotification
    STUB StorPortAllocateRegistryBuffer,__imp__StorPortAllocateRegistryBuffer@8, _gStorPortAllocateRegistryBuffer
    STUB StorPortBusy,                  __imp__StorPortBusy@8, _gStorPortBusy
    STUB StorPortCompleteRequest,       __imp__StorPortCompleteRequest@20, _gStorPortCompleteRequest
    STUB StorPortConvertUlongToPhysicalAddress, __imp__StorPortConvertUlongToPhysicalAddress@4, _gStorPortConvertUlongToPhysicalAddress
    STUB StorPortDebugPrint,            __imp__StorPortDebugPrint, _gStorPortDebugPrint
    STUB StorPortDeviceBusy,            __imp__StorPortDeviceBusy@20, _gStorPortDeviceBusy
    STUB StorPortDeviceReady,           __imp__StorPortDeviceReady@16, _gStorPortDeviceReady
    STUB StorPortFreeDeviceBase,        __imp__StorPortFreeDeviceBase@8, _gStorPortFreeDeviceBase
    STUB StorPortFreeRegistryBuffer,    __imp__StorPortFreeRegistryBuffer@8, _gStorPortFreeRegistryBuffer
    STUB StorPortGetBusData,            __imp__StorPortGetBusData@24, _gStorPortGetBusData
    STUB StorPortGetDeviceBase,         __imp__StorPortGetDeviceBase@28, _gStorPortGetDeviceBase
    STUB StorPortGetLogicalUnit,        __imp__StorPortGetLogicalUnit@16, _gStorPortGetLogicalUnit
    STUB StorPortGetPhysicalAddress,    __imp__StorPortGetPhysicalAddress@16, _gStorPortGetPhysicalAddress
    STUB StorPortGetScatterGatherList,  __imp__StorPortGetScatterGatherList@8, _gStorPortGetScatterGatherList
    STUB StorPortGetSrb,                __imp__StorPortGetSrb@20, _gStorPortGetSrb
    STUB StorPortGetUncachedExtension,  __imp__StorPortGetUncachedExtension@12, _gStorPortGetUncachedExtension
    STUB StorPortGetVirtualAddress,     __imp__StorPortGetVirtualAddress@12, _gStorPortGetVirtualAddress
    STUB StorPortLogError,              __imp__StorPortLogError@28, _gStorPortLogError
    STUB StorPortMoveMemory,            __imp__StorPortMoveMemory@12, _gStorPortMoveMemory
    STUB StorPortNotification,          __imp__StorPortNotification, _gStorPortNotification
    STUB StorPortPause,                 __imp__StorPortPause@8, _gStorPortPause
    STUB StorPortPauseDevice,           __imp__StorPortPauseDevice@20, _gStorPortPauseDevice
    STUB StorPortQuerySystemTime,       __imp__StorPortQuerySystemTime@4, _gStorPortQuerySystemTime
    STUB StorPortReadPortBufferUchar,   __imp__StorPortReadPortBufferUchar@16, _gStorPortReadPortBufferUchar
    STUB StorPortReadPortBufferUlong,   __imp__StorPortReadPortBufferUlong@16, _gStorPortReadPortBufferUlong
    STUB StorPortReadPortBufferUshort,  __imp__StorPortReadPortBufferUshort@16, _gStorPortReadPortBufferUshort
    STUB StorPortReadPortUchar,         __imp__StorPortReadPortUchar@8, _gStorPortReadPortUchar
    STUB StorPortReadPortUlong,         __imp__StorPortReadPortUlong@8, _gStorPortReadPortUlong
    STUB StorPortReadPortUshort,        __imp__StorPortReadPortUshort@8, _gStorPortReadPortUshort
    STUB StorPortReadRegisterBufferUchar,   __imp__StorPortReadRegisterBufferUchar@16, _gStorPortReadRegisterBufferUchar
    STUB StorPortReadRegisterBufferUlong,   __imp__StorPortReadRegisterBufferUlong@16, _gStorPortReadRegisterBufferUlong
    STUB StorPortReadRegisterBufferUshort,  __imp__StorPortReadRegisterBufferUshort@16, _gStorPortReadRegisterBufferUshort
    STUB StorPortReadRegisterUchar,     __imp__StorPortReadRegisterUchar@8, _gStorPortReadRegisterUchar
    STUB StorPortReadRegisterUlong,     __imp__StorPortReadRegisterUlong@8, _gStorPortReadRegisterUlong
    STUB StorPortReadRegisterUshort,    __imp__StorPortReadRegisterUshort@8, _gStorPortReadRegisterUshort
    STUB StorPortReady,                 __imp__StorPortReady@4, _gStorPortReady
    STUB StorPortRegistryWrite,         __imp__StorPortRegistryWrite@24, _gStorPortRegistryWrite
    STUB StorPortResume,                __imp__StorPortResume@4, _gStorPortResume
    STUB StorPortResumeDevice,          __imp__StorPortResumeDevice@16, _gStorPortResumeDevice
    STUB StorPortSetBusDataByOffset,    __imp__StorPortSetBusDataByOffset@28, _gStorPortSetBusDataByOffset
    STUB StorPortSetDeviceQueueDepth,   __imp__StorPortSetDeviceQueueDepth@20, _gStorPortSetDeviceQueueDepth
    STUB StorPortStallExecution,        __imp__StorPortStallExecution@4, _gStorPortStallExecution
    STUB StorPortSynchronizeAccess,     __imp__StorPortSynchronizeAccess@12, _gStorPortSynchronizeAccess
    STUB StorPortValidateRange,         __imp__StorPortValidateRange@28, _gStorPortValidateRange
    STUB StorPortWritePortBufferUchar,  __imp__StorPortWritePortBufferUchar@16, _gStorPortWritePortBufferUchar
    STUB StorPortWritePortBufferUlong,  __imp__StorPortWritePortBufferUlong@16, _gStorPortWritePortBufferUlong
    STUB StorPortWritePortBufferUshort, __imp__StorPortWritePortBufferUshort@16, _gStorPortWritePortBufferUshort
    STUB StorPortWritePortUchar,        __imp__StorPortWritePortUchar@12, _gStorPortWritePortUchar
    STUB StorPortWritePortUlong,        __imp__StorPortWritePortUlong@12, _gStorPortWritePortUlong
    STUB StorPortWritePortUshort,       __imp__StorPortWritePortUshort@12, _gStorPortWritePortUshort
    STUB StorPortWriteRegisterBufferUchar,  __imp__StorPortWriteRegisterBufferUchar@16, _gStorPortWriteRegisterBufferUchar
    STUB StorPortWriteRegisterBufferUlong,  __imp__StorPortWriteRegisterBufferUlong@16, _gStorPortWriteRegisterBufferUlong
    STUB StorPortWriteRegisterBufferUshort, __imp__StorPortWriteRegisterBufferUshort@16, _gStorPortWriteRegisterBufferUshort
    STUB StorPortWriteRegisterUchar,    __imp__StorPortWriteRegisterUchar@12, _gStorPortWriteRegisterUchar
    STUB StorPortWriteRegisterUlong,    __imp__StorPortWriteRegisterUlong@12, _gStorPortWriteRegisterUlong
    STUB StorPortWriteRegisterUshort,   __imp__StorPortWriteRegisterUshort@12, _gStorPortWriteRegisterUshort

_TEXT   ends
        END
