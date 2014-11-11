//
//  Engine.h
//  iSeer
//
//  Created by b a on 12-5-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_Engine_h
#define iSeer_Engine_h

class TouchChecker;
class CEventCenter;

class Engine
{
public:
	typedef struct stDebugInf
	{
		int AtlObj_Node_Num;
		int Titled_Node_Num;
		int UI_Node_Num;
		int UI_Sprite_Num;
		/// add more here!

		stDebugInf()
		{
			AtlObj_Node_Num = 0;
			Titled_Node_Num = 0;
			UI_Node_Num = 0;
			UI_Sprite_Num = 0;
		}

	}DebugINF;

public:
    static void  _SDMInit();

    static void  _SDMShutDown();
    
    /** 
     * @brief: 获取能否处理点击事件的检查器
     * @param: void
     * @return:TouchChecker, 
     */ 
    static TouchChecker* GetChecker();

	// Get Event Center Object
	static CEventCenter* GetEventCenter();

    /** 
     * @brief: 获取能否处理点击事件
     * @param: void
     * @return:bool, true: for enable, otherwise false
     */ 
    static bool CanTouchNow();

	static DebugINF& GetDebugInf();
    
protected:
    static TouchChecker* m_pkTouchChecker;
	static CEventCenter* m_pkEventCenter;

	static DebugINF  ms_kDebuginf;
};


#endif
