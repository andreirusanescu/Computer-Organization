; declare your structs here

struc request
	.admin: resb 1
	.prio: resb 1
	.passkey: resw 1
	.username: resb 51
endstruc

section .text
	global sort_requests
	extern printf

sort_requests:
	;; DO NOT MODIFY
	enter 0,0
	pusha

	mov ebx, [ebp + 8]      ; requests
	mov ecx, [ebp + 12]     ; length
	;; DO NOT MODIFY

	;; Your code starts here

	mov edi, ecx    ; length
	xor ecx, ecx    ; counter for loop1
	sub edi, 1		; counting until n - 1
	
loop1:
	cmp ecx, edi
	jz end_loops
	xor esi, esi    ; counter for loop2
	push ecx 		; need it for use in loop2
loop2:
	cmp esi, edi	; j < n - 1
	jz end_loop1

	mov eax, esi 	; compute the next structure index
	imul eax, request_size

	mov cl, [ebx + eax + request.admin]
	add eax, request_size
	mov dl, [ebx + eax + request.admin]
	sub eax, request_size

	cmp cl, dl		; check if admin
	jl swap 		; first is not admin, second is
	cmp cl, dl		; first is, second not, no swap
	jg end_loop2

	; both are admins, compare priorities
	mov cl, [ebx + eax + request.prio]
	add eax, request_size
	mov dl, [ebx + eax + request.prio]
	sub eax, request_size

	cmp cl, dl
	ja swap			; unsigned comparison
	cmp cl, dl
	jb end_loop2


	; both have the same priority
	; strcmp:
	push edi		; save edi
	mov dh, 0 		; dh is the flag for swapping

	; dh is -1 | 0 | 1
	; if dh is 1, swap

	xor edi, edi	; edi is now counter

while_strcmp:

	cmp dh, 0		; if the flag was set
	jnz end_strcmp

	cmp edi, 52		; it reached the end
	jz end_strcmp

	mov cl, [ebx + eax + request.username]
	add eax, request_size
	mov dl, [ebx + eax + request.username]
	sub eax, request_size

	; compare characters:
	cmp cl, dl
	jl flag_negative
	cmp cl, dl
	jg flag_positive
	jmp flag_zero

flag_negative:
	mov dh, -1
	inc edi
	inc eax
	jmp while_strcmp

flag_positive:
	mov dh, 1
	inc edi
	inc eax
	jmp while_strcmp

flag_zero:
	mov dh, 0
	inc edi
	inc eax
	jmp while_strcmp


end_strcmp:
	pop edi

	; if flag is 1, swap:
	cmp dh, 1
	jnz end_loop2
swap:
	push edi
	xor edi, edi 	; counter for deep copy

	mov eax, esi 	; compute the structure index
	imul eax, request_size

	; deep copy loop:
swap_loop:
	cmp edi, 55		; structure is packed
	jz end_swap
	mov cl, [ebx + eax]
	add eax, request_size
	mov dl, [ebx + eax]
	sub eax, request_size

	xchg cl, dl

	mov [ebx + eax], cl
	add eax, request_size
	mov [ebx + eax], dl
	sub eax, request_size

	inc edi
	inc eax
	jmp swap_loop

end_swap:
	pop edi

end_loop2:
	inc esi
	jmp loop2
end_loop1:
	pop ecx
	inc ecx
	jmp loop1

end_loops:

	;; Your code ends here

	;; DO NOT MODIFY
	popa
	leave
	ret
	;; DO NOT MODIFY