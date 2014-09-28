#ifndef __METHOD_BLOCK_H__
#define __METHOD_BLOCK_H__

#include <stdio.h>

#define METHOD_EXPORT_SECTION ".mex"
#ifdef _MSC_VER
#define METHOD_EXPORT_API
#else // default __GNUC__
#define METHOD_EXPORT_API __attribute__((section(METHOD_EXPORT_SECTION)))
#endif // _MSC_VER

#define METHOD_EXPORT_START
#define METHOD_END_FLAG   __method_end
#define METHOD_EXPORT_END void METHOD_END_FLAG(char* k, char* v){}

typedef struct _method_block_header {
    unsigned short header_length;
    unsigned char key;
    unsigned char compress_type;    // null: 0x00   lzss: 0x01   ...
    unsigned char instruction_type; // unknown: 0x00   x86: 0x01   arm32: 0x02   ...
    unsigned char reserved;
    unsigned short data_length;
    unsigned short offsets[0];
} method_block_header, *method_block_header_pointer;

typedef void (*method_entry) (char* , char* );

// TODO:
// void create_method_block(char* , size_t* , method_entry* , size_t , unsigned char , unsigned char );

void* get_method_entries(char* , size_t , method_entry* , size_t* );

void release_method_block(void* );

#endif  // __METHOD_BLOCK_H__