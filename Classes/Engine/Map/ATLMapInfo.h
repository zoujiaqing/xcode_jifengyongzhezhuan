#ifndef ATLMAPINFO_2012_12_26_H
#define ATLMAPINFO_2012_12_26_H


#include <vector>
#include "../../../cocos2dx/cocos2dx/textures/CCTexture2D.h"
#include "../../../libs/ASprite/ASprite.h"

class CATLMapInfo
{
	int m_width;
	int m_height;
	short* m_pData;
	
	int m_tileWidth;
	int m_tileHeight;

	std::vector<short> m_frameIndex;
	std::vector<int>	m_flag;

	std::string m_textureName;
	std::string m_tilesetName;

	cocos2d::CCTexture2D* m_pTexture;
	ASprite* m_pTileset;

public:
	CATLMapInfo();
	~CATLMapInfo();

	void Load(const char* pFileName);

	short GetFrameIndex(int x, int y){return m_frameIndex[x + y * m_width];}
	int GetFlag(int x, int y){return m_flag[x + y * m_width];}
	int GetHeight(void){return m_height;}
	int GetWidth(void){return m_width;}
	int GetTileWidth(void){return m_tileWidth;}
	int GetTileHeight(void){return m_tileHeight;}

	cocos2d::CCTexture2D* GetTexture(void);
	ASprite* GetTileset(void);

};

#endif