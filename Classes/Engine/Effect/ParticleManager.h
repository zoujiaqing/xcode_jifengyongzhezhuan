#pragma  once
#include "Singleton_t.h"
#include "AParticleNode.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "HeroEffectDefine.h"
#include "DamageValueEffect.h"
#include "cocos2d.h"
#include "SASpriteDefine.h"
#include "RoleActorBase.h"
#include "EffectSprite.h"
#include "SpriteMonster.h"
#include "set"
USING_NS_CC;

class ParticleManager : public TSingleton<ParticleManager>, public CCObject
{
public:
	struct ParticleConditionData{
		unsigned int type_id;
		unsigned int sumNums;
		unsigned int playedNum;
		float	lifeTime;
		std::map<CCNode*,float> attachParticleNodes;
		ParticleConditionData(){
			type_id = 0;
			sumNums = 0;
			playedNum = 0;
			lifeTime = 0;
		}
	};
public :
	ParticleManager();
	virtual ~ParticleManager();

	AParticleNode* CreateParticleNode(EffectType type);
public:
	void HandleParticleRemoved(unsigned int index);
	void DumpParticlesInfor();	
	EffectSprite* createEffectSprite(int type,const char *name,bool isNeedRegister = true);
	CCNode * createEffectSpriteAndPlay(int type,const char *name,int animId, int nloop, bool isLoop);

	void SetEnemyHpLable(float currentHp,PLAYER_ID targetMonster,unsigned int type,unsigned int enemyType,bool isSelf);

	//≤•∑≈Ãÿ–ß
	EffectSprite * playOneTimeEffect(int effectId, CCPoint position, float delayTime = 0, int playType = 0);
	EffectSprite * playArcherEffect(int effectId, CCPoint startPosition, CCPoint endPosition, float speed, float delayTime = 0, int playType = 0);

	EffectSprite * playEffectByInfo(SkillInfo info);
	EffectSprite * playEffectByInfoMonster(SkillInfo info, SpriteMonster * monster);
	bool stopEffectOfMonster(SpriteMonster * monster);

	void Update(float dt);
	void clearEffectBuff();

	void GetEffectRcsIDRange(unsigned int &min,unsigned int &max);
	bool TellIsEffectByRcsID(unsigned int id);

	void AddParticleConditonData();
	void AddOneParticleConditionData(unsigned int type,unsigned int nums,float lifeTime);
	void RemoveOneParticlePlayedNum(unsigned int type);
	bool TellWhetherOneParticleCanPlay(unsigned int type,CCNode* attachParticle,bool & bAddOneParticle);
private: 
	void AddParticleNodeToList(unsigned int index,AParticleNode* particle);
	char* GetParticlePlist(EffectType type);
	EffectType getEffectType(const char* actionTypeStr);

	EffectSprite * getFreeEffectNode(int effectId);
	void ccCallShowOneTimeEffect(CCNode * node);	
	void ccCallShowContinueEffect(CCNode * node);
	void ccCallRecycleEffect(CCNode * node);	
private: 
	unsigned int mEffectIndex;
	std::map<unsigned int, AParticleNode*> mParticleList;
	PLAYER_ID m_currentTarget;

	map<int, map<EffectSprite*, bool> > effectNodeMap;
	map<SpriteMonster*, set<EffectSprite*> > monsterEffectMap;
	set<EffectSprite*> dynZOrderMap;

	unsigned int mMinRcsID;
	unsigned int mMaxRcsID;

	std::map<unsigned int ,ParticleConditionData> ParticleConditionDatas;
};
