%macro create_new_link 0
    push ecx
    push edx
    push esi
    push dword 1
    push dword 5
    call calloc
    add esp, 8
    pop esi
    pop edx
    pop ecx
%endmacro

%macro update_linkedlist 0
    mov byte [eax], dl                              ; insert dl byte in the first byte in new link   
    cmp dword[current_link_ptr], 0
    jnz %%not_zero
    mov dword [current_link_ptr], eax
    mov dword[first_link], eax
    pushOperandStack
    inc dword [current_link_ptr]
    jmp %%end

%%not_zero:
    push esi
    mov esi, dword [current_link_ptr]               ; esi is now a ptr to the latest updated link (with offset 1)
    mov dword [esi], eax                            ; old link point to the begginig of new link
    inc eax                                         ; old link point to the second byte in new link 
    mov dword [current_link_ptr], eax               ; current_link_ptr point to second byte in new link
    pop esi
%%end:
%endmacro

%macro pushOperandStack 0
    pushad                  ; Save all register values.
    mov eax, dword[first_link]        ; eax holds the curent link address
    mov esi, dword[stackCounter]
    mov dword[operand_stack + esi*4], eax ; Next free spot gets the new link address.
    inc dword[stackCounter]         ; Increment the stack counter.
    mov dword[lastInStack], eax     ; Update the "last in stack" pointer.
    popad                           ; Restore state of the registers.
%endmacro

%macro printOperandStack 0
    pushad
    mov eax, dword[stackCounter]
%%pri_loop:
    cmp eax, 0
    jz %%finito
    dec eax
    mov ebx, dword[operand_stack + eax*4]
    pushad
    push print_next_link_message
    call printf
    add esp, 4
    popad
    printLink ebx 
    jmp %%pri_loop
%%finito:
    popad
%endmacro

%macro printLink 1
    pushad
%%printLinkLoop:
    mov edx, 0
    mov dl, byte[%1]
    pushad
    push edx
    push PrePrintNum
    call printf
    add esp, 8
    popad
    inc %1
    cmp dword[%1], 0
    jnz %%printLinkLoop
    popad
%endmacro

%macro fgets_ass 0
    push dword [stdin]              ;path to file(stdin)
    push dword 81                   ;max lenght
    push dword buffer               ;input buffer
    call fgets
    add esp, 12                    
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
    buffer: resb 81 ; max size - input line , 80 bytes + 1 byte
    operand_stack: resd 63          ; The program's operand-stack. 63 is it's maximum size.


section .data               ; initialized data.
    PrePrintNum: db "number is: %0x", 10, 0
    PrePrintString: db "%s", 10, 0
    ;preFirstLink: db "First Link is: %0x", 10, 0
    calc_str: db "calc:",0
    current_link_ptr: dd 0 
    lastInStack: dd 0                                           ; This is the "esp" of our operand_stack
    first_link: dd 0   
    print_next_link_message: db "printing next link", 10, 0
    stackCounter: dd 0                                          ; Will hold the amount of used cells at the operand-stack.


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

loop:
    mov dword[current_link_ptr], 0          ; initialize current_link_pointer to 0, so the first link will be recognized.
    fgets_ass                               ; stdio function fgets, put in buffer the wanted data
    mov ebx, 0                              
    mov ecx, 0
    mov esi, 0                              ; counter for byte index
    mov edx, 0
    mov eax, 0
    count_quantity:
        mov bl, byte [buffer + esi]                   
        inc esi
        cmp bl, 10                              ; check if empty(only NL was sent)
        jnz count_quantity
    dec esi

    cmp esi, 0
    jz loop

    dec esi
    mov bl, byte [buffer + esi]

    cmp bl, 57        
    jg lexical_commands
    cmp bl, 48
    jl mathematical_commands

    mov edx, 0

    bit_loop:
        sub bl, 48                 ; get number-value of the input char (binary representation).
        mov bh, 0
        shl bx, cl                 ; Put the bits in the right place before adding to ax.
        add cl, 3
        add dx, bx                 ; Add bits to the representation.

        dec esi

        cmp cl, 8
        jnl make_link

    cont:
        cmp esi, -1
        jz end_loop                ; jmp from loop when counter = 0.
        mov bl, [buffer + esi]
        jmp bit_loop

    make_link:
        create_new_link
    updateLL:
        update_linkedlist
        
        ; Print current link
    after_link:
    ; Print the current link's data.
        ;pushad
        ;mov esi, dword [current_link_ptr]
        ;dec esi
        ;mov edx, 0
        ;mov dl, byte[esi] 
        ;push dx
        ;push PrePrintNum
        ;call printf
        ;add esp, 6
        ;popad

        ;Print last component of the operand stack
       

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



    end_loop:                      ; We arrive here after reading all the input number.
        ;pushOperandStack                ; Push the created link to the operand-stack.


        cmp dl, 0
        jz loop
        create_new_link
        update_linkedlist
        

        ;Print last component of the operand stack
        ;pushad
        ;mov eax, dword[lastInStack]
        ;mov edx, 0
        ;mov dl, byte[eax]
        ;push edx
        ;push PrePrintNum
        ;call printf
        ;add esp, 8
        ;popad

        
    ; Print the current link's data.
        ;pushad
        ;mov esi, dword [current_link_ptr]
        ;dec esi
        ;mov edx, 0
        ;mov dl, byte[esi] 
        ;push dx
        ;push PrePrintNum
        ;call printf
        ;add esp, 6
        ;popad    

        break2:
            pushad
                mov eax, dword[stackCounter]
            pri_loop:
                cmp eax, 0
                jz finito
                dec eax
                mov ebx, dword[operand_stack + eax*4]
                pushad
                push print_next_link_message
                p1:
                call printf
                after_p1:
                add esp, 4
                popad
                    pushad
                    printLinkLoop:
                        mov edx, 0
                        mov dl, byte[ebx]
                        pushad
                        push edx
                        push PrePrintNum
                        p_2:
                        call printf
                        after_p2:
                        add esp, 8
                        popad
                        inc ebx
                        mov ebx, [ebx]
                        cmp dword[ebx + 1], 0
                        jnz printLinkLoop
                        ;Last byte in link list
                        mov edx, 0
                        mov dl, byte[ebx]
                        pushad
                        push edx
                        push PrePrintNum
                        p_3:
                        call printf
                        after_p3:
                        add esp, 8
                        popad
                    popad 
                jmp pri_loop
            finito:
            popad




    jmp loop

mathematical_commands:  

lexical_commands:

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