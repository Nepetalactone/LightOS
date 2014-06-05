////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
//
// Basic configuration using only external memory
//

-stack           0x00002000
-heap            0x00F00000

MEMORY
{
	int_ram:	ORIGIN = 0x40200000		LENGTH = 0x0FFFFFFF
	ext_ddr:	ORIGIN = 0x82000000		LENGTH = 0x02000000
	ext_ddr_extended:	ORIGIN = 0x84000000		LENGTH = 0x0FFFFFFF
	//ext_ddr_os:	ORIGIN = 0x9
}

SECTIONS
{
	.const      > ext_ddr
	.bss        > ext_ddr
	.far        > ext_ddr

	.const		> ext_ddr_extended
	.bss		> ext_ddr_extended
	.far		> ext_ddr_extended

	.stack      > ext_ddr
	.data       > ext_ddr
	.cinit      > ext_ddr
	.cio        > ext_ddr

	.stack      > ext_ddr_extended
	.data       > ext_ddr_extended
	.cinit      > ext_ddr_extended
	.cio        > ext_ddr_extended

	.text       > ext_ddr
	.sysmem     > ext_ddr

	.text       > ext_ddr_extended
	.sysmem     > ext_ddr_extended
	.intvecs    > int_ram
	.irqstack	> ext_ddr
	.abortstack	> ext_ddr
	.svcstack	> ext_ddr
}
