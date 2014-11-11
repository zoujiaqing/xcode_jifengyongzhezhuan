#pragma once

#include "ASprite.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

namespace TXGUI
{
	class PopupRewardLayer : public cocos2d::CCLayer
	{
	public:	
		PopupRewardLayer();
		virtual ~PopupRewardLayer();

		CREATE_FUNC(PopupRewardLayer);
		virtual bool init();

		static bool Show(const char* message_body,unsigned int rewardId,const char* okBtnContent = "",bool bShowCloseBtn = true,bool bModelLayer = false,cocos2d::CCObject * handler = 0, 
			SEL_MenuHandler pfnOkClicked = 0,SEL_MenuHandler pfnCloselClicked = 0);

		static bool Show(const char* message_body,unsigned int rewardId,const char* okBtnContent = "",bool bShowCloseBtn = true,bool bModelLayer = false,
			int nOkBtnHandler = 0,int nCloseBtnHandler = 0);

		static bool Show(const char* message_body,const char* rewardIcon,const char* okBtnContent = "",bool bShowCloseBtn = true,bool bModelLayer = false,
			int nOkBtnHandler = 0,int nCloseBtnHandler = 0,float displayTime = 0.0);

		void DisplayOver(float dt);

		static void AdjustStyle(string okBtnContent = "",unsigned int fontSize = 0,ccColor3B labelColor = ccc3(255,255,255));

		void Destroy();

		void ResetValue();

		void WndAnimEnd(CCNode* sender);
		void okBtnClickEvent(CCObject* sender);
        void closeBtnClickEvent(CCObject* sender);               

		CCSprite* GetSpriteByRewardID(unsigned int rewardId);
		void playUpgradeEffect();
	protected:
		virtual void registerWithTouchDispatcher();
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	private:		
		static PopupRewardLayer* selfInstance;

		TXGUI::UILayout* messageboxLayer;
		TXGUI::UIButton* okBtn;
		TXGUI::UIButton* closeBtn;
		TXGUI::UILabel* okContentLabel;
		TXGUI::UILabel* messageBodyLabel;
		CCObject* clickEventHander;
		SEL_MenuHandler pfnOkClicked;
		SEL_MenuHandler pfnCloseClicked;
        
		std::string okBtnContent;

		TXGUI::UIPicture* mIconPic;
		TXGUI::UIPicture* mIconFramePic;		
		TXGUI::UIPicture* mHitIconFramePic;

		bool mIsWndAnimOver;
		bool mIsModelLayer;
	};
};