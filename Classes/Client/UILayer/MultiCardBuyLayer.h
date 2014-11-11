#ifndef MULTI_CARD_BUY_LAYER_H
#define MULTI_CARD_BUY_LAYER_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
using namespace TXGUI;
using namespace cocos2d;

class MultiCardBuyLayer : public CCLayer
{
public:
	MultiCardBuyLayer();
	~MultiCardBuyLayer();
	CREATE_FUNC(MultiCardBuyLayer);
	virtual bool init();

	void setCardExchangeId(unsigned int exchangeId);
protected:
	void onAddOneBtClicked(CCObject* obj);
	void onAddTwoBtClicked(CCObject* obj);
	void onAddThreeBtClicked(CCObject* obj);
	void onMinusOneBtClicked(CCObject* obj);
	void onMinusTwoBtClicked(CCObject* obj);
	void onMinusThreeBtClicked(CCObject* obj);

	void onConfirmBtClicked(CCObject* obj);
	void onCancellBtClicked(CCObject* obj);
	void showSpendDiamond(unsigned int cardNum,unsigned int price);
	void showCardNum(unsigned int cardNum);
	void addCardNumToBuy(unsigned int num);
	void minusCardNumToBuy(unsigned int num);
private:
	UILayout* m_uiLayout;
	unsigned int m_currentExchageId;
	unsigned int m_numToBuy;
	unsigned int m_singlePrice;
};

#endif