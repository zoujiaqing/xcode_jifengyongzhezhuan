#ifndef ISEER_SPRITE_SEER_H
#define ISEER_SPRITE_SEER_H

#include "BoneSpriteMoveable.h"
#include "FAbstractTransition.h"
#include "SpriteMonster.h"
#include "UILabelTTF.h"
#include "TXGUIHeader.h"
class SkillStateMachine;
class StateFactory;
class AlphaTestBatchNode;

class SpriteSeer : public BoneSpriteMoveable,
				   public TXGUI::ObserverProtocol
{
	friend class IdleState;
	friend class RunState;
	friend class NormalAttackState;
	friend class LinkTimeState;
	friend class PvAIManager;
	friend class GuildInstanceManager;

public:    
    virtual ~SpriteSeer(); 
    
	static  SpriteSeer*		seerWithID(PLAYER_ID s_id,const char *name, int type, bool isPlayerSet,bool isCreateEffect = true);
    void	ShowNameLabelColor(bool bRedName);
	ccColor3B GetNameLabelColor();

	virtual void setPosition(const CCPoint &position);
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void registerWithTouchDispatcher();
    void unRegisterWithTouchDispatcher();
	virtual bool				isTouchSelf(cocos2d::CCPoint pt);
	virtual cocos2d::CCRect		getRect();

    bool   isTouchInPop(cocos2d::CCPoint pt1 ,cocos2d::CCPoint pt2 ,cocos2d::CCSize sz);
    cocos2d::CCPoint  adaptShowDot(cocos2d::CCPoint pt1 ,cocos2d::CCSize sz1 ,cocos2d::CCSize winsz);
	virtual bool	moveByJoystick(float dt);
	/// do skill 
	virtual bool	DoNormalAttack(void);
	virtual bool	DoNormalAttack(unsigned int skillId);
	void SetCanNormalAttack(bool bAttack){ m_bNormalAttack = bAttack;}
	virtual bool	DoSkillAttack(unsigned int skillId);
	int GetNormalAttackNum(){return m_nNormalAttackNum;}
	virtual void    ByAttacked(unsigned int blood, unsigned int ragePoint, bool isCrit = false,bool isFairyAttack = false);
	/// called by LevelLayer
	void Update(float fTime);

	bool tellIsAttackEnable();

	void DoRun(void);

	void startPathFinding();
	void endPathFinding();

	//自动攻击
	void startAutoAttack();
	void endAutoAttack();

	void attackToTarget(CCPoint targetPos);

	void moveToPortal();
	void activePortal();

	int getBestSkill(CCPoint targetPos);

	bool isAutoAttacking(){return autoAttack;}

	//跨地图寻路
	void moveToCityPosition(int cityId, CCPoint position,int npcId = -1);
	
	// return s
	float GetCurAnimTotalTime(void);

	virtual CCPoint GetJoystickVelocity(void);

	void setLastPosition(CCPoint pt);
	CCPoint getLastPosition();

	float GetAcceleration(void){return m_acceleration;}
	void SetAcceleratiion(float acc){m_acceleration = acc;}

	void stopAllActions(void);

	void PauseStateMachine();
	void ResumeStateMachine();
	void ExistStateMachine();

	void resetNameLabelColor(const ccColor3B& color); 

	void Die(void);
	void Revival(void);

	bool IsDie(void);

	void MoveToSpriteTranspoint(bool bNormalTranspoint);
	bool IsHadFairy(void);
	bool IsHasFightElf(unsigned int pos);

	bool CanHurt(void);
	bool CanAim(void);;

	virtual void SetRoleOpacity(GLubyte var);

	static SpriteSeer* Clone(SpriteSeer* pSprite);
	unsigned int GetAttachElfNum(){
		return m_nAttachElfNum;
	}
	void AddElfNum(){
		m_nAttachElfNum ++;
	}

	int GetFairyBasicRage(int pos);
	int GetFairyRageRate(int pos);

	void addBuffSkill(int skillId);
	void addAttackSkill(int skillId);
protected:

    SpriteSeer(int type, bool isPlayerSet);
    
	//init
    virtual bool  initWithId(PLAYER_ID index,int typeId,const char *name,bool isCreateEffect);	
	
    //actions
    virtual void	startMove();
    virtual void	endMove();

	virtual void beginAStarSearch();
	virtual void endAStarSearch();

	/// init state machine
	virtual void InitStateMachine();
	/// shut down state machine
	virtual void ShutDownStateMachine();

	bool MoveToByJoystick(float dt);

	CPhysicsLayer* createSearchLayer();

	void resetVipMark();
	void resetGuildName();
	void resetWearTitle();

	virtual void onBroadcastMessage(BroadcastMessage* msg);

protected:
	SkillStateMachine	* m_pStateMachine;
	StateFactory* m_pStateFactory;
	bool   touchStatus ;
	int m_nNormalAttackNum;
	bool m_bNormalAttack;

	//自动攻击相关字段
	bool autoAttack;
	float autoAttackCDTime;
    float attackedSoundCDTime;
	std::vector<int> attackSkillVec;
	std::vector<int> buffSkillVec;

	CCPoint m_ptLastServerPosition;

	float m_acceleration;

	unsigned int m_battleSide;
	bool m_stateMachineRunning;

	bool posInited;

	long m_curByAttackVoiceTime;
	long m_curDieVoiceTime;

	//检测是否在传送点附近并激活传送点
	CCPoint portalPosition;
	float portalCheckCDTime;
	TXGUI::UILabelTTF* m_nameLabel;
	CCSprite* m_wearTitle;
	TXGUI::UILabelTTF* m_guildNameSprite;
	AlphaTestBatchNode* m_vipPic;// vip标识

	//提示传送门已开启
	bool showActivePortalInfo;

	unsigned int m_nAttachElfNum;
	float m_nameHeight;
	// 精灵初始怒气及增长率
	int rageBasic1;
	int rageBasic2;
	int rageRate1;
	int rageRate2;
};

#endif
