// Create header file to define the table structure
// Header file - It contains the declarations for functions , data types , macros and constents. It acts as an interface between different source files , allowing us to share code and reuse functions across multiple components withour rewriting them.

/*
    Why Header file are used ?
    * Declaration vs Defination - In C , the compiler compiles one source file at a time. It needs to know a functions's  signature (return type and parameters) before we can call it.Header files provide these declarations while the actual implementation  code lives seperately in .c files or pre compiled libraries
    * Code Resusability : We write function declararions once in a .h file and import them wherever needed,
    * Code maintainance : If a function signature changes , we only update 
    
    <stdio.h> : Input/Output operations like printf() and scanf()
    <stdlib.h> : Memory management (malloc(),free()) and process control
    <string.h> : String manipulation like strlen() and strcpy()
    <math.h> : Common mathmatical functions like sqrt() and pow()
*/

#ifndef IDT_H
#define IDT_H

struct idt_entry
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
}
__attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
}
__attribute__((packed));

void idt_init(void);

#endif 
 
/*
         __attribute__((packed)) : This is critical. It forces the compiler to store the structures exactly as defines without adding 'padding' bytes that would break the CPUs expected memory layout
         lidt : This is a priviliges CPU instrcution. We cannot run this in a standard macOS/Linux program it only requires a bare metal processor
            idt_set_gate : This function is used to set up an entry in the IDT. It takes the following parameters :
            * num : The index in the IDT where we want to set the entry
            * base : The address of the interrupt handler function that should be called when this interrupt occurs
            * sel : The code segment selector for the interrupt handler. This is usually set to 0x08 which corresponds to the kernel code segment in our GDT
            * flags : The type and attributes of the interrupt gate. 0x0E indicates a 32-bit interrupt
            * The function then populates the corresponding idt_entry structure with these values, splitting the base address into low and high parts and setting the appropriate flags.
*/