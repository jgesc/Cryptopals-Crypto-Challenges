#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdio.h>

// Constants
const char * PTXT_PATH = "../../res/17.txt"; // Plaintext list

// Static variables
static unsigned char k[16], _iv[16]; // Key, IV

static int pcnt = 0; // Plaintexts count
static char * ps[64]; // Plaintext pointer array
static char pbuff[8192]; // Plaintext buffer

// Init routine
void init()
{
  static int initf = 0; // Init flag
  if(initf) return; // Init ward

  initf = 1; // Check flag

  // Initialize random seed
  srand(time(NULL));
  // Generate random key and iv
  int i;
  for(i = 0; i < 16; i++) k[i] = rand() % 256;
  for(i = 0; i < 16; i++) _iv[i] = rand() % 256;

  // Read plaintexts
  FILE * f = fopen(PTXT_PATH, "r");

  char c; // Character register
  char ** ptr = ps; // Pointer array register
  *ptr = pbuff; // Initialize first pointer
  while(!feof(f))
  {
    int i = 0; // Buffer pointer register
    char buff[8192] = {0}; // Input buffer
    // Read each character
    while((c = fgetc(f)) > 47)
      buff[i++] = c;

    // Check EOF
    if(feof(f)) break;

    // Decode
    size_t l = readb64(buff, *ptr, 8192 - (*ptr - pbuff));
    l = pkcs7(*ptr, l, 16);
    (*ptr)[l] = '\0'; // End of line

    // Set next pointer
    ptr++;
    *ptr = *(ptr - 1) + l + 1;

    // Increase plaintext count
    pcnt++;
  }

  fclose(f);
}

int getcookie(unsigned char * out, unsigned char * iv)
{
  init(); // Ensure is initialized

  // Choose plaintext
  char * p = ps[rand() % pcnt];

  // Copy IV
  memcpy(iv, _iv, 16);

  // Encrypt
  return encrypt_aes128cbc(p, strlen(p), out, k, _iv);
}

int chkcookiepad(const unsigned char * in, int inl, unsigned char * iv)
{
  init(); // Ensure is initialized

  // Decrypt cookie
  unsigned char buff[8192];
  int l = decrypt_aes128cbc(in, inl, buff, k, iv);
  return pkcs7strip(buff, l) > 0; // Check padding and return
}

int main(void)
{
  // Buffers
  unsigned char cbuff[8192];
  unsigned char iv[16];
  unsigned char pbuff[8192];

  // Get ciphertext and IV
  int l = getcookie(cbuff, iv);

  // Decrypt ciphertext
  unsigned char * xorblk = iv;
  unsigned char * targetblk = cbuff;
  unsigned char * outblk = pbuff;

  // Iterate each block
  int iblck, ibyte;
  for(iblck = 0; iblck < l / 16; iblck++)
  {
    // Iterate each byte
    for(ibyte = 0; ibyte < 16; ibyte++)
    {
      // Set decoding buffer to match known bytes with padding
      char decbuff[16] = {0};
      int i;
      for(i = 15; i > 15 - ibyte; i--)
        decbuff[i] = xorblk[i] ^ outblk[i] ^ (ibyte + 1);

      // Find byte
      int byte;
      for(byte = 0; byte < 256; byte++)
      {
        decbuff[15 - ibyte] = byte;
        if(chkcookiepad(targetblk, 16, decbuff)) break;
      }
      outblk[15 - ibyte] = (ibyte + 1) ^ byte ^ xorblk[15 - ibyte]; // Save byte
    }

    // Update pointers
    xorblk = cbuff + 16 * iblck;
    targetblk = xorblk + 16;
    outblk = pbuff + 16 * (iblck + 1);
  }

  // Print decrypted plaintext
  int plen;
  while(pbuff[++plen] > 16) continue; // Find end of plaintext
  printstr(pbuff, plen);

  return 0;
}
