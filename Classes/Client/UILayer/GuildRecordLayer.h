#ifndef _GUILD_RECORD_LAYER_H_
#define _GUILD_RECORD_LAYER_H_

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "TXGUI.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace TXGUI;

class GuildRecordLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
{
public :
    GuildRecordLayer();
    virtual ~GuildRecordLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GuildRecordLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);

    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    virtual void onEnter();

    void onClose(CCObject* sender, CCControlEvent event);
    void onLeftClicked(CCObject* sender, CCControlEvent event);
    void onRightClicked(CCObject* sender, CCControlEvent event);

	void initRecordInfo(CCObject* obj);

	void showRecordInfo();

	std::vector<std::string> split(std::string str,std::string pattern);

	//virtual void onBroadcastMessage(BroadcastMessage* msg);

private :
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_titleLabel,       TitleLabel);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_leftButton,  LeftButton);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_rightButton, RightButton);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_pageLabel,        PageLabel);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_contentLayer,     ContentLayer);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeButton, CloseButton);

    int m_currentPage;
    int m_totalPage;
};

class GuildRecordLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GuildRecordLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GuildRecordLayer);
};

#endif