#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"

#include "cocos2d.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "HelloWorldScene.h"
#include "GameManager.h"
#include "XLogicManager.h"
#include "UserData.h"
#include "GameResourceManager.h"
#include "Localization.h"

/// support lua inteface file
#include "LuaTXGUI.h"
#include "LuaNetwork.h"
#include "LuaLevelCreator.h"
#include "LuaGameAPI.h"
#include "LuaClient.h"
#include "LuaEngine.h"

#include "NetStateChecker.h"
#include "LuaTinkerManager.h"
#include "UserLoginStatistics.h"
#include "util.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <stdio.h>                                                                                           
#include <stdlib.h>           
#include <dirent.h>
#include <breakpad_startup.h>
#include "cocos-ext.h"  
#include "platform/android/jni/JniHelper.h"

#include "../android_third_pro/plugin/publish/protocols/include/PluginManager.h"
#include "../android_third_pro/plugin/publish/protocols/include/ProtocolAnalytics.h"
#include "TmLoginManager.h"

using namespace std;  
extern "C" const char* getPackageNameJNI();

extern "C" string g_apkPath;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <dirent.h>
#include "NcSDKManager.h"
#endif

extern "C" int luaopen_protobuf_c(lua_State *L);

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;



static bool bbackground = false;
static bool isAppForeground = true;/*if the app is on foreground*/

AppDelegate* AppDelegate::s_sharedAppDelegate = NULL;

AppDelegate::AppDelegate()
{
	m_bInit = false;
	s_sharedAppDelegate = this;
	m_nobackgroud = 1 ;

    m_client_version = "base";
	
}

AppDelegate::~AppDelegate()
{
    // end simple audio engine here, or it may crashed on win32
    SimpleAudioEngine::sharedEngine()->end(); 

	CCScriptEngineManager::purgeSharedManager();
	GameResourceManager::purgeInstance();
	XLogicManager::sharedManager()->removeAllChildrenWithCleanup(true);
	//XLogicManager::sharedManager()->pureXLogicManager();
	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();
	UserData::Destroy();
	
	#ifdef OUR_DEBUG
	check_leaks();
    #endif
}

AppDelegate* AppDelegate::GetShareAppDelegate()
{
	return s_sharedAppDelegate;
}

const char* AppDelegate::GetSvnVersion()
{
	const char *svn_version = "0";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	svn_version = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getsvn");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	svn_version = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getsvn");
#else

#endif

	return svn_version;
}

void AppDelegate::ClearIOSDocuments(std::string dirPath, bool flag)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    struct dirent *next_item;
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        CCLog("can't open dir: %s", dirPath.c_str());
        return;
    }

    while ( (next_item = readdir(dir)) )
    {
        std::string item(next_item->d_name);
        if (next_item->d_type == DT_REG)
        {
            CCLog("file: %s", item.c_str());
            CCString *filePath = CCString::createWithFormat("%s/%s", dirPath.c_str(), next_item->d_name);
            remove(filePath->getCString());
        }
        else if(next_item->d_type == DT_DIR && strcmp(next_item->d_name, ".") && strcmp(next_item->d_name, ".."))
        {
            CCLog("------------------- folder: %s", item.c_str());
            ClearIOSDocuments(dirPath + "/" + next_item->d_name, true);
        }
    }

    if(flag == true)
    {
        rmdir(dirPath.c_str());
    }
#endif
}

void AppDelegate::uploadCrashLog()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    struct stat st;
    const char *svn_version = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getsvn");


   // char datadir[100] = "/data/data/com.taomee.adventure/dump/";
    string datadir = CCFileUtils::sharedFileUtils()->getWriteablePath();
   datadir.append("dump/");
    datadir.append(svn_version);
    //strcat(datadir,svn_version);
    CCLOG("crash dir is %s", datadir.c_str());
   
    if(stat(datadir.c_str(),&st) == 0)
    {
        
        std::map<string, string> params;
        std::map<string, string>::iterator it;
        std::vector<std::string> headers;
        CCHttpRequest* request;
        DIR *dir;
        struct dirent *ent;
        std::vector<char *> dump_file_names;
        

        if ((dir = opendir (datadir.c_str())) != NULL) {
          /* print all the files and directories within directory */
          while ((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name, ".") == 0 ||
                strcmp(ent->d_name, "..") == 0)
                continue;
            printf ("%s\n", ent->d_name);
            dump_file_names.push_back(ent->d_name);
          }
          closedir (dir);
        } else {
          /* could not open directory */
          //perror ("");
          //return EXIT_FAILURE;
          CCLOG("failed to open file dir %s", datadir.c_str());
          return;
        }
        if(dump_file_names.size() == 0) {
            CCLOG("dir %s has no file", datadir.c_str());
            return;
        }
        request = new CCHttpRequest();  
        request->setUrl("http://wlad.61.com/crashit/upload.php");  
       // request->setUrl("http://10.1.1.101:8964/iapAdServer/crashit/trunk/index.php");  
        request->setRequestType(CCHttpRequest::kHttpPost);  
        request->setResponseCallback((CCObject*)this, callfuncND_selector(AppDelegate::onGetFinished));  
       
        headers.push_back("Content-Type: multipart/form-data;  boundary=0xKhTmLbOuNdArYcC"); 
        request->setHeaders(headers);
        // write the post data  
       
        string postData;
        postData.reserve(1.5*1024*1024);
        params["c"]= "app";
		params["a"]= "upload_dumpfile";
		params["app"]= "android_ahero";
		params["version"]=svn_version;
        const char * boundary = "0xKhTmLbOuNdArYcC";
        for(it = params.begin(); it != params.end(); it++)
        {
            postData.append("--");
            postData.append(boundary);
            postData.append("\r\n");
            postData.append("Content-Disposition: form-data; name=\""+ it->first + "\"\r\n\r\n");
            postData.append(it->second);
            postData.append("\r\n");
        }
        postData.append("--" + string(boundary) + "\r\n");
        postData.append("Content-Disposition: form-data; name=\"");
        postData.append("file\"; filename=\"crash.dmp\"\r\n");
        postData.append("Content-Type: application/octet-stream\r\n\r\n");

        if(dump_file_names.size() > 0) {
            string dumpfile = (datadir) + string("/")+dump_file_names[0];
            CCLOG("read data from file %s", dumpfile.c_str());
            FILE *fd = fopen(dumpfile.c_str(), "rb");
            if(fd == NULL){
                CCLOG("failed to open %s", dumpfile.c_str());
                return;
            }
            size_t size =0;
            CCLOG("crash file nuber is %d", dump_file_names.size());
            fseek(fd, 0, SEEK_END);
            size = ftell(fd);
            fseek(fd, 0, SEEK_SET);
            unsigned char* out = new unsigned char[size];
            memset(out, 0, size);
            size_t read = fread(out, 1, size, fd);
            
            postData.append((char const*)out, size);
            delete(out);
            
            if( remove( dumpfile.c_str()) != 0 )
            {
                cocos2d::CCLog( "Error deleting file" );
            } 
            else
            {
                cocos2d::CCLog( "File successfully deleted" );

            }
            if(dump_file_names.size() > 1 ){
                int i;
                CCLOG("this should not happen");
                int len = dump_file_names.size();
                for(i = 1; i<len; i++){
                    string dumpfile = (datadir) + string("/")+dump_file_names[i];
                    CCLOG("remove file %s", dumpfile.c_str());
                    remove( dumpfile.c_str());
                } 
             }
            rmdir(datadir.c_str());
            
        }    
       // const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";  
        postData.append("\r\n--" + string(boundary) + "--\r\n");

        request->setRequestData(postData.c_str(), postData.length());   
        if(0){
            FILE *fd2 = fopen("/sdcard/mylog.txt", "wb");
            if(fd2 != NULL){
                fwrite(postData.c_str(), 1,  postData.length(), fd2);
                fclose(fd2);
            } else {
                CCLOG("failed to open file /sdcard/mylog.txt");
            }
        }
        request->setTag("crash log");  
        CCHttpClient::getInstance()->send(request);  
        request->release();  
       
   } else {
       CCLOG("dir is not exist");
   }
#endif
}


void AppDelegate::onGetFinished(CCNode *node,void *data)
{
	CCHttpResponse *response = (CCHttpResponse*)data;
	 if (!response)
    {
        return;
    }
	 int s=response->getHttpRequest()->getRequestType();
	 CCLOG("request type %d",s);


	 if (0 != strlen(response->getHttpRequest()->getTag())) 
    {
        CCLOG("%s ------>oked", response->getHttpRequest()->getTag());
    }

	 int statusCode = response->getResponseCode();
	  CCLOG("response code: %d", statusCode);


    char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	CCLog(statusString);

	 if (!response->isSucceed()) 
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }

	std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        CCLOG("%c", (*buffer)[i]);
		
    }
    printf("\n");

}
void AppDelegate::InitOnGameStart()
{
	if(false == m_bInit)
	{
		m_bInit = true;
	}
	else
	{
		return ;
	}

	// register lua engine and init game lua interface!
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);

	tolua_TXGUI_open(pEngine->getLuaState());
	tolua_Network_open(pEngine->getLuaState());
	tolua_LevelCreator_open(pEngine->getLuaState());
	tolua_GameApi_open(pEngine->getLuaState());
	tolua_LuaClient_open(pEngine->getLuaState());
	tolua_LuaEngine_open(pEngine->getLuaState());
	luaopen_protobuf_c(pEngine->getLuaState());

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Script");
	string scriptPath = path.substr(0, path.find_last_of("S"));
	pEngine->addSearchPath(scriptPath.c_str());
#else
	std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Script/main.lua");
	std::string scriptpath = path.substr(0, path.find_last_of("S"));
	pEngine->addSearchPath(scriptpath.c_str());

	// test network
	UserData::Create();
	USERINFO mUserData = UserData::GetUserInfo();
	uint32_t id = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugUserId");
	uint32_t channel = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfig/DebugConfig.lua","GetDebugChannel");
	mUserData.id.setPlayId(id,0,channel);
	string session = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfig/DebugConfig.lua","GetDebugUserSession");
	strcpy(mUserData.userSession,session.c_str());
	UserData::SetUserInfo(mUserData);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string pbfiles[]=
	{"head.bin","cs_basic.bin", "cs_core.bin", "db.bin", "errcode.bin", "cs_battle.bin", "cs_config.bin"};
	unsigned long buffsize;

	lua_State* L_state = CCLuaEngine::defaultEngine()->getLuaState();
	//this failed on android. cause lua io library can not read file from apk  
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/handleMessage.lua", "registerAllfile");

	for(int i = 0; i < 7; i++)
	{
		const char* fullFilePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath((string("Script/Pbc/")+pbfiles[i]).c_str());
		const char * buff = (const char * )CCFileUtils::
			sharedFileUtils()->getFileData((fullFilePath), 
			"r",&buffsize);
		CCAssert(buff!=NULL, "faile to read file");
		lua_getglobal(L_state,"registerfile");
		lua_pushlstring(L_state, (const char *)(buff ), buffsize);
		lua_pcall(L_state,1,0,0);
		//LuaTinkerManager::Get()->callLuaFunc<bool>("Script/handleMessage.lua", "registerfile", buff);
		CC_SAFE_DELETE_ARRAY(buff);
	}
    
	//add crash log config to crash module
	char *svn_version = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getsvn");
    if(svn_version == NULL)
        svn_version="1111";
    //upload crash log if exist
    uploadCrashLog();
    //check if file exist
   // string datadir = CCFileUtils::sharedFileUtils()->getWriteablePath();
   {
    const char *datadir = getPackageNameJNI();
    breakpadStartup(datadir, svn_version);

    }
#else
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/handleMessage.lua", "registerAllfile");
#endif
	LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameUILayer.lua", "InitGameUILayer");
}

void AppDelegate::FlurryChannelInfo()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("FLURRY_CHANNEL_INFO" ))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("FLURRY_CHANNEL_INFO", true );
		CCUserDefault::sharedUserDefault()->flush();

		cocos2d::plugin::ProtocolAnalytics* pFlurry = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsFlurry"));
		if(pFlurry)
		{
			pFlurry->startSession("TMP7KSFBBQ2YTJYFR4SG");
#ifdef _DEBUG
			pFlurry->setDebugMode(true);
#endif
			std::string str_channelID = LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/GameConfigParse.lua","GetChannelID");

			// log an event with params
			cocos2d::plugin::LogEventParamMap paramMap;
			paramMap.insert(cocos2d::plugin::LogEventParamPair("Game Channel ID", str_channelID.c_str()));
			pFlurry->logEvent("Channel info", &paramMap);

			cocos2d::plugin::LogEventParamMap paramMap2;
			paramMap.insert(cocos2d::plugin::LogEventParamPair("Flurry_AndroidID", str_channelID.c_str()));
			pFlurry->logEvent("Flurry_AndroidID", &paramMap2);

			pFlurry->stopSession();
			//cocos2d::plugin::PluginParam mapValue(paramMap);
			//pFlurry->callFuncWithParam("logEvent", &mapValue, NULL);
		}
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::trackFlurryEvent(kFlurryStatisticEnterGame);
    NcSDKManager::trackCocoDataEvent(kCocoDataStatisticEnterGame);
    //NcSDKManager::trackHailStoneEvent(kHailStoneStatisticEnterGame);
#endif

}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
	m_nobackgroud = 1 ;
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // first delete ios Documents folder when condition is valid, added by seanxiao at 2014/7/23
    std::string ios_bundle_version = CCUserDefault::sharedUserDefault()->getStringForKey("ios_bundle_version");
    std::string cur_version = NcSDKManager::getBundleVersion();
    if(std::strcmp(ios_bundle_version.c_str(), cur_version.c_str()) != 0)
    {
        std::string wPath = CCFileUtils::sharedFileUtils()->getWriteablePath();
        CCUserDefault::purgeSharedUserDefault();
        this->ClearIOSDocuments(wPath, false);
        CCUserDefault::sharedUserDefault()->setStringForKey("ios_bundle_version", cur_version);
    }
#endif

	 GameResourceManager::sharedManager()->initSearchPath();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	//std::vector<std::string> vecSearchPaths;

	//std::string resourceUpdatePath = GameResourceManager::sharedManager()->writablePathFromRelativePath(".");
	//vecSearchPaths.push_back(resourceUpdatePath);
	//std::string apkAssetsPath = ".";
	//vecSearchPaths.push_back(apkAssetsPath);

	//// add more ...

	//// set searching path 
	//CCFileUtils::sharedFileUtils()->setSearchPaths(vecSearchPaths);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	//string path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("Script/main.lua");
	//string scriptPath = path.substr(0, path.find_last_of("S"));
	//pEngine->addSearchPath(scriptPath.c_str());
#else
	// set resource folder
	std::string exeWin32Path = CCFileUtils::sharedFileUtils()->getWriteablePath();
	exeWin32Path = exeWin32Path + "..\\..\\Resources";
	this->setResourceRootPath(exeWin32Path);
#endif
	//UserLoginStatistics::sendStatInfoToServer("launch_game");
	XLogicManager::sharedManager();
	//yi bu download config file
	NetWorkConfigInfoFromServer::sharedInstance()->downloadConfigFileList();

#if 0
	//move this to xlogicmanager
	NetWorkConfigInfoFromServer::sharedInstance()->downloadConfigFileList();
	NetWorkConfigInfoFromServer::sharedInstance()->parseRcvMessageFromServer();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    XLogicManager::sharedManager()->sendStatAtLaunch();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		XLogicManager::sharedManager()->goToLayer(XLogicManager::E_UPDATE_RESOURCE, NULL);
#else
		XLogicManager::sharedManager()->goToLayer(XLogicManager::E_SELECT_ZONE, NULL);
#endif	
#endif
	FlurryChannelInfo();

    // 读取目前版本
    m_client_version = std::string(LuaTinkerManager::Get()->callLuaFunc<const char *>("Script/client_version.lua", "getClientVersion"));

    if (m_client_version == "")
    {
        m_client_version = "base";
    }

    return true;
}
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{

    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    bbackground = true ;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
    if(!isAppOnForeground()){//app进入后台
		isAppForeground = false;
	}
	 PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
	if(channel == Wandoujia || channel == Huawei || channel == Oppo) //nd91 use dopause do things which doruse do
    {
	    TmLoginManager::tmLoginManager()->doPause();
    }
#endif
    //NetStateChecker::getInstance()->QuickSendRqsToServer();
    
	static bool bInit = false;
	cocos2d::CCLog("%s %d ,%d\n", __FUNCTION__, __LINE__ ,bInit);
	if (bInit)
	{
		/// skip the start up the application
		XLogicManager::sharedManager()->DidEnterBackground(); 
	}
	bInit = true;

	CCDirector::sharedDirector()->pause();
}

void AppDelegate::applicationWindowFocusChanged( unsigned char HasFocus)
{
	static unsigned char sFocus = 0 ;
	cocos2d::CCLog("%s %d ,[%d ,%d]\n", __FUNCTION__, __LINE__ ,sFocus ,HasFocus);
	if( HasFocus)
	{
		if(bbackground == true)
		{
			bbackground = false ;
			cocos2d::CCLog("%s %d ,[%d ,%d]\n", __FUNCTION__, __LINE__ ,sFocus ,HasFocus);

			CCDirector::sharedDirector()->resume();
		}
		CCDirector::sharedDirector()->startAnimation();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();

		static bool bInit = false;

		if (bInit)
		{
			/// skip the start up the application
			XLogicManager::sharedManager()->WillEnterForeground();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
			if(TmLoginManager::isInited()) {
				cocos2d::CCLog("%s %d \n", __FUNCTION__, __LINE__ );
				PlantformId channel = (PlantformId)LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");

				if(!isAppForeground){//从后台切到前台，打开91SDK暂停页
					 //91 is shit. add it's pause ui here

					 if(channel == Nd91) 
					 { 
						 TmLoginManager::tmLoginManager()->doPause();
					 } 
					
					 isAppForeground = true;
				}
				if(channel == JinLi
				||channel == Pptv
				||channel == Wandoujia
				||channel == Bubugao
				||channel == Duoku
				||channel == MuZhiWan
				||channel == YingYongHui)
				{
				 	cocos2d::CCLog("%s %d \n", __FUNCTION__, __LINE__ );
					if(TmLoginManager::tmLoginManager()->getUserId()=="")
					{
						cocos2d::CCLog("%s %d \n", __FUNCTION__, __LINE__ );
						TmLoginManager::tmLoginManager()->doLogin();
			
					}
				}

               // if (channel == Wandoujia || channel == Huawei || channel == Oppo)
                {
                    TmLoginManager::tmLoginManager()->onResume();
                }
			}
#endif		

		}
		bInit = true;

		
	  
	}
	else
	{
	
		cocos2d::CCLog("%s %d ,[%d ,%d]\n", __FUNCTION__, __LINE__ ,sFocus ,HasFocus);
		applicationDidEnterBackground();

	}
	sFocus = HasFocus ;
	m_nobackgroud = HasFocus ;
	
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    cocos2d::CCLog("%s %d\n", __FUNCTION__, __LINE__);

     bbackground = false ;
     CCDirector::sharedDirector()->resume();
	


  #if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS
	 applicationWindowFocusChanged(1);
  #else

    if(m_nobackgroud)
	{
	    applicationWindowFocusChanged(1);
	}
  #endif 
	
}



