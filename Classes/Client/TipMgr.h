//
//  TipMgr.h
//  HelloLua
//
//  Created by rickycheng  on 13-5-6.
//
//

#ifndef HelloLua_TipMgr_h
#define HelloLua_TipMgr_h

#include "Singleton_t.h"
#include "cocos2d.h"
#include "TXGUIHeader.h"
#include "HtmlNode.h"

class TipMgr : public TSingleton<TipMgr>,cocos2d::CCNode
{
public:
	TipMgr();
	virtual ~TipMgr();
    
    void createTip(cocos2d::CCSprite* icon, cocos2d::CCPoint pt);
    void add(cocos2d::CCSprite* labelIcon, const char* message, cocos2d::ccColor3B color, int fontsize);
    void add(const char* message,cocos2d::ccColor3B color, int fontsize);
	HtmlNode* addPart(const char *message, cocos2d::ccColor3B color, int fontsize);
	void addPart(HtmlNode* node, const char *message, cocos2d::ccColor3B color, int fontsize,bool isFinish);
    void addNodeDone();
    void destroyTip();
	void removeTipAndTouchLayer();

	void setOKButton(cocos2d::CCObject*	target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible = true);
	void setCancellButton(cocos2d::CCObject* target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible = true);
	void setMiddleButton(cocos2d::CCObject*	target,cocos2d::SEL_MenuHandler handler,const char* text,bool isVisible = true);

	void setOKLuaHandle(int hander,const char* text,bool isVisible);
	void setCancellLuaHandle(int hander,const char* text,bool isVisible);
	void setMiddleLuaHandle(int hander,const char* text,bool isVisible);

private:
    void divMessage(const char* message);
    void posNode();
    //void computerPt(cocos2d::CCPoint pt);
    
    void OnOK(cocos2d::CCObject *sender);
    void OnCancel(cocos2d::CCObject *sender);
	void OnMiddleBtClicked(cocos2d::CCObject *sender);
    
	float getMarqueePoint();

	void checkTipLayer();
private:
    cocos2d::CCLayer* m_pColorLayer;
    cocos2d::CCSprite* m_pIcon;
    cocos2d::CCArray* m_pChildNode;
    cocos2d::CCSprite* m_bkSprite;
    cocos2d::CCPoint m_pt;
	cocos2d::CCLayer *m_touchLayer;
    
	UIButton* pbtn_ok;
	UIButton* pbtn_cancle;
	UIButton* pbtn_middle;
	UILabel* label_ok;
	UILabel* label_cancle;
	UILabel* label_middle;

	cocos2d::CCObject*	m_okTarget;
	cocos2d::CCObject*	m_cancellTarget;
	cocos2d::CCObject*	m_middleTarget;
	cocos2d::SEL_MenuHandler m_okHandler;
	cocos2d::SEL_MenuHandler m_cancellHander;
	cocos2d::SEL_MenuHandler m_middleHander;

	CCPoint m_marqueePoint;

	int m_luaOkHandler;
	int m_luaCanellHander;
	int m_luaMiddleHander;

    int m_childcnt;
    float m_maxWidth;
    float m_maxHeight;
    float m_tapx, m_tapy;
	float m_delta_x;
	float m_delta_y;
	float m_start_x;
	float m_start_y;
	float m_maxContentSize; // 文本最大宽度
};

#endif
