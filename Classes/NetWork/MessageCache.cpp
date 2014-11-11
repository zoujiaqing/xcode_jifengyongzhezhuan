// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#include "NetWorkUtil.h"
#include "MessageCache.h"
#include "OnlineNetworkManager.h"
#include <algorithm>
#include <vector>
using namespace std;


MessageCache::MessageCache()
{

}

MessageCache::~MessageCache()
{
	this->clearCacheMessages();
}


//--------------------------------------------------------------------
//			cache message
//--------------------------------------------------------------------
void MessageCache::addMessage(MessageHeader* headerMsg, Message* bodyMsg, float timeout, int tag)
{
	CacheMessageInfo message;
	message.header = headerMsg;
	message.body = bodyMsg;
	message.timeOut = timeout;
	message.tag = tag;

	addMessage(message);
}


void MessageCache::addMessage(CacheMessageInfo& message)
{
	// mCacheMessageList.push_back(message);

    KSendMessageResult result = OnlineNetworkManager::sShareInstance()->sendMessage(message.header, 
        message.body,
        message.timeOut,
        message.tag);
}

//--------------------------------------------------------------------
//			remove cache message
//--------------------------------------------------------------------
bool MessageCache::removeMessage(CacheMessageInfo& message, bool cleanUp)
{
	ItemVector::iterator itor  =  std::find(mCacheMessageList.begin(), mCacheMessageList.end(), message);
	if ( itor != mCacheMessageList.end())
	{
		if (cleanUp)
		{
			delete message.header;
			delete message.body;
		}	

		mCacheMessageList.erase(itor);
		return true;
	}

	return false;
}


void MessageCache::clearCacheMessages()
{
	std::vector<CacheMessageInfo>::iterator itor = mCacheMessageList.begin();
	for (; itor != mCacheMessageList.end(); itor++)
	{
		CacheMessageInfo& message = *itor;
		delete message.header;
		delete message.body;
	}

	mCacheMessageList.clear();
}


//--------------------------------------------------------------------
//			auto send cache message
//			!!!send only one message per frame
//--------------------------------------------------------------------
void MessageCache::update(float dt)
{
    return;

	//connect 2 sever already
	if (OnlineNetworkManager::sShareInstance()->getSocketStatus() == KSocketConnecting)
	{
		CCDirector::sharedDirector()->setMsgCacheCount(mCacheMessageList.size());
        
        vector<CacheMessageInfo> succList;
        
#ifdef USE_BOOST_ASIO
        for (size_t i = 0; i < mCacheMessageList.size(); ++i)
        {
            CacheMessageInfo& message = mCacheMessageList[i];
            KSendMessageResult result = OnlineNetworkManager::sShareInstance()->sendMessage(message.header, 
                message.body,
                message.timeOut,
                message.tag);
        }

        mCacheMessageList.clear();
#else
		std::vector<CacheMessageInfo>::iterator itor = mCacheMessageList.begin();
		for (; itor != mCacheMessageList.end();itor++)
		{
			CacheMessageInfo& message = *itor;
			KSendMessageResult result = OnlineNetworkManager::sShareInstance()->sendMessage(message.header, 
				message.body,
				message.timeOut,
				message.tag,
				false,
				false);
			if (result == KResultSuccess)
			{
                succList.push_back(message);
				//removeMessage(message, true);
			}
			//send only one message per frame
			//return;
		}
        
#endif

        //CCLOG("============CACHE %d, SUCCESS %d===========",mCacheMessageList.size(), succList.size());
        vector<CacheMessageInfo>::iterator itor2 = succList.begin();
        for (; itor2 != succList.end();itor2++)
        {
            removeMessage(*itor2, true);
        }
	}
}