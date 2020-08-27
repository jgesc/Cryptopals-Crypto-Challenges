#include "file.h"

size_t freadb64(const char * path, unsigned char ** out)
{
  // Load file in memory
  char * b64;
  FILE * f = fopen(path, "r");
  if(!f)
  {
    printf("ERROR: File not found\n");
    return 0;
  }
  fseek(f, 0, SEEK_END);
  size_t fsize = ftell(f);
  rewind(f);
  b64 = malloc(fsize + 1);
  *out = malloc(fsize * 3 / 4);
  size_t i = 0;
  while(!feof(f))
  {
    char ch = fgetc(f);
    if(ch != '\n' && ch > 0)
      b64[i++] = ch;
  }
  b64[i] = '\0';
  fclose(f);
  size_t clen = readb64(b64, *out, fsize * 3 / 4);
  if(!clen)
  {
    printf("ERROR: Can't decode base64\n");
    return 0;
  }
  free(b64);

  return clen;
}
