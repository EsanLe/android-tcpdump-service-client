
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := tcpdumpservice
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES := tcpdumpservice.cpp tcpdumpapi.c

LOCAL_CFLAGS := -O2 -g
LOCAL_CFLAGS += -DHAVE_CONFIG_H -D_U_="__attribute__((unused))"

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/missing\
	external/openssl/include\
	external/libpcap
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_SHARED_LIBRARIES += libssl libcrypto
LOCAL_STATIC_LIBRARIES += libpcap
LOCAL_C_INCLUDES += frameworks/base/include system/core/include
LOCAL_LDFLAGS := -Wl,--hash-style=both -ldl
include $(BUILD_EXECUTABLE)
