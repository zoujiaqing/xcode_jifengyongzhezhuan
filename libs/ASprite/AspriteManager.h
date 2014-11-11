//
//  AspriteManager.h
//
//  Created by arthurgong on 11-11-29.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef _ASPRITE_MANAGER_
#define _ASPRITE_MANAGER_

#include <map>
#include "ASprite.h"

using namespace std;

typedef map<string, ASprite*> MAP_ASPRITE;
typedef map<string, ASprite*>::iterator MAP_ASPRITE_ITER;

class AspriteManager
{
private:
    MAP_ASPRITE m_asMap;
    bool m_init;
public:
    AspriteManager();
    ~AspriteManager();
    
    static AspriteManager* getInstance();
    static void purgeInstance();    

    ASprite *getAsprite(string key);
    void loadAllAsprite();
	void OnlyLoadLoingAsprite();
    void OnlyLoadNoticeAsprite();

	/**
	* Instruction : 获取某一帧Frame
	* @param 
	*/	
	CCSprite* getOneFrame(std::string key,std::string frameName);
    
	/**
	* Instruction : 获取Frame之间的相对位置
	* @param parentFrame 父节点名字
	* @param childFrame 子节点名字
	* @param childFrameAnchorPoint 子节点锚点
	* @result pos 相对位置
	* @result CCSprite* 子节点
	*/
	CCSprite* getFramesRelativePos(std::string key,std::string parentFrame,std::string childFrame,
		CCPoint childFrameAnchorPoint,CCPoint &pos);
private:
    void clearAllAsprite();
    void addAsprite(string key, ASprite * asprite);    

	bool m_bLoadUILoingBinFile;
    bool m_bLoadUINoticeBinFile;
	
};
#endif //_ASPRITE_MANAGER_