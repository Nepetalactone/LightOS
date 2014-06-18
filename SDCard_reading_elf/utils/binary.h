/*
 * binary.h
 *
 *  Created on: 14.06.2014
 *      Author: phil
 */

#ifndef BINARY_H_
#define BINARY_H_

#include <stdint.h>
#include "../arch/address.h"
#define binary(x) S_to_binary_(#x)


typedef uint8_t byte;


static inline uint64_t S_to_binary_(const char *s){
        uint32_t i = 0;
        while (*s) {
                i <<= 1;
                i += *s++ - '0';
        }
        return i;
}

#endif /* BINARY_H_ */
