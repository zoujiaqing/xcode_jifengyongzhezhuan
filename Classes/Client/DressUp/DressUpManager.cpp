#include <libxml/parser.h>
#include "DressUpManager.h"
#include "GameResourceManager.h"
#include "UserData.h"
#include "SASpriteDefine.h"

DressUpManager::DressUpManager()
{
	mIsLoadData = false;

	mVecAdaptedString.push_back("wuqi_");
}

DressUpManager::~DressUpManager()
{
	mIsLoadData = false;
	mIDAndEquipNames.clear();
}

bool DressUpManager::SplitStringWithDot(std::string content,std::vector<std::string> &vecOut)
{
	int startPos = 0;
	int endPos = content.find(";");
	while(std::string::npos != endPos)
	{
		std::string sub_content = content.substr(startPos,endPos-startPos);
		vecOut.push_back(sub_content);

		startPos = endPos + 1;
		endPos = content.find(";",startPos);
	}
	endPos = content.rfind(";");
	std::string endContent = content.substr(endPos+1,content.length() - endPos - 1);
	vecOut.push_back(endContent);
	return true;
}

bool DressUpManager::LoadXML()
{
	const char* pFile = "Data/clothchange.bin";
	char *xmlString =(char*) GameResourceManager::sharedManager()->loadXml(pFile);
	xmlDocPtr pDoc = xmlReadMemory(xmlString, strlen(xmlString), NULL, "UTF-8", XML_PARSE_RECOVER);
	xmlNodePtr pRoot = xmlDocGetRootElement(pDoc);

	if(pRoot != NULL)
	{
		xmlNodePtr pChildrenNode = pRoot->children;
		while (pChildrenNode != NULL)
		{
			if(xmlStrcmp(pChildrenNode->name, BAD_CAST"clothchange") == 0)
			{
				xmlAttrPtr attrPtr = pChildrenNode->properties;

				unsigned int roleLevel = 0;
				std::string weaponName = "";

				while (attrPtr != NULL)
				{
					if (!xmlStrcmp(attrPtr->name, BAD_CAST "Weapon_id"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "Weapon_id");
						roleLevel = (unsigned int)atoi((const char*)szAttr);						
						xmlFree(szAttr);
					}
					else if (!xmlStrcmp(attrPtr->name, BAD_CAST "Weapon_name"))
					{
						xmlChar* szAttr = xmlGetProp(pChildrenNode,BAD_CAST "Weapon_name");
						weaponName = std::string((const char*)szAttr);
						xmlFree(szAttr);
					}

					attrPtr = attrPtr->next;
				}

				std::vector<std::string> vecOut;
				SplitStringWithDot(weaponName,vecOut);
				mIDAndEquipNames.insert(std::make_pair(roleLevel,vecOut));

			}
			pChildrenNode = pChildrenNode->next;
		}
	}

	free(xmlString);
	xmlFreeDoc(pDoc);

	return true;
}

bool DressUpManager::GetHeroEquipByID(unsigned int id,std::vector<std::string> &vecOut)
{
	std::map<unsigned int,std::vector<std::string> >::iterator iter = mIDAndEquipNames.find(id);
	if (iter != mIDAndEquipNames.end())
	{
		vecOut = (*iter).second;
		return true;
	}
	
	return false;
}

bool DressUpManager::GetAdaptedImageName(std::string &srcImageName,std::vector<std::string> &sourceVecNames,std::string &outImageName)
{
	int index = 0;
	int sourceVecSize = sourceVecNames.size();
	if (sourceVecSize == 0)
	{
		return false;
	}

	for (std::vector<std::string>::iterator iter = mVecAdaptedString.begin();
		iter != mVecAdaptedString.end(); iter++)
	{
		if (std::string::npos != srcImageName.find((*iter)))
		{			
			if (index <= sourceVecSize-1)
			{
				int startPos = srcImageName.rfind(":");
				std::string headerString = srcImageName.substr(0,startPos+1);
				outImageName = headerString + sourceVecNames[index] + std::string(SKELETON_IMAGE_FILE_SUFFIX); 
				return true;
			}
		}
		index++;
	}
	return false;
}