#pragma once

#include "CArmature.h"
#include "CBone.h"
#include "tinyxml.h"
#include "CConstValue.h"
#include "CTextureData.h"
#include "SkeletonAnimRcsManager.h"

#include <string>
using namespace  std;

class CConnDataReader
{
public:
	CConnDataReader(const char* _xmlConnFile);
	~CConnDataReader();

public:
	/// read float value
	bool  ReadFloat(float &fVal); 
	
	/// read string valule
	bool  ReadString(string & strVal);
	
	/// read int value
	bool  ReadInt32(int & iVal);

protected:
	unsigned long m_fileSize;
	unsigned long m_curPos;
	char* m_pstrContext;
};

class CConnDataWriter
{
public:
	CConnDataWriter(const char* _xmlConnFile);
	~CConnDataWriter();

public:
	/// write float value 
	void WriteFloat(float fVal);
	
	/// write string value , first write the string lenght then the content
	void WriteString(const string& val);

	/// write an int32 value
	void WriteInt32(const int & iVal);

protected:
	string m_strFile;
};



//格式化、管理骨骼配置与骨骼动画
class CCConnectionData : public CCObject {
public:
    static CCConnectionData *sharedConnectionData();
	static void purgeConnectionData();


private:
    CCConnectionData(void);
    ~CCConnectionData(void);

public:
    virtual bool init();
    
    CCArray *getArmatureData(const char* _id);
    
    CCArmatureAniData *getAnimationData(const char* _id);

	CCTexutreData *getTextureData(const char *_textureName);
    /**
    * 将XML数据转换成内置数据 ArmatureAniData 等
    * @param _xml XML数据
    */
    void addData(const char* _xml);

	void addColliderAndPointData(const char* _xml);

    void removeAll(bool _deletMe = false);

	bool getColliderData(int role_id,int action_id,
		CCMoveAreaData &data){
			std::map<int,CCColliderData>::iterator iter;
			iter = kvRoleIdAndColliderData->find(role_id);
			if (iter == kvRoleIdAndColliderData->end())
			{
				return false;
			}
			std::map<int,CCMoveAreaData>::iterator mov_iter;
			mov_iter = (*iter).second.kvMoveIdAndAreaData.find(action_id);
			if (mov_iter == ((*iter).second.kvMoveIdAndAreaData).end())
			{
				return false;
			}
			data = (*mov_iter).second;
			return true;
	}

	bool getViewRectData(int role_id,std::string armatureName,CCAreaData &areaData)
	{
		std::map<int,CCColliderData>::iterator iter;
		iter = kvRoleIdAndColliderData->find(role_id);
		if (iter == kvRoleIdAndColliderData->end())
		{
			return false;
		}
		ArmatureActionData *pData = SkeletonAnimRcsManager::getInstance()->getOneRoleArmatureActionData(role_id);
		if (pData == 0)
		{
			return false;
		}
		for (std::map<RoleActionType,ArmatureData >::iterator _iter = pData->kvRoleActionTypeAndValues.begin();
			 _iter != pData->kvRoleActionTypeAndValues.end();_iter++)
		{
			ArmatureData armatureData = (*_iter).second;
			if (armatureData.xmlFileName == armatureName)
			{
				RoleActionType actionType = (*_iter).first;

				std::map<int,CCMoveAreaData>::iterator mov_iter;
				mov_iter = (*iter).second.kvMoveIdAndAreaData.find(actionType);
				if (mov_iter != ((*iter).second.kvMoveIdAndAreaData).end())
				{
					CCAreaData tmpAreaData = (*mov_iter).second.selfViewRectArea;
					if (tmpAreaData.IsValid())
					{
						areaData = tmpAreaData;
						return true;
					}
				}								
			}
		}		
		return false;
	}

	bool GetRoleViewRectData(unsigned int roleId,std::map<std::string,CCAreaData> & areaData);

	std::map<int,std::map<int,CCPoint> > * getAttachPointsMap(){
		return kvRoleIdAndAttachPointsData;
	}

	/**
	* Instruction : 
	* @param 
	*/
	CCPoint getOneImageAnchorPoint(const std::string &imageName);

	void ClearDataExceptColliderAndPointData();
	bool GetRoleColliderData(unsigned int id,CCColliderData& colliderData);
private:
    CCFrameNodeList	*getFrameNodeList(CConnDataReader* pFile, const char* _boneName);

    CCFrameNode	*getFrameNode(CConnDataReader* pFile);

	void decodeTextures(CConnDataReader* pFile);
	void decodeArmatures(CConnDataReader* pFile);
	void decodeAnimations(CConnDataReader* pFile);

	void decodeColliderAreaAndPointData(TiXmlElement *_rootXML);
	void decodeColliderAreas(TiXmlElement *_collidersXML,CCColliderData &);
	void decodeAttachPointData(TiXmlElement *_attachPointsXML,std::map<int,CCPoint> &);	
private:
    //save armature datas.			 <const char*, std::vector<PCCArmatureData> > ;
	CC_SYNTHESIZE_READONLY(CCDictionary *, mArmarureDatas, ArmarureDatas);

    //save armature animation datas. <const char*, CCArmatureAniData*> ;
	CC_SYNTHESIZE_READONLY(CCDictionary *, mAnimationDatas, AnimationDatas);

	//save texture datas			 <const char*, CCTextureData*>
	CC_SYNTHESIZE_READONLY(CCDictionary *, mTextureDatas, TextureDatas);
private:
    static CCConnectionData *sSharedConnectionData;
	std::map<int,CCColliderData> *kvRoleIdAndColliderData;
	std::map<int,std::map<int,cocos2d::CCPoint> > *kvRoleIdAndAttachPointsData;

	// Note: 优化得到角色碰撞框数据的速度
	///std::map<unsigned int,std::map<unsigned int,CCMoveAreaData> > m_kvRoleIdAndColliderData;
};

