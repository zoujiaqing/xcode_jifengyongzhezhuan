#include "BoneAnimSprite.h"
#include "SkeletonAnimRcsManager.h"
#include "cocos2d.h"
#include "CConnectionData.h"
#include "DressUpManager.h"
#include <string.h>

USING_NS_CC;

/// <summary>
//	得到角色相关的所有骨架
//	得到角色所有动作 及 动作相关的骨架
/// </summary>
BoneAnimSprite::BoneAnimSprite(int id)
	: m_curFrame(-1)
{	
	reset();
	m_curRoleId = id;	
	armatureNameAndActorMap = SkeletonAnimRcsManager::getInstance()->createOneRoleArmatures(id);
	hide();
	armatureActionData = SkeletonAnimRcsManager::getInstance()->getOneRoleArmatureActionData(id);
	CCConnectionData::sharedConnectionData()->GetRoleColliderData(id,m_colliderData);
	CCConnectionData::sharedConnectionData()->GetRoleViewRectData(id,m_kvViewData);
}

BoneAnimSprite::~BoneAnimSprite()
{
	if (0 != armatureNameAndActorMap)
	{
		for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
			iter != armatureNameAndActorMap->end(); iter++)
		{
			RoleActorBase* roleActorBase = (*iter).second;
			roleActorBase->removeFromParentAndCleanup(true);
		}
		armatureNameAndActorMap->clear();
		CC_SAFE_DELETE(armatureNameAndActorMap);
	}
}

/// <summary>
//	得到角色相关的所有batchNode
/// </summary>
std::vector<CCSpriteBatchNode*>* BoneAnimSprite::getBatchNode()
{
	if (armatureNameAndActorMap) 
	{
		std::vector<CCSpriteBatchNode*>* pVec = new std::vector<CCSpriteBatchNode*>();
		if (0 == pVec)
		{
			return 0;
		}
		for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
			iter != armatureNameAndActorMap->end(); iter++)
		{
			pVec->push_back((*iter).second->getBatchNode());
		}
		return pVec;
	}
	else 
	{
		return 0;
	}
}

void BoneAnimSprite::addArmatureNodeToParentNode(CCNode *root,int depth)
{
	if (0 != root && armatureNameAndActorMap != NULL)
	{
		std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
		for (;iter != armatureNameAndActorMap->end(); iter++)
		{
			root->addChild((*iter).second->getBatchNode(),depth);
			root->addChild((*iter).second,depth);
		}
	}
}

void BoneAnimSprite::SetAnimFlipX(bool bFlipsX /* = false */)
{
	if (0 != curRoleActorBase)
	{
		bCurAnimFlip = bFlipsX;

		float angle = 0;		
		if (bFlipsX)
		{
			angle = 180;
		}
		curRoleActorBase->getDisplay()->setRotationY(angle);
	}
}

void BoneAnimSprite::startAnim()
{
	SetAnim(m_animId, m_loopCount, m_loop);
}

void BoneAnimSprite::SetAnimPauseOnOver(bool bPause /* = false */)
{
	if (curRoleActorBase)
	{
		curRoleActorBase->getAnimation()->setIsPauseOnOver(bPause);
	}
}

void BoneAnimSprite::PauseAllArmatureAnim()
{
	std::map<std::string,RoleActorBase*>::iterator role_iter = armatureNameAndActorMap->begin();
	if (role_iter != armatureNameAndActorMap->end())
	{
		(*role_iter).second->pauseAllAnimation();
		role_iter ++;
	}
}

bool BoneAnimSprite::IsCurAnimCanBecomeRed()
{
	std::vector<int> vec;
	vec.push_back(0);
	vec.push_back(1000);
	vec.push_back(1001);
	vec.push_back(1002);
	vec.push_back(1003);
	vec.push_back(1004);
	vec.push_back(1008);
	vec.push_back(1009);
	vec.push_back(1010);
	vec.push_back(1011);
	vec.push_back(1012);
	vec.push_back(1013);
	vec.push_back(1023);
	vec.push_back(1024);
	vec.push_back(1025);
	vec.push_back(1026);
	vec.push_back(1027);
	vec.push_back(1028);

	for (std::vector<int>::iterator iter = vec.begin();
		 iter != vec.end(); iter++)
	{
		if (m_animId == (*iter))
		{
			return true;
		}
	}

	return false;
}

void BoneAnimSprite::SetAnim(int animId, int nloop, bool isLoop)
{
	//animId = kTypeIdle_FaceDown_Stand_OTH;
	//if (m_animId == animId && isLoop == m_loop && m_loopCount == nloop)
	//{
	//	return;
	//}
	m_delayTime = 0;
	m_loop = isLoop;
	m_loopCount = nloop;	
	m_animId = animId;

	RoleActorBase *preActorBase = curRoleActorBase;

	std::map<RoleActionType,ArmatureData>::iterator action_iter =
		armatureActionData->kvRoleActionTypeAndValues.find((RoleActionType)animId);
	if (action_iter == armatureActionData->kvRoleActionTypeAndValues.end())
	{
		return ;
	}

	std::string armature_name = (*action_iter).second.xmlFileName;
	std::string actionName = (*action_iter).second.actionName;
	std::map<std::string,RoleActorBase*>::iterator role_iter = armatureNameAndActorMap->find(armature_name);
	if (role_iter != armatureNameAndActorMap->end())
	{
		curRoleActorBase = (*role_iter).second;
		if (0 != preActorBase && preActorBase != curRoleActorBase)
		{		
			preActorBase->pauseAllAnimation();
		}

        std::map<int,CCMoveAreaData>::iterator moveAreaIter = m_colliderData.kvMoveIdAndAreaData.find(animId);
		if (moveAreaIter != m_colliderData.kvMoveIdAndAreaData.end())
		{
			moveAreaData = (*moveAreaIter).second;
		}        
        std::map<std::string,CCAreaData>::iterator viewAreaIter = m_kvViewData.find(armature_name);
		if (viewAreaIter != m_kvViewData.end())
		{
			viewAreaData = (*viewAreaIter).second;
		}        
		
		SetAnimFlipX(false);
		curRoleActorBase->getAnimation()->playTo((void *)actionName.c_str(),-1,-1,isLoop);	
		curRoleActorBase->resumeAllAnimation();
		curRoleActorBase->getCurBoneLst();
	}
	else
	{
        char buffer[100];
        sprintf("Armature Not Found %s",armature_name.c_str());
		CCAssert(true,buffer);
	}
}

void BoneAnimSprite::SetAnim(int animId, int nloop, bool isLoop, float delayTime)
{
	SetAnim(m_animId, m_loopCount, m_loop);
}

void BoneAnimSprite::pause()
{
	if (0 != curRoleActorBase)
	{
		curRoleActorBase->getAnimation()->pause();
	}	
}

void BoneAnimSprite::resume()
{
	if (0 != curRoleActorBase)
	{
		curRoleActorBase->getAnimation()->resume();
	}
}

/// <summary>
//	TODO delete
/// </summary>
void BoneAnimSprite::SetPos(int x, int y)
{
	if (0 == armatureNameAndActorMap)
	{
		return ;
	}
	for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
		iter != armatureNameAndActorMap->end(); iter++)
	{
		(*iter).second->getDisplay()->setPosition(x,y);
	}
}

void BoneAnimSprite::SetOpacity(int opacity)
{
	if (0 == armatureNameAndActorMap)
	{
		return ;
	}
	for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
		iter != armatureNameAndActorMap->end(); iter++)
	{
		((CCSprite*)((*iter).second->getDisplay()))->setOpacity(opacity);
	}
}


void BoneAnimSprite::drawFrame(int index)
{
}

CCRect BoneAnimSprite::getFirstFrameRect()
{
	return CCRectZero;
}

CCSize BoneAnimSprite::getCurContentSize()
{
	return CCSizeZero;
}

int BoneAnimSprite::getAnimFrameCount()
{
	return 0;
}

void BoneAnimSprite::setTag(int tag)
{
	// TODO: setTag
	//this->setTag(tag);
}

void BoneAnimSprite::reset()
{
	bCurAnimFlip = false;
	armatureActionData = 0;
	curRoleActorBase = 0;
	armatureNameAndActorMap = 0;
	m_curRoleId = 0;
}

/// <summary>
//	显示当前动画
/// </summary>
void BoneAnimSprite::show(RoleActorBase *actorBase /* = 0 */)
{
	if (0 != actorBase)
	{
		actorBase->getDisplay()->setVisible(true);
	}
	else
	{
		curRoleActorBase->getDisplay()->setVisible(true);
	}
}

/// <summary>
//	隐藏该角色
/// </summary>
void BoneAnimSprite::hide(RoleActorBase *actorBase /* = 0 */)
{
	if (0 != actorBase)
	{
		actorBase->getDisplay()->setVisible(false);
	}
	else
	{
		// TODO rootsrpite.setvisiable
		if (0 != armatureNameAndActorMap)
		{
			for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
				iter != armatureNameAndActorMap->end(); iter++)
			{
				RoleActorBase * second = (*iter).second;
				second->getDisplay()->setVisible(false);
				second->pauseAllAnimation();
			}
		}
	}
}

/// <summary>
//	得到当前攻击框
/// </summary>
cocos2d::CCRect BoneAnimSprite::getCurAttackBox( void )
{	
	if (curRoleActorBase)
	{
		int curFrame = curRoleActorBase->getAnimation()->getCurFrame() *
			(float)(curRoleActorBase->getAnimation()->getNoScaleListFrames()) /
			(float)(curRoleActorBase->getAnimation()->getDurationTween());
		
		// 如果有多个使用者在同一帧调用这个接口，那么第一使用者得到的结果是True，第二个得到的是False，显然，这是不对的。
		//if(m_curFrame != curFrame)
		{
			
			m_curFrame = curFrame;
			std::map<int,CCAreaData>::iterator iter = moveAreaData.kvFrameIndexAndAreaData.find(curFrame);
			if (iter != moveAreaData.kvFrameIndexAndAreaData.end())
			{
				CCAreaData areaData = (*iter).second;
				float x = 0;
				float y = 0;
				if (bCurAnimFlip)
				{
					x = (float)(-areaData.midX - areaData.width/2);
					y = (float)(areaData.midY - areaData.height/2);
				}
				else
				{
					x = (float)(areaData.midX - areaData.width/2);
					y = (float)(areaData.midY - areaData.height/2);
				}			
				return CCRect(x,y,areaData.width,areaData.height);
			}
		}
		
	}
	return cocos2d::CCRectZero;
}

/// <summary>
//	获取当前视图区
/// </summary>
CCRect BoneAnimSprite::getCurViewRect()
{
	CCAreaData areaData = viewAreaData;
	float x = 0;
	float y = 0;
	if (bCurAnimFlip)
	{
		x = (float)(-areaData.midX - areaData.width/2);
		y = (float)(areaData.midY - areaData.height/2);
	}
	else
	{
		x = (float)(areaData.midX - areaData.width/2);
		y = (float)(areaData.midY - areaData.height/2);
	}		
	return CCRect(x,y,areaData.width,areaData.height);
}
/// <summary>
//	获取当前碰撞区域
/// </summary>
CCRect BoneAnimSprite::getCurColliderRect()
{
	CCAreaData areaData = moveAreaData.selfColliderArea;
	float x = 0;
	float y = 0;
	if (bCurAnimFlip)
	{
		x = (float)(-areaData.midX - areaData.width/2);
		y = (float)(areaData.midY - areaData.height/2);
	}
	else
	{
		x = (float)(areaData.midX - areaData.width/2);
		y = (float)(areaData.midY - areaData.height/2);
	}		
	return CCRect(x,y,areaData.width,areaData.height);
}

void BoneAnimSprite::SetArmatureOpacity(GLubyte var)
{
	for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
		iter != armatureNameAndActorMap->end(); iter++)
	{
		(*iter).second->setArmaturetOpacity(var);
	}
}

void BoneAnimSprite::SetAnimUpdateOrNot(bool bUpdate)
{
	for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
		iter != armatureNameAndActorMap->end(); iter++)
	{
		(*iter).second->SetUpdateEnableOrNot(bUpdate);
	}
}

void BoneAnimSprite::SetEquipItemsData(unsigned int id)
{
	std::vector<std::string> vecOut;
	if (DressUpManager::Get()->GetHeroEquipByID(id,vecOut))
	{
		for (std::map<std::string,RoleActorBase*>::iterator iter = armatureNameAndActorMap->begin();
			iter != armatureNameAndActorMap->end(); iter++)
		{
			(*iter).second->SetEquipItemsData(vecOut);
		}
	}
}