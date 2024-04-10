ENTRY(start)
MEMORY
{
    RAM (rw) : ORIGIN = 0x80000000, LENGTH = 128K
    FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 256K
}
SECTIONS
{
    . = 0xffff000000080000;

    .text : {
        *(.text)
    } >FLASH

    .rodata : {
        *(.rodata)
    } >FLASH

    . = ALIGN(16);
    .data : {
        *(.data)
        . =  ALIGN(8);
    } >RAM AT>FLASH

    .bss : {
        bss_start = .;
        *(.bss)
        bss_end = .;
    } >RAM

    end = .;
}