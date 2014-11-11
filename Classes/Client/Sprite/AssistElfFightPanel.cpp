#include "AssistElfFightPanel.h"
#include "GameResourceManager.h"
#include "SpriteElfManager.h"
#include "SpriteElfManager.h"
#include "map"
#include "SkeletonAnimRcsManager.h"
#include "EffectSprite.h"
#include "ParticleManager.h"
#include "LuaTinkerManager.h"
ElfAssistSkillObserver::ElfAssistSkillObserver()
{

}

ElfAssistSkillObserver::~ElfAssistSkillObserver()
{
	SpriteElfManager::Get()->RemoveAssistSkillObserver(this);
}

AssistElfFightPanel::AssistElfFightPanel()
	: m_panelBgPic(NULL)
	, m_leftAssistElfIcon(NULL)
	, m_rightAssistElfIcon(NULL)
	, m_battleUILayer(NULL)
	,parentNodeEffect1(NULL)
    ,effectAnimId1(0)
    ,parentNodeEffect2(NULL)
    ,effectAnimId2(0)
{
}

AssistElfFightPanel::~AssistElfFightPanel()
{
}

void AssistElfFightPanel::init()
{
	if (m_battleUILayer && m_layout)
	{
		m_panelBgPic = m_layout->FindChildObjectByName<UIPicture>("assistFairyBgFrame");
		m_leftAssistElfIcon = m_layout->FindChildObjectByName<UIPicture>("assistFairyIcon_1");
		m_rightAssistElfIcon = m_layout->FindChildObjectByName<UIPicture>("assistFairyIcon_2");

		EquipElfPanel();
		loadNeedAnimationRcs();
	}	
}

CCNode * AssistElfFightPanel::create(CCLayer* pBattleLayer,TXGUI::UILayout* pLayout)
{
 	AssistElfFightPanel * pRet = new AssistElfFightPanel();
	if (pRet)
	{
		pRet->m_battleUILayer = pBattleLayer;
		pRet->m_layout = pLayout;
		pRet->init();
		pRet->autorelease();
	}	
	return pRet;
}

void AssistElfFightPanel::setVisible(bool visible)
{
	if (m_panelBgPic && m_leftAssistElfIcon && m_rightAssistElfIcon)
	{
		m_panelBgPic->setVisible(visible);
		m_leftAssistElfIcon->setVisible(visible);
		m_rightAssistElfIcon->setVisible(visible);
	}
	CCNode::setVisible(visible);
}

void AssistElfFightPanel::EquipElfAtPos(unsigned int pos,unsigned int elfId)
{
	if (pos == 0)
	{
		if (m_leftAssistElfIcon)
		{
          		{
				char buffer[100];
				if(elfId == 0)
				    sprintf(buffer,"UI/icon_fairy_none.pvr.ccz");
				else
					sprintf(buffer,"UI/icon_fairy_%d.pvr.ccz",elfId);
				std::string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(buffer);
				CCSprite* elfSprite = CCSprite::create(path.c_str());
				if (elfSprite)
				{
					CCNode* currentNode = m_leftAssistElfIcon->getCurrentNode();
					CCNode* parentFrameNode = currentNode->getChildByTag(QUALITY_ICON_TAG);
					if (parentFrameNode)
					{
						currentNode->setScale(UIManager::sharedManager()->getScaleFactor());
						parentFrameNode->addChild(elfSprite,1,EFFECT_ICON_TAG);
						CCSize iconSize = parentFrameNode->getContentSize();
						elfSprite->setPosition(ccp(iconSize.width / 2.0,iconSize.height / 2.0));
					}
				}
			}
		}
	}
	else
	{
		if (m_rightAssistElfIcon)
		{
            
			
			{
				char buffer[100];
				if(elfId == 0)
					sprintf(buffer,"UI/icon_fairy_none.pvr.ccz");
				else
					sprintf(buffer,"UI/icon_fairy_%d.pvr.ccz",elfId);
				std::string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(buffer);
				CCSprite* elfSprite = CCSprite::create(path.c_str());
				if (elfSprite)
				{
					CCNode* currentNode = m_rightAssistElfIcon->getCurrentNode();
					CCNode* parentFrameNode = currentNode->getChildByTag(QUALITY_ICON_TAG);
					if (parentFrameNode)
					{
						currentNode->setScale(UIManager::sharedManager()->getScaleFactor());
						parentFrameNode->addChild(elfSprite,1,EFFECT_ICON_TAG);
						CCSize iconSize = parentFrameNode->getContentSize();
						elfSprite->setPosition(ccp(iconSize.width / 2.0,iconSize.height / 2.0));
					}
				}
			}
		}
	}
}

void AssistElfFightPanel::EquipElfPanel()
{
	int status = 0;
	const std::map<PLAYER_ID, std::map<unsigned int,SpriteElfManager::EFairyPos> >& elfPosInfo = SpriteElfManager::Get()->GetElfPosInfo();	
	std::map<PLAYER_ID, std::map<unsigned int,SpriteElfManager::EFairyPos> >::const_iterator iter = elfPosInfo.find(UserData::Get()->getUserId());
	if (iter != elfPosInfo.end())
	{
		for (std::map<unsigned int,SpriteElfManager::EFairyPos>::const_iterator _iter = (*iter).second.begin();
			_iter != (*iter).second.end(); _iter++)
		{
			unsigned int elfId = (*_iter).first;
			if ((*_iter).second == SpriteElfManager::E_Assist_Pos_1)
			{
			    unsigned int Quality =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerFairyQulity",20);
				showQualityFrame(0,Quality);
				EquipElfAtPos(0,elfId);	
				status |= 0x1 ;
			}
			else if ((*_iter).second == SpriteElfManager::E_Assist_Pos_2)
			{
				 unsigned int Quality =LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/Fairy/FairyLuaInterface.lua","G_GetPlayerFairyQulity",21);
				showQualityFrame(1,Quality);
				EquipElfAtPos(1,elfId);	
				status |= 0x10 ;
			}
		}
	}
	if((status & 0x1 )== 0x0)
	{
		showQualityFrame(0,0);
		EquipElfAtPos(0,0);	
	}
    if((status& 0x10)== 0x0 	)
	{
		showQualityFrame(1,0);	
		EquipElfAtPos(1,0);
	}
}

void AssistElfFightPanel::loadNeedAnimationRcs()
{
	//SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(251,true);
	//SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(252,true);
	//SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(253,true);
}

void AssistElfFightPanel::showQualityFrame(unsigned int pos,unsigned int quality)
{
	if (pos == 0)
	{
		if (m_leftAssistElfIcon)
		{
            
			CCSprite* pQualitySprite = dynamic_cast<CCSprite*>(m_leftAssistElfIcon->getCurrentNode()->getChildByTag(QUALITY_ICON_TAG));
			if (pQualitySprite)
			{
				pQualitySprite->removeFromParent();
			}
            
            
			const char* fileName = getQualityFrameNameByQuality(quality);
			fileName = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(fileName);
			pQualitySprite = CCSprite::create(fileName);
			if (pQualitySprite)
			{
				m_leftAssistElfIcon->getCurrentNode()->addChild(pQualitySprite,1,QUALITY_ICON_TAG);
				CCSize iconSize = m_leftAssistElfIcon->getCurrentNode()->getContentSize();
				pQualitySprite->setPosition(ccp(iconSize.width / 2.0,iconSize.height / 2.0));

				CCSize frameIconSize = m_leftAssistElfIcon->getCurrentNode()->getContentSize();
				CCSize spriteSize = pQualitySprite->getContentSize();
				pQualitySprite->setScaleX(frameIconSize.width /  spriteSize.width);
				pQualitySprite->setScaleY(frameIconSize.height / spriteSize.height);
			}
		}
	}
	else
	{
		if (m_rightAssistElfIcon)
		{
			CCSprite* pQualitySprite = dynamic_cast<CCSprite*>(m_rightAssistElfIcon->getCurrentNode()->getChildByTag(QUALITY_ICON_TAG));
			if (pQualitySprite)
			{
				pQualitySprite->removeFromParent();
			}
           
            
			const char* fileName = getQualityFrameNameByQuality(quality);
			fileName = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(fileName);
			pQualitySprite = CCSprite::create(fileName);
			if (pQualitySprite)
			{
				m_rightAssistElfIcon->getCurrentNode()->addChild(pQualitySprite,1,QUALITY_ICON_TAG);
				CCSize iconSize = m_rightAssistElfIcon->getCurrentNode()->getContentSize();
				pQualitySprite->setPosition(ccp(iconSize.width / 2.0,iconSize.height / 2.0));

				CCSize frameIconSize = m_rightAssistElfIcon->getCurrentNode()->getContentSize();
				CCSize spriteSize = pQualitySprite->getContentSize();
				pQualitySprite->setScaleX(frameIconSize.width /  spriteSize.width);
				pQualitySprite->setScaleY(frameIconSize.height / spriteSize.height);
			}
		}
	}
}

const char* AssistElfFightPanel::getQualityFrameNameByQuality(unsigned int quality)
{
	const char* pContent = "";
	switch (quality)
	{
	case 0:
		{
			pContent = "UI/icon_quality_white.pvr.ccz";
			break;
		}
	case 1:
		{
			pContent = "UI/icon_quality_white.pvr.ccz";
			break;
		}
	case 2:
		{
			pContent = "UI/icon_quality_green.pvr.ccz";
			break;
		}
	case 3:
		{
			pContent =  "UI/icon_quality_blue.pvr.ccz";
			break;
		}
	case 4:
		{
			pContent = "UI/icon_quality_purple.pvr.ccz";
			break;
		}
		default:
		{
			pContent = "UI/icon_quality_purple.pvr.ccz";
			break;
		}
	}
	return pContent;
}

unsigned int AssistElfFightPanel::getIconEffectAnimationId(unsigned int type)
{
	unsigned int nRet = 0;
	switch(type)
	{
	case 1:
		{
			nRet = 251;
			break;
		}
	case 2:
		{
			nRet = 252;
			break;
		}
	case 3:
		{
			nRet = 253;
			break;
		}
	}
    
	if (0 != nRet)
	{
		SkeletonAnimRcsManager::getInstance()->LoadOneHeroRcs(nRet,true);
	}
	return nRet;
}
void AssistElfFightPanel::CallBackSkillEffect1() {

	EffectSprite* effect = dynamic_cast<EffectSprite*>(parentNodeEffect1->getChildByTag(EFFECT_ANIM_TAG));
	if (effect)
	{
		effect->removeFromParentAndCleanup(true);
		effect = NULL;
	}

	effect = ParticleManager::Get()->createEffectSprite(effectAnimId1,"");
	if (effect)
	{					
		effect->SetAnim(kType_Play,1,false);
		effect->SetAnimPauseOnOver(false);
		parentNodeEffect1->addChild(effect,1,EFFECT_ANIM_TAG);
		CCSize size = parentNodeEffect1->getContentSize();;
		effect->setPosition(ccp(size.width / 2,size.height/2 ));
	}
	CCActionInterval * delayTime = CCDelayTime::create(0.4);
	CCActionInterval *BackTo = CCScaleBy::create(0.1, 0.5f);
	CCAction *action = CCSequence::create(   delayTime,   
		BackTo,  
		NULL);  
	parentNodeEffect1->runAction(action);
	parentNodeEffect1 = NULL ;

}
void AssistElfFightPanel::CallBackSkillEffect2() {
    
	EffectSprite* effect = dynamic_cast<EffectSprite*>(parentNodeEffect2->getChildByTag(EFFECT_ANIM_TAG));
	if (effect)
	{
		effect->removeFromParentAndCleanup(true);
		effect = NULL;
	}
    
	effect = ParticleManager::Get()->createEffectSprite(effectAnimId2,"");
	if (effect)
	{
		effect->SetAnim(kType_Play,1,false);
		effect->SetAnimPauseOnOver(false);
		parentNodeEffect2->addChild(effect,1,EFFECT_ANIM_TAG);
		CCSize size = parentNodeEffect2->getContentSize();;
		effect->setPosition(ccp(size.width / 2,size.height/2 ));
	}
	CCActionInterval * delayTime = CCDelayTime::create(0.4);
	CCActionInterval *BackTo = CCScaleBy::create(0.1, 0.5f);
	CCAction *action = CCSequence::create(   delayTime,
                                          BackTo,
                                          NULL);
	parentNodeEffect2->runAction(action);
	parentNodeEffect2 = NULL;
    
}

void AssistElfFightPanel::OnSkillTrigger(unsigned int pos,unsigned int type)
{
	//CCLog("OnSkillTrigger %d %d",pos,type);
	//return ;
	
	CCNode* parentNode = NULL;
	if (pos == SpriteElfManager::E_Assist_Pos_1)
	{
		if(parentNodeEffect1 != NULL)
			return ;
        
        effectAnimId1 = getIconEffectAnimationId(type);
		if (m_leftAssistElfIcon)
		{
			parentNode = m_leftAssistElfIcon->getCurrentNode();
		}
        
        if (parentNode)
        {
            
            CCPoint acPos = parentNode->getAnchorPoint();
            if(acPos.y == 0.5)
            {
                CCPoint poself = parentNode->getPosition() ;
                
                parentNode->setAnchorPoint(ccp(0.5 ,0.0));
                parentNode->setPosition(ccpSub(poself, ccp(0.0f ,parentNode->getContentSize().height*UIManager::sharedManager()->getScaleFactor()/2.0)));
            }
			float oldScale = UIManager::sharedManager()->getScaleFactor();
            CCActionInterval *forwardTo1 = CCScaleTo::create(0.1, oldScale * 4.0f);
            
            CCActionInterval *forwardTo2 = CCScaleTo::create(0.05, oldScale * 2.0f);
            
            parentNodeEffect1 = parentNode ;
            CCAction *action = CCSequence::create(   forwardTo1,
                                                  forwardTo2,
                                                  CCCallFunc::create(this, callfunc_selector(AssistElfFightPanel::CallBackSkillEffect1)),
                                                  NULL);
           
            parentNode->runAction(action);  
            
        }
	}
	else if (pos == SpriteElfManager::E_Assist_Pos_2)
	{
		if(parentNodeEffect2 != NULL)
			return ;
        effectAnimId2 = getIconEffectAnimationId(type);
		if (m_rightAssistElfIcon)
		{
			parentNode = m_rightAssistElfIcon->getCurrentNode();
		}
        if (parentNode)
        {
           
            CCPoint acPos = parentNode->getAnchorPoint();
            if(acPos.y == 0.5)
            {
                CCPoint poself = parentNode->getPosition() ;
                
                parentNode->setAnchorPoint(ccp(0.5 ,0.0));
                parentNode->setPosition(ccpSub(poself, ccp(0.0f ,parentNode->getContentSize().height*UIManager::sharedManager()->getScaleFactor()/2.0)));
            }
 		    float oldScale = UIManager::sharedManager()->getScaleFactor();
            CCActionInterval *forwardTo1 = CCScaleTo::create(0.1, oldScale * 4.0f);
            
            CCActionInterval *forwardTo2 = CCScaleTo::create(0.05, oldScale * 2.0f);
            
            parentNodeEffect2 = parentNode ;
            CCAction *action = CCSequence::create(   forwardTo1,
                                                  forwardTo2,
                                                  CCCallFunc::create(this, callfunc_selector(AssistElfFightPanel::CallBackSkillEffect2)),
                                                  NULL);
            
            parentNode->runAction(action);
            
        }
	}

	
}