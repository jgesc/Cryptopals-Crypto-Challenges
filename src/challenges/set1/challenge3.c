#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"
#include "../../utils/textutils.h"

#include <stdio.h>
#include <stdlib.h>

struct tuple
{
  char key;
  float error;
};

int tuplecmp(const void * a, const void * b)
{
  const struct tuple *a2 = a;
  const struct tuple *b2 = b;

  if(a2->error < b2->error) return -1;
  if(a2->error == b2->error) return 0;
  if(a2->error > b2->error) return 1;
}

int main(void)
{
  const int BUFF_SIZE = 1024;
  unsigned char ciph[BUFF_SIZE];
  size_t bytelen;

  // Decode hex
  char * hexstr = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  bytelen = readhex(hexstr, ciph, BUFF_SIZE);

  // Decode and score
  struct tuple scores[256];
  int key;
  for(key = 0; key < 256; key++)
  {
    unsigned char buff[BUFF_SIZE];
    scores[key].key = key;
    sbxorarray(ciph, key, buff, bytelen);
    scores[key].error = scoretext(buff, bytelen, 1);
  }

  qsort(scores, 256, sizeof(struct tuple), tuplecmp);

  unsigned char i;
  for(i = 0; i < 256; i++)
  {
    unsigned char buff[BUFF_SIZE];
    buff[bytelen] = '\0';
    key = scores[i].key;
    sbxorarray(ciph, key, buff, bytelen);
    printf("---\nKey: %02x\nError: %6.4f\n\n%s\n\nPress 'q' to quit, any other key to continue...\n",
      key, scores[i].error, buff);

    char opt = getchar();
    if(opt == 'q') break;
  }

  return 0;
}
