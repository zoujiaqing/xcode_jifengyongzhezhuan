#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include "cocos2d.h"
#define WINDOW_WIDHT      1136
#define WINDOW_HEIGHT     640

#define HQ_NPC_UI         0.6f
#define NQ_NPC_UI         0.5f

#define HQ_CHARACTER_BODYICON 0.6f
#define NQ_CHARACTER_BODYICON 0.5f

#define HQ_MAP_RATE           1.0f
#define NQ_MAP_RATE           0.5f

#define NQ_SKELETON           0.5f
#define HQ_SKELETON_CHARACTER 0.8f
#define HQ_SKELETON_NPC       0.8f
#define HQ_SKELETON_MONSTER   0.6f
#define HQ_SKELETON_FAIRY     0.8f
#define HQ_SKELETON_EFFECT    0.7f

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

	static AppDelegate* GetShareAppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
	virtual void applicationWindowFocusChanged( unsigned char HasFocus);
	void InitOnGameStart();
	void uploadCrashLog(); 

//	void ShowToast(char* content);
	const char* GetSvnVersion();
    void ClearIOSDocuments(std::string dirPath, bool flag);
private:
	bool m_bInit;
	static AppDelegate* s_sharedAppDelegate;

	void onGetFinished(cocos2d::CCNode *node,void *data);

	void FlurryChannelInfo();
	unsigned char  m_nobackgroud ;

    CC_SYNTHESIZE(std::string, m_client_version, ClientVersion);
};

#endif // _APP_DELEGATE_H_

