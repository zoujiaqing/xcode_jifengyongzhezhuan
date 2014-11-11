#include "DragReceiver.h"

namespace TXGUI {
	// DragReceiverRegister
	DragReceiverRegister* DragReceiverRegister::pSharedManager = NULL;

	DragReceiverRegister::DragReceiverRegister()
	{
		m_pDragReceivers = new std::list<DragReceiver*>();
	}

	DragReceiverRegister::~DragReceiverRegister()
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

	DragReceiverRegister* DragReceiverRegister::sharedManager()
	{
		if(!pSharedManager)
		{
			pSharedManager = new DragReceiverRegister();
		}
		return pSharedManager;
	}

	void DragReceiverRegister::Destroy()
	{
		if(pSharedManager)
		{
			CC_SAFE_DELETE(pSharedManager);
		}
	}

	void DragReceiverRegister::registerDragReceiver(DragReceiver* receiver)
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
				if(receiver->getPriority() <= (*it)->getPriority())
				{
					break;
				}
				it++;
			}
			if(it != m_pDragReceivers->end() && *it == receiver)
			{
				return;
			}
			m_pDragReceivers->insert(it,receiver);
		}
	}

	void DragReceiverRegister::unregisterDragReceiver(DragReceiver* receiver)
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
					m_pDragReceivers->remove(*it++);
				}else
				{
					it++;
				}
			}
		}
	}

	// DragReceiver
	DragReceiver::~DragReceiver()
	{

	}

	DragReceiver::DragReceiver(int priority,DragHandlerType type,int pos)
		:m_priority(priority)
		,m_type(type)
		,m_handlerPos(pos)
		,m_checkFunc(NULL)
		,m_target(NULL)
		,m_pDragItem(NULL)
		,m_pTouchDelegate(NULL)
	{

	}

	void DragReceiver::setPriority(int priority)
	{

	}

	void DragReceiver::setCheckDragFunc(CheckDragFunc function,CCObject* obj)
	{
		m_checkFunc = function;
		m_target = obj;
	}

	bool DragReceiver::checkDragIn(CCTouch* touch)
	{
		if(m_target && m_checkFunc)
		{
			return (m_target->*m_checkFunc)(touch);
		}
		return false;
	}
}