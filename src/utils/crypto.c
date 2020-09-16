#include "crypto.h"

size_t pkcs7(unsigned char * buff, size_t msglen, size_t bsize)
{
  size_t i;
  size_t padd = (unsigned char)(bsize - msglen % bsize);
  if(padd > 255) printf("ERROR: Too much padding");
  for(i = 0; i < padd; i++) buff[msglen + i] = (unsigned char)padd;

  return msglen + padd;
}

int encrypt_aes128ecb(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k)
{
  int outl = 0;
  EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit(ctx, EVP_aes_128_ecb(), k, NULL);
  EVP_EncryptUpdate(ctx, c, &outl, p, pl);
  int _outl = 0;
  EVP_EncryptFinal(ctx, c + outl, &_outl);
  EVP_CIPHER_CTX_free(ctx);
  int cl = outl + _outl;
  return cl;
}

int decrypt_aes128ecb(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k)
{
  int outl = 0;
  EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit(ctx, EVP_aes_128_ecb(), k, NULL);
  EVP_DecryptUpdate(ctx, p, &outl, c, cl);
  int _outl = 0;
  if(!EVP_DecryptFinal(ctx, p + outl, &_outl)) puts("ER\n");
  EVP_CIPHER_CTX_free(ctx);
  int pl = outl + _outl;
  return pl;
}

void encrypt_aes128cbc(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k, const unsigned char * iv)
{
  // Point previous block to IV
  const unsigned char * pblock = iv;
  // Calculate number of blocks
  size_t nblocks = pl / 16 + (pl % 16 > 0);
  // Count length of c
  size_t cl = 0;
  // Iterate for each block
  size_t i;
  for(i = 0; i < nblocks; i++)
  {
    // XOR buffer
    char pxorc[16];
    xorarray(pblock, p + i * 16, pxorc, 16);
    // Encrypt block
    encrypt_aes128ecb(pxorc, 16, c + i * 16, k);
    // Point previous block to new ciphered block
    pblock = c + i * 16;
  }
}

void decrypt_aes128cbc(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k, const unsigned char * iv)
{
  // Point previous block to IV
  unsigned const char * pblock = iv;
  // Calculate number of blocks
  size_t nblocks = cl / 16;
  // Count length of p
  size_t pl = 0;
  // Iterate for each block
  size_t i;
  for(i = 0; i < nblocks; i++)
  {
    // XOR buffer
    char pxorc[16];
    // Decrypt block
    decrypt_aes128ecb(c + i * 16, 16, pxorc, k);
    // Xor with previous cipher block
    xorarray(pblock, pxorc, p + i * 16, 16);
    // Point previous block to last ciphered block
    pblock = c + i * 16;
  }
}

int detect_ebc(const unsigned char * c, size_t cl)
{
  // Look for repeating patterns
  int nblocks = cl / 16;
  int i, j, rep = 0;
  for(i = 0; i < nblocks - 1; i++)
    for(j = i + 1; j < nblocks; j++)
      if(strncmp(c + i * 16, c + j * 16, 16) == 0)
        rep++;

  return rep;
}

int pkcs7strip(unsigned char * p, size_t l)
{
  int pl = p[l - 1]; // Padding length

  // Check padding
  int i;
  for(i = 0; i < pl; i++)
    if(p[l - 1 - i] != pl) return 0;

  // Strip padding
  for(i = 0; i < pl; i++)
    p[l - 1 - i] = 0;

  // Return
  return pl;
}
