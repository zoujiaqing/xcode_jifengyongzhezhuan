LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)     
LOCAL_MODULE    := boostDataTimeLib    
LOCAL_SRC_FILES := lib/android/libboost_date_time-gcc-mt-1_53.a      
include $(PREBUILT_STATIC_LIBRARY)  
  
include $(CLEAR_VARS)     
LOCAL_MODULE    := boostFileSystemLib    
LOCAL_SRC_FILES := lib/android/libboost_filesystem-gcc-mt-1_53.a     
include $(PREBUILT_STATIC_LIBRARY)  
  
include $(CLEAR_VARS)     
LOCAL_MODULE    := boostIOStreamsLib  
LOCAL_SRC_FILES := lib/android/libboost_iostreams-gcc-mt-1_53.a    
include $(PREBUILT_STATIC_LIBRARY)  
  
include $(CLEAR_VARS)     
LOCAL_MODULE    := boostProgramOptionsLib  
LOCAL_SRC_FILES := lib/android/libboost_program_options-gcc-mt-1_53.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := boostRegexLib  
LOCAL_SRC_FILES := lib/android/libboost_regex-gcc-mt-1_53.a   
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)     
LOCAL_MODULE    := boostSingalsLib  
LOCAL_SRC_FILES := lib/android/libboost_signals-gcc-mt-1_53.a    
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)     
LOCAL_MODULE    := boostSystemLib  
LOCAL_SRC_FILES := lib/android/libboost_system-gcc-mt-1_53.a    
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)             
LOCAL_MODULE    := boost_static      
LOCAL_STATIC_LIBRARIES := boostDataTimeLib boostFileSystemLib boostIOStreamsLib boostProgramOptionsLib boostRegexLib boostSingalsLib boostSystemLib boostThreadLib
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(BUILD_STATIC_LIBRARY)  
