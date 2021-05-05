
%define STK_UNIT 4

%macro startFunc 1
    push ebp
    mov ebp, esp
    sub esp, %1
%endmacro

%macro endFunc 0
    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
%endmacro

%macro	my_printf2	2
section	.rodata
%%Str2:	db	%2 , 10, 0
section	.text
	push	%1
	push	%%Str2
	call	printf
	add	esp, STK_UNIT*2
%endmacro

%macro	my_printf1	1
section	.rodata
    %%Str2:	db	%1 , 10, 0
section	.text
	push	%%Str2
	call	printf
	add	esp, STK_UNIT*1
%endmacro



section .bss                ; Uninitialized data.
    operand_stack: resd 63                              ; Remember to save what is the size is from the user.
section .data               ; Initialized data.
    pointer_to_number_string: db "3"
    

section .rodata             ; Read-only data.
    initial_print: db "calc: ",10,0
    temp2: db "the number is: %d",10,0

section .text               ; executable instructions of a program
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


main:
    pushad                      ; Backup registers.
    pushfd                      ; ; Backup flags.
                                ; If there are argumrents for mycalc(), push them here.
    call mycalc

    popfd
    popad

    jmp end_of_program          ; Just in order not to go through code again.


mycalc:
    startFunc 5                 ; Macro code will replace this. Defaultly allocetes 5*5 array (5 entrences, each of size 5).
    mov ecx, esp                ; 
    add ecx, 25                 ; ecx will point at the first cell in the operand stack.

    my_printf1 "calc:"           ; print "calc:".

    ; ecx -> pointer to the input array of numbers (saved in ascii).
    mov ebx, 0                   ; Initialize bx with 0.
    mov eax, 0                   ; Initialize ax with 0.
    mov cl, 0                   ; cl will be the index counter of ax.(It is the only one that shl works with..).
    mov ecx, 0
    ; Start loop that "isolates" the relevant 8 bits.

    ; Just for the test, we initialize ecx to point at "3".
    
mov edx, pointer_to_number_string

bit_loop:
    mov bl, [edx]
    inc edx             

    sub bl, 48                 ; get number-value of the input char (binary representation).

    shl bl, cl                 ; Put the bits in the right place before adding to ax.
    add cl, 3
    add eax, ebx                 ; Add bits to the representation.
    cmp cl, 8                  ; Check if we have 8 bits already.

    ; Need to check here before we jump back that we have another number (3 bits) to read.
    cmp bl, 0
    jz end_of_program
    jl bit_loop                ; If cl<8 -> do the loop again.



    ; Test: print the number we got in ax.
    push eax
    push temp2
    call printf
    add esp, 12

    

    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
    ;endFunc                     ; Macri code will replace with code for exiting a function.


end_of_program:                            ; End the program.


    
















    ; Old code.
    ;allocate the operand-stack (default size)
    ;push dword 20
    ;call malloc
    ;add esp, 4
    ;operand_stack: %1num dd 0


    ;Testing the allocated memory:
    ;mov dword [eax+STK_UNIT*3], 12               ; STK_UNIT=4, the size of every link in the operand-stack.
    ;mov ebx, dword [eax+STK_UNIT*3]
    ;my_printf2 ebx, "The number is: %ld"






















































