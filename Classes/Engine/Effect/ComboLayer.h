#ifndef COMBO_LAYER_H
#define COMBO_LAYER_H

#include "cocos2d.h"
using namespace cocos2d; 

#define MAX_COMBO_NUMBER 999

class ComboLayer : public cocos2d::CCLayer
{
	enum ComboState
	{
		NONO_COMBO,
		COMBO_COME_IN,
		COMBO_COME_OUT,
		COMBO_SHOWING,
	};

public:
	ComboLayer();
	virtual ~ComboLayer();
	CREATE_FUNC(ComboLayer);

	virtual bool init();

	void HandleComboIncrease(unsigned int incremental);
	void onComboMoveInOver(CCNode* sender);
	void onComboMoveOutOver(CCNode* sender);
	void onComboScaleOver(CCNode* sender);
	unsigned int GetMaxComboNumber(){return m_maxComboNumber;}
protected:
	virtual void update(float time);

private:
	// combo滑入屏幕
	void comboComeIn();
	
	// combo显示中更新数字
	void updateComboNum();

	// combo滑出屏幕
	void comboComeOut();

	const char* getFontName(unsigned int num);
private:
	CCNode* m_comboNode;
	CCLabelAtlas* m_comboLabel;
	CCLabelAtlas* m_comboFrontLabel;
	unsigned int m_comboNumber; // 连击数
	unsigned int m_comboShow;	// 显示的连击数
	long long m_lastHitTime;
	ComboState m_comboState;

	CCPoint m_hidingPoint;	// combo滑入起始位置
	CCPoint m_ExtraPoint;   // combo滑入时最左突出的位置
	CCPoint m_showingPoint;	// combo显示时停留位置

	unsigned int m_maxComboNumber; // 最大连击数
};

#endif