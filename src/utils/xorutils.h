#ifndef __XORUTILS_H__
#define __XORUTILS_H__

#include <stdlib.h>

// Xor fixed size buffers
void xorarray(const unsigned char * a, const unsigned char * b, unsigned char * out, size_t len);

// Single byte xor an array
void sbxorarray(const unsigned char * a, unsigned char b, unsigned char * out, size_t len);

// Repeating key xor an array
void repxorarray(const unsigned char * p, size_t plen, const unsigned char * k, size_t klen, unsigned char * out);

unsigned int hamm(const unsigned char * a, const unsigned char * b, size_t len);

#endif /* end of include guard: __XORUTILS_H__ */
