#include "stringutils.h"

void printhex(const unsigned char * bytes, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++) printf("%02x", bytes[i]);
  putchar('\n');
}

void printb64(const unsigned char * bytes, size_t len)
{
  // Character table
  static const char * b64table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  // Buffers
  unsigned char ibuff, obuff, ibuffl;
  ibuff = bytes[0];
  ibuffl = 8;

  size_t i = 0;
  while(i < len)
  {
    obuff = 0;
    // Fetch next 6 bits
    if(ibuffl == 0)
    {
      i++;
      if(!(i < len)) break;
      ibuff = bytes[i];
      obuff = bytes[i] & 0xFC;
      ibuffl = 2;
    }
    else if(ibuffl == 2)
    {
      obuff = ibuff << 6;
      i++;
      if(i < len)
      {
        ibuff = bytes[i];
        obuff |= ibuff >> 2 & 0x3C;
        ibuffl = 4;
      }
    }
    else if(ibuffl == 4)
    {
      obuff = ibuff << 4;
      i++;
      if(i < len)
      {
        ibuff = bytes[i];
        obuff |= ibuff >> 4 & 0x0C;
        ibuffl = 6;
      }
    }
    else if(ibuffl == 6)
    {
      obuff = ibuff << 2;
      ibuffl = 0;
    }
    else if(ibuffl == 8)
    {
      obuff = ibuff & 0xFC;
      ibuffl = 2;
    }

    putchar(b64table[obuff >> 2]);
  }

  if(ibuffl == 2)
  {
    putchar('=');
  }
  else if(ibuffl == 4)
  {
    putchar('=');
    putchar('=');
  }

  putchar('\n');
}

size_t readhex(const char * str, unsigned char * out, size_t max)
{
  // Basic initial checks
  if(max < strlen(str) / 2 || strlen(str) % 2) return 0;

  // Parse
  size_t i;
  for(i = 0; i < strlen(str); i += 2)
    sscanf(&str[i], "%2hhx", &out[i / 2]);

  // Return decoded bytes
  return strlen(str) / 2;
}

size_t readb64(const char * str, unsigned char * out, size_t max)
{
  // Calculate required space
  size_t len = strlen(str);
  size_t req = 3 * len / 4 - (str[len - 1] == '=' ? (str[len - 2] == '=' ? 2 : 1) : 0);

  // Basic initial checks
  if(max < len || len % 4) return 0;

  // Decode
  static const int b64table[256] =
  {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  62, 63, 62, 62, 63,
      52, 53, 54, 55, 56, 57, 58, 59, 60, 61,-1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,-1, -1, -1, -1,  63,
     -1,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
  };

  int val =-1, valb = -8;
  size_t i, j = 0;
  for(i = 0; i < len; i++)
  {
    char c = str[i];
    if(b64table[c] == -1) return 0;
    val = (val << 6) + b64table[c];
    valb += 6;
    if(valb >= 0)
    {
      out[j++] = ((val >> valb) & 0xFF);
      valb -= 8;
    }
  }

  // Return decoded bytes
  return req;
}
