#include "LuaTinkerManager.h"
#include "CCLuaEngine.h"
#include "GameResourceManager.h"


LuaTinkerManager::LuaTinkerManager()
{
	curLuaState = CCLuaEngine::defaultEngine()->getLuaState();
}

LuaTinkerManager::~LuaTinkerManager()
{
	loadedFiles.clear();
}

bool LuaTinkerManager::ForceCheckLoadFile(string filePath)
{
	bool bRet = false;

	const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filePath.c_str());

	unsigned long  fileSize = 0;
	unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
	int ret = luaL_loadbuffer(this->curLuaState, (char*)buffer,fileSize,filePath.c_str())||lua_pcall(this->curLuaState,0,LUA_MULTRET,0);
	delete(buffer);
	if (ret != 0) 
	{
		CCLOG("load %s file error: %s", filePath.c_str(), lua_tostring(this->curLuaState, -1));
		lua_pop(this->curLuaState, 1);
	}
	else
	{
		bRet = true;
	}

	return bRet;
}

bool LuaTinkerManager::checkAnyLoadFile(string filePath)
{
	
	if (loadedFiles.find(filePath) == loadedFiles.end())
	{

		const char *path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(filePath.c_str());
      
		unsigned long  fileSize = 0;
		unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData(path, "rb", &fileSize);
        //int ret = luaL_dostringEx(this->curLuaState, (char*)buffer, fileSize);
        int ret = luaL_loadbuffer(this->curLuaState, (char*)buffer,fileSize,filePath.c_str())||lua_pcall(this->curLuaState,0,LUA_MULTRET,0);
        delete(buffer);
        if (ret != 0) {

			CCLOG("load %s file error: %s", filePath.c_str(), lua_tostring(this->curLuaState, -1));
            
            lua_pop(this->curLuaState, 1);
			return false;
		}
        

        loadedFiles.insert(filePath);
		return true;
	}

	return true;
}
