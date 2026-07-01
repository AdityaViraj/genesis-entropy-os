; The Assembly Stub 
; The CPU expects asn assembly function to handle the jump. Create the file to save the CPU's state 

extern keyboard_handler_c
global keyboard_handler_asm

keyboard_handler_asm:
    pusha            ; Save all the registers 
    call keyboard_handler_c

    mov al, 0x20 ; Send EOI(end of interrupt) to PIC
    out 0x20, al

    popa ; Restore all registers
    iret ; Return from interrupt

; pushad/popad : These instructions save and restore the CPU registers. Without this our C code would overwrite the registers and the CPU would crash when it returned to the naib program
; 0x20 (EOI) : The PIC hardware needs a signal that the interrupt was handled. If we don't send this EOI the keyboard will never send another signak again.
; IRO Mapping : By default , the keyboard is IRQ 1. We map it to index 33 in the IDT to avoid conflicts with CPU exceptions.
