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

#ifndef iSeer_UICreator_h
#define iSeer_UICreator_h

#include "TXGUI.h"

namespace TXGUI {
    class UILayerBase;
    class CustomScene;
    class UILayout;
    
	/**
     * @brief 解析resource.h，从中获取给定资源名字的编码值 
     * @param name, 资源名
     * @return int, 资源编码值
     * @
     */ 
    int getResourceIDByName(const char* name);

	void resetReloadResourceID();

    class UICreator
    {
    public: 
        /**
         * @brief 创建一个UI图层
         * @param SceneType, 
         * @param pCustomScene, 父图层
         * @return UILayerBase, UILayer的对象
         * @
         */
        static UILayerBase* CreateUILayer(SceneType _sceneType, CustomScene* pCustomScene);   
        
		/**
         * @brief 从文件中创建一个UI图层
         * @param file, 布局plist文件所在的位置
         * @param pParent, 父图层
         * @return bShow, 是否默认为显示
         * @
         */
        static UILayout*  CreateUILayoutFromFile(const char* file,  cocos2d::CCNode *pParent,
                                                     bool bShow = false); 

		/**
         * @brief 从文件中创建一个UI图层,加并入到UI系统的默认的节点上
         * @param file, 布局plist文件所在的位置
         * @return bShow, 是否默认为显示
         * @
         */
        static UILayout*  CreateUILayoutFromFile(const char* file, bool bShow = false); 
    };
    
    class TXMainSDM
    {
    public:
        static void Init();
        static void ShutDown();
    };
    
    
#define dispatchMenuEvent(pfn, pObject) (UIManager::sharedManager()->DispatchMenuEvent(this, pfn, pObject)) 
#define RegisterMenuEvent(layerout, pfn) (layerout->RegisterMenuHandler(
}




#endif
