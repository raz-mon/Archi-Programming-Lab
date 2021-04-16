section .data                    	; we define (global) initialized variables in .data section
        an: dd 0              		; an is a local variable of size double-word, we use it to count the string characters

section .text                    	; we write code in .text section
        global do_Str          		; 'global' directive causes the function do_Str(...) to appear in global scope

do_Str:                        		; do_Str function definition - functions are defined as labels
        push ebp              		; save Base Pointer (bp) original value
        mov ebp, esp         		; use Base Pointer to access stack contents (do_Str(...) activation frame)
        pushad                   	; push all signficant registers onto stack (backup registers values)
        mov ecx, dword [ebp+8]		; get function argument on stack
					; now ecx register points to the input string
	yourCode:			; use label to build a loop for treating the input string characters
                mov eax, 0              ; initialize eax with 0.
                cmp byte [ecx], 32      ; Check if [ecx] is a 'space'.
                jnz not_space           ; if not, skip next line.
                inc dword [an]          ; increment counter.
        not_space:
                cmp byte [ecx], 65      ; compare the character (int value) with 65 (the int value of 'A'). 
                jl not_in_range         ; If 65 > int value of the current character, jump to "not_in_range".
                cmp byte [ecx], 90      ; compare the current characters int value with 90 (int val of 'Z').
                jg not_in_range         ; If 90 < int value of the current character, jump to "not_in_range".
                add byte [ecx], 32      ; If 65 <= (int val of current char) <= 90, change (in place) the value of the character to the lower-case value.
        not_in_range:                   ; Jump here if the character is other than upper-case.
		inc ecx      	    	; increment ecx value; now ecx points to the next character of the string
		cmp byte [ecx], 0   	; check if the next character (character = byte) is zero (i.e. null string termination)
		jnz yourCode      	; if not, keep looping until meet null termination character

        popad                    	; restore all previously used registers
        mov eax,[an]         		; return an (returned values are in eax)
        mov esp, ebp			; free function activation frame
        pop ebp				; restore Base Pointer previous value (to returnt to the activation frame of main(...))
        ret				; returns from do_Str(...) function
