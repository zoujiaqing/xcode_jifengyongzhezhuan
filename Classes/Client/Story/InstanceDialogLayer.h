#pragma once

#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "UIPicture.h"
#include "TXGUIHeader.h"
#include "StoryData.h"

using namespace std;
using namespace cocos2d;

class InstanceDialogLayer : public CCLayer
{    
public:
	InstanceDialogLayer();
	virtual ~InstanceDialogLayer();

	CREATE_FUNC(InstanceDialogLayer);
	virtual bool init();

	void okBtnClickEvent(CCObject* pObj);
	void setStoryDialogData(StoryDialogData data);
	void setStoryRoleBasicData(StoryRoleBasicData data);
	void SetStoryDisplayPicName(std::string picName);

	void HideLayerAndClean();
protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void ShowNextIconAnimation(bool bShow);
	void CreateNetIconAnimation();
public:
	virtual void setTouchEnabled(bool value);
	void ShowDialogPanel();
	void ShowDisplayPicPanel();
	void ClearDialogPanel();
	void ClearMiddlePanel();
	void ShowDialogPanelOrNot(bool bShow);
	void ShowDisplayPanelOrNot(bool bShow);
protected:
	TXGUI::UILayout* m_pLayer;
	TXGUI::UILabel*  m_pContentLabel;
	TXGUI::UILabel*  m_pRoleNameLabel;
	TXGUI::UIPicture* m_leftHeroLogoPic;
	TXGUI::UIPicture* m_middleTipLogoPic;
	TXGUI::UIPicture* m_dialogBGPic;
	TXGUI::UIButton* m_okBtn;
	TXGUI::UIPicture* m_areaPic;
	TXGUI::UILabel*  m_okBtnLabel;
	StoryDialogData mStoryDialogData;
	StoryRoleBasicData mStoryRoleBasicData;
	std::string mDisplayPicName;
private:
	enum {LEFT_HERO_PIC_TAG = 1,MIDDLE_TIP_PIC_TAG = 2};
	CCNode* m_pNextAnimParentNode;
	cocos2d::CCSprite* m_pNextIcon;
	TXGUI::UIPicture*  m_pNextIconPic;

	float leftHeroLogoWidthOffset;
	float leftHeroLogoHeightOffset;
};