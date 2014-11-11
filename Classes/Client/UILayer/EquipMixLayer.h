#ifndef EQUIP_MIX_LAYER_H
#define EQUIP_MIX_LAYER_H

#include "TXGUIHeader.h"
#include "cocos2d.h"
#include "ItemManager.h"
using namespace TXGUI;
using namespace cocos2d;
class EquipMixLayer : public cocos2d::CCLayer
{
public:
	EquipMixLayer();
	virtual ~EquipMixLayer();

	CREATE_FUNC(EquipMixLayer);
	virtual bool init();

	void ShowMaterials(ExchangeItem* item,unsigned int pos);
	void onClickClosed(CCObject* sender);
	void onComposeClickd(CCObject* sender);
	void onMatarailIconClicked(CCObject* sender);

	virtual void closeLayerCallBack(void);
	void onReceivedComposeSuccess();
private:
	void showMixedEquipIcon(unsigned int itemId);
	void showMatarial(unsigned int itemId,unsigned int ownerNum,unsigned int requiredNum,int index);
	void showMatarialBt(int index,int itemId);

	const char* getMatarialLabelName(int index);
	const char* getMatarialNumberLabelName(int index);
	const char* getMatarialPictureName(int index);
	CCSprite* getIconSpriteById(unsigned int ItemId);
	void setMatarailIcon(unsigned int itemId,const char* name);
	void setMatarialLabel(const char* text, const char* label,const ccColor3B& color = ccWHITE);
	CCSprite* getIconFrame(unsigned int level);

	void readyToClose(float dt);

	// 根据材料查询掉落副本
	int getInstanceIdByDropMatarial(int matarial); 

	// 自动寻路
	void autoSearchPathToInstance(int instanceId);
private:
	UILayout* m_uiLayout;
	unsigned int m_itemIndex;
	unsigned int m_itemPos;
	bool	b_isMaterialEnough;
	bool	b_successCompose;
};

#endif