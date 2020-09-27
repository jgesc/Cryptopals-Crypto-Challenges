#include <string.h>

#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

int main(void)
{
  char data[] = "The quick brown fox jumps over the lazy dog";
  uint8_t mac[20];
  uint8_t k[16] = "YELLOW SUBMARINE";

  // Calculate MAC
  macsha1(data, strlen(data), mac, k);
  printf("Calculate mac for string '%s' with key ", data);
  printstr(k, 16);
  printstr(mac, 20);
  putchar('\n');

  // Check MAC
  printf("Checking MAC: %s\n",
    macsha1chk(data, strlen(data), mac, k) ? "CORRECT" : "INCORRECT");

  // Check MAC for tampered message
  data[40] = 'c';
  printf("Check MAC for tampered message '%s'\n", data);
  printf("Checking MAC: %s\n",
    macsha1chk(data, strlen(data), mac, k) ? "CORRECT" : "INCORRECT");

  return 0;
}
