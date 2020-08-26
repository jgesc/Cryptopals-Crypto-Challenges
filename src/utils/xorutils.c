#include "xorutils.h"

void xorarray(const unsigned char * a, const unsigned char * b, unsigned char * out, size_t len)
{
  size_t i;

  for(i = 0; i < len; i++) out[i] = a[i] ^ b[i];
}

void sbxorarray(const unsigned char * a, unsigned char b, unsigned char * out, size_t len)
{
  size_t i;

  for(i = 0; i < len; i++) out[i] = a[i] ^ b;
}

void repxorarray(const unsigned char * p, size_t plen, const unsigned char * k, size_t klen, unsigned char * out)
{
  size_t i;

  for(i = 0; i < plen; i++) out[i] = p[i] ^ k[i % klen];
}
