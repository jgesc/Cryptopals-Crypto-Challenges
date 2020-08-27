#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#define BLOCKSIZE 20

int main(void)
{
  char buff[256] = "YELLOW SUBMARINE";

  size_t msglen = pkcs7(buff, strlen(buff), BLOCKSIZE);
  printstr(buff, msglen);

  return 0;
}
