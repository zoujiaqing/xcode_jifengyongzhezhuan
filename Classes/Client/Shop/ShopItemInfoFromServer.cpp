#include "NetWorkConfigInfoFromServer.h"
#include <sys/stat.h>
#include <string.h>
#include "CCFileUtils.h"
#include "CCCommon.h"
#include "curl/curl.h"
#include "json_res.h"
#include "FileManager.h"
#include "GameResourceManager.h"
#include "ShopItemInfoFromServer.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"
#include "UserData.h"
#include "ResourcesUpdateManager.h"
#include "AppDelegate.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "md5.h"
#include "cocos-ext.h"  

#define RE_DOWNLOAD_TIMES 5

ShopItemInfoFromServer *ShopItemInfoFromServer::instance = NULL;

ShopItemInfoFromServer *ShopItemInfoFromServer::sharedInstance()
{
	if (instance == NULL) {
		instance = new ShopItemInfoFromServer();
	}

	return instance;
}

ShopItemInfoFromServer::ShopItemInfoFromServer()
{
	m_nRepeatTimes = RE_DOWNLOAD_TIMES;
    m_mapBuyShopInfo.clear();
}

ShopItemInfoFromServer::~ShopItemInfoFromServer()
{
}

size_t ShopItemInfoFromServer::configFileDownLoadCallback(char *ptr, size_t size, size_t nmemb, void *context)
{
	/** To append content to a string */
	ShopItemInfoFromServer::sharedInstance()->m_strFileList.append(ptr);
	return  size*nmemb;
}

size_t ShopItemInfoFromServer::configFileDownLoadShopInfo(char *ptr, size_t size, size_t nmemb, void *context)
{
    /** To append content to a string */
    ShopItemInfoFromServer::sharedInstance()->m_strShopBuyList.append(ptr);
    return  size * nmemb;
}


bool ShopItemInfoFromServer::downloadConfigFileListOneTime(const char* url,const char* postMessage)
{
	if (url == NULL) {
		return false;
	}

	std::string strHtml = "";
	std::string strRetData = "";    

	CURLcode curlRes;
	CURL* easyCurl = curl_easy_init();
	if (easyCurl == NULL) {
		return false;
	}
    ShopItemInfoFromServer::sharedInstance()->m_strFileList = "";
	//curl_easy_setopt(easyCurl, CURLOPT_WRITEFUNCTION, &ShopItemInfoFromServer::configFileDownLoadCallback);
	//curl_easy_setopt(easyCurl, CURLOPT_POSTFIELDS, postMessage);
	//curl_easy_setopt(easyCurl, CURLOPT_WRITEDATA, &strHtml);
	//curl_easy_setopt(easyCurl, CURLOPT_URL,url);
	//curl_easy_setopt(easyCurl, CURLOPT_POST, 1);
	//curl_easy_setopt(easyCurl, CURLOPT_FOLLOWLOCATION, true);
	//curlRes = curl_easy_perform(easyCurl);
	//curl_easy_cleanup(easyCurl);

	curl_easy_setopt(easyCurl,CURLOPT_URL,url);  
    // curl_easy_setopt(easyCurl, CURLOPT_CONNECTTIMEOUT,2.5);
	curl_easy_setopt(easyCurl,CURLOPT_WRITEFUNCTION,&ShopItemInfoFromServer::configFileDownLoadCallback);  
	curlRes=curl_easy_perform(easyCurl);  
	if(curlRes!=CURLE_OK)  
	{  
		char temp[100];
		snprintf(temp,100,  "failed to get product list err code %i",curlRes); 
		//AppDelegate::GetShareAppDelegate()->ShowToast(temp);
		CCLOG("%s", temp);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		//send failed log to c01
		{
        CCHttpRequest* request = new CCHttpRequest();
		snprintf( temp,100, "http://report.st.61.com/misc.js?gameid=82&step=3&flag=%d",curlRes); 
        request->setUrl(temp);
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback((CCObject*)this, callfuncND_selector(ShopItemInfoFromServer::onHttpRequestCompleted));
		
		CCHttpClient::getInstance()->send(request);
        request->release();
    	}
#endif
	}  

	curl_easy_cleanup(easyCurl);  

	bool result = (curlRes == CURLE_OK);

	return result;
}
void ShopItemInfoFromServer::onHttpRequestCompleted(CCNode *node,void *data)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	CCHttpResponse *response = (CCHttpResponse*)data;
	if (!response)
    {
        return;
    }
    
   
    if (!response->isSucceed()) 
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }	
#endif
}
bool ShopItemInfoFromServer::downloadShopBuyInfoOneTime(const char* url, const char* postMessage)
{
    if (url == NULL) {
        return false;
    }

    CURLcode curlRes;
    CURL* easyCurl = curl_easy_init();
    if (easyCurl == NULL) {
        return false;
    }

    ShopItemInfoFromServer::sharedInstance()->m_strShopBuyList = "";
    curl_easy_setopt(easyCurl,CURLOPT_URL,url);  
    curl_easy_setopt(easyCurl,CURLOPT_WRITEFUNCTION,&ShopItemInfoFromServer::configFileDownLoadShopInfo);  
    curlRes = curl_easy_perform(easyCurl);  
    if (curlRes != CURLE_OK)  
    {  
        CCLog("联网超时 %i",curlRes);  
        //AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect ! ShopItemInfoFromServer::downloadShopBuyInfoOneTime");
    }  

    curl_easy_cleanup(easyCurl);  

    bool result = (curlRes == CURLE_OK);

    return result;
}

bool ShopItemInfoFromServer::downloadConfigFileList()
{
	const char* url = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetDownLoadShopItemInfoConfigURL");

	while (m_nRepeatTimes > 0)
	{
		if (downloadConfigFileListOneTime(url,""))
		{
			return true;
		}
		m_nRepeatTimes --;
	}
	//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect !ShopItemInfoFromServer::downloadConfigFileList()");
	//CCLOG("ShopItemInfoFromServer::downloadConfigFileList() NetWork Error");
	return false;
}

bool ShopItemInfoFromServer::downloadShopBuyInfo()
{
    USERINFO mUserData = UserData::GetUserInfo();
    int user_id = mUserData.id.getID();
    int reg_time = mUserData.id.getRegTime();
    int server_id = mUserData.serverId;
    int game_id = 82;
    int channel_id = LoginManager::Get()->getChannelID();
    CCString* strSign = CCString::createWithFormat("channel_id=%d&user_id=%d&reg_time=%d&server_id=%d&key=2a8f2a21181a604976b29cee419097a1",
        channel_id, user_id, reg_time, server_id);

    // user_id = 24041656;
    // reg_time = 1401366066;
    const char* url = CCString::createWithFormat("http://wlpay.shootao.com/query/user_charge_detail.php?game_id=%d&channel_id=%d&user_id=%d&reg_time=%d&server_id=%d&sign=%s",
                                                 game_id, channel_id, user_id, reg_time, server_id, "111")->getCString();

    while (m_nRepeatTimes > 0)
    {
        if (downloadShopBuyInfoOneTime(url,""))
        {
            return true;
        }
        m_nRepeatTimes --;
    }
   // AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect !ShopItemInfoFromServer::downloadShopBuyInfo()");
    CCLOG("ShopItemInfoFromServer::downloadConfigFileList() NetWork Error");
    return false;
}

bool ShopItemInfoFromServer::parseShopBuyInfoFromServer()
{
    m_mapBuyShopInfo.clear();
    const char* pMessage = m_strShopBuyList.c_str();

    CCLOG("download step m_strShopBuyList %s",pMessage);
    if (pMessage == NULL || pMessage == "" || pMessage == "error")
    {
        return false;
    }

    Json::Value jsonValue;
    if (!parseJsonStr(pMessage, jsonValue)) 
    {
        CCLOGERROR("UpdateInfoFromServer Error: %s",pMessage);
        return false;
    }

    bool isArray = jsonValue.isArray();
    if (isArray)
    {
        unsigned int arraySize = jsonValue.size();
        for (int i = 0; i < arraySize; i++)
        {
            const Json::Value jProduct_id = jsonValue[i]["id"];
            const Json::Value jAmount = jsonValue[i]["count"];

            if (false == jProduct_id.isNull() &&
                false == jAmount.isNull())
            {
                int id = atoi(jProduct_id.asCString());
                int amount = atoi(jAmount.asCString());

                m_mapBuyShopInfo.insert(std::make_pair(id, amount));
            }
        }
    }

    return true;
}

bool ShopItemInfoFromServer::parseRcvMessageFromServer()
{
	const char* pMessage = m_strFileList.c_str();

	CCLOG("download step m_strFileList %s",pMessage);
	if (pMessage == NULL || pMessage == "" || pMessage == "error")
	{
		return false;
	}

	Json::Value jsonValue;
	if (!parseJsonStr(pMessage, jsonValue)) 
	{
		CCLOGERROR("UpdateInfoFromServer Error: %s",pMessage);
		return false;
	}
    bool isArray = jsonValue.isArray();
	if (isArray)
	{
		unsigned int arraySize = jsonValue.size();
		for (int i=0; i<arraySize; i++)
		{
			OneShopItemInfo shopItemInfo;
			unsigned int n_productId = 0;
			const Json::Value product_id = jsonValue[i]["product_id"];
			if (false == product_id.isNull())
			{
				std::string sProductId = product_id.asString();				
				n_productId = atoi(sProductId.c_str());
				shopItemInfo.product_id = n_productId;
			}
			const Json::Value items = jsonValue[i]["items"];
			if (false == items.isNull())
			{
				Json::Value::Members members = items.getMemberNames();
				for (Json::Value::Members::iterator iter = members.begin();
					iter != members.end(); iter++)
				{
					std::string s_item_id = (*iter);
					unsigned int nItemId = atoi(s_item_id.c_str());
					shopItemInfo.item_id = nItemId;

					Json::Value itemValue = items[s_item_id];
					unsigned int item_base_num = atoi(itemValue["base"].asString().c_str());
					shopItemInfo.item_base_num = item_base_num;
					unsigned int item_gift_num = atoi(itemValue["gift"].asString().c_str());
					shopItemInfo.item_gift_num = item_gift_num;
					break;
				}
			}
			const Json::Value product_name = jsonValue[i]["name"];
			if (false == product_name.isNull())
			{
				shopItemInfo.product_name = product_name.asString();
			}
			const Json::Value product_price = jsonValue[i]["price"];
			if(false == product_price.isNull())
			{
				shopItemInfo.product_price = atoi(product_price.asString().c_str());
			}
			const Json::Value product_orgin_price = jsonValue[i]["orig_price"];
			if (false == product_orgin_price.isNull())
			{
				shopItemInfo.product_orig_price = atoi(product_orgin_price.asString().c_str());
			}

            const Json::Value add_times = jsonValue[i]["add_times"];
            if (false == add_times.isNull())
            {
                shopItemInfo.add_times = atoi(add_times.asString().c_str());
            }

            const Json::Value product_attr = jsonValue[i]["product_attr"];
            if (false == product_attr.isNull())
            {
                shopItemInfo.is_command = atoi(product_attr.asString().c_str());
            }

			const Json::Value extend_data = jsonValue[i]["extend_data"];
            if (!extend_data.isNull() && extend_data.asString() != "")
            {
				std::string str = extend_data.asString();
				int dashPos = str.find_first_of("/");
				std::string productTypeStr = str.substr(0,dashPos);
				std::string subStr = str.substr(dashPos+1,str.length());
				int secDashPos = subStr.find_first_of("/");
				std::string resources = subStr.substr(0,secDashPos);
				std::string indexs = subStr.substr(secDashPos+1,subStr.length());
                shopItemInfo.product_type = atoi(productTypeStr.c_str());
				shopItemInfo.resource_name = resources;
				shopItemInfo.product_index = atoi(indexs.c_str());
            }

			if (shopItemInfo.item_id == 1)
			{
				mapShopInfo.insert(std::make_pair(n_productId,shopItemInfo));
			}			
		}
		return true;
	}	
	return false;
}

bool ShopItemInfoFromServer::parseServerInfo(const Json::Value &serverInfo)
{
	if (serverInfo.isNull())
	{
		return false;
	}
	const Json::Value serverSubInfo = serverInfo["server"];
	if (serverInfo.isNull())
	{
		return false;
	}

	return true;
}

OneShopItemInfo ShopItemInfoFromServer::GetOneShopItemInfo(unsigned int productId)
{
	std::map<unsigned int,OneShopItemInfo>::iterator iter = mapShopInfo.find(productId);
	if (iter != mapShopInfo.end())
	{
		return (*iter).second;
	}	
	return OneShopItemInfo();
}

void ShopItemInfoFromServer::PrepareNeedDataForLua()
{
	// Note: 清空原来的数据项
	mapShopInfo.clear();
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/BuyDiamondDataCenter.lua", "BuyDiamondDataCenterResetValue");

	if (false == downloadConfigFileList())
	{
		std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
		TXGUI::MessageBox::AdjustPriority(true);
		TXGUI::MessageBox::Show(msg.c_str(), NULL, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
		TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
		return ;
	}
	parseRcvMessageFromServer();

	// Note: 重新插入数据项
	for (std::map<unsigned int,OneShopItemInfo>::iterator iter = mapShopInfo.begin();
		iter != mapShopInfo.end(); iter++)
	{
		LuaTinkerManager::Get()->callLuaFunc<bool>("Script/UILayer/BuyDiamondDataCenter.lua", "BuyDiamondDataCenterInsertOneItem",(*iter).first);
	}
}

void ShopItemInfoFromServer::PrepardData()
{
	if(mapShopInfo.size() > 0){
		//only get once shop item info .jackniu
		return ;
	}
	mapShopInfo.clear();
    if (false == downloadConfigFileList())
    {
        std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
        TXGUI::MessageBox::AdjustPriority(true);
        TXGUI::MessageBox::Show(msg.c_str(), NULL, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
        TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
        TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
        return ;
    }

    parseRcvMessageFromServer();
}

void ShopItemInfoFromServer::PrepareShopData()
{
    m_mapBuyShopInfo.clear();
    if (false == downloadShopBuyInfo())
    {
        std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
        TXGUI::MessageBox::AdjustPriority(true);
        TXGUI::MessageBox::Show(msg.c_str(), NULL, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
        TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
        TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
        return ;
    }

    parseShopBuyInfoFromServer();
}


bool ShopItemSortFunc(const OneShopItemInfo& a,const OneShopItemInfo& b)
{
	return a.product_index < b.product_index;
}