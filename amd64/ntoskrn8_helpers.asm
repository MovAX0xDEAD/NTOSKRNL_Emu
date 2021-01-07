EXTRN   Create_Process_Notify_Routine_XP:PROC

PUBLIC  CREATE_PROCESS_NOTIFY_ROUTINE_asm
PUBLIC  KiCpuId

CpuEax equ 00000H
CpuEbx equ 00004H
CpuEcx equ 00008H
CpuEdx equ 0000CH

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


; VOID
; KiCpuId (
;     ULONG Function,
;     ULONG Index,
;     PCPU_INFO CpuInfo
;     );
KiCpuId PROC
	push    rbx
	mov     eax, ecx                ; set cpuid function
        mov     ecx, edx                ; set index (only used by function 4)
        cpuid                           ; get cpu information


        mov     CpuEax[r8], eax         ; save cpu information in structure
        mov     CpuEbx[r8], ebx         ;
        mov     CpuEcx[r8], ecx         ;
        mov     CpuEdx[r8], edx         ;
        pop     rbx                     ; restore nonvolatile register
        ret                             ; return

KiCpuId ENDP


_TEXT   ends
        END
