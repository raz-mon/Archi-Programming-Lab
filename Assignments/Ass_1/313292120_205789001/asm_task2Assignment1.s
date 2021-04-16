section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
					;				an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]
	an: resb 100		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)] + 1 bit for the null-char

section .data
	ret_val: dd 0			; The return value
	counter: dd 0			; Simple counter

section .text
	global convertor
	extern printf

convertor:
	push ebp					; 
	mov ebp, esp				; 
	pushad						; Save configuration of registers.
	mov ecx, dword [ebp+8]		; get function argument (pointer to string)
	mov edx, 0					; initialize our placement counter with 0.
ourCode:						; 		 				
	mov dword [counter], 0		; Initialize M[counter].
	mov al, byte [ecx]			; copy first byte of the arguments of converter to al. 
	cmp al, 'q'					; check if the input byte is 'q'.
	jz is_q						; If so, jump to is_q.
	cmp al, 10					; Check if the byte represents new-Line.
	jz end_loop					; If so, jump to end_loop.
	cmp al, 57					; Check wether the  input character is '0'-'9' or 'A'-'F' (this and the following)
	jg A_F						; If it is 'A'-'F' -> jump to A_F
	sub al, 48					; 

ll:
	mov ah, al
	AND ah, 8
	jz add_0
	mov byte [an+edx], 49
cont_loop:
	inc edx
	sal byte al, 1
	inc dword [counter]
	cmp dword [counter], 4
	jnz ll
	inc ecx
	cmp byte [ecx], 0
	jnz ourCode
	jmp end_loop

A_F:
	sub al, 55
	jmp ll

add_0:
	mov byte [an+edx], 48
	jmp cont_loop

is_q:
	mov dword [ret_val], 1
	
end_loop:
	mov byte [an+edx], 0									; null ternminator.
	jmp clean_zeros
print_and_exit:
	push an			; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 8		; clean up stack after call

	popad			
	mov eax, [ret_val]
	mov esp, ebp	
	pop ebp
	ret


clean_zeros:
	cmp byte [an], 48
	jnz print_and_exit
	
	cmp byte [an+1], 0
	jz print_and_exit

	mov ebx, an
moveleft:
	mov al, byte [ebx+1]
	mov  byte [ebx], al
	cmp byte [ebx+1], 0
	jz clean_zeros
	inc ebx
	jmp moveleft

