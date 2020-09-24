#include "../../utils/crypto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
  /// 1. Test mtcrypt()
  // Key
  uint32_t testk = 98761234;
  // Plaintext
  char * testp = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, \
    sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
  // Get plaintext length
  size_t testl = strlen(testp) + 1;
  // Ciphertext buffer
  uint8_t testc[8192];
  // Encrypt plaintext store in buffer
  mtcrypt(testp, testl, testc, testk);
  // Decryption buffer
  uint8_t testout[8192];
  // Decrypt ciphertext store in buffer
  mtcrypt(testc, testl, testout, testk);
  // Check plaintext and decryption output are the same
  printf("Encryption-Decryption test %s\n",
    strcmp(testp, testout) == 0 ? "OK" : "FAIL");

  /// 2. 16-bit bruteforce
  // Initialize PRNG
  srand(time(NULL));
  // Get 16-bit key
  uint16_t bfk = rand();
  // Input buffer
  char bfin[8192] = {0};
  // Randomize prefix
  uint16_t prel = rand() % 1024;
  int i;
  for(i = 0; i < prel; i++) bfin[i] = 48 + rand() % 20;
  // Append known plaintext
  memset(bfin + prel, 'A', 14);
  // Encrypt
  uint8_t bfc[8192];
  size_t bfl = prel + 14;
  mtcrypt(bfin, bfl, bfc, bfk);
  // Bruteforce
  uint8_t bfout[8192];
  for(i = 0; i < (1 << 16); i++)
  {
    mtcrypt(bfc, bfl, bfout, i);
    if(memcmp(bfout + prel, "AAAAAAAAAAAAAA", 14) == 0) break;
  }
  // Check results
  printf("Bruteforce test %s\n",
    i == bfk ? "OK" : "FAIL");

  /// 3. Current time seeded test
  // Token buffer
  uint8_t tkn[32];
  // Seed current time
  mtseed(time(NULL));
  // Generate token
  mtrandarr((uint32_t*)tkn, 32 / sizeof(uint32_t));
  // Wait random time
  sleep(2 + rand() % 10);
  // Check if PRNG was seeded current time
  uint32_t tbase = time(NULL);
  uint8_t buff[32];
  for(i = 0; i < 60; i++)
  {
    mtseed(tbase - i);
    mtrandarr((uint32_t*)buff, 32 / sizeof(uint32_t));
    if(memcmp(buff, tkn, 32) == 0) break;
  }
  // Check results
  printf("Find if PRNG was seeded with current time test %s\n",
    i < 60 ? "OK" : "FAIL");

  return 0;
}
