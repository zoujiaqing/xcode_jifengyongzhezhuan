#include "RoleActorBase.h"
#include "GameResourceManager.h"

#include "ATLObjectLayer.h"

extern std::string stringAppendMethod(std::string str,const char *suffix);

CCSpriteBatchNode * RoleActorBase::createBatchNode(std::string png_file_name)
{
	CCSpriteBatchNode * batchNode = AlphaTestBatchNode::create( png_file_name.c_str() );
	batchNode->m_name = png_file_name.c_str();
	return batchNode;
}

RoleActorBase *RoleActorBase::create(std::string _name, std::string _animationName, 									
									 std::string png_file_name, int _index /* = 0 */)
{
	RoleActorBase *pArmature = new RoleActorBase();
	CCSpriteBatchNode * _batchNode = RoleActorBase::createBatchNode(png_file_name);
	if (pArmature && pArmature->init(_name, _animationName,_batchNode, _index))
	{
		pArmature->autorelease();
		return pArmature;
	}
	CC_SAFE_DELETE(pArmature);
	return NULL;
}


RoleActorBase::RoleActorBase()
{
	mBatchNode = 0;
	mParentSpriteBase = 0;
	m_pAnimationEventListerner = NULL;
}

RoleActorBase::~RoleActorBase(void)
{
}

bool RoleActorBase::setParentRole(BoneSpriteBase *parent)
{
	mParentSpriteBase = parent;
	if (mParentSpriteBase == 0)
	{
		return false;
	}
	return true;
}

bool RoleActorBase::setParentAnimListerner(CCAnimationEventListener* pListerner)
{
	m_pAnimationEventListerner = pListerner;
	if (m_pAnimationEventListerner == NULL)
	{
		return false;
	}
	return true;
}

bool RoleActorBase::init(std::string _name, std::string _animationName,
						 CCSpriteBatchNode *_batchNodeL, int _index)
{
	mBatchNode = _batchNodeL;
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCArmature::init(_name.c_str(),_animationName.c_str(),_batchNodeL,_index));		
		mAnimation->setAniEventListener(this);
		bRet = true;
	} while (0);

	return bRet;
}

void RoleActorBase::animationHandler(const char* aniType, const char* aniID, const char* frameID)
{
	//CCLOG("%s, %s, %s", aniType,aniID,frameID);
	if(mParentSpriteBase != NULL)
	{
		mParentSpriteBase->animationHandler(aniType,aniID,frameID);
	}
	if (m_pAnimationEventListerner != NULL)
	{
		m_pAnimationEventListerner->animationHandler(aniType,aniID,frameID);
	}
}