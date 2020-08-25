#ifndef __TEXUTILS_H__
#define __TEXUTILS_H__

#include <ctype.h>
#include <float.h>
#include <stdlib.h>

// Score a piece of English plaintexts
float scoretext(char * in, size_t len, int strict);

#endif /* end of include guard: __TEXUTILS_H__ */
