#include "UIEditBox.h"
#include "CrossPlatform.h"
#include "GameFontManager.h"
#include "AspriteManager.h"
#include "UIManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "JniHelper.h"

#include <android/log.h>
#include <string.h>
#include <jni.h>
#endif
namespace TXGUI {
	USING_NS_CC;

	NiImplementRTTI(UIEditBox, UIControlBase);
	NiImplementCreateObject(UIEditBox);

	UIEditBox::UIEditBox()
	{
		m_pEditBox = NULL;
	}

	UIEditBox::~UIEditBox()
	{
		//CCLOG("~UIEditBox");
		#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "closeEditTextDialog", "()V")) {
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
       #endif
		resignFirstResponder();
	}

	void UIEditBox::BindNode(CCNode* pParentNode)
	{
		UIControlBase::BindNode(pParentNode);
	}

	void UIEditBox::setString(const char *label)
	{
		if (m_pEditBox)
		{
			m_pEditBox->setText(label);
		}
	}

	const char* UIEditBox::getString()
	{
		if (m_pEditBox)
		{
			return m_pEditBox->getText();
		}
		return "";
	}

	void UIEditBox::setScale(float scale)
	{
		UIControlBase::setScale(scale);

		if(m_pEditBox != NULL)
		{
            m_pEditBox->setScale(scale);
		}
	}

	void UIEditBox::setScale(float xScale, float yScale)
	{
		UIControlBase::setScale(xScale, yScale);

		if(m_pEditBox != NULL)
		{
			m_pEditBox->setScaleX(xScale);
			m_pEditBox->setScaleY(yScale);
		}
	}

	void UIEditBox::setPosition(CCPoint pt)
	{
		UIControlBase::setWorldPosition(pt);

		if (m_pEditBox)
		{
			m_pEditBox->setPosition(pt);
		}
	}

	void UIEditBox::setLengthLimit(int value)
	{

		if(m_pEditBox != NULL)
		{
			m_pEditBox->setMaxLength(value);
		}
	}

	void UIEditBox::setIsVisible(bool isVisible)
	{
		if(m_pEditBox != NULL)
		{
			m_pEditBox->setVisible(isVisible);
		}
	}

	void UIEditBox::clearString()
	{
		if(m_pEditBox != NULL)
		{
			m_pEditBox->setText("");
		}
	}

	void UIEditBox::becomeFirstResponder() {
		CCLOG("UIEditBox::becomeFirstResponder");
		if (m_pEditBox != NULL) {
			m_pEditBox->attachWithIME();
		}
	}

	void UIEditBox::resignFirstResponder() {
		CCLOG("UIEditBox::resignFirstResponder");
		if (m_pEditBox != NULL) {
			m_pEditBox->detachWithIME();
		}
	}

	void UIEditBox::LoadBinary(NiStream &kStream)
	{
		UIControlBase::LoadBinary(kStream);    

		mutableDic* dic = kStream.GetStreamData();

		float contentScale = CCDirector::sharedDirector()->getContentScaleFactor();

		uint32_t bLocalizeString = 0;
		kStream.getIntAttributeValue(dic, "bLocalizeString", bLocalizeString);

		std::string strTemp = "";
		kStream.getStringattributeValue(dic, "placeholder", strTemp);
		const char* placeholder = strTemp.c_str();
		if (bLocalizeString > 0)
		{
			placeholder = CCLocalizedString(strTemp.c_str(), strTemp.c_str());
		}

		uint32_t lengthLimit = 0;
		kStream.getIntAttributeValue(dic, "lengthLimit", lengthLimit);

		uint32_t isPassword = 0;
		kStream.getIntAttributeValue(dic, "isPassword", isPassword);

		int priority = kCCMenuHandlerPriority;
		kStream.getSignedIntAttributeValue(dic,"priority",priority);

		uint32_t dimensionWidth = 0;
		uint32_t dimensionHeight = 0;
		kStream.getIntAttributeValue(dic, "dimensionWidth", dimensionWidth);
		kStream.getIntAttributeValue(dic, "dimensionHeight", dimensionHeight);

		dimensionWidth /= contentScale;
		dimensionHeight /= contentScale;

		uint32_t alignment = 1;
		kStream.getIntAttributeValue(dic, "alignment", alignment);

		mutableDic *colorDic = static_cast<mutableDic*>(dic->objectForKey("color"));
		uint32_t color[3] = {255, 255, 255};
		if(colorDic)
		{
			kStream.getIntAttributeValue(colorDic, "red", color[0]);
			kStream.getIntAttributeValue(colorDic, "green", color[1]);
			kStream.getIntAttributeValue(colorDic, "blue", color[2]);
		}

		string font = KJLinXin;
		kStream.getStringattributeValue(dic, "font", font);

		float largeFontSize = GameFontManager::largeFontSize();
		uint32_t fontSize = largeFontSize * contentScale;
		kStream.getIntAttributeValue(dic, "fontSize", fontSize);
		fontSize /= contentScale;

		CCSize dimension = CCSizeMake(dimensionWidth, dimensionHeight);
		if(dimensionWidth > 0 && dimensionHeight < fontSize)
		{
			dimension = CCSizeMake(dimensionWidth, fontSize);
		}
		string fileName = "";
		kStream.getStringattributeValue(dic, "fileName", fileName);
		CCSprite * m_defaultSprite = NULL;
		mutableDic *rectDic = static_cast<mutableDic*>(dic->objectForKey("rect"));
		if(rectDic)
		{
			uint32_t useASRect = 0;
			std::string ASRectName = "";
			std::string ASfile = KUI_BIN;
			uint32_t anchorPoint = 0;
			// if use Asprite Rect
			kStream.getIntAttributeValue(rectDic, "useASRect", useASRect);
			kStream.getIntAttributeValue(rectDic, "anchorPoint", anchorPoint);
			if(useASRect == 1)
			{
				kStream.getStringattributeValue(rectDic, "ASRectName", ASRectName);
				if(ASRectName != "")
				{
					mutableDic* dicAsMgr = static_cast<mutableDic*>(rectDic->objectForKey("AspriteManager"));
					// content attributes 
					if (dicAsMgr)
					{
						if (IsIpad())
						{
							kStream.getStringattributeValue(dicAsMgr, "ipad", ASfile); 
						}else
						{
							kStream.getStringattributeValue(dicAsMgr, "iphone", ASfile); 
						}
					}
					int ID = getResourceIDByName(ASRectName.c_str());
					// name not exists, use normal method
					if(ID != -1)
					{
						ASprite *as = AspriteManager::getInstance()->getAsprite(ASfile);
						CCRect asRect = as->getframeRect(ID);
						CCPoint pt;
						m_defaultSprite = as->getSpriteFromFrame_Middle(ID,0,pt);
						//m_defaultSprite = as->getSpriteFromFrame_Middle(ID,0,pt);
						switch(anchorPoint)
						{
						case 0:
							{
								CCRect rect = as->getframeRect(ID);
								m_ptWorldPosition = rect.origin;
							}
							break;

						case 1:
							m_ptWorldPosition = as->GetFramePointMiddle(ID);
							break;

						default:
							break;
						}

						CCPoint parentWorldPosition = CCPointZero;
						if(m_pParent != NULL)
						{
							parentWorldPosition = m_pParent->getWorldPosition();
						}

						m_ptLocalPosition.x = m_ptWorldPosition.x - parentWorldPosition.x;
						m_ptLocalPosition.y = m_ptWorldPosition.y - parentWorldPosition.y;
					}
				}
			}
		}

		//m_pTextInputField = TextInputField::textFieldWithPlaceHolder(placeholder,
		//	dimension,
		//	(CCTextAlignment)alignment,
		//	font.c_str(),
		//	(float)fontSize
		//	);
		if(m_defaultSprite && fileName != "")
		{
            //float scale = UIManager::sharedManager()->getScaleFactor();
			CCScale9Sprite* back = CCScale9Sprite::create(fileName.c_str());
			m_pEditBox = CCEditBox::create(
				//CCSizeMake(back->getContentSize().width * scale,back->getContentSize().height * scale)
                back->getContentSize()
				,back);
            
			m_pEditBox->setAnchorPoint(ccp(0.5, 0.5));
			m_pEditBox->setPosition(m_ptLocalPosition);
			m_pEditBox->setMaxLength(lengthLimit);

			//m_pTextInputField->setText(isPassword != 0);
            m_pEditBox->setPlaceHolder(placeholder);
			m_pEditBox->setColor(ccc3(color[0], color[1], color[2]));
			m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
			m_pEditBox->setTouchPriority(priority);
            m_pEditBox->setFont(font.c_str(),fontSize);
			back->setVisible(false);	

		}

		//// make background
		//CreateWhiteBack();
		//if(m_pWhiteBack != NULL)
		//{
		//	m_pWhiteBack->setPosition(m_ptLocalPosition);
		//	m_pWhiteBack->setVisible(m_bIsVisible);
		//}

		m_pControlNode = m_pEditBox;
		m_pControlNode->setVisible(m_bIsVisible);
	}

	void UIEditBox::LinkObject(NiStream &kStream)
	{
		UIControlBase::LinkObject(kStream);
	}

	bool UIEditBox::RegisterStreamables(NiStream &kStream)
	{
		if (! UIControlBase::RegisterStreamables(kStream))            
		{
			return false;           
		}

		// Register ExtraData

		return true;
	}

	void UIEditBox::SaveBinary(NiStream &kStream)
	{
		UIControlBase::SaveBinary(kStream);

	}

	bool UIEditBox::IsEqual(NiObject *pObject)
	{
		if (! UIControlBase::IsEqual(pObject))
			return false;

		// Test for extra data equality, but ignore non-streamable data


		return true;
	}    
}

