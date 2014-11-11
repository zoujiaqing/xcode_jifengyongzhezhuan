#ifndef DAMAGE_VALUE_EFFECT_H
#define DAMAGE_VALUE_EFFECT_H

#include "cocos2d.h"
#include "UILayout.h"
#include "UILabelAtlas.h"
#include <map>
using namespace cocos2d; 
using namespace TXGUI;

class DamageValueEffect : public cocos2d::CCLayer
{
public:
	DamageValueEffect();
	virtual ~DamageValueEffect(void);
	CREATE_FUNC(DamageValueEffect);

	virtual bool init();
	void HandleCritBurst(CCPoint pt,unsigned int effectType = 0);
	void HandleDamageNumberEffect(CCPoint parentPt,unsigned int damage,bool isCrit,bool isHarmFul,bool isFairy);
	void moveWithParabola(CCSprite* mSprite, CCPoint startPoint, CCPoint endPoint, float time);
	void RemoveLable(CCNode* sender);
	CCLabelAtlas* CreateDamageLable(unsigned int damage,CCPoint parentPos,bool isHarmFul);
	CCLabelAtlas* CreateFairyDamageLabel(unsigned int damage,CCPoint parentPos);
	CCNode* CreateCritDamageLable(unsigned int damage,CCPoint parentPos,bool isHarmFul);
	CCSprite* CreateMissLabel(CCPoint parentPos);
	CCSprite* CreateBlockLabel(CCPoint parentPos);
	CCLabelAtlas* CreateAddBloodLabel(unsigned int blood,CCPoint parentPos);
	virtual void update(float dt);

	virtual void clearAllLabel();
private:
	CCAction* getBattleUIAction();
private:
	std::map<CCNode*,float>*	m_pDamageLabels;
};

#endif