#include "crypto.h"

size_t pkcs7(unsigned char * buff, size_t msglen, size_t bsize)
{
  size_t i;
  size_t padd = (unsigned char)(bsize - msglen % bsize);
  if(padd > 255) printf("ERROR: Too much padding");
  for(i = 0; i < padd; i++) buff[msglen + i] = (unsigned char)padd;

  return msglen + padd;
}
