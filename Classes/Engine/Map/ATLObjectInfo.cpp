#include "ATLObjectInfo.h"
#include "AspriteManager.h"
#include "LevelLayer.h"

#define SPRITE_DIR   "mapBin/"



//void CATLObjectInfo::CAMERA::Read( FILE* bin )
//{
//	fread(&flags, sizeof(flags), 1, bin);
//	fread(&pos, sizeof(pos), 1, bin);
//	fread(&size, sizeof(size), 1, bin);
//}

void CATLObjectInfo::CAMERA::Read( char* &_pContent )
{
	memcpy(&flags,_pContent,sizeof(flags));
	_pContent += sizeof(flags);
	memcpy(&pos,_pContent,sizeof(pos));
	_pContent += sizeof(pos);	
	memcpy(&size,_pContent,sizeof(size));
	_pContent += sizeof(size);
}


void CATLObjectInfo::LEVEL::Read( char* &_pContent )
{
	int len = 0;
	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(len);
	char szText[256] = {'\0'};
	memcpy(szText,_pContent,len);
	_pContent += len;
	name = szText;
	memcpy(&flags,_pContent,sizeof(flags));
	_pContent += sizeof(flags);
	memcpy(&id,_pContent,sizeof(id));
	_pContent += sizeof(id);
	memcpy(&pos,_pContent,sizeof(pos));
	_pContent += sizeof(pos);

	int count = 0;
	memcpy(&count,_pContent,sizeof(count));
	_pContent += sizeof(int);
	for(int i = 0; i < count; i++)
	{

		LAYER::TYPE type = LAYER::E_DUMMY;
		memcpy(&type,_pContent,sizeof(type));
		_pContent += sizeof(type);

		LAYER::BASE* pLayer = NULL;
		switch(type)
		{
		case LAYER::E_TILED:
			pLayer = new LAYER::TILED();
			break;
		case LAYER::E_OBJECT:
			pLayer = new LAYER::OBJECT();
			break;
		case LAYER::E_DUMMY:
			pLayer = new LAYER::DUMMY();
			break;
			default:
				break;
		}

		layers.push_back(pLayer);
		pLayer->Read(_pContent);
	}
}



void CATLObjectInfo::LAYER::BASE::Read( char* &_pContent )
{
	memcpy(&flags,_pContent,sizeof(flags));
	_pContent += sizeof(flags);

	memcpy(&id,_pContent,sizeof(id));
	_pContent += sizeof(int);
	
	memcpy(&pos,_pContent,sizeof(pos));
	_pContent += sizeof(pos);

	int count = 0;
	memcpy(&count,_pContent,sizeof(count));
	_pContent += sizeof(count);

	for(int i = 0; i < count; i++)
	{
		if(count == 25)
		{
			int a;
			a = 10;
		}
		LAYER::TYPE type = LAYER::E_DUMMY;

		memcpy(&type,_pContent,sizeof(type));
		_pContent += sizeof(type);

		LAYER::BASE* pLayer = NULL;
		switch(type)
		{
		case LAYER::E_TILED:
			pLayer = new LAYER::TILED();
			break;
		case LAYER::E_OBJECT:
			pLayer = new LAYER::OBJECT();
			break;
		case LAYER::E_DUMMY:
			pLayer = new LAYER::DUMMY();
			break;
		default:
			break;
		}

		childrens.push_back(pLayer);

		pLayer->Read(_pContent);

	}
}



void CATLObjectInfo::LAYER::TILED::Read( char* &_pContent )
{
	BASE::Read(_pContent);
	int len = 0;

	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(len);

	char szText[255] = {'\0'};
	memcpy(szText,_pContent,len);
	_pContent += len;
	templateFile = szText;

	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(len);

	memset(szText, 0x00, sizeof(szText));
	memcpy(szText,_pContent,len);
	_pContent += len;

	mapFile = szText;
}



void CATLObjectInfo::LAYER::OBJECT::Read( char* &_pContent )
{
	BASE::Read(_pContent);

	char szText[255] = {'\0'};
	int len = 0;
	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(int);
	memcpy(szText,_pContent,len);
	_pContent += len;

	templateName = szText;
	memcpy(&enable,_pContent,sizeof(enable));
	_pContent += sizeof(enable);

	memcpy(&visible,_pContent,sizeof(visible));
	_pContent += sizeof(visible);

	len = 0;
	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(len);
	memset(szText, 0x00, sizeof(szText));
	memcpy(szText,_pContent,len);
	_pContent += len;
	layerName = szText;
	len = 0;
	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(int);
	memset(szText, 0x00, sizeof(szText));
	memcpy(szText,_pContent,len);
	_pContent += len;
	tagName = szText;
	len = 0;
	memcpy(&len,_pContent,sizeof(len));
	_pContent += sizeof(len);
	memset(szText, 0x00, sizeof(szText));
	memcpy(szText,_pContent,len);
	_pContent += len;
	spriteFile = szText;

	memcpy(&renderFlag,_pContent,sizeof(renderFlag));
	_pContent += sizeof(renderFlag);
	memcpy(&animID,_pContent,sizeof(animID));
	_pContent += sizeof(animID);

	zOrder = MAP_Z_OBJECT;

	if(layerName == "BG_1")
	{
		zOrder = MAP_Z_BG1;
	}
	else if(layerName == "BG_2")
	{
		zOrder = MAP_Z_BG2;
	}
	else if(layerName == "BG_3")
	{
		zOrder = MAP_Z_BG3;
	}
	else if(layerName == "BG_3")
	{
		zOrder = MAP_Z_BG3;
	}
	else if(layerName == "BG_4")
	{
		zOrder = MAP_Z_BG4;
	}
	else if(layerName == "BG_5")
	{
		zOrder = MAP_Z_BG5;
	}
	else if(layerName == "BG_6")
	{
		zOrder = MAP_Z_BG6;
	}
	else if(layerName == "BG_7")
	{
		zOrder = MAP_Z_BG7;
	}
	else if(layerName == "BG_8")
	{
		zOrder = MAP_Z_BG8;
	}
	else if(layerName == "BG_9")
	{
		zOrder = MAP_Z_BG9;
	}
	else if(layerName == "LIGHT")
	{
		zOrder = MAP_Z_LIGHT;
	}
	else if(layerName == "OBJ_1")
	{
		zOrder = MAP_Z_OBJ_1;

	}
	else if(layerName == "OBJ_2")
	{
		zOrder =  MAP_Z_OBJ_2;
	}
	else if(layerName == "OBJ_3")
	{
		zOrder = MAP_Z_OBJ_3;
	}
	else if(layerName == "OBJ_4")
	{
		zOrder = MAP_Z_OBJ_4;

	}
	else if(layerName == "OBJ_5")
	{
		zOrder = MAP_Z_OBJ_5;

	}
	else if(layerName == "OBJ_6")
	{
		zOrder = MAP_Z_OBJ_6;

	}
	else if(layerName == "OBJ_7")
	{
		zOrder = MAP_Z_OBJ_7;
	}
	else if(layerName == "OBJ_8")
	{
		zOrder = MAP_Z_OBJ_8;

	}
	else if(layerName == "OBJ_TOP")
	{
		zOrder = MAP_Z_OBJ_TOP;
	}
	else if(layerName == "OBJ_SHADOW")
	{
		zOrder = MAP_Z_OBJ_SHADOW;
	}
}


// ------------------------------------------------------------------------------------------------------

CATLObjectInfo::CATLObjectInfo()
{
}

CATLObjectInfo::~CATLObjectInfo()
{
	for(size_t i = 0; i < m_levels.size(); i++)
	{
		delete m_levels[i];
	}
}

void CATLObjectInfo::Load(const char* pFileName)
{
	unsigned long	_size;
	char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(pFileName , "rb", &_size);
	char* pBuffer = _pFileContent;

	if (NULL != _pFileContent)
	{
		CAMERA camera;
		camera.Read(_pFileContent);

		int count = 0;
		count = *(int*)_pFileContent;
		_pFileContent += sizeof(int);
		for(int i = 0; i < count; i++)
		{
			LEVEL* pLevel = new LEVEL();
			m_levels.push_back(pLevel);
			pLevel->Read(_pFileContent);
		}
	}

	CC_SAFE_DELETE_ARRAY(pBuffer);
}	
