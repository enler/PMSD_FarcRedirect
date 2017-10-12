OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)

SECTIONS
{
  . = 0x921600;
  .text.start : {*(.text.start) }
  .text       : { *(.text*) }
  .rodata     : { *(.rodata) }
  .data       : { *(.data) }
  . = 0xA82BD0;
  .bss        : { *(.bss) }
}