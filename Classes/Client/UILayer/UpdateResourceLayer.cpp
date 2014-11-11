#include "OnlineNetworkManager.h"
#include "ResourcesUpdateManager.h"
#include "UpdateResourceLayer.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "UIDefine.h"
#include "UIManager.h"
#include "AspriteManager.h"
#include "XLogicManager.h"
#include "Localization.h"
#include "GameResourceManager.h"
#include "TXGUI.h"
#include "GameFontManager.h"
#include "GameAudioManager.h"
#include "AppDelegate.h"
#include "MessageBoxLayer.h"
#include "NetWorkConfigInfoFromServer.h"
#include "LuaTinkerManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "NcSDKManager.h"
#endif

//
//  int  检测文件列表结束，返回需要更新的文件个数。
//
int CheckDownLoadFileNumsCallBack(int nums)
{
	CCLOG("download step 16");
	UpdateResourceLayer::SetSumFileNums(nums);
	return 0;
}

//
//  int     更新第N个文件.
//  bool    更新第N个文件是否成功.
//  bool    是否更新完成.
//
int CheckDownLoadFileStateCallBack(int index, bool suc, bool over)
{
	CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
	UpdateResourceLayer* pLayer = NULL;
	if (pScene)
	{
		pLayer = (UpdateResourceLayer*)pScene->getChildByTag(XLogicManager::E_UPDATE_RESOURCE);
	}
	if (suc)
	{
		UpdateResourceLayer::SetDownLoadFileNums(index);		
		if (pLayer)
		{
			pLayer->ShowLoadingControls(true);
			pLayer->ShowCurProgressInfo();
			pLayer->ShowCurLoadingProgress();
			pLayer->ShowRcsDownloadTips();
		}		
	}
	if (over)
	{
		UpdateResourceLayer::SetDownLoadOver(over);
	}
	return 0;
}

int UpdateResourceLayer::nSumFileNums = 0;
int UpdateResourceLayer::nDownLoadFileNums = 0;
bool UpdateResourceLayer::bDownLoadOver = false;

UpdateResourceLayer::UpdateResourceLayer()
{
	layout = NULL;
	txtProgressLabel = NULL;
	txtSpeedLabel = NULL;
	txtUpdateLabel = NULL;
	iconLoadingPic = NULL;
	barLoadingPic = NULL;
	decLoadingPic = NULL;
	fRunningTimer = 0;
	bInit = false;
	isFirstComeGame = false;
	isAnimPause = false;
	configFileName = "firstChecker.cf";

	for (int index = 0;index<ANIMATION_COUNT;index++)
	{
		animationSprites[index] = NULL;
	}
	EUpdateState = UPDATE_STATE_UNKNOW;
	animationFrame = 0;

	for (int i = 0;i<ANIMATION_COUNT;i++)
	{
		animationTimer[i] = 5;
	}
	animatonLastTime = 0;
	oneAnimationFadTime = 0;
	labelDistance = 0;
	labelRealDistance = 0;
	labelMoveSpeed = 0;
	labelMoveTime = 0;
	isShowStartAnimationText = true;
	skipAnimationBtn = NULL;
	curLabelPageIndex = 0;
}

UpdateResourceLayer::~UpdateResourceLayer()
{
	TXGUI::UIManager::sharedManager()->RemoveUILayout("LoadingLayer");	
	MessageBox::clearMessageBox();
}

bool UpdateResourceLayer::init()
{
	bool bRet = false;
	do {
		CCLOG("download step 3");
		AspriteManager::getInstance()->OnlyLoadLoingAsprite();
	
		if (false == NetWorkConfigInfoFromServer::sharedInstance()->IsDownLoadContentFromServer())
		{
			this->scheduleOnce(schedule_selector(UpdateResourceLayer::PopupWarnningMessageBox),0);
		}
		else
		{
			InitLayer();
			CheckFirstComeIntoGameFlag();
		}

		bRet = true;
	}while (0);
	return true;
}

void UpdateResourceLayer::PopupWarnningMessageBox(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NcSDKManager::NetworkDisconnect(Localizatioin::getLocalization("M_UNCONNET"));
    return;
#endif
	std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
	TXGUI::MessageBox::AdjustPriority(true);
	TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
	TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
	TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
}


bool UpdateResourceLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return CCLayer::ccTouchBegan(pTouch,pEvent);
}

void UpdateResourceLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}

void UpdateResourceLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchEnded(pTouch,pEvent);
}

void UpdateResourceLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}

void UpdateResourceLayer::registerWithTouchDispatcher()
{
	CCLayer::registerWithTouchDispatcher();
}

void UpdateResourceLayer::ShowTipInfo()
{
	if (txtUpdateLabel)
	{
		const char* languageFlag = "M_RESOURCE_UPDATE";
		const char* content = Localizatioin::getLocalization(languageFlag);
		txtUpdateLabel->setString(content);
	}
}

void UpdateResourceLayer::ShowCurProgressInfo()
{
	if (rcsUpdateProcessLabel_1)
	{
		char buffer[100];
		sprintf(buffer,"%d / %d",this->nDownLoadFileNums,this->nSumFileNums);
		rcsUpdateProcessLabel_1->setString(buffer);
	}
}

void UpdateResourceLayer::ShowSpeedInfo()
{

}

void UpdateResourceLayer::ShowCurLoadingProgress(float _progress /* = -1 */)
{
	float progress = 0;
	float scale =  TXGUI::UIManager::sharedManager()->getScaleFactor();
	if (-1 == _progress)
	{
		if (nSumFileNums != 0)
		{
			progress = (float)nDownLoadFileNums/(float)nSumFileNums;
		}	
		else
		{
			progress = 1.0;
		}
	}
	else
	{
		progress = _progress;
	}

	//barLoadingPic->setScale(scale*progress,1);
	//decLoadingPic->setScale(scale*progress,1);

	rcsUpdateBarLoading->setScale(scale*progress,1);
}

void UpdateResourceLayer::removeCocosActivityImageView()
{
	CCLOG("download step 20");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, "com/taomee/adventure/adventure", "RemoveCocosActivityBgImageView", "()V"))
	{
	    t.env->CallStaticVoidMethod(t.classID, t.methodID);
	    t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void UpdateResourceLayer::update(float fDelta)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	static bool bRemoveView = false;	
	if (fRunningTimer<0.3)
	{
		fDelta = 0.1;
		fRunningTimer += fDelta;
		return ;
	}
	else
	{
		if (false == bRemoveView) {
			CCLOG("remove View");
			removeCocosActivityImageView();			
			bRemoveView = true;
		}
	}
#endif

	if(EUpdateState == UPDATE_STATE_START_ANIM)
	{
		UpdateStartAnimation(fDelta);
	}
	else if (EUpdateState == UPDATE_STATE_DOWN_RCS)
	{
		UpdateResource(fDelta);
	}
}

void UpdateResourceLayer::DownLoadOver()
{
	removeCocosActivityImageView();
	CCLOG("download step 14");
	txtUpdateLabel->setVisible(true);
	if (txtUpdateLabel)
	{
		const char* languageFlag = "M_LOGIN_FLOW3";
		const char* content = Localizatioin::getLocalization(languageFlag);
		txtUpdateLabel->setString(content);
	}
	scheduleOnce(schedule_selector(UpdateResourceLayer::GotoLoginScene),1.0f);
	UpdateResourceLayer::bDownLoadOver = true;
}

// Note: 开场动画相关
void UpdateResourceLayer::CheckFirstComeIntoGameFlag()
{
	//std::string path = ResourceUpdateManager::sharedInstance()->GetResourceUpdateDir();
	//std::string writablePath = GameResourceManager::sharedManager()->writablePathFromRelativePath(path.c_str());
	//std::string filePath = writablePath + "/" + configFileName;
	//
	//do
	//{
	//	// read the file from hardware
	//	FILE *fp = fopen(filePath.c_str(), "a+");
	//	CCAssert(fp != NULL,"UpdateResourceLayer file check error");
	//	
	//	{
	//		fseek(fp,0,SEEK_END);
	//		fseek(fp,0,SEEK_SET);

	//		char buffer[100];
	//		memset(buffer,'\0',100);
	//		fread(buffer,1,1,fp);
	//		
	//		if (strcmp(buffer,"") == 0)
	//		{
	//			isFirstComeGame = true;
	//			fwrite("1",1,1,fp);
	//		}
	//		else if (strcmp(buffer,"0") == 0)
	//		{
	//			isFirstComeGame = true;
	//			fwrite("1",1,1,fp);
	//		}
	//		else if (strcmp(buffer,"1") == 0)
	//		{
	//			isFirstComeGame = false;
	//		}
	//	}
	//	
	//	fclose(fp);
	//} while (0);

	//isFirstComeGame = true;
	//if (isFirstComeGame)
	//{
	//	isShowStartAnimationText = true;
	//	StartPlayStartAnimation();
	//}
	//else
	//{
	//	StartDownLoadResource();
	//}

	StartDownLoadResource();
}

void UpdateResourceLayer::StartContentAnimation(int pageIndex)
{
	curLabelPageIndex = pageIndex;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
	layout = manager->getUILayout("LoadingLayer");
	if (layout == NULL)
	{
		layout = manager->CreateUILayoutFromFile("UIplist/loading.plist", this, "LoadingLayer");
	}	
	TXGUI::UIPicture* oneLabelPic = layout->FindChildObjectByName<UIPicture>("startOneLabelArea");
	OneLabelAreaSize = oneLabelPic->getSpriteSize();
	TXGUI::UIPicture* labelAreaPic = layout->FindChildObjectByName<UIPicture>("startDialogArea");
	ContentDisplayAreaSize = labelAreaPic->getSpriteSize();	

	// Note: 得到显示文本的基本信息
	ContentDisplayAreaMidPt = ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
	ContentDownAreaPt = ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - ContentDisplayAreaSize.height/2);
	ContentUpAreaPt = ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + ContentDisplayAreaSize.height/2);

	int onePageLabelCounts = LABEL_COUNTS/ANIMATION_COUNT;
	float scale = manager->getScaleFactor();
	labelDistance = 20 * scale;
	labelRealDistance = OneLabelAreaSize.height + labelDistance;
	float moveTotalDistance = (onePageLabelCounts-1)*labelRealDistance + ContentDisplayAreaSize.height;
	float sumTime = 0;

	int startLabelIndex = (onePageLabelCounts) * pageIndex;
	int endLabelIndex = startLabelIndex + onePageLabelCounts -1;

	//for (int i = 0; i < ANIMATION_COUNT;i++)
	{
		sumTime += (animationTimer[pageIndex]);
	}
	labelMoveSpeed = moveTotalDistance / (sumTime);

	for (int index = startLabelIndex;index <= endLabelIndex; index++)
	{
		char buffer[100];
		sprintf(buffer,"STARTSCENE_TEXT_%d",index+1);
		const char* content = Localizatioin::getLocalization(buffer);
		int fontSize = 24*UIManager::sharedManager()->getScaleFactor();
		CCLabelTTF* pTTFLabel = CCLabelTTF::create(content,KJLinXin,fontSize,OneLabelAreaSize,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		displayLabels[index].pLabel = pTTFLabel;
		displayLabels[index].pLabel->setVisible(false);

		//this->addChild(displayLabels[index].pLabel,1);
		this->addChild(pTTFLabel,1);
	}
}

void UpdateResourceLayer::StartPlayStartAnimation()
{
	ShowLoadingControls(false);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	for (int index = 0;index<ANIMATION_COUNT;index++)
	{	
		char buffer[100];
		sprintf(buffer,"UI/ui_loading_scene_%d.jpg",index+1);
		animationSprites[index] = CCSprite::create(buffer);

		if (animationSprites[index] != NULL)
		{
			animationSprites[index]->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
			animationSprites[index]->setAnchorPoint(ccp(0.5,0.5));

            // animationSprites[index]->setScale(2.0f);

			this->addChild(animationSprites[index], 0);

			float scaleX = visibleSize.width/animationSprites[index]->getTexture()->getPixelsWide();
			float scaleY = visibleSize.height/animationSprites[index]->getTexture()->getPixelsHigh();
			float scale = (scaleX > scaleY) ? scaleX : scaleY;
			animationSprites[index]->setScaleX(scale);
			animationSprites[index]->setScaleY(scale);

			animationSprites[index]->setVisible(false);
		}
	}

	//if (isShowStartAnimationText)
	//{
	//	StartContentAnimation(0);
	//}

	EUpdateState = UPDATE_STATE_START_ANIM;	
	//this->schedule(schedule_selector(UpdateResourceLayer::update,0.2f));
	GameAudioManager::sharedManager()->playStartGameAnimationSound();
}

void UpdateResourceLayer::EndStartAnimation()
{
	if (isShowStartAnimationText)
	{
		for (int index = 0;index < LABEL_COUNTS; index++)
		{
			displayLabels[index].pLabel->setVisible(false);	
		}
	}

	StartDownLoadResource();

	std::string path = ResourceUpdateManager::sharedInstance()->GetResourceUpdateDir();
	std::string writablePath = GameResourceManager::sharedManager()->writablePathFromRelativePath(path.c_str());
	std::string filePath = writablePath + "/" + configFileName;
	do
	{
		// read the file from hardware
		FILE *fp = fopen(filePath.c_str(), "a+");
		CCAssert(fp != NULL,"UpdateResourceLayer file check error");
		{
			fseek(fp,0,SEEK_END);
			fseek(fp,0,SEEK_SET);

			fwrite("1",1,1,fp);
		}
		fclose(fp);
	} while (0);

	GameAudioManager::sharedManager()->stopPlayBGM();
}

void UpdateResourceLayer::InitLayer()
{
	TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
	layout = manager->getUILayout("LoadingLayer");
	if (layout == NULL)
	{
		layout = manager->CreateUILayoutFromFile("UIplist/loading.plist", this, "LoadingLayer");
		layout->setZOrder(100);
	}	

	txtProgressLabel = layout->FindChildObjectByName<UILabel>("textProgress");
	txtSpeedLabel = layout->FindChildObjectByName<UILabel>("textSpeed");
	txtUpdateLabel = layout->FindChildObjectByName<UILabel>("textUpdate");
	iconLoadingPic = layout->FindChildObjectByName<UIPicture>("iconLoading");
	barLoadingPic = layout->FindChildObjectByName<UIPicture>("barLoading");
	decLoadingPic = layout->FindChildObjectByName<UIPicture>("frameLoadingDec");
	barFrameLoading = layout->FindChildObjectByName<UIPicture>("frameLoading");
	skipAnimationBtn = layout->FindChildObjectByName<UIButton>("skipBtn");

	manager->RegisterMenuHandler("LoadingLayer","UpdateResourceLayer::skipBtnClick",
		menu_selector(UpdateResourceLayer::skipBtnClick), this);

	tipLabel = layout->FindChildObjectByName<UILabel>("textTips");
	rcsUpdateSpeedLabel = layout->FindChildObjectByName<UILabel>("rcsUpdateSpeedLabel");
	rcsUpdateProcessLabel_1 = layout->FindChildObjectByName<UILabel>("rcsUpdateProgressLabel_1");
	rcsUpdateProcessLabel_2 = layout->FindChildObjectByName<UILabel>("rcsUpdateProgressLabel_2");
	rcsUpdateCurVersionLabel = layout->FindChildObjectByName<UILabel>("rcsUpdateCurVersionLabel");
	rcsUpdateNewVersionLabel = layout->FindChildObjectByName<UILabel>("rcsUpdateNewVersionLabel");
	rcsUpdateFrameLoading = layout->FindChildObjectByName<UIPicture>("rcsUpdateFrameLoading");
	rcsUpdateBarLoading = layout->FindChildObjectByName<UIPicture>("rcsUpdateBarLoading");

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	for (int index = 0;index<ANIMATION_COUNT;index++)
	{	
		char buffer[100];
		sprintf(buffer,"UI/ui_loading_scene_%d.jpg",index+1);
		animationSprites[index] = CCSprite::create(buffer);

		if (animationSprites[index] != NULL)
		{
			animationSprites[index]->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
			animationSprites[index]->setAnchorPoint(ccp(0.5,0.5));

            // animationSprites[index]->setScale(2.0f);

			this->addChild(animationSprites[index], 0);

			float scaleX = visibleSize.width/animationSprites[index]->getTexture()->getPixelsWide();
			float scaleY = visibleSize.height/animationSprites[index]->getTexture()->getPixelsHigh();
			float scale = (scaleX > scaleY) ? scaleX : scaleY;
			animationSprites[index]->setScaleX(scale);
			animationSprites[index]->setScaleY(scale);

			animationSprites[index]->setVisible(false);
		}
	}
}

void UpdateResourceLayer::skipBtnClick(CCObject* sender)
{
	if (animationFrame < ANIMATION_COUNT)
	{
		if (animationSprites[animationFrame] != NULL)
		{
			animationSprites[animationFrame]->setVisible(false);
		}
	}	
	for (int i = 0;i<LABEL_COUNTS;i++)
	{
		if (displayLabels[i].pLabel != NULL)
		{
			displayLabels[i].pLabel->setVisible(false);
			displayLabels[i].pLabel = NULL;		
		}
	}
	StartDownLoadResource();

	std::string path = ResourceUpdateManager::sharedInstance()->GetResourceUpdateDir();
	std::string writablePath = GameResourceManager::sharedManager()->writablePathFromRelativePath(path.c_str());
	std::string filePath = writablePath + "/" + configFileName;
	do
	{
		// read the file from hardware
		FILE *fp = fopen(filePath.c_str(), "a+");
		CCAssert(fp != NULL,"UpdateResourceLayer file check error");
		{
			fseek(fp,0,SEEK_END);
			fseek(fp,0,SEEK_SET);

			fwrite("1",1,1,fp);
		}
		fclose(fp);
	} while (0);

	GameAudioManager::sharedManager()->stopPlayBGM();
}

void UpdateResourceLayer::ShowLoadingControls(bool bShowLoadingControls)
{
	txtProgressLabel->setVisible(false);
	txtSpeedLabel->setVisible(false);
	txtUpdateLabel->setVisible(false);
	iconLoadingPic->setVisible(false);
	barLoadingPic->setVisible(false);
	decLoadingPic->setVisible(false);
	barFrameLoading->setVisible(false);
	skipAnimationBtn->setVisible(false);

	tipLabel->setVisible(false);
	rcsUpdateSpeedLabel->setVisible(false);
	rcsUpdateProcessLabel_1->setVisible(false);
	rcsUpdateProcessLabel_2->setVisible(false);
	rcsUpdateCurVersionLabel->setVisible(false);
	rcsUpdateNewVersionLabel->setVisible(false);
	rcsUpdateFrameLoading->setVisible(false);
	rcsUpdateBarLoading->setVisible(false);

	// Note: 显示Layout控件
	if (bShowLoadingControls)
	{
		//txtProgressLabel->setVisible(true);
		//txtSpeedLabel->setVisible(true);
		//txtUpdateLabel->setVisible(true);
		//iconLoadingPic->setVisible(true);
		//barLoadingPic->setVisible(true);
		//decLoadingPic->setVisible(true);
		//barFrameLoading->setVisible(true);		

		tipLabel->setVisible(true);
		rcsUpdateSpeedLabel->setVisible(true);
		rcsUpdateProcessLabel_1->setVisible(true);
		rcsUpdateProcessLabel_2->setVisible(true);
		rcsUpdateCurVersionLabel->setVisible(true);
		rcsUpdateNewVersionLabel->setVisible(true);
		rcsUpdateFrameLoading->setVisible(true);
		rcsUpdateBarLoading->setVisible(true);
	}
	else
	{
		skipAnimationBtn->setVisible(true);
	}
}

void UpdateResourceLayer::StartDownLoadResource()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* pSprite = CCSprite::create("UI/frontCover.jpg");
	if (pSprite == NULL)
	{
		CCLOG("download step 6");
		pSprite = new CCSprite();
		pSprite->initWithTexture(NULL,CCRectZero);
	}

	CCLOG("download step 7");
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(pSprite, 0,1001);

	CCLOG("download step 8");

	if (pSprite->getTexture())
	{
		float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
		float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
		float scale = (scaleX > scaleY) ? scaleX : scaleY;
		pSprite->setScaleX(scale);
		pSprite->setScaleY(scale);
	}

	ShowLoadingControls(true);
	iconLoadingPic->setVisible(false);

	float scale =  TXGUI::UIManager::sharedManager()->getScaleFactor();
	rcsUpdateBarLoading->getCurrentNode()->setAnchorPoint(ccp(0,0.5));		
	rcsUpdateBarLoading->getCurrentNode()->setScaleX(scale);
	CCPoint oldPos = rcsUpdateBarLoading->getCurrentNode()->getPosition();
	rcsUpdateBarLoading->getCurrentNode()->setPosition(ccpSub(oldPos,ccp(scale * rcsUpdateBarLoading->getCurrentNode()->getContentSize().width * 0.5,0)));
	decLoadingPic->getCurrentNode()->setAnchorPoint(ccp(0,0.5));		
	decLoadingPic->getCurrentNode()->setScaleX(scale);
	oldPos = decLoadingPic->getCurrentNode()->getPosition();
	decLoadingPic->getCurrentNode()->setPosition(ccpSub(oldPos,ccp(scale * decLoadingPic->getCurrentNode()->getContentSize().width * 0.5,0)));
	iconLoadingPic->getCurrentNode()->setScaleX(scale);

	//this->ShowCurProgressInfo();
	//this->ShowCurLoadingProgress(0);
	//this->ShowTipInfo();
	// Note: 显示资源版本检测中
	ShowLoadingControls(false);
	txtUpdateLabel->setVisible(true);
	skipAnimationBtn->setVisible(false);
	if (txtUpdateLabel)
	{
		const char* languageFlag = "M_LOGIN_FLOW1";
		const char* content = Localizatioin::getLocalization(languageFlag);
		txtUpdateLabel->setString(content);
	}

	scheduleOnce(schedule_selector(UpdateResourceLayer::CheckResouceVersion),1.0f);
	
	animationFrame = 0;
	EUpdateState = UPDATE_STATE_DOWN_RCS;

	CCLOG("download step 9");
}

void UpdateResourceLayer::ShowRcsDownloadTips()
{
	if (txtUpdateLabel)
	{
		txtUpdateLabel->setVisible(true);
		skipAnimationBtn->setVisible(false);
		if (txtUpdateLabel)
		{
			const char* languageFlag = "M_LOGIN_FLOW2";
			const char* content = Localizatioin::getLocalization(languageFlag);
			txtUpdateLabel->setString(content);
		}
	}
}

void UpdateResourceLayer::GotoLoginScene(float dt)
{
	// Note: 重新加载在更新下载之前需要使用的Lua
	LuaTinkerManager::Get()->ForceCheckLoadFile("Script/GameConfigParse.lua");

	LuaTinkerManager::Get()->ForceCheckLoadFile("Script/GameConfig/Localization.lua");
	LuaTinkerManager::Get()->ForceCheckLoadFile("Script/Language.lua");
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	LuaTinkerManager::Get()->ForceCheckLoadFile("Script/svn_version.lua");
#else

#endif
	XLogicManager::sharedManager()->goToLayer(XLogicManager::E_Notice, NULL);
}

void UpdateResourceLayer::CheckResouceVersion(float dt)
{
	UpdateResourceLayer::bDownLoadOver = false;

	bool bForceExistForTest = false;
	if (bForceExistForTest)
	{
		DownLoadOver();
		return ;
	}

	int nResult = ResourceUpdateManager::sharedInstance()->UpdateNeedResourceByType(1);
	
	if (nResult == 1)
	{
		// Note: 不需要更新
		CCLOG("download step 10");
		DownLoadOver();
	}
	else if (nResult == 2)
	{
		AspriteManager::getInstance()->OnlyLoadNoticeAsprite();
		std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
		TXGUI::MessageBox::AdjustPriority(true);
		TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
		TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
		TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
	}
	else if (nResult == -1)
	{
		return;
	}
	else if (UpdateResourceLayer::nSumFileNums == 0)
	{
		CCLOG("download step 12");
		DownLoadOver();
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        removeCocosActivityImageView();			
#endif

        // 给出提示
        AspriteManager::getInstance()->OnlyLoadNoticeAsprite();

        CCString* sizeStr = CCString::createWithFormat("%.1f M", (float) ResourceUpdateManager::sharedInstance()->getUpdateSize() / 1024 / 1024);
        MessageBox::Show(CCString::createWithFormat(Localizatioin::getLocalization("M_UPDATE_QUESTION2"), 
            sizeStr->getCString())->getCString(), 
            this, menu_selector(UpdateResourceLayer::OnConfirmUpdate), 
			menu_selector(UpdateResourceLayer::OnCancel), MB_OKCANCELCLOSE,ccWHITE,28,KMessageBoxDefaultPriority);
        MessageBox::AdjustStyle(Localizatioin::getLocalization("M_CONFIRM"), Localizatioin::getLocalization("M_CANCEL"));
        MessageBox::showCloseBtn();
	}	
}

void UpdateResourceLayer::OnConfirmUpdate(CCObject* sender)
{
    this->scheduleUpdate();
}

void UpdateResourceLayer::OnCancel(CCObject* sender)
{
    // 将玩家弹出
    CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void UpdateResourceLayer::UpdateStartAnimation(float fDelta)
{
	if (animationFrame < ANIMATION_COUNT && false == isAnimPause)
	{
		if (animationSprites[animationFrame] != NULL)
		{
			if (false == animationSprites[animationFrame]->isVisible())
			{
				if (isShowStartAnimationText)
				{
					StartContentAnimation(animationFrame);
				}

				animatonLastTime = 0;
				animationSprites[animationFrame]->setVisible(true);

				if (animationFrame != 0)
				{
					CCActionInterval*  action = CCFadeIn::create(oneAnimationFadTime);
					animationSprites[animationFrame]->setOpacity(0);
					animationSprites[animationFrame]->runAction(action);
				}
			}
			else
			{
				animatonLastTime += fDelta;
				if (animatonLastTime >= (animationTimer[animationFrame] - oneAnimationFadTime))
				{
					CCActionInterval*  action = CCFadeOut::create(oneAnimationFadTime);
					CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(UpdateResourceLayer::OneAnimationOver));
					CCSequence *seq = (CCSequence*)CCSequence::create(action,rc,NULL);						
					animationSprites[animationFrame]->runAction(seq);

					isAnimPause = true;
				}
			}
		}
	}

	if (animationFrame >= ANIMATION_COUNT)
	{
		//EndStartAnimation();
		return;
	}

	int onePageLabelCounts = LABEL_COUNTS/ANIMATION_COUNT;
	int EndLabelCount = animationFrame*onePageLabelCounts + onePageLabelCounts;
	int lastLabelIndex = -1;
	// Note: 处理文字效果
	for (int index = 0;index < EndLabelCount; index++)
	{
		if (displayLabels[index].state != 0)
		{
			CCPoint newPt = displayLabels[index].pLabel->getPosition();
			if (newPt.y >= ContentUpAreaPt.y && displayLabels[index].state != 2)
			{
				if (index == LABEL_COUNTS -1)
				{
					CCLOG("");
				}
				displayLabels[index].state = 2;
				CCActionInterval*  action = CCFadeOut::create(1);				
				CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(UpdateResourceLayer::OneLabelFadOutOver));
				CCSequence *seq = (CCSequence*)CCSequence::create(action,rc,NULL);
				displayLabels[index].pLabel->runAction(seq);				
			}

			lastLabelIndex = index;
		}
	}

	
	lastLabelIndex ++;
	if (lastLabelIndex == EndLabelCount)
	{
		lastLabelIndex = EndLabelCount-1;
	}

	if (displayLabels[lastLabelIndex].state == 0)
	{
		bool bCreateNewLabel = false;
		if (lastLabelIndex != 0)
		{
			CCPoint p = displayLabels[lastLabelIndex-1].pLabel->getPosition();
			if (p.y >= ContentDownAreaPt.y + labelRealDistance)
			{
				bCreateNewLabel = true;
			}
		}
		else
		{
			bCreateNewLabel = true;
		}

		if (bCreateNewLabel)
		{
			if (lastLabelIndex == LABEL_COUNTS - 1)
			{
				CCLOG("");
			}
			displayLabels[lastLabelIndex].state = 1;
			CCActionInterval*  action = CCFadeIn::create(1);
			displayLabels[lastLabelIndex].pLabel->setOpacity(0);
			displayLabels[lastLabelIndex].pLabel->runAction(action);
			displayLabels[lastLabelIndex].pLabel->setVisible(true);
			displayLabels[lastLabelIndex].pLabel->setPosition(ContentDownAreaPt);

			CCPoint dstPoint = ccp(0,10000);
			float duration = (dstPoint.y)/labelMoveSpeed;
			CCMoveBy* moveAction = CCMoveBy::create(duration,dstPoint);
			displayLabels[lastLabelIndex].pLabel->runAction(moveAction);
		}
	}
}

void UpdateResourceLayer::OneUpdateRcsAnimationOver()
{
	animatonLastTime = 0;
	animationSprites[animationFrame]->setVisible(false);
	animationFrame ++;
	isAnimPause = false;
	if (animationFrame >= ANIMATION_COUNT)
	{
		animationFrame = 0;
	}
}

void UpdateResourceLayer::OneAnimationOver()
{
	animatonLastTime = 0;
	animationSprites[animationFrame]->setVisible(false);
	animationFrame ++;
	if (animationFrame >= ANIMATION_COUNT)
	{
		//if (isShowStartAnimationText == false)
		{
			EndStartAnimation();
		}
	}
	isAnimPause = false;
}

void UpdateResourceLayer::OneLabelFadOutOver()
{
	if (displayLabels[LABEL_COUNTS-1].state == 2)
	{
		EndStartAnimation();
	}
}

void UpdateResourceLayer::UpdateResource(float fDelta)
{
	if (animationFrame < ANIMATION_COUNT && false == isAnimPause)
	{
		if (animationSprites[animationFrame] != NULL)
		{
			if (false == animationSprites[animationFrame]->isVisible())
			{
				animatonLastTime = 0;
				animationSprites[animationFrame]->setVisible(true);
				CCNode* pNode = this->getChildByTag(1001);
				if (pNode)
				{
					pNode->setVisible(false);
				}
				//if (animationFrame != 0)
				//{
				//	CCActionInterval*  action = CCFadeIn::create(oneAnimationFadTime);
				//	animationSprites[animationFrame]->setOpacity(0);
				//	animationSprites[animationFrame]->runAction(action);
				//}
			}
			else
			{
				animatonLastTime += fDelta;
				if (animatonLastTime >= (animationTimer[animationFrame] - oneAnimationFadTime))
				{
					//CCActionInterval*  action = CCFadeOut::create(oneAnimationFadTime);
					//CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(UpdateResourceLayer::OneUpdateRcsAnimationOver));
					//CCSequence *seq = (CCSequence*)CCSequence::create(action,rc,NULL);						
					//animationSprites[animationFrame]->runAction(seq);

					animatonLastTime = 0;
					animationSprites[animationFrame]->setVisible(false);
					animationFrame ++;
					isAnimPause = false;
					if (animationFrame >= ANIMATION_COUNT)
					{
						animationFrame = 0;
					}
					animationSprites[animationFrame]->setVisible(true);
				}
			}
		}
	}

	if (false == UpdateResourceLayer::bDownLoadOver)
	{
		if (false == ResourceUpdateManager::sharedInstance()->updateResource(CheckDownLoadFileStateCallBack))
		{
			// Note: 网络异常
			CCLOG("download step 15");
			//AppDelegate::GetShareAppDelegate()->ShowToast("Sorry , NetWork is disconnect ! UpdateResourceLayer update");
		}
	}	
	else
	{
		CCNode* pNode = this->getChildByTag(1001);
		if (pNode)
		{
			pNode->setVisible(true);
		}
		if (animationFrame < ANIMATION_COUNT)
		{
			if (animationSprites[animationFrame] != NULL)
			{
				animationSprites[animationFrame]->setVisible(false);
			}
		}
		ShowLoadingControls(false);
		skipAnimationBtn->setVisible(false);
		DownLoadOver();
		EUpdateState = UPDATE_STATE_UNKNOW;
	}
}
