#include "address.h"

/*
OMAP35x technical reference manual pdf
Overview p.201
*/


/*
General information

*_S = Start address
*_E = End address

*/


/*  Q0  total size = 1GB  */

/* Boot space */
//boot space , size = 1 GB
#define BOOTGPMC_S (address) 0x00000000
#define BOOTGPMC_E (address) 0x3FFFFFFF


/*  Q1  total size = 128MB  */

/* ON CHIP MEMORY */
//boot ROM internal, size = 32KB
#define BOOT_ROM_S (address) 0x40014000
#define BOOT_ROM_E (address) 0x4001BFFF

//SRAM internal, size = 64KB
#define SRAM_S (address) 0x40200000
#define SRAM_E (address) 0x4020FFFF


/* L4 INTERCONNECTS */
//L4 Core, size = 16MB
#define L4CORE_S (address) 0x48000000
#define L4CORE_E (address) 0x48FFFFFF

//L4 Wake-up, size = 256KB
#define L4WAKEUP_S (address) 0x48300000
#define L4WAKEUP_E (address) 0x4833FFFF

//L4 Per, size = 1MB
#define L4PER_S (address) 0x49000000
#define L4PER_E (address) 0x490FFFFF

/* SGX */
//SGX, size = 64KB
#define SGX_S (address) 0x50000000
#define SGX_E (address) 0x5000FFFF

/* L4 EMULATION */
//L4 Emulation, size = 8MB
#define L4EMU_S (address) 0x54000000
#define L4EMU_E (address) 0x547FFFFF

/* IVA2.2 Subsystem */
//IVA 2.2 Subsystem, size = 48MB
#define IVASUBSYS_S (address) 0x5C000000
#define IVASUBSYS_E (address) 0x5EFFFFFF


/* L3 INTERCONNECTS */
//L3 control register, size = 16MB
#define L3CONTROLREG_S (address) 0x68000000
#define L3CONTROLREG_E (address) 0x68FFFFFF

//SMS registers, size = 16MB
#define SMS_S (address) 0x6C000000
#define SMS_E (address) 0x6CFFFFFF

//SDRC registers, size = 16MB
#define SDRC_S (address) 0x6D000000
#define SDRC_E (address) 0x6DFFFFFF

//GPMC registers, size = 16MB
#define L3GPMC_S (address) 0x6E000000
#define L3GPMC_E (address) 0x6EFFFFFF

/* SDRC/SMS */
//SDRC/SMS virtual address space 0, size = 256MB
#define SDRC_SMS_VIRT_S (address) 0x70000000
#define SDRC_SMS_VIRT_E (address) 0x7FFFFFFF

/*  Q2 total size = 1GB  */

/* SDRC/SMS */
//CS0-SDRAM, size = 512MB
#define CS0_SDRAM_S (address) 0x80000000
#define CS0_SDRAM_E (address) 0x9FFFFFFF

//CS1-SDRAM, size = 512MB
#define CS1_SDRAM_S (address) 0xA0000000
#define CS1_SDRAM_E (address) 0xBFFFFFFF

/*  Q3 total size = 1GB  */

/* SDRC/SMS */
//SDRC/SMS virtual address space 1, size = 512MB
