#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"

#include <string.h>

int main(void)
{
  // Buffer
  const int BUFF_SIZE = 1024;
  unsigned char buff[BUFF_SIZE];

  // Plaintext
  const char * p = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  const char * k = "ICE";

  // Xor and print
  repxorarray(p, strlen(p), k, strlen(k), buff);
  printhex(buff, strlen(p));

  return 0;
}
