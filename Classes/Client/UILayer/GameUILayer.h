//
//  MainMenuLayer.h
//  iSeer
//
//  Created by arthurgong on 11-12-8.
//  Copyright 2011ƒÍ __MyCompanyName__. All rights reserved.
//
#pragma once

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "UIContainer.h"
#include "UILabel.h"
#include "UIPicture.h"
#include "TaskData.h"
#include "TimeManager.h"
#include "MarketManager.h"
#include "UIPopupReward.h"

using namespace std;
using namespace cocos2d;

using namespace TXGUI;

class GameUILayer : public cocos2d::CCLayer,
					public TXGUI::ObserverProtocol,
					TimeProtocol
{    
public:
	GameUILayer();
	virtual ~GameUILayer();
   
	CREATE_FUNC(GameUILayer);
    virtual bool init();

	/// hide all buttons
	void	PuckerButtons();

	void	showSpecialButtonEffect();

	bool	IsOneButtonInExpandList(TXGUI::UIButton* pBtn);
	bool	ShrinkButtonClickRightNow(bool TrackUserSet = false,bool bPushed = false,CCCallFunc* pfnOkClicked = NULL);

	void	MapButtonPointsH(const char * szName,unsigned int tag);
	void	MapButtonPointsW(const char * szName,unsigned int tag);

	void	ShowAutoButtons(bool bShow);

	void	ReCalMapButtonPos(bool bHor);

protected:
	/// store the button points
	void	MapButtonPointsH(TXGUI::UILayout* layout, const char * szName,unsigned int tag);
	/// pucker buttons to right bottom corner
	void    PuckerButtonsH(CCPoint ptDest);
	/// un pucker buttons from right bottom corner
	void    UnPuckerButtonsH();

	/// store the button points
	void	MapButtonPointsW(TXGUI::UILayout* layout, const char * szName,unsigned int tag);
	/// pucker buttons to right bottom corner
	void    PuckerButtonsW(CCPoint ptDest);
	/// un pucker buttons from right bottom corner
	void    UnPuckerButtonsW();
	void  GameUIUpdate(float dt) ;
	/// open bag 
    void	OnButtonBag(CCObject *sender);
    
	void	OnBuyStaminaClicked(CCObject *sender);

    ///open Talent
    void    OnButtonTalent(CCObject *sender);

	void    OnButtonSetting(CCObject *sender);
	/// shrink button
	void	OnButtonShrink(CCObject* sender);	

	void	OnButtonUpgrade(CCObject* sender);

	void	OnButtonAlchemy(CCObject* sender);

	void	OnButtonStrength(CCObject* sender);
    
    void    OnButtonFirstPayReward(CCObject* sender);

	//// on button activity
	//void	OnButtonActivity(CCObject* sender);

	//// on head shot click
	void	OnHeadShotClick(CCObject* sender);
	void	OnDelRoleMessageOK(CCObject* pObject);
	void	OnPopupRewardLayerOkBtnClick(CCObject* pObject);

	void	OnCardClick(CCObject* obj);

	/**
	* Instruction
	* @param 
	*/
	void	OnButtonFairy(CCObject* sender);	
    
    /////on button reward
    void    OnButtonReWard(CCObject* sender);

	//// on task btn click
	void	OnButtonTask(CCObject* sender);

	void	OnButtonRank(CCObject* sender);


	void	HideButton(CCNode* sender);
	void	ShowButton(CCNode* sender);

	/**
	* Instruction
	* @param 
	*/
	void	OnButtonInstanceClick(CCObject* sender);

    void    OnButtonRankClicked(CCObject* sender);
    void    OnButtonMallClicked(CCObject* sender);
    void    OnButtonBOSSCombatSettingClicked(CCObject* sender);
    void    OnButtonPlatActivity(CCObject* sender);
    void    OnButtonStarsRewardClicked(CCObject* sender);
    void    OnButtonTreasureExploreClicked(CCObject* sender);
    void    OnButtonSociatyClicked(CCObject* sender);

    void    onButtonCharge(CCObject* sender);
    void    OnVipPrivilegeClicked(CCObject* sender);

	virtual void onBroadcastMessage(BroadcastMessage* msg);

public:
	/// override of node
	virtual void setVisible(bool visible);

	void	UpdateTaskUpRightIcon(std::string icon_name,eTaskState state = kType_TASK_UNKNOW);

	void  checkEquipUpgradeIcon();

	virtual void onUpdateRemainTime(long remainTime);

	void stopHeroMove();

	// Note: 红名
	void ShowRedNamePanel();
	void HideRedNamePanel();

	void RenewRedNameTimer();

    void ShowIconEffect();
    void HideIndexButton();

	void RefreshPotentiometer();
private:
	void resetPlayerGoldLabel();
	void resetPlayerDiamondLabel();
	void resetPlayerReputationLabel();
	void resetPlayerStaminaLabel();
	void setPlayerLevel();	
	void setPlayerStrength();
	void showEquipFragCanComposeIcon();
	void showFairyTipIcon();
	void showTargetTipIcon();
	void showNewSkillTipIcon();
	void showRedNamePanel(bool bShow);
    void ShowMarketButtonIcon();
	// 设置m_bShowTipOnMarketButton,根据是否购买当前vip等级对应的礼包

    void OpenMarketLayer();
    void OpenChargeLayer();
    void OpenFirstChargeLayer();
    void OpenPlatAcitivityLayer();
    void OpenVipLayer();
    void OnNextFrame(float dt);

   

protected:
	map<TXGUI::UIButton*, CCPoint> m_mapBttonPointH, m_mapBttonPointW;
	map<unsigned int,TXGUI::UIButton*> m_mapButtonIndexH, m_mapButtonIndexW;
	CCPoint m_ptDestShrink;
	CCPoint m_ptLocalDestShrink;
	CCSize mShrinkSize;

	bool m_bShowTipOnMarketButton;  // 是否在MarketButton上显示tip

	TXGUI::UIButton* m_pBtnShrink;
	bool m_bPuckered;
	unsigned int last_effectiveness ;
private:
	// Note: 红名显示相关控件
	//TXGUI::UIContainer* m_pRedNameContainer;
	TXGUI::UIPicture* m_pRedNameBackground;
	TXGUI::UILabel* m_pRedNamePublishTxtLabel;
	TXGUI::UILabel* m_pRedNamePublishNumLabel;
	TXGUI::UILabel* m_pRedNameTimerLabel;

	CCControlPotentiometer* m_expPotentiometer;

    // 按钮上的光效
	CC_SYNTHESIZE_RETAIN(CCNode*, m_firstPlayEffect, FirstPlayEffect);
};
