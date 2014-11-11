#include "UIDragLayer.h"
#include "DragReceiver.h"
#include "UIDefine.h"
#include "UIManager.h"
namespace TXGUI
{
	UIDragLayer::UIDragLayer()
		:b_active(false),
		m_pDragItem(NULL),
		m_currentTouch(NULL),
		m_target(NULL),
		m_dragMoveFunc(NULL),
		m_dragDeleteFunc(NULL),
		m_pDragReceivers(NULL)
	{

	};

	UIDragLayer::~UIDragLayer()
	{
		if(m_pDragReceivers)
		{
			for(std::list<DragReceiver*>::iterator it = m_pDragReceivers->begin();
				it != m_pDragReceivers->end(); ++it)
			{
				CC_SAFE_DELETE(*it);
			}
			m_pDragReceivers->clear();
			CC_SAFE_DELETE(m_pDragReceivers);
		}
	}

	bool UIDragLayer::init()
	{
		if(!CCLayer::init())
		{
			return false;
		}

		m_pDragReceivers = new std::list<DragReceiver*>();
		setTouchEnabled(true);
		return true;
	}

	CCSprite* UIDragLayer::deepClone(CCSprite* target)
	{
		if(!target)
		{
			return NULL;
		}

		CCSprite* sprite = NULL;
		CCTexture2D* texture = target->getTexture();
		if(texture)
		{
			sprite = CCSprite::createWithTexture(texture);
		}
		else
		{
			sprite = CCSprite::create();
		}

		bool isFlipX = target->isFlipX();
		bool isFlipY = target->isFlipY();
		sprite->setFlipX(isFlipX);
		sprite->setFlipY(isFlipY);
		sprite->setTextureRect(target->getTextureRect());
		sprite->setRotation(target->getRotation());
		sprite->setAnchorPoint(target->getAnchorPoint());
		sprite->setContentSize(target->getContentSize());
		sprite->setPosition(target->getPosition());
		sprite->setDirty(target->isDirty());
		sprite->setScale(target->getScale());

		int count = target->getChildrenCount();
		if(count > 0)
		{
			CCArray* array = target->getChildren();
			CCObject* obj;
			CCARRAY_FOREACH(array,obj)
			{		
				CCSprite* targetChild = dynamic_cast<CCSprite*>(obj);
				if(targetChild)
				{
					CCSprite* child = deepClone(targetChild);
					sprite->addChild(child);
					child->setPosition(targetChild->getPosition());
				}
			}
		}
		return sprite;
	}

	bool UIDragLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(b_active)
		{
			if(m_currentTouch == NULL)
			{
				m_currentTouch = pTouch;
			}
			else
			{
				return true;
			}
			if(m_currentDragHandler)
			{
				CCSprite* sprite = m_currentDragHandler->getDragItem(); 
				if(sprite)
				{
					if(m_pDragItem)
					{
						this->removeChild(m_pDragItem);
					}
					//CCObject* copyObj = sprite->copy();
					//m_pDragItem = dynamic_cast<CCSprite*>(copyObj);
					m_pDragItem = deepClone(sprite);
					if(m_pDragItem)
					{
						addChild(m_pDragItem);
						CCPoint pt = pTouch->getLocation();
						m_pDragItem->setPosition(pt);
						//m_pDragItem->setPosition(ccp(pt.x - sprite->getContentSize().width * 0.5f,pt.y - sprite->getContentSize().height * 0.5f));
						float scale = UIManager::sharedManager()->getScaleFactor() * 2.0f;
						m_pDragItem->setScale(scale);
					}
				}
				else
				{
					m_pDragItem = NULL;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void UIDragLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
	{
		if(m_currentTouch != pTouch)
		{
			return;
		}
		if(m_pDragItem)
		{
			CCPoint pt = pTouch->getLocation();
			m_pDragItem->setPosition(pt);
		}
	}

	void UIDragLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
	{
		if(m_currentTouch == pTouch)
		{
			b_active = false;
			if(m_pDragItem)
			{
				this->removeChild(m_pDragItem);
			}
			m_currentTouch = NULL;
		}

	}

	void UIDragLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
	{
		if(m_currentTouch == pTouch)
		{
			b_active = false;
			if(m_pDragItem)
			{
				this->removeChild(m_pDragItem);
			}
			bool claim = false;
			std::list<DragReceiver*>* dragReceivers = DragReceiverRegister::sharedManager()->getReceivers();
			for(std::list<DragReceiver*>::iterator it = dragReceivers->begin();
				it != dragReceivers->end();++it)
			{
				claim = (*it)->checkDragIn(pTouch);
				if(claim)
				{
					DragReceiver* receiver = *it;
					//dealWithDragAction(m_currentDragHandler,receiver);
					if(m_target && m_dragMoveFunc)
					{
						(m_target->*m_dragMoveFunc)(m_currentDragHandler,receiver);
					}
					break;
				}
			}
			if(!claim)
			{
				//dragEndWithoutUI(m_currentDragHandler);
				if(m_target && m_dragDeleteFunc)
				{
					(m_target->*m_dragDeleteFunc)(m_currentDragHandler);
				}
			}
			m_currentTouch = NULL;
		}
	}

	void UIDragLayer::registerWithTouchDispatcher()
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCLayerTouchPriority, true);
	}

	void UIDragLayer::registerDragReceiver(DragReceiver* receiver)
	{
		if(!receiver)
		{
			return;
		}
		//std::list<DragReceiver*>* dragReceivers = DragReceiverRegister::sharedManager()->getReceivers();
		if(m_pDragReceivers)
		{
			std::list<DragReceiver*>::iterator it = m_pDragReceivers->begin();
			while(it != m_pDragReceivers->end())
			{
				if(receiver->getPriority() < (*it)->getPriority())
				{
					break;
				}
			}
			m_pDragReceivers->insert(it,receiver);
		}
	}

	void UIDragLayer::unregisterDragReceiver(DragReceiver* receiver)
	{
		if(!receiver)
		{
			return;
		}

		if(m_pDragReceivers)
		{
			std::list<DragReceiver*>::iterator it = m_pDragReceivers->begin();
			while(it != m_pDragReceivers->end())
			{
				if(receiver == *it)
				{
					m_pDragReceivers->remove(receiver);
				}
			}
		}
	}

	void UIDragLayer::startDrag(DragReceiver* handler,CCTouch* pTouch,CCEvent *pEvent)
	{
		m_currentDragHandler = handler;
		b_active = true;
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->resetTouchWithDelegate(pTouch,pEvent,this);
	}

	void UIDragLayer::setDragDealTarget(CCObject* obj,DragMoveFunc moveFunc,DragDeleteFunc deleteFunc)
	{
		m_target = obj;
		m_dragDeleteFunc = deleteFunc;
		m_dragMoveFunc = moveFunc;
	}
}
