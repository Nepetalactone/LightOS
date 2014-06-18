/*
 * loader.c
 *
 *  Created on: 13.06.2014
 *      Author: phil
 */


#include "loader.h"

#include "../sdcard/fat32.h"
#include "../scheduler/scheduler.h"
#include "../mmu/mmu.h"
#include "../../hal/omap3530/mmu/hal_mmu_omap3530.h"

#include <string.h>
#include <strings.h>

// module-local defines
#define EI_CLASS_32_BIT			0x1
#define E_TYPE_EXECUTABLE		0x2
#define E_MACHINE_ARM			0x28

#define PT_LOAD 				1

#define PF_X 					0x1
#define PF_W 					0x2
#define PF_R 					0x4

///////////////////////////////////////////////////////////////

// module-local structures
typedef struct
{
	uint8_t	e_identMagic[ 4 ];
	uint8_t	e_identClass;
	uint8_t e_identData;
	uint8_t e_identVersion;

	uint8_t e_identOsabi;
	uint8_t e_identAbiVer;

	uint8_t e_identPad[ 7 ];

	uint16_t e_type;
	uint16_t e_machine;

	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} ELF_HEADER;

typedef struct
{
	uint32_t sh_name;                	/* Section name (string tbl index) */
	uint32_t sh_type;                	/* Section type */
	uint32_t sh_flags;              	/* Section flags */
	uint32_t sh_addr;           		/* Section virtual addr at execution */
	uint32_t sh_offset;             	/* Section file offset */
	uint32_t sh_size;                	/* Section size in bytes */
	uint32_t sh_link;                	/* Link to another section */
	uint32_t sh_info;                	/* Additional section information */
	uint32_t sh_addralign;           	/* Section alignment */
	uint32_t sh_entsize;             	/* Entry size if section holds table */
} SECTION_HEADER;

typedef struct
{
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;					/* Segment virtual address */
	uint32_t p_paddr;					/* Segment physical address */
	uint32_t p_filesz;					/* Segment size in file */
	uint32_t p_memsz;					/* Segment size in memory */
	uint32_t p_flags;
	uint32_t p_align;
} PROGRAM_HEADER;
///////////////////////////////////////////////////////////////

// module-local data
static uint8_t eIdentMagicNumber[ 4 ] = { 0x7F, 'E', 'L', 'F' };
///////////////////////////////////////////////////////////////

// module-local functions
uint32_t getVirtualAddressOfSection( const char* sectionName, ELF_HEADER* elfHeader, uint8_t* dataBuffer );
///////////////////////////////////////////////////////////////

uint32_t
loadTaskFromFile( const char* fileName )
{
	uint32_t i = 0;
	uint32_t fileSize = 0;
	uint8_t* fileBuffer = 0;
	file_id taskImageFile = 0;
	ELF_HEADER* elfHeader = 0;
	uint32_t stackPointerAddress = 0;

	process_t* task = 0;

	if ( fat32Open( fileName, &taskImageFile ) )
	{
		return 1;
	}

	fat32Size( taskImageFile, &fileSize );

	//fileBuffer = ( uint8_t* ) kmalloc( fileSize );

	if ( 0 == fileBuffer )
	{
		return 1;
	}

	// read the elf-header to check for validity
	int32_t ret = fat32Read( taskImageFile, sizeof( ELF_HEADER ), fileBuffer );
	if ( sizeof( ELF_HEADER ) != ret )
	{
		ret = 1;
		goto closeAndExit;
	}

	ret = 1;

	elfHeader = ( ELF_HEADER* ) fileBuffer;

	// ignore invalid ELF-file
	if ( 0 != memcmp( elfHeader->e_identMagic, eIdentMagicNumber, 4 ) )
	{
		goto closeAndExit;
	}

	// accept only 32-bit images
	if ( EI_CLASS_32_BIT != elfHeader->e_identClass )
	{
		goto closeAndExit;
	}

	// accept only excecutable images
	if ( E_TYPE_EXECUTABLE != elfHeader->e_type )
	{
		goto closeAndExit;
	}

	// accept only ARM ISA machines
	if ( E_MACHINE_ARM != elfHeader->e_machine )
	{
		goto closeAndExit;
	}

	// subtract from bytes left to read the header-size
	fileSize -= sizeof( ELF_HEADER );

	// read rest of the file into buffer
	ret = fat32Read( taskImageFile, fileSize, &fileBuffer[ sizeof( ELF_HEADER ) ] );
	if ( ret != fileSize )
	{
		ret = 1;
		goto closeAndExit;
	}

	// need to know the stack-pointer to assign it to R13 in initialization
	stackPointerAddress = getVirtualAddressOfSection( ".stack", elfHeader, fileBuffer );
	// no stackpointer found, invalid linkage
	if ( 0 == stackPointerAddress )
	{
		ret = 1;
		goto closeAndExit;
	}

	// create task in scheduler
	//task = createTask( elfHeader->e_entry, stackPointerAddress );
	task = process_create("FROMSD",(void*)elfHeader->e_entry);
	if ( 0 == task )
	{
		ret = 1;
		goto closeAndExit;
	}

	// switch context to the pagetable of the task and set pid in TLB
	//mmu_ttbSet( ( int32_t ) task->pageTable );
	//mmu_setProcessID( task->pid );
	hal_mmu_set_ttbr_0_asm((uint32_t) task->pt_address, task->pID);

	// iterate all programheaders and load them
	for ( i = 0; i < elfHeader->e_phnum; ++i )
	{
		PROGRAM_HEADER* programHeader = ( PROGRAM_HEADER* ) &fileBuffer[ elfHeader->e_phoff + ( i * elfHeader->e_phentsize ) ];

		// only load program-headers which are loadable
		if ( PT_LOAD != programHeader->p_type )
		{
			continue;
		}

		uint32_t mapSize = 0;
		int32_t memDelta = programHeader->p_memsz - programHeader->p_filesz;

		// if size in file is not 0, take this as a lower bound for mapping-size
		if ( 0 != programHeader->p_filesz )
		{
			mapSize = programHeader->p_filesz;
		}

		// if size in memory is not 0, take this as an upper bound for mapping-size, as it is at least the size of p_filesz if not 0
		if ( 0 != programHeader->p_memsz )
		{
			mapSize = programHeader->p_memsz;
		}

		// TODO: p_vaddr has to be a multiple of page-size otherwise it could be that it will be mapped to the same address

		//TODO: mmu_map_memory( task, programHeader->p_vaddr, mapSize );

		// if size in file is not 0, there is some data to copy
		if ( 0 != programHeader->p_filesz )
		{
			memcpy( ( uint32_t* ) programHeader->p_vaddr, &fileBuffer[ programHeader->p_offset ], programHeader->p_filesz );
		}

		// check if there is some data left to be set to 0
		if ( 0 < memDelta )
		{
//			memset( &( ( uint8_t* ) programHeader->p_vaddr )[ programHeader->p_filesz ], 0, memDelta );
		}
	}

	ret = 0;

closeAndExit:
	fat32Close( taskImageFile );

	if ( fileBuffer )
	{
		free( fileBuffer );
	}

	return ret;
}

uint32_t
getVirtualAddressOfSection( const char* sectionName, ELF_HEADER* elfHeader, uint8_t* dataBuffer )
{
	uint32_t i = 0;

	SECTION_HEADER* stringTableEntry = ( SECTION_HEADER* ) &dataBuffer[ elfHeader->e_shoff + ( elfHeader->e_shstrndx * elfHeader->e_shentsize ) ];
	const char* stringTable = stringTable = ( const char* ) &dataBuffer[ stringTableEntry->sh_offset ];

	for ( i = 0; i < elfHeader->e_shnum; ++i )
	{
		SECTION_HEADER* sectionHeader = ( SECTION_HEADER* ) &dataBuffer[ elfHeader->e_shoff + ( i * elfHeader->e_shentsize ) ];

		if ( sectionHeader->sh_name )
		{
			if ( 0 == strcasecmp( &stringTable[ sectionHeader->sh_name ], sectionName ) )
			{
				return sectionHeader->sh_addr;
			}
		}
	}

	return 0;
}
