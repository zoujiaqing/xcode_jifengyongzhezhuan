LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := txgui_static

LOCAL_CPP_EXTENSION := .cpp

# List of client source files, directly taken from Makefile.am
FILE_LIST := \
GameFontManager.cpp \
UICreator.cpp \
UIManager.cpp \
LuaTXGUI.cpp \
Common/CustomScene.cpp \
Common/LayerBase.cpp \
Common/ModelLayer.cpp \
Common/NiRTTI.cpp \
Common/NiStream.cpp \
Common/NotificationCenter.cpp \
Common/TXAudioEngine.cpp \
Common/UILayerBase.cpp \
Common/UILayerColorBase.cpp \
Common/UIModelLayerBase.cpp \
Common/UIMenu.cpp \
Common/UIDragLayer.cpp \
Common/DragReceiver.cpp \
Common/UILabelTTF.cpp \
Controls/CCScale9Sprite.cpp \
Controls/IconButton.cpp \
Controls/ListItemButton.cpp \
Controls/NiObject.cpp \
Controls/TextInputField.cpp \
Controls/UIAnimation.cpp \
Controls/UIButton.cpp \
Common/BYGraySprite.cpp \
Common/MenuItemExpand.cpp \
Controls/UIButtonToggle.cpp \
Controls/UIContainer.cpp \
Controls/UIControlBase.cpp \
Controls/UILabel.cpp \
Controls/UILabelAtlas.cpp \
Controls/UILayout.cpp \
Controls/UILayoutRenderNode.cpp \
Controls/UIPicture.cpp \
Controls/UIScrollLayer.cpp \
Controls/UIScrollList.cpp \
Controls/UIList.cpp \
Controls/UIScrollPage.cpp \
Controls/UIScrollView.cpp \
Controls/UITextInputField.cpp \
Controls/UIEditBox.cpp
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES        := \
$(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/adventurecpp \
$(LOCAL_PATH)/../../Classes/particleSystem \
$(LOCAL_PATH)/../../Classes/Client \
$(LOCAL_PATH)/../../Classes/Client/Misc \
$(LOCAL_PATH)/../../Classes/Client/Shop \
$(LOCAL_PATH)/../../Classes/Client/GameManager \
$(LOCAL_PATH)/../../Classes/Client/CharacterSelect \
$(LOCAL_PATH)/../../Classes/Client/UILayer \
$(LOCAL_PATH)/../../Classes/Client/Items \
$(LOCAL_PATH)/../../Classes/Client/NPC \
$(LOCAL_PATH)/../../Classes/Client/Task \
$(LOCAL_PATH)/../../Classes/Client/Story \
$(LOCAL_PATH)/../../Classes/Client/Monster \
$(LOCAL_PATH)/../../Classes/Client/Sprite \
$(LOCAL_PATH)/../../Classes/Client/DressUp \
$(LOCAL_PATH)/../../Classes/Client/ScriptSupport \
$(LOCAL_PATH)/../../Classes/Client/UILogic \
$(LOCAL_PATH)/../../Classes/Client/Tutorials \
$(LOCAL_PATH)/../../Classes/Client/Enum \
$(LOCAL_PATH)/../../Classes/Client/RcsUpdateOnline \
$(LOCAL_PATH)/../../Classes/Client/UILogic/SkillUI \
$(LOCAL_PATH)/../../Classes/Engine \
$(LOCAL_PATH)/../../Classes/Engine/Level \
$(LOCAL_PATH)/../../Classes/Engine/Sprite \
$(LOCAL_PATH)/../../Classes/Engine/Effect \
$(LOCAL_PATH)/../../Classes/Engine/Localization \
$(LOCAL_PATH)/../../Classes/Engine/Map \
$(LOCAL_PATH)/../../Classes/Engine/SASprite \
$(LOCAL_PATH)/../../Classes/Engine/SkillSys \
$(LOCAL_PATH)/../../Classes/Engine/StateMachine \
$(LOCAL_PATH)/../../Classes/Engine/Level/Listener \
$(LOCAL_PATH)/../../Classes/Engine/ScriptSupport \
$(LOCAL_PATH)/../../Classes/Network \
$(LOCAL_PATH)/../../Classes/Server \
$(LOCAL_PATH)/../../Classes/Server/base \
$(LOCAL_PATH)/../../Classes/Server/battlefields \
$(LOCAL_PATH)/../../Classes/Server/common \
$(LOCAL_PATH)/../../Classes/Server/config \
$(LOCAL_PATH)/../../Classes/Server/entities \
$(LOCAL_PATH)/../../Classes/Server/frontend \
$(LOCAL_PATH)/../../Classes/Server/msg_dispatcher \
$(LOCAL_PATH)/../../Classes/Server/proto \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/cocos2dx_support \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/lua \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/tolua \
$(LOCAL_PATH)/../../cocos2dx/scripting/lua/src \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx \
$(LOCAL_PATH)/../../cocos2dx/extensions \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/kazmath/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/android \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/android/jni \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libcurl/include/curl \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libjpeg/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libpng/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libprofiler/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libtiff/include \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/platform/third_party/android/prebuilt/libxml2/include/libxml \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/support \
$(LOCAL_PATH)/../../cocos2dx/cocos2dx/support/zip_support \
$(LOCAL_PATH)/../../cocos2dx/CocosDenshion/include \
$(LOCAL_PATH) \
$(LOCAL_PATH)/Common  \
$(LOCAL_PATH)/Controls \
$(LOCAL_PATH)/../../libs/PageManager \
$(LOCAL_PATH)/../../libs/PageManager/Resource \
$(LOCAL_PATH)/../../libs/CrossPlatform \
$(LOCAL_PATH)/../../libs/CrossPlatform/AndroidPlatform \
$(LOCAL_PATH)/../../libs/CrossPlatform/AndroidPlatform/IAP \
$(LOCAL_PATH)/../../libs/CrossPlatform/AndroidPlatform/JSON \
$(LOCAL_PATH)/../../libs/CrossPlatform/AndroidPlatform/SFHFKeychainUtils \
$(LOCAL_PATH)/../../libs/cocos2dx_support \
$(LOCAL_PATH)/../../libs/TXGUI \
$(LOCAL_PATH)/../../libs/TXGUI/Common \
$(LOCAL_PATH)/../../libs/TXGUI/Controls \
$(LOCAL_PATH)/../../libs/ASprite \
$(LOCAL_PATH)/../../libs/ASprite/spriteDefine \
$(LOCAL_PATH)/../../libs/tinyxml \
$(LOCAL_PATH)/../../libs/Flash2Cocos2d-X \
$(LOCAL_PATH)/../../libs/ProtoBuf \
$(LOCAL_PATH)/../../libs/ProtoBuf/include \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google/protobuf \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google/protobuf/io \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google/protobuf/stubs \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google/protobuf/compiler \
$(LOCAL_PATH)/../../libs/ProtoBuf/include/google/protobuf/compiler/cpp \
$(LOCAL_PATH)/../../libs/Boost_153/include \
$(LOCAL_PATH)/../../libs/crash/include \
$(LOCAL_PATH)/../../libs/json \
$(LOCAL_PATH)/../../libs/openssl/include \
$(LOCAL_PATH)/../../android_third_pro/plugin/protocols/include \
$(LOCAL_PATH)/../../libs/EncodeCode 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_EXPORT_LDLIBS     := -llog

include $(BUILD_STATIC_LIBRARY)

# Done.
