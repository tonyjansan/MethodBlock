# Android Makefile

LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := ../main_arm32.c ../methodblock.c ../lzss.c
LOCAL_CFLAGS := -Wall -DCROSS_COMPILE
LOCAL_CFLAGS += -fdefer-pop -fguess-branch-probability -fcprop-registers -floop-optimize\
                -fif-conversion -fif-conversion2 -ftree-ccp -ftree-dce -ftree-dominator-opts\
                -ftree-dse -ftree-ter -ftree-lrs -ftree-sra -ftree-copyrename -ftree-fre\
                -ftree-ch -funit-at-a-time -fmerge-constants -fthread-jumps -fcrossjumping\
                -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks -fgcse -fgcse-lm\
                -fexpensive-optimizations -fstrength-reduce -frerun-cse-after-loop -fpeephole2\
                -fcaller-saves -fschedule-insns -fschedule-insns2 -fsched-interblock -fsched-spec\
                -fregmove -fstrict-aliasing -fdelete-null-pointer-checks -freorder-blocks\
                -falign-functions -falign-jumps -falign-loops -falign-labels -ftree-vrp -ftree-pre

LOCAL_MODULE := methodblock
LOCAL_MODULE_FILENAME := methodblock

#LOCAL_C_INCLUDES :=
#LOCAL_STATIC_LIBRARIES :=
#LOCAL_SHARED_LIBRARIES :=

include $(BUILD_EXECUTABLE)