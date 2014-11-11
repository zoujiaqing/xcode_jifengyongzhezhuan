LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pbc_static



LOCAL_SRC_FILES := \
context.cpp varint.cpp array.cpp pattern.cpp register.cpp proto.cpp map.cpp alloc.cpp rmessage.cpp wmessage.cpp \
bootstrap.cpp stringpool.cpp decode.cpp pbc-lua.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/lua \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/src 

include $(BUILD_STATIC_LIBRARY)
