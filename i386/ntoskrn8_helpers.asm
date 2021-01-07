    .686P
    .model  flat

EXTRN   _Create_Process_Notify_Routine_XP@24:PROC

PUBLIC  _CREATE_PROCESS_NOTIFY_ROUTINE_asm
PUBLIC  _CPUID@20

_TEXT   SEGMENT

;VOID
;(*PCREATE_PROCESS_NOTIFY_ROUTINE) (
;    HANDLE   ParentId,
;    HANDLE   ProcessId,
;    BOOLEAN  Create );
_CREATE_PROCESS_NOTIFY_ROUTINE_asm PROC
;    ESP
; +C    Create
; +8    ProcessId
; +4    ParentId
;  0    return
    push    esi     ; Thread     _PspCreateThread
    push    ebx     ; ProcessCreate  _PspCreateThread
    push    edi     ; ProcessExit    _PspExitProcess

;    ESP
; +18   Create
; +14   ProcessId
; +10   ParentId
;  +C   return
;  +8   esi
;  +4   ebx
;   0   edi
    push    [esp+0Ch + 3*4] ; Create
    push    [esp+08h + 4*4] ; ProcessId
    push    [esp+04h + 5*4] ; ParentId

    call    _Create_Process_Notify_Routine_XP@24
    
    ret 0Ch                 ;ret original 3 args
_CREATE_PROCESS_NOTIFY_ROUTINE_asm ENDP



; VOID
; CPUID (
;     ULONG   InEax,
;     PULONG  OutEax,
;     PULONG  OutEbx,
;     PULONG  OutEcx,
;     PULONG  OutEdx
;     );
_CPUID@20 PROC
    push    ebx
    push    esi

    mov     eax, [esp+12]
    cpuid

    mov     esi, [esp+16]   ; return EAX
    mov     [esi], eax

    mov     esi, [esp+20]   ; return EBX
    mov     [esi], ebx

    mov     esi, [esp+24]   ; return ECX
    mov     [esi], ecx

    mov     esi, [esp+28]   ; return EDX
    mov     [esi], edx

    pop     esi
    pop     ebx

    ret     5*4
_CPUID@20 ENDP

_TEXT   ends
        END
