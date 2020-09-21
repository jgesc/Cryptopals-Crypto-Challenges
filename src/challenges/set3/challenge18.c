#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
  char b64c[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/"
                "kXX0KSvoOLSFQ=="; // Base64 encoded ciphertext
  unsigned char c[512] = {0}; // Binary ciphertext

  // Decode ciphertext
  size_t l = readb64(b64c, c, 512);

  // Decrypt ciphertext
  unsigned char p[512] = {0};
  aes128ctr(c, l, p, "YELLOW SUBMARINE", 0);

  // Print plaintext
  puts(p);

  // Test encryption-decryption
  char teststr[] = "This is a test string";
  char testk[16] = "00TESTPASSWORD11";
  uint64_t testnonce = 9867 * 9679;
  size_t testl = sizeof(teststr);

  unsigned char testc[512];
  aes128ctr(teststr, testl, testc, testk, testnonce);
  unsigned char testp[512] = {0};
  aes128ctr(testc, testl, testp, testk, testnonce);
  puts(testp);

  return 0;
}
