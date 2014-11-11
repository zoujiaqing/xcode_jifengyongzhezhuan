#include "EffectSprite.h"
#include "SkeletonAnimRcsManager.h"
#include "ParticleManager.h"
#include "EnginePlayerManager.h"
#include "cocos2d.h"
#include "GameFontManager.h"
#include "ParticleManager.h"

USING_NS_CC;

EffectSprite::EffectSprite(int Type):BoneSpriteMoveable(Type,false)
{
	mEffectNameLabel = 0;
	m_effectSprite = 0;
	bStartAnim = false;
	preType = kType_Play;
}

EffectSprite::~EffectSprite()
{
	if (bStartAnim)
	{
		bStartAnim = false;
		//ParticleManager::Get()->RemoveOneParticlePlayedNum(m_type);
	}
}

bool EffectSprite::isTouchSelf(cocos2d::CCPoint pt)
{
	if (m_animBone)
	{
		for (std::map<std::string,RoleActorBase*>::iterator iter = m_animBone->getArmatureDataAndActorBaseMap()->begin();
			iter != m_animBone->getArmatureDataAndActorBaseMap()->end(); iter++)
		{
			cocos2d::CCSize size = (*iter).second->getBatchNode()->getContentSize();
			cocos2d::CCPoint point = (*iter).second->getPosition();
			if((pt.x >= point.x) && 
				(pt.x <= point.x + size.width) &&
				(pt.y >= point.y) && 
				(pt.y <= point.y + size.height))
			{
				return true;
			}
		}
	}
	return false;
}

CCSize EffectSprite::getCurContentSize()
{
	if (m_animBone)
	{
		return m_animBone->getCurViewRect().size;
	}
	return CCSizeZero;
}

CCSize EffectSprite::getColliderRectSize()
{
	if (m_animBone)
	{
		return m_animBone->getCurColliderRect().size;
	}
	return CCSizeZero;
}

EffectSprite* EffectSprite::effectWithID(int s_id,const char *name)
{
	EffectSprite *effect = new EffectSprite(s_id);
	if (effect && effect->initWithId(s_id,name))
	{
		effect->autorelease();
		return  effect;
	}
	CC_SAFE_DELETE(effect);
	return NULL;
}


bool EffectSprite::initWithId(int id,const char *name)
{
	m_ID.setPlayId(id,0,0);
	m_TypeID = id;
	m_name = std::string(name);

	m_animBone =  EnginePlayerManager::getInstance()->addBoneAnimSprite(id); 
	attachBoneAnimToRootSprite(1);

	resetParentRole(this);

	if (0 != strcmp("",name))
	{
		float labelFontSize = GameFontManager::smallFontSize();
		CCSize nameLabelSize = CCSizeMake(150 / CC_CONTENT_SCALE_FACTOR(), labelFontSize * 1.5f);
		mEffectNameLabel = CCLabelTTF::create(name, KJLinXin, labelFontSize, nameLabelSize, kCCTextAlignmentCenter);
		m_rootSprite->addChild(mEffectNameLabel, 2);
	}

	return  true;
}

void EffectSprite::setPlayerNameLabelPos(CCPoint pos)
{
	if (mEffectNameLabel)
	{
		mEffectNameLabel->setPosition(pos);
	}
}

void EffectSprite::animationHandler(const char* _aniType, const char* _aniID, const char* _frameID)
{
	BoneSpriteBase::animationHandler(_aniType,_aniID,_frameID);

	if (0 == strcmp(_aniType,"loopComplete") ||
		0 == strcmp(_aniType,"complete"))
	{
		if (false == m_isAnimLoop)
		{
			this->setVisible(false);
			//this->setInUse(false);
			this->PauseAllArmatureAnim();
		}
		//if (bStartAnim)
		//{
		//	bStartAnim = false;
		//	ParticleManager::Get()->RemoveOneParticlePlayedNum(m_type);
		//}
	}
}

void EffectSprite::SetAnim(int animId, int nloop, bool isLoop /* = true */,BoneSpriteBase *listerner /* = NULL */)
{
	//if (bStartAnim)
	//{
	//	bStartAnim = false;
	//	ParticleManager::Get()->RemoveOneParticlePlayedNum(m_type);
	//}
	if (ParticleManager::Get()->TellWhetherOneParticleCanPlay(m_type,this,bStartAnim))
	{
		BoneSpriteMoveable::SetAnim(animId,nloop,isLoop,listerner);
	}
}

void EffectSprite::SetAnim(int animId, int nloop, bool isLoop,CCAnimationEventListener *listerner)
{
	if (ParticleManager::Get()->TellWhetherOneParticleCanPlay(m_type,this,bStartAnim))
	{
		BoneSpriteMoveable::SetAnim(animId,nloop,isLoop,listerner);
	}
}