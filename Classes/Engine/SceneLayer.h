//////////////////////////////////////////////////////////////////////////
//new Scene layer class 
//////////////////////////////////////////////////////////////////////////
#ifndef _SCENE_LAYER_
#define _SCENE_LAYER_


#include "cocos2d.h"
#include "HSJoystick.h"


#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "Win32Platform/thread_win32.h"
#endif
#include "TXGUIHeader.h"



using namespace cocos2d;


//////////////////////////////////////////////////////////////////////////
//回到地图的方式
//////////////////////////////////////////////////////////////////////////
typedef enum
{
	KMapFromLast,				//
	KMapFromMyRoom,				//
	KMapFromStar,				//
	KMapFromTransition,			//

}KMapFromType;

//////////////////////////////////////////////////////////////////////////

class SpriteSeer;
class LevelLayer;
class MainMenuLayer;
class LoadingLayer;
//////////////////////////////////////////////////////////////////////////


class SceneLayer : public cocos2d::CCLayer
{
	friend class StepMapLoader;

	/// 分步加载器，用于将加载过程切分到不同的帧中去,这样做的目的是防止阻止了网络层的update
	class StepMapLoader
	{
	public:
		StepMapLoader(SceneLayer* scene)
		{
			m_iCurStep = E_REQ;
			m_pScene = scene;
		}

		void Update();

		void Begin(){m_iCurStep = E_REQ;};

		bool isCompleted() {return m_iCurStep == E_END;} 
		
	private:
		void playMapBGM();
	protected:
		enum STATE
		{
			E_REQ,	// 从服务器请求信息
			E_RSP,	// 完全从服务器取的信息

			E_ANIM,
			E_ROLE,
			E_MAP,
			E_STORY,

			E_END,
		};
		/// current loading step, -1 means finished
	    STATE m_iCurStep;	
		SceneLayer* m_pScene;
	};
	StepMapLoader*  m_pStepMapLoader;


public:

	//////////////////////////////////////////////////////////////////////////
    enum SCENE_Z
    {
        SCENE_Z_MAP,        
        SCENE_Z_MESSAGE_BOX,
        SCENE_Z_MAIN_MENU=9,
        SCENE_Z_DYNAMIC_UI,
    };

	//////////////////////////////////////////////////////////////////////////


public:

    SceneLayer();
    virtual~SceneLayer();

public:
	CREATE_FUNC(SceneLayer);
	//init
    bool							init(); 
    

	//info    
    int								getCurMapId()               {return m_curMapId;}
    int								getLastMapId()              {return m_lastMapId;}
    int								getCurrStarID()             {return m_bCurrStarID;}    
	LevelLayer*						getLevelLayer()				{return m_LevelLayer;}

    bool							isInHometownMap();    
    SpriteSeer*						getSpriteSeer();    
	MainMenuLayer*					getMainMenuLayer();    
	CCLayer*						getUiLayerByTag(int tag);

	//
	void							setCurrStarID(int currID)	{m_bCurrStarID = currID;}
	void							setMainMenuLayerVisible(bool isVisible);
	void							setUiLayer(CCLayer* layer,int tag=0);
	void							addUiLayer(CCLayer* layer,float zorder,int tag);


	//actions    
    void							enterMyRoom();
    void							resumeLastMap();
	void							setupMap();
    void							changeMap(int mapId,KMapFromType fromMapType);    
    void							EnterFB();

    void							clearCurScene();
    void							removeUi();
	void							removeMap();  
    void							hideLoading();
    void							showLoading(bool bShowOnly = false);
    //overrides
    virtual void					ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void onFirstEnterMainland();

private:
    void                            realtimeUpdateBar(float dt);
    void                            realtimeUpdateDone(float dt);

	/// LoadMapUpdate
	void	loadMapUpdate(float dt);
    
    void                            loadResoure(int percent);
    void                            setupLoadbar(int percent);
    void                            hideloadbar_func(CCNode* sender);
    void                            removeColorLayer();
    void                            loadMap();
	void                            hideloading(float dt);
	void                            hideloadlayer();
	void							hideloadlayerOnFirstGame(float dt);
    
    void                            setLoadingBarPercent(int percent);
protected:
    
	LevelLayer*						m_LevelLayer;    
    CCLayer*						m_uiLayer;    
    
    int								m_curMapId;    
    int								m_lastMapId;    
    int								m_bCurrStarID;    
    int								m_fromMapType;

	HSJoystick *					joystick;
	LoadingLayer*					m_pLoadingUI;
	bool						    needUpdate;
    
    pthread_t                       _thread;//当前线程的线程ID
    CCSprite*                       _barFrame;
	double                          m_accumulator;
    double                          m_lastTickTime;
    
    
    TXGUI::UILayout                 *m_pLoadinglayout;
};


// -------------------------------------------------------------------------------------
class MainLandLayer : public SceneLayer
{
	// TODO : 
public:

	MainLandLayer();
	virtual ~MainLandLayer();

	CREATE_FUNC(MainLandLayer);
};

// ------------------------------------------------------------------------------------
class FBLayer : public SceneLayer
{
	// TODO : 
public:

	FBLayer();
	virtual ~FBLayer();

	CREATE_FUNC(FBLayer);
};
#endif // _SCENE_LAYER_



