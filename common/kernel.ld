/**
 * Linker script to generate RPI kernel image.
 */

SECTIONS {
    /* Startup section. The bootloader will put this coded at 0x8000. */
    .init 0x8000 : {
        *(.init)
    }

    /* Code */
    .text : {
        *(.text)
    }

    /* Data */
    .data : {
        *(.data)
    }

    /* Read-only data */
    .rodata : {
        *(.rodata)
    }

    /* zero-initialized data */
    .bss : {
        *(.bss)
    }

    /* Throw away everything else */
    /DISCARD/ : {
        *(*)
    }
}
