//
//  AspriteManager.cpp
//
//  Created by arthurgong on 11-11-29.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//

#include "AspriteManager.h"
#include "TXGUI.h"
#include "LuaTinkerManager.h"

static AspriteManager* _iserInstance = null;

AspriteManager* AspriteManager::getInstance()
{
    if(_iserInstance == null)
    {
        _iserInstance = new AspriteManager();
    }
    return _iserInstance;
}

void AspriteManager::purgeInstance()
{
    SAFE_DEL(_iserInstance);
}

AspriteManager::AspriteManager()
{
    m_init = false;
	m_bLoadUILoingBinFile = false;
    m_bLoadUINoticeBinFile = false;
}

AspriteManager::~AspriteManager()
{
    clearAllAsprite();
}

void AspriteManager::OnlyLoadLoingAsprite()
{
	ASprite* asprite = new ASprite();
	char binPath[255] = {'\0'};
	char pngPath[255] = {'\0'};
	const char* szBinPath = "UI/ui_loading.bin";
	const char* szPngPath = "UI/ui_loading.png";
	strcat(binPath, szBinPath);
	strcat(pngPath, szPngPath);
	asprite->loadFile(binPath, pngPath);
	addAsprite(szBinPath, asprite);

	m_bLoadUILoingBinFile = true;
}

void AspriteManager::OnlyLoadNoticeAsprite()
{
    if (getAsprite("UI/ui.bin") != NULL)
    {
        return;
    }

    ASprite* asprite = new ASprite();
	char binPath[255] = {'\0'};
	char pngPath[255] = {'\0'};
	const char* szBinPath = "UI/ui.bin";
	const char* szPngPath = "UI/ui.png";
	strcat(binPath, szBinPath);
	strcat(pngPath, szPngPath);
	asprite->loadFile(binPath, pngPath);
	addAsprite(szBinPath, asprite);
    
	m_bLoadUINoticeBinFile = true;
}


void AspriteManager::loadAllAsprite()
{    
	if(!m_init)
	{
		int spriteNum = LuaTinkerManager::Get()->callLuaFunc<int>("Script/spriteDataConfig.lua", "GetAspriteNum");
		for(int i = 0; i < spriteNum; i++)
		{
			ASprite* asprite = new ASprite();
			char binPath[255] = {'\0'};
			char pngPath[255] = {'\0'};
			const char* szBinPath = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/spriteDataConfig.lua", "GetASpriteData",i*2+1);
			const char* szPngPath = LuaTinkerManager::Get()->callLuaFunc<const char*>("Script/spriteDataConfig.lua", "GetASpriteData",i*2+2);
			
            if (false == m_bLoadUILoingBinFile || strcmp(szBinPath,"UI/ui_loading.bin") != 0)
			{
                if(m_bLoadUINoticeBinFile && strcmp(szBinPath, "UI/ui.bin") == 0)
                {
                    continue;
                }
                
				strcat(binPath, szBinPath);
				strcat(pngPath, szPngPath);
				asprite->loadFile(binPath, pngPath);
				addAsprite(szBinPath, asprite);
			}
		}
		ASprite* iconSprite = getAsprite(KICON_BIN);
		if(iconSprite)
		{
			iconSprite->setImageName(0,"UI/ui_system_icon.png");
			iconSprite->setImageName(1,"UI/ui_system_icon2.png");
		}
		m_init = true;
	}
}

void AspriteManager::addAsprite(string key, ASprite * asprite)
{
    m_asMap.insert( map<string, ASprite*>::value_type(key, asprite) );
}

ASprite* AspriteManager::getAsprite(string key)
{      
    MAP_ASPRITE_ITER iter = m_asMap.find(key);
    
	if(iter != m_asMap.end())
		return iter->second;

    return NULL;
}

void AspriteManager::clearAllAsprite()
{
    MAP_ASPRITE_ITER iter = m_asMap.begin();   
    for (; iter != m_asMap.end(); iter++) {
        SAFE_DEL(iter->second);
    }

    m_asMap.clear();
	m_init = false;
}

CCSprite* AspriteManager::getOneFrame(std::string key,std::string frameName)
{
	int iSpriteFrame = TXGUI::getResourceIDByName(frameName.c_str());

	ASprite *as = AspriteManager::getInstance()->getAsprite(key); 
	if (as == 0)
	{
		return 0;
	}
	CCPoint pos;
	CCSprite * pSprite = as->getSpriteFromFrame_Middle(iSpriteFrame, 0,pos);
	return pSprite;
}

CCSprite* AspriteManager::getFramesRelativePos(std::string key,std::string parentFrame,std::string childFrame,
	CCPoint childFrameAnchorPoint,CCPoint &pos)
{
	int iParentSpriteFrame = TXGUI::getResourceIDByName(parentFrame.c_str());
	int iChildSpriteFrame = TXGUI::getResourceIDByName(childFrame.c_str());

	ASprite *as = AspriteManager::getInstance()->getAsprite(key); 
	if (as == 0)
	{
		return 0;
	}
	CCPoint parent_pos;
	CCPoint child_pos;
	CCSprite * pParentSprite = as->getSpriteFromFrame_Middle(iParentSpriteFrame, 0,parent_pos);
	if (pParentSprite == 0)
	{
		return 0;
	}
	CCSprite * pChildSprite = as->getSpriteFromFrame_Middle(iChildSpriteFrame, 0,child_pos);
	if (pChildSprite == 0)
	{
		return 0;
	}
	pos = ccpSub(child_pos,parent_pos);
	CCPoint offsetAnchorPoint = ccpSub(childFrameAnchorPoint,ccp(0.5,0.5));
	CCSize size = pChildSprite->getContentSize();
	pos.x += size.width*offsetAnchorPoint.x;
	pos.y += size.height*offsetAnchorPoint.y;

	return pChildSprite;
}