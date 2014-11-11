#include "SpriteNpcPointListener.h"
#include "NpcSprite.h"
#include "SpriteSeer.h"
#include "GameManager.h"
#include "PhysicsLayer.h"

SpriteNPCListener* SpriteNPCListener::sInstance = NULL;


SpriteNPCListener::SpriteNPCListener()
{

}

SpriteNPCListener::~SpriteNPCListener()
{

}


SpriteNPCListener* SpriteNPCListener::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new SpriteNPCListener();
	}

	return sInstance;
}

void SpriteNPCListener::HandleObjectTouchEvent(SpriteBase* obj, cocos2d::CCPoint pt,CCRect colliderRect /* = CCRectZero */)
{
	SpriteNPC* npcSprite = (SpriteNPC*)obj;
	
	SpriteSeer * seer = GameManager::Get()->getHero();
	if (seer && MOVE_BY_MOUSE == seer->GetMoveType())
	{
		SEL_CallFunc cfMoveToTarget = callfunc_selector(SpriteNPC::OneActorMoveEndEvent);
		seer->SetAdditionMoveRate(0.0f);
		seer->moveToTarget(pt, seer->getPosition(), npcSprite, cfMoveToTarget,252.0f - seer->GetMoveRate(),colliderRect);
	}
}


void SpriteNPCListener::HandleLockedEvent(SpriteBase* obj)
{
}


void SpriteNPCListener::HandleTransformEvent(SpriteBase* obj)
{
}
