LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE     := libsimplefft										# name of your module
LOCAL_LDLIBS     := -L$(SYSROOT)/usr/lib								# libraries to link against
LOCAL_SRC_FILES  := alloc.c fft.c libsimplefft.c twiddle.c util.c convolution.c	ndfft.c	# list of source files

include $(BUILD_SHARED_LIBRARY)			# uncomment this line to build a shared library
#include $(BUILD_STATIC_LIBRARY)				# here, we are building a static library
