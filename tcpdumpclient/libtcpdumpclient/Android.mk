LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libtcpdumpclient
LOCAL_SRC_FILES := tcpdumpclient.cpp
LOCAL_MODULE_TAGS := optional

LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_LDFLAGS := -Wl,--hash-style=both
include $(BUILD_SHARED_LIBRARY)
