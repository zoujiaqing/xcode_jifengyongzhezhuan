#ifndef INSTANCELIST_LAYER_H
#define INSTANCELIST_LAYER_H

#include "ASprite.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "TutorialBase.h"

using namespace std;
using namespace cocos2d;

#define NORMAL_INSTANCE_MIN 0
#define NORMAL_INSTANCE_MAX 239999
#define JING_YING_FU_BEN_MIN 240000 //精英副本ID最小值
#define JING_YING_FU_BEN_MAX 249999//最大值
#define UNDERGROUND_INSTANCE_MIN 250000
#define UNDERGROUND_INSTANCE_MAX 259999
typedef struct InstanceInfo
{
	InstanceInfo()
	{
		id = 0;
		star = 0;
		can_enter_flag = 0;
		refresh_times = 0;
		isgray = false;
		battle_times = 0;
	}
	unsigned int id;// 副本ID
	unsigned int star;// 星星
	bool can_enter_flag;// 是否可进标识(true:可进 false:不可进) (对精英、地下城副本有效)
	unsigned int refresh_times;// 重置次数 (对精英、地下城副本有效)
	bool isgray;//当前是否是灰色
	unsigned int  battle_times; // 可以挂机次数, valid for normal instance
	
}InstanceInfo;

typedef struct ScrollPagePosition
{
	int index;
	int page;
	int row;
	int col;
}ScrollPagePosition;

enum INSTANCE_TYPE{
	INSTANCE_TYPE_NORMAL, 
	INSTANCE_TYPE_JINGYING,
	INSTANCE_TYPE_DUNGEON,
	INSTANCE_TYPE_GUILD,
	INSTANCE_TYPE_NOTHING,
};

class InstanceListLayer : public cocos2d::CCLayer
{
public:
	InstanceListLayer();
	virtual ~InstanceListLayer();

	CREATE_FUNC(InstanceListLayer);
	virtual bool init();

	static void showInstanceInfo(CCNode* parent,unsigned int instanceId,unsigned int instanceType,CCPoint pt);

	//打开关闭副本列表
	void openInstanceList();
	void closeInstanceList(CCObject* sender);
	void setInstanceId(unsigned int id){taskInstanceId = id;}

	//界面生成
	void refreshInstanceListLayer();
	void addInstancesFromList(const std::vector<InstanceInfo>& instanceList);
	void addSigleInstance(InstanceInfo info, ScrollPagePosition position);

	//按键响应
	void onPressInstanceBtn(CCObject *pObject);
	void clickAutoFight(CCObject *sender);
	void clickResetJingYingFuBen(CCObject *sender);
	
	//button call back 
    void OnAutoOnHook(CCObject* sender);
    void OnFight(CCObject* sender);
    void Fight(int id);
    
	//消息处理
	void onEventMsgInstanceList(const std::vector<InstanceInfo>& instanceInfos);
	void onExchangeParameterRsp(bool suc);
	void updateIconColor();
	
	//辅助计算函数
	ScrollPagePosition getScrollPagePos(int index);
	CCPoint scrollPosToPoint(ScrollPagePosition position);
	

	virtual void closeLayerCallBack(void);


	INSTANCE_TYPE getInstanceType(){return instanceType;}
	bool getShowInstance(){return showInstance;}//if user open the instance panel
	void setInstanceType(INSTANCE_TYPE v);
	//show the play times on jing ying fu ben panel
	void showJingYingPlayTimes(int);

    virtual void onExit();

    void OpenInstanceInfoLayer(InstanceInfo info, INSTANCE_TYPE type, int jingYingPlayTimes, bool tutorials = false);
    void closeInstanceInfoLayer();
private:
	ASprite*    m_curUiAs;
	TXGUI::UILabel* jingYingPlayTimesText;
	int jingYingPlayTimes;
	
	TXGUI::UILayout *instanceListLayout;	
	TXGUI::UILayout *jingYingLayout;	
	TXGUI::UILayout *instanceDungeonLayout;

    CC_SYNTHESIZE_RETAIN(CCLayer*, m_instanceInfoLayer, InstanceInfoLayer);

	unsigned int taskInstanceId;
    int instanceId;
	int page_reset;
	
 	INSTANCE_TYPE instanceType;
	bool showInstance;
	//create the uilayout
	void createLayout();
	void resetJingYingList(CCObject *sender);
	//void sendOnceDailyReq();
	void showCoinAndDiamond(TXGUI::UILayout * layout);	
	void showStarReward(CCObject* sender);
	int getCurChapterId();
	InstanceInfo * getCurInstanceWithBiggestRefrechTimes(int chapterid);
	int getResetTimes(int chapterId);
	
};

class NormalInstanceInfoUILayer;
class NormalInstanceInfoLayer :
      public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCNodeLoaderListener
	, public TXGUI::ObserverProtocol
{
public:
    NormalInstanceInfoLayer();
    virtual ~NormalInstanceInfoLayer();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(NormalInstanceInfoLayer, create);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode); 
    virtual void onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void onCloseLayer(CCObject* sender, CCControlEvent event);
    void onEnterInstance(CCObject* sender, CCControlEvent event);
    void onSelectAuto(CCObject* sender, CCControlEvent event);

    void onSelectItem1(CCObject* sender, CCControlEvent event);
    void onSelectItem2(CCObject* sender, CCControlEvent event);
    void onSelectItem3(CCObject* sender, CCControlEvent event);
    void onSelectItem4(CCObject* sender, CCControlEvent event);
    void onSelectItem5(CCObject* sender, CCControlEvent event);
    void onSelectItem6(CCObject* sender, CCControlEvent event);
    void onSelectItem7(CCObject* sender, CCControlEvent event);
    void onSelectItem8(CCObject* sender, CCControlEvent event);
    void onSelectItem9(CCObject* sender, CCControlEvent event);
    void onSelectItem10(CCObject* sender, CCControlEvent event);
    void onSelectItem11(CCObject* sender, CCControlEvent event);
    void onSelectItem12(CCObject* sender, CCControlEvent event);

	virtual void onBroadcastMessage(BroadcastMessage* msg);

    // for stamina
    void enterBagLayer(CCObject* sender);
	void enterMallChipPage(CCObject* sender);
	void enterBuyStaminaPage(CCObject* sender);

    void registerWithTouchDispatcher();
    virtual void onEnter();
    virtual void onExit();

    void RefreshTimes(CCObject* obj);

    void InitLayer(InstanceInfo info, INSTANCE_TYPE type, int jingYingPlayTimes, bool tutorials = false, bool isFromInstancePanel = true);

private :
    void ShowItemTip(int itemPos, CCNode* node);
	void onFairyDetailClicked(CCObject* sender);
	void onGetWayCallBack(CCObject* sender);
    void AddAnimation();
    CCSprite* createOneFlag(std::string bgName,std::string txtName);
	void ShowConfirmBox();
	void ShowJYConfirmBox();
	void OnResetTimes(CCObject* sender);
    void OnCancel(CCObject* sender);

private :
	CC_SYNTHESIZE(bool, m_isFromInstancePanel, IsFromInstancePanel);

    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_timesLabel,    TimesLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_powerLabel,    PowerLabel);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_bonusCash,     BonusCash);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_bonusHun,      BonusHun);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_currentCombat, CurrentCombat);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_desc1,         Desc1);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_desc2,         Desc2);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_desc3,         Desc3);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_desc4,         Desc4);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_labelLife,     LabelLife);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_labelCost,     LabelCost);

    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_bonusLayer1, BonusLayer1);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_bonusLayer2, BonusLayer2);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_bonusLayer3, BonusLayer3);
    CC_SYNTHESIZE_RETAIN(CCLayer*,    m_bonusLayer4, BonusLayer4);

    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_closeBtn, CloseBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_enterBtn, EnterBtn);
    CC_SYNTHESIZE_RETAIN(CCControlButton*, m_autoBtn,  AutoBtn);

    std::vector<CCSprite*> m_vtStars;
    std::vector<CCLayer*>  m_vtLayers;
    InstanceInfo m_instanceInfo;
    INSTANCE_TYPE m_instanceType;
    int m_jingYingPlayTimes;
	int m_fairyId;

    NormalInstanceInfoUILayer* m_uiLayer;
};

class NormalInstanceInfoUILayer :
      public UILayout
    , public UITutorialBase
{
public :
    NormalInstanceInfoUILayer(CCNode* parent);
    virtual ~NormalInstanceInfoUILayer();

    virtual CCNode* getTutorialNode(std::string name);

private :
    CC_SYNTHESIZE(NormalInstanceInfoLayer*, m_infoLayer, InfoLayer);
};

class NormalInstanceInfoLayerLoader : 
      public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(NormalInstanceInfoLayerLoader, loader);

protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(NormalInstanceInfoLayer);
};

#endif
