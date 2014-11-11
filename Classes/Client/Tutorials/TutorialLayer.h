#pragma once

#include "cocos2d.h"
#include "string"
#include "cocos-ext.h"

using namespace std;
using namespace cocos2d;

class TutorialLayer : public cocos2d::CCLayer
{
public:
	TutorialLayer();
	virtual ~TutorialLayer();

	CREATE_FUNC(TutorialLayer);

	virtual bool init();  
	//void update(float dt);
	
	void ShowLayer();
	//void ShowLayer(std::string uiName);
	//void HideLayer();
	//void ShowFlagIconAtPos_2(CCPoint pt,const char* content = "",unsigned int type = 0);
	void HideAllFlagIcon();
	void ShowLayerActorOrNot(bool bShow);

	//显示及修正箭头位置
	void ShowFlagIconAtPos(CCPoint pt,const char* content = "",unsigned int type = 0);
	void ChangeFLagPos(float deltaX,float deltaY,unsigned int type = 0);

	

	//设置及修正裁切区域位置及大小 0为第一个区域，1为第二个区域
	void SetClipAreaPos(int index, CCPoint pos, CCSize size);
	void ChangeClipAreaPos(int index, float deltaX, float deltaY);

	//设置及修正手指圈圈位置
	void SetCirclePos(CCPoint pos);
	void ChangeCirclePos(float deltaX, float deltaY);

protected:
	void ShowLayerCommonInit();

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	CCSprite* createOneFlag(std::string bgName,std::string txtName);
private:
	CCSprite* m_pUISprite;

	CCNode* m_pFlagParentNode;
	CCNode* m_pFlagParentNode_2;
	CCSprite* m_pCurFlagSprite;
	CCSprite* m_pCurFlagSprite_2;

	CCClippingNode * m_pClipNode;
	cocos2d::extension::CCScale9Sprite * m_pClipArea1;
	cocos2d::extension::CCScale9Sprite * m_pClipArea2;

	//手指及圆圈
	CCNode * m_pFingerNode;
	CCNode * m_pCircleNode;

	CCSprite * m_pFingerPic;
	CCSprite * m_pCirclePic;

	enum {FLAG_SPRITE_LABEL_TAG = 10001};
};
