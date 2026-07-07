// This is where we will handle the keyboard interrupts and process the scancodes received from the keyboard controller. The keyboard controller sends a scancode to the CPU when a key is pressed or released. We will read this scancode and perform appropriate actions based on it.

#include "io.h"

#define KEYBOARD_DATA_PORT 0x60 // Port to read scancode from keyboard controller
#define PIC1_COMMAND_PORT 0x20 // Port to send commands to the master PIC


// External References
extern unsigned char inb(unsigned short port); // Function to read a byte from the specified I/O port. This is typically implemented in assembly and allows us to communicate with hardware devices.
extern void outb(unsigned short port,unsigned char data);
extern void terminal_print(const char* str); 

// Global State variables
volatile int space_pressed=0;
volatile int hold_counter=0;
volatile int exit_game=0;

void keyboard_handler()
{
    // Read the scancode from the keyboard data port
    unsigned char scancode = inb(KEYBOARD_DATA_PORT); 

    // Check if the scancode corresponds to the spacebar being pressed or released
    if(scancode == 0x39) 
    {
        hold_counter++;
        if(hold_counter>15)
        {
            exit_game=1;
            terminal_print(">>> LONG PRESS DETECTED: EXITING GAME <<<");
        }
    }
    
    else if(scancode == 0xB9)
    {
        if(hold_counter>0 && hold_counter<15)
        {
            space_pressed=1;
            terminal_print(">>> SPACEBAR PRESSED <<<");
        }
            hold_counter=0; // Reset counter on release
    }   
    // Send End of Interrupt (EOI) signal to the PIC to indicate that we have finished processing the keyboard interrupt. This allows the PIC to send further interrupts.
    outb(PIC1_COMMAND_PORT,0x20); 
}




