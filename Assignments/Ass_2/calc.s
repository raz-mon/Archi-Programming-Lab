
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
    pointer_to_number_string: db "3"
    ;len: equ $-pointer_to_number_string
section .bss
    first_char: resb 1
section .text

; ecx -> pointer to the input array of numbers (saved in ascii).
    mov ebx, 0                   ; Initialize bx with 0.
    mov eax, 0                   ; Initialize ax with 0.
    mov cl, 0                   ; cl will be the index counter of ax.(It is the only one that shl works with..).
    mov ecx, 0
    ; Start loop that "isolates" the relevant 8 bits.
    
    ; Just for the test, we initialize ecx to point at "346542165432".
    
mov dl, byte [pointer_to_number_string]
mov byte [first_char], dl
mov edx, pointer_to_number_string

mov bl, byte [edx]
add bl, 0                      ; bl will point to the end of the input number. 
bit_loop:            
    
    sub bl, 48                 ; get number-value of the input char (binary representation).

    shl bl, cl                 ; Put the bits in the right place before adding to ax.
    add cl, 3
    add ax, bx                 ; Add bits to the representation.

    dec edx
    
    cmp bl, [first_char]        ; Check if there are any numbers left to read in the input.
    jz print_eax_test           ; If not, finish the loop. 

    mov bl, byte [edx]         ; bl points to the current character in the input.

    cmp cl, 8                  ; Check if we have 8 bits already.
    jl bit_loop                ; If cl<8 -> do the loop again.

    

    ; Test: print the number we got in ax.
print_eax_test:
    push ax
    push temp2
    call printf
    add esp, 12

    

    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
    ;endFunc                     ; Macri code will replace with code for exiting a function.