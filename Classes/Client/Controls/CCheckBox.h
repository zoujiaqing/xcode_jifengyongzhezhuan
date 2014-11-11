#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
//using namespace cocos2d::extension;
USING_NS_CC;

typedef void (CCObject::*SEL_CheckBoxHandler)(bool);
#define checkbox_selector(_SELECTOR) (SEL_CheckBoxHandler)(&_SELECTOR)

class CCCheckBox : public CCSprite, public CCTargetedTouchDelegate
{
public:
    CCCheckBox(void);
    virtual ~CCCheckBox(void);

    static CCCheckBox* create(const char* nor, const char* sel, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty = 0);
    static CCCheckBox* create(CCSpriteFrame* norFrame, CCSpriteFrame* selFrame, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty = 0);

    virtual void onEnter();
    virtual void onExit();
    bool init(const char* nor, const char* sel, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty = 0);
    bool init(CCSpriteFrame* norFrame, CCSpriteFrame* selFrame, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty = 0);

    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

    bool isTouchInside(CCTouch* touch);
    void setClickState(bool status);

    bool getClickState();

private:
	bool m_status;
	const char* m_normalPng;
	const char* m_selectedPng;
	SEL_CheckBoxHandler m_pfnSelector;
	CCCheckBox* m_label;
	CCObject* m_owner;

    CCSpriteFrame* m_norFrame;
    CCSpriteFrame* m_selFrame;
    int m_touchPrioroty;
};



