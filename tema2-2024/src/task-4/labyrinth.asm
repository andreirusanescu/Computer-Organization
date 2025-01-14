%include "../include/io.mac"

extern printf
extern position
global solve_labyrinth

; you can declare any helper variables in .data or .bss

section .text

; void solve_labyrinth(int *out_line, int *out_col, int m, int n, char **labyrinth);
solve_labyrinth:
	;; DO NOT MODIFY
	push    ebp
	mov     ebp, esp
	pusha

	mov     eax, [ebp + 8]  ; unsigned int *out_line, pointer to structure containing exit position
	mov     ebx, [ebp + 12] ; unsigned int *out_col, pointer to structure containing exit position
	mov     ecx, [ebp + 16] ; unsigned int m, number of lines in the labyrinth
	mov     edx, [ebp + 20] ; unsigned int n, number of colons in the labyrinth
	mov     esi, [ebp + 24] ; char **a, matrix representation of the labyrinth
	;; DO NOT MODIFY
   
	;; Freestyle starts here
	push eax
	push ebx
	xor eax, eax ; counter for rows
	xor ebx, ebx ; counter for columns

while:
	mov edi, eax
	inc edi
	cmp ecx, edi ; if i != m - 1
	jz end_while
	mov edi, ebx
	inc edi
	cmp edx, edi ; if j != n - 1
	jz end_while

cond1:
	mov edi, ebx
	inc edi

	; if j + 1 < n
	cmp edi, edx
	jge cond2

	push ecx
	push edx
	push esi
	push eax

	; sizeof(char *) = 4
	imul eax, 4
	add esi, eax

	; *(labyrinth + i):
	mov ecx, [esi]
	add ecx, ebx
	inc ecx

	;*(*(labyrinth + i) + j + 1):
	mov dl, byte [ecx]
	cmp dl, '0'
	jnz wall

	; not wall
	dec ecx
	; make 1 behind so it doesn't return
	mov byte [ecx], '1'

	; j++
	inc ebx

	; restore
	pop eax
	pop esi
	pop edx
	pop ecx

	jmp while
wall:
	; restore
	pop eax
	pop esi
	pop edx
	pop ecx

cond2:
	; if j - 1 >= 0
	cmp ebx, 1
	jl cond3

	push ecx
	push edx
	push esi
	push eax

	; compute for *(*(labyrinth + i) + j - 1)
	imul eax, 4
	add esi, eax
	mov ecx, [esi]
	add ecx, ebx
	dec ecx
	mov dl, byte [ecx]
	cmp dl, '0'
	jnz wall2

	;; not wall
	pop eax
	inc ecx
	mov byte [ecx], '1'
	dec ebx

	pop esi
	pop edx
	pop ecx

	jmp while
wall2:
	pop eax
	pop esi
	pop edx
	pop ecx
	
cond3:
	; if i - 1 >= 0
	cmp eax, 1
	jl cond4

	push ecx
	push edx
	push esi
	push eax

	; compute for *(*(labyrinth + i - 1) + j)
	dec eax
	imul eax, 4
	add esi, eax
	mov ecx, [esi]
	add ecx, ebx
	mov dl, byte [ecx]
	cmp dl, '0'
	jnz wall3

	; not wall
	add esi, 4
	pop eax
	dec eax
	mov ecx, [esi]
	add ecx, ebx
	mov byte [ecx], '1'

	pop esi
	pop edx
	pop ecx
	jmp while
wall3:
	pop eax
	pop esi
	pop edx
	pop ecx

cond4:
	mov edi, eax
	inc edi

	; if i + 1 < m
	cmp edi, ecx
	jge out_while

	push ecx
	push edx
	push esi
	push eax

	; compute for *(*(labyrinth + i + 1) + j)
	inc eax
	imul eax, 4
	add esi, eax
	mov ecx, [esi]
	add ecx, ebx
	mov dl, byte [ecx]
	cmp dl, '0'
	jnz wall4

	; not wall
	sub esi, 4
	mov ecx, [esi]
	add ecx, ebx
	mov byte [ecx], '1'
	pop eax
	inc eax

	pop esi
	pop edx
	pop ecx
	jmp while
wall4:
	pop eax
	pop esi
	pop edx
	pop ecx


out_while:
	
end_while:
	; lens are not needed
	mov ecx, eax
	mov edx, ebx
	pop ebx
	pop eax

	; load the indices
	mov [eax], ecx
	mov [ebx], edx

	;; Freestyle ends here
end:
	;; DO NOT MODIFY

	popa
	leave
	ret
	
	;; DO NOT MODIFY
