/*
 * fat32.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef FAT32_H_
#define FAT32_H_

#include "common.h"

typedef int32_t file_id;

/**
* Initializes the FAT32.
* Returns 0 upon success.
* Returns 1 upon failure.
*/
uint32_t fat32Init( void );

/**
* Opens a file (for reading only) from the filePath.
* Returns 0 upon success, where file will contain a valid FILE pointer.
* Returns 1 upon failure, where file will not be changed.
*/
uint32_t fat32Open( const char* filePath, file_id* fileId );
/**
* Closes a previously opened file.
* Returns 0 upon success.
* Returns 1 if not opened or invalid fileid.
*/
uint32_t fat32Close( file_id fileId );

/**
* Reads up to nBytes into buffer from file.
* Returns the number of bytes actually read or -1 if failure or 0 if EOF has reached already during a previous read.
*
* HINT: read always less than 512 bytes as it is much faster.
* A test reading a 5MB MP3 file delivered:
* 133bytes: 8.3 sec
* 510bytes: 7.5 sec
* 512,1024,496 bytes: 13.9sec
*/
int32_t fat32Read( file_id fileId, uint32_t nBytes, uint8_t* buffer );

/**
* Stores the fileSize of the file with fileId in size.
* Returns 0 upon success.
* Returns 1 if not opened or invalid fileid.
*/
uint32_t fat32Size( file_id fileId, uint32_t* size );

#endif /* FAT32_H_ */
