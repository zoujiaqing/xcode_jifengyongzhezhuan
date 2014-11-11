#ifndef BACKPACK_LAYER_H
#define BACKPACK_LAYER_H

#include "ASprite.h"
#include "UILayout.h"
#include "ItemManager.h"
#include "UIScrollPage.h"
#include "DragReceiver.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UIButtonToggle.h"
#include "GameConfigFromLuaManager.h"
using namespace std;
using namespace cocos2d;

#define UI_BOARD "UI/ui_board.bin"
#define EACH_ROW_NUMBER 3
#define EACH_COLUMN_NUMBER 4
#define EACH_PAGE_NUMBER (EACH_ROW_NUMBER * EACH_COLUMN_NUMBER)

class SpriteSeer;
class ShopUILayer;

class BackPackLayer : public cocos2d::CCLayer,
					  public TXGUI::ObserverProtocol
{
public:
	BackPackLayer();
	virtual ~BackPackLayer();

	CREATE_FUNC(BackPackLayer);
	virtual bool init();
    
    CC_SYNTHESIZE(ShopUILayer*, m_pShopUILayer, pShopUILayer);
    
	void openBackPack(bool withEquipment);
    //void openEquipment();
    void openExchangeShop();
    
	void openBackIndex(CCObject* sender);
	void openEquipFragIndex(CCObject* sender);
    void openShop(CCObject* sender);
	//void openStoreHouse(CCObject* sender);
	void openAttributeBoard(CCObject* sender);
	void closeStorage(CCObject* sender);
	void closeBackPack(CCObject* sender);
    void closeEquipment(CCObject* sender);
	void closeAttributeBoard(CCObject* sender);
	void reOrderBackPackItems(CCObject* sender);
	void reOrderStorageItems(CCObject* sender);
	void onItemClicked(CCObject* sender);
	void onFragItemClicked(CCObject* sender);
	void onFairyListItemClicked(CCObject* sender);
	void onChangeFairyClicked(CCObject* sender);
	//void onItemDoubleClicked(CCObject* sender);
	void resetBackPackItemIcon(unsigned int index);
	void resetStorageItemIcon(unsigned int index);
	void resetItemIcon(unsigned int index);
	void setOneItemIcon(BackPackItem* item,unsigned int index);
	
	bool checkInShopUIPackBg(CCTouch *pTouch);
    bool checkInBackPackBg(CCTouch *pTouch);
	bool checkInStoragekBg(CCTouch *pTouch);
	bool checkInEquipmentBg(CCTouch *pTouch);
	virtual void setVisible(bool visible);
	void onBagButtonClicked();

	//卖背包物品回调
	void onItemSellCallBack(CCObject* sender);
	void onItemUseCallBack(CCObject* sender);
	void onChangeNameCallBack(CCObject* sender);
	void onItemMultiUseCallBack(CCObject* sender);
	void onItemMixCallBack(CCObject* sender);
	void onEquipmentItemClicked(CCObject* sender);
    void OnNPCBagClicked();
    void OnNPCBagMagicShopClicked();
	void onEquipTakeOffCallBack(CCObject* sender);
	void onEquipUpgradeCallBack(CCObject* sender);
	void onEquipCallBack(CCObject* sender);
	void onEquipBreakCallBack(CCObject* sender);
	void onGetWayCallBack(CCObject* sender);

	void onAlchemyLeftBtClicked(CCObject* sender);
	void onAlchemyRightBtClicked(CCObject* sender);
	void onAlchemyItemClicked(CCObject* sender);
	// 双击装备栏button
	//void onEquipmentItemDoubleClicked(CCObject* sender);
	//void onEquipmentTapped(CCObject* sender);
	//void onTapCancel(CCObject* sender);
	//void onItemTapped(CCObject* sender);

	// 解锁背包格弹框回调函数
	void onReceivedConfirmBagItemUnlock(CCObject* sender);
	void onReceivedCancellBagItemUnlock(CCObject* sender);

	void onReceivedEquipPageChanged();
	void setEquipPageInfo();

	void dragToEquipItem(unsigned int fromPos,unsigned int toPos);

	virtual void closeLayerCallBack(void);
	void onEquipAllBtClicked(CCObject* sender);
	void onFetterInfoClicked(CCObject* sender);
	//void onFairyInfoClicked(CCObject* sender);
	/**
	* Instruction : 点击特训按钮
	* @param 
	*/	
	//void TrainButtonClicked(CCObject* sender);
	/**
	* Instruction : 隐藏右侧的面板
	* @param 
	*/
	void HideRightPanel();
	/**
	* Instruction : 显示精灵潜能加强面板
	* @param 
	*/
	void ShowElfStrengthenPanel();
	/**
	* Instruction : 同时显示背包界面&精灵界面
	* @param 
	*/
	void ShowElfPanelWithBackPackLayer();
	int getCurrentEquipPage(){return m_currentEquipPage;}
	void onReceiveFairyChanged(unsigned int fairyPos);
	void setFairySelectStatus(bool isSelected){m_isFairySelectModle = isSelected;}

	//翻页某个scroll page 用于新手引导
	void ajustScrollPageByIndex(string pageName, int index);
protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	
	/// 消息响应函数
	virtual void onBroadcastMessage(BroadcastMessage* msg);
private:
	
	void setOneBackPackIcon(BackPackItem* item,unsigned int index);
	void setOneFragmentIcon(BackPackItem* item,unsigned int index);
	void setOneStorageIconn(BackPackItem* item,unsigned int index);
	void setEquipmentIcon(BackPackItem* item,unsigned int index);
	void setAlchemyIcon(BackPackItem* item,unsigned int index);
	CCSprite* getEquipEmptyIcon(unsigned int index);
	void setFriendlyFairyIcon(unsigned int index);
	CCSprite* getEquipmentUpgradeIcon(unsigned int ItemId);
	CCSprite* getIconSpriteById(unsigned int ItemId);
	CCSprite* getIconFrame(unsigned int level);
	TXGUI::UIButton* getEquipmentButton(int index);
	void initBgTouchArea();
	void initButtonPriority();
	void initAlchemyButton();
	void showBackPackContainer(bool isRetIcon = true);
	void hideAttributeBoard();
	unsigned int getFairyPosByListIndex(int index);

	void showPlayerAlchemy(bool isVisible);

	// 人物和精灵模型
	void checkPlayerModle();
	void showPlayerModle(UIScrollPage* modlePage);
	void showFairyModle(unsigned int fairyPos,unsigned int index);
	void showFairyFriend(UIScrollPage* modlePage,unsigned int index);
	void initEquipFairyList();
	void setFairyIcon(unsigned int index,unsigned int pos);
	void setFairyListIcon(unsigned int index,CCSprite* icon,unsigned int lockLevel = 0);
	void onReceiveFairySelectEnd();
	bool isFairyPosLocked(unsigned int pos);
	void setPlayerInfoVisible(bool isVisible);
	void setFairyInfoVisible(bool isVisible,bool isFightFairy,bool isLock,bool isEmpty);
	void setFairyFriendInfoVisible(bool isVisible);
	void setDefaultSelect();
	void resetFairyListPostion(int currentPage);
	void setSelectedFairyInfo(int pos,int fairyId);
	void showCurrentFairyFetter(int pos,int fairyId);

	void showPlayerLevel();
	void showPlayerVip();
	void showGuildName();
	void showPlayerEquipTitle();
	void showPlayerEffectiveness();
	void onRecieveFairyEffectiveness();
	void onReceiveFairyFetter();
	//void showPlayerExp();
	void showPlayerAttri();
	void showEquipmentTip(unsigned int pos,CCPoint pt);
	void showItemTip(unsigned int pos,CCPoint pt);
	void showCoinLabel();
	void showFragStoneLabel();
	void showEquipFragCanComposeIcon();
	void showBagNewEquipIcon();

	void checkLayerHide();
    void openCoreExchangeShop();
	
	void initLuaNextFrame(float dt);

	void createShopLayer();

	void reqToUnlockBagItem(unsigned int pos); // 请求解锁背包格
	void unselectAllBagItem(bool isBag); // 取消所有item的选中效果 isBag: true 背包格，false 仓库格
	void setBagIndex(bool flag); // flag: true 背包,false 碎片背包



private:

	unsigned int m_tipPos;
	ASprite*    m_curUiAs;
	TXGUI::UILayout *backPackLayout;
	bool b_isBackPackOpened;
	bool b_isStorageOpened;
    bool b_isEquipmentOpened;
	bool b_isAttributeOpened;
	string m_lockPicName;
	TXGUI::UIScrollPage * m_backPackScrollPage;
	TXGUI::UIScrollPage * m_storageScrollPage;
	TXGUI::UIScrollPage * m_fragScrollPage;
	TXGUI::UIContainer* m_backPackContainer;
	TXGUI::UIContainer* m_storageContainer;
    TXGUI::UIContainer* m_equipmentContainer;
	TXGUI::UIButtonToggle* m_bagIndexToggle;
	TXGUI::UIButtonToggle* m_fragIndexToggle;
	CCPoint		m_packbgPoint;
	CCSize		m_packbgSize;
	CCPoint		m_storagebgPoint;
	CCSize		m_storagebgSize;
	CCPoint		m_equipmentbgPoint;
	CCSize		m_equipmentbgSize;
    
    DragReceiver* m_shopUIReceiver;
	DragReceiver* m_backPackReceiver;
	DragReceiver* m_storageReceiver;
	DragReceiver* m_equipmentReceiver;
    
	TXGUI::UIButton* m_equipmentHelmetButton;
	TXGUI::UIButton* m_equipmentNecklaceButton;
	TXGUI::UIButton* m_equipmentWeaponButton;
	TXGUI::UIButton* m_equipmentArmorButton;
	TXGUI::UIButton* m_equipmentRingButton;
	TXGUI::UIButton* m_equipmentShoeButton;
	std::map<unsigned int,TXGUI::UIButton*>* m_fairyBtMap; 
	int m_lastClickedFairyIndex; // 上一次点击的精灵标签
	bool m_isFairySelectModle; // 是否正在选择精灵
	CCLayer* m_playModleLayer;
	CCLayer* m_elfModleLayer;
	//SpriteSeer* m_showModle;
	//SpriteSeer* m_elfModle;
	CCSprite* m_playerExpLable;
    int m_DagPos;
	int m_currentEquipPage;

	unsigned int m_unLockExchangeId; // 解锁exchangeId
	unsigned int m_showedVipLevel;
	bool b_isBagPageFlag; // true 背包, false 装备碎片背包
	bool b_isUnlockBagFlag; // true 解锁的背包格,false 解锁的是仓库格
	bool b_isInitModle;
};

#endif