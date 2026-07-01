/*
 An .ld file is a GNU Linker used in software development and embedded systems (like ARM Cortex-M mircocontrollers) to dicatate how compiled code and data are mapped into device's physical memory.
 It tells the linker exactly where to place code(FLASH) and variables(RAM)
 
 A linker script is a configuration text file that instructs a software linker on how to map a program's code and data sections into physical memory regions.

 Linker Scripts are extremly picky so be careful !
*/

ENTRY(start)

SECTIONS
{
    /* The Kernel starts at 1MB (which is standard for multiboot) */
    . = 1M;

    .text : ALIGN(4k) { 
        *(.multiboot)
        *(.text)
    }

    .rodata : ALIGN(4k){
        *(.rodata)
    }

    .data : ALIGN(4k) {
        *(.data)
    }

    .bss : ALIGN(4k) {
        *(COMMON)
        *(.bss)
    }
}

/* 
We placed the kernel at 1MB in RAM. This is the safe zone where bootloaders expect kernel to live
ALIGN(4k) : This ensures sections start on clean memoryy boundaries which ELF linkers appreciate. 
*/
