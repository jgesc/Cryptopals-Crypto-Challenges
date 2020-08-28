#ifndef __FILE_H__
#define __FILE_H__

#include "stringutils.h"

#include <stdio.h>
#include <stdlib.h>

// Allocates space and reads a base64 encoded file
size_t freadb64(const char * path, unsigned char ** out);

#endif /* end of include guard: __FILE_H__ */
