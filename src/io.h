#ifndef IO_H
#define IO_H

void terminal_print(const char* str);
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

#endif