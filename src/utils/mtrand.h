#ifndef __MRAND_H__
#define __MRAND_H__

#include <stdint.h>

void mtseed(uint32_t seed);

uint32_t mtrand();

#endif /* end of include guard: __MRAND_H__ */
