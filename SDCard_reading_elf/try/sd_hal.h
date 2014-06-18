/*
 * sd_hal.h
 *
 *  Created on: 11.06.2014
 *      Author: phil
 */

#ifndef SD_HAL_H_
#define SD_HAL_H_

#include <inttypes.h>

uint32_t sdHalInit();
uint32_t sdHalReadBlocks( uint32_t block, uint32_t nblk, uint8_t* buffer );

typedef struct {
uint32_t raw_cid[ 4 ];
uint16_t rca;
uint32_t raw_csd[ 4 ];
uint32_t raw_scr[ 2 ];
uint32_t ocr;

uint8_t highCap;
uint32_t tranSpeed;
uint32_t blkLen;
uint32_t size;
uint32_t nBlks;
uint8_t busWidth;
uint8_t sd_ver;
} CARD_INFO;

#endif /* SD_HAL_H_ */
