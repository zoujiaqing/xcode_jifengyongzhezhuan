#include "OnlineNetworkManager.h"
#include "StoryInstanceDirector.h"
#include "HSJoystick.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "BattleUILayer.h"
#include "StoryDataCenter.h"
#include "LevelManager.h"
#include "InstanceDialogLayer.h"
#include "UserData.h"
#include "LevelLayer.h"
#include "SkeletonAnimRcsManager.h"
#include "CameraController.h"
#include "InstanceManager.h"
#include "XLogicManager.h"
#include "SpriteElfConfigFromLua.h"
#include "GameUILayer.h"
#include "BoneNPCManager.h"
#include "SpriteElfManager.h"
#include "GameDataManager.h"
#include "NetWorkConfigure.h"
#include "ParticleManager.h"
#include "LuaTinkerManager.h"
#include "GameAudioManager.h"
#include "CommonEnum.h"
#include "ATLObjectLayer.h"
#include "MainLandManager.h"
#include "LevelManager.h"

#define AnimationInternal  (1.0f / FRAME_RATES)

static StoryInstanceDirector* _storyInstanceDirector = NULL;

StoryInstanceDirector::StoryInstanceDirector():
	cloneHero(NULL)
{	
	ResetValue();
	m_dialogLayer = NULL;
	m_isHeroAutoFight = false;
	m_waitForBeginTotalTime = 2.0f;
	bSendData = true;
}

void StoryInstanceDirector::ResetAllData()
{
	ResetValue();
	m_dialogLayer = NULL;
}

void StoryInstanceDirector::ResetValue()
{
	m_isStart = false;
	m_isPause = false;	
	m_isTaskStory = false;
	m_isWaitForBegin = false;
	m_isHSJoystickEnable = false;
	m_IsFirstComeInInstance = false;
	
	mID = 0;
	mMapId = 0;
	m_curKeyID = 0;
	m_totalFrames = 0;
	m_cutAnimHappendWhen = 0;
	m_waitFromBeginLastTime = 0;
	
	m_curFrame = -1;
	m_preFrame = -1;	
	m_heroIndex = -1;

	//m_eStoryType = kType_Story_UnKnow;
		
	m_pColorLayer = NULL;
	cloneHero = NULL;

	m_mapRoleIdAndInstance.clear();
}

StoryInstanceDirector* StoryInstanceDirector::Get()
{
	if (_storyInstanceDirector == 0)
	{
		_storyInstanceDirector = new StoryInstanceDirector();
	}
	return _storyInstanceDirector;
}

void StoryInstanceDirector::Destroy()
{
	if (_storyInstanceDirector)
	{
		delete _storyInstanceDirector;
		_storyInstanceDirector = 0;
	}
}

void StoryInstanceDirector::Resume()
{
	if (m_isStart)
	{
		m_isPause = false;
	}
}

/**
* Instruction : 剧情数据初始化
* @param 
*/
bool StoryInstanceDirector::InitOnBegin()
{
	int keyID = 0;
	bool bResult = false;
	
	if (false == m_isTaskStory)
	{
		bResult = StoryDataCenter::Get()->LoadOneDataItem(mID,mMapId,m_cutAnimHappendWhen,keyID);	
	}	
	else
	{
		bResult = StoryDataCenter::Get()->LoadOneDataItem(mID,m_cutAnimHappendWhen,keyID);
	}

	if (false == bResult)
	{
		return false;
	}

	if (kType_MainLand_PreTask == m_eStoryType || 
		kType_MainLand_AcceptTask == m_eStoryType || 
		kType_MainLand_RewardTask == m_eStoryType || 
		kType_MainLand_OverTask == m_eStoryType
		)
	{
		if (IsOneItemUsed(keyID))
		{
			//End();
			ResetValue();
			return true;
		}
		else
		{
			PushUsedItemId(keyID);
			OnlineNetworkManager::sShareInstance()->sendOnceSetReqMessage(keyID+kType_MainLandStory_Key);
		}
	}

	// Note: 获取总帧数
	m_totalFrames = StoryDataCenter::Get()->GetTotalFrames();	

	SetCameraFollowType(CameraController::E_FixPoint);

	HideFBThings();
	HideMainLandThings();
	CreateChildLayers();

	// Note: 加载动画需要资源
	LoadAllNeedRcs();
	LoadAllRoles();

	// Note: 禁用主角的状态机
	SpriteSeer* pHero = GameManager::Get()->getHero();
	if (pHero)
	{
		pHero->PauseStateMachine();
		pHero->getRoot()->setVisible(false);
		pHero->setShadowVisible(false);
	}
	// Note: 禁止主角的脚步声
    int roleType = GameManager::Get()->getHero()->getType();
    if(roleType == kTypeAssassin_Boy || roleType == kTypeAssassin_Girl)
    {
        GameAudioManager::sharedManager()->stopEffect(SOUND_EFFECT_WALK_ASSASSIN);
    }
    else
    {
        GameAudioManager::sharedManager()->stopEffect(SOUND_EFFECT_WALK);
    }	

	if (m_eStoryType == kType_FB_EndMap)
	{
		PlayBecomeDarkEffect();
	}
	else if (m_eStoryType == kType_FB_BeginMap)
	{
		m_isHeroAutoFight = GameDataManager::Get()->getHeroAutoAttack();
	}

	m_isStart = true;
	//m_isPause = false;
	return bResult;
}

void StoryInstanceDirector::Begin(unsigned int id,unsigned int map_id,int nWhen,bool bTaskStory /* = false */)
{
	bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
	if(bMainLandLevel != bTaskStory)
	{
		return ;	
	}
	
	if (m_isStart || m_isWaitForBegin)
	{
		return ;
	}

	ResetValue();

	m_curKeyID = mID = id;
	mMapId = map_id;
	m_isTaskStory = bTaskStory;
	m_cutAnimHappendWhen = nWhen;

	CheckStoryType();

	if (m_eStoryType == kType_FB_EndMap)
	{
		// Note: 等待2.0秒时间开始剧情，用于副本打完后资源释放相关
		m_isWaitForBegin = true;
		m_waitFromBeginLastTime = 0;
	}
	else
	{
		InitOnBegin();
	}
}

void StoryInstanceDirector::ShowOtherPlayers(bool bShow)
{
	std::map<PLAYER_ID, OthersPlayerInfo>& otherPlayer = LevelManager::sShareInstance()->getCurrentLevelLayer()->m_othersPlayerInfo;
	for(std::map<PLAYER_ID, OthersPlayerInfo>::const_iterator iter = otherPlayer.begin(); iter != otherPlayer.end(); iter++)
	{
		BoneSpriteBase* pOtherHero = dynamic_cast<BoneSpriteBase*>(iter->second.seer); 
		pOtherHero->setVisible(bShow);
		pOtherHero->setShadowVisible(bShow);
	}
}

void StoryInstanceDirector::ShowOtherElf(bool bShow)
{
	const std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >& elfMap = SpriteElfManager::Get()->GetAllInstance();
	for(std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::const_iterator iter = elfMap.begin(); iter != elfMap.end(); iter++)
	{
		PLAYER_ID id = (*iter).first;
		if (id != UserData::Get()->getUserId())
		{
			for (std::map<unsigned int,SpriteElf*>::const_iterator elf_iter = (*iter).second.begin();
				elf_iter != (*iter).second.end(); elf_iter++)
			{
				(*elf_iter).second->setVisible(bShow);
			}			
		}
	}
}

void StoryInstanceDirector::ShowHeroElf(bool bShow)
{
	const std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >& elfMap = SpriteElfManager::Get()->GetAllInstance();
	for(std::map<PLAYER_ID, std::map<unsigned int,SpriteElf*> >::const_iterator iter = elfMap.begin(); iter != elfMap.end(); iter++)
	{
		PLAYER_ID id = (*iter).first;
		if (id == UserData::Get()->getUserId())
		{
			for (std::map<unsigned int,SpriteElf*>::const_iterator elf_iter = (*iter).second.begin();
				elf_iter != (*iter).second.end(); elf_iter++)
			{
				(*elf_iter).second->setVisible(bShow);
			}			
		}
	}
}

void StoryInstanceDirector::Update(float dt)
{
	if (m_isWaitForBegin)
	{
		m_waitFromBeginLastTime += dt;
		if (m_waitFromBeginLastTime > m_waitForBeginTotalTime)
		{
			m_isWaitForBegin = false;
			m_waitFromBeginLastTime = 0;
			InitOnBegin();
		}
		return ;
	}

	if (m_isStart)
	{
		if (m_isTaskStory)
		{
			ShowOtherPlayers(false);
			ShowOtherElf(false);
			ShowHeroElf(false);
		}
		else
		{
			ShowHeroElf(false);
		}
	}

	if (false == m_isStart || true == m_isPause)
	{
		return ;
	}

	float deltaFrames = (dt/AnimationInternal);
	//if(0 == m_curFrame)
	//{
	//	bool bPause = false;
	//	TrackDialogAnim(0);
	//	TrackDisplayPicAnim(0);
	//	TrackPlayerAnim(0);

	//	m_curFrame += deltaFrames;
	//}
	//else
	{
		m_curFrame += deltaFrames;

		// Note: 处理跳帧现象
		if (fabs(m_curFrame - m_preFrame) >= 1)
		{
			int tmpFrame = m_preFrame+1;
			while(tmpFrame <= m_curFrame)
			{
				bool bPause = false;
				if (TrackDialogAnim(tmpFrame))
				{
					bPause = true;
				}
				else
				{
					if (TrackDisplayPicAnim(tmpFrame))
					{
						bPause = true;
					}
				}
				if (bPause)
				{
					m_curFrame = tmpFrame;
					m_preFrame = tmpFrame;
					TrackPlayerAnim(tmpFrame);
					return;
				}
				else
				{
					TrackPlayerAnim(tmpFrame);
				}

				tmpFrame += 1;
			}	

			m_preFrame = (unsigned int)m_curFrame;

			if (m_preFrame >= m_totalFrames)
			{
				End();
				return ;
			}
		}
		else
		{
			return ;
		}
	}
}

void StoryInstanceDirector::End()
{
	if (m_eStoryType == kType_FB_BeginMap)
	{
		PlayBecomeDarkEffect();
	}

	// Note: 清空资源
	RemoveAndClearRcs();

	ShowFBThings();
	ShowMainLandThings();

	ClearChildLayers();

	CameraController::sharedCameraController()->SetFollowType(CameraController::E_FollowHero);

	if (m_eStoryType == kType_FB_BeginMap)
	{
		// Note: 重置主角的位置
		CCPoint playerPos = InstanceManager::Get()->getPlayerPosition();
		SpriteSeer* pHero = GameManager::Get()->getHero();
		if (pHero)
		{
			pHero->setPosition(playerPos);
		}

		LevelManager::sShareInstance()->ShowMonsterOrNot(true);
		OnlineNetworkManager::sShareInstance()->sendLoadCompleteMessage();
	}
	else if (m_eStoryType == kType_FB_EndMap)
	{
		XLogicManager::sharedManager()->FinishBattle();
	}

	SpriteSeer* pHero = GameManager::Get()->getHero();
	if (pHero)
	{
		if (m_eStoryType != kType_FB_EndMap)
		{
			pHero->ResumeStateMachine();
			pHero->getRoot()->setVisible(true);
			pHero->setShadowVisible(true);
			if (cloneHero)
			{
				cloneHero->getRoot()->setVisible(false);
				cloneHero->setShadowVisible(false);
			}
		}		
		else
		{
			if (cloneHero)
			{
				CCPoint pt = cloneHero->getPosition();
				pHero->setPosition(pt);
				cloneHero->getRoot()->setVisible(true);
				cloneHero->setShadowVisible(true);
			}
		}
	}

	ResetValue();
}

void StoryInstanceDirector::PlayBecomeDarkEffect()
{
	// Note: 播放黑屏特效
	if (m_pColorLayer != NULL)
	{
		m_pColorLayer->setVisible(true);

		float duration = 1.1f;
		CCFadeOut *fadeout = CCFadeOut::create(duration);
		m_pColorLayer->runAction(fadeout);

		SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
		if (pSceneLayer)
		{
			MainMenuLayer * pLayer = pSceneLayer->getMainMenuLayer();
			if (pLayer)
			{
				CCCallFunc *rc = CCCallFunc::create(pLayer,callfunc_selector(MainMenuLayer::DarkEffectOver));
				CCSequence *seq = (CCSequence*)CCSequence::create(CCDelayTime::create(duration),rc,NULL);		
				if (pLayer)
				{
					pLayer->runAction(seq);
				}
			}
		}

		m_isPause = true;
	}
}

void StoryInstanceDirector::DarkEffectOver()
{
	if (m_pColorLayer)
	{
		m_pColorLayer->removeFromParentAndCleanup(true);
		m_pColorLayer = 0;
	}

	m_isPause = false;

	if (m_eStoryType == kType_FB_BeginMap)
	{
		if (m_isHeroAutoFight)
		{
			GameDataManager::Get()->setHeroAutoAttack(m_isHeroAutoFight);
		}
	}
}

void StoryInstanceDirector::ShowBattleUILayer(bool bShow)
{
	SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
	if (pSceneLayer)
	{
		MainMenuLayer * mLayer = pSceneLayer->getMainMenuLayer();
		if (mLayer != NULL)
		{
			int battleTag = mLayer->getLayerTagByName("BattleUILayer");
			BattleUILayer* battleLayer = mLayer->findUILayerByTag<BattleUILayer>(battleTag);
			if (battleLayer)
			{
				battleLayer->setVisible(bShow);
			}			
		}	
	}
}

void StoryInstanceDirector::ShowMainLandUILayer(bool bShow)
{
	SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
	if (pSceneLayer)
	{
		MainMenuLayer * pLayer = pSceneLayer->getMainMenuLayer();
		if (pLayer != NULL)
		{
			GameUILayer* pGameUILayer = pLayer->GetGameUILayer();
			if (pGameUILayer)
			{
				pGameUILayer->setVisible(bShow);
			}			
		}	
	}
}

void StoryInstanceDirector::LoadAllNeedRcs()
{
	std::vector<unsigned int> vecOut;
	if (StoryDataCenter::Get()->GetOneItemOwnRoleIds(vecOut))
	{
		size_t count = vecOut.size();
		for (size_t index = 0;index<count;index++)
		{
			unsigned int roleId = vecOut[index];
			if (roleId != 0)
			{
				// Note: 加载相应的资源文件
				SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(roleId,true);
			}
		}
	}
}

void StoryInstanceDirector::LoadAllRoles()
{
	std::vector<unsigned int> vecOut;
	if (StoryDataCenter::Get()->GetOneItemAllRoleIds(vecOut))
	{
		size_t count = vecOut.size();
		for (size_t index = 0;index<count;index++)
		{
			unsigned int roleId = vecOut[index];
			SpriteSeer* pSeer = 0;
			if (roleId != 0)
			{
				pSeer = InsertOneRole(index+1,roleId);

				if (pSeer == NULL)
				{
					char buffer[100];
					sprintf(buffer,"Story Anim Rcs Not Found %d",roleId);
					std::string title = "Story Notification";
					CCMessageBox(buffer, title.c_str());
					break;
				}

				// Note: 添加阴影
				unsigned int seerTypeId = pSeer->GetTypeId();
				if (false == SpriteElfConfigFromLuaManager::getInstance()->TellIsElfId(seerTypeId) && 
					false == ParticleManager::Get()->TellIsEffectByRcsID(seerTypeId))
				{					
					//ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("spriteBin/shadow.bin");
					//if(pAsprite != NULL)
					//{
					//	CCPoint cp;
					//	CCSpriteBatchNode* pBatchNode = pAsprite->getSpriteBatchNodeFromFrame_Middle(0, 0, cp);
					//	if(pBatchNode != NULL)
					//	{
					//		pSeer->addChildToRoot(pBatchNode);
					//	}
					//}
					LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
					if (pLayer)
					{
						CATLObjectLayer* _pObjLayer = dynamic_cast<CATLObjectLayer*>(pLayer->getObjectLayer());
						if(_pObjLayer)
						{
							_pObjLayer->AddRoleShadow(pSeer);
						}
					}
				}
			}
			else
			{
				pSeer = GameManager::Get()->getHero();
				//// Note: 复制一个角色
				cloneHero = SpriteSeer::Clone(pSeer);
				pSeer = InsertOneRole(index+1,roleId,cloneHero);
				if (pSeer)
				{
					m_heroIndex = index+1;

					LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
					if (pLayer)
					{
						CATLObjectLayer* _pObjLayer = dynamic_cast<CATLObjectLayer*>(pLayer->getObjectLayer());
						if(_pObjLayer)
						{
							_pObjLayer->AddRoleShadow(pSeer);
						}
					}
				}
			}
			if (pSeer)
			{
				pSeer->getRoot()->setVisible(false);
				pSeer->setShadowVisible(false);
			}
		}
	}	
}

void StoryInstanceDirector::RemoveAndClearRcs()
{
	std::map<unsigned int,SpriteSeer*>::iterator iter = m_mapRoleIdAndInstance.begin();
	while(iter != m_mapRoleIdAndInstance.end())
	{
		SpriteSeer* pSeer = (*iter).second;
		//if (m_heroIndex == (*iter).first)
		//{
		//	pSeer->getRoot()->setVisible(true);
		//}
		//else
		//{
		//	if (m_isTaskStory)
		//	{
		//		pSeer->removeFromParentAndCleanup(true);
		//	}			
		//	else
		//	{
		//		pSeer->getRoot()->setVisible(false);
		//	}
		//}		
		if (m_isTaskStory)
		{
			pSeer->removeFromParentAndCleanup(true);
		}			
		else
		{
			pSeer->getRoot()->setVisible(false);
			pSeer->setShadowVisible(false);
			pSeer->unscheduleAllAnimationUpdate();
		}

		iter ++;
	}

	if (m_isTaskStory)
	{
		cloneHero = NULL;
	}

	m_mapRoleIdAndInstance.clear();
}

bool StoryInstanceDirector::TrackDialogAnim(unsigned int frame)
{
	// Note: 判断当前帧是否有对话动画
	StoryDialogData dialogData;
	if (StoryDataCenter::Get()->IsOneFrameHasDialog(frame,dialogData))
	{
		if (m_dialogLayer)
		{
			std::map<unsigned int,StoryRoleBasicData> roleBasicData = StoryDataCenter::Get()->GetStoryRoleBasicData();
			std::map<unsigned int,StoryRoleBasicData>::iterator itr = roleBasicData.find(dialogData.getRoleIndex());
			if (itr != roleBasicData.end())
			{
				m_dialogLayer->setStoryRoleBasicData(itr->second);
			}
			m_dialogLayer->setStoryDialogData(dialogData);
			m_dialogLayer->ShowDialogPanel();

			m_isPause = true;
			return true;
		}
	}
	else
	{
		if (m_dialogLayer)
		{
			m_dialogLayer->ClearDialogPanel();
		}
	}
	return false;
}

SpriteSeer* StoryInstanceDirector::InsertOneRole(unsigned int indexId,unsigned int roleId,SpriteSeer *pDefaultSeer /* = 0 */,CCPoint pt /* = ccp */)
{
	std::map<unsigned int,SpriteSeer*>::iterator iter = m_mapRoleIdAndInstance.find(indexId);
	if (iter != m_mapRoleIdAndInstance.end())
	{
		return (*iter).second;
	}
	
	SpriteSeer* pSeer = 0;
	if (pDefaultSeer == 0)
	{
		PLAYER_ID playerID;
		playerID.setPlayId(0,0,0);
		pSeer = SpriteSeer::seerWithID(playerID,"",roleId,false,false);
	}
	else
	{
		pSeer = pDefaultSeer;
	}

	if (pSeer)
	{
		// Note: Add To Obj Layer
		LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
		if (pLayer)
		{
			CCNode* pObjLayer = (pLayer->getObjectLayer());
			if (pObjLayer)
			{
				if (pt.x != -1 && pt.y != -1)
				{
					pObjLayer->addChild(pSeer,LevelLayer::sCalcZorder(pt));
				}
				else
				{
					pObjLayer->addChild(pSeer);
				}

				m_mapRoleIdAndInstance.insert(std::make_pair(indexId,pSeer));
			}
		}
	}

	return pSeer;
}

SpriteSeer* StoryInstanceDirector::GetOneRole(unsigned int indexId)
{
	std::map<unsigned int,SpriteSeer*>::iterator iter = m_mapRoleIdAndInstance.find(indexId);
	if (iter != m_mapRoleIdAndInstance.end())
	{
		return (*iter).second;
	}
	return 0;
}

bool StoryInstanceDirector::TrackDisplayPicAnim(unsigned int frame)
{
	std::string picName;
	if (StoryDataCenter::Get()->IsOneFrameHasDisplayPic(frame,picName))
	{
		if (m_dialogLayer)
		{
			m_dialogLayer->SetStoryDisplayPicName(picName);
			m_dialogLayer->ShowDisplayPanelOrNot(true);
			m_dialogLayer->ShowDialogPanelOrNot(false);
			m_dialogLayer->ShowDisplayPicPanel();

			m_isPause = true;
			return true;
		}
	}
	return false;
}

bool StoryInstanceDirector::TrackPlayerAnim(unsigned int frame)
{
	// Note: Add To Obj Layer
	LevelLayer* pLayer = LevelManager::sShareInstance()->getCurrentLevelLayer();
	CCNode* pObjLayer = pLayer->getObjectLayer();

	std::map<unsigned int,StoryRoleBasicData> storyData = StoryDataCenter::Get()->GetStoryRoleBasicData();
	std::map<unsigned int,StoryRoleBasicData>::iterator iter = storyData.begin();
	while(iter != storyData.end())
	{
		bool bPosChanged = false;
		SpriteSeer* pCurHero = GetOneRole((*iter).first);

		std::map<unsigned int,StoryFrameData>::iterator frameIter = (*iter).second.mMapStoryFramesData.find(frame);
		if (frameIter != (*iter).second.mMapStoryFramesData.end())
		{
			//// Note: 当前帧存在
			StoryFrameData frameData = (*frameIter).second;
			CCPoint pos = frameData.getRolePos();						

			// Note: 位置发生变化，移动位置，或者新创建角色
			if (StoryFrameData::IsPosChanged(pos))
			{
				bPosChanged = true;

				bool bVisiable = pCurHero->getRoot()->isVisible();
				if (bVisiable == false)
				{
					pCurHero->getRoot()->setVisible(true);
					pCurHero->setShadowVisible(true);
				}
				pCurHero->setPosition(pos);
				pObjLayer->reorderChild(pCurHero,LevelLayer::sCalcZorder(pos));
			}

			int nVisibleFlag = frameData.getVisible();
			if (nVisibleFlag == 0)
			{
				//pCurHero->setVisible(false);
				pCurHero->getRoot()->setVisible(false);
				pCurHero->setShadowVisible(false);
			}
			else if (nVisibleFlag == 1)
			{
				//pCurHero->setVisible(true);
				pCurHero->getRoot()->setVisible(true);
				pCurHero->setShadowVisible(true);
			}

			int actorId = frameData.getActorId();
			if (actorId != -1)
			{
				bool bFlipX = pCurHero->IsAnimFlipX();
				pCurHero->SetAnim(actorId,0);
				pCurHero->SetAnimFlipX(bFlipX);
				bool bAnimPauseOnOver = frameData.getIsPauseOnOver();
				pCurHero->SetAnimPauseOnOver(bAnimPauseOnOver);
			}

			int nFlip = frameData.getFlip();
			if (nFlip != -1)
			{
				if (nFlip == 0)
				{
					pCurHero->SetAnimFlipX(false);
				}
				else if (nFlip == 1)
				{
					pCurHero->SetAnimFlipX(true);
				}
			}
		}

		if (false == bPosChanged)
		{
			// Note: 不存在的情况下处理移动
			if (false == bPosChanged)
			{
				unsigned int preFrameIndex = 0;
				unsigned int nextFrameIndex = 0;
				CCPoint prePoint = CCPointZero;
				CCPoint nextPoint = CCPointZero;
				if(StoryDataCenter::Get()->GetPreAndNextPointFrameData((*iter).first,(int)m_curFrame,preFrameIndex,nextFrameIndex,prePoint,nextPoint))
				{
					// Note: 处理移动

					unsigned int sumMoveFrames = (nextFrameIndex - preFrameIndex);
					unsigned int runningFrames = (int)m_curFrame - preFrameIndex;
					if (runningFrames >= sumMoveFrames)
					{
						return false;
					}
					// Note: 运动速率
					CCPoint dir = ccpSub(nextPoint,prePoint);
					float distance = sqrt(ccpLengthSQ(dir));
					float vPerFrame = distance/sumMoveFrames;
					if(dir.x == 0 && dir.y == 0)
					{
						return false;
					}
					dir = ccpNormalize(dir);

					CCPoint nowPoint = ccpAdd(prePoint,ccp(vPerFrame*runningFrames*dir.x,vPerFrame*runningFrames*dir.y));
					pCurHero->setPosition(nowPoint);
					pObjLayer->reorderChild(pCurHero,LevelLayer::sCalcZorder(nowPoint));
				}
			}
		}

		iter++;
	}

	return true;
}

bool StoryInstanceDirector::IsStart()
{
	return m_isStart;
}

void StoryInstanceDirector::SendReqCheckPlayedStory()
{
	//static bool bSendData = false;
	if (bSendData)
	{
		StoryDataCenter::Get()->SendReqCheckPlayedStory();
		bSendData = false;
	}	
}

bool StoryInstanceDirector::IsOneItemUsed(int id)
{
	std::set<int>::iterator iter = m_usedStoryInstance.find(id);
	if (iter != m_usedStoryInstance.end())
	{
		return true;
	}
	return false;
}

void StoryInstanceDirector::PushUsedItemId(int id)
{
	m_usedStoryInstance.insert(id);
}

void StoryInstanceDirector::CheckStoryType()
{
	if (m_isTaskStory)
	{
		switch(m_cutAnimHappendWhen)
		{
		case 0:
			{
				m_eStoryType = kType_MainLand_PreTask;
			}
			break;
		case 1:
			{
				m_eStoryType = kType_MainLand_AcceptTask;
			}
			break;
		case 2:
			{
				m_eStoryType = kType_MainLand_RewardTask;
			}
			break;
		case 3:
			{
				m_eStoryType = kType_MainLand_OverTask;
			}
			break;
		default:
			{
				m_eStoryType = kType_Story_UnKnow;
			}
			break;
		}
	}
	else
	{
		switch(m_cutAnimHappendWhen)
		{
		case 0:
			{
				m_eStoryType = kType_FB_BeginMap;
			}
			break;
		case 1:
			{
				m_eStoryType = kType_FB_EndMap;
			}
			break;
		default:
			{
				m_eStoryType = kType_Story_UnKnow;
			}
			break;
		}
	}
}

void StoryInstanceDirector::SetCameraFollowType(CameraController::CameraFollowType eFollowType)
{
	switch(eFollowType)
	{
	case CameraController::E_FixPoint:
		{
			CCPoint cameraPoint = StoryDataCenter::Get()->GetCameraPoint();
			CameraController::sharedCameraController()->SetFollowType(CameraController::E_FixPoint);
			CameraController::sharedCameraController()->SetFollowPoint(cameraPoint);
		}
		break;
	case CameraController::E_FollowHero:
		{
			CameraController::sharedCameraController()->SetFollowType(CameraController::E_FollowHero);
		}
		break;
	default:
		{
			CameraController::sharedCameraController()->SetFollowType(CameraController::E_FollowHero);
		}
		break;
	}
}

void StoryInstanceDirector::HideMainLandThings()
{
	if (m_isTaskStory)
	{
		ShowMainLandUILayer(false);
		BoneNPCManager::getInstance()->ShowAllNpc(false);		
		// Note: 关闭其他窗体
		MainMenuLayer::CloseAllInStackLayer();
	}
}

void StoryInstanceDirector::HideFBThings()
{
	if (false == m_isTaskStory)
	{
		// Note: 隐藏摇杆UI
		m_isHSJoystickEnable = HSJoystick::sharedJoystick()->getIsEnable();
		//if (m_isHSJoystickEnable)
		{
			HSJoystick::sharedJoystick()->setVisible(false);
			HSJoystick::sharedJoystick()->setIsEnable(false);
		}
		ShowBattleUILayer(false);
		ShowHeroElf(false);
		if(m_eStoryType == kType_FB_BeginMap)
		{
			LevelManager::sShareInstance()->ShowMonsterOrNot(false);
		}
	}
}

void StoryInstanceDirector::ShowMainLandThings()
{
	if (m_isTaskStory)
	{
		ShowMainLandUILayer(true);
		BoneNPCManager::getInstance()->ShowAllNpc(true);
		ShowOtherPlayers(true);
		ShowOtherElf(true);
		ShowHeroElf(true);
	}
}

void StoryInstanceDirector::ShowFBThings()
{
	if (false == m_isTaskStory)
	{
		if (m_isHSJoystickEnable)
		{
			HSJoystick::sharedJoystick()->setVisible(true);
			HSJoystick::sharedJoystick()->setIsEnable(true);
		}
		ShowBattleUILayer(true);
		if (m_eStoryType == kType_FB_EndMap)
		{
			ShowHeroElf(false);
		}
		else
		{
			ShowHeroElf(true);
		}		
	}
}

void StoryInstanceDirector::CreateChildLayers()
{
	SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
	if (pSceneLayer)
	{
		MainMenuLayer * mLayer = pSceneLayer->getMainMenuLayer();
		if (mLayer)
		{
			int tag = mLayer->getLayerTagByName("StoryInstanceDirector");
			m_dialogLayer = (InstanceDialogLayer*)mLayer->getChildByTag(tag);
			// Note: 显示对话界面
			if (m_dialogLayer == NULL)
			{
				m_dialogLayer = InstanceDialogLayer::create();
				mLayer->addChild(m_dialogLayer,1,tag);
			}
			else
			{
				m_dialogLayer->setVisible(true);
			}
			tag = mLayer->getLayerTagByName("StoryColorLayer");
			m_pColorLayer = (CCLayerColor*)mLayer->getChildByTag(tag);
			if (m_pColorLayer == NULL)
			{
				m_pColorLayer = CCLayerColor::create(ccc4(0,0,0,255));
				m_pColorLayer->setVisible(false);
				mLayer->addChild(m_pColorLayer,0,tag);
			}
			else
			{
				m_pColorLayer->setVisible(false);
			}
		}
	}
}

void StoryInstanceDirector::ClearChildLayers()
{
	if (m_dialogLayer != NULL)
	{
		m_dialogLayer->HideLayerAndClean();
	}
}

void StoryInstanceDirector::UpdateByTaskState(unsigned int taskId,eTaskState taskState)
{
	switch(taskState)
	{
	case kType_TASK_WAIT_REWARD:
		{
			// Note: 主城中任务，等待领取奖励
			StoryInstanceDirector::Get()->Begin(taskId,0,2,true);
			break;
		}
	case kType_TASK_NOT_ACCEPT:
		{
			// Note: 主城中任务,，任务等待接受
			StoryInstanceDirector::Get()->Begin(taskId,0,0,true);

			// Note: 处理上次已经完成的任务，但是剧情可能没有触发
			unsigned int preTaskID = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Task/TaskState.lua", "GetPreBuildTaskID", taskId);
			StoryInstanceDirector::Get()->Begin(preTaskID,0,3,true);
			break;
		}
	case kType_TASK_RUNNING:
		{
			// Note: 主城中任务,，处理任务接受完成后，
			StoryInstanceDirector::Get()->Begin(taskId,0,1,true);
			break;
		}
	}
}

void StoryInstanceDirector::SetConfigDataReGetOrNot(bool bGet)
{
	bSendData = bGet;
	if (bSendData)
	{
		m_usedStoryInstance.clear();
	}
}