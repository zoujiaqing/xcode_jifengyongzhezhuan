//
//  ParticleEmitters.cpp
//  HelloCpp
//
//  Created by michaelwong on 1/25/13.
//
//

#include "particle_emitters.h"

ParticleEmitters::ParticleEmitters()
{
}

bool ParticleEmitters::init()
{
  bool ret = CCLayer::init();
  return ret;
}


ParticleEmitters::~ParticleEmitters()
{
  for(EmitterMap::iterator it = this->m_pEmitterMap.begin(); it != this->m_pEmitterMap.end(); it++) {
    CC_SAFE_RELEASE(it->second);
  }
  this->m_pEmitterMap.clear();
}

SEL_MenuHandler ParticleEmitters::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
  return NULL;
}

SEL_CCControlHandler ParticleEmitters::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
  return NULL;
}

bool ParticleEmitters::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
  // just parse CCParticleSystemQuad type object
  if (dynamic_cast<CCParticleSystemQuad*>(pNode)) {
    EmitterMap::iterator ccNodeLoadersIterator = this->m_pEmitterMap.find(pMemberVariableName);
    if (ccNodeLoadersIterator == this->m_pEmitterMap.end()) {
      CCParticleSystemQuad *particle = dynamic_cast<CCParticleSystemQuad*>(pNode);
      if (particle) {
        particle->retain();
        particle->removeFromParent();
        m_pEmitterMap.insert(EmitterMapEntry(pMemberVariableName, particle));
      }
    }
  }
  return true;
}

void ParticleEmitters::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
  CCLOG("ParticleEmitters::onNodeLoaded -> %d", m_pEmitterMap.size());
}

// get particle object with name
CCParticleSystemQuad* ParticleEmitters::getParticle(const char *name)
{
  assert(name && strlen(name) > 0);
  if (!name || strlen(name) <= 0) {
    return NULL;
  }
  EmitterMap::iterator ccNodeLoadersIterator = this->m_pEmitterMap.find(name);
  //assert(ccNodeLoadersIterator != this->m_pEmitterMap.end());
  if (ccNodeLoadersIterator != this->m_pEmitterMap.end()) {
    return createParticleWithParticle(ccNodeLoadersIterator->second);
  }
  else {
    CCLOG("!!! CAN NOT find particle : %s", name);
    return NULL;
  }
}

// get a copy of particle object
CCParticleSystemQuad  *ParticleEmitters::createParticleWithParticle(CCParticleSystemQuad *one)
{
  if (!one) {
    return NULL;
  }

  CCParticleSystemQuad *newone = CCParticleSystemQuad::createWithTotalParticles(one->getTotalParticles());
  newone->setEmitterMode(one->getEmitterMode());
  newone->setPosVar(one->getPosVar());
  newone->setEmissionRate(one->getEmissionRate());
  newone->setDuration(one->getDuration());
  newone->setLife(one->getLife());
  newone->setLifeVar(one->getLifeVar());
  newone->setStartSize(one->getStartSize());
  newone->setStartSizeVar(one->getStartSizeVar());
  newone->setEndSize(one->getEndSize());
  newone->setEndSizeVar(one->getEndSizeVar());
  newone->setStartSpin(one->getStartSpin());
  newone->setStartSpinVar(one->getStartSpinVar());
  newone->setEndSpin(one->getEndSpin());
  newone->setEndSpinVar(one->getEndSpinVar());
  newone->setAngle(one->getAngle());
  newone->setAngleVar(one->getAngleVar());
  if (kCCParticleModeRadius == newone->getEmitterMode()) {
    newone->setStartRadius(one->getStartRadius());
    newone->setStartRadiusVar(one->getStartRadiusVar());
    newone->setEndRadius(one->getEndRadius());
    newone->setEndRadiusVar(one->getEndRadiusVar());
    newone->setRotatePerSecond(one->getRotatePerSecond());
    newone->setRotatePerSecondVar(one->getRotatePerSecondVar());
  }
  if (kCCParticleModeGravity == newone->getEmitterMode()) {
    newone->setGravity(one->getGravity());
    newone->setTangentialAccel(one->getTangentialAccel());
    newone->setTangentialAccelVar(one->getTangentialAccelVar());
    newone->setSpeed(one->getSpeed());
    newone->setSpeedVar(one->getSpeedVar());
    newone->setRadialAccel(one->getRadialAccel());
    newone->setRadialAccelVar(one->getRadialAccelVar());
  }
  newone->setStartColor(one->getStartColor());
  newone->setStartColorVar(one->getStartColorVar());
  newone->setEndColor(one->getEndColor());
  newone->setEndColorVar(one->getEndColorVar());
  newone->setTexture(one->getTexture());
  newone->setBlendFunc(one->getBlendFunc());
  newone->setAutoRemoveOnFinish(true);
  newone->setPositionType(kCCPositionTypeRelative);
  return newone;
}



