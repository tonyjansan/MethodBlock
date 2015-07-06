# Makefile for methodblock (x86)

CC := gcc
CFLAGS += -O2 -Wall -fno-reorder-functions
# Without -g
# CFLAGS += -fomit-frame-pointer

METHODBLOCK := methodblock
OBJS := methodblock.o lzss.o

# x86 || x64
ARCH := $(shell uname -m)
X32 := i686
X64 := x86_64
ifeq ($(ARCH), $(X64))
OBJS += main_x64.c
else
OBJS += main_x86.c
endif

.PHONY: clean

methodblock: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(METHODBLOCK)

ifeq ($(ARCH), $(X64))
main.o: main_x64.c methodblock.h
	$(CC) $(CFLAGS) -c main_x64.c
else
main.o: main_x86.c methodblock.h
	$(CC) $(CFLAGS) -c main_x86.c
endif

methodblock.o: methodblock.c methodblock.h lzss.h
	$(CC) $(CFLAGS) -c methodblock.c

lzss.o: lzss.c lzss.h
	$(CC) $(CFLAGS) -c lzss.c

clean:
	rm -f $(METHODBLOCK) $(METHODBLOCK).exe *.o
