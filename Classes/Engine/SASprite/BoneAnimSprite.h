#pragma once

#include "cocos2d.h"
#include "SASpriteDefine.h"
#include "RoleActorBase.h"

class RoleActorBase;

class BoneAnimSprite
{
public:
	BoneAnimSprite(int Type);	
	virtual ~BoneAnimSprite();

	void pause();
	void resume();

	void SetAnim(int animId, int nloop, bool isLoop);
	void SetAnim(int animId, int nloop, bool isLoop, float delayTime);		
	void SetAnimPauseOnOver(bool bPause = false);
	void PauseAllArmatureAnim();
	void SetAnimFlipX(bool bFlipsX = false);
	void SetPos(int x, int y);
	void SetOpacity(int opacity);
	void setTag(int tag);

	CCRect getCurViewRect();
	CCRect getCurColliderRect();
	CCRect getCurAttackBox();
	CCRect getFirstFrameRect();	
	virtual CCSize getCurContentSize();
	int getAnimFrameCount();
	std::vector<CCSpriteBatchNode*>* getBatchNode();

	int getAnimId(){
		return m_animId;
	}
	RoleActorBase * getCurRoleActorBase(){
		return curRoleActorBase;
	}	
	std::map<std::string,RoleActorBase*> * getArmatureDataAndActorBaseMap(){
		return armatureNameAndActorMap;
	}

	bool IsCurAnimFlipX(){return bCurAnimFlip;}
	void drawFrame(int index);
	void addArmatureNodeToParentNode(CCNode *root,int depth);
	void SetArmatureOpacity(GLubyte var);

	void SetAnimUpdateOrNot(bool bUpdate);
	void SetEquipItemsData(unsigned int id);
	bool IsCurAnimCanBecomeRed();
protected:
	void reset();
	void startAnim();
	void show(RoleActorBase *actorBase = 0);	
	void hide(RoleActorBase *actorBase = 0);	
protected:
	float m_delayTime;	
	bool m_loop;
	bool bCurAnimFlip;
	int m_loopCount;
	int m_tag;
	int m_animId;
	int m_curRoleId;
	std::map<std::string,RoleActorBase*> *armatureNameAndActorMap;
	ArmatureActionData *armatureActionData;
	RoleActorBase *curRoleActorBase;
	CCMoveAreaData moveAreaData;
	CCAreaData  viewAreaData;
	int m_curFrame;
    
    // Note: 优化碰撞框获取速度
	CCColliderData m_colliderData;
	std::map<std::string,CCAreaData> m_kvViewData;
};