LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)     
LOCAL_MODULE    := feedback    
LOCAL_SRC_FILES := libs/armeabi/libTaomeeFeedback.a
include $(PREBUILT_STATIC_LIBRARY)
