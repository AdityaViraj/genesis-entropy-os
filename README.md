# Entropy Compass

A simple baremetal operating system built entirely from scratch in C and Assembly.

**Entropy Compass** is an experimental Travel Roulette retro arcade game that challenges users to discover the world through a single key interface.

---

## Controls

* **Tap SPACE:** Spin the globe to unlock a random destinations.
* **Hold SPACE:** Reset your journey and return to the home screen.
* **Control + Option + G:** Release your mouse and exit the QEMU emulator.

---

## Core Concepts

* **Physical Memory Manager :** Also known as PMM this is built on a custom bitmap system to provide $O(1)$ allocation checks for physical memory frames.
* **Interrupt Pipeline:** Features a remapped 8259 PIC architecture and custom Assembly ISR (Interrupt Service Routine) wrappers to ensure low latency handling for hardware events.
* **Fault Tolerance:** It has a integrated custom `panic()` system for safe CPU halting and diagnostic reporting during critical processor exceptions.

---

## Project Structure

| File | Purpose |
| --- | --- |
| `src/kernel.c` | The kernel entry point that orchestrates hardware initialization and the main event loop. |
| `src/pmm.c/h` | In this we have the bitmap based Physical Memory Manage implemented  for tracking RAM frames. |
| `src/idt.c/h` | It is a Interrupt Descriptor Table setup for CPU exception handling. |
| `src/pic.c/h` | PIC remapping logic implemented to prevent IRQ/CPU exception conflicts. |
| `src/timer.c/h` | PIT (Programmable Interval Timer) driver for system timing. |
| `src/keyboard.c` | Keyboard driver for interrupt driven, low latency user input. |
| `src/isr_wrapper.s` | It is Assembly level boundary implemented for saving and restoring CPU states during hardware interrupts. |
| `src/io.c/h` | Low level hardware port communication (`inb` / `outb`). |
| `src/panic.c` | This manages kernelvlevel fault isolation and diagnostic halting. |
| `linker.ld` | This defines the precise kernel memory layout for the bootloader. |
| `Makefile` | This is an automated build system for compiling and linking the all the OS binary together. |

---

### Requirements 

Please ensure you have the following tools installed on your computer :

* `nasm`
* `gcc` (specifically the `x86_64-elf` cross compiler)
* `qemu-system-x86_64`

### Built and Run 

```bash

git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os
make clean && make
qemu-system-x86_64 -kernel genesis.bin -display cocoa,zoom-to-fit=on

```
