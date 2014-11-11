#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ManorDataCenter
{
public :
	ManorDataCenter();

	static ManorDataCenter* Get();
	static void Destroy();
	/**
	* Instruction : ÷ÿ÷√±‰¡ø
	* @param 
	*/
	void ResetValue();
private:
	static ManorDataCenter* mManorDataCenter;
};