#ifndef DIAMOND_WARNING_LAYER_H
#define DIAMOND_WARNING_LAYER_H

#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

class DiamondWarningLayer : public cocos2d::CCLayer
{
public:	
	DiamondWarningLayer();
	virtual ~DiamondWarningLayer();

	CREATE_FUNC(DiamondWarningLayer);
	virtual bool init();

	static DiamondWarningLayer* getInstance();

	static void Show(const char* message_body,unsigned int costDiamond, SEL_MenuHandler pfnOkClicked,
		SEL_MenuHandler pfnCancelClicked ,CCObject * handler , ccColor3B labelColor = ccWHITE,const char* tipContent = "",const char* topTipContent = "");

	static void Show(const char* message_body,unsigned int costDiamond,int pfnOkHandler,
		int pfnCancelClicked,ccColor3B labelColor = ccWHITE,const char* tipContent = "",const char* topTipContent = "");

	static void SetTipContentColor(const char* tipContent, ccColor3B tipColor);

	void onConfirmBtClicked(CCObject* obj);

	void onCancelBtClicked(CCObject* obj);
private:

	UILayout* m_uiLayout;
	UILabel* m_warninglabel;
	UILabel* m_diamondLabel;
	UILabel* m_tipContentLabel;
	UILabel* m_topTipContentLabel;
	UIButton* m_okBtn;
	UIButton* m_cancelBtn;

	CCObject* m_clickEventHander;

	SEL_MenuHandler m_pfnOkClicked;
	SEL_MenuHandler m_pfnCancelClicked;
};

#endif