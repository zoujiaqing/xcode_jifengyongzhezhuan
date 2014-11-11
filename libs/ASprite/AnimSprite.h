//
//  AnimSprite.h
//
//  Created by arthurgong on 11-12-2.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//

#ifndef  _ANIM_SPRITE_
#define  _ANIM_SPRITE_

#include "ASprite.h"
#include "cocos2d.h"
#include "GameEnginePlayer.h"
#include "AreaItemNode.h"

using namespace std;

class AnimSprite : public AreaItemNode
{
public:
    
    enum  ANIM_TYPE{
        ANIM_TYPE_FIGHT,
        ANIM_TYPE_MOVE,
        ANIM_TYPE_EFFECT,
        ANIM_TYPE_CAPTURE_EFFECT,
        ANIM_MAIN_CH,
        ANIM_TYPE_CAPTURE,
        ANIM_TYPE_NPC,
        ANIM_TYPE_COUNT
    };
    

    AnimSprite();      
    AnimSprite(ASprite *asprite, int x, int y);
	AnimSprite(ASprite *asprite, CCSpriteBatchNode *batchNode, int x, int y);
    AnimSprite(string asFilename, int x, int y);
    AnimSprite(string asFilename, CCSpriteBatchNode *batchNode, int x, int y);
    AnimSprite(int index, int type, int x, int y);
    ~AnimSprite();  
    
    void update();
    void render();
    void SetAnim(int animId, int nloop, bool isLoop);
    void SetAnim(int animId, int nloop, bool isLoop, float delayTime);
    void pause();
    void resume();
    void SetTransform(GameEnginePlayer::AS_TRANS trans);
    int GetTransform();
    void SetPos(int x, int y);
    void SetOpacity(int opacity);
    GameEnginePlayer* getPlayer();
    void drawFrame(int index);
    void setEndActionWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector,void *data = NULL);
    void setTimeActionWithTarget(CCObject* pSelectorTarget, SEL_CallFunc selector);
    CCRect getCurFrameRect();
    CCRect getFirstFrameRect();
    CCSize getCurContentSize();
    void setTag(int tag);
    int getAnimFrameCount(){return m_animPlayer->GetNbFrame();};
    CCSpriteBatchNode* getBatchNode();
	CCSprite* getParentNode();

	CCPoint GetItemPosition();
	//float GetHideDistance();

    static string getAnimSpriteNameByType(int index, int type);
    CCRect GetABBox();
private:
    void reset();
    void startAnim();
    void initWithAsprite(ASprite *asprite, int x, int y, CCSpriteBatchNode *batchNode = NULL);
    
    GameEnginePlayer* m_animPlayer;
    CCSprite* m_parent;
    float m_delayTime;
    int m_animId;
    bool m_loop;
    int m_loopCount;    
    int m_tag;
};

#endif //_ANIM_SPRITE_