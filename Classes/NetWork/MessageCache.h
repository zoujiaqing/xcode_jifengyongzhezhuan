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
#ifndef MESSAGE_CACHE_H
#define MESSAGE_CACHE_H

#include <vector>

using namespace google::protobuf;



//--------------------------------------------------------------------
//			cache message info
//--------------------------------------------------------------------
typedef struct  stCacheMessageInfo
{
	MessageHeader*		header;
	Message*			body;
	float				timeOut;
	int					tag;

	bool operator == (const stCacheMessageInfo& v2) const
	{
		return (header == v2.header && body == v2.body && timeOut == v2.timeOut && tag == v2.tag);
	}

}CacheMessageInfo;

typedef std::vector<CacheMessageInfo> ItemVector;


//--------------------------------------------------------------------
//			cache message and send in proper time
//--------------------------------------------------------------------


class MessageCache
{
public:
	void	addMessage(MessageHeader* headerMsg, Message* bodyMsg, float timeout, int tag);

	void	clearCacheMessages();


	void	update(float dt);
 

	//--------------------------------------------------------------------
	//				!!!clean up flag determines whether to free
	//				!!!the header&body message buffer memory
	//--------------------------------------------------------------------
	bool	removeMessage(CacheMessageInfo& message, bool cleanUp);

protected:	
	void		addMessage(CacheMessageInfo& message);
public:
	MessageCache();
	virtual~MessageCache();


private:
	ItemVector	mCacheMessageList;
};




#endif