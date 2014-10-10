# Makefile for methodblock (x86)

CC := gcc
CFLAGS := -O2 -Wall

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