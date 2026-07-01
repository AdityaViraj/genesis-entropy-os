/*
  To move on to handling input we need to understand Interrupts.
  When we press a key  the keyboard controller does not just wait for our code to ask "Did the user press anything?" that would be inefficient. Instead it sends an Interrupt Request (IRQ) to the CPU.
  The CPU instantly stops whatever it is doing jumps to a special function called an Interrupt Service Routine (ISR) processes the keypress and then returns to our code.

  THEORY :
  * The IDT (Interrupt Descriptor Table) : A table in memory that tells the CPU exactly where to jump when an interrupt happens. We just populate the table so the CPU knows which function to execute for keyboard events.
  * The PIC (Programmable Interrupt Controller) : A hardware chip that manages interrupts. We must 'remap' the PIC because its default its default settings conflict with modern CPU exceptions.
  * Scancodes : When we press a key the keyboard does not send 'A' or 'B' it sends a numeric scancode . Our driver must translate that number into a ASCII charecter.
*/

#include "idt.h"
#include "io.h"

// Define the IDT and pointer globally so they persist.
struct idt_entry idt[256];
struct idt_ptr idtp;

extern void keyboard_handler_asm(); // Assembly wrapper for the keyboard interrupt handler . Tells the compiler that this function is defined elsewhere (in assembly) and will be linked later. This allows us to call the assembly function from C code without getting a "function not found" error during compilation.

void idt_set_gate(unsigned char num,unsigned long base,unsigned short sel,unsigned char flags)
{
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFF;
}

void pic_init()
{
    outb(0x20,0x11);
    outb(0xA0,0x11);
    outb(0x21,0x20);
    outb(0xA1,0x28);
    outb(0x21,0x04);
    outb(0xA1,0x02);
    outb(0x21,0x01);
    outb(0xA1,0x01);
    outb(0x21,0xFD);
    outb(0xA1,0xFF);
}

void idt_init(void)
{
    idtp.limit = (sizeof(struct idt_entry)*256)-1;
    idtp.base = (unsigned int)&idt;

    // Set gate for keyboard (IRQ 1 mapped to IDT index 33)
    idt_set_gate(33,(unsigned long)keyboard_handler_asm, 0x08 , 0x8E );

     // Load IDT into CPU 
    __asm__ volatile ("lidt (%0)" : : "r"(&idtp));

    pic_init(); // Intialise PIC
    __asm__ volatile("sti"); //Enable interrupts in CPU
}

    

   

/*
We will need to :
* Write an Assembly 'wrapper' that saves the CPU state before calling our C code.
* Write a C function to handle the keyboard interrupt (reading from port 0x60)
* Load the IDT into the CPU using the lidt instuction.
*/

/*
    __asm__ : This is a standard GCC extension for inline assembly. A single undeerscire _asm is often treated as a plain variable name by compilers.
    Symbol Scope : In C a function cannot see a variable unless it is defines globally or passed as an argument. By defining struct idt_entry idt[256]; globally in idt.c , the idt_intit function will be able to see it.

*/