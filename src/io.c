// This provides the actual implementation of the compiler

/* src/io.c */
void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
/*
   extern only tells the compiler : 'Trust me this function exists somehwere else , jusr compile this file for now'
   The linker is the one that actually hunts for the defination. If it cannot find the machine code for inb in any of the compiled .o files it fails with that "undefined reference" error
*/