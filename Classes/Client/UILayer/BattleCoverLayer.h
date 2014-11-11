#pragma once

#include "ASprite.h"
#include "UILabelTTF.h"
#include "UILayout.h"

enum COVER_TYPE
{
	COVER_AUTOATTACK,
	COVER_WAITINGOTHER,
	COVER_AUTOSEARCH,
	COVER_NONE,
};

using namespace std;
using namespace cocos2d;

class BattleCoverLayer : public CCLayer
{
public:
	BattleCoverLayer();
	virtual ~BattleCoverLayer();

	CREATE_FUNC(BattleCoverLayer);
	virtual bool init();

	void openBattleCoverLayer(COVER_TYPE coverType);
	void closeBattleCoverLayer(CCObject* sender);

	COVER_TYPE getCoverType(){return layerType;}

protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

private:
	UILabelTTF* infoLabel;
	COVER_TYPE layerType; 

	string waitText;
	string autoAttackText;
	string autoSearchText;

	CCSprite* m_autoFindWayInfoLabel;
	CCSprite* m_autoFightInfoLabel;
};

