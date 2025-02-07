section .data
    format db "Result: %d", 10, 0   ; printf 用フォーマット文字列

section .text
    global _start
    extern printf

_start:
    ; calculate 3-(4-2)

    mov rax, 3   ; load 3 into rax
    push rax     ; stack 3
    
    mov rax, 4   ; load 4 into rax
    push rax     ; stack 4

    mov rax, 2   ; load 2 into rax
    pop rbx      ; pop 4
    sub rbx, rax ; rbx = rax - rax
    
    pop rax      ; pop 3
    sub rax, rbx ; rax = rax - rbx
    
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
    call printf             ; printf を呼び出す

    mov rsp, rbp
    pop rbp
    ret
