EXTRN   Create_Process_Notify_Routine_XP:PROC
PUBLIC  CREATE_PROCESS_NOTIFY_ROUTINE_asm

_TEXT   SEGMENT

;VOID
;(*PCREATE_PROCESS_NOTIFY_ROUTINE) (
;    HANDLE   ParentId,
;    HANDLE   ProcessId,
;    BOOLEAN  Create );
CREATE_PROCESS_NOTIFY_ROUTINE_asm PROC
    push    r9              ; backup original r9

    push    [rsp+80h+2*8]   ; Thread (rsp+80h on original stack) _PspCreateThread
    push    r12             ; ProcessCreate    _PspCreateThread
    mov r9, rsi             ; ProcessExit      _PspExitProcess
;   r8                      ; Create
;   rdx                     ; ProcessId
;   rcx                     ; ParentId

    sub     rsp, 32         ; shadow space
    call    Create_Process_Notify_Routine_XP
    add rsp, 32 + 2*8       ; shadow space + 2*push 

    pop r9
    ret
CREATE_PROCESS_NOTIFY_ROUTINE_asm ENDP


_TEXT   ends
        END
