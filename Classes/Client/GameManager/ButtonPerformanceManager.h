#pragma once

#include "Singleton_t.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ButtonPerformanceManager :
        public TSingleton<ButtonPerformanceManager>, 
        public CCObject
{
public :
    ButtonPerformanceManager();
    virtual ~ButtonPerformanceManager();

    void removePerformanceButton(CCLayer* layer);
    void addPerformanceButton(CCLayer* layer, CCControlButton* button, int flag);
    void PerformanceIndexButton(CCLayer* layer, int flag);

private :
    std::map<unsigned int, std::map<int, CCControlButton*> > m_totalButtonList;
};