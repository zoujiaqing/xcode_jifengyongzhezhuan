#include "OnlineNetworkManager.h"
#include "ManorDataCenter.h"

ManorDataCenter* ManorDataCenter::mManorDataCenter = 0;

ManorDataCenter::ManorDataCenter()
{
}

void ManorDataCenter::ResetValue()
{
}

ManorDataCenter* ManorDataCenter::Get()
{
	if (mManorDataCenter == 0)
	{
		mManorDataCenter = new ManorDataCenter();
	}
	return mManorDataCenter;
}

void ManorDataCenter::Destroy()
{
	if (mManorDataCenter)
	{
		delete mManorDataCenter;
		mManorDataCenter = 0;
	}
}