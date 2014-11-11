#include "OnlineNetworkManager.h"
#include "ElfExploreManager.h"

ElfExploreManger* ElfExploreManger::mElfExploreManger = 0;

ElfExploreManger::ElfExploreManger()
{
	mSendExploreReq = false;
}

void ElfExploreManger::ResetValue()
{
}

ElfExploreManger* ElfExploreManger::Get()
{
	if (mElfExploreManger == 0)
	{
		mElfExploreManger = new ElfExploreManger();
	}
	return mElfExploreManger;
}

void ElfExploreManger::Destroy()
{
	if (mElfExploreManger)
	{
		delete mElfExploreManger;
		mElfExploreManger = 0;
	}
}

void ElfExploreManger::SendExploreReq(unsigned int type)
{
	if (type == 1)
	{
		//if (false == mSendExploreReq)
		{
			OnlineNetworkManager::sShareInstance()->SendExploreReq(type);
			//mSendExploreReq = true;
		}
	}
	else
	{
		OnlineNetworkManager::sShareInstance()->SendExploreReq(type);
	}	
}

void ElfExploreManger::SendExploreDetermineReq(unsigned int optionId)
{
	OnlineNetworkManager::sShareInstance()->SendExploreDetermineReq(optionId);
}

void ElfExploreManger::SendExchangeElfReq(unsigned int exchangeItemId)
{
	OnlineNetworkManager::sShareInstance()->sendElfExchangeReq(exchangeItemId);
}