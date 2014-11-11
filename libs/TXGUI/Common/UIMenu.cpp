#include "UIMenu.h"
#include "MainMenuLayer.h"
#include "ItemDragLayer.h"

#include <cmath>
namespace TXGUI {

	UIMenu::UIMenu()
		:m_lastTouchTime(0)
		, m_lastTouchedItem(NULL)
		,m_doubleTouchListener(NULL)
		,m_doubleTouchHandler(NULL)
		,m_tappedTouchListener(NULL)
		,m_tappedTouchHandler(NULL)
		,m_tapCancelTouchHandler(NULL)
		,m_bDefineDragMove(false)
		,m_touchBeginPoint(CCPointZero)
		,m_pDragHandler(NULL)
		,m_btIconSprite(NULL)
		,m_relativeController(NULL)
		,m_bDoubleTouchFlag(false)
		,m_bTouchActivity(false)
		,m_tappedFlag(false)
	{
		m_dragTimeFlag = 0;
		b_lastStatus = false;
	}

	UIMenu::~UIMenu()
	{

	}

	UIMenu* UIMenu::create()
	{
		return UIMenu::create(NULL, NULL);
	}

	UIMenu* UIMenu::createWithItem(CCMenuItem* item)
	{
		return UIMenu::create(item, NULL);
	}

	UIMenu* UIMenu::create(CCMenuItem* item, ...)
	{
		va_list args;
		va_start(args,item);

		UIMenu *pRet = UIMenu::createWithItems(item, args);

		va_end(args);

		return pRet;
	}

	UIMenu* UIMenu::createWithItems(CCMenuItem* item, va_list args)
	{
		CCArray* pArray = NULL;
		if( item )
		{
			pArray = CCArray::create(item, NULL);
			CCMenuItem *i = va_arg(args, CCMenuItem*);
			while(i)
			{
				pArray->addObject(i);
				i = va_arg(args, CCMenuItem*);
			}
		}

		return UIMenu::createWithArray(pArray);
	}

	UIMenu* UIMenu::createWithArray(CCArray* pArrayOfItems)
	{
		UIMenu *pRet = new UIMenu();
		if (pRet && pRet->initWithArray(pArrayOfItems))
		{
			pRet->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(pRet);
		}

		return pRet;
	}

	void UIMenu::registerWithTouchDispatcher()
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_initPriority, true);
	}

	bool UIMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
	{
		CC_UNUSED_PARAM(event);
		if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !m_bEnabled)
		{
			return false;
		}

		for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
		{
			if (c->isVisible() == false)
			{
				return false;
			}
		}
	
		m_pSelectedItem = this->itemForTouch(touch);
		if (m_pSelectedItem)
		{
			long long currentTime = millisecondNow();
			m_touchBeginPoint = touch->getLocation();
			m_bMoveAway = false;
			m_tappedFlag = false;
			if(m_lastTouchedItem == m_pSelectedItem && currentTime - m_lastTouchTime < 400)
			{ 
				// 双击
				m_bDoubleTouchFlag = true;
			}
			else
			{
				m_bDoubleTouchFlag = false;
			}

			if(m_bTouchActivity)
			{
				m_pSelectedItem->activate();
				//if(m_bDoubleTouchFlag)
				//{
				//	if(m_doubleTouchListener && m_doubleTouchHandler)
				//	{
				//		(m_doubleTouchListener->*m_doubleTouchHandler)(this);
				//	}
				//	else
				//	{ // 没有注册双击，仍然以单击处理
				//		m_pSelectedItem->activate();
				//	}
				//}
				//else
				//{
				//	m_pSelectedItem->activate();

				//}
			}
			
			m_eState = kCCMenuStateTrackingTouch;	
			b_lastStatus = m_pSelectedItem->isSelected();
			m_pSelectedItem->selected();
			m_lastTouchedItem = m_pSelectedItem;
			m_lastTouchTime = currentTime;

			StartAccumulateSleepTime();

			return true;
		}

		return false;
	}

	void UIMenu::StartAccumulateSleepTime()
	{
		m_dragSleepTime = 0;
		schedule(schedule_selector(UIMenu::Update), 0.02f);
	}

	void UIMenu::Update(float ft)
	{
		m_dragSleepTime += (long)(ft * 1000);
		checkMenuTapped();
	}

	void UIMenu::StopAccumulateSleepTime()
	{
		this->unschedule(schedule_selector(UIMenu::Update));
		checkMenuTapCancel();
	}

	void UIMenu::SetDragFlagTime(unsigned int second)
	{
		m_dragTimeFlag = second;
	}

	void UIMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
	{
		CC_UNUSED_PARAM(event);
		CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
		CCMenuItem *currentItem = this->itemForTouch(touch);
		if (currentItem != m_pSelectedItem) 
		{
			if (m_pSelectedItem)
			{
				if(!b_lastStatus)
				{
					m_pSelectedItem->unselected();
				}
			}
			m_pSelectedItem = currentItem;
			if (m_pSelectedItem)
			{
				b_lastStatus = m_pSelectedItem->isSelected();
				m_pSelectedItem->selected();
			}
		}
		
		if(!m_bMoveAway)
		{
			// 还没有动过，开始进行拖动判断
			CCPoint point = touch->getLocation();
			if(std::abs(point.x - m_touchBeginPoint.x) > 40 || std::abs(point.y - m_touchBeginPoint.y) > 40)
			{
				m_bMoveAway = true;
				StopAccumulateSleepTime();
				return;
			}

			if(std::abs(point.x - m_touchBeginPoint.x) > 5 || std::abs(point.y - m_touchBeginPoint.y) > 5)
			{
				if(m_bDefineDragMove && m_pDragHandler && m_pDragHandler->getDragItem())
				{
					long long currentTime = millisecondNow();

					/*if(currentTime - m_lastTouchTime > 100)
					{	*/
						if (m_dragSleepTime > m_dragTimeFlag)
						{
							int tag = MainMenuLayer::getLayerTagByName("ItemDragLayer");
							ItemDragLayer* layer = MainMenuLayer::showUILayerByTag<ItemDragLayer>(tag,true);
							if(layer)
							{
								layer->startDrag(m_pDragHandler,touch,event);
							}
						}			
					//}
				}
			}
		
		}
	}

	void UIMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
	{
		
		CC_UNUSED_PARAM(touch);
		CC_UNUSED_PARAM(event);
		CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");

		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();

			if(!m_bTouchActivity)
			{
				m_pSelectedItem->activate();
				//if(m_bDoubleTouchFlag)
				//{
				//	if(m_doubleTouchListener && m_doubleTouchHandler)
				//	{
				//		(m_doubleTouchListener->*m_doubleTouchHandler)(this);
				//	}
				//	else
				//	{ // 没有注册双击，仍然以单击处理
				//		m_pSelectedItem->activate();
				//	}
				//}
				//else
				//{
				//	m_pSelectedItem->activate();

				//}
			}
		}
		m_eState = kCCMenuStateWaiting;
		StopAccumulateSleepTime();
	}

	void UIMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
	{
		//CCMenu::ccTouchCancelled(touch,event);
		CC_UNUSED_PARAM(touch);
		CC_UNUSED_PARAM(event);
		CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
		if (m_pSelectedItem)
		{
			if(!b_lastStatus)
			{
				m_pSelectedItem->unselected();
			}
		}
		m_eState = kCCMenuStateWaiting;
		StopAccumulateSleepTime();
	}

	long long UIMenu::millisecondNow()  
	{ 
		struct cc_timeval now; 
		CCTime::gettimeofdayCocos2d(&now, NULL);
		return (now.tv_sec * 1000 + now.tv_usec / 1000); 
	} 

	void UIMenu::SetDoubleClickHandler(CCObject* listerner,SEL_MenuHandler handler)
	{
		m_doubleTouchListener = listerner;
		m_doubleTouchHandler = handler;
	}

	void UIMenu::SetTappedHandler(CCObject* listerner,SEL_MenuHandler tapHandler,SEL_MenuHandler tapCancelHandler)
	{
		m_tappedTouchListener = listerner;
		m_tappedTouchHandler = tapHandler;
		m_tapCancelTouchHandler = tapCancelHandler;
	}

	void UIMenu::SetDragMove(bool value, DragReceiver* handler)
	{
		if(m_pDragHandler == handler)
		{
			m_bDefineDragMove = value;
			return;
		}

		m_pDragHandler = handler;
		if(!m_btIconSprite || !m_pDragHandler)
		{
			m_bDefineDragMove =false;
			return;
		}

		m_bDefineDragMove = value;
		if(m_bDefineDragMove)
		{
			m_pDragHandler->SetTouchDelegate(this);
			m_pDragHandler->setDragItem(m_btIconSprite);
		}
	}

	void UIMenu::SetNormalIcon(CCSprite* normalTex)
	{
		m_btIconSprite = normalTex;
		if(m_pDragHandler)
		{
			m_pDragHandler->setDragItem(m_btIconSprite);
		}	
	}

	bool UIMenu::checkReleaseTouchInButton(CCTouch* touch)
	{
		if (! m_bVisible || !m_bEnabled)
		{
			return false;
		}

		for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
		{
			if (c->isVisible() == false)
			{
				return false;
			}
		}

		m_pSelectedItem = this->itemForTouch(touch);
		if (m_pSelectedItem)
		{
			return true;
		}
		return false;
	}

	void UIMenu::checkMenuTapped()
	{
		long long currentTime = millisecondNow();
		if(currentTime - m_lastTouchTime > 500)
		{
			if(!m_tappedFlag)
			{
				if(m_tappedTouchListener && m_tappedTouchHandler)
				{
					(m_tappedTouchListener->*m_tappedTouchHandler)(this);
				}
				m_tappedFlag = true;
			}
		}
	}
	void UIMenu::checkMenuTapCancel()
	{
		if(m_tappedFlag)
		{
			if(m_tappedTouchListener && m_tapCancelTouchHandler)
			{
				(m_tappedTouchListener->*m_tapCancelTouchHandler)(this);
			}
			m_tappedFlag = false;
		}
	}

	CCPoint UIMenu::getTouchPoint()
	{
		return m_touchBeginPoint;
	}
}