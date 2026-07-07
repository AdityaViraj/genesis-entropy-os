#include "idt.h"
#include "io.h"
#include "panic.h"

// Define the IDT and pointer globally so they persist.
struct idt_entry idt[256];
struct idt_ptr idtp;

extern void isr32_wrapper(); // Assembly wrapper for the timer interrupt handler. Tells the compiler that this function is defined elsewhere (in assembly) and will be linked later. This allows us to call the assembly function from C code without getting a "function not found" error during compilation.
extern void isr33_wrapper(); // Assembly wrapper for the keyboard interrupt handler . Tells the compiler that this function is defined elsewhere (in assembly) and will be linked later. This allows us to call the assembly function from C code without getting a "function not found" error during compilation.

void idt_set_gate(unsigned char num,unsigned long base,unsigned short sel,unsigned char flags)
{
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFF;
}

void idt_init(void)
{
    idtp.limit = (sizeof(struct idt_entry)*256)-1;
    idtp.base = (unsigned int)&idt;

    // Set gate for timer (IRQ 0 mapped to IDT index 32)
    idt_set_gate(32,(unsigned long)isr32_wrapper, 0x08 , 0x8E ); // Set gate for timer (IRQ 0 mapped to IDT index 32)

    // Set gate for keyboard (IRQ 1 mapped to IDT index 33)
    idt_set_gate(33,(unsigned long)isr33_wrapper, 0x08 , 0x8E );

     // Load IDT into CPU 
    __asm__ volatile ("lidt (%0)" : : "r"(&idtp));
}

void exception_handler(int vector)
{
    (void)vector; // To suppress unused varibale warning.
    panic("CPU exception occurred!!!!!");
}
  

