#ifndef __XORKEYFINDER_H__
#define __XORKEYFINDER_H__

#include <ctype.h>
#include <stdlib.h>
#include <float.h>

unsigned char findxorkey(unsigned char * c, size_t clen, size_t klen, size_t offset, int strict);

#endif /* end of include guard: __XORKEYFINDER_H__ */
