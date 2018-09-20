OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reloc)

MEMORY
{
	ITCM	(RWX) : ORIGIN = 0x01FF8000, LENGTH = 32K
	VECTOR	(RX)  : ORIGIN = 0x08000000, LENGTH = 0x40
	WRAM	(RWX) : ORIGIN = 0x08006000, LENGTH = 0xFA000
	DTCM	(RW)  : ORIGIN = 0x40000000, LENGTH = 16K
}

SECTIONS
{
	.text.reloc : ALIGN(16)
	{
		KEEP(*(.text.reloc))
		. = ALIGN(16);
	} >WRAM

	.vector : ALIGN(16)
	{
		__vector_lma = LOADADDR(.vector);
		__vector_s = ABSOLUTE(.);
		KEEP(*(.vector))
		. = ALIGN(16);
		__vector_e = ABSOLUTE(.);
	} >VECTOR AT>WRAM

	.text : ALIGN(16)
	{
		__text_lma = LOADADDR(.text);
		__text_s = ABSOLUTE(.);
		*(.text*)
		. = ALIGN(16);
		__text_e = ABSOLUTE(.);
	} >ITCM AT>WRAM

	.data : ALIGN(16)
	{
		__data_lma = LOADADDR(.data);
		__data_s = ABSOLUTE(.);
		*(.rodata*)
		*(.data*)
		. = ALIGN(16);
		__data_e = ABSOLUTE(.);
	} >DTCM AT>WRAM

	.bss (NOLOAD) : ALIGN(16)
	{
		__bss_s = ABSOLUTE(.);
		*(.bss*)
		. = ALIGN(16);
		__bss_e = ABSOLUTE(.);
	} >DTCM

	. = ALIGN(16);
}
