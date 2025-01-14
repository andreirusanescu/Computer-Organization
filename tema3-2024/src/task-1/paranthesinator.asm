; Interpret as 32 bits code
[bits 32]

%include "../include/io.mac"


section .text
; int check_parantheses(char *str)
global check_parantheses
check_parantheses:
    push ebp
    mov ebp, esp

    ; sa-nceapa concursul

    ; string
    mov esi, [ebp + 8]

    ; counter for string
    xor ecx, ecx
while:
    xor edx, edx
    mov dl, byte [esi + ecx]
    ; null terminator
    cmp dl, 0
    jz end_while

cond1:
    cmp dl, '('
    jnz cond2
    push edx
    jmp increment

cond2:
    cmp dl, ')'
    jnz cond3

    ; stack peak compared
    cmp dword [esp], '('
    jnz wrong
    pop edx
    jmp increment

cond3:
    cmp dl, '['
    jnz cond4
    push edx
    jmp increment

cond4:
    cmp dl, ']'
    jnz cond5

    cmp dword [esp], '['
    jnz wrong
    pop edx
    jmp increment

cond5:
    cmp dl, '{'
    jnz cond6
    push edx
    jmp increment

cond6:
    ;cmp dl, '}'
    ;jnz wrong

    cmp dword [esp], '{'
    jnz wrong
    pop edx

increment:
    inc ecx
    jmp while


end_while:
    cmp ebp, esp
    jz good
wrong:
    ; return code 1
    mov eax, 1
    jmp end
good:
    ; return code 0
    mov eax, 0
end:
    leave
    ret
