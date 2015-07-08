#include <stdio.h>
#include <stdlib.h>

#define getfilesize(fp) ({ long ret; fseek(fp, 0L, SEEK_END); ret = ftell(fp); fseek(fp, 0L, SEEK_SET); ret; })

void print_usage() {
    printf("TRANS --- The transform tool of bytes\n");
    printf("Build:    7/7/2015 Tony Jiang\n");
    printf("Usage: \n");
    printf("\ttrans <input_file> <output_file> [key]\n");
}

char* buffer_transform(char* data, size_t length, char key)
{
    size_t i = 0;
    for(; i < length; i++)
        data[i] ^= key;
    return data;
}

int main(int argc, char** args) {
    FILE *fi = NULL, *fo = NULL;
    char *buf = NULL, key = (char)(argc >= 4)? atoi(args[3]) : 0x3F;
    int len = 0;
    if (argc < 3) {
        print_usage();
        return 0;
    }

    if (!(fi = fopen(args[1], "rb"))) {
        print_usage();
        return 0;
    } else if ((len = (int)getfilesize(fi)) <= 0 || !(fo = fopen(args[2], "wb"))) {
        fclose(fi);
        print_usage();
        return 0;
    } else if (!(buf = (char*)malloc(len)) || fread(buf, sizeof(char), len, fi) < len) {
        fclose(fo); fclose(fi);
        printf("read file error!");
        return -1;
    }

    buf = buffer_transform(buf, len, key);
    if (fwrite(buf, sizeof(char), len, fo) != len)
        printf("write file error!");

    free(buf);
    fclose(fo); fclose(fi);
    return 0;
}
