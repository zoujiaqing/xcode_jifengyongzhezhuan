#ifndef _ASSIST_FIGHT_ELF_PANEL_H_
#define _ASSIST_FIGHT_ELF_PANEL_H_

#include "cocos2d.h"
#include "TXGUI.h"
#include "UIPicture.h"
#include "UILayout.h"

USING_NS_CC;
using namespace TXGUI;

class ElfAssistSkillObserver
{
public:
	ElfAssistSkillObserver();
	virtual ~ElfAssistSkillObserver();

	virtual void OnSkillTrigger(unsigned int pos,unsigned int type) = 0;
};

class AssistElfFightPanel : public CCNode, public ElfAssistSkillObserver
{
public:
	enum EAssistElfSkillType
	{
		E_Type_Attack,
		E_Type_Defend,
		E_Type_Hp
	};
public:
	AssistElfFightPanel();
	virtual ~AssistElfFightPanel();

	virtual void init();
	static CCNode * create(CCLayer* pBattleLayer,TXGUI::UILayout* pLayout);
	virtual void setVisible(bool visible);

	void EquipElfAtPos(unsigned int pos,unsigned int elfId);
	void EquipElfPanel();

	void OnSkillTrigger(unsigned int pos,unsigned int type);
	void CallBackSkillEffect1();
    void CallBackSkillEffect2();
private:
	void loadNeedAnimationRcs();
	unsigned int getIconEffectAnimationId(unsigned int type);
	void showQualityFrame(unsigned int pos,unsigned int quality);
	const char* getQualityFrameNameByQuality(unsigned int quality);
private:
	TXGUI::UIPicture* m_panelBgPic;
	TXGUI::UIPicture* m_leftAssistElfIcon;
	TXGUI::UIPicture* m_rightAssistElfIcon;
	TXGUI::UILayout* m_layout;
	CCLayer* m_battleUILayer;
	CCNode* parentNodeEffect1 ;
	unsigned int effectAnimId1 ;
    CCNode* parentNodeEffect2 ;
	unsigned int effectAnimId2 ;
	static const unsigned int EFFECT_ICON_TAG =	1000;
	static const unsigned int QUALITY_ICON_TAG = 1001;
	static const unsigned int EFFECT_ANIM_TAG = 1002;
};

#endif	//_ASSIST_FIGHT_ELF_PANEL_H_