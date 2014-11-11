#pragma once
#include "cocos2d.h"
#include "AParticleNode.h"
#include "Singleton_t.h"


using namespace cocos2d; 

class HeroParticleListener : public TSingleton<HeroParticleListener>
{
public:

	HeroParticleListener();
	~HeroParticleListener();

public:
	virtual void	HandleAddParticleEvent(int index, AParticleNode particle);
	virtual void	HandleRemoveParticleEvent(int index);
private:

	static HeroParticleListener*		sInstance;
	std::map<int, AParticleNode> mParticleList;

};