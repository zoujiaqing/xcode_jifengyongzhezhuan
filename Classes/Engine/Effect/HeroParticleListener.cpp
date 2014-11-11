#include "HeroParticleListener.h"
#include "ParticleManager.h"

HeroParticleListener* HeroParticleListener::sInstance = NULL;


HeroParticleListener::HeroParticleListener()
{

}


HeroParticleListener::~HeroParticleListener()
{

}

void  HeroParticleListener::HandleAddParticleEvent(int index, AParticleNode particle)
{

}

void	HeroParticleListener::HandleRemoveParticleEvent(int index)
{
	if (ParticleManager::Get())
	{
		ParticleManager::Get()->HandleParticleRemoved(index);
	}
}