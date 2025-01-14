; declare your structs here

struc request
	.admin: resb 1
	.prio: resb 1
	.passkey: resw 1
	.username: resb 51
endstruc

section .text
	global check_passkeys
	extern printf

check_passkeys:
	;; DO NOT MODIFY
	enter 0, 0
	pusha

	mov ebx, [ebp + 8]      ; requests
	mov ecx, [ebp + 12]     ; length
	mov eax, [ebp + 16]     ;
	;; DO NOT MODIFY

	;; Your code starts here

	; eax is the new length
	; eax will be used at divisions
	xchg eax, ecx

	mov edi, request_size	; used for division
	sub eax, 1
	imul eax, request_size
big_loop:
	xor esi, esi 	; esi is either 1 or 0
	; esi == 1, counter is in the most significant part

	cmp eax, 0
	jl end_big_loop

	push ecx
	xor edx, edx
	mov dx, [ebx + eax + request.passkey]

	;	if msb is 0
	cmp dx, word 0
	jns no_hacker
	
	; is lsb is 0
	mov ecx, dword 1
	test cx, dx
	jz no_hacker

	; counting bits loop
	push eax
	push ebx
	mov ebx, dword 14	; 14 bits to check
	shl ecx, 1
	xor eax, eax
hacker_loop:
	cmp ebx, 7			; end for lsb part
	jz end_hacker_loop
	cmp ebx, -1			; end for msb part
	jz end_hacker_loop

	test cx, dx
	jz no_one	
	inc eax 			; 1 was found
no_one:
	shl ecx, 1
	dec ebx
	jmp hacker_loop

end_hacker_loop:
	push edx
	push eax
	push ebx
	xor edx, edx
	mov ebx, dword 2
	div ebx
	pop ebx
	pop eax
	cmp esi, 0	; for lsb part
	jz lsb
	
	; if the number of 1 is even
	cmp edx, 0
	jnz hacker
	cmp edx, 0
	jz no_hacker_1

lsb:
	cmp edx, 0 	; remainder for division by 2
	jnz no_hacker_1

	; for msb part
	pop edx
	mov esi, 1
	dec ebx
	xor eax, eax
	jmp hacker_loop

no_hacker_1:
	pop edx
	pop ebx
	pop eax
	
no_hacker:
	pop ecx
	push eax
	push edx
	xor edx, edx
	div edi
	mov byte [ecx + eax], 0
	pop edx
	pop eax
	jmp end_loop
hacker:
	pop edx
	pop ebx
	pop eax
	pop ecx
	push eax
	push edx
	xor edx, edx
	div edi
	mov byte [ecx + eax], 1
	pop edx
	pop eax
	jmp end_loop

end_loop:
	sub eax, request_size
	jmp big_loop


end_big_loop:
	; establish the initial order
	xchg eax, ecx
	;; Your code ends here

	;; DO NOT MODIFY
	popa
	leave
	ret
	;; DO NOT MODIFY