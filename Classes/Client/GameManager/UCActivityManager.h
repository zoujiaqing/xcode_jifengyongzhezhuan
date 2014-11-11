#ifndef _UC_ACTIVITY_MANAGER_H_
#define _UC_ACTIVITY_MANAGER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Singleton_t.h"

USING_NS_CC;
USING_NS_CC_EXT;

struct UCBonusItemData
{
    int itemId;
    int itemAmount;

    UCBonusItemData()
    {
        itemId = 0;
        itemAmount = 0;
    }
};

struct UCAwardData
{
    int id;
    std::string lookFaceId;
    int type;
    std::string taskText;
    std::vector<UCBonusItemData> bonusList;

    UCAwardData()
    {
        id = 0;
        lookFaceId = "";
        type = 0;
        taskText = "";
        bonusList.clear();
    }
};

enum UCAwardType
{
    UCAwardType_Everyday    = 1,
    UCAwardType_Only        = 2,
    UCAwardType_FirstCharge = 3,
};

class UCActivityManager : public CCObject
    , public TSingleton<UCActivityManager>
{
public:
    UCActivityManager();
    virtual ~UCActivityManager();

    UCAwardData getIndexUCAwardBonus(int typeId);

private :
    void LoadXML();
    void ParseBonus(UCAwardData& data, std::string content);

private :
    std::map<int, UCAwardData> m_ucAwardData;
};

#endif

