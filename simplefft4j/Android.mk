LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE     := simplefft4j					# name of your module
LOCAL_LDLIBS     := -L$(SYSROOT)/usr/lib	# libraries to link against
LOCAL_SRC_FILES  := in.waslos_kneo_libsimplefft_FFTProcessor.c		# list of source files
LOCAL_SHARED_LIBRARIES := libsimplefft
include $(BUILD_SHARED_LIBRARY)			# uncomment this line to build a shared library
#include $(BUILD_STATIC_LIBRARY)				# here, we are building a static library
