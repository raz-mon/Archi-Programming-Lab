%macro create_new_link 0
    push ebx
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
    pop ebx
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
    ; Check that stackSize > stackCounter (there is more room for more numbers).
    mov ebx, dword[stackCounter]
    cmp ebx, dword[stackSize]
    jnl %%err
    ; Push number (linked-list) to operand stack.
    mov eax, dword[first_link]        ; eax holds the curent link address
    mov esi, dword[stackCounter]
    mov dword[operand_stack + esi*4], eax ; Next free spot gets the new link address.
    inc dword[stackCounter]         ; Increment the stack counter.
    mov dword[lastInStack], eax     ; Update the "last in stack" pointer.
    jmp %%end
%%err:
    push overFlow_err
    push PrePrintString
    push dword [stderr]
    call fprintf
    add esp, 12
    
    jmp loop
%%end:
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
    jz %%end
    mov %1, dword[%1]
    jmp %%printLinkLoop
%%end:
    popad
%endmacro

%macro fgets_ass 0
    push dword [stdin]              ;path to file(stdin)
    push dword 81                   ;max lenght
    push dword buffer               ;input buffer
    call fgets
    add esp, 12                    
%endmacro

%macro getStackSize 1
    mov eax, dword[%1+8]        ; eax = argv
    mov eax, dword[eax+4]       ; eax = argv[1].
    cmp eax, 0
    jnz %%nz
    mov dword[stackSize], 5    ; Default size of the operand stack.
    jmp %%end
%%nz:    
    cmp byte[eax+1], 0
    mov edx, 0                  ; We later use this in both cases.
    jz %%one_num                ; The input number is one digit.
    mov dl, byte[eax]         ; The msb of the input will be assigned to dl.
    sub dl, 48                  ; dl will hold the binary representation of the msb.
    ;mul dl, 7                   ; Multiply by 7, since the input is in base 7.
    shl dl, 3
    mov ebx, 0
    mov bl, byte[eax+1]
    sub bl, 48
    add dl, bl                  ; Add the lsb to number.
    mov dword[stackSize], edx
    jmp %%end
%%one_num:
    mov dl, byte[eax]
    sub dl, 48
    mov dword[stackSize], edx
%%end:
    push dword[stackSize]
    push stackSizeSTR
    call printf
    add esp, 8

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
    calc_str: db "calc:",0  
    current_link_ptr: dd 0 
    lastInStack: dd 0                                           ; This is the "esp" of our operand_stack
    first_link: dd 0   
    print_next_link_message: db "printing next link", 10, 0
    stackCounter: dd 0              ; Will hold the amount of used cells at the operand-stack.
    stackSize: dd 0    
    overFlow_err: db "Error: Operand Stack Overflow", 10, 0
    emptyStack_err: db "Error: Insufficient Number of Arguments on Stack", 10, 0                             
    addition_fault_str: db "Fault: less then 2 numbers in stack", 10, 0
    bad_instruction_string: db "bad instruction. Try again pal.", 10, 0
    stackSizeSTR: db "stack size is: %d", 10, 0
    preValueSTR: db "value of poped link: %oq", 10, 0

section .rodata             ; read-only data.

section .text               ; text
main:
    ;Get stack size and assign it to stackSize.
    getStackSize esp            ; stackSize will hold the stack size after this.
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
    cmp byte[buffer], 'q'
    jz end_myCalc
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

    cmp esi, 0                          ; If input is of size 0 -> get another input, this isn't valid.
    jz loop

    dec esi                             ; So buffer + esi will pointed to the wanted value.
    mov bl, byte [buffer + esi]

    cmp bl, 57        
    jg lexical_commands             ; What exactly are the "lexical commands"?
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
    cmp dl, 0
    jz loop
    create_new_link
    update_linkedlist  

    ; printOperandStack


    jmp loop

mathematical_commands:  
    cmp bl, '+'
    je addition
    cmp bl, '&'
    jz AND_op
    push bad_instruction_string         ; Bad instruction
    call printf
    add esp, 4
    jmp loop

lexical_commands:
    cmp bl, 'p'
    jz pop_and_print
    cmp bl, 'd'
    jz duplicate
    cmp bl, 'n'
    jz num_of_bytes
    push bad_instruction_string         ; Bad instruction
    call printf
    add esp, 4
    jmp loop

pop_and_print:          ; Maybe add a pointer to the representation of the input number that made the link??
    pushad

    cmp dword[stackCounter], 0
    jnz cont_p
    push emptyStack_err
    call printf
    add esp, 4
    popad
    jmp loop
cont_p:
    mov eax, dword[lastInStack]
    mov edx, 0
    mov dl, byte[eax]
    push edx
    push preValueSTR
    call printf
    add esp, 8
    dec dword[stackCounter]
; check if there is another linked-list in the operand stack to assign to last_in_stack
    cmp dword[stackCounter], 0
    jnz cont_p2
    mov dword[lastInStack], 0
    popad
    jmp loop

cont_p2:
    mov esi, dword[stackCounter]
    dec esi
    mov esi, dword[operand_stack+4*esi]
    mov dword[lastInStack], esi
    popad
    jmp loop

; Make sure with Doron that it's fine that we have that extra q after the number (from the printf)!!!!
duplicate:
    pushad
    
    popad

num_of_bytes:

AND_op:


addition:
    pushad
    mov esi, [stackCounter]
    cmp esi, 2
    jl addition_fault
    dec esi
    mov ebx, [operand_stack + (esi - 1) * 4 ]      ; eax point to one linklist
    mov ecx, [operand_stack + esi * 4]          ; ebx point to second linklist

    mov dword [operand_stack + (esi - 1) * 4], 0
    mov dword [operand_stack + esi * 4], 0

    dec esi
    mov dword [stackCounter], esi

    mov esi, ebx
    mov edi, ecx
    
    mov edx, 0
    loop_addition:
        adc dl, byte [ebx]
        adc dl, byte [ecx]
        pushfd
        create_new_link
        update_linkedlist  

        cmp dword [ebx + 1], 0
        je first_num_end
        cmp dword [ecx + 1], 0
        je second_num_end

        both_not_end:
        mov ecx, dword [ecx + 1]
        mov ebx, dword [ebx + 1]
        mov edx, 0
        popfd
        jmp loop_addition

        first_num_end:
            cmp dword [ecx + 1], 0
            je addition_end
            mov ecx, dword [ecx + 1]
            mov edx, 0
            popfd
            adc dl, byte [ecx]
            pushfd
            create_new_link
            update_linkedlist
            jmp first_num_end               

        second_num_end:
            cmp dword [ebx + 1], 0
            je addition_end
            mov ebx, dword [ebx + 1]
            mov edx, 0
            popfd
            adc dl, byte [ebx]
            pushfd
            create_new_link
            update_linkedlist
            jmp second_num_end 

        addition_end:
            popfd
            ;clean_link_list esi
            ;clean_link_list edi
            popad

            printOperandStack

            jmp loop

    addition_fault:
        pushad
        push addition_fault_str
        call printf
        add esp, 4
        popad

    popad
    jmp loop

; End of function myCalc();
end_myCalc:
    mov esp, ebp
    pop ebp
    ret

end_program:
    mov ebx, 0
    mov eax, 1
    int 0x80