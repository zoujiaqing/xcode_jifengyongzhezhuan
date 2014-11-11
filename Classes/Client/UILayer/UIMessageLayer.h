#ifndef UI_MESSAGE_H
#define UI_MESSAGE_H

#include "cocos2d.h"
#include "UILabelTTF.h"
#include <list>
#include <vector>

using namespace cocos2d;
using namespace TXGUI;

#define KMessageDefaultColor ccc3(238,166,26)
typedef struct {
	std::string show_content ;
	ccColor3B   color ;

	unsigned int type ;


} SHOW_MESSAGE_ST;
class UIMessageLayer : public cocos2d::CCLayer
{
public: 
	UIMessageLayer();
	virtual ~UIMessageLayer();
	CREATE_FUNC(UIMessageLayer);
	virtual bool init();

	virtual void updateMessage(float fDelta);

	virtual void ShowMessage(const char* message,const ccColor3B& color ,unsigned int type = 0);
	void HideMessage(CCNode* sender);

	
private:
	void popMessage();
	CCSprite* createNewNode(const char* message,const ccColor3B& color,unsigned int type);
	
private:
	//UILabelTTF* m_labelTTF;
	CCPoint m_localStartPt;
	unsigned int m_labelNum;
	unsigned int m_childTag;
	//CCSprite* m_backPic;
	//CCSprite* m_pAnmationNode;

	std::list<SHOW_MESSAGE_ST > m_showMsgVec;
	std::map<unsigned int,CCNode*> m_labelMaps;
	
	//std::string m_showMsg;
	//ccColor3B m_msgColor;
};

#endif 