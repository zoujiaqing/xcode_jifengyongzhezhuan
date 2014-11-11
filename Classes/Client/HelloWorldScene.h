#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CArmature.h"
#include "FrontCoverLayer.h"
#include "CharacterSelectLayer.h"
#include "errcode.pb.h"
#include "UIMessageLayer.h"
#include "IEvent.h"
using namespace cocos2d;

enum MoveType{NOTHING, MOVELEFT = -1, MOVERIGHT = 1, STAND, SQUAT, JUMP};

//#define TAG_HELLOWORLD    1000

class UpdateRcsScene : public cocos2d::CCScene
{
public:
	UpdateRcsScene();
	virtual ~UpdateRcsScene();

	static UpdateRcsScene *create(void);

};

class LoginScene : public cocos2d::CCScene
{
public:
	LoginScene();
	virtual ~LoginScene();

	static LoginScene *create(void);

};

// ------------------------------------------------------------------------------------

class GameScene : public cocos2d::CCScene
{
public:
	GameScene();
	virtual ~GameScene();

	static GameScene *create(void);

};

// ------------------------------------------------------------------------------------

class HelloWorld : public cocos2d::CCLayerColor, 
	public ISocketEvent
{
public:
	HelloWorld();
	virtual ~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
 

    // implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
	void update(float dt);

	void updateLogin(float dt);
    
	void updateManager(float dt);

    void backToLogin();
	//void draw();

	//void receiveNetWorkError(db_errcode_t error);

	void enterCreateRoleLayer();
    
    void enterFrontCoverLayer();

	void showMessage(const char* msg);

	FrontCoverLayer* getFrontLayer();

	/// override ISocketEvent
	virtual void OnSocketError(int nError);
	virtual void OnConnected(int nResult);
	virtual void OnDisconnect();

	void onNetWorkCallBackToFront(CCObject* obj);
	void onNetWorkCallBackExit(CCObject* obj);

	//virtual void	ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);	
	//virtual void keyBackClicked();//¼àÌýAndroid ·µ»Ø¼ü
	//virtual void keyMenuClicked();//¼àÌýAndroid ²Ëµ¥¼ü
	//void OkBtnClick(CCObject* obj);
	//void CancelBtnClick(CCObject* obj);
    void setNeedCheckNetWork(bool flag);

	// Note: ¼ì²âµ±Ç°°æ±¾ÊÇ·ñÐèÒª¸üÐÂ
	void updateOnNextFrame(float dt);
	bool sendCheckNewVersionMessage();
	void setUpLogin();

	void PopupWarnningMessageBox(float dt);
    void OnNextFrame(float dt);
protected:
//key process
	
virtual void keyBackClicked();//¼àÌýAndroid ·µ»Ø¼ü
virtual void keyMenuClicked();//¼àÌýAndroid ²Ëµ¥¼ü

void OkBtnClick(CCObject* obj);
void CancelBtnClick(CCObject* obj); 
	
private: 
	bool checkUserLogin();
	void OnVersionCheckBoxOkBtnClick(CCObject* pObject);
private:
    bool isNeedCheckNetWork;
	bool isUpdateRunning;
	CCMenuItemImage *pLoginItem;
	CCLabelTTF* pLabel;
	CCMenuItemImage *logOutItem;
	FrontCoverLayer* pFrontLayer;
	UIMessageLayer* pUIMessageLayer;
	CharacterSelectLayer* pCharacterLayer;
};

#endif  // __HELLOWORLD_SCENE_H__
