; Interpret as 64 bits code
[bits 64]

; nu uitati sa scrieti in feedback ca voiati
; assembly pe 64 de biti

section .text
global map
global reduce
map:
    ; look at these fancy registers
    push rbp
    mov rbp, rsp

    push rbx
    push rsi
    push rdi
    ; sa-nceapa turneu'

    ; rdi dst
    ; rsi  ; src
    ; rdx ; array_size
    ; rcx  ; function
    ; n - 1
    dec rdx


mapping:
    ; counting until >= 0
    cmp rdx, -1
    jz end_mapping

    push rbx
    push rsi
    push rdi

    ; function uses rdi
    ; src[i]
    mov rdi, [rsi + rdx * 8]
    call rcx

    pop rdi
    pop rsi
    pop rbx

    ; dst[i] = rax
    mov qword [rdi + rdx * 8], rax
    dec rdx
    jmp mapping

end_mapping:
    pop rdi
    pop rsi
    pop rbx
    leave
    ret


; int reduce(int *dst, int *src, int n, int acc_init, int(*f)(int, int));
; int f(int acc, int curr_elem);
reduce:
    ; look at these fancy registers
    push rbp
    mov rbp, rsp

    ; sa-nceapa festivalu'
    push rdi
    push rsi
    push rbx

    ; rdx - n

    ; from n - 1 : 0
    dec rdx

    ; rsi src
    ; rdi dst
    ; rcx - acc_init
    ; r8 - function


reducing:
    ; counting >= 0, >-1
    cmp rdx, -1
    jz end_reducing

    push rdi
    push rsi
    push rdx

    ; acc
    mov rdi, rcx

    ; src[i]
    mov rsi, [rsi + rdx * 8]
    call r8

    pop rdx
    pop rsi
    pop rdi

    ; rax - the result
    ; update acc:
    mov rcx, rax

    dec rdx
    jmp reducing



end_reducing:
    pop rbx
    pop rsi
    pop rdi

    leave
    ret

