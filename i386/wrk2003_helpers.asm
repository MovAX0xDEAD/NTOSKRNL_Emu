    .686P
    .model  flat

EXTRN   _ZwOpenFile@24:PROC

PUBLIC  _NtQueryVirtualMemory_0xB2
PUBLIC  _NtQueryVirtualMemory_0xBA
PUBLIC  _KeGetCurrentStackPointer@0 

;PUBLIC  _KeAcquireQueuedSpinLockAtDpcLevel@4
;PUBLIC  _KeReleaseInStackQueuedSpinLockFromDpcLevel@4


_TEXT   SEGMENT

_NtQueryVirtualMemory_Xxx PROC  ; virtualbox

_NtQueryVirtualMemory_0xB2:
   mov     eax, 0B2h            ; XP SP1/2/3
   jmp     NtQueryVirtualMemory_Jump 

_NtQueryVirtualMemory_0xBA:
   mov     eax, 0BAh            ; W2003 R2
   jmp     NtQueryVirtualMemory_Jump 

NtQueryVirtualMemory_Jump:
   mov     edx, _ZwOpenFile@24
   lea     edx, [edx + 5]
   jmp     edx
_NtQueryVirtualMemory_Xxx ENDP


;_KeAcquireQueuedSpinLockAtDpcLevel@4 PROC
;	mov     edx, [ecx+4]
;	mov     eax, ecx
;	xchg    eax, [edx]
;	cmp     eax, 0
;	jnz     loc_wait
;	or      edx, 2
;	mov     [ecx+4], edx
;
;loc_ret:
;	ret
;
;loc_wait:
;	or      edx, 1
;	mov     [ecx+4], edx
;	mov     [eax], ecx
;
;loc_loop1:
;	test    dword ptr [ecx+4], 1
;	jz      loc_ret
;	pause
;	jmp     loc_loop1
;_KeAcquireQueuedSpinLockAtDpcLevel@4 ENDP


;_KeReleaseInStackQueuedSpinLockFromDpcLevel@4 PROC
;    mov     eax, ecx
;    mov     edx, [ecx]
;    mov     ecx, [ecx+4]
;    test    edx, edx
;    btr     ecx, 1
;    mov     [eax+4], ecx
;    jnz     loc_rqsl40
;    xor     edx, edx
;    push    eax
;    lock cmpxchg [ecx], edx
;    pop     eax
;    jnz     loc_loop2
;    ret
;
;loc_rqsl40:
;    xor     dword ptr [edx+4], 3
;    mov     dword ptr [eax], 0
;    ret
;
;loc_loop2:
;    mov     edx, [eax]
;    test    edx, edx
;    jnz     loc_rqsl40
;    pause
;    jmp     loc_loop2
;
;_KeReleaseInStackQueuedSpinLockFromDpcLevel@4 ENDP


_KeGetCurrentStackPointer@0 PROC
    lea     eax, [esp+4]
    ret
_KeGetCurrentStackPointer@0 ENDP


_TEXT   ends
        END
