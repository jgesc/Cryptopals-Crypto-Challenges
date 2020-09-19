#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdlib.h>
#include <string.h>

static const char * prestr = "comment1=cooking%20MCs;userdata=\0";
static const char * poststr = ";comment2=%20like%20a%20pound%20of%20bacon\0";

static char k[16], iv[16];

int ORACLE(const unsigned char * in, size_t inl, unsigned char * out)
{
  static char initialized = 0;

  if(!initialized)
  {
    initialized = 1;

    // Initialize random seed
    srand(time(NULL));
    // Generate random key and iv
    int i;
    for(i = 0; i < 16; i++) k[i] = rand() % 256;
    for(i = 0; i < 16; i++) iv[i] = rand() % 256;
  }

  // Build string
  unsigned char buff[8192] = {0};
  memcpy(buff, prestr, strlen(prestr));
  memcpy(buff + strlen(prestr), in, inl);
  memcpy(buff + strlen(prestr) + inl, poststr, strlen(poststr));

  // Remove metacharacters
  int i;
  for(i = strlen(prestr); i < strlen(prestr) + inl; i++)
    if(buff[i] == '=' || buff[i] == ';') buff[i] = '?';

  // Encrypt
  size_t l = pkcs7(buff, inl + strlen(prestr) + strlen(poststr), 16);
  return encrypt_aes128cbc(buff, l, out, k, iv);
}

int is_admin(unsigned char * in, int inl)
{
  // Decrypt
  unsigned char buff[8192] = {0};
  size_t l = decrypt_aes128cbc(in, inl, buff, k, iv);
  printstr(buff, l);

  // Find ';admin=true;'
  static const char * adminstr = ";admin=true;\0";
  size_t n = strlen(adminstr);
  unsigned char *ptr = buff, *end = buff + l;
  while(++ptr < end && memchr(ptr, ';', end - ptr) != NULL)
    if(strncmp(ptr, adminstr, n) == 0) return 1;

  return 0;
}

int main(void)
{
  unsigned char buff[8192] = {0};

  char * in = "AAAAAAAAAAAAAAAAXadminXtrue";
  size_t l = ORACLE(in, strlen(in), buff);
  // Bit flip
  buff[32] ^= 0x63;
  buff[38] ^= 0x65;
  printf("Is admin = %s\n", is_admin(buff, l) ? "True" : "False");

  return 0;
}
