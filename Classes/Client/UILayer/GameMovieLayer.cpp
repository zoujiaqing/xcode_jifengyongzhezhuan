#include "OnlineNetworkManager.h"
#include "ResourcesUpdateManager.h"
#include "GameMovieLayer.h"
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
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "InstanceManager.h"
#include "GameConfigFromLuaManager.h"
#include "MainLandManager.h"

GameMovieLayer::GameMovieLayer()
{
	layout = NULL;
	isAnimPause = false;

	for (int index = 0;index<ANIMATION_COUNT;index++)
	{
		animationSprites[index] = NULL;
	}
	EUpdateState = UPDATE_STATE_UNKNOW;
	animationFrame = 0;

	for (int i = 0;i<ANIMATION_COUNT;i++)
	{
		animationTimer[i] = 8;
	}
	animatonLastTime = 0;
	oneAnimationFadTime = 1.0;
	labelDistance = 0;
	labelRealDistance = 0;
	labelMoveSpeed = 0;
	labelMoveTime = 0;
	isShowStartAnimationText = true;
	skipAnimationBtn = NULL;
	curLabelPageIndex = 0;
}

GameMovieLayer::~GameMovieLayer()
{
	TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
	if (manager)
	{
		manager->RemoveUILayout("GameMovieLayer");
	}
}

bool GameMovieLayer::init()
{
	bool bRet = false;
	do {		

		if(!CCLayerColor::init())
		{
			bRet = false;
			break;
		}

		setOpacity(255);
		InitLayer();
		this->scheduleUpdate();		
		this->setTouchEnabled(true);

		bRet = true;
	}while (0);
	return true;
}


bool GameMovieLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//return CCLayer::ccTouchBegan(pTouch,pEvent);
	if (isVisible())
	{
		return true;
	}
	return false;
}

void GameMovieLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}

void GameMovieLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchEnded(pTouch,pEvent);
}

void GameMovieLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCFirstGameAnimLayerPriority, true);	
}

void GameMovieLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}

void GameMovieLayer::update(float fDelta)
{
	if(EUpdateState == UPDATE_STATE_START_ANIM)
	{
		UpdateStartAnimation(fDelta);
	}
}

void GameMovieLayer::StartContentAnimation(int pageIndex)
{
	curLabelPageIndex = pageIndex;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
	layout = manager->getUILayout("GameMovieLayer");
	if (layout == NULL)
	{
		layout = manager->CreateUILayoutFromFile("UIplist/startAnimation.plist", this, "GameMovieLayer");
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

void GameMovieLayer::StartPlayStartAnimation()
{
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

	EUpdateState = UPDATE_STATE_START_ANIM;	
	GameAudioManager::sharedManager()->stopPlayBGM();
	GameAudioManager::sharedManager()->playStartGameAnimationSound();
}

void GameMovieLayer::EndStartAnimation()
{
	if (isShowStartAnimationText)
	{
		for (int index = 0;index < LABEL_COUNTS; index++)
		{
			displayLabels[index].pLabel->setVisible(false);	
		}
	}

	GameAudioManager::sharedManager()->stopPlayBGM();
	SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
	if (pSceneLayer)
	{
		int instanceId = InstanceManager::Get()->getCurInstanceId();
		int musicId = GameConfigFromLuaManager::Get()->getInstanceMusicId(instanceId,pSceneLayer->getCurMapId());
		GameAudioManager::sharedManager()->playBGMWithMap(musicId);
	}

	SceneLayer* pScene = GameManager::Get()->GetSceneLayer();
	if (pScene)
	{
		int tag = MainMenuLayer::getLayerTagByName("FirstStartGameAnimLayer");
		CCNode* pNode = pScene->getChildByTag(tag);
		if (pNode)
		{
			pNode->removeFromParent();
			pNode = NULL;
		}
	}

	this->removeFromParent();
}

void GameMovieLayer::InitLayer()
{
	TXGUI::UIManager *manager = TXGUI::UIManager::sharedManager();
	layout = manager->getUILayout("GameMovieLayer");
	if (layout == NULL)
	{
		layout = manager->CreateUILayoutFromFile("UIplist/startAnimation.plist", this, "GameMovieLayer");
		layout->setZOrder(100);
	}	

	skipAnimationBtn = layout->FindChildObjectByName<UIButton>("skipBtn");
	manager->RegisterMenuHandler("GameMovieLayer","GameMovieLayer::skipBtnClick",
		menu_selector(GameMovieLayer::skipBtnClick), this);
	skipAnimationBtn->setMenuInitPriority(KCCFirstGameAnimLayerControlPriority);

	EUpdateState = UPDATE_STATE_START_ANIM;
	StartPlayStartAnimation();
}

void GameMovieLayer::skipBtnClick(CCObject* sender)
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

	SceneLayer* pScene = GameManager::Get()->GetSceneLayer();
	if (pScene)
	{
		int tag = MainMenuLayer::getLayerTagByName("FirstStartGameAnimLayer");
		CCNode* pNode = pScene->getChildByTag(tag);
		if (pNode)
		{
			pNode->removeFromParent();
			pNode = NULL;
		}
	}

	GameAudioManager::sharedManager()->stopPlayBGM();
	SceneLayer* pSceneLayer = GameManager::Get()->GetSceneLayer();
	if (pSceneLayer)
	{
		int musicId = 0;
		if(MainLandManager::Get()->isMainLand(pSceneLayer->getCurMapId()))
		{
			musicId = MainLandManager::Get()->GetCityBGM(pSceneLayer->getCurMapId());
		}
		else
		{
			int instanceId = InstanceManager::Get()->getCurInstanceId();
			musicId = GameConfigFromLuaManager::Get()->getInstanceMusicId(instanceId,pSceneLayer->getCurMapId());
		}
		GameAudioManager::sharedManager()->playBGMWithMap(musicId);
	}

	this->removeFromParent();
}

void GameMovieLayer::UpdateStartAnimation(float fDelta)
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
					CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(GameMovieLayer::OneAnimationOver));
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
				CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(GameMovieLayer::OneLabelFadOutOver));
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

void GameMovieLayer::OneUpdateRcsAnimationOver()
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

void GameMovieLayer::OneAnimationOver()
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

void GameMovieLayer::OneLabelFadOutOver()
{
	if (displayLabels[LABEL_COUNTS-1].state == 2)
	{
		EndStartAnimation();
	}
}