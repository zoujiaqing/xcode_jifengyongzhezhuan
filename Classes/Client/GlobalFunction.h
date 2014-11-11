#ifndef _GLOBAL_FUNCTION_H_
#define _GLOBAL_FUNCTION_H_
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include <stdio.h>
static const char* IntToStr(int num)
{
    CCString* str = CCString::createWithFormat("%d", num);
    return str->getCString();
}

static const char* GetTimeStr(long _time)
{
    unsigned int hour = _time / 3600;
    unsigned int min = (_time - hour * 3600) / 60;
    unsigned int second = _time % 60;
    CCString * str = CCString::createWithFormat("%02d : %02d : %02d", hour,min,second);

    return str->getCString();
}


#endif