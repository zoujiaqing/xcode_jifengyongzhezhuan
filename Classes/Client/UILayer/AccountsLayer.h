//
//  AccountsLayer.h
//  HelloLua
//
//  Created by rickycheng  on 13-7-2.
//
//

#ifndef HelloLua_AccountsLayer_h
#define HelloLua_AccountsLayer_h

#include "cocos2d.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

typedef struct _accountReward
{
    unsigned int item_id;
    unsigned int item_level;
    unsigned int item_num;
    
} accountReward;

typedef struct _accountRewardDetailData
{
	bool isPveMode;
	bool isPAIvEMode;
	int star;
	int killtime;
	int killnum;
	int killcombo;
	int add_coin;
	int add_diamond;
	int add_exp;
	int add_exploit;
	int add_contributions;
	std::vector<accountReward> vec;

	void ResetData()
	{
		isPveMode = false;
		isPAIvEMode = false;
		star = 0;
		killtime = 0;
		killnum = 0;
		killcombo = 0;
		add_coin = 0;
		add_diamond = 0;
		add_exp = 0;
		add_exploit = 0;
		add_contributions = 0;
		vec.clear();
	}
	_accountRewardDetailData()
	{
		ResetData();
	}
} accountRewardDetailData;

class AccountsLayer : public CCLayer
{
public:
	AccountsLayer();
	virtual ~AccountsLayer();
    
	CREATE_FUNC(AccountsLayer);
    
	virtual bool init();
    
    void accountBattle(unsigned int star,
                       unsigned int killtime,
                       unsigned int killnum,
                       unsigned int killcombo,
                       unsigned int add_coin, unsigned int add_diomand, unsigned int add_exp, unsigned int add_exploit, unsigned int add_contributions,
                       const std::vector<accountReward>& rewards);
    
    void accountWinPvP(unsigned int add_coin, unsigned int add_diomand, unsigned int add_exp, unsigned int add_exploit,
					const std::vector<accountReward>& rewards);

	void accountWinPvP2(unsigned int bcoin, 
		unsigned int bpvp2_score, unsigned int ccoin, 
		unsigned int cpvp2_score
		);
    void openWinBattle();
    void openFailureBattleTimeOut();
    void openFailureBattleDie(unsigned int index);
    void openWinPK();
	void openFailurePK();
    void openFailurePVP();
    void openWinPVP();
	
	
    void openWinPVP2();
    void closeAccountsLayer();
	void setRandItem( unsigned int item_id ,unsigned int num);

	void DoRevival();
    
    void setBarValue(int attack, int defence, int live);

	static accountRewardDetailData s_accountDetailData;

    // for stamina
    void enterBagLayer(CCObject* sender);
	void enterMallChipPage(CCObject* sender);
	void enterBuyStaminaPage(CCObject* sender);

protected:
	void OnBack(CCObject *sender);
    void onOnceMore(CCObject* sender);
    void OnLive(CCObject* sender);
	void OnNext(CCObject* sender);
    void OnPvpItemClicked(CCObject* sender);

	void onGetFairy(CCObject *sender);
	void onTrainFairy(CCObject *sender);
	void onStrenthEquip(CCObject *sender);
	void onBuyDiamond(CCObject *sender);

    void doOne();
    void one_1();
    void one_2();
    void one_3();
    void one_4();
    void star_1();
    void star_2();
    void star_3();
    void basereWard();
    
    void specialReWard1();
    void specialReWard2();
    void specialReWard3();
    void specialReWard4();
    
    void showBackBtn();
    void labelScale(CCLabelTTF* label, float startscael, float endscale);
    void nodeScale(CCNode* node, float startscale, float endscale);
    void setSpecialReWardSprite(UIPicture* pUIPicture, int index);
    
    //void openFailureBattleTimeOut_core();
    //void openFailureBattleDie_core();
	void openFailureBattle_core();
    void openFailurePVP_core();
    void openWinPVP_core();
	void openFailurePK_core();
	void openWinPK_core() ;
	void openWinPVP2_core();

    void playWinSound();
    
    void playWinBattleWav();
    void playWinWav();
    void playFailureWav();
    
    void pvpCoinEffect();
    
	void getDiamondAndRevivalNum(unsigned int index);
    
    ccColor3B checkBarColor(int percent);
    const char* checkBarName(int percent);
    const char* checkBarFrame(int percent);
	void onItemTapped(CCObject* sender);
private:
	int userChoosenCardId;
	void picCardHalfCallback(CCNode*);
	void picCardDoneCallback(CCNode*);

	void addCardPic(UIPicture *parent, int cardId);
	void OnCardClick(CCObject *sender);
	
	void switchItemReward(int toIndex);
	void onWinPkAddIcon(CCNode* parent, int item_id,
		int item_num, int index);
	
	void flipTheCard(int cardId, bool needSwitch = false);
	
	void openWinPK_core_step2();
	void resetButtonPic(const char* buttonName);
	
	void picCardBeat(CCNode* sender);
protected:
	TXGUI::UILayout* m_pLayer;
    
    UIPicture* winBackGround;
    UIPicture* winDecoration;
	UIPicture* winDecorationCityDefence;
    UIPicture* win_icon_star1;
    UIPicture* win_icon_star2;
    UIPicture* win_icon_star3;
    
    UILabel* win_text_estimate;
    
    UILabel* win_text_finishtime;
    UILabel* win_number_finishtime;
    
    UILabel* win_text_killnumber;
    UILabel* win_number_killnumber;
    
    UILabel* win_text_combo;
    UILabel* win_number_combo;
    
    UILabel* win_text_basicReward;
    
    UIPicture* win_icon_reward1;
    UILabel* win_number_reward1;
    
    UIPicture* win_icon_reward2;
    UILabel* win_number_reward2;
    
    UIPicture* win_icon_reward3;
    UILabel* win_number_reward3;
    
    UIPicture* win_icon_reward4;
    UILabel* win_number_reward4;
    
    UILabel* win_text_specialReward;
    
    UIPicture* win_frame_specialReward1;
    UIPicture* win_frame_icon_specialReward1;
    
    UIPicture* win_frame_specialReward2;
    UIPicture* win_frame_icon_specialReward2;
    
    UIPicture* win_frame_specialReward3;
    UIPicture* win_frame_icon_specialReward3;
    
    UIPicture* win_frame_specialReward4;
    UIPicture* win_frame_icon_specialReward4;
    
    UILabel* win_text_backToTown;
	UILabel* win_text_leftBackToTown;
	UILabel* win_text_next;
    UILabel* win_text_onceMore;
	
    UIButton* win_onback;
	UIButton* win_onleftback;
	UIButton* win_onnext;
    UIButton* win_onceMore;
    
    int m_star;
    
    int m_killtime;
    int m_killnum;
    int m_killcombo;
    
    int add_coin;
    int add_domiand;
    int add_exp;
	int add_exploit;
	int add_contributions;
   
    int m_item1_id;
    int m_item1_num;
    
    int m_item2_id;
    int m_item2_num;
    
    int m_item3_id;
    int m_item3_num;
    
    int m_item4_id;
    int m_item4_num;
    
    int m_itemcnt;

	std::string m_revivalDiamond;
	std::string m_revivalNum;

	//variable for BestFighterOverReward
	int bcoin         ;    // pvp2后奖励的基础金币    
	int bpvp2_score   ;    // pvp2后基础积分奖励    
	int ccoin         ;  // pvp2后奖励的连胜金币    
	int cpvp2_score   ;    // pvp2后连胜积分奖励
};

#endif
