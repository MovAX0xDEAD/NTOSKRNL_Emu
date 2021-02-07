    .686P
    .model  flat

EXTRN   __imp__ZwOpenFile@24:PROC

PUBLIC  _NtQueryVirtualMemory_0xB2
PUBLIC  _NtQueryVirtualMemory_0xBA
PUBLIC  _KeGetCurrentStackPointer@0 

;PUBLIC  _KeAcquireQueuedSpinLockAtDpcLevel@4
;PUBLIC  _KeReleaseInStackQueuedSpinLockFromDpcLevel@4


_TEXT   SEGMENT

_NtQueryVirtualMemory_0xB2:
   mov     eax, 0B2h            ; XP SP1/2/3
   jmp     NtQueryVirtualMemory_Jump 

_NtQueryVirtualMemory_0xBA:
   mov     eax, 0BAh            ; W2003 R2
   jmp     NtQueryVirtualMemory_Jump 

NtQueryVirtualMemory_Jump:
   mov     edx, dword ptr [__imp__ZwOpenFile@24]
   lea     edx, [edx + 5]
   jmp     edx


_KeGetCurrentStackPointer@0 PROC
    lea     eax, [esp+4]
    ret
_KeGetCurrentStackPointer@0 ENDP


_TEXT   ends
        END
