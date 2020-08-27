#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"
#include "../../utils/xorkeyfinder.h"

#include <stdio.h>
#include <string.h>

#define MIN_KSIZE 2
#define MAX_KSIZE 40
#define KEYSIZEBLOCKS 10

struct hammtuple
{
  unsigned char keysize;
  float ndist;
};

int hammtuplecmp(const void * a, const void * b)
{
  const struct hammtuple *a2 = a;
  const struct hammtuple *b2 = b;

  if(a2->ndist < b2->ndist) return -1;
  if(a2->ndist == b2->ndist) return 0;
  if(a2->ndist > b2->ndist) return 1;
}

int main(void)
{
  const char * srcpath = "../../res/6.txt";

  // Load file in memory
  char * c_b64, * c;
  FILE * f = fopen(srcpath, "r");
  if(!f)
  {
    printf("ERROR: File not found\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  size_t fsize = ftell(f);
  rewind(f);
  c_b64 = malloc(fsize + 1);
  c = malloc(fsize * 3 / 4);
  size_t i = 0;
  while(!feof(f))
  {
    char ch = fgetc(f);
    if(ch != '\n' && ch > 0)
      c_b64[i++] = ch;
  }
  c_b64[i] = '\0';
  fclose(f);
  size_t clen = readb64(c_b64, c, fsize * 3 / 4);
  if(!clen)
  {
    printf("ERROR: Can't decode base64\n");
    return 1;
  }

  // Keysize analysis
  struct hammtuple ksizes[MAX_KSIZE - MIN_KSIZE];
  for(i = 0; i < MAX_KSIZE - MIN_KSIZE; i++)
  {
    size_t ksize = MIN_KSIZE + i;
    ksizes[i].keysize = ksize;
    ksizes[i].ndist = 0;

    int j;
    for(j = 0; j < KEYSIZEBLOCKS; j++);
      ksizes[i].ndist += (float)hamm(j * ksize + c, j * ksize + c + ksize, ksize) / (ksize);

    ksizes[i].ndist /= KEYSIZEBLOCKS;
  }
  qsort(ksizes, MAX_KSIZE - MIN_KSIZE, sizeof(struct hammtuple), hammtuplecmp);

  printf("Keysize analysis report:\n");
  for(i = 0; i < MAX_KSIZE - MIN_KSIZE; i++)
    printf("KEYSIZE %3d: %6.3f\n", ksizes[i].keysize, ksizes[i].ndist);
  printf("---\n");

  // Find password for each keysize
  for(i = 0; i < MAX_KSIZE - MIN_KSIZE; i++)
  {
    size_t ksize = ksizes[i].keysize;
    unsigned char key[MAX_KSIZE + 1];

    size_t j;
    for(j = 0; j < ksize; j++)
      key[j] = findxorkey(c, clen, ksize, j, 1);

    // Print results
    printf("\nKEYSIZE = %lu\nKEY = ", ksize);
    printhex(key, ksize);
    char buff[129];
    repxorarray(c, 128, key, ksize, buff);
    buff[128] = '\0';
    printf("%s...\n", buff);

    printf("Press 'q' to quit, any other key to continue...\n");
    char opt = getchar();
    if(opt == 'q') break;
  }

  // Free buffers
  free(c_b64);
  free(c);

  return 0;
}
