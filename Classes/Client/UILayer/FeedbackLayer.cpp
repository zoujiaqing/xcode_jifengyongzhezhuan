
#include "FeedbackLayer.h"
#include "AspriteManager.h"
#include "TXGUIHeader.h"

#include "GameFontManager.h"

//#include "ResoureUpdate.h"
#include "GameConfigFromLuaManager.h"
#include "MainMenuLayer.h"
#include "GameManager.h"
#include "StoryInstanceDirector.h"
#include "MessageBoxLayer.h"
#include "Localization.h"
#include "GameAudioManager.h"
#include "LuaTinkerManager.h"
#include "UserData.h"
#include "TutorialsManager.h"
#include "LevelLayer.h"
#include "SceneLayer.h"
#include "UIManager.h"
#include "LuaTinkerManager.h"
#include "util.h"


#if  CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "taomeeIosUtil.h"
#endif
#if  CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TmLoginManager.h"
#endif

static const char *g_FeedBackName="feedbacklayout";


FeedbackLayer::FeedbackLayer()	
{
	layout = NULL;
}

FeedbackLayer::~FeedbackLayer()
{
	
}

bool FeedbackLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

    createLayout();

   // MainMenuLayer::PushOneLayer(this);
	
	return true;
}

void FeedbackLayer::createLayout()
{
    UIManager *manager = UIManager::sharedManager();
	layout = manager->CreateUILayoutFromFile("UIplist/FeedbackUI.plist", this, g_FeedBackName);	


	manager->RegisterMenuHandler(g_FeedBackName,"callbackClose",
	menu_selector(FeedbackLayer::callbackClose), this);	

// 
// 	manager->RegisterMenuHandler(g_FeedBackName,"callbackHotSpot",
// 	    menu_selector(FeedbackLayer::callbackHotSpot), this);
//     
//     manager->RegisterMenuHandler(g_FeedBackName,"callbackOnline",
// 	    menu_selector(FeedbackLayer::callbackOnline), this);
//     
//     manager->RegisterMenuHandler(g_FeedBackName,"callbackForum",
//         menu_selector(FeedbackLayer::callbackForum), this);
//     
//     manager->RegisterMenuHandler(g_FeedBackName,"callbackReply",
// 		menu_selector(FeedbackLayer::callbackReply), this);

    
   // layout->FindChildObjectByName<UIButton>("Online")->setEnabled(false);
   // layout->FindChildObjectByName<UIButton>("Forum")->setEnabled(false);
   // layout->FindChildObjectByName<UIButton>("Reply")->setEnabled(false);
}
void FeedbackLayer::callPlatformFunc(int funid){
	
   	USERINFO info = UserData::GetUserInfo();
    int uid = info.id.getID();
	
    int channelId = LuaTinkerManager::Get()->callLuaFunc<unsigned int>("Script/GameConfigParse.lua","GetChannelID");
	int serverID = info.serverId;
	char *caractorName = info.szName;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if(funid == 2)
    {
        TmLoginManager::tmLoginManager()->doBBS();
    }
    else
    {
    	CCLOG("%s %d", __FUNCTION__, __LINE__);
       JniMethodInfo t;
       if (JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", 
	   	"feedback", "(IIIILjava/lang/String;)V")) {
		   CCLOG("%s %d", __FUNCTION__, __LINE__);
			jstring jname = t.env->NewStringUTF(caractorName);
		
            t.env->CallStaticVoidMethod(t.classID, t.methodID, 
				funid, uid,channelId,serverID, jname);
			t.env->DeleteLocalRef(jname);
            t.env->DeleteLocalRef(t.classID);
        }
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

   
    onFeedbackViewOpen(funid, uid, serverID, channelId, caractorName);
#else


ShowMessage("not support ");
#endif

}


void FeedbackLayer::callbackWeb(char *url)
{
	extern void openWebUrl(std::string urlstring);

    //guan wang
    CCLOG("%s %d %s", __FUNCTION__, __LINE__,
    url);
    openWebUrl( string(url));
}


void FeedbackLayer::callbackHotSpot(CCObject *sender)
{
    //re dian wen ti
    CCLOG("%s %d", __FUNCTION__, __LINE__);
    callPlatformFunc(1);
    //callbackWeb("http://www.baidu.com");
}
void FeedbackLayer::callbackOnline(CCObject *sender)
{
    //zhai xian ti wen
    CCLOG("%s %d", __FUNCTION__, __LINE__);
    callPlatformFunc(4);
    //ShowMessage(LuaTinkerManager::Get()->callLuaFunc<char *>("Script/handleMessage.lua", "strfengchi"));
    
}
void FeedbackLayer::callbackForum(CCObject *sender)
{
    //lun tan
    CCLOG("%s %d", __FUNCTION__, __LINE__);
    callPlatformFunc(2);
    //ShowMessage(LuaTinkerManager::Get()->callLuaFunc<char *>("Script/handleMessage.lua", "strfengchi"));
}
void FeedbackLayer::callbackReply(CCObject *sender)
{
    //zhai xian hui da
    CCLOG("%s %d", __FUNCTION__, __LINE__);
    //callPlatformFunc(3);
    //ShowMessage(LuaTinkerManager::Get()->callLuaFunc<char *>("Script/handleMessage.lua", "strfengchi"));
}

void FeedbackLayer::callbackClose(CCObject* sender)
{
    CCLOG("exit call back");
	this->setVisible(false);
    
    //callPlatformFunc(0);
    UIManager::sharedManager()->RemoveUILayout(g_FeedBackName);
    //CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	//ShowChatBar(true,-1);
    this->removeFromParentAndCleanup(true);
}

FeedbackLayer* FeedbackLayer::instance = NULL;
FeedbackLayer* FeedbackLayer::Get()
{
	if (instance == NULL)
	{
		instance = new FeedbackLayer;
	}
	return instance;
}
