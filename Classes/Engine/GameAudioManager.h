//
//  GameAudioManager.h
//  iSeer
//
//  Created by razer tong on 12-3-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_GameAudioManager_h
#define iSeer_GameAudioManager_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CCUserDefault.h"

//ui
typedef enum
{
	NORMAL_ATTACK, // 普通攻击

}KEffectID;



class GameAudioManager 
{
private:
	static GameAudioManager* pSharedManager; 
	int m_bIndex;//循环播放的index 1,2切换　3为停止播放背景乐　4 loop
	char *m_tempName[2];

	//effect buff
	char *m_bBuff;

	//音效开关
	bool m_bEffectSwitch;

	//背景音乐开关
	bool m_pBgmSwitch;

	bool m_messageSwitch;

	unsigned int m_userID;      //for 91
	std::string  m_nickName;    //for 91

private:

	//获得文件名
	const char *getFileName(KEffectID eid) const;

public:

	GameAudioManager();
	virtual ~GameAudioManager();

	// static instance
	static GameAudioManager* sharedManager();
	static void Destroy();
	
	/// do some initialization 
	void InitDevice();

	/// clear audio device
	void ShutDown();


	//修改了框架　供CDAudioManager回调
	void playBGM() ;

	void playBGMWithMap(unsigned int musicId);

	void playLoadingBGM();

	void playStartGameAnimationSound();

	void stopPlayBGM();

	bool isBGMPlaying();

	//精灵说话的音效
	void playEffectWithID(const int eid,const float delay =0) ;

	//技能的音效
	void playEffectWithID( const char *fileName,const float delay=0) ;
	
	//ui的音效 //捕捉的音效
	void playEffect(const KEffectID eid,bool isLoop,const bool isStopBGM =false) ;
	void playEffect(unsigned int soundID,bool isLoop,const float delay=0);

	// Note: 预加载音效
	void preLoadMonsterEffect(unsigned int monsterId);
	void preLoadEffect(unsigned int soundID);

	void stopEffect(unsigned int soundID);

	//设置音效
	void setIsEffectSwitchOn(bool isOn) ;

	void setMessageSwitch(bool isOn);

	unsigned int getNine1UserID()  { return m_userID; }
	void setNine1UserID(unsigned int userid);
	std::string getNine1Nickname() { return m_nickName; }
	void setNine1Nickname(std::string str);

	//获得
	bool getIsEffectSwitchOn();

	bool isEffectIsPlaying(unsigned int nSoundId);

	bool getIsBgmSwitchOn();

	bool getTrainSwitch() { return m_messageSwitch; }

	void stopAllEffect();

	void setBackgroundMusicVolume(float volume);

	 void setEffectsVolume(float volume);

	 // Note: 角色声音预加载
	 void LoadLevelMonsterAudio(unsigned int levelId,unsigned int mapId);
	 void LoadLevelHeroCommonAudio(unsigned int heroTypeId);
protected:
	std::set<unsigned int> m_loadedMonsterAudios;
	std::set<unsigned int> m_loadedHeroAudios;
};


#endif
