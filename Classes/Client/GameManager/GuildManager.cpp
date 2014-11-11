#include "OnlineNetworkManager.h"
#include "GuildManager.h"
#include "GMessage.h"
#include "MainMenuLayer.h"
#include "Localization.h"
#include "ItemManager.h"
#include "GlobalFunction.h"
#include "GuildInfoLayer.h"
#include "GameResourceManager.h"
#include "TimeManager.h"
#include "tinyxml.h"

GuildManager::GuildManager():
    m_totalGuildAmount(0),
	m_iAllSendRedEnvoCount(0),
	m_isShowRedTip(false)
{
    m_allGuildList.clear();
    m_allSearchList.clear();
    m_allGuildMember.clear();
    m_vecGuildShopItems.clear();
    m_vecGuildBlessInfo.clear();
    m_vecGuildContributeInfo.clear();
    m_pIsRankNeedOpen = false;
    m_pMemberOpenType = 0;
    m_pIsGuildInfoNeedOpen = false;
    m_pMemberStatus.clear();
	m_vecGuildRecordInfo.clear();
	m_vecGuildRedEnvolopeInfo.clear();
	m_vecGuildRedEnvolopeinfoFromXML.clear();
	m_vecGuildRedEnvoRecordInfo.clear();
	m_vecGuildTuHaoRedEnvoInfo.clear();
    m_pStr1 = "";
    m_pStr2 = "";
	m_redEnvoSenderName = "";
    m_isKickMember = false;
    m_GuildBaseInfo.clear();
    InitXMLData();
}

GuildManager::~GuildManager()
{
    
}

void GuildManager::InitXMLData()
{
    m_GuildBaseInfo.clear();
    std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Data/Union.xml");

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
            guildBaseInfo element;
            TiXmlAttribute* pAttribute = childElement->FirstAttribute();
            while (pAttribute != NULL)
            {
                std::string strName(pAttribute->Name());
                std::string strContent(childElement->Attribute(pAttribute->Name()));
                if (strName == "Level")
                {
                    element.levelid = atoi(strContent.c_str());
                }
                else if(strName == "Chairman_Number")
                {
                    element.chairmanNum = atoi(strContent.c_str());
                }
                else if(strName == "Elite_Number")
                {
                    element.eliteNum = atoi(strContent.c_str());
                }
                pAttribute = pAttribute->Next();
            }

            m_GuildBaseInfo.insert(std::make_pair(element.levelid, element));

            childElement = childElement->NextSiblingElement();
        }
    }
}

int GuildManager::getAppointSecChariManNumByLevel(int glevel)
{
    std::map<int, guildBaseInfo>::iterator it = m_GuildBaseInfo.find(glevel);
    if(it == m_GuildBaseInfo.end())
    {
        return 0;
    }
    return it->second.chairmanNum;
}
int GuildManager::getAppointEliteNumByLevel(int glevel)
{
    std::map<int, guildBaseInfo>::iterator it = m_GuildBaseInfo.find(glevel);
    if(it == m_GuildBaseInfo.end())
    {
        return 0;
    }
    return it->second.eliteNum;
}

void GuildManager::SendGetGuildList(unsigned int begin, unsigned int end)
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildList(begin, end);
}

void GuildManager::SendSearchIndexGuild(std::string searchName)
{
    OnlineNetworkManager::sShareInstance()->SendSearchIndexGuild(searchName);
}

void GuildManager::SendJoinGuild(int highGid, int lowGid)
{
    OnlineNetworkManager::sShareInstance()->SendJoinGuild(highGid, lowGid);
}

void GuildManager::SendCreateGuild(std::string guild_name)
{
    OnlineNetworkManager::sShareInstance()->SendCreateGuild(guild_name);
}

void GuildManager::SendGetGuildInfo()
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildInfo();
}

void GuildManager::SendModifyNotice(std::string notice, std::string note)
{
    OnlineNetworkManager::sShareInstance()->SendModifyNotice(notice, note);
}

void GuildManager::SendGetGuildMember()
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildMember();
}

void GuildManager::SendChangeGuildMemberStatus(std::vector<StrGuildStatus> vtStatus)
{
    std::vector<unsigned int> vt_user_id;
    std::vector<unsigned int> vt_reg_time;
    std::vector<unsigned int> vt_channel_id;
    std::vector<unsigned int> vt_status;
    vt_user_id.clear();
    vt_reg_time.clear();
    vt_channel_id.clear();
    vt_status.clear();
    for (size_t i = 0; i < vtStatus.size(); ++i)
    {
        vt_user_id.push_back(vtStatus[i].user_id);
        vt_reg_time.push_back(vtStatus[i].reg_tm);
        vt_channel_id.push_back(vtStatus[i].channel_id);
        vt_status.push_back(vtStatus[i].status);
    }
    OnlineNetworkManager::sShareInstance()->SendChangeGuildMemberStatus(vt_user_id, vt_reg_time, vt_channel_id, vt_status);
}

void GuildManager::SendQuitGuild()
{
    OnlineNetworkManager::sShareInstance()->SendQuitGuild();
}

void GuildManager::SendJiesanGuld()
{
    OnlineNetworkManager::sShareInstance()->SendJiesanGuld();
}

void GuildManager::SendGetGuildShopItems(int type)
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildShopItems(type);
}

void GuildManager::SendGetGuildBlessInfo(bool flag)
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildBlessInfo(flag);
}

void GuildManager::SendGetGuildContributeInfo(std::vector<int> temp)
{
    OnlineNetworkManager::sShareInstance()->SendGetGuildContributeInfo(temp);
}

void GuildManager::SendGuildContribute(int type)
{
    OnlineNetworkManager::sShareInstance()->SendGuildContribute(type);
}

void GuildManager::SenderGuildLApplySettting(int type, int fightNum)
{
    OnlineNetworkManager::sShareInstance()->SenderGuildLApplySettting(type, fightNum);
}

void GuildManager::SendGuildBuy(int id, int index)
{
    OnlineNetworkManager::sShareInstance()->SendGuildBuy(id, index);
}

void GuildManager::SendGetGuildRecordInfoReq()
{
	//const char* channelId = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetChannelID");
	OnlineNetworkManager::sShareInstance()->SendGetGuildRecordInfoReq(m_playerGuildInfo.guildinfo().gid_high(), m_playerGuildInfo.guildinfo().gid_low(), m_playerGuildInfo.guildinfo().gid_high());
}

void GuildManager::SendGetGuildUnionInfoReq()
{
	OnlineNetworkManager::sShareInstance()->SendGetGuildUnionInfoReq();
}

void GuildManager::SendGetGuildChooseInfoReq(int instanceId)
{
	OnlineNetworkManager::sShareInstance()->SendGetGuildChooseInfoReq(instanceId);
}

void GuildManager::SendCreateGuildInstanceReq(int instanceId, bool hasChoose, CSPlayerGuildInfo info)
{
	OnlineNetworkManager::sShareInstance()->SendCreateGuildInstanceReq(instanceId, hasChoose, info);
}

void GuildManager::SendGetGuildRedEnvoInfoReq()
{
	OnlineNetworkManager::sShareInstance()->SendGetGuildRedEnvoInfoReq();
}

void GuildManager::SendFirstSendRedEnvoReq(unsigned int envoID, unsigned int envoType)
{
	OnlineNetworkManager::sShareInstance()->SendFirstSendRedEnvoReq(envoID, envoType);
}

void GuildManager::SendSecondSendRedEnvoReq(unsigned int envo_id, unsigned int envo_type)
{
	OnlineNetworkManager::sShareInstance()->SendSecondSendRedEnvoReq(envo_id, envo_type);
}

void GuildManager::SendGetRedEnvoReq(CSGuildRedPacketKey key)
{
	OnlineNetworkManager::sShareInstance()->SendGetRedEnvoReq(key);
}

void GuildManager::SendGetRedEnvoRecordReq(CSGuildRedPacketKey key)
{
	OnlineNetworkManager::sShareInstance()->SendGetRedEnvoRecordReq(key);
}

void GuildManager::SendGetRedEnvoConfigReq()
{
	OnlineNetworkManager::sShareInstance()->SendGetRedEnvoConfigReq();
}

void GuildManager::onGuildRedEnvoInfo(CSGuildRedPacketQueryRsp* msg)
{
	m_vecGuildRedEnvolopeInfo.clear();
	m_isShowRedTip = false;

	for (int i = 0; i < msg->infos_size(); i++)
	{
		GuildGetRedEnvolopeInfo info;
		info.key.user_id = msg->infos(i).key().userid();
		info.key.channel_id = msg->infos(i).key().channelid();
		info.key.reg_tm = msg->infos(i).key().reg_tm();
		info.key.send_tm = msg->infos(i).key().send_tm();
		info.key.zone_id = msg->infos(i).key().zone_id();
		info.envo_id = msg->infos(i).packet_id();
		info.envo_type = msg->infos(i).packet_type();
		info.rest_times = msg->infos(i).surplus_nu();
		info.sender_name = msg->infos(i).name();
		info.isGeted = msg->infos(i).is_draw();
		info.real_dia = msg->infos(i).real_diamond();

		if (info.isGeted == false && info.rest_times > 0)
		{
			m_isShowRedTip = true;
		}

		m_vecGuildRedEnvolopeInfo.push_back(info);
	}

    processTip();

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetGuildRedEnvoInfo");
}

void GuildManager::onGetRedEnvoRsp(CSGuildRedPacketGetAwardRsp* msg)
{ 
	m_guildGetRedEnvoRsp = GetRedEnvoRspStruct();

	m_guildGetRedEnvoRsp.dia_count = msg->diamond_count();
	m_guildGetRedEnvoRsp.suc = msg->suc();

	for (int i = 0; i < msg->awards().size(); i++)
	{
		RedEnvoAwardStruct award;

		award.item_count = msg->awards(i).item_count();
		award.item_id = msg->awards(i).item_id();
		m_guildGetRedEnvoRsp.awards.push_back(award);
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetRedEnvoRsp");
}

void GuildManager::onGetRedEnvoRecordRsp(CSGuildRedPacketRecordRsp* msg)
{
	RedEnvoRecordInfo info;
	m_vecGuildRedEnvoRecordInfo.clear();

	for (int i = 0; i < msg->logs_size(); i++)
	{
		info.log = msg->logs(i).log();
		info.modify_tm = msg->logs(i).modify_tm();

		m_vecGuildRedEnvoRecordInfo.push_back(info);
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetRedEnvoRecordRsp");
}

void GuildManager::onGetRedEnvoConfigRsp(CSGuildRedPacketConfigRsp* msg)
{
	GuildRedEnvolopeInfo info;
	m_vecGuildRedEnvolopeinfoFromXML.clear();
	m_vecGuildTuHaoRedEnvoInfo.clear();

	for (int i = 0; i < msg->configs_size(); i++)
	{
		info.activity_date = msg->configs(i).activity_date();
		info.activity_qualification = msg->configs(i).activity_qualification();
		info.activity_time = msg->configs(i).activity_time();
		info.award1 = msg->configs(i).award1();
		info.award2 = msg->configs(i).award2();
		info.award3 = msg->configs(i).award3();
		info.envo_id = msg->configs(i).envelopeid();
		info.envo_type = msg->configs(i).envelope_type();
		info.labelContent = msg->configs(i).extent();
		info.time_type = msg->configs(i).time_type();
		info.iRestTimes = msg->configs(i).remain_times();

		if (info.envo_type == 1)
			m_vecGuildTuHaoRedEnvoInfo.push_back(info);

		m_vecGuildRedEnvolopeinfoFromXML.push_back(info);
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetRedEnvoConfigRsp");
}

void GuildManager::onFirstSendRedEnvoRsp(CSGuildRedPacketFirstCheckRsp* msg)
{
	m_firstSendRedEnvoRsp.all_send_count = msg->acc_packet_diamond();
	m_firstSendRedEnvoRsp.suc = msg->is_succ();

	CCNotificationCenter::sharedNotificationCenter()->postNotification("FirstSendRedEnvo");
}

void GuildManager::onSecondSendRedEnvoRsp(CSGuildRedPacketSendRsp* msg)
{
	m_secondSendRedEnvoRsp.suc = msg->suc();
	m_secondSendRedEnvoRsp.acc_red_envo_dia = msg->acc_packet_diamond();
	m_secondSendRedEnvoRsp.envo_type = msg->packet_type();
	m_secondSendRedEnvoRsp.real_dia_count = msg->real_diamond();
	m_secondSendRedEnvoRsp.envo_id = msg->packet_id();
}

void GuildManager::onGuildRecordInfo(CSGuildRecordListRsp* msg)
{
	m_vecGuildRecordInfo.clear();

	for (int i = 0; i < msg->logs_size(); i++)
	{
		GuildRecordInfoStruct info;
		info.record_type = msg->logs(i).record_type();
		info.modify_tm = msg->logs(i).modify_tm();
		info.log = msg->logs(i).log();

		m_vecGuildRecordInfo.push_back(info);
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetGuildRecordInfo");
}

void GuildManager::onGuildList(CSListGuildRsp* msg)
{
    unsigned int beginId = msg->begin();
    m_totalGuildAmount = msg->guild_num();
    for (int i = 0; i < msg->infos_size(); ++i)
    {
        GuildListInfo info;
        info.hightGid = msg->infos(i).gid_high();
        info.lowGid = msg->infos(i).gid_low();
        info.contribute = msg->infos(i).contribute();
        info.exp = msg->infos(i).exp();
        info.fightcondition = msg->infos(i).fightcondition();
        info.conditionType = msg->infos(i).join_type();
        info.gname = msg->infos(i).gname();
        info.leader_name = msg->infos(i).oname();
        info.g_notice = msg->infos(i).note();
        info.lv = msg->infos(i).lv();
        info.total_power = msg->infos(i).power_score();
        info.member_num = msg->infos(i).member_num();
        info.member_num_limit = msg->infos(i).member_num_limit();
        info.isRequired = msg->infos(i).isrequired();
        info.lead_id = msg->infos(i).ouserid();
        info.lead_reg_time = msg->infos(i).oreg_tm();
        info.lead_channel = msg->infos(i).ochannel_id();
        
        m_allGuildList[beginId + i] = info;
    }

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_GUILD_LIST, this);
}

void GuildManager::onGuildUnionInfo(CSUnionInfoRsp* msg)
{
	m_vecGuildUnionInfo.clear();

	for (int i = 0; i < msg->infos_size(); i++)
	{
		GuildUnionInfo info;
		info.level = msg->infos(i).level();
		info.exp = msg->infos(i).exp();
		info.people_num = msg->infos(i).people_num();
		info.chairman_num = msg->infos(i).chairman_num();
		info.elite_num = msg->infos(i).elite_num();
		info.donate = msg->infos(i).donate();
		info.benedition = msg->infos(i).benedition();
		info.shop = msg->infos(i).shop();
		info.transcript = msg->infos(i).transcript();
		info.target = msg->infos(i).target();
		info.boss = msg->infos(i).boss();
		info.melee = msg->infos(i).melee();
		info.scramble = msg->infos(i).scramble();
		info.redenvelope = msg->infos(i).redenvelope();

		m_vecGuildUnionInfo.push_back(info);
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetGuildUnionInfo");
}


void GuildManager::onGuildChooseInfo(CSGetRecruitMemberRsp* msg)
{
	m_vecGuildChooseInfo.clear();

	for (int i = 0; i < msg->infos_size(); i++)
	{
		m_vecGuildChooseInfo.push_back(msg->infos(i));
	}

	CCNotificationCenter::sharedNotificationCenter()->postNotification("GetGuildChooseInfo");
}

void GuildManager::onGuildSearchList(CSSearchGuildRsp* msg)
{
    m_allSearchList.clear();

    for (int i = 0; i < msg->infos_size(); ++i)
    {
        GuildListInfo info;
        info.hightGid = msg->infos(i).gid_high();
        info.lowGid = msg->infos(i).gid_low();
        info.contribute = msg->infos(i).contribute();
        info.exp = msg->infos(i).exp();
        info.fightcondition = msg->infos(i).fightcondition();
        info.gname = msg->infos(i).gname();
        info.leader_name = msg->infos(i).oname();
        info.g_notice = msg->infos(i).note();
        info.lv = msg->infos(i).lv();
        info.total_power = msg->infos(i).power_score();
        info.member_num = msg->infos(i).member_num();
        info.member_num_limit = msg->infos(i).member_num_limit();
        info.conditionType = msg->infos(i).join_type();

        m_allSearchList[i] = info;
    }

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_GUILD_LIST, this);
}

void GuildManager::onGuildCreateResult(CSCreateGuildRsp* msg)
{
    int res = msg->res();
    if (res == 0)
    {
        NotificationCenter::defaultCenter()->broadcast(GM_CREATE_GUILD_SUCCESS, this);
    }
}

void GuildManager::onGuildJoinResult(CSJoinGuildRsp* msg)
{
    int res = msg->res();
    if (res == 0)
    {
        ShowMessage(Localizatioin::getLocalization("G_APPLICATION_ERROR4"));
        NotificationCenter::defaultCenter()->broadcast(GM_CREATE_GUILD_SUCCESS, this);
    }
}
 
void GuildManager::onGetGuildInfo(CSQueryPlayerGuildInfoRsp* msg)
{
    m_playerGuildInfo = (*msg);

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_GUILD_INFO, this);

    // show tip clamation
    if(UserData::GetUserInfo().m_guildId.getHighID() == 0 && UserData::GetUserInfo().m_guildId.getLowID() == 0)
    {
        HideMainTipIcon();
    }
    else
    {
        int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
        int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
        int requestListnum = getRequestMemberList().size();
        if((1 - contributeTimes) == 0 && blessTiems > 0 && requestListnum == 0 && m_isShowRedTip == false)
        {
            HideMainTipIcon();
        }
        else
        {
            ShowMainTipIcon();
        }
    }

    processTip();
}

void GuildManager::processTip()
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        int tag = mLayer->getLayerTagByName("GuildInfoLayer");
        int status = GuildManager::Get()->getPlayerOwnerGuildInfo().playerinfo().status(); // 级别
        if(dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag)))
        {
            GuildInfoLayer* layer = dynamic_cast<GuildInfoLayer*>(mLayer->getChildByTag(tag));
            int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
            int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
            int requestListnum = getRequestMemberList().size();
		
            if(1 - contributeTimes == 0)
            {
                layer->showContributeTip(false);
            }
            else
            {
                layer->showContributeTip(true);
            }

            if(blessTiems > 0)
            {
                layer->showBlessTIp(false);
            }
            else
            {
                layer->showBlessTIp(true);
            }

            if(requestListnum == 0 || status == 1)
            {
                layer->showRequestTip(false);
            }
            else
            {
                layer->showRequestTip(true);
            }

 			if (m_isShowRedTip)
			{
				layer->showRedEnvoTip(true);
			}
			else
			{
				layer->showRedEnvoTip(false);
			}
        }
    }
}

void GuildManager::onGetGuildMember(CSListPlayerGuildRsp* msg)
{
    m_allGuildMember.clear();
    for (int i = 0; i < msg->infos_size(); ++i)
    {
        GuildMemberInfo info;
        info.userid = msg->infos(i).userid();
        info.reg_tm = msg->infos(i).reg_tm();
        info.name = msg->infos(i).name();
        info.power_score = msg->infos(i).power_score();
        info.contribute = msg->infos(i).contribute();
        info.total_contribute = msg->infos(i).total_contribute();
        info.lv = msg->infos(i).lv();
        info.channel_id = msg->infos(i).channel_id();
        info.status = msg->infos(i).status();
        info.online = msg->infos(i).online();

        m_allGuildMember[i] = info;
    }

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_GUILD_MEMBER, this);

    // show tip clamation
    if(UserData::GetUserInfo().m_guildId.getHighID() == 0 && UserData::GetUserInfo().m_guildId.getLowID() == 0)
    {
        HideMainTipIcon();
    }
    else
    {
        int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
        int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
        int requestListnum = getRequestMemberList().size();
        if((1 - contributeTimes == 0) && blessTiems > 0 && requestListnum == 0 && !GuildManager::Get()->getIsHasRedEnvo())
        {
            HideMainTipIcon();
        }
        else
        {
            ShowMainTipIcon();
        }
    }

    processTip();
}
 
void GuildManager::onUpdateGuildMember(CSChangePlayerGuildStatusRsp* msg)
{
    bool ret = msg->res();
    if (ret == false)
    {
        return;
    }

    for (int i = 0; i < msg->info_size(); ++i)
    {
        PlayerGuild info = msg->info(i);

        for (std::map<int, GuildMemberInfo>::iterator it = m_allGuildMember.begin();
            it != m_allGuildMember.end(); ++it)
        {
            if (it->second.userid == info.userid() &&
                it->second.reg_tm == info.reg_tm() &&
                it->second.channel_id == info.channel_id())
            {
                it->second.status = info.status();
                break;
            }
        }
    }

    NotificationCenter::defaultCenter()->broadcast(GM_UPDATE_GUILD_MEMBER, this);

    // show tip clamation
    if(UserData::GetUserInfo().m_guildId.getHighID() == 0 && UserData::GetUserInfo().m_guildId.getLowID() == 0)
    {
        HideMainTipIcon();
    }
    else
    {
        int contributeTimes = UserData::GetUserInfo().m_guild_contribute_times;
        int blessTiems = UserData::GetUserInfo().m_guild_bless_times;
        int requestListnum = getRequestMemberList().size();
        if((1 - contributeTimes == 0) && blessTiems > 0 && requestListnum == 0 && !GuildManager::Get()->getIsHasRedEnvo())
        {
            HideMainTipIcon();
        }
        else
        {
            ShowMainTipIcon();
        }
    }

    processTip();
}

void GuildManager::onGetGuildShopItems(CSGetShoppingItemsRsp* msg)
{
    if(msg->items_size() != 1)
    {
        m_vecGuildShopItems.clear();
    }
    
    for(int i = 0; i < msg->items_size(); i ++)
    {
        GuildShopItems info;
        info.times = msg->items(i).buytimes();
        info.max_times = msg->items(i).times();
        info.guildLvlimit = msg->items(i).limit();
        info.itemindex = msg->items(i).index();
        info.fromitemId = msg->items(i).from_item().item_id();
        info.fromamount = msg->items(i).from_item().item_num();
        info.toitemId = msg->items(i).to_item().item_id();
        info.toamount = msg->items(i).to_item().item_num();
        if(msg->items_size() == 1)
        {
            ShowMessage(Localizatioin::getLocalization("M_BUG_SUCCESS"));
            int itemid = info.toitemId;
            int itemCount = info.toamount;
            std::string itemName = ItemManager::Get()->getItemInfoById(itemid)->m_name;
            std::string tempStr = Localizatioin::getLocalization("M_PK_INTRODUCTION_2");
            tempStr.append(" ").append(itemName.c_str()).append(" x ").append(IntToStr(itemCount));
            ShowMessage(tempStr.c_str());
            UpdateShopItems(info);
            break;
        }
        m_vecGuildShopItems.push_back(info);
    }

    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuildShopItemsUpdate");
}

void GuildManager::UpdateShopItems(GuildShopItems info)
{
    for(int i = 0; i < m_vecGuildShopItems.size(); i++)
    {
        if(m_vecGuildShopItems[i].itemindex == info.itemindex)
        {
            m_vecGuildShopItems[i].times = info.times;
            m_vecGuildShopItems[i].max_times = info.max_times;
            m_vecGuildShopItems[i].guildLvlimit = info.guildLvlimit;
            m_vecGuildShopItems[i].itemindex = info.itemindex;
            m_vecGuildShopItems[i].fromitemId = info.fromitemId;
            m_vecGuildShopItems[i].fromamount = info.fromamount;
            m_vecGuildShopItems[i].toitemId = info.toitemId;
            m_vecGuildShopItems[i].toamount = info.toamount;
            break;
        }
    }
}

void GuildManager::onGetGuildBlessInfo(CSGoddessBlessRsp* msg)
{
    m_vecGuildBlessInfo.clear();
    GuildBlessInfo info;
    info.res = msg->res();
    info.nextLevel = msg->next_level();
    info.fromItems.clear();
    for(int i = 0; i < msg->from_item_size(); i++)
    {
        int fromItemid = msg->from_item(i).item_id();
        int fromItemAmount = msg->from_item(i).item_num();
        std::vector<int> tempVecFrom;
        tempVecFrom.clear();
        tempVecFrom.push_back(fromItemid);
        tempVecFrom.push_back(fromItemAmount);
        info.fromItems.push_back(tempVecFrom);
    }

    // to 
    info.toItems.clear();
    for(int j = 0; j < msg->to_item_size(); j++)
    {
        int toItemid = msg->to_item(j).item_id();
        int toItemAmout = msg->to_item(j).item_num();
        std::vector<int> tempVecTo;
        tempVecTo.clear();
        tempVecTo.push_back(toItemid);
        tempVecTo.push_back(toItemAmout);
        info.toItems.push_back(tempVecTo);
    }

    // to next
    info.toNextItems.clear();
    for(int k = 0; k < msg->next_to_item_size(); k++)
    {
        int toNextItemid = msg->next_to_item(k).item_id();
        int toNextItemAmout = msg->next_to_item(k).item_num();
        std::vector<int> tempVecNextTo;
        tempVecNextTo.clear();
        tempVecNextTo.push_back(toNextItemid);
        tempVecNextTo.push_back(toNextItemAmout);
        info.toNextItems.push_back(tempVecNextTo);
    }
    m_vecGuildBlessInfo.push_back(info);

    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuildBlessInfo_update");
}

void GuildManager::onGetGuildContributeInfo(CSExchangeItemInfoRsp* msg)
{
    m_vecGuildContributeInfo.clear();
    for(int i = 0; i < msg->items_size(); i++)
    {
        GuildContributeInfo info;
        info.type = msg->items(i).type();
        info.fromItemid = msg->items(i).from_item(0).item_id();
        info.fromItemAmount = msg->items(i).from_item(0).item_num();
        info.toItemId = msg->items(i).to_item(0).item_id();
        info.toItemAmount = msg->items(i).to_item(0).item_num();
        info.toItemId2 = msg->items(i).to_item(1).item_id();
        info.toItemAmount2 = msg->items(i).to_item(1).item_num();

        m_vecGuildContributeInfo.push_back(info);
    }

    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuildContributeInfo_update");
}
 
void GuildManager::onRspQuitGuild()
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildMemberLayer(false);
    }
    ShowMessage(Localizatioin::getLocalization("G_QUIT_ERROR1"));
    HideMainTipIcon();
}

void GuildManager::onRspDropGuild()
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildMemberLayer(false);
    }
    ShowMessage(Localizatioin::getLocalization("G_DISBAND_ERROR1"));
    HideMainTipIcon();
}

GuildListInfo GuildManager::getIndexGuildInfo(int index)
{
    GuildListInfo info;
    std::map<int, GuildListInfo>::iterator it = m_allGuildList.find(index);
    if (it == m_allGuildList.end())
    {
        info.hightGid = 0;
        info.lowGid = 0;
        return info;
    }

    return it->second;
}

GuildListInfo GuildManager::getIndexSearchInfo(int index)
{
    GuildListInfo info;
    std::map<int, GuildListInfo>::iterator it = m_allSearchList.find(index);
    if (it == m_allSearchList.end())
    {
        info.hightGid = 0;
        info.lowGid = 0;
        return info;
    }

    return it->second;
}

std::vector<GuildMemberInfo> GuildManager::getRequestMemberList()
{
    std::vector<GuildMemberInfo> vtInfos;
    vtInfos.clear();
    
    for (std::map<int, GuildMemberInfo>::iterator it = m_allGuildMember.begin();
         it != m_allGuildMember.end(); ++it)
    {
        if (it->second.status == 999)
        {
            vtInfos.push_back(it->second);
        }
    }

    return vtInfos;
}

std::vector<GuildMemberInfo> GuildManager::getMemberList()
{
    std::vector<GuildMemberInfo> vtInfos;
    vtInfos.clear();

    for (std::map<int, GuildMemberInfo>::iterator it = m_allGuildMember.begin();
        it != m_allGuildMember.end(); ++it)
    {
        if (it->second.status <= 4)
        {
            vtInfos.push_back(it->second);
        }
    }

    return vtInfos;
}

std::string GuildManager::getMemOfflineTimeStr(time_t timestamp)
{
    // cur servertime
    time_t serverTime = (time_t)TimeManager::Get()->getCurServerTime();
    time_t now = time(NULL);
    tm* x = localtime(&now);
    tm* serTimeNow = localtime(&serverTime);
    int serDay  = serTimeNow->tm_mday;
    int serHour = serTimeNow->tm_hour;
    int serMin  = serTimeNow->tm_min;
    int serSec  = serTimeNow->tm_sec;

    // offline time
    tm* offTimeNow = localtime(&timestamp);
    int offYear = offTimeNow->tm_year;
    int offm    = offTimeNow->tm_mon;
    int offDay  = offTimeNow->tm_mday;
    int offHour = offTimeNow->tm_hour;
    int offMin  = offTimeNow->tm_min;
    int offSec  = offTimeNow->tm_sec;

    // 计算当天自己过了多少个小时
    int todayDeltaHr = serHour;

    // 计算相差多少时间前离线的
    time_t deltaDistance = serverTime - timestamp;
    int disDeltaHr = (long)deltaDistance / 3600;

    if(disDeltaHr > todayDeltaHr)
    {
        // 显示离线日期
        CCString* str1 = CCString::createWithFormat(Localizatioin::getLocalization("G_FRIEND_ONLINE_6"), offYear + 1900, offm + 1, offDay);
        return str1->getCString();
    }
    else
    {
        // 显示几个小时前
        return getMemDetailTimeStr(disDeltaHr);
    }
}

std::string GuildManager::getMemDetailTimeStr(int hour)
{
    if(hour > 12)
    {
        // 大于12小时
        CCString* str1 = CCString::createWithFormat(Localizatioin::getLocalization("G_FRIEND_ONLINE_5"), 12);
        return str1->getCString();
    }
    else if(hour < 1)
    {
        // 刚刚
        CCString* str2 = CCString::createWithFormat(Localizatioin::getLocalization("G_FRIEND_ONLINE_3"));
        return str2->getCString();
    }
    else
    {
        // 几小时前
        CCString* str3 = CCString::createWithFormat(Localizatioin::getLocalization("G_FRIEND_ONLINE_4"), hour);
        return str3->getCString();
    }
}

std::vector<GuildShopItems> GuildManager::getGuildShopItems()
{
    return m_vecGuildShopItems;
}

std::string GuildManager::getGuildPersonTitle(int index)
{
    switch (index)
    {
    case 1:
        return Localizatioin::getLocalization("G_APPOINTMENT_POSITION1");
        break;
    case 2:
        return Localizatioin::getLocalization("G_APPOINTMENT_POSITION2");
        break;
    case 3:
        return Localizatioin::getLocalization("G_APPOINTMENT_POSITION3");
        break;
    case 4:
        return Localizatioin::getLocalization("G_APPOINTMENT_POSITION4");
        break;
    default:
        return "";
        break;
    }
}

void GuildManager::setMemberStatus(std::vector<StrGuildStatus> vec)
{
    m_pMemberStatus.clear();
    for(int i = 0; i < vec.size(); i++)
    {
        m_pMemberStatus.push_back(vec[i]);
    }
}

void GuildManager::HideMainTipIcon()
{
    UIManager *manager = UIManager::sharedManager();
    UILayout* layout = manager->getUILayout("MainMenu");
    UIButton* guildBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_sociaty");
    if (guildBtn)
    {
         guildBtn->hideTipIcon();
    }
}

void GuildManager::ShowMainTipIcon()
{
    UIManager *manager = UIManager::sharedManager();
    UILayout* layout = manager->getUILayout("MainMenu");
    UIButton* guildBtn = layout->FindChildObjectByName<TXGUI::UIButton>("button_sociaty");
    if (guildBtn)
    {
        guildBtn->showTipIcon();
    }
}

void GuildManager::setGetRedEnvoRspTag(int tag)
{
	m_guildGetRedEnvoRsp.envo_id = tag;
}

void GuildManager::setRedEnvoRecordTag(int tag)
{
	for (int i = 0; i < m_vecGuildRedEnvoRecordInfo.size(); i++)
	{
		m_vecGuildRedEnvoRecordInfo[i].envo_id = tag;
	}
}