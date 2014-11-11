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
#include "MessageFilter.h"
#include "explorer.h"
#include "OnlineNetworkManager.h"
#include "LuaTinkerManager.h"
#include "LevelManager.h"
#include "../Server/common/detect.hpp"
#include "head.pb.h"

//#define USE_THREAD

#ifdef USE_THREAD
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

boost::mutex io_mutex;

#endif



char* g_buffer = NULL;
int   g_curPos = 0;

/// handle data send from virtual server!
int VirtualDataReceive(const void* const data, int len)
{
#ifdef USE_THREAD
	boost::mutex::scoped_lock lock(io_mutex);
#endif

	if (g_curPos + len < 409600)
	{
		memcpy((g_buffer + g_curPos), data, len);
		g_curPos += len;
		return len;
	}else
	{
		CCAssert(0, "Local Server Buff Full");
		return 0;
	}
}


MessageFilter::MessageFilter() : 
m_pExploer(NULL),
m_bRunning(false),
isInited(false),
userInfoInited(false)
{
	//创建空的技能包
	Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSGetUserSkillRsq");
	userSkill = (CSGetUserSkillRsp *)msg;

	Message* infoMsg = MessageUtil::sShareInstance()->createMessageInstance("BasicUserInfo");
	userInfo = (BasicUserInfo *)infoMsg;
}

void MessageFilter::setServerBasicUserInfo(BasicUserInfo info)
{   
	if (m_pExploer)
	{
		m_pExploer->set_info(info);
	}
	*userInfo = info;
	userInfoInited = true;
   
}

void MessageFilter::setServerUserSkill(CSGetUserSkillRsp skill)
{
	
	if (m_pExploer)
	{
		m_pExploer->set_skill(skill);
	}
	
	//*userSkill = skill;
}

bool MessageFilter::Init()
{
	if (!userInfoInited)
	{
		CCAssert(0, "uninited user info for local server");
		return false;
	}
	/// construct explorer
	m_pExploer = new Explorer(*userInfo, *userSkill, VirtualDataReceive);
	
	g_buffer = new char[409600];
	
	isInited = true;

#ifdef USE_THREAD
	boost::thread thrd1(boost::bind(&MessageFilter::threadClock, this));;
	thrd1.join();
#endif

	return NULL != m_pExploer;
}

void MessageFilter::Uninit()
{
	isInited = false;
}

MessageFilter::~MessageFilter()
{
	delete m_pExploer;
	delete g_buffer;
	if(userSkill)
	{
		delete userSkill ;
		userSkill = NULL ;
	}
	if(userInfo)
	{
		delete userInfo ;
		userInfo = NULL ;
	}
	m_pExploer = NULL;
	g_buffer = NULL;
}

void MessageFilter::resetValue()
{
	delete m_pExploer;
	delete g_buffer;

	m_pExploer = NULL;
	g_buffer = NULL;
	g_curPos = 0;

	isInited = false;
}

void MessageFilter::clock()
{
	//TIMEUSPEND.time_begin();
	if (g_curPos > 0)
	{
		int ret = MessageFilter::Get()->PickBufferData(g_buffer, g_curPos);
		CCAssert(ret > 0, "MessageFilter::PickBufferData Error!");
	}
	//TIMEUSPEND.time_end("MessageFilter:clock:PickBufferData");

	/// tick explorer 
	if (m_pExploer)
	{
		//TIMEUSPEND.time_begin();
		m_pExploer->clock();
		//TIMEUSPEND.time_end("MessageFilter:clock:m_pExploer:clock");
	}
}

bool MessageFilter::checkFilterMsg(Message* msg)
{
	bool ret = false;
	if (msg->GetTypeName() == string("CSBattleCreateReq"))
	{
		CSBattleCreateReq* bcMsg = (CSBattleCreateReq*)msg;
		int instanceId = bcMsg->battle_id();
		bool isPVE = LevelManager::sShareInstance()->isPVELevel(instanceId);
		bool isUnder = LevelManager::sShareInstance()->isUndergroundLevel(instanceId);
		bool isElite = LevelManager::sShareInstance()->isEliteLevel(instanceId);
        bool isFirstBattle=instanceId==280000 ? true : false;
		if (isPVE || isUnder || isElite || isFirstBattle)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ret = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/NetWork/LocalServerMgr.lua", "CheckFilterMessage", msg->GetTypeName().c_str());	

	return ret;
}

bool MessageFilter::checkBothSendMsg(string typeName)
{
	bool ret = false;
	ret = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/NetWork/LocalServerMgr.lua", "CheckBothSendMsg", typeName.c_str());

	return ret;
}

void MessageFilter::ProcessMessage(Message* bodyMsg, MessageHeader* headerMsg)
{
	static byte_array_t ba;
	ba.init_postion();

	//////////////////////////////////////////////////////////////////////////
	//encode message to binary stream
	//////////////////////////////////////////////////////////////////////////
	head_msg::CSMsgName EMsgName ;
	string SMsgName = bodyMsg->GetTypeName() ;
	head_msg::CSMsgName_Parse(SMsgName ,&EMsgName);
	
	//set header cmd using body msg
	headerMsg->set_msg_id(EMsgName);

	if (encodeMessage(headerMsg, bodyMsg, ba) == false)
	{		
	
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	//write binary stream to server
	//////////////////////////////////////////////////////////////////////////
	uint32_t totalSize = ba.get_postion(); 
	if (m_pExploer)
	{
		m_pExploer->dispatch(ba.get_buf(), totalSize);
	}
}

//------------------------------------------------------------------------
bool MessageFilter::encodeMessage(MessageHeader* headerMsg, Message* bodyMsg, byte_array_t& outBuffer)
{
	
	if (headerMsg == NULL || bodyMsg == NULL)	
		return false;
	

	//write len(u32)
	uint32_t len = 4 + 4 + headerMsg->ByteSize() + bodyMsg->ByteSize();
	outBuffer.write_uint32(htonl(len));

	//write hlen(u32)
	uint32_t hlen = headerMsg->ByteSize() + 4;
	outBuffer.write_int32(htonl(hlen));

	//write header buffer
	char* headerBuffer = NULL;
	int headerLen = 0;
	bool result = MessageUtil::sShareInstance()->encodeMessage2Buffer(headerMsg, headerBuffer, headerLen);
	if (result)
	{
		outBuffer.write_buf(headerBuffer, headerLen);
		delete headerBuffer;
	}
	else
	{
		CCLOG("encode header message to buffer error");
		MessageUtil::sShareInstance()->printMessage(headerMsg);

		delete headerBuffer;

		return false;
	}

	//write body buffer
	char* bodyBuffer = NULL;
	int bodyLen = 0;
	result = MessageUtil::sShareInstance()->encodeMessage2Buffer(bodyMsg, bodyBuffer, bodyLen);
	if (result)
	{
		outBuffer.write_buf(bodyBuffer, bodyLen);
		delete bodyBuffer;
	}
	else
	{
		CCLOG("encode body message to buffer error");
		MessageUtil::sShareInstance()->printMessage(bodyMsg);

		delete bodyBuffer;

		return false;
	}

	return true;
}

//------------------------------------------------------------------------
bool MessageFilter::decodeMessage(char* inputBuffer, int bufferLen, 
	char*& headerBuffer, 
	int& headerLen,
	char*& bodyBuffer,
	int& bodyLen, 
	int& totalLen)
{
	//package length
	uint32_t* currPackageLen = (uint32_t*)inputBuffer;
	totalLen = ntohl(*currPackageLen);
	inputBuffer += 4;

	//invalid message package
	if (totalLen > bufferLen || totalLen < 12)
	{
		CCLOG("invalid message package length : %d", totalLen);				
		return false;
	}

	//head length
	uint32_t* currHeadLen = (uint32_t*)inputBuffer;
	headerLen = ntohl(*currHeadLen) - 4;
	inputBuffer += 4;

	//invalid message header
	if (headerLen > totalLen || headerLen < 0)
	{
		CCLOG("invalid message header length : %d", headerLen);	
		return false;
	}

	//head buffer
	headerBuffer = (char*)inputBuffer;	
	inputBuffer += headerLen;

	//body buffer
	bodyBuffer = (char*)inputBuffer;
	bodyLen = totalLen - 4 - 4 - headerLen;
	inputBuffer += bodyLen;

	//invalid message body
	if (bodyLen < 0)
	{
		CCLOG("invalid message body length : %d", bodyLen);	
		return false;
	}	

	return true;
}


int MessageFilter::PickBufferData(const void* pbuffer, int nLen)
{
	char* headerBuffer = NULL;
	char* bodyBuffer= NULL;
	int headerLen = 0;
	int bodyLen = 0;
	int totalLen = 0;
	bool result = false;
	
	do{
		result = decodeMessage((char*)pbuffer, nLen, headerBuffer, headerLen, bodyBuffer, bodyLen, totalLen);
		if (result)
		{
			/// call online network manager dispatch message!
			OnlineNetworkManager::sShareInstance()->dispatchMessage(headerBuffer, headerLen, bodyBuffer, bodyLen, 0);
			if (g_curPos - totalLen > 0)
			{
				memmove(g_buffer, g_buffer + totalLen, g_curPos - totalLen);
			}

			g_curPos -= totalLen;
			if (g_curPos < 0)
			{
				CCAssert(0, "impossible!!! why this happen, aha!");
				g_curPos = 0;
			}
		}
	}while(result && g_curPos > 0);

	return nLen - g_curPos;
}

void MessageFilter::threadClock()
{

}
