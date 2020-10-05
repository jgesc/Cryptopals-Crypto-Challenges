#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "md4.h"
#include "mtrand.h"
#include "xorutils.h"
#include "sha1.h"

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

void aes128ctroff(const unsigned char * in, size_t l, unsigned char * out,
  const unsigned char * k, uint64_t nonce, uint64_t offset);

void mtcrypt(const unsigned char * in, size_t l, unsigned char * out,
  uint32_t k);

void macsha1(const uint8_t * data, size_t l, uint8_t out[20],
  const uint8_t key[16]);

int macsha1chk(const uint8_t * data, size_t l, const uint8_t mac[20],
  const uint8_t key[16]);

void macmd4(uint8_t * data, size_t l, uint8_t out[16], uint8_t key[16]);

int macmd4chk(uint8_t * data, size_t l, uint8_t mac[16], uint8_t key[16]);

void hmacsha1(const uint8_t * data, size_t l, uint8_t out[20],
  const uint8_t key[64]);

int hmacsha1chk(const uint8_t * data, size_t l, const uint8_t mac[20],
  const uint8_t key[64]);

#endif /* end of include guard: __CRYPTO_H__ */
