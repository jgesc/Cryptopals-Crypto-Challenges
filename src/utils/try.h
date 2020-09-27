#ifndef __TRY_H__
#define __TRY_H__

#include <stdio.h>
#include <stdlib.h>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define TRY(X) if(!(X)) {perror("ERROR in file " STRINGIFY(__FILE__) ", line " STRINGIFY(__LINE__) "\n\t" STRINGIFY(X) "\n"); exit(1);};

#endif
