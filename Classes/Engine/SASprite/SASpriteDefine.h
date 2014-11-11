//SASpriteDefine.h file - contains all the skeleton attach define
#ifndef _skeleton_sprite_Define_ 
#define _skeleton_sprite_Define_  


#define SKELETON_ANIMATION_DIR "skeleton_animation/"
#define DATA_DIR "Data/"
#define SKELETON_IMAGE_FILE_SUFFIX ".pvr.ccz"
//#define SKELETON_IMAGE_FILE_SUFFIX ".png"

#include "cocos2d.h"
#include "map"
#include <algorithm>

using namespace cocos2d;

enum ANIMATION_STATE
{
	kType_START = 0,
	kType_RUN,
	kType_COMPLETE,
	kType_ANI_STATE_UNKNOW
};

enum ATTACH_POINT_TYPE
{
	kType_PLAYER_NAME = 0,
	kType_ATTACK_HURT,
	kType_SHOOT_HOR,
	kType_SHOOT_UP,
	kType_SHOOT_DOWN
};

enum RoleSexType 
{
	kTypeBoy = 0,
	kTypeGirl
};

enum RoleActionType
{
	// 特效
	kType_Play = 100000,
	kType_Play1 = 100001,
	kType_Play2 = 100002,
	kType_Play3 = 100003,
	kType_Play4 = 100004,
	kType_Play5 = 100005,
	// idle 主城
	kTypeIdle_FaceHor_Stand_MAIN = 0,
	kTypeIdle_FaceDown_Stand_MAIN	= 1000,
	kTypeIdle_FaceUp_Stand_MAIN		= 1001,

	// idle 副本
	kTypeIdle_FaceHor_Stand_OTH		= 1002,	
	kTypeIdle_FaceDown_Stand_OTH	= 1003,
	kTypeIdle_FaceUp_Stand_OTH		= 1004,

	// 随机动画
	kTypeIdle_FaceHor	= 1005,	
	kTypeIdle_FaceDown	= 1006,
	kTypeIdle_FaceUp	= 1007,

	// 主城中跑动
	kTypeRun_FaceHor_MAIN	= 1008,	
	kTypeRun_FaceDown_MAIN	= 1009,
	kTypeRun_FaceUp_MAIN	= 1010,

	// 副本中跑动
	kTypeRun_FaceHor_OTH	= 1011,	
	kTypeRun_FaceDown_OTH	= 1012,
	kTypeRun_FaceUp_OTH		= 1013,

	// 攻击1
	kTypeAttack_FaceHor_1	= 1014,
	kTypeAttack_FaceDown_1	= 1015,
	kTypeAttack_FaceUp_1	= 1016,

	// 攻击2
	kTypeAttack_FaceHor_2	= 1017,
	kTypeAttack_FaceDown_2	= 1018,
	kTypeAttack_FaceUp_2	= 1019,

	// 攻击3
	kTypeAttack_FaceHor_3	= 1020,
	kTypeAttack_FaceDown_3	= 1021,
	kTypeAttack_FaceUp_3	= 1022,

	// 受伤
	kTypeHurt_FaceHor	= 1023,
	kTypeHurt_FaceDown	= 1024,
	kTypeHurt_FaceUp	= 1025,

	// 死亡
	kTypeDie_FaceHor	= 1026,
	kTypeDie_FaceDown	= 1027,
	kTypeDie_FaceUp		= 1028,

	kTypeAction_UnKnow	= 1046
};

enum RoleJobType
{
	kTypeFighter_Boy = 1,
	kTypeFighter_Girl = 2,
	kTypeMagician_Boy = 3,
	kTypeMagician_Girl = 4,
	kTypeAssassin_Boy = 5,
	kTypeAssassin_Girl = 6,
	kTypeHunter_Boy = 7,
	kTypeHunter_Girl = 8,

	kType_ALL_ROLE = -1
};

// 角色相关的资源信息
struct ArmatureRcsData
{
public:
	ArmatureRcsData()
	{
		mVecOtherLandFileNames = new std::vector<std::string>();
		mVecMainLandFileNames = new std::vector<std::string>();
		mVecAttachEffects = new std::vector<int>();
	}

	~ArmatureRcsData()
	{
		if (0 != mVecOtherLandFileNames)
		{
			mVecOtherLandFileNames->clear();
			CC_SAFE_DELETE(mVecOtherLandFileNames);
		}

		if (0 != mVecMainLandFileNames)
		{
			mVecMainLandFileNames->clear();
			CC_SAFE_DELETE(mVecMainLandFileNames);
		}

		if (0 != mVecAttachEffects)
		{
			mVecAttachEffects->clear();
			CC_SAFE_DELETE(mVecAttachEffects);
		}
	}

	bool AddOneElement(std::string file_name,bool bOtherLand)
	{
		if (bOtherLand)
		{
			mVecOtherLandFileNames->push_back(file_name);
		}
		else
		{
			mVecMainLandFileNames->push_back(file_name);
		}
		return true;
	}

	bool AddOneEffectID(int id)
	{
		if (find(mVecAttachEffects->begin(), mVecAttachEffects->end(), id) == mVecAttachEffects->end())
		{
			mVecAttachEffects->push_back(id);
			return true;
		}
		return false;
	}

	int getCount(bool bOtherLand)
	{
		if (bOtherLand)
		{
			if (0 != mVecOtherLandFileNames)
			{
				return mVecOtherLandFileNames->size();
			}
		}
		else
		{
			if (0 != mVecMainLandFileNames)
			{
				return mVecMainLandFileNames->size();
			}
		}

		return 0;
	}

	std::vector<std::string> *mVecOtherLandFileNames;
	std::vector<std::string> *mVecMainLandFileNames;
	// 依赖的特效ID
	std::vector<int> *mVecAttachEffects;
};

struct RoleArmatureRcsData
{
public:
	RoleArmatureRcsData()
	{		
	}

	~RoleArmatureRcsData()
	{
		for (std::map<int,ArmatureRcsData *>::iterator iter = kvRoleIdAndRcsNameMap.begin();
			iter != kvRoleIdAndRcsNameMap.end(); iter++)
		{
			delete((*iter).second);
			(*iter).second = 0;
		}

		kvRoleIdAndRcsNameMap.clear();
	}

	bool AddOneElement(int id,ArmatureRcsData *data)
	{
		if (0 == data)
		{
			return false;
		}
		kvRoleIdAndRcsNameMap.insert(std::make_pair(id,data));
		return true;
	}

	std::map<int,ArmatureRcsData *> kvRoleIdAndRcsNameMap;
};

// 地图绑定的角色资源信息
struct MapAttachedData
{
public:
	MapAttachedData()
	{

	}

	~MapAttachedData()
	{
		mCommonIdVec.clear();
		mMainLandEffectCommonIdVec.clear();
		mOthLandEffectCommonIdVec.clear();
		for (std::map<int,std::vector<int>*>::iterator iter = mMapIdAndAttachIdMap.begin();
			iter != mMapIdAndAttachIdMap.end(); iter++)
		{
			(*iter).second->clear();
			CC_SAFE_DELETE((*iter).second);
		}
		mMapIdAndAttachIdMap.clear();
	}

	std::vector<int> mCommonIdVec;
	std::vector<int> mMainLandEffectCommonIdVec;
	std::vector<int> mOthLandEffectCommonIdVec;
	std::map<int,std::vector<int>*> mMapIdAndAttachIdMap;	
};

// 骨架基本信息列表
struct ArmatureBasicInfo
{
public:
	ArmatureBasicInfo()
	{
		armature_index = 0;
	}
public:
	int armature_index;
	std::string armature_name;
	std::string animation_name;
	std::string png_file_name;
};

// 骨架数据 
struct ArmatureData
{
public:
	ArmatureData()
	{
		this->xmlFileName = "";
		this->actionName = "";
	}
	ArmatureData(std::string xmlFileName,std::string actionName)
	{
		this->xmlFileName = xmlFileName;
		this->actionName = actionName;
	}
	~ArmatureData()
	{
	}
	std::string xmlFileName;
	std::string actionName;
};

// 骨架的动作信息
class ArmatureActionData
{
public:
	ArmatureActionData()
	{

	}

	~ArmatureActionData()
	{
		kvRoleActionTypeAndValues.clear();
	}

	std::map<RoleActionType,ArmatureData > kvRoleActionTypeAndValues;
};

class RoleArmatureActionData
{
public:
	RoleArmatureActionData()
	{
		
	}

	~RoleArmatureActionData()
	{
		for (std::map<int,ArmatureActionData *>::iterator iter = kvRoleTypeAndValues.begin();
			iter != kvRoleTypeAndValues.end(); iter++)
		{
			delete((*iter).second);
		}

		kvRoleTypeAndValues.clear();
	}
public:
	std::map<int,ArmatureActionData *> kvRoleTypeAndValues;
};

#endif //_skeleton_sprite_Define_