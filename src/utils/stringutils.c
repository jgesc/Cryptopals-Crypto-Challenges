#include "stringutils.h"

void printhex(const unsigned char * bytes, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++) printf("%2x", bytes[i]);
}

void printb64(const unsigned char * bytes, size_t len)
{

}
