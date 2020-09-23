#include "mtrand.h"

#define W 32
#define N 624
#define M 397
#define R 31
#define U 11
#define S 7
#define T 15
#define L 18
#define D 0xFFFFFFFF
#define B 0x9D2C5680
#define C 0xEFC60000
#define A 0x9908B0DF

uint32_t mt[W];
uint32_t idx = N + 1;

#define LOWER_MASK 0x7fffffff
#define UPPER_MASK 0x80000000

void mtseed(uint32_t seed)
{
  idx = N;
  mt[0] = seed;
  int i;
  for(i = 1; i < N; i++)
    mt[i] = 1812433253UL * (mt[i-1] ^ (mt[i-1] >> (W - 2))) + i;
}

uint32_t mtrand()
{
  // Check if initialized
  if(idx > N) return 0;

  if(idx == N)
  {
    int i;
    for(i = 0; i < N; i++)
    {
      uint32_t x = (mt[i] & UPPER_MASK) + (mt[(i + 1) % N] & LOWER_MASK);
      uint32_t xA = x >> 1;
      if(x % 2) xA ^= A;
      mt[i] = mt[(i + M) % N] ^ xA;
    }
    idx = 0;
  }

  uint32_t y = mt[idx++];
  y ^= ((y >> U) & D);
  y ^= ((y << S) & B);
  y ^= ((y << T) & C);
  y ^= (y >> L);

  return y;
}
