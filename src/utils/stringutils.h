#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <stdio.h>
#include <string.h>

/*  void printhex(const void * src, size_t len)
 *    Prints a raw byte array in hexadecimal
 *
 *  Parameters:
 *    src: byte array to print
 *    len: number of bytes to print
 */
void printhex(const void * src, size_t len);

/*  void printb64(const void * src, size_t len)
 *    Prints a raw byte array in base64
 *
 *  Parameters:
 *    src: byte array to print
 *    len: number of bytes to print
 */
void printb64(const void * src, size_t len);

// Convert hex string to byte array
size_t readhex(const char * str, unsigned char * out, size_t max);

// Convert base64 string to byte array
size_t readb64(const char * str, unsigned char * out, size_t max);

void printstr(const unsigned char * bytes, size_t len);

size_t b64len(const char * str);

#endif /* end of include guard: __STRINGUTILS_H__ */
