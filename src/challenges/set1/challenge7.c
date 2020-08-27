#include "../../utils/file.h"

#include <openssl/evp.h>
#include <stdio.h>

int main(void)
{
  // Open file
  unsigned char * c;
  size_t clen = freadb64("../../res/7.txt", &c);
  if(!clen) return 1;

  // Decrypt
  char * p = malloc(clen + 1);
  int outl;
  EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit(ctx, EVP_aes_128_ecb(), "YELLOW SUBMARINE", NULL);
  EVP_DecryptUpdate(ctx, p, &outl, c, clen);
  int _outl;
  EVP_DecryptFinal(ctx, p + outl, &_outl);
  EVP_CIPHER_CTX_free(ctx);
  int pl = outl + _outl;
  p[pl] = '\0';

  // Print result
  puts(p);

  // Finalize
  free(c);
  free(p);
  return 0;
}
