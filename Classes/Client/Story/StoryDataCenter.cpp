#include "OnlineNetworkManager.h"
#include "StoryDataCenter.h"
#include "GameResourceManager.h"
#include "UserData.h"
#include "GameManager.h"
#include "SpriteSeer.h"
#include "GameConfigFromLuaManager.h"
#include "LuaTinkerManager.h"
#include "Localization.h"

static StoryDataCenter* _storyDataManager = 0;

StoryDataCenter::StoryDataCenter()
{
	mIsLoadAllData = false;
}

StoryDataCenter::~StoryDataCenter()
{
	mVecTaskIdAndStoryData.clear();
}

StoryDataCenter* StoryDataCenter::Get()
{
	if(_storyDataManager == 0)
	{
		_storyDataManager = new StoryDataCenter();
	}
	return _storyDataManager;
}

void StoryDataCenter::Destroy()
{
	if (_storyDataManager)
	{
		delete _storyDataManager;
		_storyDataManager = 0;
	}
}

bool StoryDataCenter::LoadOneDataItem(unsigned int id,unsigned int map_id,unsigned int When,int &_ID)
{
	for (std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
		iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int Id = (*iter).getTaskId();
		unsigned int nWhen = (*iter).getHappendAtWhen();
		unsigned int mapId = (*iter).getMapId();
		if (Id == id && nWhen == When && mapId == map_id)
		{
			mCurIterator = iter;
			_ID = (*iter).getID();
			return true;
		}
	}	
	return false;
}

bool StoryDataCenter::LoadOneDataItem(unsigned int id,unsigned int when,int &_ID)
{
	for (std::vector<StroyData>::iterator iter = mMainLandVecTaskStoryData.begin();
		iter != mMainLandVecTaskStoryData.end(); iter++)
	{
		unsigned int Id = (*iter).getTaskId();
		unsigned int nWhen = (*iter).getHappendAtWhen();
		if (Id == id && nWhen == when)
		{
			mCurIterator = iter;
			_ID = (*iter).getID();
			return true;
		}
	}	
	return false;
}

TiXmlElement * StoryDataCenter::initXML(const char * filePath)
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

bool StoryDataCenter::SplitPosFromContent(std::string content,float &posx,float &posy)
{
	if (content == "")
	{
		return false;
	}
	int startPos = content.find("(");
	int midPos = content.find(",");
	int endPos = content.find(")");

	if (std::string::npos == startPos || 
		std::string::npos == midPos || 
		std::string::npos == endPos
		)
	{
		CCError("StoryDataCenter::SplitPosFromContent() %s format error ",content.c_str());
		return false;
	}

	std::string posx_content = content.substr(startPos+1,midPos-startPos-1);
	posx = atof(posx_content.c_str());
	posx = posx*16;

	std::string posy_content = content.substr(midPos+1,endPos-midPos-1);
	posy = atof(posy_content.c_str());
	posy = posy*16;

	return true;
}

bool StoryDataCenter::SplitFrameAndPosFromContent(std::string content,unsigned int &frame,float &posx,float &posy)
{
	if (content == "")
	{
		return false;
	}

	int startPos = content.find("/");
	if (std::string::npos == startPos)
	{
		return false;
	}
	std::string frameContent = content.substr(0,startPos);
	frame = atoi(frameContent.c_str());

	std::string posContent = content.substr(startPos+1,content.length() - startPos - 1);
	if (false == SplitPosFromContent(posContent,posx,posy))
	{
		return false;
	}

	return true;
}

bool StoryDataCenter::SplitRoleID(std::string content,unsigned int &roleId)
{
	if ("" == content)
	{
		return false;
	}
	int endPos = content.rfind("_");
	if (std::string::npos == endPos)
	{
		return false;
	}
	roleId = atoi(content.substr(endPos+1,content.length()-endPos-1).c_str());

	return true;
}

bool StoryDataCenter::SplitFrameAndActorIdFromContent(std::string content,unsigned int &frame,int &actorId,bool &bPauseOnOver)
{
	if ("" == content)
	{
		return false;
	}

	int startPos = content.find("/");\
	std::string frameContent = content.substr(0,startPos);
	std::string actorContent = "";
	if (std::string::npos == content.find("/",startPos+1))
	{
		actorContent = content.substr(startPos+1,content.length()-startPos-1);
		bPauseOnOver = false;
	}
	else
	{
		int secondStartPos = content.find("/",startPos+1);
		actorContent = content.substr(startPos+1,secondStartPos-startPos-1);

		int pauseFlag = atoi(content.substr(secondStartPos+1,content.length()-secondStartPos-1).c_str());
		if (pauseFlag == 0)
		{
			bPauseOnOver = false;
		}
		else
		{
			bPauseOnOver = true;
		}
	}

	frame = atoi(frameContent.c_str());
	actorId = atoi(actorContent.c_str());

	return true;
}

bool StoryDataCenter::SplitFrameAndTextIdFromContent(std::string content,std::vector<unsigned int> &vecFrameIndex,std::vector<std::string> &vecTextId)
{
	if ("" == content)
	{
		return false;
	}

	vecFrameIndex.clear();
	vecTextId.clear();

	std::vector<std::string> vecContent;

	int startPos = 0;
	int endPos = content.find(";");
	while(std::string::npos != endPos)
	{
		std::string sub_content = content.substr(startPos,endPos-startPos);
		vecContent.push_back(sub_content);

		startPos = endPos + 1;
		endPos = content.find(";",startPos);
	}
	endPos = content.rfind(";");
	std::string endContent = content.substr(endPos+1,content.length() - endPos - 1);
	vecContent.push_back(endContent);

	size_t count = vecContent.size();
	for (size_t index = 0;index<count;index++)
	{
		std::string oneContent = vecContent[index];

		int midPos = oneContent.find("/");
		if (std::string::npos != midPos)
		{
			unsigned int frameIndex = atoi(oneContent.substr(0,midPos).c_str());
			std::string textId = oneContent.substr(midPos+1,oneContent.length()-midPos-1);

			vecFrameIndex.push_back(frameIndex);
			vecTextId.push_back(textId);
		}
	}

	return true;
}

bool StoryDataCenter::SplitContent(std::string content,std::vector<std::string> &vecOutContent)
{
	if (content == "")
	{
		return false;
	}

	int startPos = 0;
	int endPos = content.find(";");
	if (endPos == std::string::npos)
	{
		vecOutContent.push_back(content);
		return true;
	}

	while(std::string::npos != endPos)
	{
		std::string sub_content = content.substr(startPos,endPos - startPos);
		vecOutContent.push_back(sub_content);

		startPos = endPos + 1;
		endPos = content.find(";",startPos);
	}
	endPos = content.rfind(";");
	std::string endContent = content.substr(endPos+1,content.length() - endPos - 1);
	vecOutContent.push_back(endContent);

	return true;
}

bool StoryDataCenter::ReadOneXmlItem(TiXmlElement* pElement)
{
	if (pElement == 0)
	{
		return false;
	}

	unsigned int taskID = 0;
	StroyData storyData;

	bool bInstanceStory = true;

	// Note: Parse TaskID
	TiXmlAttribute* pAttribute = pElement->FirstAttribute();
	while(pAttribute)
	{
		std::string strName(pAttribute->Name());
		std::string content = pElement->Attribute(strName.c_str());
		if (strName == "ID2")
		{
			unsigned int id = atoi(content.c_str());
			storyData.setID(id);
		}
		else if (strName == "task_ID")
		{
			bInstanceStory = false;
			unsigned int task_id = atoi(content.c_str());
			taskID = task_id;
			storyData.setTaskId(task_id);
		}
		else if (strName == "instance_ID")
		{
			bInstanceStory = true;
			unsigned int task_id = atoi(content.c_str());
			taskID = task_id;
			storyData.setTaskId(task_id);
		}
		else if (strName == "map_ID")
		{
			unsigned int map_id = atoi(content.c_str());
			storyData.setMapId(map_id);
		}
		else if (strName == "camera")
		{
			float posx = 0;
			float posy = 0;
			if (SplitPosFromContent(content,posx,posy))
			{
				storyData.setCameraPos(ccp(posx,posy));
			}
		}
		else if (strName == "end_frame")
		{
			int endFrame = atoi(content.c_str());
			storyData.setEndFrame(endFrame);
		}
		else if (strName == "when")
		{
			int nWhen = atoi(content.c_str());
			storyData.setHappendAtWhen(nWhen);
		}
		else if (std::string::npos != strName.find("set_role_position_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				std::vector<std::string> vecContent;
				SplitContent(content,vecContent);
				size_t vecSize = vecContent.size();
				for (size_t index = 0;index<vecSize;index++)
				{
					unsigned int frameIndex = 0;
					float posx = 0;
					float posy = 0;
					if(SplitFrameAndPosFromContent(vecContent[index],frameIndex,posx,posy))
					{
						storyData.InsertRolePosAtFrame(roleIndex,frameIndex,ccp(posx,posy));
					}
				}
			}	
		}
		else if (std::string::npos != strName.find("set_role_act_"))
		{
			unsigned int roleIndex = 0;			
			if (SplitRoleID(strName,roleIndex))
			{
				std::vector<std::string> vecContent;
				SplitContent(content,vecContent);
				size_t vecSize = vecContent.size();
				for (size_t index = 0;index<vecSize;index++)
				{
					unsigned int frameIndex = 0;
					int actorId = 0;
					bool bActionPauseOnOver = false;
					if (SplitFrameAndActorIdFromContent(vecContent[index],frameIndex,actorId,bActionPauseOnOver))
					{
						storyData.InsertRoleActorAtFrame(roleIndex,frameIndex,actorId,bActionPauseOnOver);
					}	
				}			
			}
		}
		else if (std::string::npos != strName.find("role_dialog_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				std::vector<unsigned int> vecFrameIndex;
				std::vector<std::string> vecTextId;
				if (SplitFrameAndTextIdFromContent(content,vecFrameIndex,vecTextId))
				{
					size_t count = vecFrameIndex.size();
					for (size_t index = 0;index<count;index++)
					{
						storyData.InsertDialogTextIdAtFrame(roleIndex,vecFrameIndex[index],vecTextId[index]);
					}
				}
			}
		}
		else if (std::string::npos != strName.find("set_role_flip_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				std::vector<std::string> vecContent;
				SplitContent(content,vecContent);
				size_t vecSize = vecContent.size();
				for (size_t index = 0;index<vecSize;index++)
				{
					unsigned int nFrame = 0;
					int nFlip = 0;
					bool bTmp = false;
					if (SplitFrameAndActorIdFromContent(vecContent[index],nFrame,nFlip,bTmp))
					{
						storyData.InsertIsFlipAtFrame(roleIndex,nFrame,nFlip);
					}
				}
			}
		}
		else if (std::string::npos != strName.find("set_role_icon_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				storyData.SetRoleLeftIcon(roleIndex,content);
			}
		}
		else if (std::string::npos != strName.find("set_display_picture"))
		{
			std::vector<unsigned int> vecFrameIndex;
			std::vector<std::string> vecPicName;
			if (SplitFrameAndTextIdFromContent(content,vecFrameIndex,vecPicName))
			{
				size_t count = vecFrameIndex.size();
				for (size_t index = 0;index<count;index++)
				{
					storyData.SetDisplayPicAtIndex(vecFrameIndex[index],vecPicName[index]);
				}
			}
		}
		else if (std::string::npos != strName.find("set_role_visible_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				std::vector<std::string> vecContent;
				SplitContent(content,vecContent);
				size_t vecSize = vecContent.size();
				for (size_t index = 0;index<vecSize;index++)
				{
					unsigned int nFrame = 0;
					int nVisible = 0;
					bool bTmp = false;
					if (SplitFrameAndActorIdFromContent(vecContent[index],nFrame,nVisible,bTmp))
					{
						storyData.InsertIsVisibleAtFrame(roleIndex,nFrame,nVisible);
					}
				}
			}
		}
		else if (std::string::npos != strName.find("role_"))
		{
			unsigned int roleIndex = 0;
			if (SplitRoleID(strName,roleIndex))
			{
				unsigned int roleId = 0;
				roleId = atoi(content.c_str());
				storyData.SetRoleIdAtIndex(roleIndex,roleId);
			}
		}
		else
		{
			CCError("StoryDataCenter::ReadOneXmlItem（） %s attribute undefined ",strName.c_str());
		}

		pAttribute = pAttribute->Next();
	}	

	if (taskID != 0)
	{
		if (bInstanceStory)
		{
			mVecTaskIdAndStoryData.push_back(storyData);
		}
		else
		{
			mMainLandVecTaskStoryData.push_back(storyData);
		}
	}

	return true;
}

bool StoryDataCenter::LoadDataFromXML()
{
	if (mIsLoadAllData)
	{
		return true;
	}
	mIsLoadAllData = true;

	std::string filePath = std::string("Data/") + std::string("cutscene.xml");
	TiXmlElement *RootElement = initXML(filePath.c_str());
	if (0 != RootElement)
	{
		TiXmlElement *childElement = RootElement->FirstChildElement();
		while (0 != childElement)
		{
			ReadOneXmlItem(childElement);

			childElement = childElement->NextSiblingElement();
		}

		TiXmlDocument *doc = RootElement->GetDocument();
		doc->Clear();
		CC_SAFE_DELETE(doc);
	}
	else
	{
		return false;
	}
	return true;
}

bool StoryDataCenter::IsOneItemHasStory(unsigned int Id,unsigned int mapId,unsigned int when,unsigned int &type)
{
	for (std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
		iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		unsigned int map_id = (*iter).getMapId();
		unsigned int nWhen = (*iter).getHappendAtWhen();
		if (id == Id && when == nWhen && map_id == mapId)
		{
			type = (*iter).getHappendAtWhen();
			return true;
		}
	}

	return false;
}

bool StoryDataCenter::IsOneMainLandStoryPlayed(unsigned int mapId)
{
	return false;
}

unsigned int StoryDataCenter::GetTotalFrames(unsigned int Id)
{
	for (std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
		iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == Id)
		{
			return (*iter).getEndFrame();
		}
	}

	return 0;
}

bool StoryDataCenter::IsOneFrameHasDialog(unsigned int taskId,unsigned int frameIndex,StoryDialogData & dialogData)
{
	for (std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
		iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == taskId)
		{
			std::map<unsigned int,StoryDialogData>::iterator frameIter = (*iter).mMapStroyDialogsData.find(frameIndex);
			if (frameIter != (*iter).mMapStroyDialogsData.end())
			{
				if ((*frameIter).second.getIsOver())
				{
					return false;
				}
				((*frameIter).second).setIsOver(true);
				dialogData = (*frameIter).second;
				return true;
			}
		}
	}

	return false;
}

bool StoryDataCenter::GetOneItemOwnRoleIds(unsigned int taskId,std::vector<unsigned int> &vecOut)
{
	bool bHasKey = false;
	std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
	for (iter; iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == taskId)
		{
			bHasKey = true;
			break;
		}
	}
	if (false == bHasKey)
	{
		return false;
	}

	std::map<unsigned int,StoryRoleBasicData>::iterator _iter = (*iter).mMapStoryRolesBasicData.begin();
	while(_iter != (*iter).mMapStoryRolesBasicData.end())
	{
		unsigned int roleId = (*_iter).second.getRoleId();

		// Note: 保证不会重复插入
		bool bRoleHasInserted = false;
		size_t count = vecOut.size();
		for (size_t index = 0;index<count;index++)
		{
			if (vecOut[index] == roleId)
			{
				bRoleHasInserted = true;
				break;
			}
		}
		if (false == bRoleHasInserted)
		{
			vecOut.push_back(roleId);
		}		

		_iter++;
	}

	return true;
}

bool StoryDataCenter::GetOneItemOneFrameRoleInfo(unsigned int taskId,unsigned int frameIndex, 
	std::vector<unsigned int> &vecPlayerId,std::vector<StoryFrameData>& vecStoryData)
{
	bool bHasKey = false;
	std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
	for (iter; iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == taskId)
		{
			bHasKey = true;
			break;
		}
	}
	if (false == bHasKey)
	{
		return false;
	}

	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*iter).mMapStoryRolesBasicData.begin();
	while(secIter != (*iter).mMapStoryRolesBasicData.end())
	{
		unsigned int roleId = (*secIter).second.getRoleId();
		bool bHasFrame = false;

		std::map<unsigned int,StoryFrameData>::iterator frameIter = (*secIter).second.mMapStoryFramesData.find(frameIndex);
		if (frameIter != (*secIter).second.mMapStoryFramesData.end())
		{
			if ((*frameIter).second.getIsOver() == false)
			{
				vecPlayerId.push_back(roleId);
				vecStoryData.push_back((*frameIter).second);
			}
			(*frameIter).second.setIsOver(true);
		}


		secIter++;
	}

	return true;
}

bool StoryDataCenter::IsOneRoleVisible(unsigned int taskId,unsigned int roleId)
{
	bool bHasKey = false;
	std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
	for (iter; iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == taskId)
		{
			bHasKey = true;
			break;
		}
	}
	if (false == bHasKey)
	{
		return false;
	}

	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*iter).mMapStoryRolesBasicData.find(roleId);
	if (secIter != (*iter).mMapStoryRolesBasicData.end())
	{
		return (*secIter).second.getIsVisible();
	}

	return false;
}

bool StoryDataCenter::SetOneRoleVisibleOrNot(unsigned int taskId,unsigned int roleId,bool bVisible)
{
	bool bHasKey = false;
	std::vector<StroyData>::iterator iter = mVecTaskIdAndStoryData.begin();
	for (iter; iter != mVecTaskIdAndStoryData.end(); iter++)
	{
		unsigned int id = (*iter).getTaskId();
		if (id == taskId)
		{
			bHasKey = true;
			break;
		}
	}
	if (false == bHasKey)
	{
		return false;
	}

	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*iter).mMapStoryRolesBasicData.find(roleId);
	if (secIter != (*iter).mMapStoryRolesBasicData.end())
	{
		(*secIter).second.setIsVisible(bVisible);
	}

	return true;
}

bool StoryDataCenter::GetRoleNameByRoleId(unsigned int roleId, std::string& roleName)
{
	char sRoleId[10] = {};
	sprintf(sRoleId,"%d",roleId);
	roleName = "";
	if (0 == roleId)
	{
		roleName = UserData::Get()->getUserName();
	}
	else if (roleId>=501 && roleId<=600)
	{
		roleName = "NPC_";
		roleName += sRoleId;
		roleName += "_name";
		roleName = Localizatioin::getLocalization(roleName.c_str());
	}
	else if (roleId>=2001 && roleId<=3001)
	{
		roleName = "FAIRY_";
		roleName += sRoleId;
		roleName += "_NAME";
		roleName = Localizatioin::getLocalization(roleName.c_str());
	}
	else if (roleId>=10001 && roleId<=20001)
	{
		roleName = "ENEMY_NAME_";
		roleName += sRoleId;
		roleName = Localizatioin::getLocalization(roleName.c_str());
	}
	return true;
}

// Note: 依赖iter

unsigned int StoryDataCenter::GetTotalFrames()
{
	return (*mCurIterator).getEndFrame();
}

bool StoryDataCenter::IsOneFrameHasDialog(unsigned int frameIndex,StoryDialogData & dialogData)
{
	std::map<unsigned int,StoryDialogData>::iterator frameIter = (*mCurIterator).mMapStroyDialogsData.find(frameIndex);
	if (frameIter != (*mCurIterator).mMapStroyDialogsData.end())
	{
		((*frameIter).second).setIsOver(true);
		dialogData = (*frameIter).second;
		return true;
	}

	return false;
}

bool StoryDataCenter::IsOneFrameHasDisplayPic(unsigned int frameIndex,std::string & picName)
{
	std::map<unsigned int,std::string>::iterator frameIter = (*mCurIterator).mMapStoryDisplayPicData.find(frameIndex);
	if (frameIter != (*mCurIterator).mMapStoryDisplayPicData.end())
	{
		picName = (*frameIter).second;
		return true;
	}

	return false;
}

bool StoryDataCenter::GetOneItemOwnRoleIds(std::vector<unsigned int> &vecOut)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator _iter = (*mCurIterator).mMapStoryRolesBasicData.begin();
	while(_iter != (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		unsigned int roleId = (*_iter).second.getRoleId();

		// Note: 保证不会重复插入
		bool bRoleHasInserted = false;
		size_t count = vecOut.size();
		for (size_t index = 0;index<count;index++)
		{
			if (vecOut[index] == roleId)
			{
				bRoleHasInserted = true;
				break;
			}
		}
		if (false == bRoleHasInserted)
		{
			vecOut.push_back(roleId);
		}		

		_iter++;
	}

	return true;
}

bool StoryDataCenter::GetOneItemAllRoleIds(std::vector<unsigned int> &vecOut)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator _iter = (*mCurIterator).mMapStoryRolesBasicData.begin();
	while(_iter != (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		unsigned int roleId = (*_iter).second.getRoleId();
		vecOut.push_back(roleId);

		_iter++;
	}

	return true;
}

bool StoryDataCenter::GetOneItemOneFrameRoleInfo(unsigned int frameIndex,
	std::vector<unsigned int> &vecPlayerId,std::vector<StoryFrameData>& vecStoryData)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*mCurIterator).mMapStoryRolesBasicData.begin();
	while(secIter != (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		unsigned int roleId = (*secIter).second.getRoleId();
		bool bHasFrame = false;

		std::map<unsigned int,StoryFrameData>::iterator frameIter = (*secIter).second.mMapStoryFramesData.find(frameIndex);
		if (frameIter != (*secIter).second.mMapStoryFramesData.end())
		{
			if ((*frameIter).second.getIsOver() == false)
			{
				vecPlayerId.push_back(roleId);
				vecStoryData.push_back((*frameIter).second);
			}
			(*frameIter).second.setIsOver(true);
		}


		secIter++;
	}

	return true;
}

std::map<unsigned int,StoryRoleBasicData>& StoryDataCenter::GetStoryRoleBasicData()
{
	return (*mCurIterator).mMapStoryRolesBasicData;
}

bool StoryDataCenter::IsOneRoleVisible(unsigned int roleId)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*mCurIterator).mMapStoryRolesBasicData.find(roleId);
	if (secIter != (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		return (*secIter).second.getIsVisible();
	}

	return false;
}

bool StoryDataCenter::SetOneRoleVisibleOrNot(unsigned int roleId,bool bVisible)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator secIter = (*mCurIterator).mMapStoryRolesBasicData.find(roleId);
	if (secIter != (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		(*secIter).second.setIsVisible(bVisible);
	}

	return true;
}

bool StoryDataCenter::GetNextEndPointFrameAndPos(unsigned int roleIndex,unsigned int curFrameIndex,unsigned int& nextFrameIndex,CCPoint &nextPoint)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator Iter = (*mCurIterator).mMapStoryRolesBasicData.find(roleIndex);
	if (Iter == (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		return false;
	}

	StoryRoleBasicData basicData = (*Iter).second;
	std::map<unsigned int,StoryFrameData>::iterator secIter = basicData.mMapStoryFramesData.find(curFrameIndex);
	if (secIter == basicData.mMapStoryFramesData.end())
	{
		return false;
	}

	secIter++;
	while(secIter != basicData.mMapStoryFramesData.end())
	{
		CCPoint pos = (*secIter).second.getRolePos();
		if (StoryFrameData::IsPosChanged(pos))
		{
			nextFrameIndex = (*secIter).first;
			nextPoint = pos;

			return true;
		}

		secIter++;
	}

	return false;
}

bool StoryDataCenter::GetPreAndNextPointFrameData(unsigned int roleIndex,unsigned int curFrameIndex,unsigned int &preFrameIndex,
	unsigned int &nextFrameIndex,CCPoint &prePoint,CCPoint &nextPoint)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator Iter = (*mCurIterator).mMapStoryRolesBasicData.find(roleIndex);
	if (Iter == (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		return false;
	}

	StoryRoleBasicData basicData = (*Iter).second;
	preFrameIndex = 0;
	nextFrameIndex = 0;

	std::map<unsigned int,StoryFrameData>::iterator secIter = basicData.mMapStoryFramesData.begin();
	while(secIter != basicData.mMapStoryFramesData.end())
	{
		CCPoint pos = (*secIter).second.getRolePos();
		if (StoryFrameData::IsPosChanged(pos))
		{
			unsigned int tempIndex = (*secIter).first;
			if (curFrameIndex > tempIndex)
			{
				preFrameIndex = tempIndex;
				prePoint = pos;
			}
			else
			{
				nextFrameIndex = tempIndex;
				nextPoint = pos;
				break;
			}
		}
		secIter++;
	}

	if (nextFrameIndex <= curFrameIndex || preFrameIndex >= curFrameIndex)
	{
		return false;
	}
	if (prePoint.x == nextPoint.x && prePoint.y == nextPoint.y)
	{
		return false;
	}

	return true;
}

bool StoryDataCenter::GetOneRoleLeftIcon(unsigned int roleIndex,std::string &iconName)
{
	std::map<unsigned int,StoryRoleBasicData>::iterator Iter = (*mCurIterator).mMapStoryRolesBasicData.find(roleIndex);
	if (Iter == (*mCurIterator).mMapStoryRolesBasicData.end())
	{
		return false;
	}

	StoryRoleBasicData basicData = (*Iter).second;
	iconName = basicData.getRoleLogoIcon();
	if (iconName == "")
	{
		SpriteSeer* pHero = GameManager::Get()->getHero();
		if (pHero)
		{
			unsigned int index = pHero->GetTypeId();
			char buffer[100];
			sprintf(buffer,"ui_mc_cutscene_%d",index);
			iconName = buffer;
		}		
	}

	return true;
}

CCPoint StoryDataCenter::GetCameraPoint()
{
	return (*mCurIterator).getCameraPos();
}

void StoryDataCenter::SendReqCheckPlayedStory()
{
	std::vector<int> vec;
	std::vector<StroyData>::iterator iter = mMainLandVecTaskStoryData.begin();
	for (iter;iter != mMainLandVecTaskStoryData.end(); iter++)
	{
		vec.push_back((*iter).getID() + kType_MainLandStory_Key);
	}

	OnlineNetworkManager::sShareInstance()->sendOnceReqMessage(vec);
}