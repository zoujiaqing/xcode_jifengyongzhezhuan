#include "UITutorial.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "TutorialsManager.h"
#include "GameManager.h"
#include "MainMenuLayer.h"
#include "GameUILayer.h"
#include "UIControlsApearControler.h"
#include "TutorialBase.h"
#include "LuaTinkerManager.h"
#include "TaskManager.h"
#include "ItemManager.h"

UITutorial::UITutorial()
{	
	this->Reset();
}

UITutorial::UITutorial(unsigned int id,TutorialDataCenter* pDataCenter):TutorialBase(id)
{
	this->Reset();
	mDataCenter = pDataCenter;
}

UITutorial::~UITutorial()
{

}

void UITutorial::Reset()
{
	mTutorialLayer = NULL;
	mCurLayout = NULL;
	mDstLayout = NULL;
	mDataCenter = NULL;
	mCurActiveUIButton = NULL;
	mCurActiveIconButton = NULL;
	mCurActiveUIButton_2 = NULL;
	mCurActiveIconButton_2 = NULL;
    mCurActiveControlButton = NULL;
    mCurActiveControlButton_2 = NULL;
	mCurLayoutName = "";
	mDstLayoutName = "";
	m_eTutorialType = kTutorialUI;
	mDelayFrames = 0;
	mRunningFrames = 0;
	mIsStartFrameCount = false;
	mIsStartCalControlPos = false;
	mControlItemOldPripority = 0;
	mCurUITutorialType = kType_Tutorail_UnKnow;
	mDragUIType = 0;
	mDragToUIPos = 0;
	mEventType = kType_Event_UnKnow;
	mCurActiveControlName = "";
	mOldButtonPoint = CCPointZero;
	mOldButtonPoint_2 = CCPointZero;
	mControlName = "";
	mControlName_2 = "";
	mControlType = 0;
	mControlType_2 = 0;
	mIsNextTutorialNewWnd = true;
	mIsAllowInEmptyArea = false;
	mIsCareDstPos = true;
	mIsEndWithSpecialControl = false;
}

void UITutorial::Start(const char * attachStartControlName /* = "" */)
{
	MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
	int tag = mLayer->getLayerTagByName("UITutorial");
	mTutorialLayer = (TutorialLayer*)mLayer->getChildByTag(tag);

	if (NULL != mTutorialLayer)
	{
		mTutorialLayer->removeFromParentAndCleanup(true);
	}

	mTutorialLayer = TutorialLayer::create();
	mLayer->addChild(mTutorialLayer,Z_Order_TutorialLayer,tag);
	mTutorialLayer->ShowLayer();

	FirstCheckAllReleaseControls(attachStartControlName);

	MoveNextTutorial();	
}

void UITutorial::End()
{
	if (mTutorialLayer)
	{
		mTutorialLayer->removeFromParentAndCleanup(true);
		mTutorialLayer = NULL;
	}
	// Note: 下一帧的时候销毁
	this->autorelease();

	mCurUITutorialType = kType_Tutorail_UnKnow;
	TutorialsManager::Get()->ResetTutorialInstance();

	if (mTutorialType ==  kTutorialTask)
	{
		if (mIsEndWithSpecialControl)
		{
			TaskManager::getInstance()->SendAutoTaskSystemState();
		}
		mTutorialType = kTutorialUnKnow;
	}
}

void UITutorial::HightLightControls()
{
	if (mDataCenter)
	{
		HightLightOneControl(mCurActiveUIButton,FIRTST_LAYOUT);
		HightLightOneControl(mCurActiveIconButton,FIRTST_LAYOUT);
		HightLightOneControl(mCurActiveUIButton_2,SECOND_LAYOUT);
		HightLightOneControl(mCurActiveIconButton_2,SECOND_LAYOUT);
        HightLightOneControl(mCurActiveControlButton, FIRTST_LAYOUT);
        HightLightOneControl(mCurActiveControlButton_2, SECOND_LAYOUT);
	}
}

void UITutorial::GetLayerControls(std::string name,int id,unsigned int type)
{
	if (mCurLayout && "" != name)
	{
		TXGUI::UILayout *pTmpLayout = NULL;
		if (id == 1)
		{
			pTmpLayout = mCurLayout;
		}
		else if (id == 2)
		{
			pTmpLayout = mDstLayout;
		}

		UIButton* btn = pTmpLayout->FindChildObjectByName<TXGUI::UIButton>(name.c_str());
		IconButton* pIconButton = NULL;
		
        //背包物品特殊处理
        if (type != 3)
        {
            if (name.find("bagItem_") != string::npos)
            {
                string buttonName = ItemManager::Get()->getItemButtonNameWithAjust(name);
                pIconButton = pTmpLayout->FindIconButtonChildByName(buttonName.c_str());
            }
            else
            {
                pIconButton = pTmpLayout->FindIconButtonChildByName(name.c_str());
            }
        }

        CCControlButton* cBtn = NULL;
        UITutorialBase* tBase = dynamic_cast<UITutorialBase*>(pTmpLayout);
        if (tBase)
        {
            cBtn = dynamic_cast<CCControlButton*>(tBase->getTutorialNode(name));
        }
				
		if (id == 1)
		{
			mCurActiveUIButton = (btn && (type == 1)) ? btn : NULL;
			mCurActiveIconButton = (pIconButton && (type == 2)) ? pIconButton : NULL;
            mCurActiveControlButton = (cBtn && (type == 3)) ? cBtn : NULL;
		}
		else if (id == 2)
		{
			mCurActiveUIButton_2 = (btn && (type == 1)) ? btn : NULL;
			mCurActiveIconButton_2 = (pIconButton && (type == 2)) ? pIconButton : NULL;
            mCurActiveControlButton_2 = (cBtn && (type == 3)) ? cBtn : NULL;
		}
	}
}

void UITutorial::HightLightOneControl(TXGUI::UIButton *pBtn,unsigned int id)
{
	if (pBtn)
	{
		mControlItemOldPripority = pBtn->getHandlerPriority();
		pBtn->setHandlerPriority(KCCLayerTutorialControlPripority);

		ShowFlagIconWithOneContorl(pBtn,id);
	}
}

void UITutorial::HightLightOneControl(TXGUI::IconButton *pBtn,unsigned int id)
{
	if (pBtn)
	{
		mControlItemOldPripority = pBtn->getHandlerPriority();
		pBtn->setHandlerPriority(KCCLayerTutorialControlPripority);

		ShowFlagIconWithOneContorl(pBtn,id);
	}
}

void UITutorial::HightLightOneControl(CCControlButton* pBtn, unsigned int id)
{
    if (pBtn)
    {
        mControlItemOldPripority = pBtn->getTouchPriority();
        pBtn->setTouchPriority(KCCLayerTutorialControlPripority);

        ShowFlagIconWithOneContorl(pBtn,id);
    }
}

void UITutorial::ResetAllControl()
{
	if (mDataCenter)
	{
		ResetOneControl(mCurActiveUIButton);
		ResetOneControl(mCurActiveUIButton_2);
		ResetOneControl(mCurActiveIconButton);
		ResetOneControl(mCurActiveIconButton_2);
	}
}

void UITutorial::ResetOneControl(TXGUI::UIButton* pBtn)
{
	if (pBtn)
	{
		if (0 != mControlItemOldPripority)
		{
			pBtn->setHandlerPriority(mControlItemOldPripority);
		}	
	}
}

void UITutorial::ResetOneControl(TXGUI::IconButton* pBtn)
{
	if (pBtn)
	{
		if (0 != mControlItemOldPripority)
		{
			pBtn->setHandlerPriority(mControlItemOldPripority);
		}		
	}
}

void UITutorial::HandleOneTutorialItemWithDelay(unsigned int numFrames /* = 2 */)
{
	mDelayFrames = numFrames;
	mIsStartFrameCount = true;
	mRunningFrames = 0;	
}

void UITutorial::HandleOneTutorialItemWithExpandMenuOver()
{
	this->HightLightControls();
}

void UITutorial::InitNextTutorial()
{
	if (mDataCenter && 0 != m_nTutorialID)
	{
		mDataCenter->SetCurIterator(m_nTutorialID);

		mCurUITutorialType = mDataCenter->GetOneTutorialType();
		mDragUIType = mDataCenter->GetOneTutorialUIType(2);
		mDragToUIPos = mDataCenter->GetOneTutorialPos(2);
		mEventType = mDataCenter->GetOneTutorialEventType();
		mCurActiveControlName = mControlName = mDataCenter->GetOneTutorialControlName(1);
		mControlType = mDataCenter->GetOneTutorialControlType(1);
		mControlName_2 = mDataCenter->GetOneTutorialControlName(2);
		mControlType_2 = mDataCenter->GetOneTutorialControlType(2);		
		mIsAllowInEmptyArea = mDataCenter->GetOneTutorialIsAllowInEmptyArea();
		mIsCareDstPos = mDataCenter->GetOneTutorialIsCareDstPos();

		mCurLayoutName = mDataCenter->GetOneTutorialLayoutName();
		mDstLayoutName = mDataCenter->GetOneTutorialDestLayoutName();
	}
}
 
void UITutorial::HandleOneTutorialItem()
{
	if (0 != m_nTutorialID && NULL != mDataCenter && NULL != mCurLayout)
	{
		mIsStartCalControlPos = true;

		GameUILayer* pGameUILayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer()->GetGameUILayer();
		if (pGameUILayer)
		{
			if (pGameUILayer->IsOneButtonInExpandList(mCurActiveUIButton))
			{
				CCCallFunc *rc = CCCallFunc::create(this,callfunc_selector(UITutorial::HandleOneTutorialItemWithExpandMenuOver));
				if (false == pGameUILayer->ShrinkButtonClickRightNow(true,true,rc))
				{
					this->HightLightControls();
				}
			}
			else
			{
				this->HightLightControls();
			}
		}
	}
}

void UITutorial::ShowFlagIconWithOneContorl(TXGUI::UIButton *pBtn,unsigned int id)
{
	if (pBtn)
	{
		unsigned int flagDisplayType = mDataCenter->GetOneTutorialTipType(id);

		bool bShow = true;
		if (flagDisplayType == 0)
		{
			bShow = false;
		}

		if (false == bShow)
		{
			mTutorialLayer->ShowFlagIconAtPos(CCPointZero,"",0);
		}
		else
		{
			CCPoint btnPt = pBtn->getWorldPosition();
			CCSize btnSize = pBtn->getButtonSize();

			CCPoint relativePt = mDataCenter->GetOneTutorialRelativePt(id);

			const char* content = mDataCenter->GetOneTutorialContent(id);

			CCPoint realPt = ccp(btnPt.x + relativePt.x,btnPt.y + relativePt.y);
			mTutorialLayer->ShowFlagIconAtPos(realPt,content,flagDisplayType);

			if (id == FIRTST_LAYOUT)
			{
				mOldButtonPoint = btnPt;
				mTutorialLayer->SetClipAreaPos(0, btnPt, btnSize);
				mTutorialLayer->SetCirclePos(btnPt);
			}
			else if (id == SECOND_LAYOUT)
			{
				mOldButtonPoint_2 = btnPt;
				mTutorialLayer->SetClipAreaPos(1, btnPt, btnSize);
			}
		}
	}
}

void UITutorial::ShowFlagIconWithOneContorl(TXGUI::IconButton *pBtn,unsigned int id)
{
	if (pBtn)
	{
		unsigned int flagDisplayType = mDataCenter->GetOneTutorialTipType(id);

		bool bShow = true;
		if (flagDisplayType == 0)
		{
			bShow = false;
		}

		if (false == bShow)
		{
			mTutorialLayer->ShowFlagIconAtPos(CCPointZero,"",0);
		}
		else
		{
			CCPoint btnPt = pBtn->getParent()->convertToWorldSpace(pBtn->getPosition());
			CCSize btnSize = pBtn->getContentSize();

			CCPoint relativePt = mDataCenter->GetOneTutorialRelativePt(id);

			const char* content = mDataCenter->GetOneTutorialContent(id);

			CCPoint realPt = ccp(btnPt.x + relativePt.x,btnPt.y + relativePt.y);
			mTutorialLayer->ShowFlagIconAtPos(realPt,content,flagDisplayType);

			if (id == 1)
			{
				mOldButtonPoint = btnPt;
				mTutorialLayer->SetClipAreaPos(0, btnPt, btnSize);
				mTutorialLayer->SetCirclePos(btnPt);
			}
			else if (id == 2)
			{
				mOldButtonPoint_2 = btnPt;
				mTutorialLayer->SetClipAreaPos(1, btnPt, btnSize);
			}
		}		
	}
}

void UITutorial::ShowFlagIconWithOneContorl(CCControlButton* pBtn, unsigned int id)
{
    if (pBtn)
    {
        unsigned int flagDisplayType = mDataCenter->GetOneTutorialTipType(id);

        bool bShow = true;
        if (flagDisplayType == 0)
        {
            bShow = false;
        }

        if (false == bShow)
        {
            mTutorialLayer->ShowFlagIconAtPos(CCPointZero,"",0);
        }
        else
        {
            CCPoint btnPt = pBtn->getParent()->convertToWorldSpace(pBtn->getPosition());
            CCSize btnSize = pBtn->getContentSize();

            CCPoint relativePt = mDataCenter->GetOneTutorialRelativePt(id);

            const char* content = mDataCenter->GetOneTutorialContent(id);

            CCPoint realPt = ccp(btnPt.x + relativePt.x,btnPt.y + relativePt.y);
            mTutorialLayer->ShowFlagIconAtPos(realPt,content,flagDisplayType);

            if (id == 1)
            {
                mOldButtonPoint = btnPt;
                mTutorialLayer->SetClipAreaPos(0, btnPt, btnSize);
				mTutorialLayer->SetCirclePos(btnPt);
            }
            else if (id == 2)
            {
                mOldButtonPoint_2 = btnPt;
                mTutorialLayer->SetClipAreaPos(1, btnPt, btnSize);
            }
        }		
    }
}


void UITutorial::ResetTutorialValue()
{
	if (mTutorialLayer)
	{
		mTutorialLayer->HideAllFlagIcon();
	}
	ResetAllControl();

	mOldButtonPoint = CCPointZero;
	mOldButtonPoint_2 = CCPointZero;
	mCurActiveIconButton = NULL;
	mCurActiveUIButton = NULL;	
	mCurActiveIconButton_2 = NULL;
	mCurActiveUIButton_2 = NULL;	
	mCurLayout = NULL;
	mDstLayout = NULL;
	mCurLayoutName = "";
	mDstLayoutName = "";
	mControlName = "";
	mControlName_2 = "";
	mControlType = 0;
	mControlType_2 = 0;
	mDragToUIPos = 0;
	mDragUIType = 0;
	mEventType = kType_Event_UnKnow;
	mIsStartCalControlPos = false;	
}

void UITutorial::MoveNextTutorial()
{	
//#ifdef _WIN32
//	return ;
//#endif	
	bool disableUITutorial = LuaTinkerManager::Get()->callLuaFunc<bool>("Script/GameConfig/DebugConfig.lua", "IsDisableUITutorial");
	if (disableUITutorial)
	{
		return ;
	}
	ResetTutorialValue();
	
	if (0 == m_nTutorialID)
	{
		End();
	}
	else
	{
		InitNextTutorial();

		if (mIsNextTutorialNewWnd)
		{
			HandleOneTutorialItemWithDelay();
		}
		else
		{
			HandleOneTutorialItemWithDelay(0);
		}

		if (1 == mControlType)
		{
			UIControlAppearControler::Get()->TackOneTutorialEvent("UIButton",mControlName,mCurLayoutName);
		}		
		else if (2 == mControlType)
		{
			UIControlAppearControler::Get()->TackOneTutorialEvent("IconButton",mControlName,mCurLayoutName);
		}
	}
}

bool UITutorial::HandleOneEvent(const char* name,unsigned int type)
{
	bool bRst = true;

	int nCompareRst = strcmp(name,mCurActiveControlName.c_str());
	if (IsInTutorial())
	{
		if (mTutorialLayer->isVisible())
		{
			if (mCurUITutorialType == kType_Tutorial_Drag || (nCompareRst == 0 && type != mEventType))
			{
				bRst = false;
				return bRst;
			}
		}
	}

	if (nCompareRst == 0)
	{
		if (type == mEventType)
		{
			// Note: UIButton 点击等处理
			if (mCurUITutorialType == kType_Tutorial_Normal || mCurUITutorialType == kType_Tutorial_TaskLead)
			{
				unsigned int tmpTD = mDataCenter->GetNextTutorialID();
				mIsNextTutorialNewWnd = mDataCenter->GetOneTutorialIsNewWnd();
				if (m_nTutorialID == tmpTD)
				{
					CCError("Tutorial ID Same Error %d",tmpTD);
					return bRst;
				}
				m_nTutorialID = tmpTD;
				MoveNextTutorial();
			}
		}
	}

	return bRst;
}

bool UITutorial::HandleDragEventOver(int type /* = 0 */,int pos /* = 0 */,bool bInEmptyUI /* = false */)
{
	if (mCurUITutorialType == kType_Tutorial_Drag)
	{
		bool bSuc = false;
		// Note: 处理拖拽时间
		if (bInEmptyUI)
		{
			if (mIsAllowInEmptyArea)
			{
				bSuc = true;
			}
		}
		else
		{
			if (mIsCareDstPos)
			{
				if (type == mDragUIType && pos == mDragToUIPos)
				{
					bSuc = true;
				}
			}
			else
			{
				if (type == mDragUIType)
				{
					bSuc = true;
				}
			}
		}

		if (bSuc)
		{
			m_nTutorialID = mDataCenter->GetNextTutorialID();
			mIsNextTutorialNewWnd = mDataCenter->GetOneTutorialIsNewWnd();
			MoveNextTutorial();

			return true;
		}
	}
	return false;
}

bool UITutorial::IsInDragTutorial()
{
	if (mCurUITutorialType == kType_Tutorial_Drag)
	{
		return true;
	}
	return false;
}

bool UITutorial::IsInTutorial()
{
	if (mCurUITutorialType != kType_Tutorail_UnKnow)
	{
		return true;
	}

	return false;
}

bool UITutorial::IsLayoutVisiable(TXGUI::UILayout* pLayout)
{
	if (pLayout == NULL)
	{
		return false;
	}

	bool isLayoutVisible = true;

	CCNode *pNode = pLayout->getCurrentNode();
	for (CCNode *c = pNode; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			isLayoutVisible = false;
			break;
		}
	}

	return isLayoutVisible;
}

void UITutorial::Update(float dt)
{
	if (0 != m_nTutorialID && NULL != mDataCenter && NULL != mTutorialLayer && "" != mCurLayoutName)
	{
		bool bNeedCheckDestLayout = false;
		if (mDstLayoutName != "")
		{
			bNeedCheckDestLayout = true;
		}

		// Note: 得到Layout实例
		//if (mCurLayout == NULL)
		{
			mCurLayout = UIManager::sharedManager()->getUILayout(mCurLayoutName.c_str());
		}
		if (bNeedCheckDestLayout)
		{
			//if (mDstLayout == NULL)
			{
				mDstLayout = UIManager::sharedManager()->getUILayout(mDstLayoutName.c_str());
			}
		}

		// Note: 得到Layout上对应的Button
		if (mCurLayout != NULL)
		{
			//if (mCurActiveUIButton == NULL && mCurActiveIconButton == NULL)
			{
				GetLayerControls(mControlName,1,mControlType);
			}
		}
		if (bNeedCheckDestLayout)
		{
			if (mDstLayout != NULL)
			{
				//if (mCurActiveUIButton_2 == NULL && mCurActiveIconButton_2 == NULL)
				{
					GetLayerControls(mControlName_2,2,mControlType_2);
				}
			}
		}

		bool bLayoutComeOut = false;
		if (bNeedCheckDestLayout)
		{
			if (mCurLayout != NULL && mDstLayout != NULL && (mCurActiveUIButton != NULL || mCurActiveIconButton != NULL || mCurActiveControlButton != NULL)
				 && (mCurActiveUIButton_2 != NULL || mCurActiveIconButton_2 != NULL || mCurActiveControlButton_2 != NULL))
			{
				bLayoutComeOut = true;
			}
		}
		else
		{
			if (mCurLayout != NULL && (mCurActiveUIButton != NULL || mCurActiveIconButton != NULL || mCurActiveControlButton != NULL))
			{
				bLayoutComeOut = true;
			}
		}

		MainMenuLayer * mLayer = GameManager::Get()->GetSceneLayer()->getMainMenuLayer();
		int tag = mLayer->getLayerTagByName("UITutorial");
		mTutorialLayer = (TutorialLayer*)mLayer->getChildByTag(tag);
		if (NULL == mTutorialLayer)
		{
			mTutorialLayer = TutorialLayer::create();
			mLayer->addChild(mTutorialLayer,Z_Order_TutorialLayer,tag);
		}

		if (bLayoutComeOut)
		{
			bool isLayoutVisible = IsLayoutVisiable(mCurLayout);
			mTutorialLayer->ShowLayerActorOrNot(isLayoutVisible);
			if (isLayoutVisible)
			{
				if (mIsStartFrameCount)
				{
					if (mRunningFrames >= mDelayFrames)
					{
						HandleOneTutorialItem();

						mIsStartFrameCount = false;
						mRunningFrames = mDelayFrames = 0;
					}
					else
					{
						mRunningFrames ++;
					}
				}
			}
		}

		// Note: 开始进行引导Icon的位置修正
		if (mIsStartCalControlPos)
		{
			if (NULL != mCurActiveUIButton)
			{
				CCPoint btnPt = mCurActiveUIButton->getWorldPosition();
				float deltaX = btnPt.x - mOldButtonPoint.x;
				float deltaY = btnPt.y - mOldButtonPoint.y;
				if (fabs(deltaX) > 0.1 || 
					fabs(deltaY) > 0.1 )
				{
					mTutorialLayer->ChangeFLagPos(deltaX,deltaY,0);
					mOldButtonPoint = btnPt;
				}
			}
			if (NULL != mCurActiveIconButton)
			{
				CCPoint btnPt = mCurActiveIconButton->getParent()->convertToWorldSpace(mCurActiveIconButton->getPosition());
				float deltaX = btnPt.x - mOldButtonPoint.x;
				float deltaY = btnPt.y - mOldButtonPoint.y;
				if (fabs(deltaX) > 0.1 || 
					fabs(deltaY) > 0.1 )
				{
					mTutorialLayer->ChangeFLagPos(deltaX,deltaY,0);
					mOldButtonPoint = btnPt;
				}
			}
			if (NULL != mCurActiveUIButton_2)
			{
				CCPoint btnPt = mCurActiveUIButton_2->getWorldPosition();
				float deltaX = btnPt.x - mOldButtonPoint_2.x;
				float deltaY = btnPt.y - mOldButtonPoint_2.y;
				if (fabs(deltaX) > 0.1 || 
					fabs(deltaY) > 0.1 )
				{
					mTutorialLayer->ChangeFLagPos(deltaX,deltaY,1);
					mOldButtonPoint_2 = btnPt;
				}
			}
			if (NULL != mCurActiveIconButton_2)
			{
				CCPoint btnPt = mCurActiveIconButton_2->getParent()->convertToWorldSpace(mCurActiveIconButton_2->getPosition());
				float deltaX = btnPt.x - mOldButtonPoint_2.x;
				float deltaY = btnPt.y - mOldButtonPoint_2.y;
				if (fabs(deltaX) > 0.1 || 
					fabs(deltaY) > 0.1 )
				{
					mTutorialLayer->ChangeFLagPos(deltaX,deltaY,1);
					mOldButtonPoint_2 = btnPt;
				}
			}
            if (NULL != mCurActiveControlButton)
            {
                CCPoint btnPt = mCurActiveControlButton->getParent()->convertToWorldSpace(mCurActiveControlButton->getPosition());
                float deltaX = btnPt.x - mOldButtonPoint.x;
                float deltaY = btnPt.y - mOldButtonPoint.y;
                if (fabs(deltaX) > 0.1 || 
                    fabs(deltaY) > 0.1 )
                {
                    mTutorialLayer->ChangeFLagPos(deltaX,deltaY,0);
                    mOldButtonPoint = btnPt;
                }
            }

            if (NULL != mCurActiveControlButton_2)
            {
                CCPoint btnPt = mCurActiveControlButton_2->getParent()->convertToWorldSpace(mCurActiveControlButton_2->getPosition());
                float deltaX = btnPt.x - mOldButtonPoint_2.x;
                float deltaY = btnPt.y - mOldButtonPoint_2.y;
                if (fabs(deltaX) > 0.1 || 
                    fabs(deltaY) > 0.1 )
                {
                    mTutorialLayer->ChangeFLagPos(deltaX,deltaY,1);
                    mOldButtonPoint_2 = btnPt;
                }
            }
		}
	}
}

void UITutorial::FirstCheckAllReleaseControls(const char * attachStartControlName /* = "" */)
{
	bool bFirstCheck = false;
	unsigned int newTutorialId = m_nTutorialID;
	std::string lastControlName = "";

	TutorialDataCenter* pDataCenter = TutorialsManager::Get()->GetTutorialData();
	const std::map<unsigned int ,OneTutorialData> tutorialData = pDataCenter->GetData();
	std::map<unsigned int ,OneTutorialData>::const_iterator iter = tutorialData.find(m_nTutorialID);
	if (iter != tutorialData.end())
	{
		for (std::map<unsigned int ,OneTutorialData>::const_iterator _iter = iter; _iter != tutorialData.end(); _iter++)
		{
			if ((*_iter).second.nNextId == 0)
			{
				break ;
			}
			else
			{
				int tmpTutorialId = (*_iter).first;

				mDataCenter->SetCurIterator(tmpTutorialId);

				std::string controlName = mDataCenter->GetOneTutorialControlName(1);
				unsigned int controlType = mDataCenter->GetOneTutorialControlType(1);
				std::string controlName_2 = mDataCenter->GetOneTutorialControlName(2);
				unsigned int controlType_2 = mDataCenter->GetOneTutorialControlType(2);		
				std::string curLayoutName = mDataCenter->GetOneTutorialLayoutName();

				if (false == bFirstCheck && strcmp(attachStartControlName,"") != 0)
				{					
					if (strcmp(attachStartControlName,controlName.c_str()) == 0)
					{
						newTutorialId = _iter->second.nNextId;
						bFirstCheck = true;
					}					
				}

				lastControlName = controlName;

				if (controlName != "")
				{
					if (1 == controlType)
					{
						UIControlAppearControler::Get()->TackOneTutorialEvent("UIButton",controlName,curLayoutName,true);
					}
					else if (2 == controlType)
					{
						UIControlAppearControler::Get()->TackOneTutorialEvent("IconButton",controlName,curLayoutName,true);
					}
				}

				if (controlName_2 != "")
				{
					if (1 == controlType_2)
					{
						UIControlAppearControler::Get()->TackOneTutorialEvent("UIButton",controlName_2,curLayoutName,true);
					}		
					else if (2 == controlType_2)
					{
						UIControlAppearControler::Get()->TackOneTutorialEvent("IconButton",controlName_2,curLayoutName,true);
					}
				}
			}
		}
	}

	if (lastControlName != "" && (lastControlName != "button_instance" && lastControlName.find("TaskAnswerBtn_") == std::string::npos))
	{
		mIsEndWithSpecialControl = true;
	}

	m_nTutorialID = newTutorialId;
	mDataCenter->SetCurIterator(newTutorialId);
}