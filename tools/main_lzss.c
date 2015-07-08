#include <stdio.h>
#include <stdlib.h>

#include "../src/lzss.h"

void print_usage() {
    printf("LZSS --- The compress tool of improving LZ77\n");
    printf("Origin:  4/6/1989 Haruhiko Okumura\n");
    printf("Build:   7/7/2015 Tony Jiang\n");
    printf("Usage: \n");
    printf("\tEncode --- lzss e <input_file> <output_file>\n");
    printf("\tDecode --- lzss d <input_file> <output_file>\n");
}

#define getfilesize(fp) ({ long ret; fseek(fp, 0L, SEEK_END); ret = ftell(fp); fseek(fp, 0L, SEEK_SET); ret; })

int main(int argc, char** args) {
    int in_len = 0, out_len = 0, count = 0;
    unsigned char *in_buf = NULL, *out_buf = NULL;
    FILE *fi = NULL, *fo = NULL;
    if (argc < 4 || (args[1][0] != 'e' && args[1][0] != 'd')) {
        print_usage();
        return 0;
    }

    if (!(fi = fopen(args[2], "rb"))) {
        print_usage();
        return 0;
    } else if ((in_len = (int)getfilesize(fi)) <= 0
        || (out_len = (args[1][0] == 'e')? (in_len << 1) : (in_len << 4)) <= 0
        || !(fo = fopen(args[3], "wb"))) {
        fclose(fi);
        print_usage();
        return 0;
    } else if (!(in_buf = (unsigned char*)malloc(in_len)) || fread(in_buf, sizeof(unsigned char), in_len, fi) < in_len) {
        fclose(fo); fclose(fi);
        printf("read file error!");
        return -1;
    } else if (!(out_buf = (unsigned char*)malloc(out_len))) {
        free(in_buf);
        fclose(fo); fclose(fi);
        printf("alloc memory error!");
        return -1;
    }

    count = (args[1][0] == 'e')? lzss_encode(in_buf, in_len, out_buf, out_len)
        : lzss_decode(in_buf, in_len, out_buf, out_len);
    if (count > 0) {
        out_len = fwrite(out_buf, sizeof(unsigned char), count, fo);
        if (out_len != count)
            printf("write file error!");
    }
    free(out_buf); free(in_buf);
    fclose(fo); fclose(fi);
    return 0;
}
