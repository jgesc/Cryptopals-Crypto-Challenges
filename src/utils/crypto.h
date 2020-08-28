#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "xorutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

size_t pkcs7(unsigned char * buff, size_t msglen, size_t bsize);

int encrypt_aes128ecb(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k);

int decrypt_aes128ecb(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k);

void encrypt_aes128cbc(const unsigned char * p, size_t pl, unsigned char * c, const unsigned char * k, const unsigned char * iv);

void decrypt_aes128cbc(const unsigned char * c, size_t cl, unsigned char * p, const unsigned char * k, const unsigned char * iv);

#endif /* end of include guard: __CRYPTO_H__ */