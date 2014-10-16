# Makefile for methodblock (x86)

CC := gcc
CFLAGS += -O2 -Wall -fno-reorder-functions
# Without -g
# CFLAGS += -fomit-frame-pointer

TEST_X86 := methodblock
OBJS := main_x86.o methodblock.o lzss.o

.PHONY: clean

methodblock: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TEST_X86)

main.o: main_x86.c methodblock.h
	$(CC) $(CFLAGS) -c main_x86.c

methodblock.o: methodblock.c methodblock.h lzss.h
	$(CC) $(CFLAGS) -c methodblock.c

lzss.o: lzss.c lzss.h
	$(CC) $(CFLAGS) -c lzss.c

clean:
	rm -f $(TEST_X86) $(TEST_X86).exe *.o