#ifndef ATLOBJECTLAYER_2013_1_20_H
#define ATLOBJECTLAYER_2013_1_20_H

#include "cocos2d.h"
#include "ATLObjectInfo.h"
#include <vector>

#include "../../../cocos2dx/cocos2dx/sprite_nodes/CCSpriteBatchNode.h"

using namespace  std;


class LevelLayer;
class AnimSprite;


// ------------------------------------------------------------------------

USING_NS_CC;

class AlphaTestBatchNode : public CCSpriteBatchNode
{
	GLint m_location;
	float m_coef;
    bool m_isNeedUpdteTransformEveryFrame;
    unsigned int m_updateTransformCount;
	bool m_bDepthTest;
public:
	AlphaTestBatchNode();
	virtual ~AlphaTestBatchNode();
	static AlphaTestBatchNode* create(const char* pszFileName);
	bool initWithFile(const char* fileImage, unsigned int capacity);
	bool initWithTexture(CCTexture2D* pTexture, unsigned int capacity);
	virtual void draw();

	void SetCoef(float coef){m_coef = coef;}
    void SetIsUpdateEveryFrame(bool bNeed){m_isNeedUpdteTransformEveryFrame = bNeed;}

	void SetDepthTest(bool b){m_bDepthTest = b;}


};

class RoleShadowBatchNode : public CCSpriteBatchNode
{
protected:
	
public:
	RoleShadowBatchNode();
	virtual ~RoleShadowBatchNode();
	static RoleShadowBatchNode* create(const char* pszFileName);
	virtual void visit();
    virtual void draw();
	void RemoveShadow(CCNode* child);
};

// ------------------------------------------------------------------------
class CATLObjectLayer  : public cocos2d::CCLayer
{
public:
	CATLObjectLayer();
	virtual ~CATLObjectLayer();

	void ParseObject(CATLObjectInfo* pObjectInfo, int levelID, LevelLayer* pLevelLayer);

	virtual void draw(void);
	virtual void addChild(CCNode * child, int zOrder);
    virtual void reorderChild(CCNode *child, int zOrder);
    
	//// for debug usage
	size_t getBatchNodeNumber();
    
	void AddRoleShadow(cocos2d::CCNode* pRole);

	virtual void removeChild(CCNode* child, bool cleanup);

protected:
	/// parse layer information , build objects  in the current layer
	void ParseLayer(CATLObjectInfo::LAYER::BASE* pLayer, CATLObjectInfo::VECTOR2D parentPos);

	/// remove all the objects
	void  RemoveLayerObjects();

	void ParseImmovableLayer(CATLObjectInfo::LAYER::OBJECT* pObject, const cocos2d::CCPoint& pos, CCSpriteBatchNode* pBatchNode, ASprite* pAsprite, cocos2d::CCTexture2D* pTexture);
	void ParseAnimationLayer(CATLObjectInfo::LAYER::OBJECT* pObject, const cocos2d::CCPoint& pos, ASprite* pAsprite, cocos2d::CCTexture2D* pTexture);

protected:
	int m_height;
    float m_objectScale;

	vector<AnimSprite* > m_lstAnimSprites;

	struct OBJECT_BATCHNODE
	{
		OBJECT_BATCHNODE():pBatchNode(NULL),pAsprite(NULL),pTexture(NULL){}

		CCSpriteBatchNode* pBatchNode;
		ASprite* pAsprite;
		cocos2d::CCTexture2D* pTexture;

	};
	std::map<std::string, OBJECT_BATCHNODE> m_objectBatchNode;

	RoleShadowBatchNode* m_pRoleShadow;
};

#endif