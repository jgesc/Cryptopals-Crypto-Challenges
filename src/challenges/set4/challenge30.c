#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUT_UINT32_LE(n,b,i)                    \
{                                               \
    (b)[(i)    ] = (uchar) ( (n)       );       \
    (b)[(i) + 1] = (uchar) ( (n) >>  8 );       \
    (b)[(i) + 2] = (uchar) ( (n) >> 16 );       \
    (b)[(i) + 3] = (uchar) ( (n) >> 24 );       \
}

static uchar md4_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char tkn[] = "comment1=cooking%20MCs;userdata=foo;comment2=%20like"
  "%20a%20pound%20of%20bacon";
static uint8_t k[16];

void gettoken(uint8_t mac[16])
{
  // Generate random key
  srand(time(NULL));
  int i;
  for(i = 0; i < 16; i++) k[i] = rand();

  // Generate MAC
  macmd4(tkn, strlen(tkn), mac, k);
}

size_t gluepadding(size_t l, uint8_t * out)
{
  uint32_t total[2];

  uint32_t left, fill;

  left = total[0] & 0x3F;
  fill = 64 - left;

  total[0] += l;
  total[0] &= 0xFFFFFFFF;

  if( total[0] < l )
      total[1]++;


  uint32_t last, padn;
  uint32_t high, low;
  uchar msglen[8];

  high = ( total[0] >> 29 )
       | ( total[1] <<  3 );
  low  = ( total[0] <<  3 );

  PUT_UINT32_LE( low,  msglen, 0 );
  PUT_UINT32_LE( high, msglen, 4 );

  last = total[0] & 0x3F;
  padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

  memcpy(out, md4_padding, padn);
  memcpy(out+padn, msglen, 8);
  //md4_update( ctx, md4_padding, padn );
  //md4_update( ctx, msglen, 8 );

  return padn + 8;
}

int chktkn(uint8_t * msg, size_t msglen, uint8_t mac[16])
{
  return macmd4chk(msg, msglen, mac, k);
}

int main(void)
{
  // Get token MAC
  uint8_t tknmac[16];
  gettoken(tknmac);
  printf("For token:\n\t%s\nGot MAC:\n\t", tkn);
  printhex(tknmac, 16);
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
  // Forge MD4 hash
  md4_context ctx;
  md4_starts(&ctx);
  uint32_t * state = (uint32_t*)ctx.state;
  uint32_t * _mac = (uint32_t*)tknmac;
  // Replace state registers
  int i;
  for(i = 0; i < 4; i++)
    state[i] = le32toh(_mac[i]);

  uint32_t left, fill;
  left = ctx.total[0] & 0x3F;
  fill = 64 - left;
  ctx.total[0] += frgtknl+16;
  ctx.total[0] &= 0xFFFFFFFF;
  if( ctx.total[0] < frgtknl+16 )
      ctx.total[1]++;

  // Append ';admin=True'
  char * tknappend = ";admin=true";
  md4_update(&ctx, tknappend, strlen(tknappend));
  memcpy(frgtkn+frgtknl, tknappend, strlen(tknappend));
  frgtknl += strlen(tknappend);
  uint8_t frgmac[16];
  md4_finish(&ctx, frgmac);
  // Print results
  printf("For token:\n\t");
  printstr(frgtkn, frgtknl);
  printf("Got MAC:\n\t");
  printhex(frgmac, 16);

  // Check results
  printf("\n\nChecking MAC validity: %s\n",
    chktkn(frgtkn, frgtknl, frgmac) ? "OK" : "FAIL");

  return 0;
}
