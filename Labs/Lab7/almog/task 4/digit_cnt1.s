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

section .data               ; initialized data.
    empty_str: db "Empty String" , 10, 0
    num_of_digits: db "%d" , 10 , 0
section .rodata             ; read-only data.

section .text               ; text
main:
    digit_cnt:
        push ebp
        mov ebp, esp
    mov ecx, 0
    ;Get stack size and assign it to stackSize.
    mov eax, dword[esp + 8]        ; eax = argv
    mov eax, dword[eax + 4]       ; eax = argv[1].

    cmp eax, 0
    jnz loop

    pushad
    push empty_str
    call printf
    add esp, 4
    popad

    jmp end_digit_cnt
loop:
    cmp byte[eax], 0
    je end_loop
    cmp byte[eax], 48
    jl next_and_loop
    cmp byte[eax], 57
    jg next_and_loop

    inc ecx
    next_and_loop:
    inc eax
    jmp loop

    end_loop:
        pushad
        push ecx
        push num_of_digits
        call printf
        add esp, 8
        popad
    end_digit_cnt:
        mov esp, ebp
        pop ebp
        ret
end_program:
    mov ebx, 0
    mov eax, 1
int 0x80