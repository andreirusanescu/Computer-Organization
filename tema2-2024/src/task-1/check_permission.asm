%include "../include/io.mac"

extern ant_permissions

extern printf
global check_permission

section .text

check_permission:
	;; DO NOT MODIFY
	push    ebp
	mov     ebp, esp
	pusha

	mov     eax, [ebp + 8]  ; id and permission
	mov     ebx, [ebp + 12] ; address to return the result
	;; DO NOT MODIFY
   
	;; Your code starts here

	mov edi, eax ; store the value of n
	shr eax, 24  ; just the id

	; store the address of ant[i]
	lea edx, [ant_permissions + eax * 4]

	; store the number at ant[i] in eax
	mov eax, dword [edx]

	xor ecx, ecx
	mov edx, dword 1 ; used for checking if n & (1 << ecx)
iterate:
	; loop from 0 to 23 to check
	; what rooms the ant wants

	cmp ecx, 24
	jz yes
	; if the loop got to 24, it means
	; it never jumped to no

	test edi, edx ; check if ant wants room
	jz end_iterate ; ant doesn't want room

	test eax, edx  ; check if room is empty
	jz no ; edx & eax == 0 => room is occupied

end_iterate:
	inc ecx
	shl edx, 1
	jmp iterate

yes:
	mov [ebx], dword 1
	jmp final

no:
	mov [ebx], dword 0

final:

	;; Your code ends here
	
	;; DO NOT MODIFY

	popa
	leave
	ret
	
	;; DO NOT MODIFY
