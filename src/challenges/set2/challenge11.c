#include "../../utils/crypto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EBC 1
#define CBC 2

void gen16key(char * out)
{
  int i;
  for(i = 0; i < 16; i++) out[i] = rand() % 256;
}

int randencrypt(const char * in, size_t inl, unsigned char * out)
{
  // Generate random 16 bytes key
  char k[16];
  gen16key(k);

  // Copy plaintext and append random bytes
  char buff[8192];
  int prel = 5 + rand() % 5; // Random prefix length
  int sufl = 5 + rand() % 5; // Random suffix length
  size_t i;
  for(i = 0; i < prel; i++) buff[i] = rand() % 256;
  strncpy(buff + prel, in, inl);
  for(i = 0; i < sufl; i++) buff[prel + inl + i] = rand() % 256;
  size_t len = prel + inl + sufl;

  // Encrypt
  if(rand() % 2)
  {
    encrypt_aes128ecb(buff, len, out, k);
    return EBC;
  }
  else
  {
    // Generate IV
    char iv[16];
    gen16key(iv);

    encrypt_aes128cbc(buff, len, out, k, iv);
    return CBC;
  }
}

int main(void)
{
  srand(time(NULL));
  const int ntests = 1000;

  // Buffers
  char in[512] = {0};
  char out[8192];

  // Test
  int hit = 0;
  int i;
  for(i = 0; i < ntests; i++)
  {
    int true_mode = randencrypt(in, 512, out);
    int pred_mode = detect_ebc(out, 512 + 16) > 1 ? EBC : CBC;

    if(true_mode == pred_mode) hit++;
  }

  // Print results
  printf("Detected mode correctly %d/%d times\n", hit, ntests);
}
