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
  extern gets 
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr


main:
    pushad
    pushfd
                                ; If there are argumrents for mycalc(), push them here.
    call mycalc


mycalc:
    push ebp					; 
	mov ebp, esp				; 
	pushad						; Save configuration of registers.

    push initial_print          ; print "calc:".
    call printf
    add esp, 4

    




    





















































