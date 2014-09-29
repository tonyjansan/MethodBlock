# Android Makefile

LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := ../main_arm32.c ../mblock.c ../lzss.c
LOCAL_CFLAGS := -Wall -DCROSS_COMPILE
LOCAL_MODULE := methodblock
LOCAL_MODULE_FILENAME := methodblock

#LOCAL_C_INCLUDES :=
#LOCAL_STATIC_LIBRARIES :=
#LOCAL_SHARED_LIBRARIES :=

include $(BUILD_EXECUTABLE)