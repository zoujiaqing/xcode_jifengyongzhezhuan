//
//  particle_manager.cpp
//  HelloCpp
//
//  Created by michaelwong on 1/25/13.
//
//

#include "particle_manager.h"
#include "particle_emitters.h"
#include "GameResourceManager.h"

// singleton stuff
static ParticleManagerX *s_SharedParticleManager = NULL;

ParticleManagerX* ParticleManagerX::sharedManager(void)
{
  if (!s_SharedParticleManager)
  {
    s_SharedParticleManager = new ParticleManagerX();
    s_SharedParticleManager->init();
  }
  
  return s_SharedParticleManager;
}

void ParticleManagerX::purgeParticleManager(void)
{
	
  CC_SAFE_DELETE(s_SharedParticleManager);
}

ParticleManagerX::ParticleManagerX()
{
    m_ParticleEmittersMap.clear();
}

ParticleManagerX::~ParticleManagerX()
{
  for(ParticleEmittersMap::iterator it = this->m_ParticleEmittersMap.begin(); it != this->m_ParticleEmittersMap.end(); it++) {
    CC_SAFE_RELEASE(it->second);
	//it->second->removeFromParentAndCleanup(true);
  }
  this->m_ParticleEmittersMap.clear();
}

bool ParticleManagerX::init(void)
{
  return true;
}

// load resource
bool ParticleManagerX::load(const char * fileName)
{
  CCAssert(fileName && strlen(fileName) > 0, "!!! ParticleManager::load(fileName) fileName is invalid");
  if (!fileName || strlen(fileName) <= 0) {
    return false;
  }
  bool result = false;
  // load particles from ccbi file
  ParticleEmittersMap::iterator emittersIterator = this->m_ParticleEmittersMap.find(fileName);
  if (emittersIterator == this->m_ParticleEmittersMap.end()) {
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("ParticleEmitters", ParticleEmittersLoader::loader());
    
    // Create an autorelease CCBReader.
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
      
    ccbReader->setCCBRootPath("particle/");
    //ccbReader->setCCBRootPath("");
      
    // Load the scene from the ccbi-file, setting this class as
    // the owner will cause lblTestTitle to be set by the CCBReader.
    // lblTestTitle is in the TestHeader.ccbi, which is referenced
    // from each of the test scenes.
    ParticleEmitters *pParticleEmitters = (ParticleEmitters*)ccbReader->readNodeGraphFromFile(fileName, this);
	CC_SAFE_RETAIN(pParticleEmitters);
    if (pParticleEmitters) {
      this->m_ParticleEmittersMap.insert(ParticleEmittersMapEntry(fileName, pParticleEmitters));
      result = true;
    }
    CC_SAFE_RELEASE_NULL(ccbReader);
  }
  else {
    result = true;
  }
    
  return result;
}

// unload resource
bool ParticleManagerX::unload(const char *fileName)
{
  CCAssert(fileName && strlen(fileName) > 0, "!!! ParticleManager::unload(fileName) fileName is invalid");
  if (!fileName || strlen(fileName) <= 0) {
    return false;
  }
  bool result = false;
  ParticleEmittersMap::iterator emittersIterator = this->m_ParticleEmittersMap.find(fileName);
  if (emittersIterator != this->m_ParticleEmittersMap.end()) {
    CC_SAFE_RELEASE_NULL(emittersIterator->second);
    this->m_ParticleEmittersMap.erase(emittersIterator);
  }
  return result;
}

CCParticleSystemQuad* ParticleManagerX::getParticle(const char *name)
{
  CCAssert(name && strlen(name) > 0, "!!! ParticleManager::getParticle(name) name is invalid");
  if (!name || strlen(name) <= 0) {
    return NULL;
  }
  CCParticleSystemQuad *particle = NULL;
  for(ParticleEmittersMap::iterator it = this->m_ParticleEmittersMap.begin(); it != this->m_ParticleEmittersMap.end(); it++) {
    ParticleEmitters *emitters = (ParticleEmitters*)(it->second);
    if (emitters && name) {
      particle = emitters->getParticle(name);
    }
    if (particle) {
      particle->setAutoRemoveOnFinish(true);
      break;
    }
  }
  return particle;
}

CCNode* ParticleManagerX::getParticles_uieffect(const char* name)
{
    //return NULL;
    if(ParticleManagerX::sharedManager()->load("particle/ccb/3333.ccbi"))
    {
        if(!strcmp(name,"particle_effect_upgradesuccess"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_upgradesuccess1");

            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_upgradesuccess2");

            node->addChild(particle1);
            node->addChild(particle2);
            particle1->setAutoRemoveOnFinish(true);
            particle2->setAutoRemoveOnFinish(true);
            return node;
        }
        else if(!strcmp(name,"particle_effect_mixsuccess"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess4");
            CCParticleSystemQuad* particle5 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess5");
            CCParticleSystemQuad* particle6 = ParticleManagerX::sharedManager()->getParticle("particle_effect_mixsuccess6");

            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            node->addChild(particle5);
            node->addChild(particle6);
            
            particle1->setAutoRemoveOnFinish(true);
            particle2->setAutoRemoveOnFinish(true);
            particle3->setAutoRemoveOnFinish(true);
            particle4->setAutoRemoveOnFinish(true);
            particle5->setAutoRemoveOnFinish(true);
            particle6->setAutoRemoveOnFinish(true);
            
            particle3->setPosition(ccp(-107.1, -5.2));
            particle4->setPosition(ccp(3.5, 98.5));
            particle5->setPosition(ccp(112.0, -5.2));
            particle6->setPosition(ccp(1.7, -110.6));
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_transformelite"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_transformelite1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_transformelite2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("particle_effect_transformelite3");
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            particle1->setAutoRemoveOnFinish(true);
            particle2->setAutoRemoveOnFinish(true);
            particle3->setAutoRemoveOnFinish(true);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_soulupgrade"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_soulupgrade1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_soulupgrade2");
            node->addChild(particle1);
            node->addChild(particle2);
            particle1->setAutoRemoveOnFinish(true);
            particle2->setAutoRemoveOnFinish(true);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_findsoulsuccess"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_findsoulsuccess1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_findsoulsuccess2");
            node->addChild(particle1);
            node->addChild(particle2);
            particle1->setAutoRemoveOnFinish(true);
            particle2->setAutoRemoveOnFinish(true);
            return node;
        }
        else if(!strcmp(name,"particle_effect_taskguide"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("particle_effect_taskguide1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("particle_effect_taskguide2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("particle_effect_taskguide3");

            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            return node;
        }
    }
    
    return NULL;
}

CCNode* ParticleManagerX::getParticles(const char* name)
{
    if(ParticleManagerX::sharedManager()->load("particle/ccb/TestParticleSystems.ccbi"))
    {
        if(!strcmp(name,"particle_effect_spirit_474014"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474014_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474014_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474014_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474014_4");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474008"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474008_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474008_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474008_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474008_4");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474009"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474009_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474009_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474009_3");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474013"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474013_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474013_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474013_3");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474011"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474011_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474011_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474011_3");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474015"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474015_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474015_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474015_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474015_4");

            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474003"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474003_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474003_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474003_3");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474004"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474004_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474004_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474004_3");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474005"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474005_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474005_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474005_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474005_4");

            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474006"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474006_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474006_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474006_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474006_4");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474007"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474007_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474007_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474007_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474007_4");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474002"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474002_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474002_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474002_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474002_4");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474001"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474001_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474001_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474001_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474001_4");
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474010"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474010_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474010_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474010_3");
            CCParticleSystemQuad* particle4 = ParticleManagerX::sharedManager()->getParticle("474010_4");
            CCParticleSystemQuad* particle5 = ParticleManagerX::sharedManager()->getParticle("474010_5");

            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            node->addChild(particle4);
            node->addChild(particle5);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            particle4->setAutoRemoveOnFinish(false);
            particle5->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_474012"))
        {
            CCNode* node = CCNode::create();
            
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("474012_1");
            CCParticleSystemQuad* particle2 = ParticleManagerX::sharedManager()->getParticle("474012_2");
            CCParticleSystemQuad* particle3 = ParticleManagerX::sharedManager()->getParticle("474012_3");
            
            
            node->addChild(particle1);
            node->addChild(particle2);
            node->addChild(particle3);
            
            particle1->setAutoRemoveOnFinish(false);
            particle2->setAutoRemoveOnFinish(false);
            particle3->setAutoRemoveOnFinish(false);
            
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473001"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473001_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473002"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473002_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473007"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473007_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473010"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473010_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473006"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473006_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473005"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473005_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473003"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473003_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473011"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473011_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473004"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473004_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473009"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473009_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473014"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473014_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473008"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473008_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        else if(!strcmp(name,"particle_effect_spirit_473012"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473012_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473013"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473013_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
        
        else if(!strcmp(name,"particle_effect_spirit_473015"))
        {
            CCNode* node = CCNode::create();
            CCParticleSystemQuad* particle1 = ParticleManagerX::sharedManager()->getParticle("473015_1");
            node->addChild(particle1);
            particle1->setAutoRemoveOnFinish(false);
            return node;
        }
    }
	return NULL;
}













