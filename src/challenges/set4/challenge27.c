#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdlib.h>
#include <string.h>

static const char * prestr = "comment1=cooking%20MCs;userdata=\0";
static const char * poststr = ";comment2=%20like%20a%20pound%20of%20bacon\0";

static char k[16], *iv;

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
    iv = k;
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

int is_admin(unsigned char * in, int inl, int * err, char * errmsg)
{
  // Decrypt
  unsigned char buff[8192] = {0};
  size_t l = decrypt_aes128cbc(in, inl, buff, k, iv);
  printstr(buff, l);

  // Check ASCII compliance
  int i;
  for(i = 0; i < l; i++)
  {
    if(buff[i] > 127)
    {
      if(err && errmsg)
      {
        *err = 1;
        memcpy(errmsg, buff, l);
      }

      return 0;
    }
  }

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

  // Get ciphertext with at least 3 blocks
  size_t l = ORACLE(NULL, 0, buff);
  // Set second block to 0 and copy first to third
  memset(buff+16, 0, 16);
  memcpy(buff+32, buff+0, 16);
  // Get plaintext from error
  int err;
  char errmsg[8192];
  is_admin(buff, l, &err, errmsg);
  if(!err)
  {
    puts("Error, please try again");
    return 1;
  }
  // Get key from error message
  char key[16];
  xorarray(errmsg, errmsg+32, key, 16);
  printf("Found key: ");
  printstr(key, 16);
  // Craft token
  char * p = "comment1=cooking%20MCs;"
    "admin=true;"
    "comment2=%20like%20a%20pound%20of%20bacon";
  l = encrypt_aes128cbc(p, strlen(p), buff, key, key);

  printf("Is admin = %s\n", is_admin(buff, l, NULL, NULL) ? "True" : "False");

  return 0;
}
