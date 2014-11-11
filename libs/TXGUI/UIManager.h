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

#ifndef iSeer_UIManager_h
#define iSeer_UIManager_h
#include <map>
#include "TXGUI.h"
#include "UIControlBase.h"
#include "UILayout.h"



namespace TXGUI {

    class UIContainer;

    class UIManager
    {
    public:
        static UIManager* sharedManager();
		static void Destroy();

		/// set/get ui default root
		void	setUIDefaultRoot(CCNode* pRoot);
		CCNode*    getUIDefaultRoot();
        
        void DispatchMenuEvent(UIControlBase* pSender, string namepfn, CCObject* pObject);

		float getScaleFactor();
		float getScaleFactorX();
		float getScaleFactorY();

		/*
		*@prototype, CreateUILayoutFromFile()
		*@param, fileName - plist file name
		*@param, UIName - ui name
		*@param, bShow - visible
		*/
		UILayout * CreateUILayoutFromFile(const char *fileName, cocos2d::CCNode *parent, std::string UIName, bool bShow = false);
		UILayout * CreateUILayoutFromFile(const char *fileName, std::string UIName, bool bShow = false);

		UILayout *getUILayout(std::string UIName);

		void RegisterMenuHandler(std::string UIName, std::string name, SEL_MenuHandler pfn, CCObject* pSelect);

		void UnregisterMenuHandler(std::string UIName, std::string name);

		void UnregisterAllMenuHandler(std::string UIName);

		void RemoveUILayout(std::string UIName);

		void UpdateLayout(std::string UIName);
        void AddLayout(UILayout* layout, std::string UIName);
        void RemoveLayout(std::string name);

		void SetOtherUILayoutTouchEnableExcept(bool value,std::string UIName);
		// container在基准分辨率下的中心点坐标
		cocos2d::CCPoint containerCenterPosition(UIContainer *container);
        
    protected:
		UIManager();
		~UIManager();

        static UIManager* pSharedManager;
		
		CCNode* m_pUIDefaultRoot;
	private:
		std::map<std::string, UILayout*> m_UIMap;
    };
}

#endif
