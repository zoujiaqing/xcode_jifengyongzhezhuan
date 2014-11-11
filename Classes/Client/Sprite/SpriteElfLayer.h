#pragma once

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

class SpriteElfLayer : public CCLayer
{    
public:
	SpriteElfLayer();
	virtual ~SpriteElfLayer();

	CREATE_FUNC(SpriteElfLayer);
	virtual bool init();

public:
	virtual void setTouchEnabled(bool value);
protected:
	/// callback of close button
	void   OnClose(CCObject *sender);

	void   OnClickListItem(CCObject* sender);
protected:
	/// room list
	UIScrollList* m_pList;

	TXGUI::UILayout* m_pLayer;
};