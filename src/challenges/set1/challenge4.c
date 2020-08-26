#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"
#include "../../utils/textutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tuple
{
  char key;
  float error;
  unsigned int line;
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
  // Read and decode file
  #define NLINES 327
  #define LINELEN 60
  const char * srcpath = "../../res/4.txt";
  unsigned char lines[NLINES][LINELEN / 2];
  size_t linelen[NLINES];

  FILE * f = fopen(srcpath, "r");
  int i, j;
  for(i = 0; i < NLINES; i++)
  {
    char line[LINELEN + 2] = {0};
    fgets(line, LINELEN + 2, f);
    char * eol = strchr(line, '\n');
    *eol = '\0';
    linelen[i] = eol - line;
    readhex(line, lines[i], LINELEN / 2);
  }
  fclose(f);

  // Xor and score
  struct tuple scores[NLINES * 256];
  int key, line;
  for(line = 0; line < NLINES; line++)
  {
    for(key = 0; key < 256; key++)
    {
      scores[line * 256 + key].line = line;
      unsigned char buff[LINELEN / 2];
      scores[line * 256 + key].key = key;
      sbxorarray(lines[line], key, buff, linelen[line] / 2);
      scores[line * 256 + key].error = scoretext(buff, linelen[line] / 2, 1);
    }
  }

  // Sort
  qsort(scores, NLINES * 256, sizeof(struct tuple), tuplecmp);

  // Print
  for(i = 0; i < NLINES * 256; i++)
  {
    unsigned char buff[LINELEN / 2 + 1];
    buff[LINELEN / 2] = '\0';
    key = scores[i].key;
    sbxorarray(lines[scores[i].line], key, buff, linelen[scores[i].line] / 2);
    printf("---\nLine: %03d\nKey: %02x\nError: %6.4f\n\n%s\n\nPress 'q' to quit, any other key to continue...\n",
      scores[i].line, key, scores[i].error, buff);

    char opt = getchar();
    if(opt == 'q') break;
  }

  return 0;
}
