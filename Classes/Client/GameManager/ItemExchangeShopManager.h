#pragma once

#include "Singleton_t.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <iostream>
USING_NS_CC;
USING_NS_CC_EXT;

class ItemExchangeShopManager : public CCObject
                  , public TSingleton<ItemExchangeShopManager>
{
public:
    ItemExchangeShopManager();
    virtual ~ItemExchangeShopManager();

    // send 

    // receive

    // interface
    void InitXMLData();
private:
    CC_SYNTHESIZE(CCPoint, m_headScrollPos, HeadScrollPos);
};