#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "xorutils.h"

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

size_t pkcs7(unsigned char * buff, size_t msglen, size_t bsize);

int encrypt_aes128ecb(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k);

int decrypt_aes128ecb(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k);

size_t encrypt_aes128cbc(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k, const unsigned char * iv);

size_t decrypt_aes128cbc(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k, const unsigned char * iv);

int detect_ebc(const unsigned char * c, size_t cl);

int pkcs7strip(unsigned char * p, size_t l);

void aes128ctr(const unsigned char * in, size_t l, unsigned char * out,
  const unsigned char * k, uint64_t nonce);

#endif /* end of include guard: __CRYPTO_H__ */
