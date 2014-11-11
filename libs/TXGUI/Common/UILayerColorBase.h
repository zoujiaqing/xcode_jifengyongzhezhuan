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

#ifndef iSeer_UILayerColorBase_h
#define iSeer_UILayerColorBase_h

#include "LayerBase.h"
#include "ObserverProtocol.h"

namespace TXGUI {
    
    
    //class UILayerColorBase : public ColorLayerBase  
	class UILayerColorBase : public LayerBase
    {
    public:
		UILayerColorBase();

        UILayerColorBase(std::string fileName, bool touchEnable, bool schedule, ccColor4B color);
        
        UILayerColorBase(bool touchEnable, bool schedule, ccColor4B color);
        
        virtual ~UILayerColorBase();
        
		virtual bool init();
        virtual void onBroadcastMessage(BroadcastMessage *msg);
        
        virtual void notifyUIMsg(UILayerMsgId msgId, uint32_t param);
        
        void clearAllChildren();
        void showLayer(bool show);

		/// model layer means swallow all the touch event,
		void EnableModeless(bool bModelLayer,int priority = kCCScrollTouchBgPriority);

		/// exit layer now
		virtual void  onExit();
        
    protected:
        virtual void onInit();
        virtual void onUnInit();
        
    protected:
        virtual cocos2d::ccColor4B getLayerColor();
        virtual void update(float deltaTime);
        virtual void onItemClickCallback(int tagId);
        virtual void onItemValueChanged(int tagId);


		virtual void registerWithTouchDispatcher();
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
        
    private:
        ccColor4B _layerColor;

		/// swallow touch event flag, true: swallow touch message, false : transparent to touch event
		/// default is true;
        bool m_bSwallowTouchEvent;
		int m_touchPriority;
    };
}



#endif
