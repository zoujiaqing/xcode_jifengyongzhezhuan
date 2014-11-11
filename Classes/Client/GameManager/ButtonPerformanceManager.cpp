#include "ButtonPerformanceManager.h"

ButtonPerformanceManager::ButtonPerformanceManager()
{
    m_totalButtonList.clear();
}

ButtonPerformanceManager::~ButtonPerformanceManager()
{

}

void ButtonPerformanceManager::addPerformanceButton(CCLayer* layer, CCControlButton* button, int flag)
{
    unsigned int uuid = layer->m_uID;

    if (m_totalButtonList.find(uuid) == m_totalButtonList.end())
    {
        std::map<int, CCControlButton*> imap;
        imap.clear();
        m_totalButtonList.insert(std::make_pair(uuid, imap));
    }

    m_totalButtonList[uuid][flag] = button;
}

void ButtonPerformanceManager::removePerformanceButton(CCLayer* layer)
{
    unsigned int uuid = layer->m_uID;
    
    if (m_totalButtonList.find(uuid) == m_totalButtonList.end())
    {
        return;
    }

    m_totalButtonList.erase(uuid);
}

void ButtonPerformanceManager::PerformanceIndexButton(CCLayer* layer, int flag)
{
    unsigned int uuid = layer->m_uID;

    std::map<unsigned int, std::map<int, CCControlButton*> >::iterator it;
    it = m_totalButtonList.find(uuid);

    if (it == m_totalButtonList.end())
    {
        return;
    }

    std::map<int, CCControlButton*> mapButtonList = it->second;

    // ±éÀú
    std::map<int, CCControlButton*>::iterator itButton;
    for (itButton = mapButtonList.begin(); itButton != mapButtonList.end(); ++itButton)
    {
        int indexFlag = itButton->first;
        CCControlButton* button = itButton->second;
        if (indexFlag == flag)
        {
            button->setHighlighted(true);
            button->setEnabled(false);
        } else
        {
            button->setEnabled(true);
            button->setHighlighted(false);
        }
		
    }
}


