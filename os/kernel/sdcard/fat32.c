/*
 * fat32.c
 *
 *  Created on: 11.06.2014
 *      Author: phil
 *  (c) Jonathan Thaler
 */
#include "fat32.h"

#include "sd_hal.h"

#include <string.h>
#include <strings.h>
#include <ctype.h>
/**
 * Sources for implementation:
 * Microsoft fat32 spec
 * https://www.pjrc.com/tech/8051/ide/fat32.html
 * http://wiki.osdev.org/FAT
 * http://www.easeus.com/resource/fat32-disk-structure.htm
 */

#define MAX_FILE_DESCRIPTORS			16

#define SD_CARD_BLOCK_SIZE				512

#define MBR_AND_BPS_SIZE				0x200

#define MBR_SIGNATURE_VALUE				0xAA55
#define MBR_SIGNATURE_OFFSET			0x1FE
#define PRIMARY_PARTITION_OFFSET		0x1BE

#define PRIMARY_PARTITION_FAT32_TYPE	0xB

#define FAT32_MIN_CLUSTER_COUNT			65525

#define DIRECTORY_UNUSED 			0xE5
#define DIRECTORY_END				0x0

#define DIR_ATTRIB_READONLY			0x1
#define DIR_ATTRIB_HIDDEN			0x2
#define DIR_ATTRIB_SYSTEM			0x4
#define DIR_ATTRIB_VOLID			0x8
#define DIR_ATTRIB_LONG_NAME		0xF
#define DIR_ATTRIB_DIRECTORY		0x10
#define DIR_ATTRIB_ARCHIVE			0x20

#define IS_READONLY( attrib ) 		( DIR_ATTRIB_READONLY & attrib )
#define IS_HIDDEN( attrib )			( DIR_ATTRIB_HIDDEN & attrib )
#define IS_SYSTEM( attrib )			( DIR_ATTRIB_SYSTEM & attrib )
#define IS_VOLUMEID( attrib )		( DIR_ATTRIB_VOLID & attrib )
#define IS_LONGNAME( attrib ) 		( ( DIR_ATTRIB_LONG_NAME & attrib ) == DIR_ATTRIB_LONG_NAME )
#define IS_DIRECTORY( attrib )		( DIR_ATTRIB_DIRECTORY & attrib )
#define IS_FILE( attrib )			( DIR_ATTRIB_ARCHIVE & attrib )


// module-local structures
typedef struct
{
	uint8_t entryState;
	uint8_t startHead;
	uint16_t startCylinder;
	uint8_t type;
	uint8_t endHead;
	uint16_t endCylinder;
	uint32_t partitionStart_lba;
	uint32_t numbersOfSectors;
} PRIMARY_PARTITION_INFO_STRUCT;

// NOTE: using packed AND enabling --unaligned-access=on in compiler options doesnt help:
// unaligned access will lead to data-abort interruptions => insert alignmentbytes by hand
// and copy piece-wise
typedef struct
{
	uint8_t		bootjmp[ 3 ];
	uint8_t		oem_name[ 8 ];

	// align next to 12
	uint8_t		__alignmentByte1;

	uint16_t	bytes_per_sector;
	uint8_t		sectors_per_cluster;

	// align next to 16
	uint8_t		__alignmentByte2;

	uint16_t	reserved_sector_count;
	uint8_t		number_fats;

	// align next to 20
	uint8_t		__alignmentByte3;

	uint16_t	root_entry_count;
	uint16_t	total_sectors_16;
	uint8_t		media_type;

	// align next to 26
	uint8_t		__alignmentByte4;

	uint16_t	table_size_16;
	uint16_t	sectors_per_track;
	uint16_t	head_side_count;
	uint32_t 	hidden_sector_count;
	uint32_t 	total_sectors_32;

	// FAT32 specific fields
	uint32_t	table_size_32;			// the number of SECTORS (NOT bytes!) occupied by one FAT32 FAT table
	uint16_t	extended_flags;
	uint16_t	fat_version;
	uint32_t	root_cluster;
	uint16_t	fat_info;
	uint16_t	backup_BS_sector;
	uint8_t 	reserved_0[12];
	uint8_t		drive_number;

	// align next to 70
	uint8_t		__alignmentByte5;

	uint8_t		reserved_1;
	uint8_t		boot_signature;
	uint32_t	volume_id;
	uint8_t		volume_label[ 11 ];

	// align next to 88
	uint8_t		__alignmentByte6;

	uint8_t		fat_type_label[ 8 ];

	uint8_t 	boot_code[ 420 ];
	uint16_t 	signature;
} FAT32_BPS_INFO_STRUCT;

typedef struct
{
	char		fileName[ 11 ];
	uint8_t 	attributes;
	uint8_t 	reserved;
	uint8_t 	createdSecFract;
	uint16_t 	creationTime;
	uint16_t 	creationDate;
	uint16_t 	lastAccess;
	uint16_t 	clusterNumberHigh;
	uint16_t 	lastModTime;
	uint16_t 	lastModDate;
	uint16_t 	clusterNumberLow;
	uint32_t	fileSize;
} FAT32_ENTRY;

typedef enum
{
	ARCHIVE = 0,
	DIRECTORY,
	VOLUME
} DIR_TYPE;

typedef struct __DIR_ENTRY
{
	char				fileName[ 12 ];
	DIR_TYPE 			type;
	uint32_t 			startClusterNumber;
	uint32_t			fileSize;

	int32_t					childrenCount;
	struct __DIR_ENTRY*		children;
} DIR_ENTRY;

typedef struct
{
	DIR_ENTRY* 	dirEntry;

	uint8_t*	currentClusterBuffer;

	uint8_t* 	currentFatSectorBuffer;
	uint32_t	currentFatSectorLba;

	uint32_t 	currentCluster;
	uint32_t 	clusterCursor;
	uint32_t 	fileCursor;
} FILE_DESCRIPTOR;
///////////////////////////////////////////////////////////////////

// module-local data //////////////////////////////////////////////
// global structures and data
static PRIMARY_PARTITION_INFO_STRUCT _primaryPartition;
static FAT32_BPS_INFO_STRUCT _fat32Bps;
static DIR_ENTRY _fsRoot;
static FILE_DESCRIPTOR _fileDescriptors[ MAX_FILE_DESCRIPTORS ];

// global buffers
static uint8_t* _clusterBuffer;

/// global constants
static uint32_t _clusterBufferSize;
static uint8_t _blocksPerSector;
static uint32_t _bytesPerCluster;
static uint32_t _clusterBegin_lba;
static uint32_t _firstFatSectorLba;
///////////////////////////////////////////////////////////////////

// module-local functions /////////////////////////////////////////
static uint32_t loadPrimaryPartition( void );
static uint32_t loadFsRoot( void );
static uint32_t loadDirectory( uint32_t cluster, DIR_ENTRY* dir );
static void readDirectory( uint8_t* buffer, DIR_ENTRY* dir );
static uint32_t locateDirEntry( const char* filePath, DIR_ENTRY* parent, DIR_ENTRY** entry );
static file_id findFreeFileDescriptor( void );
static uint32_t countChildren( uint8_t* buffer );
static uint32_t calculateClusterLba( uint32_t clusterNumber );
static uint32_t readCluster( uint32_t clusterLba, uint8_t* buffer );
static FILE_DESCRIPTOR* getOpenedFdById( file_id fileId );
///////////////////////////////////////////////////////////////////

uint32_t fat32Init( void )
{
	//interrupt when card is inserted

	// initialize sd
	if ( sdHalInit() )
	{
		return 1;
	}

	if ( loadPrimaryPartition() )
	{
		return 1;
	}

	if ( loadFsRoot() )
	{
		return 1;
	}

	return 0;
}

uint32_t fat32Open( const char* filePath, file_id* fileId )
{
	DIR_ENTRY* entry = 0;

	file_id fId = findFreeFileDescriptor();
	// no free file-descriptors found, error
	if ( -1 == fId )
	{
		return 1;
	}

	if ( locateDirEntry( filePath, &_fsRoot, &entry ) )
	{
		return 1;
	}

	FILE_DESCRIPTOR* fd = &_fileDescriptors[ fId ];

	fd->dirEntry = entry;
	fd->currentCluster = entry->startClusterNumber;

	//fd->currentFatSectorBuffer =kmalloc( _fat32Bps.bytes_per_sector );
	memset( fd->currentFatSectorBuffer, 0, _fat32Bps.bytes_per_sector );

	//fd->currentClusterBuffer = kmalloc( _clusterBufferSize );
	memset( fd->currentClusterBuffer, 0, _clusterBufferSize );

	*fileId = fId;

	return 0;
}

uint32_t fat32Size( file_id fileId, uint32_t* size )
{
	FILE_DESCRIPTOR* fd = getOpenedFdById( fileId );
	if ( 0 == fd )
	{
		return 1;
	}

	*size = fd->dirEntry->fileSize;

	return 0;
}

uint32_t fat32Close( file_id fileId )
{
	FILE_DESCRIPTOR* fd = getOpenedFdById( fileId );
	if ( 0 == fd )
	{
		return 1;
	}

	free( fd->currentFatSectorBuffer );
	free( fd->currentClusterBuffer );

	memset( fd, 0, sizeof( FILE_DESCRIPTOR ) );

	return 0;
}

int32_t fat32Read( file_id fileId, uint32_t nBytes, uint8_t* buffer )
{
	FILE_DESCRIPTOR* fd = getOpenedFdById( fileId );
	if ( 0 == fd )
	{
		return 1;
	}

	// alredy reached EOF in a previous read-operation, return 0 bytes read (no error)
	if ( fd->fileCursor == fd->dirEntry->fileSize )
	{
		return 0;
	}

	uint32_t bytesRead = 0;

	while ( 1 )
	{
		// cluster has not yet been read
		if ( 0 == fd->clusterCursor )
		{
			uint32_t clusterLba = calculateClusterLba( fd->currentCluster );
			if ( readCluster( clusterLba, fd->currentClusterBuffer ) )
			{
				return -1;
			}
		}

		if ( fd->fileCursor + nBytes > fd->dirEntry->fileSize )
		{
			nBytes = fd->dirEntry->fileSize - fd->fileCursor;
		}

		uint32_t nBytesCopy = nBytes;

		// reading more bytes than left in the current cluster
		if ( fd->clusterCursor + nBytes > _clusterBufferSize )
		{
			nBytesCopy = _clusterBufferSize - fd->clusterCursor;
		}

		// copy the data between buffers
		memcpy( &buffer[ bytesRead ], &fd->currentClusterBuffer[ fd->clusterCursor ], nBytesCopy );

		bytesRead += nBytesCopy;
		fd->clusterCursor += nBytesCopy;
		fd->fileCursor += nBytesCopy;
		nBytes -= nBytesCopy;

		// reached end of file, stop
		if ( fd->fileCursor == fd->dirEntry->fileSize )
		{
			break;
		}

		// reached end of cluster but not of file, setup for cluster-reload during next read
		if ( fd->clusterCursor == _clusterBufferSize )
		{
			uint32_t fatOffset = fd->currentCluster * 4;
			uint32_t fatSectorLba = _firstFatSectorLba + ( fatOffset / _fat32Bps.bytes_per_sector );
			uint32_t entOffset = fatOffset % _fat32Bps.bytes_per_sector;

			// check if cached fat-sector has changed and load it if it has
			if ( fd->currentFatSectorLba != fatSectorLba )
			{
				// update cached fat-sector
				fd->currentFatSectorLba = fatSectorLba;
				// load the according sector of the FAT to access the next cluster or detect end of file
				if ( readCluster( fd->currentFatSectorLba, fd->currentFatSectorBuffer ) )
				{
					return 1;
				}
			}

			//remember to ignore the high 4 bits (FAT32 is in fact FAT28 )
			uint32_t tableValue = *( uint32_t* ) &fd->currentFatSectorBuffer[ entOffset ] & 0x0FFFFFFF;
			// reached end of file
			if ( tableValue >= 0x0FFFFFF8 )
			{
				break;
			}

			// tableValue stores now the next cluster of this file
			fd->currentCluster = tableValue;
			// reset clustercursor to start, will do a cluster-read during the next
			fd->clusterCursor = 0;
		}
		// not end of cluster yet, but read nBytes, leaving loop
		else
		{
			break;
		}
	}

	return bytesRead;
}

FILE_DESCRIPTOR* getOpenedFdById( file_id fileId )
{
	FILE_DESCRIPTOR* fd = 0;

	// invalid file-id, error
	if ( fileId > MAX_FILE_DESCRIPTORS || fileId < 0 )
	{
		return 0;
	}

	fd = &_fileDescriptors[ fileId ];
	// file is not opened, error
	if ( 0 == fd->dirEntry )
	{
		return 0;
	}

	return fd;
}

uint32_t loadPrimaryPartition( void )
{
	uint8_t dataBuffer[ MBR_AND_BPS_SIZE ];
	memset( dataBuffer, 0, MBR_AND_BPS_SIZE );

	// read the MBR from card
	if ( sdHalReadBlocks( 0, 1, dataBuffer ) )
	{
		return 1;
	}

	// check signature of MBR and reject invalid MBR signatures
	uint16_t signature = ( dataBuffer[ MBR_SIGNATURE_OFFSET ] ) | ( dataBuffer[ MBR_SIGNATURE_OFFSET + 1 ] << 8 );
	if ( MBR_SIGNATURE_VALUE != signature )
	{
		return 1;
	}

	// store primary-partition information in structure
	memcpy( &_primaryPartition, &dataBuffer[ PRIMARY_PARTITION_OFFSET ], sizeof( _primaryPartition ) );

	// check if primary partition is FAT32
	if ( PRIMARY_PARTITION_FAT32_TYPE != _primaryPartition.type )
	{
		return 1;
	}

	// read BPS of primary partition, will contain the information
	if ( sdHalReadBlocks( _primaryPartition.partitionStart_lba, 1, dataBuffer ) )
	{
		return 1;
	}

	// copy piece-wise ignoring alignment-bytes - this is a workaround of the alignment problems (see declaration of struct)
	memcpy( &_fat32Bps, &dataBuffer, 11 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 12 ], &dataBuffer[ 11 ], 3 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 16 ], &dataBuffer[ 14 ], 3 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 20 ], &dataBuffer[ 17 ], 5 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 26 ], &dataBuffer[ 22 ], 43 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 70 ], &dataBuffer[ 65 ], 17 );
	memcpy( &( ( uint8_t* ) &_fat32Bps )[ 88 ], &dataBuffer[ 82 ], 512 - 82 );

	// calculate total clusters and check if violates FAT32
	uint32_t root_dir_sectors = ( ( _fat32Bps.root_entry_count * 32 ) + ( _fat32Bps.bytes_per_sector - 1 ) ) / _fat32Bps.bytes_per_sector;
	uint32_t data_sectors = _fat32Bps.total_sectors_16 - ( _fat32Bps.reserved_sector_count + ( _fat32Bps.number_fats * _fat32Bps.table_size_32 ) + root_dir_sectors );
	uint32_t total_clusters = data_sectors / _fat32Bps.sectors_per_cluster;

	// ERROR: this is not a FAT32 system
	if ( total_clusters < FAT32_MIN_CLUSTER_COUNT )
	{
		return 1;
	}

	// will always be a division without rest as bytes_per_sector is either 512, 1024, 2048 or 4096
	_blocksPerSector = _fat32Bps.bytes_per_sector / SD_CARD_BLOCK_SIZE;
	_bytesPerCluster = _fat32Bps.sectors_per_cluster * _fat32Bps.bytes_per_sector;
	_firstFatSectorLba = _primaryPartition.partitionStart_lba + _fat32Bps.reserved_sector_count;

	return 0;
}

uint32_t loadFsRoot( void )
{
	_clusterBegin_lba = _primaryPartition.partitionStart_lba + _fat32Bps.reserved_sector_count + ( _fat32Bps.number_fats * _fat32Bps.table_size_32 );

	// allocate memory to hold one complete cluster
	_clusterBufferSize = _fat32Bps.bytes_per_sector * _fat32Bps.sectors_per_cluster;
	//_clusterBuffer = kmalloc( _clusterBufferSize );
	memset( _clusterBuffer, 0, _clusterBufferSize );

	if ( loadDirectory( _clusterBegin_lba, &_fsRoot ) )
	{
		return 1;
	}

	return 0;
}

uint32_t loadDirectory( uint32_t clusterLba, DIR_ENTRY* dir )
{
	if ( readCluster( clusterLba, _clusterBuffer ) )
	{
		return 1;
	}

	readDirectory( _clusterBuffer, dir );

	return 0;
}

void readDirectory( uint8_t* buffer, DIR_ENTRY* dir )
{
	uint32_t i = 0;
	uint32_t childIndex = 0;

	dir->childrenCount = countChildren( buffer );
	//dir->children = kmalloc( dir->childrenCount * sizeof( DIR_ENTRY ) );
	memset( dir->children, 0, dir->childrenCount * sizeof( DIR_ENTRY ) );

	for ( i = 0; i < _bytesPerCluster; i += 32 )
	{
		// ignore unused files
		if ( DIRECTORY_UNUSED == buffer[ i ] )
		{
			continue;
		}
		else if ( DIRECTORY_END == buffer[ i ] )
		{
			// reached end of directory, stop iterating through data
			break;
		}

		FAT32_ENTRY* fat32Entry = ( FAT32_ENTRY* ) &buffer[ i ];

		// long-name files have the first 4 bits set
		if ( 0 == IS_LONGNAME( fat32Entry->attributes ) )
		{
			if ( IS_HIDDEN( fat32Entry->attributes ) ||
					IS_SYSTEM( fat32Entry->attributes ) ||
					IS_VOLUMEID( fat32Entry->attributes ) )
			{
				// ignore hidden / system / volumeid files
				continue;
			}
		}

		if ( DIR_ATTRIB_DIRECTORY & fat32Entry->attributes )
		{
			dir->children[ childIndex ].type = DIRECTORY;
			dir->children[ childIndex ].childrenCount = -1; // set to -1 to mark as not loaded yet - will be done lazily when searching through the filesystem during open file
		}

		if ( DIR_ATTRIB_ARCHIVE & fat32Entry->attributes )
		{
			dir->children[ childIndex ].type = ARCHIVE;
		}

		dir->children[ childIndex ].fileSize = fat32Entry->fileSize;
		dir->children[ childIndex ].startClusterNumber = ( fat32Entry->clusterNumberHigh << 0x10 ) | fat32Entry->clusterNumberLow;

		uint8_t c = 0;
		uint8_t cDir = 0;
		bool foundSpace = FALSE;

		for ( c = 0; c < sizeof( fat32Entry->fileName ); ++c )
		{
			if ( ' ' == fat32Entry->fileName[ c ] )
			{
				foundSpace = TRUE;
			}
			else
			{
				if ( foundSpace )
				{
					dir->children[ childIndex ].fileName[ cDir ] = '.';
					++cDir;
					foundSpace = FALSE;
				}

				dir->children[ childIndex ].fileName[ cDir ] = tolower( fat32Entry->fileName[ c ] );
				++cDir;
			}
		}

		++childIndex;
	}

	// TODO: a directory could span multiple clusters - handle it!
}

uint32_t locateDirEntry( const char* filePath, DIR_ENTRY* parent, DIR_ENTRY** entry )
{
	uint32_t i = 0;

	char* isDirectory = strchr( filePath, '/' );
	char* fileName = ( char* ) filePath;

	if ( isDirectory )
	{
		fileName = strtok( fileName, "/" );
	}

	for ( i = 0; i < parent->childrenCount; ++i )
	{
		DIR_ENTRY* child = &( ( DIR_ENTRY* ) parent->children )[ i ];

		// look for a matching file-name
		if ( 0 == strcasecmp( fileName, child->fileName ) )
		{
			// the entry is a directory, check if its part of the filename and traverse down
			if ( DIRECTORY == child->type )
			{
				// its part of the filename, look recursively in the subdirectory
				if ( isDirectory )
				{
					// directory not yet loaded, load it now
					if ( -1 == child->childrenCount )
					{
						uint32_t cluster_lba = calculateClusterLba( child->startClusterNumber );

						// loading failed
						if ( loadDirectory( cluster_lba, child ) )
						{
							return 1;
						}
					}

					// look recursively in subdirectory
					return locateDirEntry( isDirectory + 1, child, entry );
				}
				// its the ending of the filename
				else
				{
					// want to open a directory, is not allowed
					return 1;
				}
			}
			// the entry is a file, found it if its not part of the filename but its ending
			else if ( ARCHIVE == child->type )
			{
				if ( ! isDirectory )
				{
					// found
					*entry = child;
					return 0;
				}
			}
		}
	}

	return 1;
}

uint32_t countChildren( uint8_t* buffer )
{
	uint32_t i = 0;
	uint32_t counter = 0;

	for ( i = 0; i < _bytesPerCluster; i += 32 )
	{
		// ignore unused files
		if ( DIRECTORY_UNUSED == buffer[ i ] )
		{
			continue;
		}
		else if ( DIRECTORY_END == buffer[ i ] )
		{
			// reached end of directory, stop iterating through data
			break;
		}

		FAT32_ENTRY* fat32Entry = ( FAT32_ENTRY* ) &buffer[ i ];

		// long-name files have the first 4 bits set
		if ( 0 == IS_LONGNAME( fat32Entry->attributes ) )
		{
			if ( IS_HIDDEN( fat32Entry->attributes ) ||
					IS_SYSTEM( fat32Entry->attributes ) ||
					IS_VOLUMEID( fat32Entry->attributes ) )
			{
				// ignore hidden / system / volumeid files
				continue;
			}
		}

		counter++;
	}

	return counter;
}

uint32_t readCluster( uint32_t clusterLba, uint8_t* buffer )
{
	if ( sdHalReadBlocks( clusterLba, _fat32Bps.sectors_per_cluster * _blocksPerSector, buffer ) )
	{
		return 1;
	}

	return 0;
}

file_id findFreeFileDescriptor( void )
{
	int32_t i = 0;

	for ( i = 0; i < MAX_FILE_DESCRIPTORS; ++i )
	{
		// if dirEntry is 0, then this descriptor is not in use
		if ( ! _fileDescriptors[ i ].dirEntry )
		{
			return i;
		}
	}

	return -1;
}

uint32_t calculateClusterLba( uint32_t clusterNumber )
{
	return _clusterBegin_lba + ( clusterNumber - 2 ) * _fat32Bps.sectors_per_cluster;
}
