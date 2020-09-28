#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static char tkn[] = "comment1=cooking%20MCs;userdata=foo;comment2=%20like"
  "%20a%20pound%20of%20bacon";
static uint8_t k[16];

void gettoken(uint8_t mac[20])
{
  // Generate random key
  srand(time(NULL));
  int i;
  for(i = 0; i < 16; i++) k[i] = rand();

  // Generate MAC
  macsha1(tkn, strlen(tkn), mac, k);
}

size_t gluepadding(size_t l, uint8_t * out)
{
  int i;
  unsigned char finalcount[8];
  uint32_t count[2] = {0};
  unsigned char c;
  int j = 0;
  uint32_t k;

  k = count[0];
  if ((count[0] += l << 3) < k)
      count[1]++;
  count[1] += (l >> 29);

  for (i = 0; i < 8; i++)
  {
    finalcount[i] = (unsigned char) ((count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);      /* Endian independent */
  }

  c = 0200;
  out[j++] = c;
  k = count[0];
  if ((count[0] += 1 << 3) < k)
      count[1]++;
  count[1] += (1 >> 29);
  //SHA1Update(context, &c, 1);
  while ((count[0] & 504) != 448)
  {
    c = 0000;
    out[j++] = c;
    k = count[0];
    if ((count[0] += 1 << 3) < k)
        count[1]++;
    count[1] += (1 >> 29);
    //SHA1Update(context, &c, 1);
  }

  memcpy(out+j, finalcount, 8);
  j += 8;
  //SHA1Update(context, finalcount, 8); /* Should cause a SHA1Transform() */

  return j;
}

int chktkn(const uint8_t * msg, size_t msglen, const uint8_t mac[20])
{
  return macsha1chk(msg, msglen, mac, k);
}

int main(void)
{
  // Get token MAC
  uint8_t tknmac[20];
  gettoken(tknmac);
  printf("For token:\n\t%s\nGot MAC:\n\t", tkn);
  printhex(tknmac, 20);

  printf("\nForging MAC...\n\n");

  /// Forge MAC
  // Get glue padding
  uint8_t glue[512];
  size_t gluel;
  gluel = gluepadding(strlen(tkn) + 16, glue);
  // Build forged token
  char frgtkn[8192] = {0};
  size_t frgtknl = 0;
  memcpy(frgtkn, tkn, strlen(tkn));
  frgtknl += strlen(tkn);
  memcpy(frgtkn+frgtknl, glue, gluel);
  frgtknl += gluel;
  // Forge SHA1 hash
  SHA1_CTX ctx;
  SHA1Init(&ctx);
  uint32_t * state = ctx.state;
  uint32_t * _mac = (uint32_t*)tknmac;
  // Replace state registers
  int i;
  for(i = 0; i < 5; i++)
    state[i] = be32toh(_mac[i]);

  uint32_t j = ctx.count[0];
  if ((ctx.count[0] += (frgtknl+16) << 3) < j)
      ctx.count[1]++;
  ctx.count[1] += ((frgtknl+16) >> 29);

  // Append ';admin=True'
  char * tknappend = ";admin=true";
  SHA1Update(&ctx, tknappend, strlen(tknappend));
  memcpy(frgtkn+frgtknl, tknappend, strlen(tknappend));
  frgtknl += strlen(tknappend);
  uint8_t frgmac[20];
  SHA1Final(frgmac, &ctx);
  // Print results
  printf("For token:\n\t");
  printstr(frgtkn, frgtknl);
  printf("Got MAC:\n\t");
  printhex(frgmac, 20);

  // Check results
  printf("\n\nChecking MAC validity: %s\n",
    chktkn(frgtkn, frgtknl, frgmac) ? "OK" : "FAIL");
}
