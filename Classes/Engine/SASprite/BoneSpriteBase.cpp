#include "SpriteBase.h"
#include "LevelDefine.h"
#include "BoneSpriteBase.h"
#include "Engine.h"
#include "EnginePlayerManager.h"
#include "stdlib.h"
#include "ABoxAnimation.h"
#include "SkillDispMgr.h"
#include "Painter.h"
#include "cocos2d.h"
#include "GameResourceManager.h"
#include "LuaTinkerManager.h"

USING_NS_CC;

BoneSpriteBase::BoneSpriteBase(int type)
	:SpriteBase(type)
	,m_animBone(NULL)
	,m_boxAnimation(NULL)
	,m_bInRedState(false)
	,m_alpha(255)
	,weaponId(0)
{
	m_animationState = kType_ANI_STATE_UNKNOW;
	m_bShowColliderBox = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "ShowDebugBox", "collider");
	m_bShowAttackBox = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "ShowDebugBox", "attack");
	m_bShowViewBox = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "ShowDebugBox", "view");
	m_InRedTime = 0;
	mShadowVisible = true;
}

BoneSpriteBase::~BoneSpriteBase()
{
	this->unscheduleAllSelectors();	

	if (m_animBone != NULL)
	{		
		EnginePlayerManager::getInstance()->removeBoneAsprite(m_animBone);
		m_animBone = NULL;
	}

	if(m_effectSprite)
	{
		EnginePlayerManager::getInstance()->removeEffectAsprite(m_effectSprite);
		m_effectSprite = NULL;
	}

	if (m_boxAnimation)
	{
		delete m_boxAnimation;
		m_boxAnimation = NULL;
	}
}

void BoneSpriteBase::attachBoneAnimToRootSprite(int depth)
{
	if (0 != m_rootSprite && 0 != m_animBone)
	{
		m_animBone->addArmatureNodeToParentNode(m_rootSprite,depth);
	}
}

bool BoneSpriteBase::canDealWithTouch(cocos2d::CCTouch* touch)
{
	return containsTouchLocation(touch) && Engine::CanTouchNow();
}


bool BoneSpriteBase::containsTouchLocation(cocos2d::CCTouch* touch)
{
	//
	CCPoint oldPT = touch->getLocationInView();
	oldPT = CCDirector::sharedDirector()->convertToGL(oldPT);	

	CCPoint newPT = LevelMultiResolution::sTransformWindowPointToMap(oldPT);
	CCPoint point = m_rootSprite->convertToNodeSpaceAR(newPT);		
	if (isTouchSelf(CCPointMake(point.x, point.y)))
	{        
		return true;
	}
	else
	{
		return false;
	}
}

bool BoneSpriteBase::isTouchSelf(cocos2d::CCPoint pt)
{
	cocos2d::CCRect rect = getRect();

	if((pt.x >= rect.origin.x) && 
		(pt.x <= rect.origin.x + rect.size.width) &&
		(pt.y >= rect.origin.y) && 
		(pt.y <= rect.origin.y + rect.size.height))
	{
		return true;
	}

	return false;	
}


void BoneSpriteBase::setPosition(const CCPoint &position)
{
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	// 转换成屏幕坐标系

	float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();

	CCPoint realPosition = CCPoint(position.x / deviceScale, position.y / deviceScale);

	// 转换成屏幕坐标系

	m_rootSprite->setPosition(realPosition);
}

cocos2d::CCPoint BoneSpriteBase::getPosition()
{
	return m_rootSprite->getPositionInPixels();
}

/// set box animation
void BoneSpriteBase::SetBoxAnimation(ABoxAnimation* boxAnimation)
{
	delete m_boxAnimation;
	m_boxAnimation = boxAnimation;
}

/// get box animation
const ABoxAnimation* BoneSpriteBase::GetBoxAnimation()
{
	return m_boxAnimation;
}

void BoneSpriteBase::SetAnimPauseOnOver(bool bPause /* = false */)
{
	if (m_animBone)
	{
		m_animBone->SetAnimPauseOnOver(bPause);
	}
}

void BoneSpriteBase::resetParentRole(BoneSpriteBase *parent)
{
	if (0 == parent || 0 == m_animBone)
	{
		return ;
	}
	std::map<std::string,RoleActorBase*> *pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();
	for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
		iter != pMapArmatures->end();
		iter ++)
	{
		(*iter).second->setParentRole(parent);
	}
}

void BoneSpriteBase::PauseAllArmatureAnim()
{
	if (m_animBone)
	{
		m_animBone->PauseAllArmatureAnim();
	}
}

void  BoneSpriteBase::SetAnim(int animId, int nloop, bool isLoop /* = true */,
							  BoneSpriteBase *listerner /* = 0 */)
{
	m_isAnimLoop = isLoop;
	m_animationState = kType_START;
	if (m_animBone)
	{
		RoleActorBase * curRoleActor = m_animBone->getCurRoleActorBase();
		if (0 != curRoleActor)
		{
			curRoleActor->setParentRole(NULL);
		}

		if (false == this->isVisible())
		{
			this->setVisible(true);
		}
		m_animBone->SetAnim(animId, nloop, isLoop);

		curRoleActor = m_animBone->getCurRoleActorBase();
		if (0 != curRoleActor)
		{
			if (0 == listerner)
			{
				listerner = this;
			}
			curRoleActor->setParentRole(listerner);
		}
	}

	m_boxAnimation = SkillDispMgr::Get()->GetCollisionBoxAnim(animId);
}

void BoneSpriteBase::SetAnim(int animId, int nloop, bool isLoop ,CCAnimationEventListener *listerner)
{
	m_isAnimLoop = isLoop;
	m_animationState = kType_START;
	if (m_animBone)
	{
		RoleActorBase * curRoleActor = m_animBone->getCurRoleActorBase();
		if (0 != curRoleActor)
		{
			curRoleActor->setParentRole(NULL);
		}

		if (false == this->isVisible())
		{
			this->setVisible(true);
		}
		m_animBone->SetAnim(animId, nloop, isLoop);

		curRoleActor = m_animBone->getCurRoleActorBase();
		if (0 != curRoleActor)
		{
			curRoleActor->setParentAnimListerner(listerner);
		}
	}

	m_boxAnimation = SkillDispMgr::Get()->GetCollisionBoxAnim(animId);
}

///
void  BoneSpriteBase::SetAnimFlipX(bool bFlipsX /* = false */)
{
	if (m_animBone)
	{
		m_animBone->SetAnimFlipX(bFlipsX);
	}

	if (m_boxAnimation)
	{
		m_boxAnimation->SetAnimFlipX(bFlipsX);
	}
}

int BoneSpriteBase::getCurAnimationFrameID()
{
	//return curAnimationFrame;
	if (0 != m_animBone)
	{
		RoleActorBase *curRoleActor = m_animBone->getCurRoleActorBase();
		if (0 != curRoleActor)
		{			
			return curRoleActor->getAnimation()->getCurFrame();
		}
	}
	return 0;
}

ANIMATION_STATE BoneSpriteBase::getCurAnimationState()
{
	return m_animationState;
}

void BoneSpriteBase::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{

	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		m_animationState = kType_COMPLETE;
	}
	else if (0 == strcmp(_aniType,"start"))
	{
		m_animationState = kType_START;
	}
	else if (0 == strcmp(_aniType,"inFrame"))
	{
		m_animationState = kType_RUN;
	}
}

void BoneSpriteBase::drawEditorRect()
{
	if (m_bShowAttackBox)
	{
		CCRect rect = getRelativeParentAttackBox();
		Painter::drawRect(rect,0, 0, 255, 255);
	}
		
	if (m_bShowColliderBox)
	{
		CCRect rect = getRelativeParentColliderRect();
		Painter::drawRect(rect,255, 0, 255, 255);
	}

	if (m_bShowViewBox)
	{
		CCRect rect = getRelativeParentViewRect();
		Painter::drawRect(rect,255, 255, 0, 255);
	}
}

void BoneSpriteBase::draw()
{
	drawEditorRect();
}

bool BoneSpriteBase::getAttachPoint(int pointPosType,cocos2d::CCPoint &point)
{
	if (false == SpriteBase::getAttachPoint(pointPosType,point))
	{
		return false;
	}
	if (m_animBone)
	{
		if(m_animBone->IsCurAnimFlipX())
		{
			point.x = -point.x;
		}
	}
	return true;	
}

void BoneSpriteBase::releaseAllUsedTextureFrames()
{
	if (m_animBone)
	{
		std::map<std::string,RoleActorBase*> * pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();

		for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
			iter != pMapArmatures->end();
			iter ++)
		{
			std::string plist = (*iter).second->getName() + ".plist";
			plist = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,plist.c_str());
			const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist.c_str());
			CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);

			CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
			int format = 0;

			CCDictElement* pElement = NULL;
			CCDICT_FOREACH(framesDict, pElement)
			{
				CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
				std::string spriteFrameName = pElement->getStrKey();

				CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str());
				if (pFrame)
				{
					pFrame->release();
				}
			}

			dict->release();
		}
	}
}

void BoneSpriteBase::retainAllUsedTextureFrames()
{
	if (m_animBone)
	{
		std::map<std::string,RoleActorBase*> * pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();

		for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
			iter != pMapArmatures->end();
			iter ++)
		{
			std::string plist = (*iter).second->getName() + ".plist";
			plist = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,plist.c_str());
			const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist.c_str());
			CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);

			CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
			int format = 0;

			CCDictElement* pElement = NULL;
			CCDICT_FOREACH(framesDict, pElement)
			{
				CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
				std::string spriteFrameName = pElement->getStrKey();

				CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str());
				if (pFrame)
				{
					pFrame->retain();
				}
			}

			dict->release();
		}
	}
}

bool BoneSpriteBase::playBecomeRedEffect(bool isMonster)
{
	if (m_animBone)
	{
		if (m_animBone->IsCurAnimCanBecomeRed() || isMonster)
		{
			if (false == m_bInRedState)
			{		
				ccColor3B color;
				color.r = 255;
				color.g = color.b = 0;

				std::map<std::string,RoleActorBase*> * pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();
				for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
					iter != pMapArmatures->end();
					iter ++)
				{
					(*iter).second->setArmatureColor(color);
				}

				m_bInRedState = true;

				return true;
			}
		}
	}
	return false;
}

void BoneSpriteBase::unscheduleAllAnimationUpdate()
{
	std::map<std::string,RoleActorBase*> * pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();
	for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
		iter != pMapArmatures->end();
		iter ++)
	{
		(*iter).second->unscheduleUpdate();
	}
}

void BoneSpriteBase::revertToOriginColor()
{
	if (m_bInRedState)
	{
		ccColor3B color;
		color.r = color.g = color.b = 255;

		std::map<std::string,RoleActorBase*> * pMapArmatures = m_animBone->getArmatureDataAndActorBaseMap();
		for (std::map<std::string,RoleActorBase*>::iterator iter = pMapArmatures->begin();
			iter != pMapArmatures->end();
			iter ++)
		{
			(*iter).second->setArmatureColor(color);
		}
		
		m_bInRedState = false;
	}
}

/// <summary>
//	获取区域信息
/// </summary>
cocos2d::CCRect BoneSpriteBase::getRelativeParentColliderRect()
{
	if(m_animBone != NULL)
	{
		cocos2d::CCRect rect = m_animBone->getCurColliderRect();		
		cocos2d::CCPoint pos = GetItemPosition();
		return CCRectMake(rect.origin.x + pos.x, rect.origin.y + pos.y, rect.size.width, rect.size.height);	
	}	

	return CCRectMake(0, 0, 0, 0);	
}

cocos2d::CCRect BoneSpriteBase::getRelativeParentAttackBox()
{
	cocos2d::CCRect rect;
	if (m_animBone)
	{
		rect = m_animBone->getCurAttackBox();
		CCPoint pos = getPosition();
		rect.origin.x += pos.x;
		rect.origin.y += pos.y;
	}

	return rect;
}

cocos2d::CCRect BoneSpriteBase::getRelativeParentViewRect()
{
	cocos2d::CCRect rect;
	if (m_animBone)
	{
		rect = m_animBone->getCurViewRect();
		CCPoint pos = GetItemPosition();
		rect.origin.x += pos.x;
		rect.origin.y += pos.y;
	}

	return rect;
}

/// <summary>
//	获取绑定点的位置
/// </summary>
cocos2d::CCPoint BoneSpriteBase::getAttachPoint(ATTACH_POINT_TYPE type)
{
	CCPoint p = CCPointZero;
	if (false == SpriteBase::getAttachPoint(type,p))
	{
		return CCPointZero;
	}
	if (m_animBone)
	{
		if(m_animBone->IsCurAnimFlipX())
		{
			p.x = -p.x;
		}
	}
	return p;
}

unsigned int BoneSpriteBase::GetAnimID( void )
{
	return m_animBone->getAnimId();
}

bool BoneSpriteBase::IsAnimFlipX( void )
{
	return m_animBone->IsCurAnimFlipX();
}

void BoneSpriteBase::SetRoleOpacity(GLubyte var)
{
	if (m_animBone)
	{
		m_animBone->SetArmatureOpacity(var);
		m_alpha = var;
	}
}

GLubyte BoneSpriteBase::GetRoleOpacity( void )
{
	return m_alpha;
}

CCPoint BoneSpriteBase::GetItemPosition() 
{
	return this->getPosition();
}

void BoneSpriteBase::SetUpdateOrNot(bool bUpdate)
{
	AreaItemNode::SetUpdateOrNot(bUpdate);
	if (m_animBone)
	{
		m_animBone->SetAnimUpdateOrNot(bUpdate);
	}
}

CCRect BoneSpriteBase::GetABBox()
{
	if(m_animBone != NULL)
	{
		return getRelativeParentViewRect();
	}
}

void BoneSpriteBase::SetEquipItemsData(unsigned int id)
{
	if (m_animBone)
	{
		weaponId = id;
		//if (0 == id)
		//{
		//	return ;
		//}
		m_animBone->SetEquipItemsData(id);
	}
}

unsigned int BoneSpriteBase::GetEquipWeaponId()
{
	return weaponId;
}

void BoneSpriteBase::setVisible(bool visible)
{
    if (m_animBone) {
        if (false == visible) {
            m_animBone->SetAnimUpdateOrNot(false);
        }
        else{
            m_animBone->SetAnimUpdateOrNot(true);
        }
    }

    CCNode::setVisible(visible);
}


