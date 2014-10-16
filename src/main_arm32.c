#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "methodblock.h"

/* METHOD_EXPORT_START
METHOD_EXPORT_API
void strongEncode(char* value, char* key) {
    unsigned char b = (*value ^ *key);
    b = (b << 4) + (b >> 4);
    b = ((b & 0x33) << 2) + ((b & 0xCC) >> 2);
    *value = ((b & 0x55) << 1) + ((b & 0xAA) >> 1);
    *value &= 0xFF;
    *key = *key + 1;
}

METHOD_EXPORT_API
void strongDecode(char* value, char* key) {
    unsigned char b = *value & 0xFF;
    b = (b << 4) + (b >> 4);
    b = ((b & 0x33) << 2) + ((b & 0xCC) >> 2);
    *value = ((b & 0x55) << 1) + ((b & 0xAA) >> 1);
    *value ^= *key;
    *key = *key + 1;
}
METHOD_EXPORT_END */

char shellcodes[] = {
    // method_block_header
    0x0C, 0x00, // header_length    : word
    0x00,       // key              : byte
    0x01,       // compress_type    : byte
    0x02,       // instruction_type : byte
    0x00,       // reserved         : byte
    0xA0, 0x00, // data_length      : word
    0x00, 0x00, 0x50, 0x00, // offsets : word []
    // method_block_data
    0xFF, 0x00, 0x20, 0xD1, 0xE5, 0x00, 0x30, 0xD0, 0xE5, 0xFF, 0x03, 0x30, 0x22, 0xE0, 0x03, 0x22,
    0xA0, 0xE1, 0xFF, 0x23, 0x32, 0x82, 0xE1, 0xCC, 0x20, 0x03, 0xE2, 0xFF, 0x22, 0x21, 0xA0, 0xE1,
    0x33, 0x30, 0x03, 0xE2, 0xDF, 0x03, 0x31, 0x82, 0xE0, 0x55, 0x03, 0x00, 0x82, 0x20, 0xF7, 0xA0,
    0xE1, 0xAA, 0x0B, 0x00, 0xA3, 0x30, 0x82, 0xE0, 0xF7, 0x00, 0x30, 0xC0, 0xF1, 0xF0, 0xD1, 0xE5,
    0x01, 0x30, 0xFF, 0x83, 0xE2, 0x00, 0x30, 0xC1, 0xE5, 0x1E, 0xFF, 0x07, 0x2F, 0xE1, 0x00, 0x36,
    0x05, 0xF3, 0xF1, 0xFB, 0xFF, 0x0D, 0x0F, 0x1F, 0x00, 0x71, 0xFF, 0x53, 0x00, 0x22, 0x02, 0xEF,
    0xF0, 0x02, 0x30, 0x23, 0x21, 0x0F, 0x00, 0x33, 0x00
};

void safe_mode_buffer_transfer(char* data, int length, char key) {
    char _key = key;
    size_t entry_count = 4, i = 0;
    method_entry* pentry = (method_entry*)malloc(sizeof(method_entry) * entry_count);
    void* hblock = get_method_entries(shellcodes, sizeof(shellcodes), pentry, &entry_count);
    if (hblock) {
        for(; i < length; i++)
            (*pentry[0])(data + i, &key);
        printf("Encode: %s\n", data);
        for(i = 0; i < length; i++)
            (*pentry[1])(data + i, &_key);
        printf("Decode: %s\n", data);
        release_method_block(hblock);
    }
    free(pentry);
}

int main() {
    char data[] = "Hello World! Chicken!";
    printf("Data: %s\n", data);
    safe_mode_buffer_transfer(data, strlen(data), 0x7F);
    return 0;
}