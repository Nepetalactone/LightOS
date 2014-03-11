#ifndef BINARY_H
#define BINARY_H


#define binary(x) S_to_binary_(#x)


typedef volatile unsigned int byte;

/*
	turns a string representation of a binary number into TODO change return type?

*/
static inline unsigned long long S_to_binary_(const char *s)
{
        unsigned long long i = 0;
        while (*s) {
                i <<= 1;
                i += *s++ - '0';
        }
        return i;
}

#endif
