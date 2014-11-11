#ifndef _MARKET_MANAGER_H_
#define _MARKET_MANAGER_H_

#include "Singleton_t.h"
#include "cocos2d.h"

USING_NS_CC;

struct MarketData
{
    int market_id;
    int item_id;
    int item_amount;
    int type;
    int cost_item_id;
    int cost_item_amount;
    int discount_amount;
    int vip_level;
    int restriction;
    int sort;
};

struct ExchangePrice
{
    int exchangeId;
    int price;
};

class MarketManager :
    public TSingleton<MarketManager>,public CCObject
{
public :
    MarketManager();
    virtual ~MarketManager();

    void InitXmlData();
    std::vector<MarketData> getIndexLevelItemList(int type, int vip_level);
    std::vector<MarketData> getVipShopItemList(int minVipLevel, int maxVipLevel);

    static bool sortData(MarketData data1, MarketData data2);

    void UpdateShopInfo(std::map<int, int> mapShoppingInfo);
    int  getIndexMarketItemShopTimes(int marketId);
    void ShowShopSuccess();
    void buyShopItem(int index);

    void UpdateExchangeList(std::vector<ExchangePrice> vtPrices);
    int  GetExchangePrice(int id);
    int  GetIndexPrice(int id);
    void updateRefreshTimes(float f);

	void ReqShopTimesFromServer();

	bool isShopedCurrentVipPacks();

	int getVip0PackId();

private :
    std::map<int, MarketData> m_totalMarketInfo;
    std::map<int, int> m_totalShoppingInfo;
    CC_SYNTHESIZE(int, m_lastShopItemId, LastShopItemId);
    std::map<int, int> m_exchangeParameters;
};

#endif