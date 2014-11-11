#pragma once

#include "Singleton_t.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <iostream>
#include "cs_core.pb.h"
USING_NS_CC;
USING_NS_CC_EXT;

struct treasureInfo
{
    int id;
    int type;
    std::string item;
    int freeTimes;
    treasureInfo()
    {
        id = -1;
    }
};

struct tresureItems
{
    int id;
    int num;
    tresureItems()
    {
        id = -1;
    }
};

class TreasureExploreManager : public CCObject
                  , public TSingleton<TreasureExploreManager>
{
public:
    TreasureExploreManager();
    virtual ~TreasureExploreManager();

    // send 

    // receive
    void onReceiveTreasureItems(CSNotifyItemExploreRewardRsp * msg);

    // interface
    void InitXMLData();
    int GetTreasureMapId(int mapIndex);
    int GetTreasureItemId(int mapIndex);
    int GetTreasureItemCount(int mapIndex);
    int GetCurMapFreeTimes(int mapIndex);
    CCSpriteFrame* GetMapSpriteFrame(int mapIndex);
    std::vector<tresureItems> getTreasureItemsFromServer() { return m_vecTreasureItems; }

private:
    std::map<int, treasureInfo> m_mpTreasureInfo;
    int m_alreadyGetIndex;
    CC_SYNTHESIZE(int, m_forwardRate, ForwardRate);
    CC_SYNTHESIZE(int, m_forwardAlreadyIdx, ForwardAlreadyIdx);
    std::vector<tresureItems> m_vecTreasureItems;
	//CC_SYNTHESIZE(int, m_refreshTimes, RefreshTimes);
	CC_SYNTHESIZE(int, m_resetTimes, ResetTimes);
	CC_SYNTHESIZE(int, m_refreshOpenLv, RefreshOpenLv);
	CC_SYNTHESIZE(bool, m_isReset, IsReset);
};