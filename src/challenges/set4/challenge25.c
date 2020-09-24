#include "../../utils/crypto.h"
#include "../../utils/file.h"
#include "../../utils/stringutils.h"
#include "../../utils/xorutils.h"

#include <stdlib.h>

const char * PLAIN_PATH = "../../res/25.txt";

static uint8_t k[16];
static uint64_t nonce = 1;

size_t getciphertext(uint8_t * c)
{
  // Generate random key and nonce
  int i;
  for(i = 0; i < 16; i++) k[i] = rand();
  for(i = 0; i < 16; i++) nonce *= rand();

  // Load file
  unsigned char * fcont;
  size_t l = freadb64(PLAIN_PATH, &fcont);

  // Decrypt ECB
  char p[8192];
  decrypt_aes128ecb(fcont, l, p, "YELLOW SUBMARINE");
  l -= pkcs7strip(p, l);

  // Encrypt contents
  aes128ctr(p, l, c, k, nonce);

  // Return
  free(fcont);
  return l;
}

void edit(uint8_t * c, size_t offset, uint8_t * newtext, size_t l)
{
  // Calculate block offset
  size_t iblkoff = offset % 16;
  size_t iblkbeg = offset - iblkoff;
  size_t eblkoff = offset + l + (16 - l % 16);
  size_t buffl = eblkoff - iblkbeg;

  // Allocate buffer and decrypt
  uint8_t * buff = malloc(buffl);
  aes128ctroff(c + iblkbeg, buffl, buff, k, nonce, iblkbeg / 16);

  // Modify plaintext
  memcpy(buff, newtext, l);

  // Re-encrypt
  aes128ctroff(buff, buffl, c + iblkbeg, k, nonce, iblkbeg / 16);
}

int main(void)
{
  // Get ciphertext
  uint8_t c[8192];
  size_t l = getciphertext(c);

  // Write all 0's and get raw stream
  uint8_t zeros[8192];
  uint8_t stream[8192];
  edit(stream, 0, zeros, l);

  // Xor original ciphertext with stream and get plaintext
  uint8_t p[8192];
  xorarray(c, stream, p, l);

  // Print results
  puts(p);

  return 0;
}
