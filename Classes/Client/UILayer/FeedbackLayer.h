#ifndef FeedbackLayer_LAYER_H
#define FeedbackLayer_LAYER_H

#include "ASprite.h"
#include "UILayout.h"
#include "UIScrollList.h"

using namespace std;
using namespace cocos2d;
using namespace TXGUI;

class FeedbackLayer : public cocos2d::CCLayer
{
public:
	FeedbackLayer();
	virtual ~FeedbackLayer();

	CREATE_FUNC(FeedbackLayer);
	virtual bool init();

	static FeedbackLayer* Get();

	//°´¼üÏìÓ¦
	virtual void callbackHotSpot(CCObject *sender);
	virtual void callbackOnline(CCObject *sender);
	virtual void callbackForum(CCObject *sender);
	virtual void callbackReply(CCObject *sender);
	
	void callbackWeb(char * url);
	virtual void callbackClose(CCObject *sender);
private:
	void createLayout();
	void callPlatformFunc(int funid);
	TXGUI::UILayout* layout;
	static FeedbackLayer* instance;
#if 0
	TXGUI::UIButton* btnHotSpot;
	TXGUI::UIButton* btnOnline;
	TXGUI::UIButton* btnForum;
	TXGUI::UIButton* btnReply;

	TXGUI::UILabel *lblHotSpot;
	TXGUI::UILabel *lblOnline;
	TXGUI::UILabel *lblForum;
	TXGUI::UILabel *lblReply;

#endif
};

#endif
