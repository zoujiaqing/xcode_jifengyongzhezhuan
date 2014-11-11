#include "CCheckBox.h"
#include "HelloWorldScene.h"
USING_NS_CC;

CCCheckBox::CCCheckBox(void) :
	m_normalPng(NULL),
	m_selectedPng(NULL),
	m_status(false),
	m_owner(NULL),
    m_norFrame(NULL),
    m_selFrame(NULL),
    m_touchPrioroty(0)
{

}

CCCheckBox::~CCCheckBox(void)
{
}

void CCCheckBox::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_touchPrioroty, true);
	CCSprite::onEnter();
}

void CCCheckBox::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}  

bool CCCheckBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (! isTouchInside(pTouch))
		return false;

	return true;
}

void CCCheckBox::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

}

void CCCheckBox::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_status)
    {
        m_status = false;

        if (m_normalPng)
        {
            initWithFile(m_normalPng);
        }

        if (m_norFrame)
        {
            initWithSpriteFrame(m_norFrame);
        }
    }
    else
    {
        m_status = true;

        if (m_selectedPng)
        {
            initWithFile(m_selectedPng);
        }

        if (m_selFrame)
        {
            initWithSpriteFrame(m_selFrame);
        }
    }

    if (m_pfnSelector)
        (m_owner->*m_pfnSelector)(m_status);
} 

CCCheckBox* CCCheckBox::create(const char * nor, const char * sel, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty)
{
	CCCheckBox* pRet = new CCCheckBox();

	if (pRet && pRet->init(nor, sel, selector, owner, touchPrioraty))
	{
		pRet->autorelease();
	} 
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

CCCheckBox* CCCheckBox::create(CCSpriteFrame* norFrame, CCSpriteFrame* selFrame, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty)
{
    CCCheckBox* pRet = new CCCheckBox();
    if (pRet && pRet->init(norFrame, selFrame, selector, owner, touchPrioraty))
    {
        pRet->autorelease();
    } else
    {
        delete pRet;
        pRet = NULL;
    }

    return pRet;
}

bool CCCheckBox::init(const char* nor, const char* sel, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty)
{
	m_normalPng = nor;
	m_selectedPng = sel;
	m_pfnSelector = selector;
	m_owner = owner;
    m_norFrame = NULL;
    m_selFrame = NULL;
    m_touchPrioroty = touchPrioraty;

    // position the label on the center of the screen
    //m_label->setPosition(ccp(-10, -10));

    //this->addChild(m_label, 1);

	return initWithFile(nor);
}

bool CCCheckBox::init(CCSpriteFrame* norFrame, CCSpriteFrame* selFrame, SEL_CheckBoxHandler selector, CCObject* owner, int touchPrioraty)
{
    m_normalPng = NULL;
    m_selectedPng = NULL;
    m_pfnSelector = selector;
    m_owner = owner;
    m_norFrame = norFrame;
    m_selFrame = selFrame;
    m_touchPrioroty = touchPrioraty;

    return initWithSpriteFrame(norFrame);
}

bool CCCheckBox::isTouchInside(CCTouch* touch)
{
    CCPoint touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    CCRect bBox = boundingBox();
    return bBox.containsPoint(touchLocation);
}

void CCCheckBox::setClickState(bool status)
{
    if (m_status == status)
    {
        return;
    }

    m_status = status;
    if (m_status == false)
    {
        if (m_normalPng)
        {
            initWithFile(m_normalPng);
        }

        if (m_norFrame)
        {
            initWithSpriteFrame(m_norFrame);
        }
    }
    else
    {
        if (m_selectedPng)
        {
            initWithFile(m_selectedPng);
        }

        if (m_selFrame)
        {
            initWithSpriteFrame(m_selFrame);
        }
    }

    if (m_pfnSelector)
        (m_owner->*m_pfnSelector)(m_status);
}

bool CCCheckBox::getClickState()
{
	return m_status;
}