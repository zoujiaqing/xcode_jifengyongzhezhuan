#pragma once
#include "cocos2d.h"
#include "UIButton.h"
#include "ASprite.h"

using namespace std;
using namespace cocos2d;

class BattleWinLayer : public cocos2d::CCLayer
{    
public:
	BattleWinLayer();
	virtual ~BattleWinLayer();

	CREATE_FUNC(BattleWinLayer);
	virtual bool init();

	void clickOk(CCObject *sender);
	void ShowLayout();
	void HideLayout();
private:
	ASprite*    m_curUiAs;
};