// This provides the actual implementation of the compiler

void terminal_print(const char* str)
{
    volatile char* video_memory=(volatile char*)0xb8000;
    static int offset=0;

    for(int i=0;str[i]!='\0';i++)
    {
        video_memory[offset]=str[i];
        video_memory[offset+1]=0x07;
        offset+=2;
    }
}

void outb(unsigned short port, unsigned char val) 
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) 
{
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
