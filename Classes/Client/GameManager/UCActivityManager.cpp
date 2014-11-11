#include "UCActivityManager.h"
#include "tinyxml.h"
#include "GameResourceManager.h"

UCActivityManager::UCActivityManager()
{
    LoadXML();
}

UCActivityManager::~UCActivityManager()
{

}

void UCActivityManager::LoadXML()
{
    std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Data/UC_Award.xml");

    unsigned long	_size;
    char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "r", &_size);
    TiXmlDocument	*_document = new TiXmlDocument();
    _document->Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

    CC_SAFE_DELETE_ARRAY(_pFileContent);

    m_ucAwardData.clear();
    TiXmlElement *RootElement = _document->RootElement();
    if (NULL != RootElement)
    {
        TiXmlElement *childElement = RootElement->FirstChildElement();
        while (childElement != NULL)
        {
            UCAwardData data;

            // ½âÎö½±ÀøÊý¾Ý
            TiXmlAttribute* pAttribute = childElement->FirstAttribute();
            while (pAttribute != NULL)
            {
                std::string strName(pAttribute->Name());
                std::string strContent(childElement->Attribute(pAttribute->Name()));
                if (strName == "ID")
                {
                    data.id = atoi(strContent.c_str());
                } else 
                if (strName == "Task_Text")
                {
                    data.taskText = strContent;
                } else 
                if (strName == "Task_LookFace")
                {
                    data.lookFaceId = strContent;
                } else
                if (strName == "Type1")
                {
                    data.type = atoi(strContent.c_str());
                } else
                if (strName.find("RaidDrops") != -1)
                {
                    ParseBonus(data, strContent);
                }

                pAttribute = pAttribute->Next();
            }

            m_ucAwardData.insert(std::make_pair(data.type, data));
            childElement = childElement->NextSiblingElement();
        }
    }
}

void UCActivityManager::ParseBonus(UCAwardData& data, std::string content)
{
    int dot = content.find_first_of('/');
    if (dot == -1)
    {
        return;
    }

    int itemId = atoi(content.substr(0, dot).c_str());
    int amount = atoi(content.substr(dot + 1, content.size() - dot - 1).c_str());
    
    UCBonusItemData bonusItem;
    bonusItem.itemId = itemId;
    bonusItem.itemAmount = amount;
    data.bonusList.push_back(bonusItem);
}

UCAwardData UCActivityManager::getIndexUCAwardBonus(int typeId)
{
    UCAwardData data;
    std::map<int, UCAwardData>::iterator it = m_ucAwardData.find(typeId);
    if (it == m_ucAwardData.end())
    {
        data.id = -1;
        return data;
    }

    data = it->second;
    return data;
}