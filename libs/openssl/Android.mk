LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)     
LOCAL_MODULE    := opensslLib    
LOCAL_SRC_FILES := libs/android/libssl.a      
include $(PREBUILT_STATIC_LIBRARY)  
  
include $(CLEAR_VARS)     
LOCAL_MODULE    := cryptoLib
LOCAL_SRC_FILES := libs/android/libcrypto.a      
include $(PREBUILT_STATIC_LIBRARY)  
  

include $(CLEAR_VARS)             
LOCAL_MODULE    := openssl_static      
LOCAL_STATIC_LIBRARIES := opensslLib cryptoLib
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(BUILD_STATIC_LIBRARY)  
