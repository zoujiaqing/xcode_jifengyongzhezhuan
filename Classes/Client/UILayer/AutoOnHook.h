//
//  AutoOnHook.h
//  HelloLua
//
//  Created by rickycheng  on 13-5-20.
//
//

#ifndef HelloLua_AutoOnHook_h
#define HelloLua_AutoOnHook_h

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

#include "TimeProtocol.h"
using namespace std;
using namespace cocos2d;
using namespace TXGUI;
extern bool g_rcv_battle_rsp;
extern unsigned int missing_ret;
extern unsigned int missing_instance_id ;
extern unsigned int missing_type;

typedef struct _OnHookA2
{
    int mon_id;
    int mon_num;
}OnHookA2;

typedef struct 
{
    unsigned int item_id;
    unsigned int item_level;
    unsigned int item_num;
}autoOnHookReward;
extern std::vector<autoOnHookReward> g_rewards;

class AutoOnHook :  public cocos2d::CCLayer,
								public TXGUI::ObserverProtocol,
								TimeProtocol
{
public:
	AutoOnHook();
	AutoOnHook(bool fromInstancePanel);
	void inithook();
	virtual ~AutoOnHook();
    
	//CREATE_FUNC(AutoOnHook);
	virtual bool init();
    
    
public:
	static void createAfterStartUp();
	virtual void setTouchEnabled(bool value);
    void startCD(int timeout);
	void progressDone(CCNode *sender);
	void ClearReward();
	void ShowReward();
	void SetReward(int item_num, int item_id, unsigned int item_level);
	void setScrollist(const std::vector<OnHookA2>& onHooksA2);

	std::vector<autoOnHookReward> m_rewards;
	CC_SYNTHESIZE(bool, m_isWaitingReward, IsWaitingReward);
	CC_SYNTHESIZE(bool, m_isProgressDone, IsProgressDone);
	CC_SYNTHESIZE(bool, m_isShownReward, IsShownReward);
    
    void sendCSABMonsterReq(unsigned int instance_id);
    void setItemIcon(int item_num, int item_id, unsigned int item_index);
    void setIconAllGet(int number_diamand, int number_coin, int number_exploit, int number_exp);

    void clearVecIDs();
    virtual void closeLayerCallBack(void);
	bool IsOnCloseLayerCallBack(void) {return m_bCloseLayerCallBack;}
    
    void setPictureAndWord(int instance_id);
    void hideCloseBtn();
    
    virtual void onUpdateRemainTime(long remainTime);
    
    CC_SYNTHESIZE(int, m_musicTime, musicTime)
	unsigned int getinstanceId(){return m_instanceId;}
	void setinstanceId(unsigned int s);
    //CC_SYNTHESIZE(unsigned int, m_instanceId, instanceId)
    INSTANCE_TYPE getInstanceType(){return instanceType;}
	void setInstanceType(int instanceId, INSTANCE_TYPE v);
	void setchapterList(std::map<int, ChapterInfo>* tiles);
	void setColdTime(long seconds);
	void MsgAutoBattleRsp(int ret);
//设置cd结束时间，首次创建Timer时使用
	void setCoolDownEndTime(long endTime);
	long getCoolDownEndTime(){return this->coolDownEndTime;}
	void initAutoOnHook(int instance_id, PB_INSTANCE_TYPE_T type);

	void RefreshTimesAndStamina(CCObject* obj);
	protected:

	/// 消息响应函数
	virtual void onBroadcastMessage(BroadcastMessage* msg);
	void sendAtrReq();
	void changeTime();
	
    // for stamina
    void enterBagLayer(CCObject* sender);
	void enterMallChipPage(CCObject* sender);
	void enterBuyStaminaPage(CCObject* sender);

private:

	int m_state;
	unsigned int m_instanceId;
	void initNormalTimer(long seconds);
	void initJingyingTimer(long seconds);
	void initUnderTimer(long seconds);

    void setFuBen(const char* fuBenName);
    void setFuBen(int frame, const char* fuBenName);
    void setupIconButtons();

    void OnClose(CCObject *sender);
    void OnSpeedUpHook(CCObject* sender);
    void OnStopHook(CCObject* sender);
    void OnStartAutoHook(CCObject* sender);
    void onTapCancel(CCObject* sender);
	void onItemTapped(CCObject* sender);
    void OnMasterClickListItem(CCObject* sender);
    
    void OnClickTurn1(CCObject* sender);
    void OnClickTurn2(CCObject* sender);
    void OnClickTurn3(CCObject* sender);
    
    void OnConfirm(CCObject *sender);
    void OnCancle(CCObject *sender);

    
    //void update(float dt);
    void setUpCDTime(const char* strtime);
    void setUpCDText(const char* strtxt);
    
    void initNormalFuBenUi(int instanceId);
    void processVisibleCD(bool visible);
    
    void showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt);
    void setIconAllGet(bool visible);
    
    void setDiamandTxt();
    void convertSecToMinute(int time);
    void showStartBtn(bool visible);
    void clearItemBtn();
    
    void freshBag();
    boolean updateSpendDiamand();
//common func
	void createLayout(int instanceId);
	
	TXGUI::UILayout* getCurLayout();
	void initCountDownTimerUi();
	void initCommonButtonUi();
	void initJingYingFuBenUi();
	void initUnderInstanceUI();
	void initProgressTimer();
	void setButtonState(int state);
	void removeLayout();
	void unregisterTime();

	
//jing ying fu ben func
	void onPressJYChapterBtn(CCObject *sender);
	void setchaptername(INSTANCE_TYPE type);
    void updateCoinAndDiamand(TXGUI::UILayout* player);
    //pu tong fu ben reset fun
    
	void OnResetTimes(CCObject* sender);
	void ShowConfirmBox();
	void ShowJYConfirmBox();
	void OnCancel(CCObject* sender);
protected:
	bool haveParentPanel;	
	//used for normal fuben
    UIScrollList* m_pListV;
    CCSprite* m_FuBenIcon;
    UILabelTTF* m_FuBenName;
    UILabelTTF* m_MasterFuBenName;
    
    UILabel* m_pLabelCDNumber;
    UILabel* m_pLabelCDText;
    UIPicture* m_pCDFrame;
	UIPicture* m_pProgressBg;
	UILabel* m_pLabelSweep;
	UILabel* m_pLabelCue;
	CCSprite* m_pProgressTimer;

    
    CCLayer* m_pIconButtonLayer;
    TXGUI::UILayout* instanceListLayout;
    
    std::vector<IconButton*> m_pIconButtonList;
    std::vector<int> m_VecIDs;
    
    float m_dt;
    int m_cnt;
    
    int m_turnId;
    
    UIButton* m_pCloseBtn;
    
    UIButton* m_pStartBtn;
    UIButton* m_pSpeedUpBtn;
    UIButton* m_pStopBtn;
    
    UILabel* m_pStartLabel;
    UILabel* m_pSpeedupLabel;
    UILabel* m_pStopLabel;

    UIButton* m_pTurnBtn1;
    UIButton* m_pTurnBtn2;
    UIButton* m_pTurnBtn3;
    
    UILabel* m_pTurnText1;
    UILabel* m_pTurnText2;
    
    UILabel* m_pTurnSami1;
    UILabel* m_pTurnSami2;
    UILabel* m_pTurnSami3;
    
    UIPicture* m_pTurnFrame;
    bool m_bCloseLayerCallBack;

	///////////////////////////////////////
	//used for jing ying fu ben
	
	TXGUI::UIScrollList* chapterList;
	TXGUI::UIPicture* chapterArrow;
    TXGUI::UILayout* jingYingLayout;
    bool m_bStopSel;
    int m_currentTag;
	int m_currentChapterId;
	TXGUI::UILabel * chapterLabel;
	std::map<int, TXGUI::UIButton *>m_buttonList;
	///////////////////////////////////////
	//used by all fu ben type.
	TXGUI::UILayout* m_pUseDiamand;
    int m_num_diamand;
    int m_num_coin;
    int m_num_exploit;
    int m_num_exp;
    
    std::map<int, int> m_itemMap;
    
	long m_remainTime; // 剩余时间
 	INSTANCE_TYPE instanceType;
	int m_initremainTime;
	long coolDownEndTime;

	//地下城副本
	TXGUI::UILayout* instanceUnderLayout;
};

#endif
