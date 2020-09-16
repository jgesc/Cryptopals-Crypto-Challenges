#include "../../utils/crypto.h"
#include "../../utils/file.h"
#include "../../utils/stringutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Oracle function, receives an input string and
// outputs AES-128-ECB(random-string || your-string || unknown-string, random-key)
// Should be treated as a black box
int ORACLE(const unsigned char * in, size_t inl, unsigned char * out)
{
  static char initialized = 0;
  static char k[16];
  static unsigned char * str;
  static size_t strl;
  static char randstr[256] = {0};
  static size_t randstrl;
  if(!initialized)
  {
    initialized = 1;

    // Initialize random seed
    srand(time(NULL));
    // Generate random key
    int i;
    for(i = 0; i < 16; i++) k[i] = rand() % 256;
    // Generate random string
    randstrl = rand() % 256;
    for(i = 0; i < randstrl; i++) randstr[i] = rand() % 256;

    strl = freadb64("../../res/12.txt", &str);
  }

  unsigned char buff[8192];
  memcpy(buff, randstr, randstrl);
  memcpy(buff + randstrl, in, inl);
  memcpy(buff + randstrl + inl, str, strl);
  return encrypt_aes128ecb(buff, inl + randstrl + strl, out, k);
}

int main(void)
{
  // Generic buffers/variables
  unsigned char buff1[8192], buff2[8192];
  int i, j;
  size_t l;

  /// Find prefix length
  // Find prefix block count
  memset(buff1, 'A', 512); // Pattern
  l = ORACLE(buff1, 512, buff2);
  for(i = 0; i < l / 16; i++)
  {
    for(j = i + 1; j < l / 16; j++)
      if(strncmp(buff2 + i * 16, buff2 + j * 16, 16) != 0) break; // Non-repeating block
    if(j > i + 4) break; // Repeating block found
  }
  int preblkc = i;
  unsigned char cpat[16]; // Store encrypted pattern
  memcpy(cpat, buff2 + preblkc * 16, 16);
  // Find prefix length
  for(i = 0; i < 16; i++)
  {
    ORACLE(buff1, i + 16, buff2);
    if(strncmp(buff2 + preblkc * 16, cpat, 16) == 0) break;
  }
  int prel = preblkc * 16 - i;
  printf("Prefix string length = %d\n", prel);
  int prepadd = preblkc * 16 - prel;
  printf("Bytes required to fill prefix block = %d\n", prepadd);

  /// Find suffix length
  l = ORACLE(NULL, 0, buff2);
  for(i = 0; i < 16; i++)
    if(ORACLE(buff1, i, buff2) != l) break;

  int msglen = l - i - prel;
  printf("Message string length = %d\n", msglen);

  /// Break ECB
  // Initialize buffers
  // Crafted input to send
  char inbuff[512];
  char * craft = inbuff + prepadd;
  // Dictionary
  unsigned char dict[256][16]; // 8192 bytes of extra space

  // Break one byte at a time
  printf("Decoded message:\n\n");
  for(i = 0; i < msglen; i++)
  {
    // Generate dictionary
    for(j = 0; j < 256; j++)
    {
      craft[15] = (char)j; // Modify last byte
      char * dictentry = dict[j]; // Point to store location
      char dicbuff[8192];
      ORACLE(inbuff, 16 + prepadd, dicbuff); // Call the oracle function
      memcpy(dictentry, dicbuff + preblkc * 16, 16);
    }

    // Crafted block bytes to send
    int craftedlen = prepadd + 16 - 1 - (i % (16));
    // Call the oracle function
    unsigned char buffer[8192];
    ORACLE(inbuff, craftedlen, buffer);
    unsigned char * buffmsg = buffer + preblkc * 16;
    // Point to the block we are going to decrpyt
    unsigned char * currblk = buffmsg + i / 16 * 16;
    // Look for block in dictionary
    for(j = 0; j < 256; j++)
    {
      unsigned char * dictentry = dict[j];
      if(memcmp(currblk, dictentry, 16) == 0)
        break;
    }
    if(j == 256)
    {
      printf("Error: can't decode block %d\n", i);
      return 1;
    }
    putchar(j);

    // Shift decblk to the left
    craft[16 - 1] = j;
    for(j = 0; j < 16 - 1; j++)
      craft[j] = craft[j + 1];
  }

  putchar('\n');

  return 0;
}
