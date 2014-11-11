//
//  ParticleEmitters.h
//  HelloCpp
//
//  Created by michaelwong on 1/25/13.
//
//
#ifndef __PARTICLE_EMITTERS_H__
#define __PARTICLE_EMITTERS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <stdio.h>

USING_NS_CC;
USING_NS_CC_EXT;

// map for particles storage
typedef std::map<std::string, CCParticleSystemQuad *>   EmitterMap;
typedef std::pair<std::string, CCParticleSystemQuad *>  EmitterMapEntry;

class ParticleEmitters
: public CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ParticleEmitters, create);
  
  ParticleEmitters();
  virtual ~ParticleEmitters();
  
  virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
  virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
  virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
  virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
  
  virtual bool init();
  
  // get particle object with name
  CCParticleSystemQuad* getParticle(const char *name);
    
protected:
  
  // particle name -> particle object array
  EmitterMap            m_pEmitterMap;
  
  // get a copy of particle object
  CCParticleSystemQuad  *createParticleWithParticle(CCParticleSystemQuad *one);
};

class ParticleEmittersLoader : public cocos2d::extension::CCLayerLoader {
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ParticleEmittersLoader, loader);
    
protected:
  CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ParticleEmitters);
};

#endif /* defined(__PARTICLE_EMITTERS_H__) */
