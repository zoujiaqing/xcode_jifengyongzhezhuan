#include "OnlineNetworkManager.h"
#include "MarketManager.h"
#include "MainMenuLayer.h"
#include "cs_core.pb.h"
#include "tinyxml.h"
#include "GameResourceManager.h"
#include "WaitingLayerManager.h"
#include "ItemManager.h"
#include "LuaTinkerManager.h"
#include "TimeManager.h"
#include "GMessage.h"
#include "MarketLayer.h"
#include "Localization.h"

MarketManager::MarketManager() :
    m_lastShopItemId(0)
{
    m_totalShoppingInfo.clear();
    m_totalMarketInfo.clear();
    InitXmlData();

    long nowTime = TimeManager::Get()->getCurServerTime();
    CCScheduler* scheduler = CCDirector::sharedDirector()->getScheduler();
    float res_time = 7200 - nowTime % 7200;
    scheduler->scheduleSelector(schedule_selector(MarketManager::updateRefreshTimes), this, 7200.0f, kCCRepeatForever, res_time, false);
}

MarketManager::~MarketManager()
{

}

void MarketManager::InitXmlData()
{
    m_totalMarketInfo.clear();

    std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Data/shop.xml");

    unsigned long	_size;
    char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "r", &_size);
    TiXmlDocument	*_document = new TiXmlDocument();
    _document->Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

    CC_SAFE_DELETE_ARRAY(_pFileContent);

    TiXmlElement *RootElement = _document->RootElement();
    if (NULL != RootElement)
    {
        TiXmlElement *childElement = RootElement->FirstChildElement();
        while (childElement != NULL)
        {
            const char *elementValue = childElement->Value();
            MarketData element;
            
            TiXmlAttribute* pAttribute = childElement->FirstAttribute();
            bool flag = true;
            while (pAttribute != NULL)
            {
                std::string strName(pAttribute->Name());
                std::string strContent(childElement->Attribute(pAttribute->Name()));
                if (strName == "Type")
                {
                    element.type = atoi(strContent.c_str());
                } else if (strName == "Vip_Level")
                {
                    element.vip_level = atoi(strContent.c_str());
                } else if (strName == "Item")
                {
                    if (strContent.size () <= 5)
                    {
                        flag = false;
                        break;
                    }

                    int dotPoint = strContent.find_first_of("/");
                    element.item_id = atoi(strContent.substr(0, dotPoint).c_str());
                    element.item_amount = atoi(strContent.substr(dotPoint + 1, strContent.size() - dotPoint - 1).c_str());
                } else if (strName == "Money")
                {
                    if (strContent.size () <= 5)
                    {
                        flag = false;
                        break;
                    }

                    element.cost_item_id = atoi(strContent.substr(0, 6).c_str());
                    element.cost_item_amount = atoi(strContent.substr(7, strContent.size() - 7).c_str());
                } else if (strName == "Discount_Money")
                {
                    if (strContent.size() <= 7)
                    {
                        element.discount_amount = atoi(strContent.c_str());
                    } else
                    {
                        element.discount_amount = atoi(strContent.substr(7, strContent.size() - 7).c_str());
                    }
                } else if (strName == "Restriction")
                {
                    element.restriction = atoi(strContent.c_str());
                } else if (strName == "ID")
                {
                    element.market_id = atoi(strContent.c_str());
                } else if (strName == "Sort")
                {
                    element.sort = atoi(strContent.c_str());
                }
                pAttribute = pAttribute->Next();
            }

            if (flag == true)
            {
                m_totalMarketInfo.insert(std::make_pair(element.market_id, element));
            }

            childElement = childElement->NextSiblingElement();
        }
    }
}

std::vector<MarketData> MarketManager::getIndexLevelItemList(int type, int vip_level)
{
    std::vector<MarketData> vtDatas;
    vtDatas.clear();

    for (std::map<int, MarketData>::iterator it = m_totalMarketInfo.begin();
         it != m_totalMarketInfo.end(); ++it)
    {
        MarketData data = it->second;
        if (data.type != type)
        {
            continue;
        }

        if (data.vip_level > vip_level)
        {
            continue;
        }

        // 检查是否有物品一样的
        bool flag = false;
        for (size_t i = 0; i < vtDatas.size(); ++i)
        {
            if (vtDatas[i].item_id == data.item_id &&
                vtDatas[i].item_amount == data.item_amount &&
                vtDatas[i].vip_level < data.vip_level)
            {
                vtDatas[i] = data;
                flag = true;
                break;
            }
        }

        if (flag == false)
        {
            vtDatas.push_back(data);
        }
    }

    std::sort(vtDatas.begin(), vtDatas.end(), MarketManager::sortData);
    return vtDatas;
}

bool MarketManager::sortData(MarketData data1, MarketData data2)
{
    return data1.sort < data2.sort;
}

std::vector<MarketData> MarketManager::getVipShopItemList(int minVipLevel, int maxVipLevel)
{
    std::vector<MarketData> vtDatas;
    vtDatas.clear();

    for (std::map<int, MarketData>::iterator it = m_totalMarketInfo.begin();
        it != m_totalMarketInfo.end(); ++it)
    {
        MarketData data = it->second;
        if (data.type != 18)
        {
            continue;
        }

        if (data.vip_level > maxVipLevel ||
            data.vip_level < minVipLevel)
        {
            continue;
        }

        // 检查是否有物品一样的
        vtDatas.push_back(data);
    }

    std::sort(vtDatas.begin(), vtDatas.end(), MarketManager::sortData);
    return vtDatas;
}

void MarketManager::UpdateShopInfo(std::map<int, int> mapShoppingInfo)
{
    for (std::map<int, int>::iterator it = mapShoppingInfo.begin();
         it != mapShoppingInfo.end(); ++it)
    {
        int marketId = it->first;
        int buyTimes = it->second;

        m_totalShoppingInfo[marketId] = buyTimes;
    }

    if(GameManager::Get()->GetSceneLayer() && GameManager::Get()->GetSceneLayer()->getMainMenuLayer())
    {
        MarketLayer* layer = dynamic_cast<MarketLayer*>(GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->getMarketLayer());
        if (layer)
        {
            layer->UpdateShoppingInfo();
        }
    }
}

int MarketManager::getIndexMarketItemShopTimes(int marketId)
{
    if (m_totalShoppingInfo.find(marketId) == m_totalShoppingInfo.end())
    {
        return 0;
    }

    return m_totalShoppingInfo[marketId];
}

int MarketManager::getVip0PackId()
{
	return m_totalShoppingInfo.begin()->first;
}

void MarketManager::ShowShopSuccess()
{
    if (m_lastShopItemId == 0)
    {
        return;
    }

    int itemId = m_lastShopItemId;
    setLastShopItemId(0);

    CCString* pStr = NULL;
    if (itemId > 100000)
    {
        ItemBase* itemBaseInfo = ItemManager::Get()->getItemInfoById(itemId);
        if (itemBaseInfo)
        {
            pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_BUY"), itemBaseInfo->m_name);
        }    
    } else
    {
        const char* fairyName = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/Fairy/FairyLuaInterface.lua","G_GetFairyNameByID", itemId);
        pStr = CCString::createWithFormat(Localizatioin::getLocalization("M_SHOP_BUY"), fairyName);
    }
    ShowMessage(pStr->getCString());
}

void MarketManager::buyShopItem(int index)
{
    Message* msg = MessageUtil::sShareInstance()->createMessageInstance("CSBuyShopItemReq");
    CSBuyShopItemReq* bodyMsg = (CSBuyShopItemReq*)msg;
    bodyMsg->set_index(index);
    OnlineNetworkManager::sShareInstance()->sendMessage(bodyMsg, 2.0, NETWORK_TAG_PLAYER_ENTER_MAP, true, true);
	WaitingLayerManager::Get()->PushOneMessage("CSBuyShopItemReq");
}

void MarketManager::UpdateExchangeList(std::vector<ExchangePrice> vtPrices)
{
    for (size_t i = 0; i < vtPrices.size(); ++i)
    {
        int id = vtPrices[i].exchangeId;
        int price = vtPrices[i].price;

        m_exchangeParameters[id] = price;
    }

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_EXCHANGE_PARAMETER, this);
}

int MarketManager::GetExchangePrice(int id)
{
    if (m_exchangeParameters.find(id) == m_exchangeParameters.end())
    {
        int price = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/ExchangeParameterControl.lua", "getExchangeParameterCostNum", id);
        return price;
    }

    return m_exchangeParameters[id];
}

int MarketManager::GetIndexPrice(int id)
{
    if (m_exchangeParameters.find(id) == m_exchangeParameters.end())
    {
        return 0;
    }

    return m_exchangeParameters[id];
}

void MarketManager::updateRefreshTimes(float f)
{
    USERINFO info = UserData::GetUserInfo();
    info.m_refreshTimes = info.m_refreshTimes + 1;
    UserData::SetUserInfo(info);
    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_SHOP_REFRESH_TIMES, this);
}

void MarketManager::ReqShopTimesFromServer()
{
    std::vector<int> itemsList;
    itemsList.clear();
	int vipPacksId[11] = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010};

    for (size_t i = 0; i < 11; ++i)
    {
        itemsList.push_back(vipPacksId[i]);
    }

    OnlineNetworkManager::sShareInstance()->SendGetShopTimes(itemsList);
}

bool MarketManager::isShopedCurrentVipPacks()
{
	int vipLevels = UserData::GetVipLevel();
	int vipPacksId[11] = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010};
	int buyTimes;

	if (m_totalShoppingInfo.size() == 0)
		return true;

	for (int i = 0; i <= vipLevels; i++)
	{
		buyTimes = m_totalShoppingInfo[vipPacksId[i]];

		if (buyTimes == 0)
			return false;
	}

	return true;
}
