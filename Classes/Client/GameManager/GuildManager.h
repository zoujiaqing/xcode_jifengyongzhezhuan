#ifndef _GUILD_MANAGER_H_
#define _GUILD_MANAGER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Singleton_t.h"
#include "cs_core.pb.h"

USING_NS_CC;
USING_NS_CC_EXT;

struct GuildListInfo
{
    unsigned int hightGid;
    unsigned int lowGid;
    unsigned int exp;
    unsigned int contribute;
    unsigned int lv;
    unsigned int fightcondition;
    unsigned int conditionType;
    std::string gname;
    std::string leader_name;
    std::string g_notice;
    unsigned int total_power;
    unsigned int member_num;
    unsigned int member_num_limit;
    bool isRequired;
    unsigned int lead_id;
    unsigned int lead_reg_time;
    unsigned int lead_channel;

    GuildListInfo()
    {
        hightGid = 0;
        lowGid = 0;
        exp = 0;
        contribute = 0;
        lv = 0;
        fightcondition = 0;
        gname = "";
        leader_name = "";
        g_notice = "";
        total_power = 0;
        member_num = 0;
        lead_id = 0;
    }
};

struct GuildMemberInfo
{
    unsigned int userid;
    unsigned int reg_tm;
    unsigned int channel_id;
    std::string  name;
    unsigned int lv;
    unsigned int contribute;
    unsigned int power_score;
    unsigned int status;
    unsigned int total_contribute;
    unsigned int online;

    GuildMemberInfo()
    {
        userid = 0;
    }
};

struct StrGuildStatus
{
    unsigned int user_id;
    unsigned int reg_tm;
    unsigned int channel_id;
    unsigned int status;
};

struct GuildShopItems
{
    unsigned int itemindex;
    unsigned int toitemId;
    unsigned int toamount;
    unsigned int fromitemId;
    unsigned int fromamount;
    unsigned int times;
    unsigned int max_times;
    unsigned int guildLvlimit;
    GuildShopItems()
    {
        itemindex = -1;
    }
};

struct GuildRedEnvolopeKey
{
	unsigned int user_id;
    unsigned int reg_tm;
    unsigned int channel_id;
	unsigned int zone_id;
	unsigned int send_tm;

	GuildRedEnvolopeKey()
	{
		user_id = 0;
		reg_tm = 0;
		channel_id = 0;
		zone_id = 0;
		send_tm = 0;
	}
};

struct GuildGetRedEnvolopeInfo       //从服务器获取的可领取的红包信息
{
	GuildRedEnvolopeKey key;
	string				sender_name;
	unsigned int		envo_type;
	unsigned int		envo_id;
	unsigned int		rest_times;
	bool				isGeted;
	unsigned int		real_dia;

	GuildGetRedEnvolopeInfo()
	{ 
		sender_name = "";
		envo_type = 0;
		envo_id = 0;
		rest_times = 0;
		isGeted = false;
		real_dia = 0;
	}
};

struct GuildRedEnvolopeInfo        //从XML配表中获得的红包信息
{
	unsigned int envo_id;
	unsigned int envo_type;
	unsigned int time_type;
	string       activity_date;
	string		 activity_time;
	string		 activity_qualification;
	string		 award1;
	string		 award2;
	string		 award3;
	string		 labelContent;
	int		     iRestTimes;

	GuildRedEnvolopeInfo()
	{
		envo_id = 0;
		envo_type = 0;
		time_type = 0;
		activity_date = "";
		activity_time = "";
		activity_qualification = "";
		award1 = "";
		award2 = "";
		award3 = "";
		labelContent = "";
		iRestTimes = 0;
	}
};

struct GuildBlessInfo
{
    unsigned int res;
    std::vector< std::vector<int> > fromItems;
    std::vector< std::vector<int> > toItems;
    std::vector< std::vector<int> > toNextItems;
    unsigned int nextLevel;
    GuildBlessInfo()
    {
        res = -1;
        fromItems.clear();
        toItems.clear();
        toNextItems.clear();
    }
};

struct GuildContributeInfo
{
    unsigned int type;
    unsigned int fromItemid;
    unsigned int fromItemAmount;
    unsigned int toItemId;
    unsigned int toItemAmount;
    unsigned int toItemId2;
    unsigned int toItemAmount2;
    GuildContributeInfo()
    {
        type = -1;
    }
};

struct GuildRecordInfoStruct
{
	unsigned int record_type;
	unsigned int modify_tm;
	std::string  log;

	GuildRecordInfoStruct()
	{
		record_type = 0;
		modify_tm = 0;
		log = "";
	}
};

struct FirstSendRedEnvoRspStruct
{
	unsigned int all_send_count;
	bool suc;

	FirstSendRedEnvoRspStruct()
	{
		all_send_count = 0;
		suc = false;
	}
};

struct SecondSendRedEnvoRspStruct
{
	bool suc;
	unsigned int envo_id;
	unsigned int envo_type;
	unsigned int acc_red_envo_dia;
	unsigned int real_dia_count;

	SecondSendRedEnvoRspStruct()
	{
		suc = 0;
		envo_id = 0;
		envo_type = 0;
		acc_red_envo_dia = 0;
		real_dia_count = 0;
	}
};

struct RedEnvoAwardStruct
{
	unsigned int item_id;
	unsigned int item_count;

	RedEnvoAwardStruct()
	{
		item_id = 0;
		item_count = 0;
	}
};

struct GetRedEnvoRspStruct
{
	bool suc;
	unsigned int dia_count;
	unsigned int envo_id;
	unsigned int envo_type;
	vector<RedEnvoAwardStruct> awards;

	GetRedEnvoRspStruct()
	{
		suc = 0;
		dia_count = 0;
		envo_id = 0;
		envo_type = 0;
		awards.clear();
	}
};

struct RedEnvoRecordInfo
{
	std::string log;
	unsigned int envo_id;
	unsigned int modify_tm;
	unsigned int envo_type;

	RedEnvoRecordInfo()
	{
		log = "";
		modify_tm = 0;
		envo_id = 0;
		envo_type = 0;
	}
};

//用于记录右边tableview中每一项对应的红包
struct SingleRedEnvoInfo
{
	unsigned int envo_id;
	unsigned int send_time;
	unsigned int user_id;

	SingleRedEnvoInfo()
	{
		envo_id = 0;
		send_time = 0;
		user_id = 0;
	}
};

struct GuildUnionInfo
{
	unsigned int level;
	unsigned int exp;
	unsigned int people_num;
	unsigned int chairman_num;
	unsigned int elite_num;
	unsigned int donate;
	unsigned int benedition;
	unsigned int shop;
	unsigned int transcript;
	unsigned int target;
	unsigned int boss;
	unsigned int melee;
	unsigned int scramble;
	unsigned int redenvelope;
	GuildUnionInfo()
	{

	}
};

struct guildBaseInfo
{
    unsigned int levelid;
    unsigned int chairmanNum;
    unsigned int eliteNum;
    guildBaseInfo()
    {
        levelid = 0;
        chairmanNum = 0;
        eliteNum = 0;
    }
};

class GuildManager : public CCObject
    , public TSingleton<GuildManager>
{
public:
    GuildManager();
    virtual ~GuildManager();

    struct JobBigger
    {
        bool operator()(GuildMemberInfo a, GuildMemberInfo b)
        {
            return a.status > b.status;
        }
    };
    struct LvBigger
    {
        bool operator()(GuildMemberInfo a, GuildMemberInfo b)
        {
            return a.lv > b.lv;
        }
    };
    struct PowerBigger
    {
        bool operator()(GuildMemberInfo a, GuildMemberInfo b)
        {
            return a.power_score > b.power_score;
        }
    };
    struct ContributeBigger
    {
        bool operator()(GuildMemberInfo a, GuildMemberInfo b)
        {
            return a.total_contribute > b.total_contribute;
        }
    };
    struct StatusBigger
    {
        bool operator()(GuildMemberInfo a, GuildMemberInfo b)
        {
            if(a.online == 0 || b.online == 0)
            {
                return a.online < b.online;
            }
            else
            {
                return a.online > b.online;
            }
            
        }
    };


    // send message
    void SendGetGuildList(unsigned int begin, unsigned int end);
    void SendSearchIndexGuild(std::string searchName);
    void SendJoinGuild(int highGid, int lowGid);
    void SendCreateGuild(std::string guild_name);
    void SendGetGuildInfo();
    void SendModifyNotice(std::string notice, std::string note);
    void SendGetGuildMember();
    void SendChangeGuildMemberStatus(std::vector<StrGuildStatus> vtStatus);
    void SendQuitGuild();
    void SendJiesanGuld();
    void SendGetGuildShopItems(int type = 0);
    void SendGetGuildBlessInfo(bool flag);
    void SendGetGuildContributeInfo(std::vector<int> temp);
    void SendGuildContribute(int type);
    void SenderGuildLApplySettting(int type, int fightNum = 0);
    void SendGuildBuy(int id, int index);
	void SendGetGuildRecordInfoReq();
	void SendGetGuildUnionInfoReq();
	void SendGetGuildChooseInfoReq(int instanceId);
	void SendCreateGuildInstanceReq(int instanceId, bool hasChoose, CSPlayerGuildInfo info);
	void SendGetGuildRedEnvoInfoReq();
	void SendFirstSendRedEnvoReq(unsigned int envoID, unsigned int envoType);
	void SendSecondSendRedEnvoReq(unsigned int envo_id, unsigned int envo_type);
	void SendGetRedEnvoReq(CSGuildRedPacketKey key);
	void SendGetRedEnvoRecordReq(CSGuildRedPacketKey key);
	void SendGetRedEnvoConfigReq();

    // recieve message
    void onGuildList(CSListGuildRsp* msg);
    void onGuildSearchList(CSSearchGuildRsp* msg);
    void onGuildCreateResult(CSCreateGuildRsp* msg);
    void onGuildJoinResult(CSJoinGuildRsp* msg);
    void onGetGuildInfo(CSQueryPlayerGuildInfoRsp* msg);
    void processTip();
    void onGetGuildMember(CSListPlayerGuildRsp* msg);
    void onUpdateGuildMember(CSChangePlayerGuildStatusRsp* msg);
    void onGetGuildShopItems(CSGetShoppingItemsRsp* msg);
    void onGetGuildBlessInfo(CSGoddessBlessRsp* msg);
    void onGetGuildContributeInfo(CSExchangeItemInfoRsp* msg);
    void onRspQuitGuild();
    void onRspDropGuild();
	void onGuildRecordInfo(CSGuildRecordListRsp* msg);
	void onGuildUnionInfo(CSUnionInfoRsp* msg);
	void onGuildChooseInfo(CSGetRecruitMemberRsp* msg);
	void onGuildRedEnvoInfo(CSGuildRedPacketQueryRsp* msg);
	void onFirstSendRedEnvoRsp(CSGuildRedPacketFirstCheckRsp* msg);
	void onSecondSendRedEnvoRsp(CSGuildRedPacketSendRsp* msg);
	void onGetRedEnvoRsp(CSGuildRedPacketGetAwardRsp* msg);
	void onGetRedEnvoRecordRsp(CSGuildRedPacketRecordRsp* msg);
	void onGetRedEnvoConfigRsp(CSGuildRedPacketConfigRsp* msg);

    // interface
    GuildListInfo getIndexGuildInfo(int index);
    GuildListInfo getIndexSearchInfo(int index);
    CSQueryPlayerGuildInfoRsp getPlayerOwnerGuildInfo() { return m_playerGuildInfo; }
    unsigned int getTotalGuildAmount() { return m_totalGuildAmount; }
    unsigned int getTotalSearchCount() { return m_allSearchList.size(); }
    std::vector<GuildMemberInfo> getRequestMemberList();
    std::vector<GuildMemberInfo> getMemberList();
    std::string getMemOfflineTimeStr(time_t timestamp);
    std::string getMemDetailTimeStr(int hour);

    std::vector<GuildShopItems> getGuildShopItems();
    void UpdateShopItems(GuildShopItems info);
    std::vector<GuildBlessInfo> getGuildBlessInfo() { return m_vecGuildBlessInfo;}
    std::vector<GuildContributeInfo> getGuildContributeInfo() { return m_vecGuildContributeInfo;}
    std::string getGuildPersonTitle(int index);
    void setMemberStatus(std::vector<StrGuildStatus> vec);
    std::vector<StrGuildStatus> getMemberStatus(){ return m_pMemberStatus;}  
    void HideMainTipIcon();
    void ShowMainTipIcon();

	std::vector<GuildRecordInfoStruct> getRecordInfo(){ return m_vecGuildRecordInfo; }
	std::vector<GuildUnionInfo> getUnionInfo(){ return m_vecGuildUnionInfo; }
	std::vector<CSPlayerGuildInfo> getChooseInfo(){ return m_vecGuildChooseInfo; }

	std::vector<GuildGetRedEnvolopeInfo> getRedEnvolopeInfo(){ return m_vecGuildRedEnvolopeInfo; }

	void setRedEnvolopeInfo(unsigned int index, unsigned int resttime, unsigned int name) {}

	std::vector<GuildRedEnvolopeInfo> getRedEnvolopeInfoFromXML() { return m_vecGuildRedEnvolopeinfoFromXML; }

	std::vector<GuildRedEnvolopeInfo> getTuHaoRedEnvolopeInfoFrom() { return m_vecGuildTuHaoRedEnvoInfo; }

	int getAllSendRedEnvoCount() { return m_iAllSendRedEnvoCount; }

	GetRedEnvoRspStruct getGetRedEnvoRsp() { return m_guildGetRedEnvoRsp; }
	void setGetRedEnvoRspTag(int tag);

	std::vector<RedEnvoRecordInfo> getRedEnvoRecordInfo() { return m_vecGuildRedEnvoRecordInfo; }
	void setRedEnvoRecordTag(int tag);

	void setCurrentRedEnvoSenderName(string name) { m_redEnvoSenderName = name; }
	string getCurrentRedEnvoSenderName() { return m_redEnvoSenderName; }

	SecondSendRedEnvoRspStruct getSecondSendRedEnvoRsp() { return m_secondSendRedEnvoRsp; }

	FirstSendRedEnvoRspStruct getFirstSendRedEnvoRsp() { return m_firstSendRedEnvoRsp; }



	bool getIsHasRedEnvo() { return m_isShowRedTip; }

    void InitXMLData();
    int getAppointSecChariManNumByLevel(int glevel);
    int getAppointEliteNumByLevel(int glevel);

private :
    std::map<int, GuildListInfo> m_allGuildList;
    std::map<int, GuildListInfo> m_allSearchList;
    std::map<int, GuildMemberInfo> m_allGuildMember;
    std::vector<GuildShopItems> m_vecGuildShopItems;
    std::vector<GuildBlessInfo> m_vecGuildBlessInfo;
    std::vector<GuildContributeInfo> m_vecGuildContributeInfo;
	std::vector<GuildRecordInfoStruct> m_vecGuildRecordInfo;
	std::vector<GuildUnionInfo> m_vecGuildUnionInfo;
	std::vector<CSPlayerGuildInfo> m_vecGuildChooseInfo;
	std::vector<GuildGetRedEnvolopeInfo> m_vecGuildRedEnvolopeInfo;
	std::vector<GuildRedEnvolopeInfo> m_vecGuildRedEnvolopeinfoFromXML;
	std::vector<GuildRedEnvolopeInfo> m_vecGuildTuHaoRedEnvoInfo;
	std::vector<RedEnvoRecordInfo> m_vecGuildRedEnvoRecordInfo;
	GetRedEnvoRspStruct m_guildGetRedEnvoRsp;
	SecondSendRedEnvoRspStruct m_secondSendRedEnvoRsp;
	FirstSendRedEnvoRspStruct  m_firstSendRedEnvoRsp;
	int m_iAllSendRedEnvoCount;
    std::map<int, guildBaseInfo> m_GuildBaseInfo;

	string m_redEnvoSenderName;

    CSQueryPlayerGuildInfoRsp m_playerGuildInfo;
    unsigned int m_totalGuildAmount;

    CC_SYNTHESIZE(bool, m_pIsRankNeedOpen, IsRankNeedOpen);
    CC_SYNTHESIZE(int,  m_pMemberOpenType, MemberOpenType);
    CC_SYNTHESIZE(int,  m_pIsGuildInfoNeedOpen, IsGuildInfoNeedOpen);
    std::vector<StrGuildStatus> m_pMemberStatus;
    CC_SYNTHESIZE(std::string, m_pStr1, Str1);
    CC_SYNTHESIZE(std::string, m_pStr2, Str2);
    CC_SYNTHESIZE(bool, m_isKickMember, IsKickMemeber);

	bool m_isShowRedTip;
};

#endif

