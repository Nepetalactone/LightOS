/*
 * fat16.h
 *
 *  Created on: 20.06.2014
 *      Author: itm13
 */

#ifndef FAT16_H_
#define FAT16_H_

#include <stdint.h>
#include "../arch/address.h"

/* GENERAL */
#define FAT16_BLOCK_SIZE 		512 //BYTES
#define FAT16_DIRECTORY_ENTRY 	32 //BYTES
#define FAT16_FAT_ENTRY 		2 //BYTES

#define FAT16_BASE_ADDRESS (base_address) 0x00000000 //TODO load dynamically (maybe)

#define CLUSTER_BASE_NR 0x02
/* GENERAL END */
#define ROOT_DIR_ADDRESS (address) FAT16_BASE_ADDRESS + (REG_READ(FAT16_BASE_ADDRESS + NR_BLOCKS_PER_FAT) * REG_READ(FAT16_BASE_ADDRESS + NR_OF_FATS) + 1);

/* BOOT BLOCK */
#define NR_BYTES_PER_BLOCK 	(offset) 0x0b //2bytes
#define NR_BLOCKS_PER_UNIT 	(offset) 0x0D //1byte
#define NR_OF_FATS 			(offset) 0x10 //1byte
#define NR_ROOT_DIRS 		(offset) 0x11 //2bytes
#define NR_BLOCKS_PER_FAT 	(offset) 0x16 //2byte
/* BOOT BLOCK END */

/* DIRECTORY AND FILE */
#define FILENAME 				(offset)	0x00
#define FILE_EXTENSION 			(offset) 	0x08
#define FILE_ATTRIBUTES 		(offset) 	0x0b
#define RESERVED 				(offset) 	0x0c
#define TIME_CREATED_OR_UPDATED (offset) 	0x16
#define DATE_CREATED_OR_UPDATED (offset) 	0x18
#define START_CLUSTER 			(offset) 	0x1a
#define FILE_SIZE 				(offset) 	0x1c

#define FILENAME_LENGTH 				(unint32_t) 	8 	//BYTES
#define FILE_EXTENSION_LENGTH 			(unint32_t) 	3 	//BYTES
#define FILE_ATTRIBUTES_LENGTH 			(unint32_t) 	1 	//BYTES
#define RESERVED_LENGTH 				(unint32_t) 	10 	//BYTES
#define TIME_CREATED_OR_UPDATED_LENGTH 	(unint32_t) 	2 	//BYTES
#define DATE_CREATED_OR_UPDATED_LENGTH 	(unint32_t) 	2 	//BYTES
#define START_CLUSTER_LENGTH 			(unint32_t) 	2 	//BYTES
#define FILE_SIZE_LENGTH 				(unint32_t) 	4 	//BYTES

typedef struct {
	char* filename;
	char* file_extension;
	uint8_t file_attributes;
	char* time_created;
	char* date_created;
	uint16_t start_cluster;
	uint32_t file_size;
} entry;

/* FILE NAME MODIFIERS */
#define UNUSED 							0x00
#define SPECIAL_CHAR_USED_IN_FILENAME 	0x05
#define DELETED 						0xE5
#define DIRECTORY 						0x2E
/* FILE NAME MODIFIERS END */

/* FILE ATTRIBUTES  */
#define FAT16_READONLY 		0x01
#define FAT16_HIDDEN 		0x02
#define FAT16_SYSTEM_FILE 	0x04
#define FAT16_VOLUME_LABEL 	0x08
#define FAT16_SUBDIRECTORY 	0x10
#define FAT16_ARCHIVE 		0x20
/* FILE ATTRIBUTES END */

/* DIRECTORY AND FILE END */

uint32_t fat16_init();
uint32_t file_open(char* filename);
uint32_t file_close(char* filename);
uint32_t file_read(char* filename);

#endif /* FAT16_H_ */
