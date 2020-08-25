#include "../../utils/stringutils.h"

#include <stdio.h>

int main(void)
{
  const int BUFF_SIZE = 1024;
  unsigned char buffer[BUFF_SIZE];
  size_t bytelen;

  // Read hex
  char * hexstr = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  if(!(bytelen = readhex(hexstr, buffer, BUFF_SIZE)))
  {
    printf("Error reading hex string\n");
    return 1;
  }

  // Print b64
  printb64(buffer, bytelen);

  return 0;
}
