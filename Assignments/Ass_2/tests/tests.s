%macro create_new_link 0
    pushad
    push pt
    call printf
    add esp, 4
    popad
    
    push ecx
    ;mov word[temp], dx
    push edx
    push dword 5
    call malloc
    add esp, 4
    pop edx
    ;mov dx, word[temp]
    pop ecx
%endmacro

%macro update_linkedlist 0
    mov byte [eax], dl                              ; insert dl byte in the first byte in new link   
    cmp dword[current_link_ptr], 0
    jnz %%not_zero
    mov dword [current_link_ptr], eax
    inc dword [current_link_ptr]
    jmp %%end

%%not_zero:
    mov esi, dword [current_link_ptr]               ; esi is now a ptr to the latest updated link (with offset 1)
    mov dword [esi], eax                            ; old link point to the begginig of new link
    inc eax                                         ; old link point to the second byte in new link 
    mov dword [current_link_ptr], eax               ; current_link_ptr point to second byte in new link
%%end:
%endmacro

section .text               ; text.
  align 16
  global main
  extern printf
  extern fprintf 
  extern fflush
  extern malloc 
  extern calloc 
  extern free 
  ;extern gets 
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr

section .bss                ; uninitialized data.

section .data               ; initialized data.
    PrePrintNum: db "number is: %0x", 10, 0
    PrePrintString: db "%s", 10, 0
    calc_str: db "calc:",0
    current_link_ptr: dd 0
    pt: db "creating new link",10,0
    

section .rodata             ; read-only data.


section .text               ; text
main:
    ; argc in [esp].
    ; 
    ;mov esi, [esp]
    ;cmp esi, 1                 ; If one -> no input from user.
    ;mov eax, [esp+4]           ; [esp+4] points to argv 

    push calc_str
    push PrePrintString
    call printf
    add esp, 8

    call mycalc

    jmp end_program


mycalc:
    push ebp
    mov ebp, esp

    mov ebx, 0                   ; Initialize bx with 0.
    mov eax, 0                   ; Initialize ax with 0.
    mov edx, 0                   ; Initialize dx with 0.
    mov cx,  0                    ; cx will be the index counter of ax.(It is the only one that shl works with..).
    mov ecx, 0

section .bss
    pointer: resb 4              ; Will be used to point at the input string at different
section .data               ; Initialized data.
    input_string: db "46512345",0              
    temp: dw 0
    counter: dd 0
section .text
    
mov dl, byte [input_string]
sub dl, 48
mov dl, 0
                                                                        ; 100110101
mov dword[pointer], input_string
add dword[pointer], 7
mov dword[counter], 7

bit_loop:
    mov esi, dword [pointer]
    mov bl, byte [esi]         ; bl points to the current character in the input.
    sub bl, 48                 ; get number-value of the input char (binary representation).
    shl bx, cl                 ; Put the bits in the right place before adding to ax.
    add cl, 3
    add dx, bx                 ; Add bits to the representation.

    dec dword [pointer]

    cmp cl, 8
    jnl make_link

cont:
    cmp dword [counter], 0
    jz end_loop                ; jmp from loop when counter = 0.
    dec dword [counter]
    jmp bit_loop

    ;cmp cl, 8                  ; Check if we have 8 bits already.
    ;jpl bit_loop                ; If cl<8 -> do the loop again.

    ;pushad
    ;push dx
    ;push PrePrintNum
    ;call printf
    ;add esp, 6
    ;popad

make_link:
    create_new_link
    update_linkedlist

    ; Print current link
    pushad
    mov esi, dword [current_link_ptr]
    dec esi
    mov edx, 0
    mov dl, byte[esi] 
    push dx
    push PrePrintNum
    call printf
    add esp, 6
    popad

break1:

    mov dl, dh
    mov dh, 0               ; Needs to be.
    cmp cl, 8
    jnz next1
    mov cl, 0
    jmp cont
    
next1:
    cmp cl, 9
    jnz next2
    mov cl, 1
    jmp cont

next2:
    mov cl, 2
    jmp cont



end_loop:                               ; We arrive here after reading all the input number.
    create_new_link
    update_linkedlist

    ; Print current link
    pushad
    mov esi, dword [current_link_ptr]
    dec esi
    mov edx, 0
    mov dl, byte[esi] 
    push dx
    push PrePrintNum
    call printf
    add esp, 6
    popad


    mov esp, ebp
    pop ebp
    ret




end_program:
    mov ebx, 0
    mov eax, 1
    int 0x80















































