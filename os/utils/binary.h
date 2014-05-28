#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include "../kernel/arch/address.h"
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

#endif
