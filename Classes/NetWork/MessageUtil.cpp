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
#include "MessageUtil.h"

#include "descriptor.h"

#include "cocos2d.h"


MessageUtil* MessageUtil::sInstance =  NULL;



MessageUtil::MessageUtil()	
{
	
}


MessageUtil::~MessageUtil()
{

	//shut down proto buf system
	google::protobuf::ShutdownProtobufLibrary();
}



//////////////////////////////////////////////////////////////////////////
//share instance
//////////////////////////////////////////////////////////////////////////
MessageUtil* MessageUtil::sShareInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new MessageUtil();
	}

	return sInstance;
}

void MessageUtil::Destroy()
{
	if (sInstance)
	{
		delete sInstance;
		sInstance = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
//creator
//////////////////////////////////////////////////////////////////////////
Message* MessageUtil::createMessageInstance(const std::string typeName)
{	

	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
		
			return prototype->New();
		}
	}

	return NULL;
}

MessageHeader* MessageUtil::createMessageHeader()
{
	return (MessageHeader*)createMessageInstance("CSMessageHeader");
}


//////////////////////////////////////////////////////////////////////////
//encode/decode
//////////////////////////////////////////////////////////////////////////
bool MessageUtil::decodeBuffer2Message(Message* msg, const char* buffer, const int length)
{
	//can decode the buffer
	if (msg != NULL)
	{
		return msg->ParseFromArray((const void*)buffer, length);
	}	
	
	return false;
}

bool MessageUtil::decodeString2Message(Message* msg, const std::string& msgStr)
{
	//can decode the buffer
	if (msg != NULL)
	{
		return msg->ParseFromString(msgStr);
	}	

	return false;
}



bool MessageUtil::encodeMessage2Buffer(Message* msg, char*& outBuffer,int& length)
{
	if (msg != NULL)
	{
		//avoid out of memory range
		int byteCount = msg->ByteSize();
		if (length < byteCount)
		{
			delete outBuffer;
			outBuffer = new char[byteCount];			
			length = byteCount;
		}

		return msg->SerializeToArray((void*)outBuffer, length);
	}
	
	return false;
}


bool MessageUtil::encodeMessage2String(Message* msg, std::string& str)
{
	if (msg != NULL)
	{
		int byteCount = msg->ByteSize();
		str.resize(byteCount);		
		return msg->SerializeToString(&str);
	}

	return false;
}

void MessageUtil::printMessage(Message* msg)
{
	if (msg)
	{
		std::string str = msg->DebugString();		
		CCLOG("%s", str.c_str());		
	}
	else
	{
		CCLOG("Message is NULL");
	}
}
