
#pragma once

#include <string>

using namespace std;

typedef struct   stMSG_PACKAGE_HEADER
{
	//user data	
	int				ret;
	int				tag;
	int				uid;

	std::string		cmd;

}MSG_PACKAGE_HEADER;


typedef struct  stNORMAL_MSG_PACKAGE
{
	//net work data
	google::protobuf::Message*				msg;

	MSG_PACKAGE_HEADER		header;	

}NORMAL_MSG_PACKAGE;


typedef struct   stWILD_MSG_PACKAGE
{
	//net work data
	char*					buffer;
	int						length;

	MSG_PACKAGE_HEADER		header;	

protected:
	int curPos;

public:
	stWILD_MSG_PACKAGE()
	{
		curPos = 0;
	}

	int ReadInt32()
	{
		if (curPos >= length)
			return 0;

		int val = *(int*)(buffer + curPos);
		curPos += 4;
		return val;
	}

	short ReadInt16()
	{
		if (curPos >= length)
			return 0;

		short val = *(short*)(buffer + curPos);
		curPos += 2;
		return val;
	}

	double ReadNumber32()
	{
		if (curPos >= length)
			return 0;

		double val = *(double*)(buffer + curPos);
		curPos += 4;
		return val;
	}

	std::string ReadString()
	{
		if (curPos >= length)
			return "";

		// 字符前一个buffer 长度区
		int strLen = *(int*)(buffer + curPos);
		curPos += 4;

		if (curPos >= length)
			return "";

		char *szVal = new char[strLen + 1];
		memcpy(szVal, (char*)(buffer + curPos), strLen);
		szVal[strLen] = '\0';

		// 字符的内容区 
		curPos += strLen;

		std::string val = szVal;
		delete []szVal;

		return val;
	}

	std::string ReadChar()
	{
		if (curPos >= length)
			return "";

		char val[2] ="";
		val[0] = *(char*)(buffer + curPos);
		curPos += 1;
		val[1] ='\0';
		return val;
	}

}WILD_MSG_PACKAGE;


class BaseMsgHandler
{	
public:
	 virtual void HandlerMessage(string cmd, NORMAL_MSG_PACKAGE* package) = 0;
};