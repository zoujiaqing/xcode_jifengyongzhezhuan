//
//  AnimSprite.cpp
//
//  Created by arthurgong on 11-12-2.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//

#include "AnimSprite.h"
#include "AspriteManager.h"
#include "UIDefine.h"

AnimSprite::AnimSprite()
{
    reset();
}

AnimSprite::~AnimSprite()
{
    SAFE_DEL(m_animPlayer)
}

AnimSprite::AnimSprite(ASprite *asprite, int x, int y)
{
    reset();
    
    initWithAsprite(asprite, x, y);   
}

AnimSprite::AnimSprite(ASprite *asprite, CCSpriteBatchNode *batchNode, int x, int y)
{
	reset();

	initWithAsprite(asprite, x, y, batchNode);   
}

AnimSprite::AnimSprite(string asFilename, int x, int y)
{
    reset();  

	AspriteManager* asmanager = AspriteManager::getInstance();    
	if(asmanager)
	{
		initWithAsprite(asmanager->getAsprite(asFilename), x, y);
	}
}

AnimSprite::AnimSprite(int index, int type, int x, int y)
{
    reset();
    
    AspriteManager* asmanager = AspriteManager::getInstance();    
    string key = "Actor/" + getAnimSpriteNameByType(index, type);
    if(asmanager)
        initWithAsprite(asmanager->getAsprite(key), x, y);
}

AnimSprite::AnimSprite(string asFilename, CCSpriteBatchNode *batchNode, int x, int y)
{
    reset();
    
    AspriteManager* asmanager = AspriteManager::getInstance();    
    if(asmanager)
        initWithAsprite(asmanager->getAsprite(asFilename), x, y, batchNode);
}

void AnimSprite::initWithAsprite(ASprite *asprite, int x, int y, CCSpriteBatchNode *asBatchNode)
{
    if(asprite)
    {
        CCSpriteBatchNode* batchNode = asBatchNode ? asBatchNode: CCSpriteBatchNode::create(asprite->getImageName().c_str());
        if(batchNode)
        {
            //m_parent = CCSprite::spriteWithBatchNode(batchNode, CCRect(0,0,0,0));
			m_parent = CCSprite::createWithTexture(batchNode->getTexture(), CCRect(0,0,0,0));
            m_tag = -1;
            
            float deviceScale = CC_CONTENT_SCALE_FACTOR();
            
            x *= deviceScale;
            y *= deviceScale;
            
            m_animPlayer = gll_new GameEnginePlayer(asprite, batchNode, m_parent, x, y);
            batchNode->addChild(m_parent);
        }
        else 
        {
            CCLOG("batchNodeWithFile failed!");
        }
    }
    else
    {
        CCLOG("asprite invalid!");
    }
}

string AnimSprite::getAnimSpriteNameByType(int index, int type)
{
    ASSERT(type >= 0 && type < ANIM_TYPE_COUNT, "invalid anim sprite type");
    
    char key[NAME_MAX];
    if(type == ANIM_MAIN_CH)
    {
        sprintf(key, "seer_%d.bin", index);
    }
    else if(type == ANIM_TYPE_FIGHT)
    {
        sprintf(key, "c_%d.bin", index);
    }
    else if(type == ANIM_TYPE_MOVE)
    {
        sprintf(key, "sq_%d.bin", index);
    }
    else if(type == ANIM_TYPE_EFFECT)
    {
        sprintf(key, "effect_%d.bin", index);
    }
    else if(type == ANIM_TYPE_CAPTURE_EFFECT)
    {
        sprintf(key, "capture_%d.bin", index);
    }
    else if(type == ANIM_TYPE_CAPTURE)
    {
        sprintf(key, "bz_%d.bin", index);
    }
    else if(type == ANIM_TYPE_NPC)
    {
        sprintf(key, "npc_%d.bin", index);
    }
    
    return key;
}

CCSpriteBatchNode* AnimSprite::getBatchNode()
{
    if (m_animPlayer) 
    {
        return m_animPlayer->getBatchNode();
    }
    else 
    {
        return NULL;
    }
}

CCSprite* AnimSprite::getParentNode()
{
	if (m_animPlayer) 
	{
		return m_animPlayer->getParentNode();
	}
	else 
	{
		return NULL;
	}
}

void AnimSprite::setEndActionWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector,void *data)
{
    if(m_animPlayer)
        m_animPlayer->setActionWithTarget(pSelectorTarget, selector,data);
}

void AnimSprite::setTimeActionWithTarget(CCObject* pSelectorTarget, SEL_CallFunc selector)
{
    if(m_animPlayer)
        m_animPlayer->setTimeWithTarget(pSelectorTarget, selector);
}

void AnimSprite::startAnim()
{
    SetAnim(m_animId, m_loopCount, m_loop);
}

void AnimSprite::SetAnim(int animId, int nloop, bool isLoop)
{
	if (m_parent)
		m_parent->stopAllActions();
    
	if (m_animPlayer)
		m_animPlayer->SetAnim(animId, nloop, isLoop);
}

void AnimSprite::SetAnim(int animId, int nloop, bool isLoop, float delayTime)
{
    m_loop = isLoop;
    m_loopCount = nloop;
    m_delayTime = delayTime;
    m_animId = animId;
    
    if( delayTime >= -0.000001 && delayTime <= 0.000001)
    {
        SetAnim(animId, nloop, isLoop);
    }
    else if(delayTime > 0.000001)
    {        
        CCDelayTime* delay = CCDelayTime::create(delayTime);
        CCCallFunc* callback = CCCallFunc::create(m_parent, callfunc_selector(AnimSprite::startAnim));
        CCAction* action = CCSequence::create(delay,callback, NULL);        
        m_parent->runAction(action);
    }
    else
    {
        SetAnim(m_animId, m_loopCount, m_loop);
    }
}

void AnimSprite::pause()
{
    m_animPlayer->pause();
}

void AnimSprite::resume()
{
    m_animPlayer->resume();
}

void AnimSprite::SetTransform(GameEnginePlayer::AS_TRANS trans)
{
    m_animPlayer->SetTransform(trans);
}

int AnimSprite::GetTransform()
{
    return m_animPlayer->GetTransform();
}

void AnimSprite::SetPos(int x, int y)
{
	/*if(IsRetinaDisplay())
	{
	x*=2;
	y*=2;
	}*/
    m_animPlayer->SetPos(x, y);
}

void AnimSprite::SetOpacity(int opacity)
{
    if(m_parent)
    {
        m_parent->setOpacity(opacity);
    }
}

GameEnginePlayer* AnimSprite::getPlayer()
{
    return m_animPlayer;
}

void AnimSprite::update()
{
    if(m_animPlayer)
    {
        m_animPlayer->Update();
    }
}

void AnimSprite::render()
{    
    if(m_animPlayer)
    {
        m_animPlayer->Render();
    }
}

void AnimSprite::drawFrame(int index)
{
	if (m_animPlayer)
		m_animPlayer->drawFrame(index);
}

CCRect AnimSprite::getCurFrameRect()
{
	if (m_animPlayer)
    {
		return m_animPlayer->getCurFrameRect();       
	}else
	{
		return CCRectZero;
	}
}

CCRect AnimSprite::getFirstFrameRect()
{
	if (m_animPlayer)
    {
		return m_animPlayer->getFirstFrameRect();
	}else
	{
		return CCRectZero;
	}
}

CCSize AnimSprite::getCurContentSize()
{
    return  m_parent->getContentSize();
}

void AnimSprite::setTag(int tag)
{
}

void AnimSprite::reset()
{
    m_animPlayer = NULL;
    m_parent = NULL;
}

CCPoint AnimSprite::GetItemPosition()
{
	return m_parent->getPosition();
}

//float AnimSprite::GetHideDistance()
//{
//	return 400;
//}

CCRect AnimSprite::GetABBox()
{
	cocos2d::CCRect rect = getCurFrameRect();
	cocos2d::CCPoint pos = GetItemPosition();
	return CCRectMake(rect.origin.x + pos.x, rect.origin.y + pos.y, rect.size.width, rect.size.height);	
}