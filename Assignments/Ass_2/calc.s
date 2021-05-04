
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

section .data               ; Initialized data.

section .rodata             ; Read-only data.
    initial_print: db "calc: ",10,0

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

    ;testing shit:
    mov ebx, 10o
    my_printf2	ebx, "The number is: %ld"
    mov ecx, 20o
    add ebx, ecx
    my_printf2	ebx, "The number is: %ld"
    


    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
    ;endFunc                     ; Macri code will replace with code for exiting a function.


end_of_program:                            ; End the program.


    





















































