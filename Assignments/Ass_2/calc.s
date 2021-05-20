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
    mov dword [esi], eax                            ; old link points to the begginig of new link
    inc eax                                         ; old link points to the second byte in new link 
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
    ;mov dword[lastInStack], eax     ; Update the "last in stack" pointer.
    jmp %%end
%%err:
    pushad
    push overFlow_err
    push PrePrintString
    push dword [stderr]
    call fprintf
    add esp, 12
    popad

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

%macro freeLinkedList 1           ; The argument to the macro is a pointer to the link we wish to free.
    pushad
    mov ebx, dword %1               ; ebx holds the pointer to the linked-list to free.
    mov eax, dword[%1+1]            ; eax points to the next link to free in the linked-list.
    cmp eax, 0
    jz %%lastToFree

%%free_loop:
    pushad
    push ebx
    call free
    add esp, 4
    popad

    mov ebx, eax
    mov eax, dword[ebx+1]           ; eax points to next link.
    cmp eax, 0
    jz %%lastToFree
    jmp %%free_loop


%%lastToFree:
    pushad
    push ebx
    call free
    add esp, 4
    popad

    popad
%endmacro

%macro printInstructionCounter_oq 0  
    pushad

    mov eax, dword[instructionCounter]      ; eax holds the binary rep. of the amount of instructions.
    mov ebx, 0                               

    ; First two bits of eax
    mov esi, eax
    and esi, 0xC0000000
    shr esi, 30
    cmp esi, 0
    jz %%no_print_1
    pushad
    push esi
    push PrePrintNum_NoEnt
    call printf
    add esp, 8
    popad
%%no_print_1:
    shl eax, 2
    ; From here on, eax holds the 30 remaining bits.

%%loop_l:
    mov esi, eax
    and esi, 0xE0000000                           ; Mask only three lsb bits.
    shr esi, 29
    cmp esi, 0
    jz %%no_print
    pushad
    push esi
    push PrePrintNum_NoEnt
    call printf
    add esp, 8
    popad
%%no_print:
    shl eax, 3
    cmp eax, 0
    jnz %%loop_l
    ; Print newLine.
    pushad
    push newLinestr
    call printf
    add esp,4
    popad

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
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr

section .bss                ; uninitialized data.
    buffer: resb 81                 ; max size - input line , 80 bytes + 1 byte
    operand_stack: resd 63          ; The program's operand-stack. 63 is it's maximum size.
    output_buffer: resb 640

section .data               ; initialized data.
    PrePrintNum: db "%0x", 10, 0
    PrePrintCalc: db "%s", 0
    PrePrintString: db "%s", 10, 0
    calc_str: db "calc: ",0  
    current_link_ptr: dd 0 
    first_link: dd 0   
    print_next_link_message: db "printing next link", 10, 0
    stackCounter: dd 0              ; Will hold the amount of used cells at the operand-stack.
    stackSize: dd 0    
    overFlow_err: db "Error: Operand Stack Overflow", 10, 0
    emptyStack_err: db "Error: Insufficient Number of Arguments on Stack", 0                             
    addition_fault_str: db "Fault: less then 2 numbers in stack", 10, 0
    duplicate_fault_str: db "Fault: no operand in stack", 10, 0 
    bad_instruction_string: db "bad instruction. Try again pal.", 10, 0
    stackSizeSTR: db "stack size is: %d", 10, 0
    preValueSTR: db "value of poped link: %oq", 10, 0
    current_pop_and_print: db "%d", 0
    last_pop_and_print: db "%d" , 10 , 0
    instructionCounter: dd 0
    PrePrintNum_NoEnt: db "%0x",0
    newLinestr: db "", 10, 0

section .rodata             ; read-only data.

section .text               ; text
main:
    ;Get stack size and assign it to stackSize.
    getStackSize esp            ; stackSize will hold the stack size after this.
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
    ; Print "calc:"
    pushad
    push calc_str
    push PrePrintCalc
    call printf
    add esp, 8
    popad

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

;    printOperandStack


    jmp loop

mathematical_commands: 
    inc dword[instructionCounter]
    cmp bl, '+'
    je addition
    cmp bl, '&'
    jz AND_op
    pushad
    push bad_instruction_string         ; Bad instruction
    call printf
    add esp, 4
    popad
    jmp loop

lexical_commands:
    inc dword[instructionCounter]
    cmp bl, 'p'
    jz pop_and_print
    cmp bl, 'd'
    jz duplicate
    cmp bl, 'n'
    jz num_of_bytes
    cmp bl, 't'                           ; This is for us only!! Pops one operand from the operand-stack.
    jz pop
    pushad
    push bad_instruction_string         ; Bad instruction
    call printf
    add esp, 4
    popad
    jmp loop

pop:
    pushad
    cmp dword[stackCounter], 0
    jnz non_zero
    push emptyStack_err
    push PrePrintString
    push dword [stderr]
    call fprintf
    add esp, 12
    popad
    jmp loop
non_zero:
    dec dword[stackCounter]
    mov esi, dword[stackCounter]

    mov eax, dword[operand_stack + esi*4] 
    ;freeLinkedList eax                      ; Free the poped linked-list.
   
    mov dword[operand_stack + esi*4], 0
    popad
    jmp loop

pop_and_print:          ; Maybe add a pointer to the representation of the input number that made the link??
pushad
    mov esi, [stackCounter]
    cmp esi, 1
    jl pop_and_print_fault
    dec esi
    mov ebx, [operand_stack + esi * 4]          ; ebx point to second linklist
    
    mov ebp, [operand_stack + esi * 4]          ; ebp points to the linked-list to delete and free.

    mov dword [operand_stack + esi * 4], 0
    mov dword [stackCounter], esi

    mov eax, 0
    mov esi, ebx
    count_linklist_size:
        inc eax
        cmp dword [esi + 1] , 0
        je initialize_data
        mov esi, dword [esi + 1]
        jmp count_linklist_size

    initialize_data:
    mov esi, ebx
    mov edx, 3
    mul edx
    mov byte[output_buffer + eax] , 10
    dec eax
    mov edi, 0
    mov edx, 0
    mov ecx, 0
    loop_pop_and_print:
        mov dl, byte [ebx]
        
        print_loop:
            cmp edi, 6
            jge pop_and_print_greater

            mov byte[output_buffer + eax], 7
            ;mov esi, 7
            and byte[output_buffer + eax], dl
            ;and esi , edx
            add byte[output_buffer + eax], 48
            ;add esi , 48
            dec eax

            shr edx, 3
            add edi, 3
            jmp print_loop

        pop_and_print_greater:
            cmp edi, 6
            je pop_and_print_greater_6
            cmp edi, 7
            je pop_and_print_greater_7
            cmp edi, 8
            je pop_and_print_greater_8

            pop_and_print_greater_6:
                cmp dword [ebx + 1], 0
                je print_and_done
                mov ebx, [ebx + 1]
                mov ecx, 0
                mov cl, byte [ebx]
                shl ecx, 2
                add edx, ecx
                mov edi, -2
                jmp print_loop

            pop_and_print_greater_7:
                cmp dword [ebx + 1], 0
                je print_and_done
                mov ebx, [ebx + 1]
                mov ecx, 0
                mov cl, byte [ebx]
                shl ecx, 1
                add edx, ecx
                mov edi, -1
                jmp print_loop

            pop_and_print_greater_8:
                cmp dword [ebx + 1], 0
                je print_and_done
                mov ebx, [ebx + 1]
                mov edi, 0
                jmp loop_pop_and_print

        print_and_done:
            mov byte[output_buffer + eax], 3
            and byte[output_buffer + eax], dl
            add byte[output_buffer + eax], 48
            
            mov ebx, output_buffer
            delete_nullchar_loop:
                cmp byte [ebx] , 0
                jne delete_zeros_loop
                add ebx , 1
                jmp delete_nullchar_loop

            delete_zeros_loop:
                cmp byte [ebx] , 48
                jne pop_and_print_end
                add ebx , 1
                jmp delete_zeros_loop

        pop_and_print_end:
            pushad
            push ebx
            call printf
            add esp, 4
            popad
            ;freeLinkedList esi

        mov ecx, 640
        reset_output_buffer:
            cmp ecx, 0
            je loop
            mov byte [output_buffer + ecx - 1] , 0
            dec ecx
            jmp reset_output_buffer   

        popad
        jmp loop

    pop_and_print_fault:
        pushad
        push emptyStack_err
        push PrePrintString
        push dword [stderr]
        call fprintf
        add esp, 12
        popad

    ;freeLinkedList ebp
    popad
    jmp loop

duplicate:
    pushad
    mov esi, [stackCounter]
    cmp esi, 1
    jl duplicate_fault
    dec esi
    mov ebx, [operand_stack + esi * 4]          ; ebx point to second linklist

    ;mov dword [operand_stack + esi * 4], 0     ; Why?? We don't need to pop anything..
    mov edx, 0
    loop_duplicate:
        mov dl, byte [ebx]
        create_new_link
        update_linkedlist  

        cmp dword [ebx + 1], 0
        je duplicate_end

        mov ebx, [ebx + 1]
        jmp loop_duplicate
        duplicate_end:
            ;printOperandStack
            popad
            jmp loop

    duplicate_fault:
        pushad
        push emptyStack_err
        push PrePrintString
        push dword [stderr]
        call fprintf
        add esp, 12
        popad

    popad
    jmp loop



num_of_bytes:
    pushad                          ; Save registers state
    ; If stackCounter = 0 --> error to stderr.
    cmp dword[stackCounter], 0
    jnz not_zero1
    push emptyStack_err
    push PrePrintString
    push dword [stderr]
    call fprintf
    add esp, 12
    popad
    jmp loop
    ; stackCounter > 0   
not_zero1:
    mov esi, dword[stackCounter]    ; esi holds the amount of components in the operand-stack.
    mov edi, dword[operand_stack + (esi-1)*4]   ; edi holds pointer to the last linked-list in the operand-stack.
    
    mov ebp, dword[operand_stack + (esi-1)*4]   ; ebp will be used to free the popped linked-list.
    
    mov edx, 0                      ; edx will be our counter.
count_loop:
    inc edx                         ; Increment counter.
    cmp dword[edi+1], 0             ; Check if the linked-list has another link.
    jz end_count_loop
    mov edi, dword[edi + 1]           ; esi is assigned the next link in the linked-list.
    jmp count_loop
end_count_loop:    
    ; malloc 5 spaces (link), and assign the data the counter eax. No next link
    push esi                        ; Save the value of stackCounter.
    push edx                        ; Save the value of the counter.
    push dword 1
    push dword 5
    call calloc
    add esp, 8
    pop edx                         ; Retrieve the counter's value from before the calloc
    pop esi                         ; esi = stackCounter's value.
    mov byte[eax], dl               ; Assign the counter's value to the data of the link we are going to push to the operand-stack.
    mov dword[operand_stack + (esi-1)*4], eax   ; Insert the pointer to the link to the operand stack.

    ;pushad
    ;mov eax, dword[operand_stack + (esi-1)*4]
    ;mov ebx, 0
    ;mov bl, byte[eax]
    ;push ebx
    ;push PrePrintNum
    ;call printf
    ;add esp, 8
    ;popad

    ;freeLinkedList ebp              ; Free the popped linked-list.

    popad                           ; Retrieve registers state.
    jmp loop

AND_op:
    pushad
    mov esi, [stackCounter]
    cmp esi, 2
    jl AND_fault
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
    loop_AND:
        mov dl, 255
        and dl, byte [ebx]
        and dl, byte [ecx]
        create_new_link
        update_linkedlist  

        cmp dword [ebx + 1], 0
        je first_num_end_AND
        cmp dword [ecx + 1], 0
        je second_num_end_AND

        both_not_end_AND:
        mov ecx, dword [ecx + 1]
        mov ebx, dword [ebx + 1]
        mov edx, 0
        jmp loop_AND

        first_num_end_AND:
            cmp dword [ecx + 1], 0
            je AND_end
            mov ecx, dword [ecx + 1]
            mov edx, 0
            mov dl, 255
            and dl, byte [ecx]
            create_new_link
            update_linkedlist
            jmp first_num_end_AND               

        second_num_end_AND:
            cmp dword [ebx + 1], 0
            je AND_end
            mov ebx, dword [ebx + 1]
            mov edx, 0
            mov dl, 255
            and dl, byte [ebx]
            create_new_link
            update_linkedlist
            jmp second_num_end_AND 

        AND_end:
            ;freeLinkedList esi          ; Free linked-list that edi points to.
            ;freeLinkedList edi          ; Free linked-list that edi points to.
            popad

            ;printOperandStack

            jmp loop

    AND_fault:
        pushad
        push emptyStack_err
        push PrePrintString
        push dword [stderr]
        call fprintf
        add esp, 12
        popad

    popad
    jmp loop
    


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
            ;freeLinkedList esi          ; Free linked-list that edi points to.
            ;freeLinkedList edi          ; Free linked-list that edi points to.
            popad

            ;printOperandStack

            jmp loop

    addition_fault:
        pushad
        push emptyStack_err
        push PrePrintString
        push dword [stderr]
        call fprintf
        add esp, 12
        popad

    popad
    jmp loop

; End of function myCalc();
end_myCalc:
    printInstructionCounter_oq             ; Macro that will print the instruction counter in octal.
    mov esp, ebp
    pop ebp
    ret

end_program:
    mov ebx, 0
    mov eax, 1
int 0x80