//
//  Notice1UILayer.h
//  HelloLua
//
//  Created by rickycheng  on 13-10-23.
//
//

#ifndef HelloLua_Notice1UILayer_h
#define HelloLua_Notice1UILayer_h


#include "cocos2d.h"
#include "AspriteManager.h"
#include "UIButton.h"
#include "UILayout.h"
#include "TXGUIHeader.h"
#include "InstanceManager.h"

#include "UpdateResourceDelegate.h"


using namespace std;
using namespace cocos2d;

class AssetsManager;

class Notice1UILayer : public CCLayer, public NetworkOperationDelegate
{
public:
	Notice1UILayer();
	virtual ~Notice1UILayer();
    
	CREATE_FUNC(Notice1UILayer);
	virtual bool init();
    
	virtual void setTouchEnabled(bool value);
    
    void requestHttp();
    void insert(int r, int g, int b, int fontsize, const std::string& context);
    
    virtual void operationDidFinish(AssetsManager *operation, const char* strVal);
    virtual void operationDidFail(AssetsManager *operation);
    
protected:
	void OnClose(CCObject *sender);
    void loadPlist();
    void transScene(float dt);
    void gotoLayer(float dt);
    
protected:
	UIScrollList* m_pList;
	TXGUI::UILayout* m_pLayer;
    AssetsManager *m_pAssetsManager;
};

#endif
