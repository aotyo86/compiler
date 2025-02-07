section .data
    format db "Result: %d", 10, 0   ; printf 用フォーマット文字列

section .text
    global _start
    extern printf

_start:
    ; calculate 0+(1+(3-2))-(0+(1+(2-3)))

    mov rax, 0   ; load 0 into rax
    push rax     ; stack 0
    
    mov rax, 1   ; load 1 into rax
    push rax     ; stack 1

    mov rax, 3   ; load 3 into rax
    push rax     ; stack 3

    mov rax, 2   ; load 2 into rax
    pop rbx      ; pop 3

    sub rbx, rax ; rbx = 3 - 2
    pop rax      ; pop 1

    add rax, rbx ; rax = 1 + (3-2)    
    pop rbx        ; pop 0

    add rbx, rax ; rbx = 0 + (1+(3-2))
    push rbx     ; stack 0 + (1+(3-2))

    mov rax, 0   ; load 0 into rax
    push rax     ; stack 0

    mov rax, 1   ; load 1 into rax
    push rax     ; stack 1

    mov rax, 2   ; load 2 into rax
    push rax     ; stack 2

    mov rax, 3   ; load 3 into rax
    pop rbx      ; pop 2

    sub rbx, rax ; rbx = 2 - 3
    pop rax      ; pop 1

    add rax, rbx ; rax = 1 + (2-3)    
    pop rbx        ; pop 0

    add rbx, rax ; rbx = 0 + (1+(2-3))

    pop rax      ; pop 0+(1+(3-2))

    sub rax, rbx ; rax = 0+(1+(3-2)) - 0+(1+(2-3))
    
    mov rdi, rax ;
    call _print  ;
  
    mov rax, 60;
    xor rdi, rdi
    syscall

; _print 関数（printf を呼ぶ）
_print:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov rsi, rdi
    lea rdi, [format];
    xor rax, rax ;
    call printf             ; printf を呼び出

    mov rsp, rbp
    pop rbp
    ret
