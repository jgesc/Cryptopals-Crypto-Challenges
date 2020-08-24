#pragma #ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <stdio.h>

/*  void printhex(const unsigned char * bytes, size_t len)
 *    Prints a raw byte array in hexadecimal
 *
 *  Parameters:
 *    bytes: byte array to print
 *    len: number of bytes to print
 */
void printhex(const unsigned char * bytes, size_t len);

/*  void printb64(const unsigned char * bytes, size_t len)
 *    Prints a raw byte array in base64
 *
 *  Parameters:
 *    bytes: byte array to print
 *    len: number of bytes to print
 */
void printb64(const unsigned char * bytes, size_t len);

int fromhex(const char * str, unsigned char * out, size_t max);

int fromb64(const char * str, unsigned char * out, size_t max);

#endif /* end of include guard: __STRINGUTILS_H__ */
