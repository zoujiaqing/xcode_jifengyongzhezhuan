//
//  TalentLayer.h
//  HelloLua
//
//  Created by rickycheng  on 13-5-13.
//
//

#ifndef HelloLua_TalentLayer_h
#define HelloLua_TalentLayer_h

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

using namespace std;
using namespace cocos2d;

typedef struct _TalentQuery
{
    int talent_id;
    int level;
}TalentQuery;

class TalentLayer : public CCLayer
{
public:
	TalentLayer();
	virtual ~TalentLayer();
    
	CREATE_FUNC(TalentLayer);
	virtual bool init();
    
public:
	virtual void setTouchEnabled(bool value);
    void clearScrollist();
    void addNewTalent(int talent_id, int level, int add);
    
    void showOne(unsigned int talent_id,
                 unsigned int level,
                 unsigned int plus_add,
                 unsigned int next_exploit,
                 unsigned int next_time,
                 unsigned int diff_physical_attack,
                 unsigned int diff_magic_attack,
                 unsigned int diff_skill_attack,
                 unsigned int diff_physical_defence,
                 unsigned int diff_magic_defence,
                 unsigned int diff_skill_defence,
                 unsigned int diff_hp);
    
    virtual void closeLayerCallBack(void);
	bool IsOnCloseLayerCallBack(void){return m_bCloseLayerCallBack;}
    void startCD(int timeout);
    void updateLevel(int level, int plus, int next_exploit);
    void updateCoinAndDiamand();
    
    CC_SYNTHESIZE(int, m_musicTime, musicTime)
    CC_SYNTHESIZE(int, m_curTalentID, curTalentID)
    CC_SYNTHESIZE(int, m_curLevel, curLevel)
    CC_SYNTHESIZE(int, m_eachplus, eachplus)
    
protected:
	void OnClose(CCObject *sender);
    void OnClickListItem(CCObject* sender);
    void OnLevelUpTalent(CCObject* sender);
    
    void OnConfirm(CCObject *sender);
    void OnCancle(CCObject *sender);
    
private:
    void showOneTalent(int index, int talent_id, int headIconId, int roleLevel, int add, int attackLevel, int levelNeed, int GongXi);
    void showTalentDetail(bool visible);
    void setUpCDTime(const char* strtime);
    void updateCDTime(float dt);
    std::string getLocationTxt(int location_id);
    void setDiamandTxt();
    void convertSecToMinute(int time);
    void showSpriteParticleEffect(CCNode* parent,unsigned int id,CCPoint pt);
    
    int  talentIDToFrameId(int talent_id);
    void updateSpendDiamand();
    
protected:
	UIScrollList* m_pList;
	TXGUI::UILayout* m_pLayer;
    TXGUI::UILayout* m_pUseDiamand;
    TXGUI::UIButton* m_plevelupBtn;
    bool m_bTimeLevelUp;

    int m_goldCoinNum;
    int m_diamondNum;
    int m_gongxiNum;
    int m_talent_cnt;

    bool m_bCloseLayerCallBack;
    std::vector<TalentQuery> m_TalentQueries;
    std::vector<CCLabelTTF*> m_LevelLabels;
    std::vector<CCLabelTTF*> m_PlusLabels;
    std::vector<UIButton*> m_UIButtonlist;
    float m_dt;
    int m_cnt;

public:
	static long m_stopTimer;
    static bool m_TalentLevelupFlag;
};

#endif
