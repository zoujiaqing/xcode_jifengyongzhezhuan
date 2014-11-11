#pragma once

#include "string"
#include "cocos2d.h"
#include "NPCDataDefine.h"
#include "BoneSpriteMoveable.h"

USING_NS_CC;

// Note: NPC实例类
class SpriteNPC :  public BoneSpriteMoveable
{
public:
	SpriteNPC(int type);
	virtual ~SpriteNPC();

	static SpriteNPC* NPCWithData(NPCData *pData);
	std::string GetName(){
		return mNpcDataInfo->mName;
	}

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void registerWithTouchDispatcher();
	virtual bool				isTouchSelf(cocos2d::CCPoint pt);
	virtual cocos2d::CCRect		getRect();

	void ShowFlagSignIcon(NPCFlagSign flag);
	void OneActorMoveEndEvent();
	NPCData* GetData(){return mNpcDataInfo;}
	/**
	* Instruction : 移到到某处
	* @param 
	*/
	void MoveToTarget(CCPoint pos,CCRect colliderRect = CCRectZero);
protected:
	virtual bool InitWithData(NPCData *pData);
	void PlayIdleAnimation();

	// Note: 获取NPC相关的图标
	void GetTaskFlagSignIcon();
	void GetUpLogoImageIcon();
	//void GetTaskIntroImageIcon();
	//void GetHeadImageIcon();
	void GetShadowImageIcon();

	// Note: 根据资源名字得到CCSprite
	CCSprite* GetSpriteFromSystemIconByName(const char *rcs_name);

	// Note: 绑定NPC想图标
	void AttachImageIcon();
private:
	std::string mName;
	NPCData*	mNpcDataInfo;

	// Note: Icon Asprite
	CCSprite *mTaskFlagSignIcon[3];
	CCSprite *mHeadImageIcon;
	ASprite *mShadowImageIcon;

	CCRect mSelfColliderRect;
};