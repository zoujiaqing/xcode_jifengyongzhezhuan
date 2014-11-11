#ifndef PKLAYER_H
#define PKLAYER_H

#include "cocos2d.h"
#include "TXGUIHeader.h"
#include "cs_basic.pb.h"
#include <vector>
#include <map>
#include "TimeProtocol.h"
using namespace std;
using namespace cocos2d;

typedef struct PVPLearnInfo
{
	int uid;
	int reg_tm ;
	string playName;
	int channel_id ;
	int effectiveness ;
	int type ;
	bool mandatory ;
    int status ;
}PVPLearnInfo;

class PVPLearnLayer :
	public cocos2d::CCLayer ,//, 
	TimeProtocol
	//public ISocketEvent
{
public:
	PVPLearnLayer();
	virtual ~PVPLearnLayer();

	CREATE_FUNC(PVPLearnLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	
	void setStyle(PVPLearnInfo LearnInfo);

	void PKbuttonCb(CCObject* pSender);
	void update(float dt);
	  virtual void onUpdateRemainTime(long remainTime);
     void setVisblePKButton(bool  val ,PVPLearnInfo LearnInfo);
	static bool  getShow() ;
	void updateLogin(float dt) ; 
	void mandSendAccept();
protected:

	void OnClose(CCObject* pSender);
	void OnConfirm(CCObject* pSender);
	
private:

	
private:
	
	UILayout* m_layout;
	PVPLearnInfo mLearnInfo ;
static 	bool   isShow ;
    int delet_menu  ;

};

#endif