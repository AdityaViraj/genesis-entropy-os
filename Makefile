# Compiler and Flags
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -c
LDFLAGS = 
# Files
SOURCES = src/kernel.c
OBJECTS = arch/boot.o src/kernel.o src/isr_wrapper.o src/idt.o src/keyboard.o src/io.o src/timer.o src/pmm.o src/pic.o src/panic.o 

all: genesis.bin

# Assemble the bootloader
arch/boot.o: arch/boot.s
		$(AS) -f elf32 $< -o $@

# Compile the kernel 
src/kernel.o: src/kernel.c
		$(CC) $(CFLAGS) $< -o $@

# Compile idt.c
src/idt.o: src/idt.c
		$(CC) $(CFLAGS) $< -o $@

src/isr_wrapper.o: src/isr_wrapper.s
		$(AS) -f elf32 $< -o $@
# $@ is a special automatic variable automatic variable that represents that represents the name od the target file

src/keyboard.o: src/keyboard.c
		$(CC) $(CFLAGS) $< -o $@

src/io.o: src/io.c
		$(CC) $(CFLAGS) $< -o $@

src/timer.o: src/timer.c
		$(CC) $(CFLAGS) $< -o $@

src/pmm.o: src/pmm.c
		$(CC) $(CFLAGS) $< -o $@

src/pic.o: src/pic.c
		$(CC) $(CFLAGS) $< -o $@

src/panic.o: src/panic.c
		$(CC) $(CFLAGS) $< -o $@


# Link everything together
genesis.bin: arch/boot.o src/kernel.o src/idt.o src/isr_wrapper.o src/keyboard.o src/io.o src/timer.o src/pmm.o src/pic.o src/panic.o linker.ld
		$(LD) -m elf_i386 -T linker.ld -o genesis.bin arch/boot.o src/kernel.o src/idt.o src/isr_wrapper.o src/keyboard.o src/io.o src/timer.o src/pmm.o src/pic.o src/panic.o

clean:
		rm -f arch/*.o src/*.o *.bin
