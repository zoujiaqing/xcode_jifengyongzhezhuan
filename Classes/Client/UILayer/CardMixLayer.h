#ifndef CARD_MIX_LAYER_H
#define CARD_MIX_LAYER_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
using namespace cocos2d;
using namespace TXGUI;

class CardMixLayer : public cocos2d::CCLayer
{
public: 
	CardMixLayer();
	virtual ~CardMixLayer();
	CREATE_FUNC(CardMixLayer);
	virtual bool init();

	void showCardMixLayer(unsigned int cardId,CCSprite* fromCardSprite,unsigned int mixCard,CCSprite* mixSprite,
							unsigned int exchangeId,unsigned int cardNum,unsigned int mixCardNum);
	void hideLayer();

	void onConfirmBtClicked(CCObject* obj);

	void onCancellBtClicked(CCObject* obj);

	void onReceivedCardComposeSuccess(int error);

	static unsigned int getCardAttrKey(unsigned int id);
	static unsigned int getCardLevel(unsigned int id);
	static unsigned int getCardAttrNum(unsigned int id,unsigned int key);
	static const char* getCardAttrText(unsigned int key,unsigned int num);
//private:
//	const char* getCardAttrById(unsigned int id);
private:
	UILayout* m_uiLayout;
	UIPicture* m_fromCardPic;
	UIPicture* m_toCardPic;
	UILabel* m_fromCardName;
	UILabel* m_toCardName;
	UILabel* m_fromCardAttr;
	UILabel* m_toCardAttr;
	UILabel* m_CardNumLabel;
	UILabel* m_MixCardNumLabel;
	unsigned int m_carId;
	unsigned int m_exchangeId;
	unsigned int m_cardNum;

	bool isComposeFlag;
};

#endif