#pragma once

#include "ASprite.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;


/*
 * MessageBox() Flags
 */
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_OKCANCELCLOSE            0x00000006L
#define MB_MIDDLECLOSE              0x00000007L
// 弹出消息框的优先级，防止不同消息框的相互影响
enum
{
	KMessageBoxDefaultPriority				= 0,	// 默认优先级
	KMessageBoxExitApp						= 90,	// 退出程序
	KMessageBoxNetWorkDisconnectPriority	= 100,	// 网络连接中断
};

namespace TXGUI
{
	class MessageBox : public cocos2d::CCLayer
	{
	public:	
		MessageBox();
		virtual ~MessageBox();

		CREATE_FUNC(MessageBox);
		virtual bool init();

		static void Show(const char* message_body,cocos2d::CCObject * handler = 0, 
			SEL_MenuHandler pfnOkClicked = 0,SEL_MenuHandler pfnCancelClicked = 0,
			unsigned int uType = MB_YESNO,ccColor3B labelColor = ccc3(255,255,255),int fontSize = 32,
			int priority = KMessageBoxDefaultPriority);

		static void Show(const char* message_body,int pfnOkHandler,int pfnCancelHandler,
			unsigned int uType = MB_YESNO,ccColor3B labelColor = ccc3(255,255,255),int fontSize = 32,
			int priority = KMessageBoxDefaultPriority);

		static void ShowTextExt(string contentExt,ccColor3B labelColor = ccc3(56,221,7),int fontSize = 22);
		static void AdjustStyle(string okBtnContent,string cancelBtnContent);
		static void AdjustMiddleStyle(string middleBtnContent);
		static void AdjustZOrder(unsigned int zOrder);
		static void AdjustPriority(bool bUseHightPriority);
		static void SetClickOkBtnDefaultBehaviorOrNot(bool bDefault);

		static void clearMessageBox();

		void Destroy();

		void ResetValue();

		void middleBtnClickEvent(CCObject* sender);
		void okBtnClickEvent(CCObject* sender);
		void cancelBtnClickEvent(CCObject* sender);
        void closeBtnClickEvent(CCObject* sender);
        
        static void showCloseBtn();
        
		CC_PROPERTY(int, m_nMessagePriority, MessagePriority);
	protected:
		virtual void registerWithTouchDispatcher();
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	private:		
		static MessageBox* selfInstance;

		static bool sUseHightPriotiry;

		TXGUI::UILayout* messageboxLayer;
		TXGUI::UILabel* messageBodyLabel;
		TXGUI::UILabel* messageBodyLabelExt;
		TXGUI::UIButton* middleBtn;
		TXGUI::UIButton* okBtn;
		TXGUI::UIButton* closeBtn;
		TXGUI::UIButton* cancelBtn;
		TXGUI::UILabel* middleContentLabel;
		TXGUI::UILabel* okContentLabel;
		TXGUI::UILabel* cancelContentLabel;
		CCObject* clickEventHander;
		SEL_MenuHandler pfnMiddleClicked;
		SEL_MenuHandler pfnOkClicked;
		SEL_MenuHandler pfnCancelClicked;
        //TXGUI::UIButton* closeBtn;

		std::string middleBtnContent;
		std::string okBtnContent;
		std::string cancelBtnContent;

		bool isOkBtnDefaultBehavior;
	};
};
