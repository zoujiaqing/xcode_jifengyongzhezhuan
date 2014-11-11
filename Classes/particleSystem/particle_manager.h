//
//  particle_manager.h
//  HelloCpp
//
//  Created by michaelwong on 1/25/13.
//
//

#ifndef __PARTICLE_MANAGER__
#define __PARTICLE_MANAGER__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class ParticleEmitters;

// .ccbi file name -> ParticleEmitters object
typedef std::map<std::string, ParticleEmitters *>   ParticleEmittersMap;
typedef std::pair<std::string, ParticleEmitters *>  ParticleEmittersMapEntry;

class ParticleManagerX
: public CCObject
{
public:
  
  ParticleManagerX();
  virtual ~ParticleManagerX();
  
  // returns a shared instance of the ParticleManager
  static ParticleManagerX* sharedManager(void);
  
  static void purgeParticleManager(void);

protected:
  
  // this is singlton class
  bool init();

public:
  
  // load ccbi particle resource
  bool load(const char *fileName);
  // unload ccbi particle resource
  bool unload(const char *fileName);
  
  // get particle object with name
  CCParticleSystemQuad* getParticle(const char* name);
  CCNode* getParticles(const char* name);
  CCNode* getParticles_uieffect(const char* name);
protected:
  
  ParticleEmittersMap m_ParticleEmittersMap;
};

#endif /* defined(__PARTICLE_MANAGER__) */
