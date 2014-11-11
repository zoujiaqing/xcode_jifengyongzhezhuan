// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#ifndef iSeer_TXGUI_h
#define iSeer_TXGUI_h
#include <string>
#include <map>
#include <vector>

#include "UIDefine.h"
#include "UIColorDefine.h"
#include "cocos2d.h"


using namespace std;

namespace TXGUI {
    
#define MODEL_LAYER_PRIORITY    (-150)
#define MODEL_MENU_PRIORITY     (-160)
    
#define TX_PROPERTY(varType, varName)\
protected: varType varName;\
public: virtual varType get##varName(void);\
public: virtual void set##varName(varType var);
    
#define TX_SYNTHESIZE(varType, varName)\
protected: varType varName;\
public: inline varType get##varName(void) const { return varName; }\
public: inline void set##varName(varType var){ varName = var; }
    
    enum SceneType {
        TypeNil = 0,
        
        // UI Layer
        UI_LAYOUT,
        
        
        
        MAX_COUNT,
    };
    
    enum UILayerMsgId {
        UILayerMsg_Nil = 0,   
        
        // to do ...     
    };  
    
    enum LayerOrder {
        Default_Layer = 0,
        
        Game_Layer = 2,
        UI_Layer = 5,
        
        Dialog_Layer = 8,
        Model_Layer = 10,
    };
    
    enum LayerTag {
        Default_Tag = 0,
        
        Game_Tag,
        UI_Tag,
        
        Model_Tag,
    };
    
    // 这里的消息仅仅给出游戏过程中需要关心的东西，不关心的一概不加入；随着游戏开发过程的继续，相信这里的内容会发生很大的变化
    enum eObserverMessage 
    {
        
        GAME_BROADCAST_START_UI_ANSWER_START,
        
        GAME_EVENT_LOADING_UI_REPORT_LOADING_DONE,  // 资源加载完成了，开始播放动画
        GAME_EVENT_REOURCEMANAGER_REPORT_LOADING_REOURCE_DONE, 

        
    };
    

#ifndef KUI_BIN
//#     define KUI_BIN (IsIpad()?"UI/ui.bin":"UI/ui_iphone.bin")
#define KUI_BIN "UI/ui.bin"
#endif
#ifndef KUI_BIN2
//#     define KUI_BIN (IsIpad()?"UI/ui.bin":"UI/ui_iphone.bin")
#define KUI_BIN2 "UI/ui2.bin"
#endif
#ifndef KICON_BIN
#define KICON_BIN "UI/ui_system_icon.bin"
#endif

#ifndef KPPVE_BIN
#define KPPVE_BIN "UI/ui_ppve.bin"
#endif
	
#ifndef KUI_LEAD_BIN
#define KUI_LEAD_BIN "UI/ui_lead.bin"
#endif
    int getResourceIDByName(const char* name);   
   

	/// data type define
	typedef cocos2d::CCArray  mutableArray;
	typedef cocos2d::CCDictionary mutableDic;
	typedef std::map<std::string, std::string>  StringStringMap;
}

#endif
