#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"
#include "../../utils/xorkeyfinder.h"
#include "../../utils/xorutils.h"

#include <stdio.h>

struct array
{
  size_t l;
  unsigned char data[512];
};

// Constants
const char * PTXT_PATH = "../../res/20.txt"; // Plaintext list

// Global variables
static struct array cs[128]; // Ciphertext array
static int n = 0; // Number of ciphertexts
static size_t maxl = 0; // Max ciphertext length

// Init routine
void init()
{
  // Initialize random seed
  srand(time(NULL));
  // Generate random key
  unsigned char k[16];
  int i;
  for(i = 0; i < 16; i++) k[i] = rand() % 256;

  // Read plaintexts
  FILE * f = fopen(PTXT_PATH, "r");

  char c; // Character register
  while(!feof(f))
  {
    int i = 0; // Buffer pointer register
    char buff[8192] = {0}; // Input buffer
    // Read each character
    while((c = fgetc(f)) > 31)
      buff[i++] = c;
    buff[i] = '\0';

    // Check EOF
    if(feof(f)) break;

    // Decode
    char p[512];
    size_t pl = readb64(buff, p, 512);
    if(!pl) continue;

    // Encrypt and store
    if(pl > maxl) maxl = pl;
    cs[n].l = pl;
    aes128ctr(p, pl, cs[n].data, k, 0);
    n++;
  }
  fclose(f);
}

int main(void)
{
  // Initialize
  init();

  // Try to find key
  unsigned char key[512]; // Predicted key
  int i, j, k;
  for(i = 0; i < maxl; i++)
  {
    // Build block from i-th byte for each ciphertext
    unsigned char buff[512];
    k = 0;
    for(j = 0; j < n; j++)
      if(i < cs[j].l) buff[k++] = cs[j].data[i];

    // Predict key
    key[i] = findxorkey(buff, k, 1, 0, 1);
  }

  // Print result
  for(i = 0; i < n; i++)
  {
    char p[512];
    xorarray(key, cs[i].data, p, cs[i].l);
    printstr(p, cs[i].l);
  }

  return 0;
}
