#include "io.h"

// The PIC Ports
#define PIC1_COMMAND 0x20 // Master PIC Command Port
#define PIC1_DATA 0x21 // Master PIC Data Port
#define PIC2_COMMAND 0xA0 // Slave PIC command Port
#define PIC2_DATA 0xA1 // Slave PIC data port   

// Intialisation Command Word 1 (ICW!)
// 0x11 = intialize command (0x10) + indicate that ICW4 will be needed (0x01)
#define ICW1_INIT   0x11

// Intialisation Command Word 4 (ICW4)
#define ICW4_8086   0x01

void remap_pic()
{
    // 1. Start the intilisation sequence in casacade mode
    outb(PIC1_COMMAND,ICW1_INIT); // Wake up Master PIC
    outb(PIC2_COMMAND,ICW1_INIT); // Wake up Slave PIC
    
    // 2. ICW2 : Vector Offsets (This is the actual remapping!)
    outb(PIC1_DATA,0x20); //Master PIC vectors start at 32 (0x20)
    outb(PIC2_DATA,0x20); //Slave PIC vectors starts at 40 (0x28)
    
    // 3. ICW3 : Cascade setup (Tells the PICs how they are wired together)
    outb(PIC1_DATA,4);
    outb(PIC2_DATA,2);

    // 4. ICW4 : Environment Setup
    outb(PIC1_DATA,ICW4_8086); // Set Master to x86 mode
    outb(PIC2_DATA,ICW4_8086); // Set Slave to x86 mode

    // 5. Unmask interrupts by writing 0 to data ports
    outb(PIC1_DATA,0xFF);
    outb(PIC2_DATA,0xFF);
}

