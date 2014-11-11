//
//  GameAudioManager.cpp
//  iSeer
//
//  Created by razer tong on 12-3-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameAudioManager.h"
#include "XPlatformManager.h"
#include "GameResourceManager.h"
#include "GameConfigFromLuaManager.h"
#include "MainLandManager.h"
#include "LuaTinkerManager.h"
USING_NS_CC;
using namespace CocosDenshion;

const char *EFFECT_SWITCH_	= "EFFECT_SWITCH_";
const char *BGM_SWITCH_			= "BGM_SWITCH_";
const char *TRAIN_SWITCH_	= "TRAIN_SWITCH_";
const char *FIRST_START_GAME	= "FIRST_START";
const char *NINE_1_USER_ID_ = "NINE_1_USER_ID_";
const char *NINE_1_NICKNAME_	= "NINE_1_NICKNAME_";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
const char * effectSuffix=".ogg";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char * effectSuffix=".mp3";
#else
const char * effectSuffix=".wav";
#endif
/// all the background music which used by our game are put here! That is all.

GameAudioManager* GameAudioManager::pSharedManager = NULL;

GameAudioManager::GameAudioManager()
{
	m_bIndex = 3;
	m_tempName[0] = NULL;
	m_tempName[1] = NULL;
	m_bBuff = (char *)malloc(30 *sizeof(char));
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
}


/// do some initialization 
void GameAudioManager::InitDevice()
{
	//第一次启动
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(FIRST_START_GAME)) 
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey(FIRST_START_GAME, true);

		//初始化并赋值
		m_bEffectSwitch = true;
		m_pBgmSwitch = true;
		m_messageSwitch = true;
		CCUserDefault::sharedUserDefault()->setBoolForKey(EFFECT_SWITCH_, m_bEffectSwitch);
		CCUserDefault::sharedUserDefault()->setBoolForKey(BGM_SWITCH_, m_pBgmSwitch);
		CCUserDefault::sharedUserDefault()->setBoolForKey(TRAIN_SWITCH_, m_messageSwitch);
	}
	else
	{
		m_bEffectSwitch = CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_SWITCH_);
		m_pBgmSwitch    = CCUserDefault::sharedUserDefault()->getBoolForKey(BGM_SWITCH_);
		m_messageSwitch    = CCUserDefault::sharedUserDefault()->getBoolForKey(TRAIN_SWITCH_);
	}

}

/// clear audio device
void GameAudioManager::ShutDown()
{
	 
}

GameAudioManager::~GameAudioManager()
{
	ShutDown();

	SimpleAudioEngine::sharedEngine() ->end();
	free(m_bBuff);
}


GameAudioManager* GameAudioManager::sharedManager()
{
	if(!pSharedManager)
	{
		pSharedManager = new GameAudioManager();
	}
	return pSharedManager;
}

void GameAudioManager::Destroy()
{
	if (pSharedManager)
	{
		delete pSharedManager;
		pSharedManager = NULL;
	}
}


void GameAudioManager::setIsEffectSwitchOn(bool isOn) 
{
	m_bEffectSwitch = isOn;
	CCUserDefault::sharedUserDefault()->setBoolForKey(EFFECT_SWITCH_, isOn);
}


void GameAudioManager::setMessageSwitch(bool isOn)
{
	m_messageSwitch = isOn;
	CCUserDefault::sharedUserDefault()->setBoolForKey(TRAIN_SWITCH_, m_messageSwitch);	
}

void GameAudioManager::setNine1UserID(unsigned int userid)
{
	m_userID = userid;
	CCUserDefault::sharedUserDefault()->setIntegerForKey(NINE_1_USER_ID_, (int)m_userID);
}

void GameAudioManager::setNine1Nickname(std::string str)
{
	m_nickName = str;
	CCUserDefault::sharedUserDefault()->setStringForKey(NINE_1_NICKNAME_, m_nickName);
}

bool GameAudioManager::getIsEffectSwitchOn()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_SWITCH_);
}

bool GameAudioManager::getIsBgmSwitchOn()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey(BGM_SWITCH_);
}

void GameAudioManager::playBGM()
{
	if (m_bIndex !=3) 
	{
		if (SimpleAudioEngine::sharedEngine() ->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::sharedEngine() ->stopBackgroundMusic();
		}
		if (m_bIndex == 1 && m_tempName[0]!=NULL) 
		{
			m_bIndex = 2;
			SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(m_tempName[0],false);
		}else if(m_bIndex ==2 && m_tempName[1]!=NULL)
		{
			m_bIndex = 1;
			SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(m_tempName[1],false);
		}else if(m_bIndex == 4 && m_tempName[0]!=NULL)
		{
			m_bIndex = 4;
			SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(m_tempName[0],true);
		}
	}

	if (SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() <= 0)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void GameAudioManager::playBGMWithMap(unsigned int musicId)
{	
	if(musicId == 0)
	{
		return;
	}
	std::string audioPath = "Sound/";
	char musicChar[10];
	sprintf(musicChar,"%d",musicId);
	audioPath.append(musicChar);
	audioPath.append(".mp3");

	std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(audioPath.c_str());
	SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(pFullFilePath.c_str(),true);
	if (SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() <= 0)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void GameAudioManager::playLoadingBGM()
{
	std::string audioPath = "Sound/300000.mp3";
	std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(audioPath.c_str());
	SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(pFullFilePath.c_str(),true);
	if (SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() <= 0)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void GameAudioManager::playStartGameAnimationSound()
{
	std::string audioPath = "Sound/300001.mp3";
	std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(audioPath.c_str());
	SimpleAudioEngine::sharedEngine() ->playBackgroundMusic(pFullFilePath.c_str(),false);	
	if (SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() <= 0)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void GameAudioManager::stopPlayBGM()
{
	m_bIndex =3;
	SimpleAudioEngine::sharedEngine() ->stopBackgroundMusic();
}

bool GameAudioManager::isBGMPlaying()
{
	return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

void GameAudioManager::playEffectWithID(const int eid, const float delay) 
{
	if (SimpleAudioEngine::sharedEngine()->getEffectsVolume() <= 0)
	{
		return ;
	}
	if (m_bEffectSwitch) 
	{
		sprintf(m_bBuff,"%u%s",eid, effectSuffix);
		if (delay ==0)
		{
			SimpleAudioEngine::sharedEngine() ->playEffect(m_bBuff);
		}else
		{            
			IPlatform* platform = XPlatformManager::GetPlatformDevice();
			if (platform)
			{
				platform->PlayAudioEffect(m_bBuff, delay);
			}
		}
	}
}

void GameAudioManager::playEffectWithID( const char *fileName,const float delay) 
{
	if (SimpleAudioEngine::sharedEngine()->getEffectsVolume() <= 0)
	{
		return ;
	}
	if (m_bEffectSwitch) 
	{
		sprintf(m_bBuff,"%s%s",fileName, effectSuffix);
		if (delay ==0)
		{
			SimpleAudioEngine::sharedEngine() ->playEffect(m_bBuff);
		}else
		{            
			IPlatform* platform = XPlatformManager::GetPlatformDevice();
			if (platform)
			{
				platform->PlayAudioEffect(m_bBuff, delay);
			}
		}
	}
}

void GameAudioManager::playEffect(const KEffectID eid,bool isLoop,const bool isStopBGM) 
{
	if (SimpleAudioEngine::sharedEngine()->getEffectsVolume() <= 0)
	{
		return ;
	}
	if (m_bEffectSwitch) 
	{
		if (isStopBGM)
		{
			m_bIndex = 3;
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}
		const char * audioPath = this->getFileName(eid);
		std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(audioPath);
		SimpleAudioEngine::sharedEngine()->playEffect(pFullFilePath.c_str(),isLoop);
	}
}

void GameAudioManager::playEffect(unsigned int soundID,bool isLoop,const float delay) 
{
	if (SimpleAudioEngine::sharedEngine()->getEffectsVolume() <= 0)
	{
		return ;
	}
	if (m_bEffectSwitch) 
	{
		sprintf(m_bBuff,"%u%s",soundID, effectSuffix);
		std::string path = "Sound/";
		path.append(m_bBuff);
        
        std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(path.c_str());
      //  printf("%s %s", __FUNCTION__, pFullFilePath.c_str());
		if (delay ==0)
		{
			SimpleAudioEngine::sharedEngine()->playEffect(pFullFilePath.c_str(),isLoop);
           
            //CCLOG("GameAudioManager::playEffect: %s, soundID : %u", pFullFilePath.c_str(), soundID);
            

		}/*else
		 {            
			IPlatform* platform = XPlatformManager::GetPlatformDevice();
			if (platform)
			{
				platform->PlayAudioEffect(m_bBuff, delay);
			}
		 }*/
	}
}

void GameAudioManager::preLoadEffect(unsigned int soundID)
{
	if (soundID == 0)
	{
		return;
	}
	sprintf(m_bBuff,"%u%s",soundID, effectSuffix);
	std::string path = "Sound/";
	path.append(m_bBuff);

	std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(path.c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(pFullFilePath.c_str());
}

void GameAudioManager::preLoadMonsterEffect(unsigned int monsterId)
{
	std::set<unsigned int>::iterator iter = m_loadedMonsterAudios.find(monsterId);
	if (iter == m_loadedMonsterAudios.end())
	{
		// Note: 加载怪物的死亡音效
		int soundId = LuaTinkerManager::Get()->getLuaConfig<int>("MonsterBasicInfomation", "MonsterBasicInfomation", monsterId, "SoundID_Death");

		preLoadEffect(soundId);
		// Note: 加载怪物的技能音效
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/MapDataTable.lua", "PreLoadOneMonsterAudio",monsterId);	

		m_loadedMonsterAudios.insert(monsterId);
	}
}

void GameAudioManager::stopEffect(unsigned int soundID)
{

	if (m_bEffectSwitch) 
	{
		sprintf(m_bBuff,"%u%s",soundID, effectSuffix);
		std::string path = "Sound/";
		path.append(m_bBuff);
		std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(path.c_str());

		unsigned int hash = CocosDenshion::getHashCodeByString(pFullFilePath.c_str());
		//CCLOG("GameAudioManager::stopEffect: %s| hash= %d, soundID : %u", pFullFilePath.c_str(), hash, soundID);

		SimpleAudioEngine::sharedEngine()->stopEffect(hash);

	}
}

//判断当前音效是否已经播放完
bool GameAudioManager::isEffectIsPlaying(unsigned int nSoundId)
{
	sprintf(m_bBuff,"%u%s",nSoundId, effectSuffix);
	std::string path = "Sound/";
	path.append(m_bBuff);
	std::string pFullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(path.c_str());
	unsigned int hash = CocosDenshion::getHashCodeByString(pFullFilePath.c_str());

	return SimpleAudioEngine::sharedEngine()->getEffectIsPlaying(hash);
}

const char *GameAudioManager::getFileName(KEffectID eid) const
{
	if(eid == NORMAL_ATTACK) 
		return "Sound/321000.mp3";
	return "";
}

void GameAudioManager::stopAllEffect()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void GameAudioManager::setBackgroundMusicVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume/100.0f);
	float _volume = SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
	if (_volume <= 0)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	else
	{
		if (false == SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
	}
}

void GameAudioManager::setEffectsVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume/100.0f);
	float _volume = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	if (_volume <= 0)
	{
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
	}
}

void GameAudioManager::LoadLevelMonsterAudio(unsigned int levelId,unsigned int mapId)
{
	// 加载地图相关资源
	// Note: 从Lua配表中得到怪物的配置信息
	std::vector<unsigned int> monsterIdVec;
	GameConfigFromLuaManager::Get()->getOneMapMonsterIds(levelId,mapId,monsterIdVec);

	size_t vecSize = monsterIdVec.size();
	for (size_t index = 0;index<vecSize;index++)
	{
		unsigned int typeId = monsterIdVec[index];
		preLoadMonsterEffect(typeId);
	}
}

void GameAudioManager::LoadLevelHeroCommonAudio(unsigned int heroTypeId)
{
	std::set<unsigned int>::iterator iter = m_loadedHeroAudios.find(heroTypeId);
	if (iter == m_loadedHeroAudios.end())
	{
		// Note: voice by attacked
		const unsigned int VOICE[7][3] =
		{
			{0, 0, 0},

			{3211010, 3211011, 3211012},
			{3211210, 3211211, 3211212},

			{3211310, 3211311, 3211312},
			{3211410, 3211411, 3211412},

			{3211510, 3211511, 3211512},
			{3211610, 3211611, 3211612},
		};

		for (int index = 0;index < 3;index++)
		{
			unsigned int voiceId = VOICE[heroTypeId][index];

			GameAudioManager::sharedManager()->preLoadEffect(voiceId);
		}

		// Note: voice when dead
		const unsigned int DIE_VOICE[7] =
		{
			0,

			3211009,
			3211209,

			3211309,
			3211409,

			3211509,
			3211609,
		};

		unsigned int voiceId = DIE_VOICE[heroTypeId];

		GameAudioManager::sharedManager()->preLoadEffect(voiceId);

		m_loadedHeroAudios.insert(heroTypeId);
	}
}