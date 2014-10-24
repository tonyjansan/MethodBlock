#ifdef unix
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#else
#include <stdlib.h>
#include <string.h>
#endif // unix

#include "methodblock.h"
#include "lzss.h"

char* buffer_transform(char* data, size_t length, char key)
{
    size_t i = 0;
    for(; i < length; i++)
        data[length] ^= key;
    return data;
}

void create_method_block(char* data, size_t* length, method_entry* methods, size_t count,
    unsigned char key, unsigned char compress_type)
{
    char buffer[BLOCK_MAXSIZE], *pentry;
    size_t i = 0;

    method_block_header* pmbh = (method_block_header*)data;
    pmbh->header_length = sizeof(method_block_header) + (count * sizeof(unsigned short));
    pentry = data + pmbh->header_length;

    pmbh->key = key;
    pmbh->compress_type = compress_type;
#ifndef CROSS_COMPILE
    pmbh->instruction_type = 0x01;
#else
    pmbh->instruction_type = 0x02;
#endif
    pmbh->data_length = 0;
    for(; i <= count; i++)
        pmbh->offsets[i] = (size_t)methods[i] - (size_t)methods[0];

    for(i = 0; i < count; i++) {
        pmbh->data_length += pmbh->offsets[i + 1] - pmbh->offsets[i];
        memcpy(buffer + pmbh->offsets[i], methods[i], pmbh->offsets[i + 1] - pmbh->offsets[i]);
    }

    *length = pmbh->header_length;
    if (compress_type == 0x01) // lzss
        *length += lzss_encode((unsigned char*)buffer, pmbh->data_length, (unsigned char*)pentry, BLOCK_MAXSIZE);
    else {
        *length += pmbh->data_length;
        memcpy(pentry, buffer, pmbh->data_length);
    }

    // Encrypt if having a key
    if (pmbh->key)
        buffer_transform(pentry, *length - pmbh->header_length, pmbh->key);
}

void* get_method_entries(char* data, size_t length, method_entry* methods, size_t* count)
{
    method_block_header* pmbh = (method_block_header*)data;
    char *buffer, *pentry = data + pmbh->header_length;
    size_t i = (pmbh->header_length - sizeof(method_block_header)) / sizeof(unsigned short);
#ifdef unix
    int fd;
#endif

    // decrypt first if having a key
    if (pmbh->key) {
        buffer_transform(pentry, length - pmbh->header_length, pmbh->key);
        pmbh->key = 0;
    }

    // map memory and set execute flag
#ifdef unix
    fd = open("/dev/zero", 0);
    if (fd == -1)
        return 0;
    buffer = mmap(0, BLOCK_MAXSIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE, fd, 0);
    close(fd);
#else
    buffer = (char*)malloc(BLOCK_MAXSIZE);
#endif // unix
    if (!buffer) return 0;

    // copy method shell-codes
    if (pmbh->compress_type == 0x01) { // lzss
        length = lzss_decode((unsigned char*)pentry, length - pmbh->header_length, (unsigned char*)buffer, pmbh->data_length);
        if (length != pmbh->data_length) {} // Decompress Exception!
    }
    else
        memcpy(buffer, pentry, length);

    // rebuild method entries
    if (i < *count) *count = i;
    for(i = 0; i < *count; i++)
        methods[i] = (method_entry)(buffer + pmbh->offsets[i]);

    return buffer;
}

void release_method_block(void* p)
{
#ifdef unix
    munmap(p, BLOCK_MAXSIZE);
#else
    free(p);
#endif // unix
}