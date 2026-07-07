// To handle system halts

#include "io.h"
#include "panic.h"

extern void terminal_print(const char* str);

void panic(const char* message)
{
    __asm__ volatile("cli");

    terminal_print("\n KERNEL PANIC !! \n");
    terminal_print("Reason:");
    terminal_print(message);

    while(1)
    {
        __asm__ volatile("hlt");
    }
}