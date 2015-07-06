#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "methodblock.h"

METHOD_EXPORT_START
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
METHOD_EXPORT_END

char shellcodes[] = {
    // method_block_header
    0x0C, 0x00, // header_length    : word
    0x00,       // key              : byte
    0x00,       // compress_type    : byte
    0x01,       // instruction_type : byte
    0x00,       // reserved         : byte
    0x60, 0x00, // data_length      : word
    0x00, 0x00, 0x30, 0x00, // offsets : word []
    0x0F, 0xB6, 0x06, 0x32, 0x07, 0xC0, 0xC8, 0x04, 0x89, 0xC1, 0x25, 0xCC, 0x00, 0x00, 0x00, 0x83,
    0xE1, 0x33, 0xC1, 0xE8, 0x02, 0x8D, 0x04, 0x88, 0x89, 0xC2, 0x83, 0xE0, 0x55, 0x81, 0xE2, 0xAA,
    0x00, 0x00, 0x00, 0xD1, 0xFA, 0x8D, 0x14, 0x42, 0x88, 0x17, 0x80, 0x06, 0x01, 0xC3, 0x66, 0x90,
    0x0F, 0xB6, 0x07, 0xC0, 0xC8, 0x04, 0x89, 0xC1, 0x25, 0xCC, 0x00, 0x00, 0x00, 0x83, 0xE1, 0x33,
    0xC1, 0xE8, 0x02, 0x8D, 0x04, 0x88, 0x89, 0xC2, 0x83, 0xE0, 0x55, 0x81, 0xE2, 0xAA, 0x00, 0x00,
    0x00, 0xD1, 0xFA, 0x8D, 0x14, 0x42, 0x88, 0x17, 0x32, 0x16, 0x88, 0x17, 0x80, 0x06, 0x01, 0xC3
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
    size_t l = BLOCK_MAXSIZE, count = 3;
    char data[] = "Hello World! Chicken! Come On!", block[BLOCK_MAXSIZE], key = 0x7F, _key = 0x7F;
    void* hblock = 0; FILE* fp = 0;

    method_entry methods[3] = {strongEncode, strongDecode, METHOD_END_FLAG};
    printf("MethodBlock Original Size: %ld\n", (size_t)methods[count - 1] - (size_t)methods[0]);
    create_method_block(block, &l, methods, count - 1, 0x3F, 1);
    printf("MethodBlock Output Size: %ld\n\n", l);

    fp = fopen("mb.dat", "wb");
    if (fp) {
        fwrite(block, sizeof(char), l, fp);
        fclose(fp);
    }

    hblock = get_method_entries(block, l, methods, &count);
    if (hblock) {
        printf("Data:   %s\n", data);
        for(l = 0; l < strlen(data); l++)
            (*methods[0])(data + l, &key);
        printf("Encode: %s\n", data);
		for(l = 0; l < strlen(data); l++)
            (*methods[1])(data + l, &_key);
        printf("Decode: %s\n", data);
        release_method_block(hblock);
    }

    safe_mode_buffer_transfer(data, strlen(data), 0x7F);
    return 0;
}
