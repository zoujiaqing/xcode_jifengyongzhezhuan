#include "TutorialLayer.h"
#include "GameManager.h"
#include "SceneLayer.h"
#include "UIDefine.h"
#include "TXGUI.h"
#include "AspriteManager.h"
#include "GameFontManager.h"
#include "TXGUI.h"
#include "UILabelTTF.h"
#include "UIManager.h"
#include "TutorialBase.h"
#include "LuaTinkerManager.h"

//使用剪切图片的大小
#define CLIP_SIZE 100
#define CLIP_TRANSP 0
#define CLIP_SCALE 1.05

TutorialLayer::TutorialLayer()
	:m_pUISprite(NULL),
	m_pCurFlagSprite(NULL),
	m_pCurFlagSprite_2(NULL),
	m_pFlagParentNode(NULL),
	m_pFlagParentNode_2(NULL),
	m_pClipNode(NULL),
	m_pClipArea1(NULL),
	m_pClipArea2(NULL),
	m_pFingerNode(NULL),
	m_pCircleNode(NULL),
	m_pFingerPic(NULL),
	m_pCirclePic(NULL)
{
}

TutorialLayer::~TutorialLayer()
{
	this->stopAllActions();
}

bool TutorialLayer::init()
{
	bool bRet = false;
	do {
		//遮罩node
		m_pClipNode = CCClippingNode::create();
		m_pClipNode->setInverted(true);
		m_pClipNode->setAlphaThreshold(0);
		this->addChild(m_pClipNode, 5);

		CCLayerColor * layerColor = CCLayerColor::create(ccc4(0, 0, 0, CLIP_TRANSP));
		m_pClipNode->addChild(layerColor, 1);

		CCNode * nodef = CCNode::create();

		//使用drawNode有问题，改用sprite实现
		//m_pClipArea1 = CCSprite::create("UI/ui_leadpic.png");
		m_pClipArea1 = CCScale9Sprite::create("UI/ui_leadpic.png");
		m_pClipArea2 = CCScale9Sprite::create("UI/ui_leadpic.png");

		nodef->addChild(m_pClipArea1);
		nodef->addChild(m_pClipArea2);

		m_pClipNode->setStencil(nodef);

		// 第一个箭头
		m_pFlagParentNode = CCNode::create();
		this->addChild(m_pFlagParentNode,10);

		//第二个箭头
		m_pFlagParentNode_2 = CCNode::create();
		this->addChild(m_pFlagParentNode_2,10);

		//手指
		m_pFingerNode = CCNode::create();
		this->addChild(m_pFingerNode,11);

		m_pFingerPic = AspriteManager::getInstance()->getOneFrame(KUI_LEAD_BIN, "map_ui_lead_FRAME_BUTTON_LEAD_RIGHT");
		m_pFingerPic->setAnchorPoint(ccp(0.5, 0.5));
		m_pFingerPic->setScale(TXGUI::UIManager::sharedManager()->getScaleFactor());
		m_pFingerNode->addChild(m_pFingerPic);

		//手指移动 
		CCPoint desPos = ccp(20, -20);
		CCActionInterval* action = CCMoveBy::create(0.6,desPos);
		CCActionInterval* action_back = action->reverse();
		CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

		CCRepeatForever* pAction = CCRepeatForever::create(seq);
		m_pFingerPic->stopAllActions();
		m_pFingerPic->runAction(pAction);

		//圆圈
		m_pCircleNode = CCNode::create();
		this->addChild(m_pCircleNode,10);

		m_pCirclePic = AspriteManager::getInstance()->getOneFrame(KUI_LEAD_BIN, "map_ui_lead_FRAME_LEAD_CIRCLE");
		m_pCirclePic->setAnchorPoint(ccp(0.5, 0.5));
		m_pCirclePic->setScale(TXGUI::UIManager::sharedManager()->getScaleFactor());
		m_pCircleNode->addChild(m_pCirclePic);

		//圆圈缩放
		CCActionInterval* scale = CCScaleBy::create(0.8, 1.5, 1.5);
		CCActionInterval* scale_back = scale->reverse();

		CCActionInterval* scaleSeq = (CCActionInterval*)(CCSequence::create( scale, scale_back, NULL ));

		CCRepeatForever* pScaleAction = CCRepeatForever::create(scaleSeq);
		m_pCirclePic->runAction(pScaleAction);

		bool disableUITutorial = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "IsDisableUITutorial");
		if (false == disableUITutorial)
		{
			setTouchEnabled(true);
		}		
		bRet = true;
	}while (0);
	return true;
}

//void TutorialLayer::update(float dt)
//{
//}

CCSprite* TutorialLayer::createOneFlag(std::string bgName,std::string txtName)
{
	CCSprite* pSprite = AspriteManager::getInstance()->getOneFrame(KUI_LEAD_BIN,bgName.c_str());
	if (pSprite)
	{
		//m_pFlagParentNode->addChild(pSprite,1);
		pSprite->setVisible(false);
		pSprite->setAnchorPoint(ccp(0.5,0.5));

		CCPoint relativePoint;
		CCSprite* pTaskTextExpLabelSprite = AspriteManager::getInstance()->getFramesRelativePos(KUI_LEAD_BIN,bgName,txtName,ccp(0.5,0.5),relativePoint);
		CCSize labelSize = pTaskTextExpLabelSprite->getContentSize();
		TXGUI::UILabelTTF * taskTextExpLabel = TXGUI::UILabelTTF::create("", KJLinXin, 24,CCSize(labelSize.width,labelSize.height*1.5f),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
		CCSize  picSize = pSprite->getContentSize();
		taskTextExpLabel->setPosition(ccp(relativePoint.x+picSize.width/2,relativePoint.y+picSize.height/2));
		taskTextExpLabel->setColor(ccc3(0, 0, 0));
		pSprite->addChild(taskTextExpLabel,1,FLAG_SPRITE_LABEL_TAG);		
		pSprite->setScale(TXGUI::UIManager::sharedManager()->getScaleFactor());
	}
	return pSprite;
}

void TutorialLayer::ShowLayerCommonInit()
{
	if (m_pUISprite)
	{
		m_pUISprite->removeFromParentAndCleanup(true);
		m_pUISprite = NULL;
	}
}

void TutorialLayer::ShowLayer()
{
	ShowLayerCommonInit();

	this->setVisible(true);
}

//void TutorialLayer::ShowLayer(std::string uiName)
//{
//	ShowLayerCommonInit();
//
//	m_pUISprite = CCSprite::create(uiName.c_str());
//	if (m_pUISprite)
//	{
//		this->addChild(m_pUISprite,1);
//	}
//
//	this->setVisible(true);
//}

void TutorialLayer::HideAllFlagIcon()
{
	if (m_pCurFlagSprite)
	{
		m_pCurFlagSprite->removeFromParentAndCleanup(true);
		m_pCurFlagSprite = NULL;
	}

	if (m_pCurFlagSprite_2)
	{
		m_pCurFlagSprite_2->removeFromParentAndCleanup(true);
		m_pCurFlagSprite_2 = NULL;
	}

	if (m_pClipArea1)
	{
		m_pClipArea1->setPosition(ccp(-100, -100));
		m_pClipArea1->setPreferredSize(CCSize(100, 100));
	}

	if (m_pClipArea2)
	{
		m_pClipArea2->setPosition(ccp(-100, -100));
		m_pClipArea2->setPreferredSize(CCSize(100, 100));
	}

	if (m_pFingerNode)
	{
		m_pFingerNode->setPosition(ccp(-500, -500));
	}

	if (m_pCircleNode)
	{
		m_pCircleNode->setPosition(ccp(-500, -500));
	}

}

void TutorialLayer::ChangeFLagPos(float deltaX,float deltaY,unsigned int type /* = 0 */)
{
	
	if (0 == type)
	{
		//修正第一个箭头的位置
		if (m_pFlagParentNode)
		{
			CCPoint p = m_pFlagParentNode->getPosition();
			m_pFlagParentNode->setPosition(ccp(p.x+deltaX,p.y+deltaY));
			ChangeClipAreaPos(0, deltaX, deltaY);
			ChangeCirclePos(deltaX, deltaY);
		}
	}
	else if (1 == type)
	{
		//修正第二个箭头的位置
		if (m_pFlagParentNode_2)
		{
			CCPoint p = m_pFlagParentNode_2->getPosition();
			m_pFlagParentNode_2->setPosition(ccp(p.x+deltaX,p.y+deltaY));
			ChangeClipAreaPos(1, deltaX, deltaY);
		}
	}
}

void TutorialLayer::ShowFlagIconAtPos(CCPoint pt,const char* content /* = "" */,unsigned int type /* = 0 */)
{
	bool isFlipX = false;
	bool isFlipY = false;

	CCSprite* pTmpSprite = NULL;
	if (0 == type)
	{
		return ;
	}
	else if (1 == type)
	{
		pTmpSprite = createOneFlag("map_ui_lead_FRAME_LEAD_TALK1","map_ui_lead_FRAME_LEAD_TALK1_TEXT");
	}
	else if (2 == type)
	{
		isFlipX = true;
		pTmpSprite = createOneFlag("map_ui_lead_FRAME_LEAD_TALK2","map_ui_lead_FRAME_LEAD_TALK2_TEXT");
	}
	else if (3 == type)
	{
		pTmpSprite = createOneFlag("map_ui_lead_FRAME_LEAD_TALK3","map_ui_lead_FRAME_LEAD_TALK3_TEXT");
	}
	else if (4 == type)
	{
		isFlipY = true;
		pTmpSprite = createOneFlag("map_ui_lead_FRAME_LEAD_TALK4","map_ui_lead_FRAME_LEAD_TALK4_TEXT");
	}

	CCNode* pParentNode = NULL;
	if (pTmpSprite)
	{
		if (m_pCurFlagSprite == NULL)
		{
			m_pCurFlagSprite = pTmpSprite;
			pParentNode = m_pFlagParentNode;
			m_pFlagParentNode->addChild(pTmpSprite);
		}
		else
		{
			m_pCurFlagSprite_2 = pTmpSprite;
			pParentNode = m_pFlagParentNode_2;
			m_pFlagParentNode_2->addChild(pTmpSprite);
		}
	}

	if (pTmpSprite)
	{
		pTmpSprite->setVisible(true);

		CCSize size = pTmpSprite->getContentSize();
		float ptX = 0;
		float ptY = 0;
		ptX = pt.x;
		ptY = pt.y;

		//去掉这段，始终以中心为锚点

		//if (1 == type || 2 == type)
		//{
		//	if (false == isFlipX)
		//	{
		//		//ptX = ptX - size.width/2;
		//		pTmpSprite->setAnchorPoint(ccp(1,0.5));
		//	}
		//	else
		//	{
		//		//ptX = ptX + size.width/2;
		//		pTmpSprite->setAnchorPoint(ccp(0,0.5));
		//	}
		//}

		//if (3 == type || 4 == type)
		//{
		//	if (false == isFlipY)
		//	{
		//		//ptY = ptY + size.height/2;
		//		pTmpSprite->setAnchorPoint(ccp(0.5,0.0));
		//	}
		//	else
		//	{
		//		//ptY = ptY - size.height/2;
		//		pTmpSprite->setAnchorPoint(ccp(0.5,1.0));
		//	}
		//}

		pParentNode->setPosition(ccp(ptX,ptY));

		//float deltaDst = -20;
		//if (1 == type || 2 == type)
		//{
		//	if (isFlipX)
		//	{
		//		deltaDst = -deltaDst;
		//	}
		//}

		//if (3 == type || 4 == type)
		//{
		//	if (false == isFlipY)
		//	{
		//		deltaDst = -deltaDst;
		//	}
		//}

		//deltaDst = deltaDst * TXGUI::UIManager::sharedManager()->getScaleFactor();

		//pTmpSprite->stopAllActions();
		//{
		//	CCActionInterval* action = NULL;
		//	if (type == 1 || type == 2)
		//	{
		//		action = CCMoveBy::create(0.8,ccp(deltaDst,0));
		//	}
		//	else if (type == 3 || type == 4)
		//	{
		//		action = CCMoveBy::create(0.8,ccp(0,deltaDst));
		//	}

		//	CCActionInterval* action_back = action->reverse();
		//	CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( action, action_back, NULL ));

		//	CCRepeatForever* pAction = CCRepeatForever::create(seq);
		//	//pTmpSprite->runAction( pAction );
		//}

		TXGUI::UILabelTTF* pLabelNode = dynamic_cast<TXGUI::UILabelTTF*>(pTmpSprite->getChildByTag(FLAG_SPRITE_LABEL_TAG));
		if (pLabelNode)
		{
			pLabelNode->setString(content);
		}
	}
}

void TutorialLayer::SetClipAreaPos(int index, CCPoint pos, CCSize size)
{
	float scale = TXGUI::UIManager::sharedManager()->getScaleFactor();
	if (0 ==  index && m_pClipArea1)
	{
		m_pClipArea1->setPosition(pos);
		m_pClipArea1->setPreferredSize(CCSize(size.width * scale * CLIP_SCALE, size.height * scale * CLIP_SCALE));
	}
	else if (1 == index && m_pClipArea2)
	{
		m_pClipArea2->setPosition(pos);
		m_pClipArea2->setPreferredSize(CCSize(size.width * scale * CLIP_SCALE, size.height * scale * CLIP_SCALE));
	}
}

void TutorialLayer::ChangeClipAreaPos(int index, float deltaX, float deltaY)
{
	if (0 ==  index && m_pClipArea1)
	{
		CCPoint oldPos = m_pClipArea1->getPosition();
		m_pClipArea1->setPosition(ccp(oldPos.x + deltaX, oldPos.y + deltaY));
	}	
	else if (1 == index && m_pClipArea2)
	{
		CCPoint oldPos = m_pClipArea2->getPosition();
		m_pClipArea2->setPosition(ccp(oldPos.x + deltaX, oldPos.y + deltaY));
	}
}

void TutorialLayer::SetCirclePos(CCPoint pos)
{
	CCPoint deltaPos = ccp(20, -20);
	if (m_pFingerNode)
	{
		m_pFingerNode->setPosition(ccpAdd(pos, deltaPos));
	}

	if (m_pCircleNode)
	{
		m_pCircleNode->setPosition(pos);
	}

}

void TutorialLayer::ChangeCirclePos(float deltaX, float deltaY)
{
	if (m_pFingerNode)
	{
		CCPoint oldPos = m_pFingerNode->getPosition();
		m_pFingerNode->setPosition(ccp(oldPos.x + deltaX, oldPos.y + deltaY));
	}

	if (m_pCircleNode)
	{
		CCPoint oldPos = m_pCircleNode->getPosition();
		m_pCircleNode->setPosition(ccp(oldPos.x + deltaX, oldPos.y + deltaY));
	}
}


bool TutorialLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isVisible())
	{
		return true;
	}

	return false;
}

void TutorialLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}

void TutorialLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchEnded(pTouch,pEvent);
}

void TutorialLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}

void TutorialLayer::registerWithTouchDispatcher()
{
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, KCCLayerTutorialPripority, true);
	}
}

void TutorialLayer::ShowLayerActorOrNot(bool bShow)
{
	if (m_pCurFlagSprite)
	{
		m_pCurFlagSprite->setVisible(bShow);
	}

	if (m_pCurFlagSprite_2)
	{
		m_pCurFlagSprite_2->setVisible(bShow);
	}
	
	if (m_pUISprite)
	{
		m_pUISprite->setVisible(bShow);
	}

	if (m_pClipNode)
	{
		m_pClipNode->setVisible(bShow);
	}
}