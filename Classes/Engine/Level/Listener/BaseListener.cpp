#include "BaseListener.h"
#include <list>
#include <algorithm>
using namespace std;

using namespace cocos2d;


static ArrayBaseListener  g_sArrayListener;

void BaseListener::_SDM_init_Listener()
{

}

void BaseListener::_SDM_shutdown_Listener()
{
	while(g_sArrayListener.size() > 0)
	{
		BaseListener* item = g_sArrayListener.front();
		g_sArrayListener.erase(g_sArrayListener.begin());
		delete item;
	}


	g_sArrayListener.clear();
}



BaseListener* BaseListener::sInstance = NULL;


BaseListener::BaseListener()
{
	g_sArrayListener.push_back(this);
}


BaseListener::~BaseListener()
{
	g_sArrayListener.remove(this);
}


BaseListener* BaseListener::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new BaseListener();
	}

	return sInstance;
}


void BaseListener::HandleLayerActiveEvent(cocos2d::CCNode* obj, sActiveLevelConfig* config)
{
	CCTargetedTouchDelegate* target = dynamic_cast<CCTargetedTouchDelegate*>(obj);
	if (target)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(target, 0, true);
	}

}


void BaseListener::HandleLayerDeactiveEvent(cocos2d::CCNode* obj)
{

	CCTargetedTouchDelegate* target = dynamic_cast<CCTargetedTouchDelegate*>(obj);
	if (target)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(target);
	}
}
