#ifndef CARD_ATTRIBUTION_LAYER_H
#define CARD_ATTRIBUTION_LAYER_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
#include <map>
using namespace cocos2d;
using namespace TXGUI;

class CardAttributionLayer : public cocos2d::CCLayer
{
public:
	CardAttributionLayer();
	virtual ~CardAttributionLayer();
	CREATE_FUNC(CardAttributionLayer);
	virtual bool init();

	void onCloseBtClicked(CCObject* obj);
	void showCardsAttr();
private:
	
	void setAttrLabel(unsigned int attrKey,unsigned int value);
	void checkUserCardsAttr();
private:
	std::map<unsigned int,unsigned int> m_cardAttrMap;
	UILayout*	m_uiLayout;
};

#endif