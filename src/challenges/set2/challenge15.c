#include "../../utils/crypto.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
  // Test counters
  int testc = 3;
  int suctest = 0;

  // Test strings
  char str1[] = "ICE ICE BABY\x04\x04\x04\x04";
  char str2[] = "ICE ICE BABY\x05\x05\x05\x05";
  char str3[] = "ICE ICE BABY\x01\x02\x03\x04";

  /// Tests
  char * str;
  // Test 1
  str = str1;
  if(pkcs7strip(str, strlen(str)) == 4 && strlen(str) == 12)
    suctest++;

  // Test 2
  str = str2;
  if(pkcs7strip(str, strlen(str)) == 0 && strlen(str) == 16)
    suctest++;

  // Test 3
  str = str3;
  if(pkcs7strip(str, strlen(str)) == 0 && strlen(str) == 16)
    suctest++;

  printf("Test results: %d/%d\n", suctest, testc);

  return 0;
}
