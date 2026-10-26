/*
 * =============================================================================
 *  stm32f446re.ld  —  Linker script
 * =============================================================================
 *  Tells the linker WHERE in the chip's memory each part of the program goes.
 *  You don't need to edit this for any challenge.
 *
 *    FLASH (512 KB, starts at 0x08000000)  ->  your code + constants
 *    RAM   (128 KB, starts at 0x20000000)  ->  variables + the stack
 *
 *  Addresses come from RM0390 Section 2.2.2 (memory map).
 * =============================================================================
 */

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* The stack starts at the very top of RAM and grows downward. */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* The vector table MUST be first in flash: on reset the CPU reads the
     * initial stack pointer from 0x08000000 and the reset handler address
     * from 0x08000004. */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code and read-only data (constants, string literals). */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .preinit_array : { PROVIDE_HIDDEN(__preinit_array_start = .); KEEP(*(.preinit_array*)); PROVIDE_HIDDEN(__preinit_array_end = .); } > FLASH
    .init_array    : { PROVIDE_HIDDEN(__init_array_start = .);    KEEP(*(SORT(.init_array.*))); KEEP(*(.init_array*)); PROVIDE_HIDDEN(__init_array_end = .); } > FLASH
    .fini_array    : { PROVIDE_HIDDEN(__fini_array_start = .);    KEEP(*(SORT(.fini_array.*))); KEEP(*(.fini_array*)); PROVIDE_HIDDEN(__fini_array_end = .); } > FLASH

    /* Where the startup code finds the initial values of .data in flash. */
    _sidata = LOADADDR(.data);

    /* Initialized variables (int x = 5;). They live in RAM while running,
     * but their starting values are stored in flash and copied over by
     * Reset_Handler before main() runs. */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /* Zero-initialized variables (int count;). Reset_Handler fills with 0. */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    /* Start of free RAM (used by malloc, if anyone ever calls it). */
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
