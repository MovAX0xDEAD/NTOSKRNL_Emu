#undef    __STDC_WANT_SECURE_LIB__
#define   __STDC_WANT_SECURE_LIB__ 0

#include <ntddk.h> 
#include <ntimage.h>
#include "common.h"
#include "wrk2003.h"

#define NtQuery_BUFFERSIZE (100 * 1024) // 100KB/sizeof(RTL_PROCESS_MODULE_INFORMATION) 100*1024/284 = 360 max records

uintptr_t __security_cookie = 0xDEADBEEF;

const LARGE_INTEGER MmShortTime = {(ULONG)(-10 * 1000 * 10), -1}; // 10 milliseconds

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
    HANDLE Section;                 // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;


typedef struct _RTL_PROCESS_MODULES {
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;


typedef struct _KLDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    // ULONG padding on IA64
    PVOID GpValue;
    PNON_PAGED_DEBUG_INFO NonPagedDebugInfo;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT __Unused5;
    PVOID SectionPointer;
    ULONG CheckSum;
    // ULONG padding on IA64
    PVOID LoadedImports;
    PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;


#if defined(_X86_)
void __fastcall
#else
void __cdecl
#endif
__security_check_cookie (uintptr_t _StackCookie)
{
    ;
}


BOOLEAN
KeSetCoalescableTimer_k8 (
    KTIMER         *Timer,
    LARGE_INTEGER   DueTime,
    ULONG           Period,
    ULONG           TolerableDelay,
    KDPC           *Dpc)
{
   return KeSetTimerEx(
            Timer,
            DueTime,
            Period,
            Dpc );     
}


PVOID
GetRoutineAddress (
    PUNICODE_STRING SystemRoutineName,
    const PCHAR Modulename)
{
    NTSTATUS                Status;
    ANSI_STRING             AnsiString;
    PVOID                   FunctionAddress;
    LOGICAL                 Found;
    ULONG                   ReturnLength;
    PRTL_PROCESS_MODULES    ModuleInformation;
    PRTL_PROCESS_MODULE_INFORMATION ModuleInfo;
    ULONG                   Index;
    CHAR                   *FileName;
    ULONG_PTR               OldUserProbeAddress;
    ULONG_PTR              *pUserProbeAddress;

    do {
        Status = RtlUnicodeStringToAnsiString(&AnsiString,
                                              SystemRoutineName,
                                              TRUE);
        if (NT_SUCCESS(Status))
        {
            break;
        }

        KeDelayExecutionThread(KernelMode, FALSE, (PLARGE_INTEGER)&MmShortTime);

    } while (TRUE);

    if (KeGetCurrentIrql() >= DISPATCH_LEVEL)
        ModuleInformation = ExAllocatePoolWithTag(NonPagedPool, NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');
    else    
        ModuleInformation = ExAllocatePoolWithTag(PagedPool,    NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');

    RtlZeroMemory(ModuleInformation, NtQuery_BUFFERSIZE * sizeof(UCHAR));

    pUserProbeAddress = &MmUserProbeAddress;
    OldUserProbeAddress = *pUserProbeAddress;
    *pUserProbeAddress  = (ULONG_PTR)-1; // hack to allow NtQuerySystemInformation write
                                         // to Buffer allocated in kernel space > 0x7FFF0000
    Status = NtQuerySystemInformation(  SystemModuleInformation,
                                        ModuleInformation,
                                        NtQuery_BUFFERSIZE * sizeof(UCHAR),
                                        &ReturnLength );
    *pUserProbeAddress  = OldUserProbeAddress;

    if (Status != STATUS_SUCCESS) {
        RtlFreeAnsiString (&AnsiString);
        ExFreePoolWithTag(ModuleInformation, 'pmuD');
        return NULL;
    }

    Found = FALSE;
    for (Index = 0; Index < ModuleInformation->NumberOfModules; Index++)
    {
        ModuleInfo = &ModuleInformation->Modules[Index];
        FileName = ModuleInfo->FullPathName + ModuleInfo->OffsetToFileName;

            if (strcmp(FileName, Modulename) == 0)
            {
                Found = TRUE;
                break;
            }
    }
    
    if (Found == TRUE)
        FunctionAddress = MiFindExportedRoutineByName( ModuleInfo->ImageBase,
                                                       &AnsiString );
    else
        FunctionAddress = NULL;

    RtlFreeAnsiString (&AnsiString);
    ExFreePoolWithTag(ModuleInformation, 'pmuD');
    return FunctionAddress;
}


PVOID
GetModuleBaseAddress (
    const PCHAR Modulename)
{
    NTSTATUS                Status;
    PVOID                   FunctionAddress;
    LOGICAL                 Found;
    ULONG                   ReturnLength;
    PRTL_PROCESS_MODULES    ModuleInformation;
    PRTL_PROCESS_MODULE_INFORMATION ModuleInfo;
    ULONG                   Index;
    CHAR                   *FileName;
    ULONG_PTR               OldUserProbeAddress;
    ULONG_PTR              *pUserProbeAddress;

    if (KeGetCurrentIrql() >= DISPATCH_LEVEL)
        ModuleInformation = ExAllocatePoolWithTag(NonPagedPool, NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');
    else    
        ModuleInformation = ExAllocatePoolWithTag(PagedPool,    NtQuery_BUFFERSIZE * sizeof(UCHAR), 'pmuD');

    RtlZeroMemory(ModuleInformation, NtQuery_BUFFERSIZE * sizeof(UCHAR));

    pUserProbeAddress = &MmUserProbeAddress;
    OldUserProbeAddress = *pUserProbeAddress;
    *pUserProbeAddress  = (ULONG_PTR)-1; // hack to allow NtQuerySystemInformation write
                                         // to Buffer allocated in kernel space > 0x7FFF0000
    Status = NtQuerySystemInformation(  SystemModuleInformation,
                                        ModuleInformation,
                                        NtQuery_BUFFERSIZE * sizeof(UCHAR),
                                        &ReturnLength );
    *pUserProbeAddress  = OldUserProbeAddress;

    if (Status != STATUS_SUCCESS) {
        ExFreePoolWithTag(ModuleInformation, 'pmuD');
        return NULL;
    }

    Found = FALSE;
    for (Index = 0; Index < ModuleInformation->NumberOfModules; Index++)
    {
        ModuleInfo = &ModuleInformation->Modules[Index];
        FileName = ModuleInfo->FullPathName + ModuleInfo->OffsetToFileName;

            if (strcmp(FileName, Modulename) == 0)
            {
                Found = TRUE;
                break;
            }
    }
    
    if (Found == TRUE)
        FunctionAddress = ModuleInfo->ImageBase;
    else
        FunctionAddress = NULL;

    ExFreePoolWithTag(ModuleInformation, 'pmuD');
    return FunctionAddress;
}
