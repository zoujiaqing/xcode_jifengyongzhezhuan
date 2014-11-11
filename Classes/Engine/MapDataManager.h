//
//  GameDataManager.h
//  iSeer
//
//  Created by razer tong on 11-12-6.
//  Copyright (c) 2011Äê __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_MapDataManager_h
#define iSeer_MapDataManager_h

#include <vector>
#include <map>
#include <string>
//#include <libxml/parser.h>
#include <libxml/tree.h>


struct STileLayerInfo
{
	std::string name;
	int zorder;
	bool isVisible;
};
#define STAR_PLIST "Data/Stars.plist"

class MapDataManager 
{
public:
	MapDataManager();
	~MapDataManager();
	static MapDataManager *sharedManager();
	static void Destroy();

	//Ñ°Â·µØÍ¼
	unsigned char* getMapFileByMapID(const int mapID) ;


private:
	static MapDataManager* s_pSharedManager;

	

	std::map<int, unsigned char*> m_physicsFileBuffer;

};

#endif
