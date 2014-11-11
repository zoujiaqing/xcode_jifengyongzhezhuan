
#pragma once

#include "PPVEMessage.h"

#include "AspriteManager.h"
#include "cocos2d.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

using namespace std;
using namespace cocos2d;

class PVPLayer : public CCLayer
{    
public:
	PVPLayer();
	virtual ~PVPLayer();
    
	CREATE_FUNC(PVPLayer);
	virtual bool init();
    
    bool   AddNewRoom(const PVPRoomBase& room);
	void   ClearAllRooms();
    
public:
	virtual void setTouchEnabled(bool value);
    virtual void closeLayerCallBack(void);
	bool IsOnCloseLayerCallBack(void){return m_bCloseLayerCallBack;}
    
    CC_SYNTHESIZE(bool, m_b1vs1, b1vs1)
    CC_SYNTHESIZE(bool, m_b2vs2, b2vs2)
    CC_SYNTHESIZE(bool, m_bprotect, bprotect)
    CC_SYNTHESIZE(bool, m_bfree, bfree)
    CC_SYNTHESIZE(bool, m_sp, sp)
    CC_SYNTHESIZE(bool, m_sf, sf)

    
protected:
	/// callback of close button
	void   OnClose(CCObject *sender);
	void   OnClickListItem(CCObject* sender);
    
    void   onSel_1vs1(CCObject *sender);
    void   onSel_2vs2(CCObject *sender);
    void   onSel_protect(CCObject* sender);
    void   onSel_free(CCObject* sender);
    
protected:
    
	///room list
	UIScrollList* m_pList;
	TXGUI::UILayout* m_pLayer;
    
	/// all the maps
	MapInfoVector  m_vecMaps;
    
    UIButtonToggle* m_btn1vs1;
    UIButtonToggle* m_btn2vs2;
    UIButtonToggle* m_btnProtect;
    UIButtonToggle* m_btnFree;
    
    bool m_bCloseLayerCallBack;
    
};




