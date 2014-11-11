#include "UIMessageLayer.h"
#include "GameFontManager.h"
#include "AspriteManager.h"
#include "ASprite.h"
#include "TXGUIHeader.h"
#include "UIManager.h"

#define KMessageBackPictureName "map_ui_FRAME_MSG_BAR"
#define KMessageBackPictureStreg "map_ui_FRAME_FRAME_STRENGTH_CHANGE"
UIMessageLayer::UIMessageLayer()
{
	m_labelNum = 0;
	m_childTag = 0;
}

UIMessageLayer::~UIMessageLayer()
{
	m_showMsgVec.clear();
	unschedule(schedule_selector(UIMessageLayer::updateMessage));
}

bool UIMessageLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	m_localStartPt = ccp(visibleSize.width *0.5,visibleSize.height * 0.6);

	this->schedule(schedule_selector(UIMessageLayer::updateMessage),0.2f);
	return true;
}

void UIMessageLayer::updateMessage(float fDelta)
{

	if(m_showMsgVec.size() > 0)
	{
		popMessage();
	}
}

void UIMessageLayer::popMessage()
{
	if(m_showMsgVec.size()>0)
	{
		std::list<SHOW_MESSAGE_ST>::iterator it = m_showMsgVec.begin();
		SHOW_MESSAGE_ST msg = (*it);
		string msgText = msg.show_content;
		ccColor3B msgColor = msg.color;
		unsigned int type = msg.type ;
		setVisible(true);

		CCSprite* node = createNewNode(msgText.c_str(),msgColor,type );
		node->setPosition(m_localStartPt);
		node->setOpacity(255);
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCMoveBy* actionMove1 = CCMoveBy::create(1.0f,ccp(0,visibleSize.height * 0.15f));
		CCActionInterval* actionMove2 = CCMoveBy::create(0.6f,ccp(0,visibleSize.height * 0.2f));
		CCActionInterval* actionFade = CCFadeOut::create(0.6f);
		//CCSpawn* SecondAction = CCSpawn::createWithTwoActions(actionMove2,actionFade);
		node->runAction(CCSequence::create(actionMove1,CCDelayTime::create(0.4f),
			CCCallFuncN::create(this,callfuncN_selector(UIMessageLayer::HideMessage)),NULL));


		m_showMsgVec.pop_front();
	}
	
}

void UIMessageLayer::ShowMessage(const char* message,const ccColor3B& color ,unsigned int type )
{
	SHOW_MESSAGE_ST  st_Message ;
	st_Message.show_content= std::string(message);
	st_Message.color = color;
	st_Message.type = type ;
	m_showMsgVec.push_back(st_Message);
	
}

 void UIMessageLayer::HideMessage(CCNode* node)
 {
	 if(m_labelMaps.find(node->getTag()) != m_labelMaps.end())
	 {
		 m_labelNum--;
		 m_labelMaps.erase(node->getTag());
		 node->removeFromParentAndCleanup(true);
		 node = NULL;

		 if(m_labelNum == 0)
		 {
			 setVisible(false);
		 }
	 }
 }

 CCSprite* UIMessageLayer::createNewNode(const char* message,const ccColor3B& color ,unsigned int type)
 {
	 float scale = UIManager::sharedManager()->getScaleFactor();

	 CCSprite* pAnmationNode = CCSprite::create();
	 pAnmationNode->setPosition(CCPointZero);
	 pAnmationNode->setOpacityModifyRGB(false);
	 pAnmationNode->setScale(scale);
	 addChild(pAnmationNode,m_childTag,m_childTag);
	 
	 int fontSize = 26;
	 if(type == 1)
		 fontSize = 40 ;
	 UILabelTTF* mlabelTTF = UILabelTTF::create(message, KJLinXin, fontSize);
	 mlabelTTF->setColor(color);

	 ASprite* as = AspriteManager::getInstance()->getAsprite(KUI_BIN); 
	 if(type == 0)
	 {
		 if(as)
		 {
			 CCPoint pt;
			 int iSpriteFrame = getResourceIDByName(KMessageBackPictureName);
			 CCSprite* mbackPic = as->getSpriteFromFrame_Middle(iSpriteFrame, 0, pt);
			 if(mbackPic)
			 {
				 mbackPic->setAnchorPoint(ccp(0.5,0.5));
				 pAnmationNode->addChild(mbackPic);
				 mbackPic->setPosition(CCPointZero);
			 }
		 }
		 pAnmationNode->addChild(mlabelTTF);
		 mlabelTTF->setPosition(CCPointZero);

	 }
	 else if(type == 1)
	 {
		 if(as)
		 {
			 CCPoint pt;
			 int iSpriteFrame = getResourceIDByName(KMessageBackPictureStreg);
			 CCSprite* mbackPic = as->getSpriteFromFrame_Middle(iSpriteFrame, 0, pt);
			 if(mbackPic)
			 {
				 mbackPic->setAnchorPoint(ccp(0.5,0.5));
				 pAnmationNode->addChild(mbackPic);
				 mbackPic->setPosition(CCPointZero);
			 }
		 }
		 pAnmationNode->addChild(mlabelTTF);
		 mlabelTTF->setAnchorPoint(ccp(0.3,0.6));
		 


	 }

	 m_labelNum++;
	 m_labelMaps.insert(make_pair(m_childTag,pAnmationNode));
	 m_childTag++;
	 return pAnmationNode;
 }