; subtask 2 - bsearch

section .text
    global binary_search
    ;; no extern functions allowed

binary_search:
    ;; create the new stack frame
    enter 0, 0

    ; fastcall convention
    ; ecx - the array
    ; edx - the needle

    ;; save the preserved registers
    push ebx
    push esi
    push edi

    ;; recursive bsearch implementation goes here

    ; start
    mov ebx, [ebp + 8]
    ; end
    mov eax, [ebp + 12]
    cmp ebx, eax
    jg end_negative

    ; end - start
    sub eax, ebx

    ; divide by 2
    shr eax, 1

    ; mid = start + (end - start) / 2
    add eax, ebx

    ; v[mid]
    mov ebx, [ecx + eax * 4]

    ; value found
    ; already saved in eax
    cmp ebx, edx
    jz out

cond1:
    cmp ebx, edx
    jg cond2
    inc eax

    ; end
    push dword [ebp + 12]
    ; mid + 1
    push eax
    call binary_search
    ; restore stack pointer
    add esp, 8
    jmp out

cond2:
    dec eax
    ; mid - 1
    push eax
    ; start
    push dword [ebp + 8]
    call binary_search

    ; restore stack pointer
    add esp, 8
    jmp out


end_negative:
    ; return code -1
    mov eax, -1

    ;; restore the preserved registers
out:
    pop edi
    pop esi
    pop ebx

    leave
    ret
