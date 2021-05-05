
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

%macro fgets_ass 0
    push dword [stdin]              ;path to file(stdin)
    push dword 81                   ;max lenght
    push dword buffer               ;input buffer
    call fgets
    add esp, STK_UNIT*3                     
%endmacro

%macro create_new_link 0
    push 5
    call malloc
    add esp, STK_UNIT * 1
%endmacro

%macro update_linkedlist 0
    mov byte [eax], dl                              ; insert dl byte in the first byte in new link
    mov esi, dword [current_link_ptr]               ; esi is now a ptr to the latest updated link (with offset 1)
    mov dword [esi], eax                            ; old link point to the begginig of new link
    inc esi                                         ; old link point to the second byte in new link 
    mov dword [current_link_ptr], esi               ; current_link_ptr point to second byte in new link
%endmacro



section .bss                ; Uninitialized data.
    operand_stack: resd 63                              ; Remember to save what is the size is from the user.
    buffer:        resb 81                              ; max size - input line , 80 bytes + 1 byte

section .data               ; Initialized data.
    current_link_ptr: db 4

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
    startFunc 0                 ; Macro code will replace this. Defaultly allocetes 5*5 array (5 entrences, each of size 5).

    my_printf1 "calc:"           ; print "calc:".

section .data               ; Initialized data.
    input_string: db "123"
    ;len: equ $-input_string
    temp: dw 0
    counter: dd 0
section .bss
    first_char: resb 1
    pointer: resb 4              ; Will be used to point at the input string at different
section .text

; ecx -> pointer to the input array of numbers (saved in ascii).
    mov ebx, 0                   ; Initialize bx with 0.
    mov eax, 0                   ; Initialize ax with 0.
    mov edx, 0                   ; Initialize dx with 0.
    mov cx, 0                    ; cx will be the index counter of ax.(It is the only one that shl works with..).
    mov ecx, 0
    ; Start loop that "isolates" the relevant 8 bits.
    
    ; Just for the test, we initialize ecx to point at "".
    
mov dl, byte [input_string]
sub dl, 48
mov byte[first_char], dl
mov dl, 0

mov dword[pointer], input_string

add dword[pointer], 2

mov dword[counter], 2

mov edx, 0

bit_loop:    
    ;mov bx, 0
    mov esi, dword [pointer]
    mov bl, byte [esi]         ; bl points to the current character in the input.
    sub bl, 48                 ; get number-value of the input char (binary representation).

    shl bx, cl                 ; Put the bits in the right place before adding to ax.
    add cl, 3
    add dx, bx                 ; Add bits to the representation.

    dec dword[pointer]

    cmp dword [counter], 0
    jz print_dx_test            ; jmp from loop when counter = 0.
    dec dword [counter]

    cmp cl, 8                  ; Check if we have 8 bits already.
    jl bit_loop                ; If cl<8 -> do the loop again.

;construct_new_link: 
;    create_new_link
;    update_linkedlist
;    jmp bit_loop

print_dx_test:
    ;and dx, 0xFF
    push dx
    push temp2
    call printf
    add esp, 8

    endFunc                     ; Macro code will replace with code for exiting a function.                       ; return from the function.


end_of_program:                            ; End the program.







;loop:
;    fgets_ass                               ; stdio function fgets, put in buffer the wanted data
;    mov edx, buffer                         ; pointer to buffer
;    mov eax, 0                              
;    mov ebx, 0                              ; counter for byte index
;    mov al, byte [buffer + ebx]             ; set al as first buffer's byte
;    cmp al, 10                              ; check if empty(only NL was sent)
;    jz loop

;    cmp al, 57        
;    jg lexical_commands
;    cmp al, 48
;    jl mathematical_commands
;        numbers:
;        sub al, 48
;        my_printf2	eax, "The number is: %d"
;        inc ebx                                     
;        mov al, byte [buffer + ebx]                 
;        cmp al, 10                                  
;        jnz numbers                               
;jmp loop


















