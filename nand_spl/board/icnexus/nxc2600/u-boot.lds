OUTPUT_ARCH(mips)
ENTRY(_start)
MEMORY
{
	ram	: ORIGIN = 0x80000000 , LENGTH = 0x100000
}

SECTIONS
{
	. = ALIGN(4);
	.text : { *(.text*) } > ram

	. = ALIGN(4);
	.rodata : { *(.rodata*) } > ram

	. = ALIGN(4);
	.sdata : { *(.sdata*) } > ram

	. = ALIGN(4);
	.data : { *(.data*) *(.reginfo*) } > ram

	_gp = ALIGN(16);
	.got : { *(.got*) } > ram

	. = ALIGN(4);
	.sbss : { *(.sbss*) } > ram
	.bss : { *(.bss*) } > ram
	. = ALIGN (4);
}

