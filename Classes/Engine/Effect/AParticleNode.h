#pragma once
#include "cocos2d.h"
using namespace cocos2d; 

class AParticleNode : public cocos2d::CCNode
{
public:
	static AParticleNode* CreateParticleNode(const char* plistfile,unsigned int index);
	virtual ~AParticleNode(void);
	virtual bool init(const char* plistfile);  

	int GetTotalParticleNum();
	CCParticleSystemQuad* GetParticle();
private:
	AParticleNode(unsigned int index);

protected: 
	CCParticleSystemQuad* m_emitter; 
	unsigned int mIndex;
};