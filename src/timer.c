// Create a new file for our timer. This code interacts directly with the motherboard's I/O ports.                 

#include "io.h" // Required to use outb() for hardware port communication

// Define the hardwarefixed base frquency of the PIT chip
#define PIT_BASE_FREQ 1193182 

// Global variabel tracking kernel uptime
volatile unsigned int system_ticks=0;

// Function to intialise the timer to a specific tick
void init_timer(unsigned int freq)
{
    
    // Calculate the 16 bit hardware divisor needed to achecive to target frequency
    unsigned int divisor = PIT_BASE_FREQ / freq;

    //Send command byte 0x36 to PIT Command Port 0x43
    // 0x36 means Channel 0 expect LSB then MSB , Square Wave Mode , 16-bit binary counter.
    outb(0x43,0x36);

    //Isolate the lower 8 bits (The least signicifcant Bytes) of our 16 bit divisor
    unsigned char low_byte = (unsigned char)(divisor & 0xFF);

    //Isloate the upper 8 bits (Most Significant Byte) of our 16 bit divisor
    unsigned char high_byte = (unsigned char)((divisor>>8) & 0xFF);

    //Push the low byte to Channel 0 Data Port (0x40)
    outb(0x40, low_byte);

    //Push the high byte to Channel 0 Data port (0x40)
    outb(0x40, high_byte);
}

void timer_handler()
{
    system_ticks++; // Increment the global tick counter
    outb(0x20, 0x20); // Send End of Interrupt (EOI) signal to the PIC to indicate that we have finished processing the timer interrupt. This allows the PIC to send further interrupts.
}