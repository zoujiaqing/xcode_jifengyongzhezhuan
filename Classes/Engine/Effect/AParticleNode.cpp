#include "AParticleNode.h"
#include "HeroParticleListener.h"

AParticleNode::AParticleNode(unsigned int index)
{
	mIndex = index;
}


AParticleNode::~AParticleNode(void)
{
	if (HeroParticleListener::Get())
	{
		HeroParticleListener::Get()->HandleRemoveParticleEvent(mIndex);
	}
}

AParticleNode* AParticleNode::CreateParticleNode(const char* plistfile,unsigned int index)
{
	AParticleNode* particleNode = new AParticleNode(index);
	if(particleNode && particleNode->init(plistfile))
	{
		particleNode->autorelease();
		return particleNode;
	}
	CC_SAFE_DELETE(particleNode);
	return NULL;
}

bool AParticleNode::init(const char* plistfile)
{
		m_emitter = CCParticleSystemQuad::create(plistfile);
		m_emitter->setBlendAdditive(false);
		m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());
		//ÊÇ·ñ»ìºÏ
		//CCSize s = CCDirector::sharedDirector()->getWinSize(); 
		//emitter->setPosition(ccp( s.width/2-30, s.height/2+60));
		addChild(m_emitter); 
		return true;
}

int AParticleNode::GetTotalParticleNum()
{
	if(m_emitter)
	{
		return m_emitter->getTotalParticles();
	}
	return 0;
}

CCParticleSystemQuad* AParticleNode::GetParticle()
{
	return m_emitter;
}