//
//  ASpriteManager.h
//  iser
//
//  Created by arthurgong on 11-11-28.
//  Copyright 2011Äê taomee. All rights reserved.
//

#ifndef __ASPRITE_MANAGER_H__
#define __ASPRITE_MANAGER_H__
#include <list>
#include "ASprite.h"
#include "AnimSprite.h"
#include "GameEnginePlayer.h"
#include "SASpriteDefine.h"
#include "BoneAnimSprite.h"
#include "EffectSprite.h"

typedef  list<AnimSprite*>::iterator ASANIM_ITERATOR;
typedef list<AnimSprite*> ASANIM_LIST;

typedef  list<BoneAnimSprite*>::iterator BONE_ANIM_ITERATOR;
typedef list<BoneAnimSprite*> BONE_ANIM_LIST;

using namespace std;

class EnginePlayerManager
{    
private:
    ASANIM_LIST m_aspriteList;
    ASANIM_ITERATOR m_iterator;

	BONE_ANIM_LIST m_boneSrpiteList;
	BONE_ANIM_ITERATOR m_boneIterator;
	list<EffectSprite*> m_effectSpriteList;
	list<EffectSprite*>::iterator m_effectIterator;
public:
    EnginePlayerManager();
    virtual ~EnginePlayerManager();
    
    static EnginePlayerManager* getInstance(void);
	static void purgeInstance();
    
    void update(float dt);
    void addAsprite(AnimSprite * asprite);
    void removeAsprite(AnimSprite *&asprite, bool clean = true);
    void stopAsprite(AnimSprite *asprite);
	void addBoneAsprite(BoneAnimSprite * asprite);
	void removeBoneAsprite(BoneAnimSprite *&asprite, bool clean = true);
	void stopBoneAsprite(BoneAnimSprite *asprite);
	void addEffectAsprite(EffectSprite * asprite);
	void removeEffectAsprite(EffectSprite *&asprite, bool clean = true);
	void stopEffectAsprite(EffectSprite *asprite);
    void stopAll();
    void removeAll();
    
    static AnimSprite* addAnimSprite(ASprite *asprite, int x, int y);
	static AnimSprite* addAnimSprite(ASprite *asprite, CCSpriteBatchNode* pBatchNode, int x, int y);
    static AnimSprite* addAnimSprite(string asFilename, int x, int y);
    static AnimSprite* addAnimSprite(string asFilename, CCSpriteBatchNode *batchNode, int x, int y);
    static AnimSprite* addAnimSprite(int index, int type, int x, int y);

	static BoneAnimSprite* addBoneAnimSprite(int type);
	static BoneAnimSprite* addBoneAnimSprite(int type,int x,int y);
};
#endif