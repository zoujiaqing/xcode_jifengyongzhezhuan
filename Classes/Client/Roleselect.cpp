#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "ServerInfoFromServer.h"
#include "XLogicManager.h"
#include "FrontCoverLayer.h"
#include "Roleselect.h"
#include "TXGUIHeader.h"
#include "GameAudioManager.h"

#include "GameResourceManager.h"
#include "GameFontManager.h"
#include "AspriteManager.h"
#include "ASprite.h"
#include "PlayerRoleManager.h"
#include "UICreator.h"
#include "AssetsManager.h"
#include "HelloWorldScene.h"
#include "Localization.h"
#include "GameConfigFromLuaManager.h"
#include "GMessage.h"
#include "ParticleManager.h"
#include "MessageBoxLayer.h"
#include "SkeletonAnimRcsManager.h"
#include "UserData.h"
#include "LuaTinkerManager.h"
#include "LoginManager.h"

using namespace TXGUI;

#define LOADINGTIME 10

FrontCoverLayer * Roleselect::pFrontCoverLayer = NULL ;
Roleselect::Roleselect()
	:index(0)
{
	
	m_layout = NULL;
	m_MenuItem = NULL ;

}

Roleselect::~Roleselect()
{
	UIManager::sharedManager()->RemoveUILayout("RoleselectUI");

	
	m_layout = NULL;

	
	
}
void Roleselect::OnClose(CCObject* pSender)
{
    
	 pFrontCoverLayer->setRoleselect(NULL);
	 removeFromParentAndCleanup(true);
}
void Roleselect::OnSelectRole(CCObject* pSender)
{
	CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(pSender);
	if(menuItem)
	{
		index = menuItem->getTag();
		if(m_MenuItem)
		{
			m_MenuItem->unselected();
		}
		menuItem->selected();
		m_MenuItem = menuItem ;

		

	
	}

}
void Roleselect::OnConfirm(CCObject* pSender)
{
	USERINFO info = UserData::GetUserInfo();
	info.id.setRegTime(0);
	if(pvec_UserRoleUser != NULL)
	{
		UserRole * pUserRole = NULL ;
		if(index <= pvec_UserRoleUser->size())
			pUserRole = pvec_UserRoleUser->at(index-1);
		if(pUserRole != NULL)
		{
			 info.id.setRegTime(pUserRole->getRegTime());
			 UserData::SetUserInfo(info);
			 XLogicManager::sharedManager()->entryLoginRole();
		}
		else
		{
			XLogicManager::sharedManager()->entryCreateRole();
		}					
	}
	
	//pFrontCoverLayer->menuLoginGameCallback(pSender);

	 
}
bool Roleselect::init()
{	
	bool bRet = false;
	pvec_UserRoleUser = NULL ;

	
	map<int ,vector<UserRole*>* > & tmMap_UserRole = PlayerRoleManager::Get()->getMapUserRole();
	mMap_UserRole = tmMap_UserRole ;
	do {


		m_layout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/Roleselect.plist", this, "RoleselectUI");
		if(m_layout == NULL)
			return false ;
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnClose",
			menu_selector(Roleselect::OnClose), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnConfirm",
			menu_selector(Roleselect::OnConfirm), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnSelectRole1",
			menu_selector(Roleselect::OnSelectRole), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnSelectRole2",
			menu_selector(Roleselect::OnSelectRole), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnSelectRole3",
			menu_selector(Roleselect::OnSelectRole), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnSelectRole4",
			menu_selector(Roleselect::OnSelectRole), this);
		UIManager::sharedManager()->RegisterMenuHandler("RoleselectUI","Roleselect::OnSelectRole5",
			menu_selector(Roleselect::OnSelectRole), this);
	    
	    USERINFO info = UserData::GetUserInfo();
		
		map<int ,vector<UserRole*>* >::iterator  miter =mMap_UserRole.find(info.serverId);
		if(miter != mMap_UserRole.end())
		{
			 pvec_UserRoleUser = miter->second ;
		}
		else
		{
			// removeFromParentAndCleanup(true);
			// return false;
		}
		
	    

		CCPoint pt1;
		ASprite * as1 = AspriteManager::getInstance()->getAsprite(KUI_BIN);
		int Roleselect_headframe = getResourceIDByName("map_ui_FRAME_ROLESELECT_HEADFRAME");
		CCSprite * pRoleselect_headframe = as1->getSpriteFromFrame_Middle(Roleselect_headframe, 0, pt1);
		
	 //   ASprite * ascom = AspriteManager::getInstance()->getAsprite(KUI_BIN);
		//int playerResourceIdcom = getResourceIDByName("map_ui_system_icon_FRAME_ICON_ROLEONMAP_1");
		//CCSprite *	playerImgcom = ascom->getSpriteFromFrame_Middle(playerResourceIdcom, 0, pt1);
		//UIPicture * pRoleselect_frame_roleiconval = m_layout->FindChildObjectByName<UIPicture>("Roleselect_frame_role1icon");
		//CCSize psizerole = pRoleselect_frame_roleiconval->getSpriteSize();
		//CCSize psize1role = playerImgcom->getContentSize();
		//float xScalerole = psizerole.width / psize1role.width ;
		//float yScalerole = psizerole.height / psize1role.height ;
		//float minScaleRole =  xScalerole >yScalerole ? yScalerole : xScalerole ;
		//playerImgcom->removeAllChildrenWithCleanup(true);

    //UIPicture * pRoleselect_headframe = m_layout->FindChildObjectByName<UIPicture>("Roleselect_headframe");
	UIPicture * pRoleselect_frame_roleheadval = m_layout->FindChildObjectByName<UIPicture>("Roleselect_frame_role1head");
	CCSize psize = pRoleselect_frame_roleheadval->getSpriteSize();
	CCSize psize1 = pRoleselect_headframe->getContentSize();
	float xScale = psize.width / psize1.width ;
	float yScale = psize.height / psize1.height ;
	float minScale =  xScale >yScale ? yScale : xScale ;
	float scale = UIManager::sharedManager()->getScaleFactor();

	pRoleselect_headframe->removeAllChildrenWithCleanup(true);
	//pRoleselect_headframe->setScale(minScale);

	for(int i = 1 ; i<6 ; i++)
	{
		UserRole * pUserRole = NULL ;
		if(pvec_UserRoleUser && i <= pvec_UserRoleUser->size() )
			 pUserRole = pvec_UserRoleUser->at(i-1);
	

		ASprite * as2 = AspriteManager::getInstance()->getAsprite(KUI_BIN);
		CCSprite * pRoleselect_headframe2 = as2->getSpriteFromFrame_Middle(Roleselect_headframe, 0, pt1);
		pRoleselect_headframe2->setScale(minScale/scale);
		stringstream playerResStreamhead;
		playerResStreamhead << "Roleselect_frame_role" << i<<"head" ;
		string playerResNamehead = playerResStreamhead.str();

		UIPicture * pRoleselect_frame_rolehead = m_layout->FindChildObjectByName<UIPicture>(playerResNamehead.c_str());
		pRoleselect_frame_rolehead->setSprite(pRoleselect_headframe2);


		stringstream playerResStreamicon;
		playerResStreamicon << "Roleselect_frame_role" << i<<"icon" ;
		string playerResNameicon = playerResStreamicon.str();
		UIPicture * pRoleselect_frame_roleicon = m_layout->FindChildObjectByName<UIPicture>(playerResNameicon.c_str());


		stringstream  playerResStreamrole;
		playerResStreamrole << "Roleselect_norole" << i ;
		string playerResNamerole = playerResStreamrole.str();

		UILabel * pRoleselect_norole = m_layout->FindChildObjectByName<UILabel>(playerResNamerole.c_str());
		pRoleselect_norole->setString(Localizatioin::getLocalization("M_ROLESELECT_UNCREATE"));

		stringstream  playerResStreamName;
		playerResStreamName << "Roleselect_role" << i << "name" ;
		string playerResNameName = playerResStreamName.str();
		UILabel * pRoleselect_rolename = m_layout->FindChildObjectByName<UILabel>(playerResNameName.c_str());

		stringstream  playerResStreamlevel;
		playerResStreamlevel << "Roleselect_role" << i << "level" ;
		string playerResNamelevel = playerResStreamlevel.str();
		UILabel * pRoleselect_rolelevel = m_layout->FindChildObjectByName<UILabel>(playerResNamelevel.c_str());
		if(pUserRole != NULL)
		{
			

			ASprite * as = AspriteManager::getInstance()->getAsprite(KICON_BIN);

			int playerType =pUserRole->getRoleType() ; //UserData::Get()->GetUserType();
			const char* headIconName = LuaTinkerManager::Get()->getLuaConfig<const char *>("characterString", "Characters",playerType, "HeadPicture");

			/*stringstream playerResStream;
			playerResStream << "map_ui_system_icon_FRAME_ICON_ROLEONMAP_" << playerType;
			string playerResName = playerResStream.str();*/
			int playerResourceId = getResourceIDByName(headIconName);
			CCSprite *	playerImg = as->getSpriteFromFrame_Middle(playerResourceId, 0, pt1);
			CCSize  iconCs = pRoleselect_frame_roleicon->getSpriteSize();
			CCSize  imCs = playerImg->getContentSize();
			float xSc = iconCs.width/imCs.width ;


			if(playerImg)
				playerImg->setScale(xSc*0.8/scale);
			pRoleselect_frame_roleicon->setSprite(playerImg);
			pRoleselect_norole->setVisible(false);
			pRoleselect_rolename->setString(pUserRole->getRoleName().c_str());
			pRoleselect_rolename->setColor(ccc3(255,162,0));

			stringstream playerLevelStream ;
			playerLevelStream << "Lv" << pUserRole->getRoleLevel();
			string playerLevelString = playerLevelStream.str();

			pRoleselect_rolelevel->setString(playerLevelString.c_str());
		}
		else
		{
			pRoleselect_rolename->setString(Localizatioin::getLocalization("M_ROLESELECT_ADDNEW"));
			pRoleselect_rolename->setColor(ccc3(99,99,99));
			pRoleselect_frame_roleicon->setVisible(false);
			pRoleselect_rolelevel->setVisible(false);
		}
		

	}

	
	


   


	UIButton *p_UIButton =m_layout->FindChildObjectByName<UIButton>("SelectRole1");
	m_MenuItem = dynamic_cast<CCMenuItem*>(p_UIButton->getMenuItemSprite());
	m_MenuItem->selected();
	index =1 ;
        /// response to keyboard
		this->setKeypadEnabled(true);
		
		bRet = true;
	}while (0);
	return bRet;
}
void Roleselect::update(float dt)
{
	//   unschedule(schedule_selector(FrontCoverLayer::update));
	//   //schedule(schedule_selector(FrontCoverLayer::readyLogin_func), 0.5f);
	//ReadyToLogin();
}

void Roleselect::setVisible(bool visible)
{
	CCLayer::setVisible(visible);
	/*if(visible && !m_bCheckLatestServer)
	{
	ReadyToLogin();
	m_bCheckLatestServer = true;
	}*/
}




