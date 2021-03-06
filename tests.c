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

void
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

#if (NTDDI_VERSION < NTDDI_WIN7)
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


void Routine1 (
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    ;
}
void Routine2 (
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    volatile int a=0x0L;
    
}
void Routine3 (
    PEPROCESS Process,
    HANDLE ProcessId,
    PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    volatile int a=0x1L;
}


typedef struct _test1 {
    /*char *arrayOfCatchableTypes1[1];
    int* arrayOfCatchableTypes2[2];
    const long *arrayOfCatchableTypes3[1];
    __int64* const arrayOfCatchableTypes4[1];
    const __int64* const arrayOfCatchableTypes5[1];*/


    //const char          *Ptr1;      // const char*  Ptr
    //      char* const    Ptr2;      // char* const Ptr
    //const char* const    Ptr3;      // const char* const Ptr
    //const char* const   *Ptr4;      // const char* const *Ptr4
    //volatile const char *Ptr5;
    //char* volatile const Ptr6;
    //volatile const char* volatile const   Ptr7;
    //volatile const char* volatile const  *Ptr8;

    //const char              &Ptr10;
    //char&                   Ptr20;
    //const char&             Ptr30;
    //volatile const char     &Ptr50;
    //volatile const char&     Ptr70;

    //const char           &Ptr11;
    //char* const          &Ptr21;
    //const char* const    &Ptr31;
    //char                *&Ptr40;
    //char*                &Ptr41;
    //char               **&Ptr42;
    //volatile const char  &Ptr51;
    //char* volatile const &Ptr61;
    //volatile const char* volatile const   &Ptr71;

    _ULARGE_INTEGER ff;

} test1_t;


void
RunTests (void)
{
    //test1_t* test1;
    //test1->Ptr1 = NULL;
    //*test1.Ptr1 = '1';

    /*PsSetCreateProcessNotifyRoutineEx_k8 (Routine1, FALSE);
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine2, FALSE);
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine3, FALSE);    
    
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine2, TRUE);    
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine3, TRUE);    
    
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine2, FALSE);
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine3, FALSE);    
    
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine2, TRUE);
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine1, TRUE);    
    PsSetCreateProcessNotifyRoutineEx_k8 (Routine3, TRUE);    */

    /*HANDLE  Handle = NULL;
    void const * Ptr = NULL;
    MEMORY_INFORMATION_CLASS Class = MemoryBasicVlmInformation;
    PVOID Ptr2 = NULL;
    SIZE_T Size = 1;
    PSIZE_T Size2 = &Size;

    ZwQueryVirtualMemory_k8(
        Handle,
        Ptr,
        Class,
        Ptr2,
        Size,
        Size2);*/

}