//
//  MainMenuLayer.h
//  iSeer
//
//  Created by arthurgong on 11-12-8.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UIPicture.h"
#include "UILabelTTF.h"
#include "UILayout.h"
#include "UILabel.h"
#include "TimeProtocol.h"
#include "HealthPointProtocol.h"
#include <map>
using namespace std;
using namespace cocos2d;

class ComboLayer;

//
// 管理战斗UI点击逻辑
//
class BattleUILayer : public cocos2d::CCLayer, public TimeProtocol
{    
public:
    enum Z_MAINMENU
    {
        Z_ITEM_BACK,
        Z_ITEM_TOUCH,
        Z_ITEM_TOUCH2,
        Z_ITEM_BACK2,
        
    };
    
public:
	BattleUILayer();
	virtual ~BattleUILayer();
   
	CREATE_FUNC(BattleUILayer);
    virtual bool init();

	void initSkillIcon();

    void selectSpritePack(CCObject *sender);
    void selectMap(CCObject *sender);

    void selectItemPack(CCObject *sender);

    void selectShop(CCObject *sender);
    void selectTask(CCObject *sender);

    void selectSet(CCObject *sender);
    void selectHelp(CCObject *sender);
    
	void normalAttack(CCObject *sender);
    
    void autoSkillAttack(int skillId);

	void skillOneAttack(CCObject *sender);
	void skillTwoAttack(CCObject *sender);
	void skillThreeAttack(CCObject *sender);
	void skillFourAttack(CCObject *sender);
	void skillFiveAttack(CCObject *sender);
    
	void backToTown(CCObject *sender);
	void findEnemy(CCObject *sender);

	void onClickedAffirmBt(CCObject* sender);
	void onClickedCancellBt(CCObject* sender);

	void onPressTimeoutOK(CCObject* sender);
	void onPressPlayerDieOK(CCObject* sender);

    void updateMapName();
	void update(float dt);
	TXGUI::UIButton* getNormalAttackBtn(){return m_normalAttackBtn;}

	void SetEnemyHpLable(float currentHp,bool isAnim,PLAYER_ID targetMonster,unsigned int type,unsigned int enemyType);
    
    void setupOwnerCD(int skillID, const char* buttonname);
    void setupShareCD(const char* buttonname);
    
    void runSkillProgress(TXGUI::UIButton* button, CCSprite* sprite, float dt);
    
    void progressDone(cocos2d::CCNode *sender);
    
    bool isProgressRunning(const char* uibuttonname);
    
    //remain time
    void initRemainTime(long seconds);
	virtual void onUpdateRemainTime(long remainTime);
    
	void HandleComboIncrease(unsigned int count = 1);
    
    void clearSkillProgressTimer();

	unsigned int GetMaxComboNumber();
	UILayout* get_layout(){return  m_layout;}
protected:   

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

#if WIN32
	virtual void processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam);
#endif
    
private:
	void setPlayerLevel();
	HealthPointProtocol* getHealthPointContainer(unsigned int monsterType);
private:
    UILabelTTF* m_mapName;
    UILabelTTF* m_doubleExpCountLabel;
    CCSprite*   m_doubleExp;
    ASprite*    m_curUiAs;
    
    CCNode*     m_bottomNode;   // parent node of bottom UI
    
    UILabelTTF* m_gemLabel;
	UILabelTTF* m_userName;
	TXGUI::UILabel* m_enemyNameLabel;
	CCSprite* m_enemyHpLable;
	CCSprite* m_enemyHpFrame;
	TXGUI::UIButton* m_normalAttackBtn;
	unsigned int M_targetMonsterId;
	bool b_checkMonster;
	TXGUI::UIPicture* m_headPic;
	UILayout* m_layout;
    
	ComboLayer* m_comboLayer; // 显示combo

    bool m_isSKill1Run;
    bool m_isSkill2Run;
    bool m_isSkill3Run;
    bool m_isSkill4Run;
    bool m_isSkill5Run;
    
    CCProgressTimer *m_skillOwn1;
    CCProgressTimer *m_skillShare1;
    
    CCProgressTimer *m_skillOwn2;
    CCProgressTimer *m_skillShare2;
    
    CCProgressTimer *m_skillOwn3;
    CCProgressTimer *m_skillShare3;
    
    CCProgressTimer *m_skillOwn4;
    CCProgressTimer *m_skillShare4;
    
    CCProgressTimer *m_skillOwn5;
    CCProgressTimer *m_skillShare5;
    
    UILabelTTF* remainText;
    UILabelTTF* remainNum;
    std::map<unsigned int,HealthPointProtocol*>* m_hpLabelMap;
	HealthPointProtocol* m_currentHPLable;
    
    unsigned int m_skillId1, m_skillId2, m_skillId3, m_skillId4, m_skillId5;
};






