#include "xorkeyfinder.h"

unsigned char findxorkey(unsigned char * c, size_t clen, size_t klen, size_t offset, int strict)
{
  // Character frequency model
  static const float model[] = {0.082, 0.015, 0.028, 0.043, 0.13, 0.022, 0.02,
    0.061, 0.07, 0.0015, 0.0077, 0.04, 0.024, 0.067, 0.075, 0.018, 0.00095,
    0.06, 0.063, 0.091, 0.028, 0.0098, 0.024, 0.015, 0.02, 0.00074, 0.0};

  // Characters per fragment
  size_t flen = (clen - offset) / klen;

  // Best key registers
  float minerr = FLT_MAX;
  unsigned char minerrk = 0;

  // Search keyspace
  int key;
  for(key = 0; key < 256; key++)
  {
    int brkflag = 0;
    // Character hits
    unsigned int hits[27] = {0};

    int i; // Iterator

    // Calculate character frequencies
    for(i = 0; i < flen; i++)
    {
      char pch = c[offset + i * klen] ^ key;
      // Discard if contains nonprintable characters
      if(strict && !isprint(pch) && !isspace(pch))
      {
        brkflag = 1;
        break;
      }
      if(isalpha(pch))
        hits[toupper(pch) - 65]++;
      else
        hits[26] += 10;
    }

    // Check brkflag
    if(brkflag) continue;

    // Calculate error
    float error = 0;
    for(i = 0; i < 27; i++) error += (model[i] - (float)hits[i] / flen) * (model[i] - (float)hits[i] / flen);

    // Compare with best key
    if(error < minerr)
    {
      minerr = error;
      minerrk = key;
    }
  }

  return (unsigned char)minerrk;
}
