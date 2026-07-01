# Compiler and Flags
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -c
LDFLAGS = 

# Files
SOURCES = src/kernel.c
OBJECTS = arch/boot.o src/kernel.o src/isr_wrapper.o src/idt.o src/keyboard.o src/io.o

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

# Link everything together
genesis.bin: arch/boot.o src/kernel.o src/idt.o src/isr_wrapper.o src/keyboard.o src/io.o linker.ld
		$(LD) -m elf_i386 -T linker.ld -o genesis.bin arch/boot.o src/kernel.o src/idt.o src/isr_wrapper.o src/keyboard.o src/io.o

clean:
		rm -f arch/*.o src/*.o *.bin

# -ffreestanding : Tells GCC that we are in a environement without standard library.
# -nostdlib : Explicitly tells the linker not to use the host systems C library.
# -f elf64 : We are targetting 64 bit architecture
# -T linker.ld : We are forcing the linker to use the memory map we wrote earlier.

# We use gcc to link , we are telling the compiler "Hey use your internal knowledge to map these flags correctly for the environment"
# The systems's raw ld (the one we are calling) is designed for linking macos applications , not raw kernel binaries. Calling it directly with Linux specific flags will always cause ukown option error

# -Wl, : This is a magical prefix. It tells the compiler (GCC/Clang) "Hey take everything after this comman and pass it directly to the underlying linker
# By writing , -Wl,-T,linker.ld you are tellin the compiler to stop trying to parse the -T flag itself and just pass it to the linker which does know how to handle it.

# x86_64-elf tools are designed specifically for building kernels that target 'bare metals'
# These tools support -T and -ffreestanding natively because they aren't trying to build macOS apps




