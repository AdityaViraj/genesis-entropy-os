ENTRY(start)

SECTIONS
{
    /* Force placement at 1MB */
    . = 1M;

    /* Explicitly define the header location */
    .text : ALIGN(4K)
    {
        /* Force the multiboot section to be the very first bytes of this section */
        KEEP(*(.multiboot))
        *(.text)
    }

    .rodata : ALIGN(4K) { *(.rodata) }
    .data : ALIGN(4K) { *(.data) }
    .bss : ALIGN(4K) { *(COMMON) *(.bss) }

    _kernel_end = .;
}