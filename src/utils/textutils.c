#include "textutils.h"

float scoretext(char * in, size_t len, int strict)
{
  // Character hits
  unsigned int hits[27] = {0};

  // Count character occurrences
  size_t i;
  for(i = 0; i < len; i++)
  {
    // Discard if contains nonprintable characters
    if(strict && !isprint(in[i])) return FLT_MAX;
    if(isalpha(in[i]))
      hits[toupper(in[i]) - 65]++;
    else
      hits[26]++;
  }

  // Score character frequency
  float model[] = {0.082, 0.015, 0.028, 0.043, 0.13, 0.022, 0.02, 0.061, 0.07,
    0.0015, 0.0077, 0.04, 0.024, 0.067, 0.075, 0.018, 0.00095, 0.06, 0.063,
    0.091, 0.028, 0.0098, 0.024, 0.015, 0.02, 0.00074, 0.0};

  // Calculate squared error from model
  float error = 0;
  for(i = 0; i < 27; i++) error += (model[i] - (float)hits[i] / len) * (model[i] - (float)hits[i] / len);

  return error;
}
