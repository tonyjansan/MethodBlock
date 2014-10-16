# Makefile for methodblock (x86)

CC := gcc
CFLAGS := -Wall
#CFLAGS += -O2    except -freorder-functions
CFLAGS += -fdefer-pop -fguess-branch-probability -fcprop-registers -floop-optimize\
          -fif-conversion -fif-conversion2 -ftree-ccp -ftree-dce -ftree-dominator-opts\
          -ftree-dse -ftree-ter -ftree-lrs -ftree-sra -ftree-copyrename -ftree-fre\
          -ftree-ch -funit-at-a-time -fmerge-constants -fthread-jumps -fcrossjumping\
          -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks -fgcse -fgcse-lm\
          -fexpensive-optimizations -fstrength-reduce -frerun-cse-after-loop -fpeephole2\
          -fcaller-saves -fschedule-insns -fschedule-insns2 -fsched-interblock -fsched-spec\
          -fregmove -fstrict-aliasing -fdelete-null-pointer-checks -freorder-blocks\
          -falign-functions -falign-jumps -falign-loops -falign-labels -ftree-vrp -ftree-pre

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