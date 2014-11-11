#include "GuildRecordLayer.h"
#include "UIManager.h"
#include "MainMenuLayer.h"
#include "GuildManager.h"
#include "Localization.h"

#include "GMessage.h"

#include <stdio.h>
//#include <crtdefs.h>
#include <time.h>

GuildRecordLayer::GuildRecordLayer() :
    m_titleLabel(NULL),
    m_leftButton(NULL),
    m_rightButton(NULL),
    m_pageLabel(NULL),
    m_contentLayer(NULL),
    m_closeButton(NULL),
    m_currentPage(1),
    m_totalPage(100)
{

}

GuildRecordLayer::~GuildRecordLayer()
{
    setTitleLabel(NULL);
    setLeftButton(NULL);
    setRightButton(NULL);
    setPageLabel(NULL);
    setContentLayer(NULL);
    setCloseButton(NULL);

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "GetGuildRecordInfo");   
}

SEL_MenuHandler GuildRecordLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler GuildRecordLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "close", GuildRecordLayer::onClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "left",  GuildRecordLayer::onLeftClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "right", GuildRecordLayer::onRightClicked);
    return NULL;
}

bool GuildRecordLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title",         CCLabelTTF*,      m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "left_btn",      CCControlButton*, m_leftButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "right_btn",     CCControlButton*, m_rightButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "content_layer", CCLayer*,         m_contentLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_label",    CCLabelTTF*,      m_pageLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "close_btn",     CCControlButton*, m_closeButton);
    return false;
}

void GuildRecordLayer::onNodeLoaded(CCNode * node, CCNodeLoader * nodeLoader)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(s.width / 2, s.height / 2);

    setTouchEnabled(true);

	//NotificationCenter::defaultCenter()->registerAllMsgObserver(this);//GetGuildRecordInfo

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                        callfuncO_selector(GuildRecordLayer::initRecordInfo), "GetGuildRecordInfo", NULL);

    float scale = UIManager::sharedManager()->getScaleFactor();
    setScale(scale);

    m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_currentPage, m_totalPage)->getCString());

	// 向服务器请求帮会基本信息
    GuildManager::Get()->SendGetGuildRecordInfoReq();


}

bool GuildRecordLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void GuildRecordLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCScrollTouchBgPriority, true);
}

void GuildRecordLayer::onEnter()
{
    CCLayer::onEnter();
    m_closeButton->setTouchPriority(kCCScrollMenuPriority);
    m_leftButton->setTouchPriority(kCCScrollMenuPriority);
    m_rightButton->setTouchPriority(kCCScrollMenuPriority);

	CCLayer* layerInfo = NULL;
	CCLabelTTF* labelYear;
	CCLabelTTF* labelTime;
	CCLabelTTF* labelMsg;

	for (int i = 0; i < 7; i++)
	{
		layerInfo = (CCLayer*)m_contentLayer->getChildByTag(i + 1);
		labelYear = (CCLabelTTF*)layerInfo->getChildByTag(1);
		labelTime = (CCLabelTTF*)layerInfo->getChildByTag(2);
		labelMsg = (CCLabelTTF*)layerInfo->getChildByTag(3);

		labelYear->setString("");
		labelTime->setString("");
		labelMsg->setString("");
	}
}

void GuildRecordLayer::onClose(CCObject* sender, CCControlEvent event)
{
    MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
    if (mLayer)
    {
        mLayer->ShowGuildRecordLayer(false);
        mLayer->ShowGuildInfoLayer(true);
    }
}

void GuildRecordLayer::onLeftClicked(CCObject* sender, CCControlEvent event)
{
    if (m_currentPage <= 1)
    {
        return;
    }

    m_currentPage--;
	showRecordInfo();
    m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_currentPage, m_totalPage)->getCString());
}

void GuildRecordLayer::onRightClicked(CCObject* sender, CCControlEvent event)
{
    if (m_currentPage >= m_totalPage)
    {
        return;
    }

    m_currentPage++;
	showRecordInfo();
    m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_currentPage, m_totalPage)->getCString());
}

void GuildRecordLayer::showRecordInfo()
{
	std::vector<GuildRecordInfoStruct> info = GuildManager::Get()->getRecordInfo();
	CCLayer* layerInfo = NULL;
	CCLabelTTF* labelYear;
	CCLabelTTF* labelTime;
	CCLabelTTF* labelMsg;

	struct tm* time;
	time_t long_time;
	char timebuffer[15] = {0};
	char timebuffer1[10] = {0};
	std::vector<std::string> splitedStr;

	for (int i = 0; i < 7; i++)
	{
		layerInfo = (CCLayer*)m_contentLayer->getChildByTag(i + 1);
		labelYear = (CCLabelTTF*)layerInfo->getChildByTag(1);
		labelTime = (CCLabelTTF*)layerInfo->getChildByTag(2);
		labelMsg = (CCLabelTTF*)layerInfo->getChildByTag(3);

		labelYear->setString("");
		labelTime->setString("");
		labelMsg->setString("");
		labelMsg->removeAllChildrenWithCleanup(true);
	}

	int infoStartIndex;
	int infoEndIndex;

	if ((m_currentPage * 7) > info.size())
	{
		infoStartIndex = (m_currentPage - 1) * 7;
		infoEndIndex = info.size();
	}
	else
	{
		infoStartIndex = (m_currentPage - 1) * 7;
		infoEndIndex = m_currentPage * 7;
	}

	const char * huizhang;
	const char * fuhuizhang;

	huizhang = Localizatioin::getLocalization("G_APPOINTMENT_POSITION4");
	fuhuizhang =  Localizatioin::getLocalization("G_APPOINTMENT_POSITION3");

	const char * jingying = Localizatioin::getLocalization("G_APPOINTMENT_POSITION2");
	const char * huiyuan =  Localizatioin::getLocalization("G_APPOINTMENT_POSITION1");

	for (int i = infoStartIndex; i < infoEndIndex; i++)
	{
		splitedStr.clear();
		char tempStr[200] = {0};

		int n = i % 7 + 1;
		layerInfo = (CCLayer*)m_contentLayer->getChildByTag(n);
		labelYear = (CCLabelTTF*)layerInfo->getChildByTag(1);
		labelTime = (CCLabelTTF*)layerInfo->getChildByTag(2);
		labelMsg = (CCLabelTTF*)layerInfo->getChildByTag(3);

		long_time = info[i].modify_tm;
		time = localtime(&long_time);

		sprintf(timebuffer, "%d-%d-%d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
		labelYear->setString(timebuffer);

		sprintf(timebuffer1, "%d:%02d", time->tm_hour, time->tm_min);
		labelTime->setString(timebuffer1);

		switch(info[i].record_type)
		{
		case 1:        //捐献金币             
			{
				splitedStr = split(info[i].log, "/");
				
				if (strcmp(splitedStr[2].c_str(), "0") == 0)
				{
					const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT15");
					sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str(), splitedStr[3].c_str());
				}
				else
				{
					const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT11");
					sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str(), splitedStr[2].c_str(), splitedStr[3].c_str());
				}
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(0, 255, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 2:       //捐献砖石
			{
				splitedStr = split(info[i].log, "/");
				
				if (strcmp(splitedStr[2].c_str(), "0") == 0)
				{
					const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT14");
					sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str(), splitedStr[3].c_str());
				}
				else
				{
					const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT1");
					sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str(), splitedStr[2].c_str(), splitedStr[3].c_str());
				}
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(0, 255, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 3:      //人员职位变动
			{
				splitedStr = split(info[i].log, "/");
				const char * tempString = "";

				if (strcmp(huizhang, splitedStr[0].c_str()))
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT6");
				}
				else if(strcmp(fuhuizhang, splitedStr[0].c_str()))
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT5");
				}
				else if(strcmp(jingying, splitedStr[0].c_str()))
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT4");
				}
				else if(strcmp(huiyuan, splitedStr[0].c_str()))
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT3");
				}

				sprintf(tempStr, tempString, splitedStr[0].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 97, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 4:        //玩家被踢出
			{
				splitedStr = split(info[i].log, "/");

				const char * tempString = "";
			
				if (strcmp(huizhang, splitedStr[0].c_str()) == 0)
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT2");
				}
				else if (strcmp(fuhuizhang, splitedStr[0].c_str()) == 0)
				{
					tempString = Localizatioin::getLocalization("G_RECORD_TEXT13");
				}

				sprintf(tempStr, tempString, splitedStr[1].c_str(), splitedStr[2].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 97, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 5:        //人员加入
			{
				splitedStr = split(info[i].log, "/");
				
				const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT4");
				sprintf(tempStr, tempString, splitedStr[0].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 97, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 6:        //人员离开
			{
				splitedStr = split(info[i].log, "/");
			
				const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT7");
				sprintf(tempStr, tempString, splitedStr[0].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 97, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 7:        //强制决斗失败
			{
				splitedStr = split(info[i].log, "/");
			
				const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT12");
				sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str(), splitedStr[2].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 0, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 8:        //公会升级
			{
				splitedStr = split(info[i].log, "/");
			
				sprintf(tempStr, "%s", splitedStr[0].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(255, 0, 255));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
			break;
		case 9:
			{
				splitedStr = split(info[i].log, "/");
			
				const char * tempString = Localizatioin::getLocalization("G_RECORD_TEXT16");
				sprintf(tempStr, tempString, splitedStr[0].c_str(), splitedStr[1].c_str());
				CCLabelTTF* label1 = CCLabelTTF::labelWithString(tempStr, "Helvetica", 22);
				label1->setColor(ccc3(0, 255, 0));
				label1->setAnchorPoint(labelMsg->getAnchorPoint());
				labelMsg->addChild(label1);
			}
		default:
			break;
		}
	}
}


void GuildRecordLayer::initRecordInfo(CCObject* obj)
{
	std::vector<GuildRecordInfoStruct> info = GuildManager::Get()->getRecordInfo();

	m_totalPage = info.size() % 7 ? (info.size() / 7 + 1) : info.size() / 7;

	m_pageLabel->setString(CCString::createWithFormat("%d / %d", m_currentPage, m_totalPage)->getCString());

	showRecordInfo();
}

//字符串分割函数
std::vector<std::string> GuildRecordLayer::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}