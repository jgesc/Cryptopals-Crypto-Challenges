#include "../../utils/crypto.h"
#include "../../utils/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Oracle function, receives an input string and
// outputs AES-128-ECB(your-string || unknown-string, random-key)
// Should be treated as a black box
int ORACLE(const unsigned char * in, size_t inl, unsigned char * out)
{
  static char initialized = 0;
  static char k[16];
  static unsigned char * str;
  static size_t strl;
  if(!initialized)
  {
    initialized = 1;

    // Initialize random seed
    srand(time(NULL));
    // Generate random key
    int i;
    for(i = 0; i < 16; i++) k[i] = rand() % 256;

    strl = freadb64("../../res/12.txt", &str);
  }

  unsigned char buff[8192];
  memcpy(buff, in, inl);
  memcpy(buff + inl, str, strl);
  return encrypt_aes128ecb(buff, inl + strl, out, k);
}

int main(void)
{
  /// Find block and message size
  char buffer[8192];
  int basesize = ORACLE(buffer, 0, buffer);
  int fullblksize;
  int blksize;
  // Fill last block
  for(blksize = 0; blksize < 8192; blksize++)
  {
    int newsize = ORACLE(buffer, blksize, buffer);
    if(basesize < newsize)
    {
      fullblksize = newsize;
      break;
    }
  }
  int padding = blksize; // Padding found
  int msglen = basesize - padding;
  printf("Secret message length is %d\n", msglen);
  // Count bytes required to create a new block
  for(; blksize < 8192; blksize++)
    if(fullblksize < ORACLE(buffer, blksize, buffer))
      break;
  blksize -= padding;
  printf("Block size is %d\n", blksize);

  /// Detect if the function is using ECB
  char zeros[512] = {0};
  int l = ORACLE(zeros, 512, buffer);
  if(detect_ebc(buffer, l))
    printf("The function IS using ECB\n");
  else
  {
    printf("ERROR: The function is NOT using ECB\n");
    return 1;
  }

  /// Break ECB
  // Initialize buffers
  // Crafted input to send
  char * craftedin = malloc(blksize - 1);
  memset(craftedin, 'A', blksize - 1);
  // Dictionary
  unsigned char * dict = malloc(256 * blksize + 8192); // 8192 bytes of extra space
  // Decoding block
  char * decblk = malloc(blksize);
  memcpy(decblk, craftedin, blksize - 1);

  // Break one byte at a time
  printf("Decoded message:\n\n");
  int i, j;
  for(i = 0; i < msglen; i++)
  {
    // Generate dictionary
    for(j = 0; j < 256; j++)
    {
      decblk[blksize - 1] = (char)j; // Modify last byte
      char * dictentry = dict + j * blksize; // Point to store location
      ORACLE(decblk, blksize, dictentry); // Call the oracle function
    }

    // Crafted block bytes to send
    int craftedlen = blksize - 1 - (i % (blksize));
    // Call the oracle function
    ORACLE(craftedin, craftedlen, buffer);
    // Point to the block we are going to decrpyt
    unsigned char * currblk = buffer + i / blksize * blksize;
    // Look for block in dictionary
    for(j = 0; j < 256; j++)
    {
      char * dictentry = dict + j * blksize;
      if(strncmp(currblk, dictentry, blksize) == 0)
        break;
    }
    if(j == 256)
    {
      printf("Error: can't decode block %d\n", i);
      return 1;
    }
    putchar(j);

    // Shift decblk to the left
    decblk[blksize - 1] = j;
    for(j = 0; j < blksize - 1; j++)
      decblk[j] = decblk[j + 1];
  }

  putchar('\n');

  free(craftedin);
  free(dict);
  free(decblk);

  return 0;
}
