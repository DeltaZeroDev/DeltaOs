OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
	{
		. = 0x100000;
		.text : {
				. = ALIGN(8);
				KEEP(*(.multiboot))
				*(.text)
			}
		.data :{ *(.data) }
		.bss : { *(.bss) }
	}