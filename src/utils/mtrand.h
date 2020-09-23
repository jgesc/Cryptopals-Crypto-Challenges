#ifndef __MRAND_H__
#define __MRAND_H__

#include <stdint.h>
#include <stdlib.h>

void mtseed(uint32_t seed);

uint32_t mtrand();

uint32_t * mtgetintstate();

void mtrandarr(uint32_t * arr, size_t n);

uint32_t mtuntemper(uint32_t y);

#endif /* end of include guard: __MRAND_H__ */
