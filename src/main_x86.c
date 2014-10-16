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
    0x3F,       // key              : byte
    0x01,       // compress_type    : byte
    0x01,       // instruction_type : byte
    0x00,       // reserved         : byte
    0xE0, 0x00, // data_length      : word
    0x00, 0x00, 0x70, 0x00, // offsets : word []
    0xFF, 0x55, 0x89, 0xE5, 0x83, 0xEC, 0x10, 0x8B, 0x45, 0xF7, 0x08, 0x0F, 0xB6, 0xF3, 0xF0, 0x0C,
    0x0F, 0xB6, 0x00, 0xFF, 0x31, 0xD0, 0x88, 0x45, 0xFF, 0xC0, 0x4D, 0xFF, 0xFF, 0x04, 0x0F, 0xB6,
    0x45, 0xFF, 0x83, 0xE0, 0x33, 0xDF, 0x89, 0xC2, 0xC1, 0xE2, 0x02, 0x09, 0x01, 0x25, 0xCC, 0x7F,
    0x00, 0x00, 0x00, 0xC1, 0xF8, 0x02, 0x01, 0x01, 0x01, 0x7A, 0x15, 0x02, 0xAA, 0x1B, 0x00, 0xD1,
    0xF8, 0x89, 0xC1, 0xF4, 0xF2, 0xFF, 0x55, 0xFF, 0x83, 0xE2, 0x55, 0x01, 0xD2, 0x01, 0x8B, 0xCA,
    0x88, 0xF3, 0xF6, 0x08, 0x41, 0x01, 0xFC, 0xF0, 0xF9, 0xF1, 0x83, 0xBF, 0xC2, 0x01, 0x88, 0x10,
    0xC9, 0xC3, 0xEE, 0xF8, 0x00, 0xD0, 0x02, 0x0F, 0x14, 0x0F, 0x26, 0x0F, 0x38, 0x0D, 0x08, 0x4E,
    0x05, 0x08, 0x31, 0x00, 0x40, 0x02, 0x50, 0x0B
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
    printf("MethodBlock Original Size: %u\n", (size_t)methods[count - 1] - (size_t)methods[0]);
    create_method_block(block, &l, methods, count - 1, 0x3F, 1);
    printf("MethodBlock Output Size: %u\n\n", l);

    fp = fopen("mb.dat", "wb");
    if (fp) {
        fwrite(block, sizeof(char), l, fp);
        fclose(fp);
    }

    hblock = get_method_entries(block, l, methods, &count);
    if (hblock) {
        printf("Data: %s\n", data);
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