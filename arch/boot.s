section .multiboot
align 4
    dd 0x1BADB002       ; Multiboot
    dd 0x00           ; Flags
    dd -(0x1BADB002 + 0X00) ; Checksum

section .text
global start
extern kernel_main

start:
    ; Setup a stack (essential for C code)
    mov esp, stack_top

    ; Call our C kernel entry point
    call kernel_main

.hang:
    hlt ; Halt the CPU
    jmp .hang  ; Loop forever if kernel_main returns

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB
stack_top:
