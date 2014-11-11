#pragma once

#include "TutorialsDataCenter.h"
#include "cocos2d.h"

class TutorialBase:public CCObject
{
public:
	TutorialBase();
	TutorialBase(unsigned int id);
	virtual ~TutorialBase();

	virtual void Start(const char * attachStartControlName = "") = 0;
	virtual void End() = 0;
	virtual void Update(float dt) = 0;

	void SetTutorailType(unsigned int type)
	{
		mTutorialType = type;
	}
protected:
	ETutorialType m_eTutorialType;
	unsigned int m_nTutorialID;	
	unsigned int mTutorialType;
};

class UITutorialBase
{
public :
    virtual CCNode* getTutorialNode(std::string name) = 0;
};