; subtask 1 - qsort

section .text
    global quick_sort
    ;; no extern functions allowed

quick_sort:
    ;; create the new stack frame
    enter 0, 0

    ;; save the preserved registers
    push ebx
    push esi
    push edi

    ;; recursive qsort implementation goes here

    ; ecx is the buffer
    mov ecx, [ebp + 8]
    ; start
    mov edx, [ebp + 12]
    ; end
    mov ebx, [ebp + 16]
    ; if start - end >= 0
    cmp edx, ebx
    jge end

    mov eax, ebx
    add eax, edx
    ; divide by 2
    shr eax, 1
    ; mid = (start + end) / 2;

    ; buff[start] is the pivot

    ; swap(buff[mid], buff[st])

    ; buff[start]
    push dword [ecx + edx * 4]
    ; buff[mid]
    push dword [ecx + eax * 4]
    ; buff[start] = buff[mid]
    pop dword [ecx + edx * 4]
    ; buff[mid] = old_buff[start]
    pop dword [ecx + eax * 4]


    ; i = start
    mov esi, edx

    ; j = end
    mov edi, ebx

    ; k = 0, k is either 0 or 1
    xor eax, eax

    push edx
    push ebx
    xor edx, edx
    xor ebx, ebx
sorting:
    cmp esi, edi
    jz end_sorting

    ; buff[i]
    mov dword ebx, [ecx + esi * 4]
    ; buff[j]
    mov dword edx, [ecx + edi * 4]
    cmp ebx, edx
    jle no_swap

    ; swap buff[i] with buff[j]
    ; push buff[i]
    push dword [ecx + esi * 4]
    ; push buff[j]
    push dword [ecx + edi * 4]
    ; buff[i] = buff[j]
    pop dword [ecx + esi * 4]
    ; buff[j] = old_buff[i]
    pop dword [ecx + edi * 4]

    ; k = 1 - k
    mov ebx, eax
    ; mov 1 in eax for difference
    mov eax, 1
    sub eax, ebx

no_swap:
    ; i = i + k
    add esi, eax

    ; j = j - (1 - k);
    mov ebx, 1
    sub ebx, eax
    sub edi, ebx

    jmp sorting

end_sorting:
    pop ebx
    pop edx

    mov edi, esi

    ; i + 1
    add edi, 1  
    ; i - 1
    sub esi, 1

    ; quick_sort(buffer, start, i - 1)
    push esi
    push edx
    push ecx
    call quick_sort
    ; restore stack pointer
    add esp, 12

    ; quick_sort(buffer, i + 1, end)
    push ebx
    push edi
    push ecx
    call quick_sort
    ; restore stack pointer
    add esp, 12

    ;; restore the preserved registers

end:
    pop edi
    pop esi
    pop ebx

    leave
    ret
