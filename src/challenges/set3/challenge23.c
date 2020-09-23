#include "../../utils/mtrand.h"

#include <stdio.h>
#include <time.h>

#define NTESTS 1200000
//#define PRNTESTS // Uncomment to print tests

int main(void)
{
  int i;

  // Seed PRNG
  mtseed(time(NULL));

  // Get 624 samples
  uint32_t samples[624];
  mtrandarr(samples, 624);

  // Get 10 test samples
  uint32_t test[NTESTS];
  mtrandarr(test, NTESTS);

  // Reset internal state
  mtseed(0);

  // Rebuild internal state
  uint32_t * mt = mtgetintstate();
  for(i = 0; i < 624; i++)
    mt[i] = mtuntemper(samples[i]);

  // Get NTESTS samples and compare with test vector
  #ifdef PRNTESTS
    printf("%10s%10s\n", "TEST", "CLONE");
  #endif
  int ok = 0;
  for(i = 0; i < NTESTS; i++)
  {
    uint32_t rn = mtrand();
    #ifdef PRNTESTS
      printf("%10x%10x\n", test[i], rn);
    #endif
    if(test[i] == rn) ok++;
  }
  printf("\nTests %s\n", ok == NTESTS ? "OK" : "FAILED");

  return 0;
}
