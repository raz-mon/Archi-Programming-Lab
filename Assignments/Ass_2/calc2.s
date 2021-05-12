
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
fgets_ass

    my_printf1 "calc:"           ; print "calc:".

section .data               ; Initialized data.
    input_string: db "3"
    ;len: equ $-input_string
section .bss
    first_char: resb 1
    pointer: resb 4              ; Will be used to point at the input string at different
section .text

; ecx -> pointer to the input array of numbers (saved in ascii).
    mov ebx, 0                   ; Initialize bx with 0.
    mov eax, 0                   ; Initialize ax with 0.
    mov edx, 0                   ; Initialize dx with 0.
    mov cl, 0                   ; cl will be the index counter of ax.(It is the only one that shl works with..).
    mov ecx, 0
    ; Start loop that "isolates" the relevant 8 bits.
    
    ; Just for the test, we initialize ecx to point at "3".
    
<<<<<<< HEAD
<<<<<<< HEAD
loop:
    fgets_ass                               ; stdio function fgets, put in buffer the wanted data
    mov edx, buffer                         ; pointer to buffer
    mov eax, 0                              
    mov ebx, 0                              ; counter for byte index
    mov al, byte [buffer + ebx]             ; set al as first buffer's byte
    cmp al, 10                              ; check if empty(only NL was sent)
    jz loop

    sec_loop:
        cmp al, 57        
        
        inc ebx                                     
        mov al, byte [buffer + ebx]                 
        cmp al, 10                                  
        jnz sec_loop                               
    jmp loop

    ;sec_loop:
    ;    my_printf2	eax, "The number is: %d"        ; print each bye in buffer, temporary for as
    ;    inc ebx                                     ; inc index's counter
    ;    mov al, byte [buffer + ebx]                 ; move to next byte in buffer
    ;    cmp al, 10                                  ; check if end of line
    ;    jnz sec_loop                                ; repeat sec loop
    ;jmp loop

    ;operand_stack: %1num dd 0
=======
mov edx, pointer_to_number_string

bit_loop:
    mov bl, [edx]
    inc edx             
>>>>>>> 530f8f02e314467666e50364b13002165c7d6812

=======
mov dl, byte [input_string]
sub dl, 48
mov byte[first_char], dl
;   mov edx, pointer_to_number_string
;   add edx, 1                      ; bl will point to the end of the input number. 
mov dword[pointer], input_string
add dword[pointer], 0
 

bit_loop:    
    mov eax, dword [pointer]
    mov bl, byte [eax]         ; bl points to the current character in the input.
>>>>>>> e40f1dfb0ab75d90bdc08a29b268ed0f586e0cbb
    sub bl, 48                 ; get number-value of the input char (binary representation).

    ;push bx
    ;push temp2
    ;call printf
    ;add esp, 6

    shl bl, cl                 ; Put the bits in the right place before adding to ax.
    add cl, 3
    add dx, bx                 ; Add bits to the representation.

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
    dec dword[pointer]
    
    cmp bl, [first_char]        ; Check if there are any numbers left to read in the input.
    jz print_dx_test           ; If not, finish the loop. 
>>>>>>> e40f1dfb0ab75d90bdc08a29b268ed0f586e0cbb

    cmp cl, 8                  ; Check if we have 8 bits already.
    jl bit_loop                ; If cl<8 -> do the loop again.

    ; Test: print the number we got in ax.
print_dx_test:
    push dx
    push temp2
    call printf
    add esp, 6


>>>>>>> 530f8f02e314467666e50364b13002165c7d6812
    mov esp, ebp                ; "release" the activation frame.
    pop ebp                     ; restore activation frame of main.
    ret                         ; return from the function.
    ;endFunc                     ; Macro code will replace with code for exiting a function.


end_of_program:                            ; End the program.
    








;        mov dl, byte [pointer_to_number_string]
;    sub dl, 48
;    mov byte[first_char], dl
;    mov edx, pointer_to_number_string
;    add edx, 1                      ; bl will point to the end of the input number. 
;
;    bit_loop:            
;        mov bl, byte [edx]         ; bl points to the current character in the input.
;        sub bl, 48                 ; get number-value of the input char (binary representation).
;
;        push bx
;        push temp2
;        call printf
;        add esp, 6
;
;        shl bl, cl                 ; Put the bits in the right place before adding to ax.
;        add cl, 3
;        add ax, bx                 ; Add bits to the representation.
;
;        dec edx
;        
;        cmp bl, [first_char]        ; Check if there are any numbers left to read in the input.
;        jz end_of_program           ; If not, finish the loop. 
;
;        cmp cl, 8                  ; Check if we have 8 bits already.
;        jl bit_loop                ; If cl<8 -> do the loop again.











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






















































