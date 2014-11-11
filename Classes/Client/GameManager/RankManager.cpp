#include "OnlineNetworkManager.h"
#include "RankManager.h"

static RankManager* m_instance = NULL;
RankManager::RankManager()
{
    m_pPlayerRank = 0;
    m_pRankName   = 0;
    m_receiveCount = 1;
    allRankInfo.clear();
    m_pInitPos = CCPointZero;
    m_pBagInitPos = CCPointZero;
	m_pPartyInitPos = CCPointZero;
    m_pRankIndex = 0;
}

RankManager::~RankManager()
{

}

RankManager* RankManager::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new RankManager();
	}
	return m_instance;
}

void RankManager::SetRankVectorInfoNULL()
{
    allRankInfo.clear();
}

void RankManager::OnReceivePlayerRank(int rank)
{
    this->SetRankVectorInfoNULL();
    m_pPlayerRank = rank;

    // count player rank in 50 or not 
    if(rank >= 1 && rank <= 50)
    {
        OnlineNetworkManager::sShareInstance()->SendIndexTypeRankInfo(m_pRankName, 1, 1, 50);
        m_receiveCount = 1;
    }
    else if(rank >= 51 && rank <= 53)
    {
        OnlineNetworkManager::sShareInstance()->SendIndexTypeRankInfo(m_pRankName, 1, 1, rank + 2);
        m_receiveCount = 1;
    }
    else
    {
        OnlineNetworkManager::sShareInstance()->SendIndexTypeRankInfo(m_pRankName, 1, 1, 50);
        OnlineNetworkManager::sShareInstance()->SendIndexTypeRankInfo(m_pRankName, 1, rank - 2, rank + 2);
        m_receiveCount = 2;
    }
}

void RankManager::OnReceiveRankInfoData(std::vector<RankInfoX> info)
{
    for(int i = 0; i < info.size(); i++)
    {
        allRankInfo.push_back(info[i]);

        // request index playerinfo
        PLAYER_ID id = PlayerIdMake(info[i].userid, info[i].reg_time, info[i].channel_id);
        OnlineNetworkManager::sShareInstance()->sendUserInfoReq(id);
    }

    // if player rank is in rankinfo ,notice to refresh ui
    m_receiveCount --;
    if(m_receiveCount == 0)
    {
         CCNotificationCenter::sharedNotificationCenter()->postNotification(Msg_Refresh_RankList);
    }
}

// get 32 bit integer value
uint32_t RankManager::Offset32Bit(uint64_t num)
{
    uint32_t val = (num >> 32);
    return val;
}
