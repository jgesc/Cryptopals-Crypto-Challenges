#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"
#include "../../utils/file.h"

#include <stdio.h>

int main(void)
{
  // Open file
  unsigned char * c;
  size_t clen = freadb64("../../res/10.txt", &c);
  if(!clen)
  {
    printf("Error opening file\n");
    return 1;
  }

  // Decrypt
  unsigned char * k = "YELLOW SUBMARINE";
  unsigned char iv[16] = {0};
  char p[4096] = {0};
  decrypt_aes128cbc(c, clen, p, k, iv);
  puts(p);

  return 0;
}
