    .686P
    .model  flat

EXTRN   _Create_Process_Notify_Routine_XP@24:PROC
PUBLIC  _CREATE_PROCESS_NOTIFY_ROUTINE_asm

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


_TEXT   ends
        END
