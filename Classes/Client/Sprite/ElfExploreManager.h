#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ElfExploreManger
{
public :
	ElfExploreManger();

	static ElfExploreManger* Get();
	static void Destroy();
	/**
	* Instruction : ÷ÿ÷√±‰¡ø
	* @param 
	*/
	void ResetValue();

	void SendExploreReq(unsigned int type);
	void SendExploreDetermineReq(unsigned int optionId);
	void SendExchangeElfReq(unsigned int exchangeItemId);
private:
	static ElfExploreManger* mElfExploreManger;
	bool mSendExploreReq;
};