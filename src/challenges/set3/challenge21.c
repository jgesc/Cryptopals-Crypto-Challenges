#include "../../utils/mtrand.h"

#include <stdio.h>

int main(void)
{
  mtseed(5489);
  int i;
  for(i = 0; i < 10; i++)
    printf("%08x\n", mtrand());
  putchar('\n');

  return 0;
}
