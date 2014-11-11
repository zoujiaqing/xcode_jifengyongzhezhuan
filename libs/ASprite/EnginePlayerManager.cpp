//
//  ASpriteManager.cpp
//  iseer
//
//  Created by arthurgong on 11-11-28.
//  Copyright 2011Äê taomme . All rights reserved.
//

#include "EnginePlayerManager.h"
#include "UIDefine.h"
#include "cocos2d.h"
#include "SceneLayer.h"
#include "GameManager.h"
#include "SkeletonAnimRcsManager.h"
#include "ItemUpdateManager.h"

#include <algorithm>
using namespace std;


static EnginePlayerManager* _asManager = NULL;

EnginePlayerManager* EnginePlayerManager::getInstance(void)
{
    if(_asManager == NULL)
    {
        _asManager = new EnginePlayerManager();
    }
    return _asManager;
}

void EnginePlayerManager::purgeInstance()
{
	if (_asManager)
	{
		delete _asManager;
		_asManager = NULL;
	}
}

EnginePlayerManager::EnginePlayerManager()
{

}

EnginePlayerManager::~EnginePlayerManager()
{
	removeAll();
}

void EnginePlayerManager::removeAll()
{
    for(ASANIM_ITERATOR iter = m_aspriteList.begin(); iter != m_aspriteList.end(); iter++)
    {
        SAFE_DEL(*iter);		
    }
    m_aspriteList.clear();

	for(BONE_ANIM_ITERATOR iter = m_boneSrpiteList.begin(); iter != m_boneSrpiteList.end(); iter++)
	{
		SAFE_DEL(*iter);
	}
	m_boneSrpiteList.clear();

	/*for(list<EffectSprite*>::iterator iter = m_effectSpriteList.begin(); iter != m_effectSpriteList.end(); iter++)
	{
	SAFE_DEL(*iter);
	}*/
	m_effectSpriteList.clear();
}

void EnginePlayerManager::update(float dt)
{
    for(ASANIM_ITERATOR iter = m_aspriteList.begin(); iter != m_aspriteList.end(); iter++)
    {
        if (*iter && (*iter)->GetUpdateEnabled()) 
        {
             (*iter)->update();
        }       
    }
    m_aspriteList.remove(NULL);
    for(ASANIM_ITERATOR iter = m_aspriteList.begin(); iter != m_aspriteList.end(); iter++)
    {
		if ((*iter)->GetUpdateEnabled())
		{
			(*iter)->render();
		}        
    }
}

void EnginePlayerManager::addAsprite(AnimSprite *asprite)
{
    if(asprite)
     {
		 if (find(m_aspriteList.begin(), m_aspriteList.end(), asprite) == m_aspriteList.end())
		 {
			 m_aspriteList.push_back(asprite);   
			 ItemUpdateManager::Get()->addToUpdateManager(asprite);
		 }
	}
}

void EnginePlayerManager::removeAsprite(AnimSprite * &asprite, bool clean)
{
    if(NULL == asprite)
        return;

    ASANIM_ITERATOR iter = m_aspriteList.begin();
    for(; iter != m_aspriteList.end(); iter++)
    {
        if((*iter) == asprite)
        {
            *iter = NULL;
			m_aspriteList.erase(iter);
			ItemUpdateManager::Get()->removeFromUpdateManager(asprite);
			break;
        }
    }
    
    if (clean) 
    {
        SAFE_DEL(asprite);
    }    
}

void EnginePlayerManager::stopAsprite(AnimSprite *asprite)
{
    for(ASANIM_ITERATOR iter = m_aspriteList.begin(); iter != m_aspriteList.end(); iter++)
    {
        if((*iter) == asprite)
        {
            (*iter)->getPlayer()->animIsOver = true;
			break;
        }
    }
}

void EnginePlayerManager::addBoneAsprite(BoneAnimSprite *asprite)
{
	if(asprite)
	{
		if (find(m_boneSrpiteList.begin(), m_boneSrpiteList.end(), asprite) == m_boneSrpiteList.end())
		{
			m_boneSrpiteList.push_back(asprite);
		}
	}
}

void EnginePlayerManager::removeBoneAsprite(BoneAnimSprite * &asprite, bool clean)
{
	if(NULL == asprite)
		return;

	BONE_ANIM_ITERATOR iter = m_boneSrpiteList.begin();
	for(; iter != m_boneSrpiteList.end(); iter++)
	{
		if((*iter) == asprite)
		{
			*iter = NULL;
			m_boneSrpiteList.erase(iter);
			break;
		}
	}

	if (clean) 
	{
		SAFE_DEL(asprite);
	}    
}

void EnginePlayerManager::stopBoneAsprite(BoneAnimSprite *asprite)
{
	for(BONE_ANIM_ITERATOR iter = m_boneSrpiteList.begin(); iter != m_boneSrpiteList.end(); iter++)
	{
		if((*iter) == asprite)
		{
			//(*iter)->getPlayer()->animIsOver = true;
			(*iter)->pause();
			break;
		}
	}
}

void EnginePlayerManager::stopAll()
{
    for(ASANIM_ITERATOR iter = m_aspriteList.begin(); iter != m_aspriteList.end(); iter++)
    {
        (*iter)->getPlayer()->animIsOver = true;
    }


	for(BONE_ANIM_ITERATOR iter = m_boneSrpiteList.begin(); iter != m_boneSrpiteList.end(); iter++)
	{
		(*iter)->pause();
	}

	//for(list<EffectSprite*>::iterator iter = m_effectSpriteList.begin(); iter != m_effectSpriteList.end(); iter++)
	//{
	//	(*iter)->pause();
	//}
}

AnimSprite* EnginePlayerManager::addAnimSprite(ASprite *asprite, int x, int y)
{
    AnimSprite* pAnimSprite = new AnimSprite(asprite, x, y);
    EnginePlayerManager::getInstance()->addAsprite(pAnimSprite);
    return pAnimSprite;
}


AnimSprite* EnginePlayerManager::addAnimSprite( ASprite *asprite, CCSpriteBatchNode* pBatchNode, int x, int y )
{
	AnimSprite* pAnimSprite = new AnimSprite(asprite, pBatchNode, x, y);
	EnginePlayerManager::getInstance()->addAsprite(pAnimSprite);
	return pAnimSprite;
}



AnimSprite* EnginePlayerManager::addAnimSprite(string asFilename, int x, int y)
{
    AnimSprite* pAnimSprite = new AnimSprite(asFilename, x, y);
    EnginePlayerManager::getInstance()->addAsprite(pAnimSprite);
    return pAnimSprite;
}

AnimSprite* EnginePlayerManager::addAnimSprite(string asFilename, CCSpriteBatchNode *batchNode, int x, int y)
{
    AnimSprite* pAnimSprite = new AnimSprite(asFilename, batchNode, x, y);
    EnginePlayerManager::getInstance()->addAsprite(pAnimSprite);
    return pAnimSprite;
}

AnimSprite* EnginePlayerManager::addAnimSprite(int index, int type, int x, int y)
{
    AnimSprite* pAnimSprite = new AnimSprite(index, type, x, y);
    EnginePlayerManager::getInstance()->addAsprite(pAnimSprite);
    return pAnimSprite;
}

BoneAnimSprite* EnginePlayerManager::addBoneAnimSprite(int type)
{
	if (false == SkeletonAnimRcsManager::getInstance()->IsOneActorRcsExist(type))
	{
		return 0;
	}
	
	BoneAnimSprite *pBoneAnimSprite = new BoneAnimSprite(type);
	EnginePlayerManager::getInstance()->addBoneAsprite(pBoneAnimSprite); 
	return pBoneAnimSprite;
}

BoneAnimSprite* EnginePlayerManager::addBoneAnimSprite(int type,int x,int y)
{
	BoneAnimSprite *pBoneAnimSprite = new BoneAnimSprite(type);
	EnginePlayerManager::getInstance()->addBoneAsprite(pBoneAnimSprite);
	return pBoneAnimSprite;
}

void EnginePlayerManager::addEffectAsprite(EffectSprite * asprite)
{
	if(asprite)
	{
		if (find(m_effectSpriteList.begin(), m_effectSpriteList.end(), asprite) == m_effectSpriteList.end())
		{
			m_effectSpriteList.push_back(asprite);
		}
	}
}
void EnginePlayerManager::removeEffectAsprite(EffectSprite *&asprite, bool clean)
{
	if(NULL == asprite)
		return;

	list<EffectSprite*>::iterator iter = m_effectSpriteList.begin();
	for(; iter != m_effectSpriteList.end(); iter++)
	{
		if((*iter) == asprite)
		{
			*iter = NULL;
			m_effectSpriteList.erase(iter);
			 
			/*if (clean) 
			{
			asprite->removeFromParentAndCleanup(true);
			} */
			break;
		}
	}  
	
}
void EnginePlayerManager:: stopEffectAsprite(EffectSprite *asprite)
{
	//for(list<EffectSprite*>::iterator iter = m_effectSpriteList.begin(); iter != m_effectSpriteList.end(); iter++)
	//{
	//	if((*iter) == asprite)
	//	{
	//		//(*iter)->getPlayer()->animIsOver = true;
	//		(*iter)->pause();
	//		break;
	//	}
	//}
}