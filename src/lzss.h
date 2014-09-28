#ifndef __LZSS_H__
#define __LZSS_H__

size_t lzss_encode(unsigned char* , size_t , unsigned char* , size_t );
size_t lzss_decode(unsigned char* , size_t , unsigned char* , size_t );

#endif  // __LZSS_H__