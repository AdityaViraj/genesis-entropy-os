UNAME_S := $(shell uname -s)
AS = nasm
QEMU = qemu-system-x86_64

ifeq ($(UNAME_S),Darwin)
    # macOS has no native ELF toolchain, so we use the
    # x86_64-elf-gcc cross compiler from Homebrew.
    CC = x86_64-elf-gcc
    LD = x86_64-elf-ld
    QEMU_DISPLAY = -display cocoa,zoom-to-fit=on
else
    # Linux (and Linux-inside-WSL) already produces ELF binaries,
    # so the system gcc/ld work fine with -m32.
    CC = gcc
    LD = ld
    QEMU_DISPLAY = -display gtk,zoom-to-fit=on
endif

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector -c
LDFLAGS =

SOURCES = src/kernel.c
OBJECTS = arch/boot.o src/kernel.o src/isr_wrapper.o src/idt.o src/keyboard.o src/io.o src/timer.o src/pmm.o src/pic.o src/panic.o

.PHONY: all run clean

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
# $@ is a special automatic variable that represents the name of the target file

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
genesis.bin: $(OBJECTS) linker.ld
	$(LD) -m elf_i386 -T linker.ld -o genesis.bin $(OBJECTS)

# Build and immediately launch in QEMU (uses the right display backend for your OS automatically)
run: all
	$(QEMU) -kernel genesis.bin $(QEMU_DISPLAY)

clean:
	rm -f arch/*.o src/*.o *.bin
