//
//  ShopUILayer.h
//  HelloLua
//
//  Created by rickycheng  on 13-6-18.
//
//

#ifndef HelloLua_ShopUILayer_h
#define HelloLua_ShopUILayer_h

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"
#include "TimeProtocol.h"

using namespace std;
using namespace cocos2d;

typedef struct _shopItem
{
    int  to_id;
    int  to_num;
    int  from_id;
    int  from_num;
    bool isBuy;
    int  index;

}shopItem;

typedef enum  _ShopState
{
    kShop,
    kBuyBack,
    kExchange,
} ShopState;

class ShopUILayer : public CCLayer,
                    public TimeProtocol
                    //public TXGUI::ObserverProtocol
{
public:
	ShopUILayer();
	virtual ~ShopUILayer();
    
	CREATE_FUNC(ShopUILayer);
	virtual bool init();
    
public:
    void sendShopList();
    void showExchangeShopList();
    void setupShopList(std::vector<shopItem>& shoplist);
    void setupIconButtons();
    void setItemIcon(int item_num, int item_id, unsigned int item_index);
    
	virtual void setTouchEnabled(bool value);
    
    
    bool IsInShop(CCPoint location);
    void SendToBag(int DragPos);
    
    
    void floatCoin(int coinnum);
    void buyshop(int index, bool succ);
    
    void freshShopList();
    void reconstructCallBuyScrollist();
    
    virtual void onUpdateRemainTime(long remainTime);
    void initRemainTimer(long seconds);//server times, not remain time.
    
    void openShop();
    void closeShop();
    
    void updateCoinAndDiamand();
    void freshExchangeCoinVal();
    void OnClose(CCObject *sender);
	 CC_SYNTHESIZE(CCPoint, m_ShopTouchEndPt, ShopTouchEndPt)
    //virtual void onBroadcastMessage(BroadcastMessage* msg);

	void updateRefreshCardCount();
	void updateSoulStoneCount();
private:
    void createButtonNode(int item_id, int item_num, int from_id, int from_num, bool isbuy, int buyIndex,int btnIndex, bool shownum);
    void processBottomBoard(bool isvisible);
    void removeLabel(CCNode* sender);
    void setDiamandTxt();
    void setupNobuyBack(bool isvisible);
    void setupIconSide();
    
    void updateSpendDiamand();
    
    void setExchangeBoard();
    void setShopBoard();
    
    
    void OnDoubleClick(CCObject* sender);
    void OnFreshShoplist(CCObject* sender);
    void OnCallbackBuylist(CCObject* sender);
    
    void OnConfirm(CCObject *sender);
    void OnCancle(CCObject *sender);
    void onShopTip(CCObject* sender);
private:
    TXGUI::UILayout* m_pLayer;
    TXGUI::UILayout* m_pUseDiamand;
    std::vector<CCNode*> m_BtnList;

	std::vector<int> fromIdList;
    
    std::vector<int> m_VecIconIDs;
    std::vector<shopItem> m_veclist;
    UIScrollList* m_pShopList;    
    CCPoint m_DagPoint;
    ShopState m_shopSate;
    bool m_bInitRemainTime;
    bool m_bOpenDiamand;
    
    //int m_diamandMoney;
    //int m_coinMoney;
    int m_remainTime;
    
    std::map<unsigned int,unsigned int>* m_exchangeItemList;
    std::vector<unsigned int> m_pricelist;
    std::vector<unsigned int> m_shopPriceList;
	int refreshDiamond;
};

#endif
