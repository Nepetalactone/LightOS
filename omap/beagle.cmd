////////////////////////////////////////////////////////
// Linker command file for BeagleBoard)
// 
// Basic configuration using only external memory
//

-stack           0x00002000
-heap            0x00002000

MEMORY
{
   int_ram:  ORIGIN = 0x40200000  LENGTH = 0x0000FFFF
   ext_ddr:  ORIGIN = 0x82000000  LENGTH = 0x1FFFFFFF
}

SECTIONS
{
   .const      > ext_ddr
   .bss        > ext_ddr
   .far        > ext_ddr

   .stack      > ext_ddr
   .data       > ext_ddr
   .cinit      > ext_ddr
   .cio        > ext_ddr

   .text       > ext_ddr
   .sysmem     > ext_ddr
   .switch     > ext_ddr

   ._irq_stack > ext_ddr
   ._abort_stack > ext_ddr
   ._intvecs_addr > int_ram
   ._kernel_stack > ext_ddr
   ._system_stack > ext_ddr
}
