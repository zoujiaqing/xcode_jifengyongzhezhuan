#include "InterfaceManager.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
InterfaceManager::InterfaceManager()
{
     m_pSelector = NULL;
     m_OkClicked = NULL;
     //m_CancelClicked = NULL;
}

InterfaceManager::~InterfaceManager()
{

}

CCSpriteFrame* InterfaceManager::getSpriteFrameFromFile(const char* plistPath, const char* pngName)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(plistPath);
    return cache->spriteFrameByName(pngName);
}

CCSprite* InterfaceManager::getIconByItemid(int itemid)
{
    CCSprite* sprite = CCSprite::create();
    switch (itemid)
    {
    case DIAMOND_ITEM_ID:
        sprite->setDisplayFrame(getSpriteFrameFromFile("ccbResources/market.plist", "5.png"));
        break;
    case CONTRIBUTE_ITEM_ID:
        sprite->setDisplayFrame(getSpriteFrameFromFile("ccbResources/guild.plist", "guild_pres.png"));
        break;
    default:
        break;
    }

    return sprite;
}

void InterfaceManager::ShowNOEnoughStaminaBox(CCObject* delegate, SEL_MenuHandler clicked)
{
	MessageBox::Show(Localizatioin::getLocalization("M_NO_ENOUGH_STAMINA"), this, menu_selector(InterfaceManager::enterBagLayer), menu_selector(InterfaceManager::enterMallChipPage),MB_MIDDLECLOSE);
    MessageBox::AdjustMiddleStyle(Localizatioin::getLocalization("M_BUY_BREAD"));
    MessageBox::showCloseBtn();
    m_pSelector = delegate;
    m_OkClicked = clicked;
    //m_CancelClicked = canceled;
}

void InterfaceManager::enterBagLayer(CCObject* sender)
{
    (this->m_pSelector->*m_OkClicked)(sender);
}

void InterfaceManager::enterMallChipPage(CCObject* sender)
{
    //(this->m_pSelector->*m_CancelClicked)(sender);
}

CCProgressTimer* InterfaceManager::CreateProgressTimer(const char* plistPath, const char* pngName)
{
    CCSpriteFrame* frame = getSpriteFrameFromFile(plistPath, pngName);
    if(!frame)
    {
        return NULL;
    }
    CCSprite* sprite = CCSprite::createWithSpriteFrame(frame);
    if(!sprite)
    {
        return NULL;
    }
    CCProgressTimer* timer = CCProgressTimer::create(sprite);
    if(!timer)
    {
        return NULL;
    }
    return timer;
}