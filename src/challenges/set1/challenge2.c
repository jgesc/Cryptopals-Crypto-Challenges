#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"

int main(void)
{
  const int BUFF_SIZE = 1024;
  unsigned char buffa[BUFF_SIZE], buffb[BUFF_SIZE], buffc[BUFF_SIZE];
  size_t bytelen;

  // Read hex
  char * hexa = "1c0111001f010100061a024b53535009181c";
  char * hexb = "686974207468652062756c6c277320657965";
  bytelen = readhex(hexa, buffa, BUFF_SIZE);
  readhex(hexb, buffb, BUFF_SIZE);

  // Xor
  xorarray(buffa, buffb, buffc, bytelen);

  // Print b64
  printhex(buffc, bytelen);

  return 0;
}
