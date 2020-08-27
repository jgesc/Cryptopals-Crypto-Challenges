#include <stdio.h>
#include <string.h>

#define NLINES 204
#define LINELEN 320

int main(void)
{
  // Read file
  char * path = "../../res/8.txt";
  char txt[NLINES][LINELEN + 1];

  FILE * f = fopen(path, "r");

  int i;
  for(i = 0; i < NLINES; i++)
  {
    fgets(txt[i], LINELEN + 1, f);
    txt[i][LINELEN] = '\0';
    fgetc(f); // Read end of line character
  }
  fclose(f);

  // Look for repeating patterns
  int line, j;
  for(line = 0; line < NLINES; line++)
    for(i = 0; i < LINELEN / 32 - 1; i++)
      for(j = i + 1; j < LINELEN / 32; j++)
        if(strncmp(txt[line] + i * 32, txt[line] + j * 32, 32) == 0)
        {
          char buff[33];
          strncpy(buff, txt[line] + i * 32, 32);
          buff[32] = '\0';
          printf("Repeating pattern in line %3d(%3d): '%s'\n", line, i*32, buff);
        }

  return 0;
}
