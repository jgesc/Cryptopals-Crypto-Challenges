#include "../../utils/mtrand.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

long long unsigned int getrand()
{
  // Wait random time
  srand(time(NULL));
  sleep(rand() % 20 + 10);

  // Seed RNG
  mtseed(time(NULL));

  // Wait and return
  sleep(rand() % 20 + 10);
  return mtrand();
}

int main(void)
{
  // Get random number
  long long unsigned int rn = getrand();
  printf("Random number = %llu\n", rn);

  // Find seed
  long long unsigned int n = 0;
  long long unsigned int t = time(NULL) + 1;
  while(n != rn)
  {
    mtseed(--t);
    n = mtrand();
  }
  printf("Seed = %llu\n", t);

  return 0;
}
