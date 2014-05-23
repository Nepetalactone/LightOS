#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#define binary(x) S_to_binary_(#x)


typedef uint8_t byte;

uint8_t getBitValue(address a, offset o, uint32_t p);

static inline uint64_t S_to_binary_(const char *s);

#endif
