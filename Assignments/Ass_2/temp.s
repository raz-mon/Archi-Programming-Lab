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

%macro fgets_ass 0
    push dword [stdin]              ;path to file(stdin)
    push dword 81                   ;max lenght
    push dword buffer               ;input buffer
    call fgets
    add esp, STK_UNIT*3                     
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
    buffer:        resb 81                              ; max size - input line , 80 bytes + 1 byte for NL + 1 byte for '0'

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
    mov ebx, 10
    my_printf2	ebx, "The number is: %ld"
    mov ecx, 20
    add ebx, ecx
    my_printf2	ebx, "The number is: %ld"
    
    ;allocate the operand-stack (default size)
    push dword 20
    call malloc
    add esp, 4
    
loop:
    fgets_ass                               ; stdio function fgets, put in buffer the wanted data
    mov edx, buffer                         ; pointer to buffer
    mov eax, 0                              
    mov ebx, 0                              ; counter for byte index
    mov al, byte [buffer + ebx]             ; set al as first buffer's byte
    cmp al, 10                              ; check if empty(only NL was sent)
    jz loop

    cmp al, 57        
    jg lexical_commands
    cmp al, 48
    jl mathematical_commands
        numbers:
        sub al, 48
        my_printf2	eax, "The number is: %d"
        inc ebx                                     
        mov al, byte [buffer + ebx]                 
        cmp al, 10                                  
        jnz numbers                               
    jmp loop


    ;sec_loop:
    ;    my_printf2	eax, "The number is: %d"        ; print each bye in buffer, temporary for as
    ;    inc ebx                                     ; inc index's counter
    ;    mov al, byte [buffer + ebx]                 ; move to next byte in buffer
    ;    cmp al, 10                                  ; check if end of line
    ;    jnz sec_loop                                ; repeat sec loop
    ;jmp loop

    ;operand_stack: %1num dd 0


    ;Testing the allocated memory:
    ;mov dword [eax+STK_UNIT*3], 12               ; STK_UNIT=4, the size of every link in the operand-stack.
    ;mov ebx, dword [eax+STK_UNIT*3]
    ;my_printf2 ebx, "The number is: %ld"

lexical_commands:

mathematical_commands:

    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
    ;endFunc                     ; Macri code will replace with code for exiting a function.


end_of_program:                            ; End the program.