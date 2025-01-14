%include "../include/io.mac"

; The `expand` function returns an address to the following type of data
; structure.
struc neighbours_t
    .num_neighs resd 1  ; The number of neighbours returned.
    .neighs resd 1      ; Address of the vector containing the `num_neighs` neighbours.
                        ; A neighbour is represented by an unsigned int (dword).
endstruc

section .bss
; Vector for keeping track of visited nodes.
visited resd 10000
global visited

section .data
; Format string for printf.
fmt_str db "%u", 10, 0

section .text
global dfs
extern printf

; C function signature:
;   void dfs(uint32_t node, neighbours_t *(*expand)(uint32_t node))
; where:
; - node -> the id of the source node for dfs.
; - expand -> pointer to a function that takes a node id and returns a structure
; populated with the neighbours of the node (see struc neighbours_t above).
; 
; note: uint32_t is an unsigned int, stored on 4 bytes (dword).
dfs:
    push ebp
    mov ebp, esp

    ; TODO: Implement the depth first search algorith, using the `expand`
    ; function to get the neighbours. When a node is visited, print it by
    ; calling `printf` with the format string in section .data.
    push ebx
    push esi
    push edi

    ; the node
    mov ebx, [ebp + 8]

    ; the function
    mov edx, [ebp + 12]

    ; mark visited node
    mov dword [visited + ebx * 4], 1

    ; save registeers before call
    push eax
    push edx
    push ecx

    push ebx
    push fmt_str
    call printf

    ; restore stack pointer
    add esp, 8

    pop ecx
    pop edx
    pop eax

    ; save registeers before call
    push edx    
    push eax

    push ebx
    call edx

    ; restore stack pointer
    add esp, 4

    ; save the return value in esi
    mov esi, eax  
    pop eax
    pop edx

    ; eax - neighbours_t *neighbours

    ; counter for the loop:
    xor ecx, ecx
iterating:
    cmp ecx, dword [esi]
    jge end_iterating

    ; address of the array
    mov ebx, [esi + 4]

    ; value at position i
    mov ebx, dword [ebx + ecx * 4]

    ; check if visited
    cmp dword [visited + ebx * 4], 0
    jnz continue

    ; save ecx and eax
    push ecx
    push eax

    push edx
    push ebx
    call dfs

    ; restore stack pointer
    add esp, 8

    pop eax
    pop ecx

continue:
    inc ecx
    jmp iterating

end_iterating:


    pop edi
    pop esi
    pop ebx

    leave
    ret
