// added by xiaoh at 2014/4/1
#ifndef _CALL_NATIVE_FUNC_MANAGER_H_
#define _CALL_NATIVE_FUNC_MANAGER_H_
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

enum NewLayerFlag{
	RANK_LIST_LAYER = 0,
    TREASURE_EXPLORE_LAYER = 1,
    ITEM_EXCHANGE_SHOP_LAYER = 2,
};

class CallNativeFuncManager : public CCObject
{
public:
	CallNativeFuncManager();
	virtual ~CallNativeFuncManager();

	static CallNativeFuncManager* getInstance();
	void AddIndexLayerToMainScene(int whatLayer);

    // index layer
	void AddRankListLayer();
    void RemoveRankListLayer();
    void ShowRankListLayer(bool flag);
    void ShowTreasureExploreLayer(bool flag,bool isPopLayer = true);
    void ShowItemExchangeShopLayer(bool flag);
    void ShowCombatSettingLayer(bool flag);
    
    // interface 
    void SetRankLayerVipFlgVisible(bool flag);
    void ShowWBCombatSettingTip(bool flag);
    bool isOwnGuildPlayer(int otherHighGid, int otherLowGid);
private:

};
#endif