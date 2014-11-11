#ifndef _RANK_MANAGER_H_
#define _RANK_MANAGER_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include <iostream>
USING_NS_CC;
USING_NS_CC_EXT;

struct RankInfoX
{
    int userid			;
    int reg_time		;
    int channel_id		;
    std::string name	;
    uint64_t score	    ;
    int rank			;
    int updown			;
    int rank_name       ;
    int type;
    std::string guild_name;
};

#define Msg_Refresh_RankList "refresh_rank_list"

class RankManager : public CCObject
{
public:
	RankManager();
	virtual ~RankManager();

	static RankManager* getInstance();

    // send (all send logic is added at OnlineNetWorkManager)

    // receive
    void OnReceivePlayerRank(int rank);
    void OnReceiveRankInfoData(std::vector<RankInfoX> info);

    // interface
    void SetRankVectorInfoNULL();
    std::vector<RankInfoX> GetRankInfo(){ return allRankInfo; }
    uint32_t Offset32Bit(uint64_t num);

private:
	std::vector<RankInfoX> allRankInfo;
    CC_SYNTHESIZE(int,     m_pPlayerRank, PlayerRank);
    CC_SYNTHESIZE(int,     m_pRankName,   RankName);
    CC_SYNTHESIZE(CCPoint, m_pInitPos,    InitPos);
    CC_SYNTHESIZE(CCPoint, m_pBagInitPos, BagInitPos);
	CC_SYNTHESIZE(CCPoint, m_pPartyInitPos,PartyInitPos);
    CC_SYNTHESIZE(int,     m_pRankIndex,  RankIndex);
    int m_receiveCount;
};
#endif