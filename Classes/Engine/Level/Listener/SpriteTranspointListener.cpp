#include "OnlineNetworkManager.h"
#include "SpriteTranspointListener.h"
#include "SpriteTransPoint.h"
#include "SpriteSeer.h"
#include "GameManager.h"
#include "MapDataManager.h"
#include "UIDefine.h"
#include "InstanceManager.h"
#include "InstanceListLayer.h"
#include "MainMenuLayer.h"
#include "XLogicManager.h"
SpriteTranspointListener* SpriteTranspointListener::sInstance = NULL;


SpriteTranspointListener::SpriteTranspointListener()
{

}

SpriteTranspointListener::~SpriteTranspointListener()
{

}


SpriteTranspointListener* SpriteTranspointListener::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new SpriteTranspointListener();
	}

	return sInstance;
}

void SpriteTranspointListener::HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* data)
{
	BaseListener::HandleLayerActiveEvent(obj,data);
}


void SpriteTranspointListener::HandleObjectTouchEvent(SpriteBase* obj, cocos2d::CCPoint pt)
{
	SpriteTransPoint* transpoint = (SpriteTransPoint*)obj;
	
	if (transpoint)
	{
		int transId = transpoint->getMapId();
		
		InstanceListLayer * layer = MainMenuLayer::getUILayerByTag<InstanceListLayer>(MainMenuLayer::getLayerTagByName("InstanceListLayer"));
		ASSERT(layer , "create instance list failed");

		//普通副本传送门
		if(200101 == transId)
		{
			if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_UNDERGROUND))
			{
				//地下城副本传送门
				layer->setInstanceType(INSTANCE_TYPE_DUNGEON);
				// OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(UNDERGROUND_INSTANCE_MIN, UNDERGROUND_INSTANCE_MAX);
			}
			else if (LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS)
				|| LevelManager::sShareInstance()->getCurMapId() == InstanceManager::Get()->getSpecialEnteranceMapId(ENTERANCE_GUILDSCENE))
			{
				SpriteSeer * seer = GameManager::Get()->getHero();
				SEL_CallFunc enterBossInstance = callfunc_selector(SpriteSeer::activePortal);
				seer->moveToTarget(transpoint->getPosition(), seer->getPosition(), seer, enterBossInstance);
				return;
			}
			else
			{
				layer->setInstanceType(INSTANCE_TYPE_NORMAL);
				// OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(NORMAL_INSTANCE_MIN, NORMAL_INSTANCE_MAX);
			}
			moveAndOpenInstanceList(transpoint, layer);

		}
		else if (200102 == transId)
		{
			layer->setInstanceType(INSTANCE_TYPE_JINGYING);//jackniu
			//jing ying fu ben range is 240000 to 249999
			// OnlineNetworkManager::sShareInstance()->sendGetInstanceListMessage(JING_YING_FU_BEN_MIN, JING_YING_FU_BEN_MAX);

            #if 0
            //check if we shold send oncedailyreq message
            std::vector<InstanceInfo> &v = InstanceManager::Get()->curInstanceList;
            int len = v.size();
            bool need_send = true;
            std::vector<unsigned int> ids;
            for(int i = 0; i < len; i++)
            {
                //if all msg are jing ying fu ben .
                //then send 
                InstanceInfo info = v[i];
                if(info.id < JING_YING_FU_BEN_MIN)
                {
                    need_send = false;
                    break;
                }
                ids.push_back(info.id-JING_YING_FU_BEN_MIN);
                            
            }
                    
			if(need_send && ids.size() > 0) {
				unsigned int *a = &ids[0];
                OnlineNetworkManager::sShareInstance()->sendOnceDailyReq(a, len);
            }
            #endif
			moveAndOpenInstanceList(transpoint, layer);
		}
		else
		{
			//副本内的传送门
			SpriteSeer * seer = GameManager::Get()->getHero();
			if(seer && MOVE_BY_MOUSE == seer->GetMoveType())
			{
				SEL_CallFunc cfTransToNext = callfunc_selector(SpriteSeer::activePortal);
				seer->moveToTarget(transpoint->getPosition(), seer->getPosition(), seer, cfTransToNext);
			}
		}
		
			
	}
}

void SpriteTranspointListener::moveAndOpenInstanceList(SpriteTransPoint * transpoint, InstanceListLayer * layer)
{
	SpriteSeer * seer = GameManager::Get()->getHero();
	if (seer && MOVE_BY_MOUSE == seer->GetMoveType())
	{
		SEL_CallFunc cfOpenInstanceList = callfunc_selector(InstanceListLayer::openInstanceList);
		seer->moveToTarget(transpoint->getPosition(), seer->getPosition(), layer, cfOpenInstanceList);
	}
	else
	{
		layer->openInstanceList();
	}
}
