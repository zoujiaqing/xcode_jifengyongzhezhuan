#include "ArmatureSpritePool.h"

ArmatureSpritePool::ArmatureSpritePool()
{
	mSpriteDic = new CCDictionary();
}

ArmatureSpritePool::~ArmatureSpritePool()
{
	mSpriteDic->removeAllObjects();
	CC_SAFE_DELETE(mSpriteDic);
}

ArmatureSpritePool *ArmatureSpritePool::create() 
{
	ArmatureSpritePool *pInstance = new ArmatureSpritePool();
	if (pInstance && pInstance->init()) 
	{
		pInstance->autorelease();
		return pInstance;
	}
	CC_SAFE_DELETE(pInstance);
	return 0;
}

bool ArmatureSpritePool::init()
{
	return true;
}

CCSprite* ArmatureSpritePool::GetOneSprite(std::string _boneName,std::string _imgName)
{
	CCDictionary* pDic = (CCDictionary*)mSpriteDic->objectForKey(_imgName.c_str());
	if (pDic == 0)
	{
		pDic = new CCDictionary();
		mSpriteDic->setObject(pDic,_imgName);

		CCSprite *_newSprite = CCSprite::createWithSpriteFrameName(_imgName.c_str());
		pDic->setObject(_newSprite,_boneName.c_str());

		return _newSprite;
	}
	else
	{
		CCSprite* pSprite = (CCSprite*) pDic->objectForKey(_boneName);
		if (pSprite == 0)
		{
			CCSprite *_newSprite = CCSprite::createWithSpriteFrameName(_imgName.c_str());
			pDic->setObject(_newSprite,_boneName.c_str());

			return _newSprite;
		}
		else
		{
			return pSprite;
		}
	}

	return 0;
}

bool ArmatureSpritePool::CreateOneSprite(std::string _boneName,std::string _imgName)
{
	return true;
}