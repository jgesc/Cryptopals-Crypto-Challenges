#ifndef _MD4_H
#define _MD4_H

#include <stdint.h>
#include <stdlib.h>

#define uchar uint8_t

typedef struct
{
    uint32_t total[2];
    uint32_t state[4];
    uchar buffer[64];
}
md4_context;

/*
 * Core MD4 functions
 */
void md4_starts( md4_context *ctx );
void md4_update( md4_context *ctx, uchar *input, uint length );
void md4_finish( md4_context *ctx, uchar digest[16] );

/*
 * Output MD5(file contents), returns 0 if successful.
 */
int md4_file( char *filename, uchar digest[16] );

/*
 * Output MD4(buf)
 */
void md4_csum( uchar *buf, uint buflen, uchar digest[16] );

/*
 * Output HMAC-MD4(buf,key)
 */
void md4_hmac( uchar *buf, uint buflen, uchar *key, uint keylen,
                uchar digest[16] );

/*
 * Checkup routine
 */
int md4_self_test( void );

#endif /* md4.h */
