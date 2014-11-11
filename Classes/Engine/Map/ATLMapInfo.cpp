#include "ATLMapInfo.h"
#include "AspriteManager.h"

#define SPRITE_DIR   "mapBin/"

CATLMapInfo::CATLMapInfo()
	:m_width(0)
	,m_height(0)
	,m_tileWidth(0)
	,m_tileHeight(0)
	,m_pData(NULL)
	,m_pTexture(NULL)
	,m_pTileset(NULL)
{
}

CATLMapInfo::~CATLMapInfo()
{

}

void CATLMapInfo::Load(const char* pFileName)
{
	// load map data
	// structure: mapW mapH TilesetW TilesetH
	// spriteName.Size() spriteName
	// spriteTextureName.size() spriteTextureName
	// frameIndex00 frameIndex01 frameIndex 02 ...
	// frameIndex10 frameIndex11 ...
	// flag00 flag01 flagx 02 ...
	// flag10 flag11 ...
    
	unsigned long	_size;
	char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(pFileName , "rb", &_size);
	char* pBuffer = _pFileContent;

	int fileSize = 0;

	if(_pFileContent != NULL)
	{
        
		//m_width = *(int*)_pFileContent;
        memcpy(&m_width,_pFileContent,sizeof(m_width));

		_pFileContent += sizeof(int);
        
		//m_height = *(int*)_pFileContent;
        memcpy(&m_height,_pFileContent,sizeof(m_height));

        
		_pFileContent += sizeof(int);
        
		//m_tileWidth = *(int*)_pFileContent;
        memcpy(&m_tileWidth,_pFileContent,sizeof(m_tileWidth));

		_pFileContent += sizeof(int);
        
        
		//m_tileHeight = *(int*)_pFileContent;
        memcpy(&m_tileHeight,_pFileContent,sizeof(m_tileHeight));


		int size = 0;
		_pFileContent += sizeof(int);
        
        
		//size = *(int*)_pFileContent;
        memcpy(&size,_pFileContent,sizeof(size));

		_pFileContent += sizeof(int);
        

		char szPath[255];
		szPath[0] = '\0';
		memcpy(szPath,_pFileContent,size);		
		szPath[size] = '\0';
        
        

		m_tilesetName = szPath;
        

		_pFileContent += size;
        

		szPath[0] = '\0';

		size = 0;
		//size = *(int*)_pFileContent;
        memcpy(&size,_pFileContent,sizeof(size));

        
		_pFileContent += sizeof(int);
        

		memcpy(szPath,_pFileContent,size);
		szPath[size] = '\0';
		m_textureName = szPath;
        
		_pFileContent += size;
		m_frameIndex.resize(m_width * m_height);
		m_flag.resize(m_width * m_height);
		//fread(&m_frameIndex[0], m_frameIndex.size() * sizeof(short), 1, pFile);
		//fread(&m_flag[0], m_flag.size() * sizeof(int), 1, pFile);

		memcpy(&m_frameIndex[0],_pFileContent,m_frameIndex.size() * sizeof(short));
		_pFileContent += m_frameIndex.size() * sizeof(short);
		memcpy(&m_flag[0], _pFileContent,m_flag.size() * sizeof(int));						
	}

	CC_SAFE_DELETE_ARRAY(pBuffer);

}	

cocos2d::CCTexture2D* CATLMapInfo::GetTexture(void)
{
	if(m_pTexture == NULL)
	{
		m_pTexture = CCTextureCache::sharedTextureCache()->addImage((std::string(SPRITE_DIR)+ m_textureName).c_str());
	}
	return m_pTexture;
}

ASprite* CATLMapInfo::GetTileset(void)
{
	if(m_pTileset == NULL)
	{
		m_pTileset = AspriteManager::getInstance()->getAsprite(std::string(SPRITE_DIR) + m_tilesetName.c_str());
		
	}
	return m_pTileset;
}