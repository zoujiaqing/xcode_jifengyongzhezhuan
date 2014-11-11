#include "TreasureExploreManager.h"
#include "GameResourceManager.h"
#include "tinyxml.h"
#include "InterfaceManager.h"

TreasureExploreManager::TreasureExploreManager()
{
    m_mpTreasureInfo.clear();
    m_forwardRate = 0;
    m_forwardAlreadyIdx = 0;
    InitXMLData();
    m_vecTreasureItems.clear();
    //m_refreshTimes = 0;
	m_resetTimes = 0;
    m_refreshOpenLv = 0;
}

TreasureExploreManager::~TreasureExploreManager()
{

}

void TreasureExploreManager::InitXMLData()
{
    m_mpTreasureInfo.clear();
    std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Data/Treasure.xml");

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
            treasureInfo element;
            TiXmlAttribute* pAttribute = childElement->FirstAttribute();
            while (pAttribute != NULL)
            {
                std::string strName(pAttribute->Name());
                std::string strContent(childElement->Attribute(pAttribute->Name()));
                if (strName == "ID")
                {
                    element.id = atoi(strContent.c_str());
                }
                else if(strName == "Type")
                {
                     element.type = atoi(strContent.c_str());
                }
                else if(strName == "Item")
                {
                    element.item = strContent.c_str();
                }
                else if(strName == "Free_Number")
                {
                    element.freeTimes = atoi(strContent.c_str());
                }
                pAttribute = pAttribute->Next();
            }

            m_mpTreasureInfo.insert(std::make_pair(element.type, element));

            childElement = childElement->NextSiblingElement();
        }
    }
}

int TreasureExploreManager::GetTreasureMapId(int mapIndex)
{
    std::map<int, treasureInfo>::iterator it = m_mpTreasureInfo.find(mapIndex);
    if(it == m_mpTreasureInfo.end())
    {
        return -1;
    }
    return it->second.id;
}

CCSpriteFrame* TreasureExploreManager::GetMapSpriteFrame(int mapIndex)
{
    CCString* str = CCString::createWithFormat("ui_explore_map_%d.png", mapIndex);
    CCSpriteFrame* frame = InterfaceManager::Get()->getSpriteFrameFromFile("ccbResources/treasureexplore.plist", str->getCString());
    if(!frame)
    {
        return NULL;
    }
    return frame;
}

int TreasureExploreManager::GetTreasureItemId(int mapIndex)
{
    // get base xml info id
    //std::map<int, treasureInfo>::iterator it = m_mpTreasureInfo.find(mapIndex);
    //if(it == m_mpTreasureInfo.end())
    //{
    //    return -1;
    //}
    //std::string str = it->second.item;
    //int index = str.find_first_of("/");
    //int itemid = atoi(str.substr(0, index).c_str());
    //return itemid;

    // get server push item id 
    if(m_vecTreasureItems.size() == 0 || (mapIndex - 1) >= m_vecTreasureItems.size())
    {
        return -1;
    }
    return m_vecTreasureItems[mapIndex - 1].id;
}
int TreasureExploreManager::GetTreasureItemCount(int mapIndex)
{
    //std::map<int, treasureInfo>::iterator it = m_mpTreasureInfo.find(mapIndex);
    //if(it == m_mpTreasureInfo.end())
    //{
    //    return -1;
    //}
    //std::string str = it->second.item;
    //int index = str.find_first_of("/");
    //int count = atoi(str.substr(index + 1, str.length() - index - 1).c_str());
    //return count;

     // get server push item id 
    if(m_vecTreasureItems.size() == 0 || (mapIndex - 1) >= m_vecTreasureItems.size())
    {
        return -1;
    }
    return m_vecTreasureItems[mapIndex - 1].num;
}
int TreasureExploreManager::GetCurMapFreeTimes(int mapIndex)
{
    std::map<int, treasureInfo>::iterator it = m_mpTreasureInfo.find(mapIndex);
    if(it == m_mpTreasureInfo.end())
    {
        return -1;
    }
    return it->second.freeTimes;
}

void TreasureExploreManager::onReceiveTreasureItems(CSNotifyItemExploreRewardRsp * msg)
{
    m_vecTreasureItems.clear();
    for(int i = 0; i < msg->items_size(); i++)
    {
        tresureItems items;
        items.id = msg->items(i).item_id();
        items.num = msg->items(i).item_num();
        m_vecTreasureItems.push_back(items);
    }
	setIsReset(!msg->isfresh());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("refreshTreasureItems");    
}