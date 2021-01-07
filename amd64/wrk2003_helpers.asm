PUBLIC  NtQueryVirtualMemory_0x1F
PUBLIC  NtQueryVirtualMemory_0x20
PUBLIC  KeGetCurrentStackPointer


_DATA   SEGMENT

align   8
public  g_pfnKiServiceLinkage 
g_pfnKiServiceLinkage:
    dq 0

align   8
public  g_pfnKiServiceInternal
g_pfnKiServiceInternal:
     dq 0

_DATA   ends


_TEXT   SEGMENT

NtQueryVirtualMemory_0x1F PROC
        mov     eax, 01Fh
        jmp     NtQueryVirtualMemory_Jump 
NtQueryVirtualMemory_0x1F ENDP

NtQueryVirtualMemory_0x20 PROC
        mov     eax, 020h
        jmp     NtQueryVirtualMemory_Jump 
NtQueryVirtualMemory_0x20 ENDP

NtQueryVirtualMemory_Jump PROC
        cli
        mov     r10, rsp                ; save call frame pointer.
        mov     r11, qword ptr [g_pfnKiServiceLinkage]
        push    0
        push    0
        push    r10                     ; call frame pointer (incoming rsp).
        pushfq
        push    10h
        push    r11                     ; r11 = KiServiceLinkage (ret w/ unwind info)
        jmp     qword ptr [g_pfnKiServiceInternal]
NtQueryVirtualMemory_Jump ENDP


KeGetCurrentStackPointer PROC
    lea     rax, [rsp+8]
    ret
KeGetCurrentStackPointer ENDP


_TEXT   ends
        END
