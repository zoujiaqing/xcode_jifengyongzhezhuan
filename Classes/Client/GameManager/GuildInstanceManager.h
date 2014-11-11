#ifndef _GUILDINSTANCEMANAGER_H_
#define _GUILDINSTANCEMANAGER_H_

#include "Singleton_t.h"
#include "SpriteSeer.h"
#include "UILayout.h"
#include "UserData.h"

class GuildInstanceManager : public TSingleton<GuildInstanceManager>
{
public:
	GuildInstanceManager();
	~GuildInstanceManager();

	void setAIPlayerId(PLAYER_ID uid){this->aiPlayerId = uid;}
	PLAYER_ID getAIPlayerId(){return aiPlayerId;}

	void setAIAutoAttack(bool autoAttack);

	//ÅÐ¶ÏSpriteSeerÊÇ·ñÎªai
	bool isAIPlayer(SpriteSeer * player){return player == aiPlayer;}
	SpriteSeer * getAIPlayer(){return aiPlayer;}

	void setAISkillsId(vector<int> equipSkills);

	void resetData();

	void Update(float fTime);
	void AutoAttack();

	void attackToTarget(CCPoint targetPos);

	void moveToPortal();

	int getBestSkill(CCPoint targetPos);
private:

	SpriteSeer * aiPlayer;

	std::vector<int> attackSkillVec;
	std::vector<int> buffSkillVec;

	bool autoAttack;
	float autoAttackCDTime;
	PLAYER_ID aiPlayerId;

	CC_SYNTHESIZE(bool, m_isEnterFromGuildScene, IsEnterFromGuildScene);

	int rageBasic1;
	int rageBasic2;
	int rageRate1;
	int rageRate2;
};

#endif