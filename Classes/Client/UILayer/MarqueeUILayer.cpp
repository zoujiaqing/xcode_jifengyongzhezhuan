#include "MarqueeUILayer.h"
#include "UIManager.h"
#include "UIPicture.h"
#include "UILayout.h"

MarqueeUILayer::MarqueeUILayer()
{
	m_viewSize = CCSizeZero;
	m_bgPoint = CCPointZero;
}

MarqueeUILayer::~MarqueeUILayer()
{
	//unscheduleUpdate();
	TXGUI::UIManager::sharedManager()->RemoveUILayout("MarqueeLayout");
}

bool MarqueeUILayer::init()
{
	bool bRet = false;
	do 
	{
		// fo test
		//m_viewSize = CCSizeMake(600,400);
		//CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
		//m_bgPoint = ccp(winSize.width / 2,400);
		TXGUI::UILayout* layout = TXGUI::UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/marqueeUI.plist",this, "MarqueeLayout");
		TXGUI::UIPicture* pic = layout->FindChildObjectByName<TXGUI::UIPicture>("frameBG");
		m_viewSize = pic->getSpriteSize();
		m_bgPoint = pic->getPosition();
		pic->getCurrentNode()->setVisible(false);
		//this->scheduleUpdate();
		bRet = true;
	} while (0);
	return bRet;
}

//void MarqueeUILayer::update(float fDelta)
//{
//
//}

void MarqueeUILayer::visit()
{
	//glEnable(GL_SCISSOR_TEST);

	//float deviceScale = CCDirector::sharedDirector()->getContentScaleFactor();


	//glScissor(m_bgPoint.x * deviceScale - m_viewSize.width / 2 * deviceScale,
	//	m_bgPoint.y * deviceScale - m_viewSize.height / 2 * deviceScale,
	//	m_viewSize.width * deviceScale,
	//	m_viewSize.height * deviceScale);


	CCLayer::visit();
	//glDisable(GL_SCISSOR_TEST);
}