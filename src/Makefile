# Makefile for methodblock (x86)

CC := gcc
CFLAGS := -Wall
#CFLAGS += -O2
CFLAGS += -fdefer-pop -fguess-branch-probability -fcprop-registers -floop-optimize\
          -fif-conversion -fif-conversion2 -ftree-ccp -ftree-dce -ftree-dominator-opts\
		  -ftree-dse -ftree-ter -ftree-lrs -ftree-sra -ftree-copyrename -fmerge-constants

TEST_X86 := methodblock
OBJS := main_x86.o methodblock.o lzss.o

.PHONY: clean

methodblock: $(OBJS)
	$(CC) $(OBJS) -o $(TEST_X86) $(CFLAGS)

main.o: main_x86.c methodblock.h
	$(CC) -c main_x86.c $(CFLAGS)

methodblock.o: methodblock.c methodblock.h lzss.h
	$(CC) -c methodblock.c $(CFLAGS)

lzss.o: lzss.c lzss.h
	$(CC) -c lzss.c $(CFLAGS)

clean:
	rm -f $(TEST_X86) $(TEST_X86).exe *.o