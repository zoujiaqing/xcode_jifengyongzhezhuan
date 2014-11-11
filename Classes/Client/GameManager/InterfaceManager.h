// added by seanxiao at 2014/4/10
#ifndef _INTERFACE_MANAGER_H_
#define _INTERFACE_MANAGER_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Singleton_t.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define DIAMOND_ITEM_ID 416002
#define CONTRIBUTE_ITEM_ID 416025

class InterfaceManager : public CCObject
                       , public TSingleton<InterfaceManager>
{
public:
    InterfaceManager();
    virtual ~InterfaceManager();

    CCSpriteFrame* getSpriteFrameFromFile(const char* plistPath, const char* pngName);
    CCSprite* getIconByItemid(int itemid);

    void ShowNOEnoughStaminaBox(CCObject* delegate, SEL_MenuHandler clicked);
    void enterBagLayer(CCObject* sender);
    void enterMallChipPage(CCObject* sender);
    CCObject* m_pSelector;
    SEL_MenuHandler m_OkClicked;
    //SEL_MenuHandler m_CancelClicked;

    CCProgressTimer* CreateProgressTimer(const char* plistPath, const char* pngName);
};

#endif

