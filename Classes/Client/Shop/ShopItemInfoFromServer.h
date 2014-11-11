#ifndef _SHOP_ITEM_INFO_FROM_SERVER_H_
#define _SHOP_ITEM_INFO_FROM_SERVER_H_

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "json_res.h"
#include "vector"
#include "set"
#include "map"
USING_NS_CC;

class OneShopItemInfo
{
public:
	// Note: 
	unsigned int product_id;
	unsigned int item_id;
	unsigned int item_base_num;
	unsigned int item_gift_num;
	std::string product_name;
	unsigned int product_price;
	unsigned int product_orig_price;
    unsigned int add_times;
    unsigned int is_command;
	unsigned int product_type; // 0 正常商品 1 月卡
	unsigned int product_index;
	std::string resource_name;
	OneShopItemInfo(){
		product_id = 0;
		item_id = 0;
		item_base_num = 0;
		item_gift_num = 0;
		product_name = "";
		product_price = 0;
		product_orig_price = 0;
        add_times = 1;
        is_command = 0;
		product_type = 0;
		resource_name = "";
		product_index = 0;
	}
};

bool ShopItemSortFunc(const OneShopItemInfo& a,const OneShopItemInfo& b);

class ShopItemInfoFromServer
{
public:
	static ShopItemInfoFromServer *sharedInstance();
	~ShopItemInfoFromServer();
	
	/**
	* Instruction : 下载配置信息表 多次尝试下载
	* @param 
	*/	
	bool downloadConfigFileList();
    bool downloadShopBuyInfo();
	std::string GetDownLoadInfoString(){return m_strFileList;}
	bool parseRcvMessageFromServer();	
    bool parseShopBuyInfoFromServer();

	// Note: 基本信息查询函数
	OneShopItemInfo GetOneShopItemInfo(unsigned int productId);
	// Note: 为Lua准备需要的数据
	void PrepareNeedDataForLua();
    void PrepardData();

    void PrepareShopData();

    std::map<unsigned int, OneShopItemInfo> getShopInfo() { return mapShopInfo; }
    std::map<int, int> getShopBuyInfo() { return m_mapBuyShopInfo;}
	void onHttpRequestCompleted(CCNode *node,void *data);

private:
	ShopItemInfoFromServer();

	/**
	* Instruction : URL download. 单次下载
	* @param 
	*/	
	bool downloadConfigFileListOneTime(const char* url,const char* postMessage);
    bool downloadShopBuyInfoOneTime(const char* url, const char* postMessage);
	bool parseServerInfo(const Json::Value &serverInfo);	
	static size_t configFileDownLoadCallback(char* ptr, size_t size, size_t nmemb, void* context);
	static size_t configFileDownLoadShopInfo(char* ptr, size_t size, size_t nmemb, void* context);

private:
	static ShopItemInfoFromServer* instance;
	Json::Value m_jvJsonValue;
	std::string m_strFileList;
    std::string m_strShopBuyList;
	int m_nRepeatTimes;
	std::map<unsigned int,OneShopItemInfo> mapShopInfo;
    std::map<int, int> m_mapBuyShopInfo;
};

#endif //_SHOP_ITEM_INFO_FROM_SERVER_H_
