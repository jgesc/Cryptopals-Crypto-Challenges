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

unsigned int hamm(const unsigned char * a, const unsigned char * b, size_t len)
{
  // Unset bits lookup table
  static const unsigned char bittable[256] =
  {
  #   define B2(n)     n,     n+1,     n+1,     n+2
  #   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
  #   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
      B6(0), B6(1), B6(1), B6(2)
  };

  unsigned int cnt = 0; // Bit counter

  size_t i;
  for(i = 0; i < len; i++) cnt += bittable[a[i] ^ b[i]];

  return cnt;
}
