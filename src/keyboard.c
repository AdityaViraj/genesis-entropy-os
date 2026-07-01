// This is where we will handle the keyboard interrupts and process the scancodes received from the keyboard controller. The keyboard controller sends a scancode to the CPU when a key is pressed or released. We will read this scancode and perform appropriate actions based on it.

extern unsigned char inb(unsigned short port); // Function to read a byte from the specified I/O port. This is typically implemented in assembly and allows us to communicate with hardware devices.

volatile int space_pressed=0;
volatile int hold_counter=0;
volatile int exit_game=0;

extern void terminal_print(const char* str); //Functiion to print a string to the terminal. This is defined in kernel.c and allows us to display messages on the screen.

void keyboard_handler_c()
{
    unsigned char scancode = inb(0x60); 

    if(scancode == 0x39) // Spacebar Pressed 
    {
        hold_counter++;
        if(hold_counter>15)
        {
            exit_game=1;
        }
    }
    
        else if(scancode == 0xB9)
        {
            if(hold_counter>0 && hold_counter<15)
            {
                space_pressed=1;
            }
            hold_counter=0;
        }
}
    
    

