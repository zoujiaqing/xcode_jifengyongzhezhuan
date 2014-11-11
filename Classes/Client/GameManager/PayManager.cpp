
#include "ShopItemInfoFromServer.h"
#include "UserData.h"
#include "XLogicManager.h"
#include "LuaTinkerManager.h"
#include "PayManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "TmPayManager.h"
//#include "PluginUtils.h"
#include "PluginManager.h"
#endif
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
using namespace cocos2d::plugin;
#endif
using namespace cocos2d;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "GlobalFunction.h"
#include "NcSDKManager.h"
#endif

PayManager::PayManager()
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    uint32_t channel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");

    TmPayManager::tmPayManager()->loadIAPPlugin((PlantformId)channel);
#endif
}

void PayManager::loadPlugin()
{
    CCLOG("%s %d", __FUNCTION__, __LINE__);
}
void PayManager::unloadPlugin()
{
    CCLOG("%s %d", __FUNCTION__, __LINE__);
}

void PayManager::payForProduct(int id, int value)
{
    CCLOG("%s id:%d product id:%d", __FUNCTION__, id, value);
    OneShopItemInfo osii = ShopItemInfoFromServer::sharedInstance()->GetOneShopItemInfo(value);
    USERINFO mUserData = UserData::GetUserInfo();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CCLOG("%s price:%d gift number:%d", __FUNCTION__, osii.product_price, osii.item_gift_num);
    PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");

	TProductInfo pInfo;
	if(channel == Taomee 
		|| channel == Oppo
		|| channel == MuZhiWan
		|| channel == Wandoujia
		|| channel == WeiYouXi
		|| channel == Bubugao
		|| channel == OuPeng
		|| channel == YingYongHui
		|| channel == ShanG
		|| channel == AnZhi
		|| channel == MuMayi
		|| channel == Huawei
		|| channel == PPS
		|| channel == JiFeng
		|| channel == JinLi
		|| channel == Lenovo
		|| channel == youXifengYun
		|| channel == TM
		|| channel == Duoku
		|| channel == Pptv)
	{
		char temp[500];
	    CCLOG("the channel is %d channel ", channel);
            
        snprintf(temp, sizeof(temp), "%d", osii.product_price);
        pInfo["m"] = string(temp);
        pInfo["gd"] = "82";
		snprintf(temp, sizeof(temp), "%d", channel);
        pInfo["cd"] = string(temp);
		
		if(channel == PPS){
			snprintf(temp, sizeof(temp), "ppsmobile_s%u", mUserData.serverId);
		}else{
			snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
		}
        pInfo["sd"] = string(temp);
        snprintf(temp, sizeof(temp), "%u", osii.product_id);

        pInfo["pd"] = string(temp);
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());
        pInfo["ud"] =  string(temp);
        
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["ut"] = string(temp);

        pInfo["gn"] = "ahero";
           
	}
    else if(channel == Qihoo360)
	{
	    char temp[500];
	    CCLOG("the channel is 360 channel ");
        
        /////////////////////////////////
        pInfo["defaultPayType"] = "zfb"; //由游戏自行设定，必填
        snprintf(temp, sizeof(temp), "%u", osii.product_id);
        pInfo["productId"] = string(temp); //由游戏提供，必填
        pInfo["nickName"] = string(mUserData.szName); //游戏角色名，由游戏提供，必填

        pInfo["accessToken"] = string(mUserData.accessToken); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

        pInfo["qihooId"] = string(temp); //qihooId由游戏提供，必填
        pInfo["gameId"] = "82"; //由c01分配，值参见文档，必填
        pInfo["gameName"] = "ahero"; //由公共平台分配，值参见文档，必填
        snprintf(temp, sizeof(temp), "%u", channel);
        pInfo["channelId"] = string(temp);; //由c01分配，值参见文档，必填
         snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
        pInfo["serverId"] = string(temp); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["userCreateTime"] = string(temp); //由游戏提供，必填
      

        pInfo["currency"] = "0"; //由游戏提供，必填
	   
	}
     else if(channel == Xiaomi)
	{
	    char temp[500];
	    CCLOG("the channel is xiaomi channel ");
        
        
        snprintf(temp, sizeof(temp), "%u", osii.product_id);
        pInfo["productId"] = string(temp); //由游戏提供，必填

        pInfo["gameId"] = "82"; //由c01分配，值参见文档，必填
        pInfo["gameName"] = "ahero"; //由公共平台分配，值参见文档，必填
         snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
        pInfo["serverId"] = string(temp); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["userCreateTime"] = string(temp); //由游戏提供，必填
      

        pInfo["currency"] = "0"; //由游戏提供，必填

  
    	pInfo["userBalance"] 		= "33";		//用户余额,由游戏提供，必填
		unsigned int vip = mUserData.m_vip;

		snprintf(temp, sizeof(temp), "%u", vip);
    	pInfo["userVip"] 			= string(temp);		//vip等级,由游戏提供，必填
    	snprintf(temp, sizeof(temp), "%u", mUserData.level);
    	pInfo["userLevel"] 			= string(temp);		//角色等级,由游戏提供，必填
    	pInfo["userPartyName"] 		= "jifeng";		//工会，帮派,由游戏提供，必填
    	
    	pInfo["userRoleName"] 		= string(mUserData.szName);		//角色名称,由游戏提供，必填
    	snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

    	pInfo["userRoleId"] 		= string(temp);		//角色id
		pInfo["userId"] = string(temp); 	//角色id

		snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
    	pInfo["userServerName"] 	= string(temp);		//所在服务器由游戏提供，必填
		snprintf(temp, sizeof(temp), "%u", channel);
		pInfo["channelId"] = string(temp);;
	//	snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

        
	} else if(channel == Nd91)
	{
	    char temp[500];
	    CCLOG("the channel is nd91 channel ");
        
        /////////////////////////////////
        snprintf(temp, sizeof(temp), "%u", osii.product_id);
        pInfo["productId"] = string(temp); //由游戏提供，必填

        pInfo["gameId"] = "82"; //由c01分配，值参见文档，必填
        pInfo["gameName"] = "ahero"; //由公共平台分配，值参见文档，必填
         snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
        pInfo["serverId"] = string(temp); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["userCreateTime"] = string(temp); //由游戏提供，必填
      
  
    	snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

    	pInfo["nd91Id"] 		= string(temp);		//角色id

       
	} 
	 else if(channel == Downjoy
	 	|| channel == UC)
	{
	    char temp[500];
        /////////////////////////////////
        snprintf(temp, sizeof(temp), "%u", osii.product_id);
        pInfo["productId"] = string(temp); //由游戏提供，必填

        pInfo["gameId"] = "82"; //由c01分配，值参见文档，必填
        pInfo["gameName"] = "ahero"; //由公共平台分配，值参见文档，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
        pInfo["serverId"] = string(temp); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["userCreateTime"] = string(temp); //由游戏提供，必填
               
		pInfo["currency"] = "0"; 
				
		snprintf(temp, sizeof(temp), "%u", channel);
        pInfo["channelId"] = string(temp); //由c01分配，值参见文档，必填

       	snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

    	pInfo["userId"] = string(temp);		//角色id
	}else if(channel == Coco 
		|| channel == DianXin 
		|| channel == Punchbox
		|| channel == YingYongBao
		|| channel == CocoPunchbox 
		|| channel == 59 
		|| (channel >=44 && channel <=57)
		|| (channel >=100 && channel <=199))
	{
	    char temp[500];
	    CCLOG("the channel is %d channel", channel);
        
        /////////////////////////////////
        snprintf(temp, sizeof(temp), "%u", osii.product_id);
        pInfo["productId"] = string(temp); //由游戏提供，必填

        pInfo["gameId"] = "82"; //由c01分配，值参见文档，必填
        pInfo["gameName"] = "ahero"; //由公共平台分配，值参见文档，必填
        pInfo["roleName"] 		= string(mUserData.szName);		
        snprintf(temp, sizeof(temp), "%u", mUserData.serverId);
        pInfo["serverId"] = string(temp); //由游戏提供，必填
        snprintf(temp, sizeof(temp), "%u", mUserData.id.getRegTime());
        pInfo["userCreateTime"] = string(temp); //由游戏提供，必填
      
		pInfo["currency"] = "0"; //由游戏提供，必填
				
       	snprintf(temp, sizeof(temp), "%u", mUserData.id.getID());

    	pInfo["userId"] = string(temp);		//角色id
    		
		snprintf(temp, sizeof(temp), "%u", channel);
        pInfo["channelId"] = string(temp);
	} 
    else
	{
		CCAssert(0, "the channel is not useable");
        CCLOG("the channel is not useable %d", channel);
    }
	 ///////////////////////
    std::map<std::string, std::string>::iterator it;
    for(it=pInfo.begin(); it != pInfo.end(); it++) {
        CCLog("pinfo[%s] = %s", it->first.c_str(), it->second.c_str());

    }

    TmPayManager::tmPayManager()->payForProduct(pInfo);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::doPay(IntToStr(osii.product_id), IntToStr(mUserData.id.getID()), IntToStr(mUserData.serverId), IntToStr(0), IntToStr(osii.product_price), IntToStr(mUserData.id.getRegTime()), IntToStr(mUserData.level));
#endif
}


PayManager::~PayManager()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    TmPayManager::tmPayManager()->unloadIAPPlugin();
#endif
}

