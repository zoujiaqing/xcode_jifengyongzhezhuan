#pragma once

#include "cocos2d.h"
#include "iostream"

#include "SASpriteDefine.h"
#include "CArmature.h"
#include "BoneSpriteBase.h"

class BoneSpriteBase;

using namespace cocos2d;

/*骨架动画层次管理*/
class RoleActorBase : public CCArmature,
					  public CCAnimationEventListener
{
	
public:
	virtual ~RoleActorBase(void);


	/************************************************************************
	* param:
	* _name: 骨架名
	* _animationName: 动画名
	* _plistPath: pList路径
	* _imagePath: png路径
	* png_file_name: 资源文件名
	* _index: 层
	************************************************************************/
	static RoleActorBase *create(std::string _name, std::string _animationName, 
								 std::string png_file_name, int _index = 0);

	// 初始化骨骼动画
	virtual bool init(std::string _name, std::string _animationName,
					  CCSpriteBatchNode * _batchNodeL, int _index = 0);
	/************************************************************************
	* param:
	* _aniType: start , complete , loopComplete , inFrame
	* _aniID: 动画名
	* _frameID: 帧ID	
	************************************************************************/
	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);

	/************************************************************************
	* 设置动画附属的角色
	************************************************************************/
	bool setParentRole(BoneSpriteBase *parent);
	bool setParentAnimListerner(CCAnimationEventListener* pListerner);

	CCSpriteBatchNode *getBatchNode(){return mBatchNode;}

	static CCSpriteBatchNode * createBatchNode(std::string png_file_name);

protected:
	RoleActorBase();
	
private:
	BoneSpriteBase *mParentSpriteBase;
	CCSpriteBatchNode *mBatchNode;	
	CCAnimationEventListener* m_pAnimationEventListerner;
};