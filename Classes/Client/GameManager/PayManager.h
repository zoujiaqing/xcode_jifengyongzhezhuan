#pragma once
#include "Singleton_t.h"
#include "cocos2d.h"

USING_NS_CC;


class PayManager : public TSingleton<PayManager>,public CCObject
{
public:
	PayManager();
	~PayManager();
	void payForProduct(int, int);
	void loadPlugin();
	void unloadPlugin();
private:
    

};
