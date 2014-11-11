#include "OnlineNetworkManager.h"
#include "NetWorkConfigInfoFromServer.h"
#include "ServerInfoFromServer.h"
#include "XLogicManager.h"
#include "FrontCoverLayer.h"
#include "TXGUIHeader.h"
#include "GameAudioManager.h"
#include "TimeManager.h"
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
#include "WaitingLayerManager.h"
#include "Notice1UILayer.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "TmLoginManager.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "NcSDKManager.h"
#endif
using namespace TXGUI;

#define LOADINGTIME 10

FrontCoverLayer::FrontCoverLayer()
    :pLoginItem(NULL)
    , logOutItem(NULL)
    , m_bserverSelect(false)
    , m_bLogin(false)

{   m_Roleselect = NULL ;
    m_severScrollPage = NULL;
    this->m_accumulator = 0;
    this->m_lastTickTime = 0;
    m_layout = NULL;
    m_statusPoint = CCPointZero;
    m_rolePoint = CCPointZero ;
    m_recommendServerName = "";
    m_LastServerName ="";
    //m_bLatestServerFlag = true;
    m_lastServerStat = 2;
    m_bCheckLatestServer = false;
    m_server_req_type = 0 ;
}

FrontCoverLayer::~FrontCoverLayer()
{
    UIManager::sharedManager()->RemoveUILayout("SplashLayout");

    m_lstButton.clear();
    ServerInfoFromServer::pureInstance();
	PlayerRoleManager::Get()->clearRoleList();
}

bool FrontCoverLayer::init()
{    

    bool bRet = false;
    #ifdef OUR_DEBUG
    check_memory();
   #endif
    do {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NcSDKManager::SetFloatButtonHidden(true);
#endif
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        const char* path = GameResourceManager::sharedManager()->storedFullPathFromRelativePath("UI/frontCover.jpg");
        CCSprite* pSprite = CCSprite::create(path);

        // position the sprite on the center of the screen
        float scale = UIManager::sharedManager()->getScaleFactor();
        if(pSprite != NULL)
        {
            float scaleX = visibleSize.width/pSprite->getTexture()->getPixelsWide();
            float scaleY = visibleSize.height/pSprite->getTexture()->getPixelsHigh();
            scale = (scaleX > scaleY) ? scaleX : scaleY;

            pSprite->setScaleX(scale);
            pSprite->setScaleY(scale);
            pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
            this->addChild(pSprite, 0);
        }

        SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(410);
        //SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(411);
        //SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(412);
        //SkeletonAnimRcsManager::getInstance()->LoadOneRoleRcsOnly(413);

        EffectSprite* effect1 = ParticleManager::Get()->createEffectSprite(410,"",false);
        if(effect1)
        {
            effect1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
            effect1->getRoot()->setScale(scale);
            this->addChild(effect1, 5);
            effect1->SetAnim(kType_Play,1,true);
        }
        
        const char* vision = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/main.lua", "getVisionNum");
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        vision = NcSDKManager::getBundleVersion().c_str();
#endif
       
        char result[100]; 
        strcpy(result,vision); 
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS

        {
        const char *svn_version = LuaTinkerManager::Get()->callLuaFunc<char *>("Script/svn_version.lua", "getresversion");
        strcat(result,"\n");
        if(svn_version != NULL)
		{
			strcat(result,svn_version); // append svn_version  to the result.
		}
        }

#endif
        m_visionLabel = CCLabelTTF::create(result,KJLinXin,20);
        if(m_visionLabel)
        {
            float scale = UIManager::sharedManager()->getScaleFactor();
            m_visionLabel->setColor(ccWHITE);
            m_visionLabel->setScale(scale);
            m_visionLabel->setAnchorPoint(ccp(1.0, 1.0));
            addChild(m_visionLabel,100);
            
            m_visionLabel->setPosition(ccp(visibleSize.width,
                visibleSize.height ));
            //m_visionLabel->setPosition(ccp(visibleSize.width -m_visionLabel->getContentSize().width/2,
                //visibleSize.height -m_visionLabel->getContentSize().height/2));
        }

        m_layout = UIManager::sharedManager()->CreateUILayoutFromFile("UIplist/splashUI.plist", this, "SplashLayout");
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::menuLoginGameCallback",
            menu_selector(FrontCoverLayer::menuLoginGameCallback), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedMoreRole",
            menu_selector(FrontCoverLayer::onClickedMoreRole), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedMoreSever",
            menu_selector(FrontCoverLayer::onClickedMoreSever), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onCLickedSeverCloseBt",
            menu_selector(FrontCoverLayer::onCLickedSeverCloseBt), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedLatestSever",
            menu_selector(FrontCoverLayer::onClickedLatestSever), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedRecommendServer",
            menu_selector(FrontCoverLayer::onClickedRecommendServer), this);
        UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedReturnToLogin",
            menu_selector(FrontCoverLayer::onClickedReturnToLogin), this);
		UIManager::sharedManager()->RegisterMenuHandler("SplashLayout","FrontCoverLayer::onClickedNoticeBt",
            menu_selector(FrontCoverLayer::onClickedNoticeBt), this);
        if(m_layout)
        {
            m_severScrollPage = m_layout->FindChildObjectByName<UIScrollPage>("severScrollPage");
            setLastServerLabel(UserData::GetLastServerName());
            m_LastServerName = std::string(UserData::GetLastServerName());
            m_layout->setVisible(false);
        }
        
        /// response to keyboard
  
        //this->setKeypadEnabled(true);
        
        bRet = true;
    }while (0);
    return bRet;
}

void FrontCoverLayer::menuLoginGameCallback(CCObject* pSender)
{
    // check network
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    bool alreadyLogin = XLogicManager::sharedManager()->isAlreadyLogin();
	unsigned int channelId = LoginManager::Get()->getChannelID();
    char channelStr[20];
    sprintf(channelStr,"%d",channelId);
	bool flag = ServerInfoFromServer::sharedInstance()->downloadConfigFileList(channelStr);
    if(flag == false && alreadyLogin == false)
    {
        std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
        TXGUI::MessageBox::AdjustPriority(true);
        TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
        TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
        TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
        return;
    }
#endif
    
    if(XLogicManager::sharedManager()->isIOSNeedWaitingLayer == true)
    {
        return;
    }
    //if(m_bLatestServerFlag)
    if(m_lastServerStat ==0 || m_lastServerStat ==1)
    {
        USERINFO info = UserData::GetUserInfo();
        unsigned int serverId = info.serverId;
    
        if(serverId == 0)
        {
            showmsg(Localizatioin::getLocalization("M_SELECTSERVER"));
            return;
        }
        m_bserverSelect = false;
        m_bLogin = true;
        if(OnlineNetworkManager::sShareInstance()->getSocketStatus() == KSocketConnecting)
        {
			if(checkHasRoleList(serverId))
			{
				if(UserData::GetUserInfo().id.getRegTime() == 0)
					XLogicManager::sharedManager()->entryCreateRole();
				else
					XLogicManager::sharedManager()->entryLoginRole();
				return ;
			}
			else
			{
				// 没有拉到角色列表，重发协议拉取，并弹出提示

			}
        }
        else
        {
            #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			     XLogicManager::sharedManager()->logIn();
            #else
			    onLogoutConformBtClicked(NULL);
            #endif
        }     
    }
    else if( m_lastServerStat ==4)
    {
        const char* str = Localizatioin::getLocalization("M_SERVER_FULL");
        if(str != NULL)
        {
            MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
        }
    }
    else
    {
        const char* str = Localizatioin::getLocalization("M_SERVER_NOTOPEN");
        if(str != NULL)
        {
            MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
        }
    }
}

void FrontCoverLayer::menuLogOutCallback(CCObject* pSender)
{
    XLogicManager::sharedManager()->logOut();
}

void FrontCoverLayer::setVisible(bool visible)
{
    CCLayer::setVisible(visible);
//    if(visible)
//    {
//        m_server_req_type =3 ;
//        USERINFO info = UserData::GetUserInfo();
//        unsigned int serverId = info.serverId;
//        if(serverId != 0 && OnlineNetworkManager::sShareInstance()->getSocketStatus() == KSocketConnecting)
//        {
//            OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(serverId);
//        }
//        else
//        {
//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//            XLogicManager::sharedManager()->logIn();
//#else
//            onLogoutConformBtClicked(NULL);
//#endif
//        }
//        
//    }
    if(visible && !m_bCheckLatestServer)
    {
		ReadyToLogin();
		m_bCheckLatestServer = true;
    }
}
void FrontCoverLayer::onReceivedRoleList(CSGetRoleListRsp* msg)
{
    /*UILayout* m_layout = UIManager::sharedManager()->getUILayout("SplashLayout");
    if(m_layout)
    {
		if(m_severScrollPage)
		{
			map<int ,vector<UserRole*>* > & pMap_UserRole = PlayerRoleManager::Get()->getMapUserRole();

			int count = msg->servers_size();
			for(int i=0;i < count;i++)
			{
				unsigned int serverId = msg->servers(i);
				map<int ,vector<UserRole*>* >::iterator  miter = pMap_UserRole.find(serverId);
				if(miter != pMap_UserRole.end())
				{
					int rolecount = 0 ;
					vector<UserRole*>* pvec_UserRoleUser = (*miter).second;
					if(pvec_UserRoleUser )
					{
					    rolecount = pvec_UserRoleUser->size() ;
					    if(rolecount >5)rolecount = 5 ;

					}
					if(rolecount > 0)
					{
						const std::vector<IconButton*>& buttonList = m_severScrollPage->GetIconButton();
						for(std::vector<IconButton*>::const_iterator it = buttonList.cbegin();it != buttonList.cend();it++)
						{
							unsigned int index = (*it)->GetButtonIndex();
							if(index == serverId)
							{
								CCNode* headSprite = (*it)->getChildByTag(1001);
								if(NULL == headSprite)
								{
									ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
									int roleBackId = TXGUI::getResourceIDByName("map_ui_FRAME_SELECTSERVE_FRAME_ROLES");
									CCPoint pt;
									CCSprite *pRoleItem = as->getSpriteFromFrame_Middle(roleBackId,0 ,pt);
									pRoleItem->setPosition(getRoleSpritePosition());

									for(int j =0 ; j<rolecount ;j++)
									{
										UserRole* pUserRole = pvec_UserRoleUser->at(j);
										CCSprite * headRole = getRoleHeadSprite(pUserRole->getRoleType());
										(*it)->addChild(headRole ,13);
										headRole->setPosition(getRoleHeadSpritePosition(rolecount,j+1));
									}
								}
							}
						}
					}
				}
			}
		}
	}*/
}

void FrontCoverLayer::ReadyToLogin()
{

    if(m_layout)
    {
        m_layout->setVisible(true);
    }

    USERINFO info = UserData::GetUserInfo();
    unsigned int serverId = info.serverId;
    if(serverId == 0)
    {
        onSelectSever();
    }
    else
    {
        std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
        int count = serverList.size();
        if(count > 0)
        {
            unsigned int stat = 2;
            for(int i =0; i< count; i++)
            {
                int tempServer = atoi(serverList[i].tServerId.c_str());
                if(tempServer == serverId)
                {
                    int status = atoi(serverList[i].tStatus.c_str());
                    stat = status;
                    break;
                }
            }
            m_lastServerStat = stat;
        }
    }

}

void FrontCoverLayer::showmsg(const char *msg)
{
    CCNode* parent = this->getParent();
    HelloWorld* helloworld = dynamic_cast<HelloWorld*>(parent);
    if(helloworld)
    {
        helloworld->showMessage(msg);
    }
}

void FrontCoverLayer::keyBackClicked()
{
    std::string content = Localizatioin::getLocalization("S_Message_common_content");
    TXGUI::MessageBox::AdjustPriority(true);
    TXGUI::MessageBox::Show(content.c_str(),
        this,
        menu_selector(FrontCoverLayer::OkBtnClick),
        menu_selector(FrontCoverLayer::CancelBtnClick),
        MB_OKCANCEL,
        ccWHITE,
        24,
        KMessageBoxExitApp);
    TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
}

void FrontCoverLayer::keyMenuClicked()
{
    CCLOG("keyMenuClicked");
}

void FrontCoverLayer::onLogoutConformBtClicked(CCObject* obj)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    bool platformFlag = false;
    unsigned int nPlatformFlag = NetWorkConfigInfoFromServer::sharedInstance()->GetUsePlatformFlag();
    if (nPlatformFlag != 0)
    {
        platformFlag = true;
    }
    if(!platformFlag)
    {
        this->setVisible(false); 
    }
   // XLogicManager::sharedManager()->setLoginStatus(0);
    XLogicManager::sharedManager()->backToLogin();
#endif
}

void FrontCoverLayer::OkBtnClick(CCObject* obj)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    TmLoginManager::tmLoginManager()->doSdkExit();
#endif
    CCDirector::sharedDirector()->end();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}

void FrontCoverLayer::CancelBtnClick(CCObject* obj)
{
    CCLOG("FrontCoverLayer::CancelBtnClick");
}

void FrontCoverLayer::onReceivedServerPageChanged()
{
	/*if(m_severScrollPage)
	{
		vector<unsigned int> serverList;
		map<int ,vector<UserRole*>* > & pMap_UserRole = PlayerRoleManager::Get()->getMapUserRole();
		map<int ,long>& pMap_ReqTime = PlayerRoleManager::Get()->getRoleReqTime();
		int currentPage = m_severScrollPage->getCurPage();
		int column = 3;
        int row = 2;
		long currentTime = TimeManager::Get()->getCurServerTime();
		for(int i = 0;i < column;i++)
		{
			for(int j = 0; j < row; j++)
			{
				IconButton* bt = m_severScrollPage->GetIconButton(currentPage,column,row);
				if(bt)
				{
					unsigned int index = bt->GetButtonIndex();
					map<int ,vector<UserRole*>* >::iterator  miter = pMap_UserRole.find(index);
					if(miter == pMap_UserRole.end())
					{
						map<int ,long>::iterator it = pMap_ReqTime.find(index);
						bool canSendReq = false;
						if(it == pMap_ReqTime.end())
						{
							canSendReq = true;
							pMap_ReqTime.insert(make_pair(index,currentTime));
						}
						else
						{
							if(currentTime - (*it).second > 5)
							{
								canSendReq = true;
								(*it).second = currentTime;
							}
						}
						if(canSendReq)
						{
							serverList.push_back(index);
						}
					}
				}
			}
		}
		if(serverList.size() > 0)
		{
			OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(serverList);
		}
	}*/
}

void FrontCoverLayer::onReceiveSeverInfo()
{
    clearSeverList();
    setLastLoginSever();
    UILayout* m_layout = UIManager::sharedManager()->getUILayout("SplashLayout");
    if(m_layout)
    {
        UIContainer* m_splashContainer = m_layout->FindChildObjectByName<UIContainer>("splashContainer");
        if(m_splashContainer)
        {
            m_splashContainer->setVisible(false);
        }
        UIContainer* m_severContainer = m_layout->FindChildObjectByName<UIContainer>("selectServeContainer");
        if(m_severContainer)
        {
            m_severContainer->setVisible(true);

            std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
            int count = serverList.size();
            if(count > 0)
            {
                bool hasRecommendFlag = false;
                OnlineServerInfo recommendServer;
                if(m_severScrollPage)
                {
                    CCLayer* layer = CCLayer::create();
                    m_severScrollPage->addPage(layer);

                    
                    int currentPage = 0;
                    int column = 3;
                    int row = 2;

                    ASprite *as = AspriteManager::getInstance()->getAsprite(KUI_BIN);
                    int idx = TXGUI::getResourceIDByName("map_ui_FRAME_SELECTSERVE_BUTTON");
                    int selectedId = TXGUI::getResourceIDByName("map_ui_FRAME_SELECTSERVE_BUTTON_CLICKED");
                    int roleBackId = TXGUI::getResourceIDByName("map_ui_FRAME_SELECTSERVE_FRAME_ROLES");
                    float scale = UIManager::sharedManager()->getScaleFactor();
                    float width = m_severScrollPage->getContentSize().width / scale;
                    float height = m_severScrollPage->getContentSize().height / scale;

                    CCRect rect = as->getframeRect(idx);
                    CCRect rectRole = as->getframeRect(roleBackId);

                    float deltaWidth = (width - column * rect.size.width) /(column + 1);
                    float deltaHeigth = (height - row *(rect.size.height + rectRole.size.height)  )/(row + 1);

                    for(int i =0; i< count; i++)
                    {
                        int index = i;
                        int page = index / (column * row);
                        int rowIndex = (index - page * column * row) / column;
                        int columnIndex = (index - page * column * row) % column;
                        if(page != currentPage)
                        {
                            currentPage = page;
                            CCLayer* newlayer = CCLayer::create();
                            m_severScrollPage->addPage(newlayer);
                        }

                        CCPoint pt;
                        CCSprite *pEmptyItem = as->getSpriteFromFrame_Middle( idx, 0, pt);
                        CCSprite *pSelectedItem = as->getSpriteFromFrame_Middle(selectedId ,0,pt); 
                        CCSprite *pRoleItem = as->getSpriteFromFrame_Middle(roleBackId,0 ,pt);
                        IconButton * pIcon = new IconButton(pEmptyItem, NULL, pSelectedItem, this, 
                            menu_selector(FrontCoverLayer::onClickedOneSever));
                        pIcon->autorelease();
                        pIcon->SetButtonIndex(index);
                        pIcon->setContentSize(pEmptyItem->getContentSize());

                        float x = -width * 0.5f + deltaWidth * (columnIndex + 1) + (columnIndex + 0.5f) * rect.size.width ;
                        float y = height * 0.5f - (rowIndex + 0.5f) * (deltaHeigth + rect.size.height +rectRole.size.height)-0.5f*rectRole.size.height;
                        float yRole = 0.5f*rectRole.size.height;
                        pIcon->setPosition(ccp(x, y));
                        
                        int status = atoi(serverList[i].tStatus.c_str());
                        int serverId = atoi(serverList[i].tServerId.c_str());
                        int lastServerId = UserData::GetUserInfo().serverId;
                        if(serverId == lastServerId)
                        {
                         
                            setLastLoginSever(status);
                        }

                        CCSprite* statusSprite = getServerStatusSprite(status);
                        CCPoint statusPt = getStatusSpritePosition();
                        if(statusSprite != NULL)
                        {
                            pIcon->addChild(statusSprite,9);
                            statusSprite->setPosition(statusPt);
                        }

                        UILabelTTF* label = UILabelTTF::create(serverList[i].tServerName.c_str(),KJLinXin,40,pEmptyItem->getContentSize(),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
                        
						map<int ,vector<UserRole*>* >& pMap_UserRole = PlayerRoleManager::Get()->getMapUserRole();
                        map<int ,vector<UserRole*>* >::iterator  miter =pMap_UserRole.find(serverId);
                        vector<UserRole*>* pvec_UserRoleUser = NULL ;
                        if(miter != pMap_UserRole.end())
                        {
                            pvec_UserRoleUser = miter->second ;
                        }
                        int rolecount = 0 ;
                        if(pvec_UserRoleUser )
                        {
                            rolecount = pvec_UserRoleUser->size() ;
                            if(rolecount >5)rolecount = 5 ;

                        }    
                        
                        if(rolecount >0)
                        {
                            pIcon->addChild(pRoleItem,11,1001);
                            pRoleItem->setPosition(getRoleSpritePosition());

                            for(int j =0 ; j<rolecount ;j++)
                            {
                                UserRole* pUserRole = pvec_UserRoleUser->at(j);
                                CCSprite * headRole = getRoleHeadSprite(pUserRole->getRoleType());
                                pIcon->addChild(headRole ,13);
                                headRole->setPosition(getRoleHeadSpritePosition(rolecount,j+1));
                            }
                            
                        }

                        pIcon->addChild(label,10);
                        m_lstButton.push_back(pIcon);
                        CCLayer* currentLayer = m_severScrollPage->getPageLayer(currentPage);
                        
                        currentLayer->addChild(pIcon,index);    


                        int recommend = atoi(serverList[i].tRecommend.c_str());
                        if(!hasRecommendFlag && recommend == 1)
                        {
                            recommendServer = serverList[i];
                            hasRecommendFlag = true;
                        }
                    }

                    if(hasRecommendFlag)
                    {
                        int serverId = atoi(recommendServer.tServerId.c_str());
                        string serverName = recommendServer.tServerName;
                        m_recommendServerName = serverName;
                        int status = atoi(recommendServer.tStatus.c_str());
                        setRecommendServerLabel(serverName.c_str(),serverId,status);
                    }
                }
                
            }
        }
    }
}

void FrontCoverLayer::onClickedOneSever(CCObject* pSender)
{
    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(pSender);
    if(menuItem)
    {
        CCNode* node = menuItem->getParent();
        CCMenu* menu = dynamic_cast<CCMenu*>(node);
        if(menu)
        {
            CCNode* parent = menu->getParent();
            IconButton* button = dynamic_cast<IconButton*>(parent);
            if (button)
            {
                int buttonIndex = button->GetButtonIndex();

                std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
                OnlineServerInfo info;

                if (buttonIndex < serverList.size())
                    info = serverList[buttonIndex];
                else
                    return ;

                USERINFO data = UserData::GetUserInfo();

                unsigned int serverId = atoi(info.tServerId.c_str());
                int status = atoi(info.tStatus.c_str());
                sprintf(data.lastServerName,"%s",info.tServerName.c_str());

                if(status == 4)
                {
                    const char* str = Localizatioin::getLocalization("M_SERVER_FULL");
                    if(str != NULL)
                    {
                        MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
                    }
                }
                else if(serverId == 0 || status == 2)
                {
                    const char* str = Localizatioin::getLocalization("M_SERVER_NOTOPEN");
                    if(str != NULL)
                    {
                        MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
                    }
                }
                else
                {
                    data.serverId = serverId;
                    UserData::SetUserInfo(data);
                    m_lastServerStat = status;
					// 检查是否要进行gateWay重连
					checkNeedReconnect(serverId);
                    onCLickedSeverCloseBt((CCObject*)NULL);
                    setLastServerLabel(info.tServerName.c_str());
                }

            }
        }
    }
}
void FrontCoverLayer::onSelectSever()
{
    if(!m_bserverSelect)
    {
        m_bserverSelect = true;
        unsigned int channelId = LoginManager::Get()->getChannelID();
        char channelStr[20];
        sprintf(channelStr,"%d",channelId);
        bool receiveFlag = ServerInfoFromServer::sharedInstance()->downloadConfigFileList(channelStr);
        if(receiveFlag)
        {
            if(ServerInfoFromServer::sharedInstance()->parseRcvMessageFromServer())
            {
                onReceiveSeverInfo();
            }            
        }
        else
        {
            std::string msg = Localizatioin::getLocalization("M_UPDATEFULL_NETWORKFAILURE");
            TXGUI::MessageBox::AdjustPriority(true);
            TXGUI::MessageBox::Show(msg.c_str(), this, NULL,NULL, MB_OK,ccWHITE,24,KMessageBoxNetWorkDisconnectPriority);
            TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
            TXGUI::MessageBox::SetClickOkBtnDefaultBehaviorOrNot(true);
        }
        m_bserverSelect = false;
    }

}
void FrontCoverLayer::onClickedMoreSever(CCObject* pSender)
{
    
    USERINFO info = UserData::GetUserInfo();
    unsigned int serverId = info.serverId;
    //m_server_req_type =2 ;
    onSelectSever();
}
void FrontCoverLayer::OkQuitGameClick(CCObject* obj)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    TmLoginManager::tmLoginManager()->doSdkExit();
#endif
    CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
}
void FrontCoverLayer::enterRoleselect()
{
    if(m_Roleselect != NULL)
    {
        m_Roleselect->setVisible(true);
        return ;
    }
    Roleselect::setFrontCoverLayer(this);
    m_Roleselect = Roleselect::create();
    if(m_Roleselect != NULL)
        this->addChild(m_Roleselect,Z_Order_PopupDialogLayer);
}
void FrontCoverLayer::onClickedMoreRole(CCObject* pSender)
{

    if( m_lastServerStat ==4)
    {
        const char* str = Localizatioin::getLocalization("M_SERVER_FULL");
        if(str != NULL)
        {
            MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
        }
        return;
    }
    else if(m_lastServerStat != 0 && m_lastServerStat != 1)
    {
        const char* str = Localizatioin::getLocalization("M_SERVER_NOTOPEN");
        if(str != NULL)
        {
            MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
        }
        return;
    }

    USERINFO info = UserData::GetUserInfo();
    unsigned int serverId = info.serverId;
	if(checkHasRoleList(serverId,false))
	{
		enterRoleselect();
	}
	else
	{
		OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(serverId);
	}
    
}

void FrontCoverLayer::onCLickedSeverCloseBt(CCObject* pSender)
{
    UILayout* m_layout = UIManager::sharedManager()->getUILayout("SplashLayout");
    if(m_layout)
    {
        UIContainer* m_splashContainer = m_layout->FindChildObjectByName<UIContainer>("splashContainer");
        if(m_splashContainer)
        {
            m_splashContainer->setVisible(true);
        }
        UIContainer* m_severContainer = m_layout->FindChildObjectByName<UIContainer>("selectServeContainer");
        if(m_severContainer)
        {
            m_severContainer->setVisible(false);
        }
    }
}

void FrontCoverLayer::onClickedRecommendServer(CCObject* pSender)
{
    CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(pSender);
    if(menuItem)
    {
        int recommendServerId = menuItem->getTag();
        int recommendStatus = 2;
		std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
        for(std::vector<OnlineServerInfo>::iterator iter = serverList.begin();iter != serverList.end();iter++)
        {
            unsigned int serverId = atoi((*iter).tServerId.c_str());
            int status = atoi((*iter).tStatus.c_str());
            if(recommendServerId == serverId)
            {
                recommendStatus = status;
                break;
            }
        }

		if(recommendStatus == 4)
        {
            const char* str = Localizatioin::getLocalization("M_SERVER_FULL");
            if(str != NULL)
            {
                MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
            }
        }
        else if(recommendStatus == 0 || recommendStatus == 2)
        {
            const char* str = Localizatioin::getLocalization("M_SERVER_NOTOPEN");
            if(str != NULL)
            {
                MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
            }
        }
        else
        {

            USERINFO data = UserData::GetUserInfo();
			setLastServerLabel(m_recommendServerName.c_str());
        
			snprintf(data.lastServerName,30,"%s",m_recommendServerName.c_str());
			data.serverId = recommendServerId;
            UserData::SetUserInfo(data);
            m_lastServerStat = recommendStatus;

			checkNeedReconnect(recommendServerId);
            onCLickedSeverCloseBt((CCObject*)NULL);
        }		
    }
}

void FrontCoverLayer::onClickedLatestSever(CCObject* pSender)
{
    //if(m_bLatestServerFlag)
    if(m_lastServerStat ==0 || m_lastServerStat == 1)
    {
        USERINFO info = UserData::GetUserInfo();
        unsigned int serverId = info.serverId;
        snprintf(info.lastServerName,30 ,"%s",m_LastServerName.c_str());
        //info.id.setRegTime(getLastRole(info.serverId));
        UserData::SetUserInfo(info);
        setLastServerLabel(m_LastServerName.c_str());

		checkNeedReconnect(serverId);
        onCLickedSeverCloseBt((CCObject*)NULL);
    }
    else
    {
        const char* str = Localizatioin::getLocalization("M_SERVER_NOTOPEN");
        if(str != NULL)
        {
            MessageBox::Show(str,NULL,NULL,NULL,MB_OK);
        }
    }
    
}
void FrontCoverLayer::onClickedNoticeBt(CCObject* pSender)
{
	Notice1UILayer* pNoticeParent = Notice1UILayer::create();
    this->addChild(pNoticeParent,100);
}



void FrontCoverLayer::onClickedReturnToLogin(CCObject* pSender)
{
    std::string content = Localizatioin::getLocalization("M_QUIT_CHANGE_ACCOUNT");
    onCLickedSeverCloseBt(NULL);
    if(m_Roleselect != NULL)
    {
        m_Roleselect->removeFromParentAndCleanup(true);
        m_Roleselect = NULL ;
    }
    TXGUI::MessageBox::AdjustPriority(true);
    TXGUI::MessageBox::Show(content.c_str(),
        this,
        menu_selector(FrontCoverLayer::onLogoutConformBtClicked),
        menu_selector(FrontCoverLayer::CancelBtnClick),
        MB_OKCANCEL,
        ccWHITE,
        24);
    TXGUI::MessageBox::AdjustZOrder(Z_Order_MessageBox_2);
}

void FrontCoverLayer::clearSeverList()
{
    if(m_severScrollPage)
    {
        m_severScrollPage->removeAllPages();
    }
    m_lstButton.clear();
}

void FrontCoverLayer::addNewSever()
{
    
}

void FrontCoverLayer::setLastServerLabel(const char * name)
{
    if(m_layout)
    {
        UILabel* label = m_layout->FindChildObjectByName<UILabel>("serverLabel");
        if(label)
        {
            //const char* name = UserData::GetLastServerName();
            if(*name != '\0')
            {
                label->setString(name);
            }
        }
    }
}

void FrontCoverLayer::setLastLoginSever(int status)
{
    if(m_layout)
    {
        UIButton* button = m_layout->FindChildObjectByName<UIButton>("latestSeverBt");
        if(button)
        {
            USERINFO info = UserData::GetUserInfo();
            unsigned int serverId = info.serverId;
            const char* name = UserData::GetLastServerName();
            if(serverId == 0)
            {
                button->setEnabled(false);
            }
            else
            {
                UILabelTTF* label = button->getTextLabel();
                if(label)
                {
                    label->setString(name);
                }
            }
        }
    }
}

void FrontCoverLayer::setRecommendServerLabel(const char* name,int server,int status)
{
    if(m_layout)
    {
        UIButton* button = m_layout->FindChildObjectByName<UIButton>("recommondSeverBt");
        if(button)
        {
            if(server == 0)
            {
                button->setEnabled(false);
            }
            else
            {
                button->setTag(server);
                UILabelTTF* label = button->getTextLabel();
                if(label)
                {
                    label->setString(name);
                }
            }
        }
    }
}

CCSprite* FrontCoverLayer::getServerStatusSprite(int status)
{
    CCSprite* sprite = NULL;
    string fileName;
    switch(status)
    {
    case 0:
        fileName = "map_ui_server_state_FRAME_UI_SERVER_STATE2";
        break;
    case 1:
        fileName = "map_ui_server_state_FRAME_UI_SERVER_STATE4";
        break;
    case 2:
        fileName = "map_ui_server_state_FRAME_UI_SERVER_STATE3";
        break;
    case 3:
        fileName = "map_ui_server_state_FRAME_UI_SERVER_STATE1";
        break;
    case 4:
        fileName = "map_ui_server_state_FRAME_UI_SERVER_STATE5";
        break;
    default:
        fileName = "";
        break;
    }
    if(fileName != "")
    {
        ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui_server_state.bin");
        CCPoint statusPt;
        sprite = as->getSpriteFromFrame_Middle(getResourceIDByName(fileName.c_str()),0,statusPt);
    }
    return sprite;
}

CCPoint FrontCoverLayer::getStatusSpritePosition()
{
    if(!m_statusPoint.equals(CCPointZero))
    {
        return m_statusPoint;
    }

    ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    CCPoint statusPt;
    as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_SELECTSERVE_PIC_STATE"),0,statusPt);
    CCPoint buttonPt;
    CCSprite* buttonSize = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_SELECTSERVE_BUTTON"),0,buttonPt);
    m_statusPoint = ccpSub(statusPt,buttonPt);
    return m_statusPoint;
}
CCPoint FrontCoverLayer::getRoleSpritePosition()
{
    if(!m_rolePoint.equals(CCPointZero))
    {
        return m_rolePoint;
    }

    ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    CCPoint rolePt;
    as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_SELECTSERVE_FRAME_ROLES"),0,rolePt);
    CCPoint buttonPt;
    CCSprite* buttonSize = as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_SELECTSERVE_BUTTON"),0,buttonPt);
    m_rolePoint = ccpSub(rolePt,buttonPt);
    return m_rolePoint;
}
CCPoint FrontCoverLayer::getRoleHeadSpritePosition(int count ,int index)
{
    ASprite* as = AspriteManager::getInstance()->getAsprite("UI/ui.bin");
    CCPoint rolePt;
    as->getSpriteFromFrame_Middle(getResourceIDByName("map_ui_FRAME_SELECTSERVE_BUTTON"),0,rolePt);
    CCPoint roleHeadPt;
    stringstream stringstreamRes ;
    stringstreamRes<<"map_ui_FRAME_SELECTSERVE_ICON_ROLE"<<count<<"_"<<index ;
    string roleHeadStr = stringstreamRes.str();
    as->getSpriteFromFrame_Middle(getResourceIDByName(roleHeadStr.c_str()),0,roleHeadPt);
    return ccpSub(roleHeadPt ,rolePt);

}
CCSprite *FrontCoverLayer::getRoleHeadSprite(int playerType)
{
    CCPoint pt;
    ASprite * as = AspriteManager::getInstance()->getAsprite(KICON_BIN);
    stringstream playerResStream;
    playerResStream << "map_ui_system_icon_FRAME_ICON_SERVER_ROLE" << playerType;
    string playerResName = playerResStream.str();

    int playerResourceId = getResourceIDByName(playerResName.c_str());
    return as->getSpriteFromFrame_Middle(playerResourceId, 0, pt);
}
//unsigned int FrontCoverLayer::getLastRole(unsigned int sever_id)
//{
//    map<int ,vector<UserRole*>* >& pMap_UserRole =getMapUserRole();
//    map<int ,vector<UserRole*>* >::iterator  miter =pMap_UserRole.find(sever_id);
//    vector<UserRole*>* pvec_UserRoleUser = NULL ;
//    if(miter != pMap_UserRole.end())
//    {
//        pvec_UserRoleUser = miter->second ;
//    }
//    if(pvec_UserRoleUser == NULL)
//        return 0 ;
//    UserRole* role = NULL ;
//    unsigned int  maxLastLogin = 0  ;
//    unsigned int reg_time = 0 ;
//    for(int i = 0 ; i< pvec_UserRoleUser->size();i++)
//    {
//        role = pvec_UserRoleUser->at(i);
//        if(maxLastLogin <= role->getLoginTime())
//        {
//            maxLastLogin = role->getLoginTime();
//            reg_time = role->getRegTime() ;
//        }
//
//    }
//
//    return reg_time ;
//
//}

bool FrontCoverLayer::checkHasRoleList(unsigned int serverID,bool checkLastRole)
{
	bool hasRole = false;
	map<int ,vector<UserRole*>* >& pMap_UserRole = PlayerRoleManager::Get()->getMapUserRole();
    map<int ,vector<UserRole*>* >::iterator  miter =pMap_UserRole.find(serverID);
    if(miter != pMap_UserRole.end())
    {
		hasRole = true;
        vector<UserRole*>* pvec_UserRoleUser = miter->second ;
		if(checkLastRole)
		{
			USERINFO data = UserData::GetUserInfo();
			unsigned int  maxLastLogin = 0  ;
			unsigned int reg_time = 0 ;
			for(int i = 0 ; i< pvec_UserRoleUser->size();i++)
			{
				UserRole* role = pvec_UserRoleUser->at(i);
				if(maxLastLogin <= role->getLoginTime())
				{
					maxLastLogin = role->getLoginTime();
					reg_time = role->getRegTime() ;
				}
			}
			data.id.setRegTime(reg_time);
			UserData::SetUserInfo(data);
		}
    }
	return hasRole;
}

void FrontCoverLayer::checkAndGetRoleByServer(unsigned int serverID)
{
	if(!checkHasRoleList(serverID,false))
	{
		OnlineNetworkManager::sShareInstance()->sendGetRoleListMessage(serverID);
	}
}

void FrontCoverLayer::checkNeedReconnect(unsigned int serverID)
{
	std::vector<OnlineServerInfo> serverList = ServerInfoFromServer::sharedInstance()->GetServerInfo();
    for(std::vector<OnlineServerInfo>::iterator iter = serverList.begin();iter != serverList.end();iter++)
    {
        unsigned int server = atoi((*iter).tServerId.c_str());
		if(server == serverID)
		{
			if(!OnlineNetworkManager::sShareInstance()->startReconnect((*iter).tIp.c_str(),(*iter).tPort))
			{
				checkAndGetRoleByServer(serverID);
			}
            else
            {
                XLogicManager::sharedManager()->setStartConectint();
            }
			break;
		}
    }
}