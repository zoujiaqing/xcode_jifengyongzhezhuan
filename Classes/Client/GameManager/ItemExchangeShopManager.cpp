#include "ItemExchangeShopManager.h"
#include "GameResourceManager.h"
#include "tinyxml.h"

ItemExchangeShopManager::ItemExchangeShopManager()
{
   // m_mpTreasureInfo.clear();
    m_headScrollPos = CCPointZero;
    InitXMLData();
}

ItemExchangeShopManager::~ItemExchangeShopManager()
{

}

void ItemExchangeShopManager::InitXMLData()
{
   /* m_mpTreasureInfo.clear();
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
    }*/
}