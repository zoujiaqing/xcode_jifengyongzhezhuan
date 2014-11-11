#include "AppDelegate.h"
#include "SkeletonAnimRcsManager.h"
#include "GameResourceManager.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "SASpriteDefine.h"
#include "CConnectionData.h"
#include "LevelManager.h"
#include "CProcessBase.h"
#include "MainLandManager.h"
#include "ParticleManager.h"

#include "GameConfigFromLuaManager.h"
#include "GameManager.h"

#include "GameAudioManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <direct.h>
#include <io.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "SystemInfoJni.h"
#include <sys/stat.h>
#include <sys/types.h>

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#endif

USING_NS_CC;

static SkeletonAnimRcsManager* _roleActorManager = 0;

std::string stringAppendMethod(std::string str,const char *suffix)
{
	if (0 ==suffix)
	{
		return 0;
	}	
	return std::string(str+std::string(suffix));
}

SkeletonAnimRcsManager* SkeletonAnimRcsManager::getInstance(void)
{
	if(_roleActorManager == 0)
	{
		_roleActorManager = new SkeletonAnimRcsManager();
	}
	return _roleActorManager;
}

void SkeletonAnimRcsManager::Destroy()
{
	if (_roleActorManager)
	{
		delete _roleActorManager;
		_roleActorManager = NULL;
	}
}


SkeletonAnimRcsManager::SkeletonAnimRcsManager():
mapAttachData(NULL),
roleArmatureRcsData(NULL),
roleArmatureActionData(NULL),
m_isLoadAllData(false)
{
	if (init())
	{
		LoadSourceDataFromXML();
	}
}

void SkeletonAnimRcsManager::LoadSourceDataFromXML()
{
	std::string filePath = std::string("Data/") + std::string("RoleActionConfig.xml");
	TiXmlElement *RootElement = initXML(filePath.c_str());
	if (0 != RootElement)
	{
		TiXmlElement *childElement = RootElement->FirstChildElement();
		while (0 != childElement)
		{
			const char *elementValue = childElement->Value();
			if (0 == strcmp(elementValue,"Map"))
			{
				readMapAttachedRcsDataFromFile(childElement);
			}
			else if (0 == strcmp(elementValue,"RcsFiles"))
			{
				readRcsDataFromFile(childElement);
			}
			else if (0 == strcmp(elementValue,"RoleActions"))
			{
				readArmatureDataFromFile(childElement);
			}
			childElement = childElement->NextSiblingElement();
		}
	}
	TiXmlDocument *doc = RootElement->GetDocument();
	doc->Clear();
	CC_SAFE_DELETE(doc);
}

SkeletonAnimRcsManager::~SkeletonAnimRcsManager()
{
	clearAllData();
}

void SkeletonAnimRcsManager::clearAllData()
{
	CC_SAFE_DELETE(roleArmatureRcsData);
	CC_SAFE_DELETE(roleArmatureActionData);
	CC_SAFE_DELETE(mapAttachData);

	CCConnectionData::purgeConnectionData();
}

bool SkeletonAnimRcsManager::init()
{

	roleArmatureRcsData = new RoleArmatureRcsData();
	if (0 == roleArmatureRcsData)
	{
		return false;
	}
	roleArmatureActionData = new RoleArmatureActionData();
	if (0 == roleArmatureActionData)
	{
		return false;
	}
	mapAttachData = new MapAttachedData();
	if (0 == mapAttachData)
	{
		return false;
	}
	return true;
}

TiXmlElement * SkeletonAnimRcsManager::initXML(const char * filePath)
{
	std::string fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filePath);

	unsigned long	_size;
	char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullFilePath.c_str() , "r", &_size);
	TiXmlDocument	*_document = new TiXmlDocument();
	_document->Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

	CC_SAFE_DELETE_ARRAY(_pFileContent);

	TiXmlElement *RootElement = _document->RootElement();
	return RootElement;
}

bool SkeletonAnimRcsManager::readRcsDataFromFile(TiXmlElement *root)
{
	if (0 != root && 0 != roleArmatureRcsData)
	{
		TiXmlElement *childElement = root->FirstChildElement();
		while (0 != childElement)
		{						
			int roleId = atoi(childElement->Attribute("id"));
			
			ArmatureRcsData *armatureRcsData = new ArmatureRcsData();
			if (armatureRcsData)
			{
				TiXmlElement * grandChildElement = childElement->FirstChildElement();
				while(0 != grandChildElement)
				{
					const char *name = grandChildElement->Value();
					if (0 == strcmp(name,"AttachEffects"))
					{
						TiXmlElement * subElement = grandChildElement->FirstChildElement();
						while(0 != subElement)
						{
							int id = atoi(subElement->Attribute("id"));
							armatureRcsData->AddOneEffectID(id);
							subElement = subElement->NextSiblingElement();
						}
					}
					else if (0 == strcmp(name,"MainLand"))
					{
						TiXmlElement * subElement = grandChildElement->FirstChildElement();
						while(0 != subElement)
						{
							const char *xmlName = subElement->Attribute("xml_name");
							armatureRcsData->AddOneElement(std::string(xmlName),false);
							subElement = subElement->NextSiblingElement();
						}
					}
					else if (0 == strcmp(name,"OtherLand"))
					{
						TiXmlElement * subElement = grandChildElement->FirstChildElement();
						while(0 != subElement)
						{
							const char *xmlName = subElement->Attribute("xml_name");
							armatureRcsData->AddOneElement(std::string(xmlName),true);
							subElement = subElement->NextSiblingElement();
						}
					}
					else
					{
						const char *xmlName = grandChildElement->Attribute("xml_name");
						armatureRcsData->AddOneElement(std::string(xmlName),true);
					}
					grandChildElement = grandChildElement->NextSiblingElement();
				}
			}
			roleArmatureRcsData->AddOneElement(roleId,armatureRcsData);
				
			childElement = childElement->NextSiblingElement();
		}
	}
	return true;
}

RoleActionType SkeletonAnimRcsManager::getActionType(const char* actionTypeStr)
{
	RoleActionType actionType = kTypeAction_UnKnow;

	std::string actionTypeString(actionTypeStr);
	int startPos = actionTypeString.rfind("_");
	if (std::string::npos != startPos)
	{
		std::string identityFlagString = actionTypeString.substr(startPos+1,actionTypeString.length()-startPos-1);
		actionType = (RoleActionType)atoi(identityFlagString.c_str());
		return actionType;
	}
	return actionType;
}

bool SkeletonAnimRcsManager::readArmatureDataFromFile(TiXmlElement *root)
{
	if (0 != root)
	{
		TiXmlElement *childElement = root->FirstChildElement();
		while (0 != childElement)
		{
			const char *roleTypeValue = childElement->Attribute("value");
			if(0 != roleTypeValue)
			{
				ArmatureActionData * armatureActionData = new ArmatureActionData();
				if (0 != armatureActionData)
				{
					TiXmlElement *actionElement = childElement->FirstChildElement();
					while(0 != actionElement)
					{								
						TiXmlElement *actionContentElement = actionElement->FirstChildElement();
						const char * xml_file_name = actionContentElement->Attribute("xml_name");
						const char * action_name = actionContentElement->Attribute("action_name");


						ArmatureData armatureData ;
						armatureData.xmlFileName = xml_file_name;
						armatureData.actionName = action_name;

						const char *actionTypeValue = actionElement->Attribute("value");
						RoleActionType roleActionType ;
						roleActionType = getActionType(actionTypeValue);
						armatureActionData->kvRoleActionTypeAndValues.insert(std::make_pair(roleActionType, armatureData));

						actionElement = actionElement->NextSiblingElement();
					}
				}

				int type = 0;
				{
					std::string typeStr(roleTypeValue);
					int startPos = typeStr.find("_");
					typeStr = typeStr.substr(startPos+1,typeStr.length()-startPos);
					type = atoi(typeStr.c_str());
				}

				if (roleArmatureActionData)
				{
					roleArmatureActionData->kvRoleTypeAndValues.insert(std::make_pair(type,armatureActionData));
				}
			}
			childElement = childElement->NextSiblingElement();
		}
	}

	return true;
}

bool SkeletonAnimRcsManager::readMapAttachedRcsDataFromFile(TiXmlElement *root)
{
	if (0 != root && 0 != mapAttachData)
	{
		TiXmlElement *childElement = root->FirstChildElement();		
		while (0 != childElement)
		{
			const char *elementName = childElement->Value();

			if(0 == strcmp(elementName,"common"))
			{
				TiXmlElement *one_element = childElement->FirstChildElement();
				while(0 != one_element)
				{
					int id = atoi(one_element->Attribute("id"));
					mapAttachData->mCommonIdVec.push_back(id);
					one_element = one_element->NextSiblingElement();
				}
			}
			else if (0 == strcmp(elementName,"effect_mainLand_common"))
			{
				TiXmlElement *one_element = childElement->FirstChildElement();
				while(0 != one_element)
				{
					int id = atoi(one_element->Attribute("id"));
					mapAttachData->mMainLandEffectCommonIdVec.push_back(id);
					one_element = one_element->NextSiblingElement();
				}
			}
			else if (0 == strcmp(elementName,"effect_otherLand_common"))
			{
				TiXmlElement *one_element = childElement->FirstChildElement();
				while(0 != one_element)
				{
					int id = atoi(one_element->Attribute("id"));
					mapAttachData->mOthLandEffectCommonIdVec.push_back(id);
					one_element = one_element->NextSiblingElement();
				}
			}
			else if (0 == strcmp(elementName,"map"))
			{
				int mapId = atoi(childElement->Attribute("id"));
				std::vector<int>* pVecIds = new std::vector<int>();
				TiXmlElement *one_element = childElement->FirstChildElement();
				while(0 != one_element)
				{
					int id = atoi(one_element->Attribute("id"));
					pVecIds->push_back(id);
					one_element = one_element->NextSiblingElement();
				}
				mapAttachData->mMapIdAndAttachIdMap.insert(std::make_pair(mapId,pVecIds));
			}

			childElement = childElement->NextSiblingElement();
		}
		return true;
	}
	return false;
}

/// <summary>
//	从XML表中加载数据，得到所有骨骼的动画数据，并存储
/// </summary>
void SkeletonAnimRcsManager::LoadAllData()
{
	if (m_isLoadAllData)
	{
		return ;
	}
	m_isLoadAllData = true;

	//if (roleArmatureRcsData)
	//{
	//	for(std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.begin();
	//		roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end();
	//		roleRcsMapIter ++)
	//	{
	//		std::vector<std::string> * pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
	//		for (size_t index = 0;index<pVecFileNames->size();index++)
	//		{
	//			std::string fileName = (*pVecFileNames)[index];
	//			// 骨骼动画XML资源Load
	//			std::string rcsPath = 
	//				GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
	//				stringAppendMethod(fileName,".ani").c_str());
	//			CCConnectionData::sharedConnectionData()->addData((rcsPath).c_str());
	//		}		

	//		pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
	//		for (size_t index = 0;index<pVecFileNames->size();index++)
	//		{
	//			std::string fileName = (*pVecFileNames)[index];
	//			// 骨骼动画XML资源Load
	//			std::string rcsPath = 
	//				GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
	//				stringAppendMethod(fileName,".ani").c_str());
	//			CCConnectionData::sharedConnectionData()->addData((rcsPath).c_str());
	//		}
	//	}
	//}


	// Add 区域大小加载代码
	std::string rcsPath = 
		GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
		stringAppendMethod("AreaConfigTable",".xml").c_str());
	CCConnectionData::sharedConnectionData()->addColliderAndPointData((rcsPath).c_str());
}

void SkeletonAnimRcsManager::ClearAllRoleConnectionData()
{
	CCConnectionData::sharedConnectionData()->ClearDataExceptColliderAndPointData();
	setLoadRolesConnectionData.clear();
	setLoadRcsFileNames.clear();
	m_loadedHeroAnims.clear();
}

/// <summary>
//	
/// </summary>
void SkeletonAnimRcsManager::LoadOneRoleAttachEffectRcs(int roleId)
{
	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(roleId);
	if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		std::vector<std::string> * pVecFileNames = 0;
		std::vector<int> * pVecAttachEffects = 0;
		pVecAttachEffects = (((*roleRcsMapIter).second)->mVecAttachEffects);

		if (pVecAttachEffects)
		{
			// 加载Effects
			for (size_t index = 0;index<pVecAttachEffects->size();index++)
			{
				int effectId = (*pVecAttachEffects)[index];

				std::map<int,ArmatureRcsData *>::iterator effectRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(effectId);
				if (effectRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
				{
					pVecFileNames = (((*effectRcsMapIter).second)->mVecOtherLandFileNames);

					for (size_t index = 0;index<pVecFileNames->size();index++)
					{
						std::string fileName = ((*pVecFileNames)[index]);
						LoadOneRcs(fileName);
					}
				}
			}
		}
	}
}

/// <summary>
//	
/// </summary>
void SkeletonAnimRcsManager::LoadOneLevelNPCRcs(int levelId)
{
	std::map<unsigned int,NpcBasicInfo>* pNpcBasicInfo = MainLandManager::Get()->GetOneCityNpcInfos(levelId);
	for (std::map<unsigned int,NpcBasicInfo>::iterator iter = pNpcBasicInfo->begin();
		 iter != pNpcBasicInfo->end();iter++)
	{
		int npc_id = (*iter).first;
		
		std::map<int,ArmatureRcsData *>::iterator rcs_iter = (roleArmatureRcsData->kvRoleIdAndRcsNameMap).find(npc_id);
		if (rcs_iter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
		{
			ArmatureRcsData* pRcsData = (*rcs_iter).second;
			std::string rcs_name = (*(pRcsData->mVecOtherLandFileNames))[0];
			LoadOneRcs(rcs_name);
		}
	}
}

/// <summary>
//	加载某主角相关的资源
//	ADD
/// </summary>
void SkeletonAnimRcsManager::LoadOneHeroRcs(int roleId,bool bInOtherLand)
{
	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(roleId);
	if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		std::vector<std::string> * pVecFileNames = 0;
		if (bInOtherLand)
		{
			pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
			LoadOneRoleAttachEffectRcs(roleId);
		}
		else
		{
			pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
		}
		for (size_t index = 0;index<pVecFileNames->size();index++)
		{
			std::string fileName = ((*pVecFileNames)[index]);
			LoadOneRcs(fileName);
		}
	}
}

/// <summary>
//	加载公用的动画资源 例如UI特效 等..
/// </summary>
void SkeletonAnimRcsManager::LoadCommonRcs(bool bInOtherLand)
{
	if (mapAttachData)
	{
		for (std::vector<int>::iterator iter = mapAttachData->mCommonIdVec.begin();
			iter != mapAttachData->mCommonIdVec.end(); iter++)
		{
			int id = (*iter);
			std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(id);
			if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
			{
				std::vector<std::string> * pVecFileNames = 0;
				if (bInOtherLand)
				{
					pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
				}
				else
				{
					pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
				}
				for (size_t index = 0;index<pVecFileNames->size();index++)
				{
					std::string fileName = ((*pVecFileNames)[index]);
					LoadOneRcs(fileName);
				}
			}
		}
	}
}

void SkeletonAnimRcsManager::LoadOneRcs(std::string rcsPath)
{
	//std::set<std::string>::iterator iter = setLoadRcsFileNames.find(rcsPath);
	//if (iter == setLoadRcsFileNames.end())
	{
		//setLoadRcsFileNames.insert(rcsPath);

		// 骨骼动画XML资源Load
		std::string pXmlFilePath = 
			GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
			stringAppendMethod(rcsPath,".ani").c_str());
		CCConnectionData::sharedConnectionData()->addData((pXmlFilePath).c_str());

		std::string pListFilePath =
			GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
			stringAppendMethod(rcsPath,".plist").c_str());
		std::string pngFilePath =
			GameResourceManager::sharedManager()->storedFullPathFromRelativePath(SKELETON_ANIMATION_DIR,
			stringAppendMethod(rcsPath,SKELETON_IMAGE_FILE_SUFFIX).c_str());

		CCDictionary*plistDic = GameResourceManager::sharedManager()->dictionaryFromCache(pListFilePath.c_str());
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(pngFilePath.c_str());

        std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();

        // client version
        if (strcmp(clientVersion.c_str(), "base") == 0)
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture);
        } else if (strcmp(clientVersion.c_str(), "hq") == 0)
        {
            // 根据资源来匹配
            if (rcsPath.find("swordswoman") == 0 ||
                rcsPath.find("swordsman") == 0 ||
                rcsPath.find("mage") == 0 ||
                rcsPath.find("mageman") == 0 ||
                rcsPath.find("assassinwoman") == 0 ||
                rcsPath.find("assassin") == 0 ||
                rcsPath.find("skill") == 0)
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, HQ_SKELETON_CHARACTER);
            } else if (rcsPath.find("npc") == 0)
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, HQ_SKELETON_NPC);
            } else if (rcsPath.find("monster") == 0)
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, HQ_SKELETON_MONSTER);
            } else if (rcsPath.find("fairy") == 0)
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, HQ_SKELETON_FAIRY);
            } else if (rcsPath.find("effect") == 0)
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, HQ_SKELETON_EFFECT);
            } else
            {
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture);
            }
        } else
        if (strcmp(clientVersion.c_str(), "nq") == 0)
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture, NQ_SKELETON);
        } else
        {
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(plistDic, texture);
        }
	}
}

void SkeletonAnimRcsManager::LoadOneRoleRcsOnly(int roleId)
{
	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(roleId);
	if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		std::vector<std::string> * pVecFileNames = 0;
		pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
		for (size_t index = 0;index<pVecFileNames->size();index++)
		{
			std::string fileName = ((*pVecFileNames)[index]);
			LoadOneRcs(fileName);
		}

		pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
		for (size_t index = 0;index<pVecFileNames->size();index++)
		{
			std::string fileName = ((*pVecFileNames)[index]);
			LoadOneRcs(fileName);
		}
	}
}

/// <summary>
//	加载某一个地图依赖的所有资源
/// </summary>
void SkeletonAnimRcsManager::LoadOneMapRcs(int levelID,int mapId,bool bInOtherLand)
{
	// 加载通用特效资源
	if (mapAttachData)
	{
		std::vector<int> vecEffectRcsId;
		if (bInOtherLand)
		{
			vecEffectRcsId = mapAttachData->mOthLandEffectCommonIdVec;
		}
		else
		{
			vecEffectRcsId = mapAttachData->mMainLandEffectCommonIdVec;
		}
		for (std::vector<int>::iterator iter = vecEffectRcsId.begin();
			iter != vecEffectRcsId.end(); iter++)
		{
			int id = (*iter);
			std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(id);
			if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
			{
				std::vector<std::string> * pVecFileNames = 0;
				//if (bInOtherLand)
				{
					pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
				}
				//else
				//{
				//	pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
				//}
				for (size_t index = 0;index<pVecFileNames->size();index++)
				{
					std::string fileName = ((*pVecFileNames)[index]);
					LoadOneRcs(fileName);
				}
			}
			if (id == 101)
			{
				ParticleManager::Get()->AddParticleConditonData();
			}
		}
	}

	// 加载地图相关资源
	// Note: 从Lua配表中得到怪物的配置信息
	std::vector<unsigned int> monsterIdVec;
	GameConfigFromLuaManager::Get()->getOneMapMonsterIds(levelID,mapId,monsterIdVec);

	size_t vecSize = monsterIdVec.size();
	for (size_t index = 0;index<vecSize;index++)
	{
		unsigned int id = monsterIdVec[index];
		// 加载某角色相关的资源
		std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(id);
		if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
		{
			std::vector<std::string> * pVecFileNames = 0;
			pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);

			for (size_t index = 0;index<pVecFileNames->size();index++)
			{
				std::string fileName = ((*pVecFileNames)[index]);
				LoadOneRcs(fileName);
			}
		}

		if (bInOtherLand)
		{
			// 加载Effects
			LoadOneRoleAttachEffectRcs(id);
		}
	}	
}

bool SkeletonAnimRcsManager::IsOneActorRcsExist(int typeId)
{
	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(typeId);
	if (roleRcsMapIter == roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		return false;
	}
	return true;
}

/// <summary>
//	为一个角色创建所依赖的所有骨架数据
/// </summary>
std::map<std::string,RoleActorBase*>* SkeletonAnimRcsManager::createOneRoleArmatures(int role_id)
{
	if (role_id>=1 && role_id <= 6)
	{
		//std::set<unsigned int >::iterator iter = m_loadedHeroAnims.find(role_id);
		//if (iter == m_loadedHeroAnims.end())
		{
			//bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
			//this->LoadOneHeroRcs(role_id,!bMainLandLevel);
			//m_loadedHeroAnims.insert(role_id);
		}
	}

	std::map<std::string,RoleActorBase*>* roleActorDataMap = new std::map<std::string,RoleActorBase*>();
	if (0 == roleActorDataMap)
	{
		return 0;
	}

	std::vector<std::string> * pVecFileNames = 0;
	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(role_id);
	if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		bool bMainLandLevel = MainLandManager::Get()->isMainLand(LevelManager::sShareInstance()->getCurMapId());
		if (bMainLandLevel)
		{
			pVecFileNames = (*roleRcsMapIter).second->mVecMainLandFileNames;
			if (pVecFileNames->size() == 0)
			{
				pVecFileNames = (*roleRcsMapIter).second->mVecOtherLandFileNames;
			}
		}
		else
		{
			pVecFileNames = (*roleRcsMapIter).second->mVecOtherLandFileNames;
		}

		for (std::vector<std::string>::iterator iter = pVecFileNames->begin();
			 iter != pVecFileNames->end(); iter++)
		{
			std::string name = (*iter);
			std::string pngFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath
				(SKELETON_ANIMATION_DIR,stringAppendMethod(name,SKELETON_IMAGE_FILE_SUFFIX).c_str());
			RoleActorBase * actorBase = RoleActorBase::create(name,name,pngFilePath);

            // client version
            float f = 1.0f;
            std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();
            if (strcmp(clientVersion.c_str(), "hq") == 0)
            {
                // 根据资源来匹配
                if (name.find("swordswoman") == 0 ||
                    name.find("swordsman") == 0 ||
                    name.find("mage") == 0 ||
                    name.find("mageman") == 0 ||
                    name.find("assassinwoman") == 0 ||
                    name.find("assassin") == 0 ||
                    name.find("skill") == 0)
                {
                    f = HQ_SKELETON_CHARACTER;
                } else if (name.find("npc") == 0)
                {
                    f = HQ_SKELETON_NPC;
                } else if (name.find("monster") == 0)
                {
                    f = HQ_SKELETON_MONSTER;
                } else if (name.find("fairy") == 0)
                {
                    f = HQ_SKELETON_FAIRY;
                } else if (name.find("effect") == 0)
                {
                    f = HQ_SKELETON_EFFECT;
                }
            }  else if (strcmp(clientVersion.c_str(), "nq") == 0)
            {
                f = NQ_SKELETON;
            }
                
            actorBase->getDisplay()->setScale(1.0f / f);


			if (0 != actorBase)
			{
				actorBase->pauseAllAnimation();
				roleActorDataMap->insert(std::make_pair(name,actorBase));
			}
		}
	}

	std::map<int,ArmatureActionData *>::iterator iter = roleArmatureActionData->kvRoleTypeAndValues.find(role_id);
	if (iter == roleArmatureActionData->kvRoleTypeAndValues.end())
	{
		// 特效没有单独进行配置 使用默认添加的方式创建一个名为“play”的动作
		std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(role_id);
		if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
		{
			RoleActionType actionType = kType_Play;
			ArmatureActionData *_armatureActionData = new ArmatureActionData();
			std::vector<std::string> * pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
			for (size_t index = 0;index<pVecFileNames->size();index++)
			{				
				ArmatureData armatureData;
				armatureData.actionName = "play";
				armatureData.xmlFileName = (*pVecFileNames)[index];
				_armatureActionData->kvRoleActionTypeAndValues.insert(std::make_pair(actionType,armatureData));
			}
			roleArmatureActionData->kvRoleTypeAndValues.insert(std::make_pair(role_id,_armatureActionData));
		}
	}

	return roleActorDataMap;
}

/// <summary>
//	获取某角色相关的动画数据：动作类型 + 骨骼名 + 动作名
//	特效只有一个动作返回 0 动作名称为 “Play”
/// </summary>
ArmatureActionData * SkeletonAnimRcsManager::getOneRoleArmatureActionData(int id)
{
	std::map<int,ArmatureActionData *>::iterator iter =
		roleArmatureActionData->kvRoleTypeAndValues.find(id);

	if (iter != roleArmatureActionData->kvRoleTypeAndValues.end())
	{
		return (*iter).second;
	}

	return 0;
}

int SkeletonAnimRcsManager::getOneAnimationDurationTween(int roleId,int animId)
{
	std::map<int,ArmatureActionData *>::iterator iter = roleArmatureActionData->kvRoleTypeAndValues.find(roleId);
	if (iter != roleArmatureActionData->kvRoleTypeAndValues.end())
	{
		ArmatureActionData *pData = (*iter).second;
		std::map<RoleActionType,ArmatureData >::iterator _iter = pData->kvRoleActionTypeAndValues.find((RoleActionType)animId);
		if (_iter != pData->kvRoleActionTypeAndValues.end())
		{
			ArmatureData armature_data = (*_iter).second;
			std::string name = armature_data.actionName;
			std::string animation_name = armature_data.xmlFileName;

			//LoadOneRcs(animation_name);

			CCArmatureAniData* _animationData = CCConnectionData::sharedConnectionData()->getAnimationData(animation_name.c_str());
			if (_animationData != 0)
			{
				CCBoneAniData *pAniData = _animationData->getAnimation( name.c_str() );
				if (pAniData == 0)
				{
					return 0;
				}
				return pAniData->getDurationTween();
			}
		}		
	}

	return 0;
}

float SkeletonAnimRcsManager::getOneAnimationDuration(int roleId,int animId)
{
	int frame = getOneAnimationDurationTween(roleId,animId);
	return ((float)frame) / (FRAME_RATES);
}

void SkeletonAnimRcsManager::LoadOneSpriteElfRcs(unsigned int elfId,bool bInOtherLand)
{
	bool bElfLoad = false;

	for (std::vector<unsigned int>::iterator iter = vecLoadedElfIds.begin();
		 iter != vecLoadedElfIds.end(); iter++)
	{
		unsigned int id = (*iter);
		if (id == elfId)
		{
			bElfLoad = true;
			return ;
		}
	}

	vecLoadedElfIds.push_back(elfId);

	std::map<int,ArmatureRcsData *>::iterator roleRcsMapIter = roleArmatureRcsData->kvRoleIdAndRcsNameMap.find(elfId);
	if (roleRcsMapIter != roleArmatureRcsData->kvRoleIdAndRcsNameMap.end())
	{
		std::vector<std::string> * pVecFileNames = 0;
		if (bInOtherLand)
		{
			pVecFileNames = (((*roleRcsMapIter).second)->mVecOtherLandFileNames);
			LoadOneRoleAttachEffectRcs(elfId);
		}
		else
		{
			pVecFileNames = (((*roleRcsMapIter).second)->mVecMainLandFileNames);
		}
		for (size_t index = 0;index<pVecFileNames->size();index++)
		{
			std::string fileName = ((*pVecFileNames)[index]);
			LoadOneRcs(fileName);
		}
	}
}

void SkeletonAnimRcsManager::addOneRoleTypeIdToMap(std::vector<unsigned int> vecIn)
{
	vecLoadNeedRoleTypes.clear();
	vecLoadNeedRoleTypes = vecIn;
}

void SkeletonAnimRcsManager::LoadRoleRcsOneMap(bool bMainLand)
{
	size_t count = vecLoadNeedRoleTypes.size();
	for (size_t index = 0;index<count;index++)
	{
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(vecLoadNeedRoleTypes[index],!bMainLand);
		m_loadedHeroAnims.insert(vecLoadNeedRoleTypes[index]);
		if (bMainLand == false)
		{
			GameAudioManager::sharedManager()->LoadLevelHeroCommonAudio(vecLoadNeedRoleTypes[index]);
		}
	}
	vecLoadNeedRoleTypes.clear();	
}