#pragma once
#include "Singleton_t.h"
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class NickNameManager:public TSingleton<NickNameManager>
{
public:
	NickNameManager();
	~NickNameManager();

	void setNickNameCallback(CCNode* target,SEL_CallFuncND func);
	void unRegisterNickNameCallback();
	const char * getNickName(bool isMale);

	void onReceiveNewNickName(const char* nick,unsigned int sexual);
private:
	bool b_isNeedNickFlag;
	CCNode* m_callTarget;
	SEL_CallFuncND m_callHandler;
	std::vector<std::string> m_maleNickVec;
	std::vector<std::string> m_femaleNickVec;
};