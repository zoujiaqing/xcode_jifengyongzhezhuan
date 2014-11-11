#include "TextInputField.h"

const static float DELTA = 40.0f;
#define DELAY 1.0f

namespace TXGUI
{
	TextInputField::TextInputField()
	{
		CCTextFieldTTF();

		m_pCursorSprite = NULL;
		m_pCursorAction = NULL;

		m_strInputText = "";
		m_strShowText = "";

		m_bIsPassword = false;
		m_bIsScheduled = false;

		m_cursorPos = CCPointZero;
		m_beginPos = ccp(-1.0, -1.0);
		m_rootPos = CCPointZero;

		m_iLengthLimit = 0;
		m_priority = kCCMenuHandlerPriority;
		m_touchInputSize = CCSizeZero;
        b_isSelect = false;
	}

	TextInputField::~TextInputField()
	{

	}

	void TextInputField::onEnter()
	{
		CCTextFieldTTF::onEnter();
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, m_priority, false);
		this->setDelegate(this);
	}

	void TextInputField::setInitPriority(int priority)
	{
		m_priority = priority;
	}

	TextInputField * TextInputField::textFieldWithPlaceHolder(const char *placeholder, CCSize dimensions, 
		CCTextAlignment alignment, const char *fontName, float fontSize)
	{
		TextInputField *pRet = new TextInputField();

		if(pRet && pRet->initWithString("",fontName, fontSize, dimensions, alignment))
		{
			pRet->autorelease();
			if (placeholder)
			{
				pRet->setPlaceHolder(placeholder);
			}
			pRet->initCursorSprite(fontSize);

			return pRet;
		}

		CC_SAFE_DELETE(pRet);

		return NULL;
	}

	TextInputField * TextInputField::textFieldWithPlaceHolder(const char *placeholder, 
		const char *fontName, float fontSize)
	{
		TextInputField *pRet = new TextInputField();

		if(pRet && pRet->initWithString("", fontName, fontSize))
		{
			pRet->autorelease();
			if (placeholder)
			{
				pRet->setPlaceHolder(placeholder);
			}
			pRet->initCursorSprite(fontSize);

			return pRet;
		}

		CC_SAFE_DELETE(pRet);

		return NULL;
	}

	void TextInputField::initCursorSprite(int nHeight)
	{
		int column = 4;
        nHeight *= CC_CONTENT_SCALE_FACTOR();
		//int pixels[nHeight][column];
		int *pixels = new int[nHeight * column];
		for (int i=0; i<nHeight; ++i) {
			for (int j=0; j<column; ++j) {
				//pixels[i][j] = 0xffffffff;
				pixels[i*4+j] = 0xffffffff;
			}
		}

		CCTexture2D *texture = new CCTexture2D();
		texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));

		delete[] pixels;

		m_pCursorSprite = CCSprite::createWithTexture(texture);
		texture->release();
		CCSize winSize = getContentSize();
		m_cursorPos = ccp(0, winSize.height / 2);
		m_pCursorSprite->setPosition(m_cursorPos);
		this->addChild(m_pCursorSprite);
		m_pCursorSprite->setVisible(false);

		m_pCursorAction = CCRepeatForever::create(
			(CCActionInterval *) CCSequence::create(
			CCFadeOut::create(0.25f), 
			CCFadeIn::create(0.25f), 
			NULL
			)
			);

		m_pCursorSprite->runAction(m_pCursorAction);
	}

	bool TextInputField::attachWithIME()
	{
		m_pCursorSprite->setVisible(true);
        std::string curStr = getString();
		if(!curStr.length())
		{
			CCTextFieldTTF::setString("");
		}
        b_isSelect = true;
		return CCTextFieldTTF::attachWithIME();
	}

	bool TextInputField::detachWithIME()
	{
		m_pCursorSprite->setVisible(false);

		std::string curStr = getString();
		if(!curStr.length())
		{
			CCTextFieldTTF::setString("");
		}
        b_isSelect = false;
		return CCTextFieldTTF::detachWithIME();
	}

	const char *TextInputField::getContentText()
	{
		return m_strInputText.c_str();
	}

	bool TextInputField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{    
		m_beginPos = pTouch->getLocationInView();
		m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);

		return true;
	}

	void TextInputField::setTouchInputSize(CCSize touchSize)
	{
		m_touchInputSize = touchSize;
	}

	CCRect TextInputField::getRect()
	{
		CCSize size = CCSizeZero;
		if(CCSize::CCSizeEqualToSize(m_touchInputSize,CCSizeZero))
		{
			size = getContentSize();
		}
		else
		{
			size = CCSizeMake(m_touchInputSize.width,m_touchInputSize.height) ;
		}
		CCPoint anchorPoint = getAnchorPoint();

		return  CCRectMake(-size.width * anchorPoint.x, -size.height * anchorPoint.y, size.width, size.height);
	}

	bool TextInputField::isInTextField(cocos2d::CCTouch *pTouch)
	{
        if(!this->isVisible())
        {
            return false;
        }
        
        CCNode* parent = this->getParent();
        while(parent != NULL)
        {
            if (!parent->isVisible()) {
                return false;
            }
            parent = parent->getParent();
        }
        
		return getRect().containsPoint(convertTouchToNodeSpaceAR(pTouch));
	}

	void TextInputField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		CCPoint endPos = pTouch->getLocationInView();
		endPos = CCDirector::sharedDirector()->convertToGL(endPos);

		if (::abs(endPos.x - m_beginPos.x) > DELTA ||
			::abs(endPos.y - m_beginPos.y) > DELTA)
		{
			// 不是点击事件
			m_beginPos.x = m_beginPos.y = -1;

			return;
		}

		// 判断是打开输入法还是关闭输入法
		isInTextField(pTouch) ? attachWithIME() : detachWithIME();
	}

	bool TextInputField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
	{
		if (m_strInputText.empty()) {
			return false;
		}

		CCPoint newCursorPos = ccp(getContentSize().width, m_pCursorSprite->getPosition().y);
		m_pCursorSprite->setPosition(newCursorPos);

		return false;
	}

	bool TextInputField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
	{
		if(m_bIsPassword)
		{
			if(m_bIsScheduled)
			{
				this->unschedule(schedule_selector(TextInputField::ChangeToPassword));
			}
			ChangeToPassword(0.0f);
		}

     
		std::string tmp = text;
		int enterPos = tmp.find_first_of('\n');
		bool hasEnter = (enterPos != std::string::npos);

		if(hasEnter)
		{
			tmp = tmp.substr(0, enterPos);
		}
        
		m_strInputText.append(tmp);
		m_strShowText.append(tmp);

		if(m_iLengthLimit != 0 && (int) m_strInputText.length() > m_iLengthLimit)
		{
			m_strInputText.resize(m_iLengthLimit);
		}

		if(m_iLengthLimit != 0 && (int) m_strShowText.length() > m_iLengthLimit)
		{
			m_strShowText.resize(m_iLengthLimit);
		}

		setString(m_strShowText.c_str());
        
		if(m_bIsPassword)
		{
			this->schedule(schedule_selector(TextInputField::ChangeToPassword), DELAY);
			m_bIsScheduled = true;
		}

		CCPoint newCursorPos = ccp(getContentSize().width, m_pCursorSprite->getPosition().y);
		m_pCursorSprite->setPosition(newCursorPos);

		if(hasEnter)
		{
			detachWithIME();
		}

		return true;
	}

	bool TextInputField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
	{
		m_strInputText.resize(m_strInputText.size() - nLen);
		m_strShowText.resize(m_strShowText.size() - nLen);

		setString(m_strShowText.c_str());

		CCPoint newCursorPos = ccp(getContentSize().width, m_pCursorSprite->getPosition().y);
		m_pCursorSprite->setPosition(newCursorPos);

		if (m_strShowText.empty()) {
			m_pCursorSprite->setPosition(ccp(0, m_pCursorSprite->getPosition().y));
		}

		return false;
	}

	bool TextInputField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
	{
		return false;
	}

	void TextInputField::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
	{
        if(!b_isSelect)
        {
            return;
        }
        
        CCRect rect = this->getRect();
		CCPoint pt = this->getPosition();
        pt.y -= rect.size.height * 0.5f;
        if(this->getParent() != NULL)
        {
            pt = this->getParent()->convertToWorldSpace(pt);
        }
		if (pt.y < info.end.origin.y + info.end.size.height)
		{
			CCNode* rootNode = this;
			while(rootNode->getParent() != NULL)
			{
				rootNode = rootNode->getParent();
			}
			m_rootPos = rootNode->getPosition();
			CCPoint destPos = m_rootPos;
			destPos.y += info.end.origin.y + info.end.size.height - pt.y + 10;
			CCMoveTo* moveTo = CCMoveTo::create(info.duration, destPos);
			rootNode->runAction(moveTo);
		}
	}

	void TextInputField::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
	{
		CCPoint destPos = m_rootPos;
		CCNode* rootNode = this;
		while(rootNode->getParent() != NULL)
		{
			rootNode = rootNode->getParent();
		}
		CCMoveTo* moveTo = CCMoveTo::create(info.duration, destPos);
		rootNode->runAction(moveTo);
	}

	void TextInputField::onExit()
	{
		CCTextFieldTTF::onExit();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	}

	void TextInputField::setIsPassword(bool value)
	{
		m_bIsPassword = value;
	}

	void TextInputField::setLengthLimit(int value)
	{
		if(value < 0)
			return;

		m_iLengthLimit = value;
	}

	CCSize TextInputField::getDimensions()
	{
		return m_tDimensions;
	}

	void TextInputField::ChangeToPassword(float dt)
	{
		this->unschedule(schedule_selector(TextInputField::ChangeToPassword));
		m_bIsScheduled = false;

		for(int i = 0; i < (int)m_strShowText.length(); i++)
		{
			m_strShowText[i] = '*';
		}

		setString(m_strShowText.c_str());

		CCPoint newCursorPos = ccp(getContentSize().width, m_pCursorSprite->getPosition().y);
		m_pCursorSprite->setPosition(newCursorPos);
	}

	const char *TextInputField::getString()
	{
		return m_strInputText.c_str();
	}

	void TextInputField::clearString()
	{
		m_strShowText = "";
		m_strInputText = "";
		setString("");

		m_pCursorSprite->setPosition(ccp(0, m_pCursorSprite->getPosition().y));
	}
    
    void TextInputField::resetInputText(const char *label)
    {
        m_strShowText = "";
		m_strShowText.append(label);
		m_strInputText = "";
		m_strInputText.append(label);

		if(m_iLengthLimit != 0 && (int) m_strInputText.length() > m_iLengthLimit)
		{
			m_strInputText.resize(m_iLengthLimit);
		}

		if(m_iLengthLimit != 0 && (int) m_strShowText.length() > m_iLengthLimit)
		{
			m_strShowText.resize(m_iLengthLimit);
		}

        CCTextFieldTTF::setString(label);
    }
    
}
