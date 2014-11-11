//
//  UILabelTTF.cpp
//  HelloLua
//
//  Created by Wesley Yin on 3/22/13.
//
//

#include "UILabelTTF.h"

namespace TXGUI {
    
    UILabelTTF::UILabelTTF()
    {
        m_labelTTf = NULL;
        m_renderTexture = NULL;
        m_renderColer = ccBLACK;
        m_renderSize = 1;

		/*CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
			callfuncO_selector(UILabelTTF::listenToBackground),
			EVNET_COME_TO_FOREGROUND,
			NULL);*/
    }
    
    UILabelTTF::~UILabelTTF()
    {
         //CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVNET_COME_TO_FOREGROUND);
    }
    
    UILabelTTF * UILabelTTF::create(const char *string, const char *fontName, float fontSize)
    {
        return UILabelTTF::create(string, fontName, fontSize,
                                  CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    }
    
    UILabelTTF * UILabelTTF::create(const char *string, const char *fontName, float fontSize,
                                    const CCSize& dimensions, CCTextAlignment hAlignment)
    {
        return UILabelTTF::create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop);
    }
    
    UILabelTTF * UILabelTTF::create(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment)
    {
        if(string == NULL)
            return NULL ;
        UILabelTTF *pRet = new UILabelTTF();
        if(pRet && pRet->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
    bool UILabelTTF::initWithString(const char *string, const char *fontName, float fontSize,
                   const CCSize& dimensions, CCTextAlignment hAlignment,
                   CCVerticalTextAlignment vAlignment)
    {
        m_labelTTf = CCLabelTTF::create(string, fontName, fontSize, dimensions, hAlignment, vAlignment);
		if(m_labelTTf == NULL)
		{
			return false;
		}
		CreateStroke();
		addChild(m_labelTTf);

        //m_renderTexture = createStroke(m_labelTTf, m_renderSize, m_renderColer);
        //if(m_renderTexture != NULL)
        //{
		//	addChild(m_renderTexture);
        //}
        return true;
    }
    
    CCRenderTexture* UILabelTTF::createStroke(CCLabelTTF* label, float size ,ccColor3B cor)
    {
        float RealSize = size;
        if(label == NULL || label->getTexture() == NULL)
        {
            return NULL;
        }
		label->setAnchorPoint(ccp(0.5, 0.5));
        CCRenderTexture* rt = CCRenderTexture::create(label->getContentSize().width + RealSize*2, label->getContentSize().height + RealSize*2);
		rt->setAnchorPoint(ccp(0.0,0.0));
		rt->setPosition(0,0);
        if(rt->getSprite() && rt->getSprite()->getTexture())
        {
            rt->getSprite()->getTexture()->setAntiAliasTexParameters();
			rt->getSprite()->setOpacityModifyRGB(true);
        }
		else
		{
			CCLOG("UILabelTTF null texture");
		}

		CCPoint originalPos = label->getPosition();
		ccColor3B originalColor = label->getColor();
		bool originalVisibility = label->isVisible();
		label->setColor(cor);
		label->setVisible(true);
		ccBlendFunc originalBlend = label->getBlendFunc();
		ccBlendFunc tBlendFunc = {GL_SRC_ALPHA, GL_ONE };
		label->setBlendFunc(tBlendFunc);

		rt->clear(0, 0, 0, 0);
		CCPoint center = ccp(label->getContentSize().width/2 ,label->getContentSize().height /2 );
		rt->begin();
		label->setPosition(ccp(center.x, center.y - RealSize));
		label->visit();
		label->setPosition(ccp(center.x + 2*RealSize, center.y - 2*RealSize));
		label->visit();
		label->setPosition(ccp(center.x + RealSize, center.y));
		label->visit();
		label->setPosition(ccp(center.x + 2*RealSize, center.y - RealSize));
		label->visit();
		label->setColor(originalColor);
		label->setPosition(ccp(center.x,center.y));
		label->visit();
		rt->end();

        label->setBlendFunc(originalBlend);
        label->setVisible(originalVisibility);
        label->setPosition(originalPos);

		// for test
		//char file[100];
		//sprintf(file,"../../temp/%s_creat.png",label->getString());
		//rt->saveToFile(file,kCCImageFormatPNG);
        return rt;
    }

	void UILabelTTF::setColor(const ccColor3B& color)
	{
		if(m_labelTTf)
		{
			m_labelTTf->setColor(color);
			updateRenderTexture();
		}
	}
    
	ccColor3B UILabelTTF::getColor(void)
	{
		if(m_labelTTf)
		{
			return m_labelTTf->getColor();
		}
		return ccWHITE;
	}

    void UILabelTTF::setString(const char *label)
    {
        if(m_labelTTf)
        {
            m_labelTTf->setString(label);
            updateRenderTexture();
        }
    }

	void UILabelTTF::setStringWithColor(const char *label,const ccColor3B& color)
	{
		if(m_labelTTf)
		{
			m_labelTTf->setColor(color);
			m_labelTTf->setString(label);
			updateRenderTexture();
		}
	}

    const char* UILabelTTF::getString(void)
    {
        if(m_labelTTf)
        {
            return m_labelTTf->getString();
        }
        else
        {
            return NULL;
        }
    }

	void UILabelTTF::setOpacity(GLubyte opacity)
	{
		/*if(m_renderTexture && m_renderTexture->getSprite())
		{
			m_renderTexture->getSprite()->setOpacity(opacity);
		}*/
		if(m_labelTTf)
		{
			m_labelTTf->setOpacity(opacity);
		}
	}
    
	GLubyte UILabelTTF::getOpacity()
	{
		/*if(m_renderTexture && m_renderTexture->getSprite())
		{
			return m_renderTexture->getSprite()->getOpacity();
		}*/

		if(m_labelTTf)
		{
			return m_labelTTf->getOpacity();
		}
		return 0;
	}

	void UILabelTTF::setOpacityModifyRGB(bool bValue)
	{
		if(m_renderTexture && m_renderTexture->getSprite())
		{
			m_renderTexture->getSprite()->setOpacityModifyRGB(bValue);
		}
		//updateRenderTexture();
	}

	bool UILabelTTF::isOpacityModifyRGB(void)
	{
		if(m_renderTexture && m_renderTexture->getSprite())
		{
			return m_renderTexture->getSprite()->isOpacityModifyRGB();
		}
		return false;
	}

    void UILabelTTF::updateRenderTexture()
    {
		return;
        CCAssert(m_labelTTf != NULL , "UILabelTTF's label is NULL");
		if(m_labelTTf->getTexture() == NULL)
		{
			if(m_renderTexture)
			{
				m_renderTexture->removeFromParentAndCleanup(true);
				m_renderTexture = NULL;
			}
			return;
		}
        CCPoint originalPos = CCPointZero;
        ccColor3B originalColor = m_labelTTf->getColor();
        bool originalVisibility = m_labelTTf->isVisible();
        m_labelTTf->setColor(m_renderColer);
        m_labelTTf->setVisible(true);
        ccBlendFunc originalBlend = m_labelTTf->getBlendFunc();
        ccBlendFunc tBlendFunc = {GL_SRC_ALPHA, GL_ONE };
        m_labelTTf->setBlendFunc(tBlendFunc);
        CCPoint center = ccp(m_labelTTf->getTexture()->getContentSize().width/2 + m_renderSize,m_labelTTf->getTexture()->getContentSize().height /2 +m_renderSize);
				
		if(m_renderTexture)
		{
			m_renderTexture->removeFromParentAndCleanup(true);
		}
		m_renderTexture = CCRenderTexture::create(m_labelTTf->getContentSize().width + m_renderSize*2, m_labelTTf->getContentSize().height + m_renderSize*2);
		addChild(m_renderTexture);
		m_renderTexture->setAnchorPoint(ccp(0,0));
		if(m_renderTexture->getSprite() && m_renderTexture->getSprite()->getTexture())
		{
			m_renderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
			m_renderTexture->getSprite()->setOpacityModifyRGB(true);
		}
		else
		{
			CCLOG("UILabelTTF null texture");
		}
		
		m_renderTexture->clear(0, 0, 0, 0);
		m_renderTexture->begin();
		m_labelTTf->setPosition(ccp(center.x - m_renderSize, center.y));
		m_labelTTf->visit();
		m_labelTTf->setPosition(ccp(center.x + m_renderSize, center.y - m_renderSize));
		m_labelTTf->visit();
		m_labelTTf->setPosition(ccp(center.x, center.y + m_renderSize));
		m_labelTTf->visit();
		m_labelTTf->setPosition(ccp(center.x + m_renderSize, center.y));
		m_labelTTf->visit();
        m_labelTTf->setColor(originalColor);
        m_labelTTf->setPosition(ccp(center.x,center.y));
        m_labelTTf->visit();
        m_renderTexture->end();

        m_labelTTf->setColor(originalColor);
        m_labelTTf->setBlendFunc(originalBlend);
        m_labelTTf->setVisible(originalVisibility);
        m_renderTexture->setPosition(originalPos);

		//for test
		/*char file[100];
		sprintf(file,"../../temp/%s%s.png",m_labelTTf->getString(),"_up");
		m_renderTexture->saveToFile(file,kCCImageFormatPNG);*/
		
    }
    
    void UILabelTTF::visit()
    {
		CCNode::visit();
#if 0
        // quick return if not visible. children won't be drawn.
        if (!m_bVisible)
        {
            return;
        }
        
        kmGLPushMatrix();
        
        if (m_pGrid && m_pGrid->isActive())
        {
            m_pGrid->beforeDraw();
        }
        
        this->transform();

		if(m_renderTexture)
		{
			m_renderTexture->visit();
		}

        if(m_labelTTf && m_labelTTf->isVisible())
        {
            m_labelTTf->setVisible(false);
        }

        m_uOrderOfArrival = 0;
        
        if (m_pGrid && m_pGrid->isActive())
        {
            m_pGrid->afterDraw(this);
        }
        
        kmGLPopMatrix();
#endif
    }

	void UILabelTTF::setAnchorPoint(const CCPoint &anchorPoint)
	{
		CCNode::setAnchorPoint(anchorPoint);
		if(m_labelTTf)
		{
			m_labelTTf->setAnchorPoint(anchorPoint);
		}
		if(m_renderTexture)
		{
			m_renderTexture->setAnchorPoint(anchorPoint);
		}
	}

	CCSize UILabelTTF::getContentSize()
	{
		/*if(m_renderTexture && m_renderTexture->getSprite() && m_renderTexture->getSprite()->getTexture())
		{
		return m_renderTexture->getSprite()->getTexture()->getContentSize();
		}*/
		if(m_labelTTf)
		{
			return m_labelTTf->getContentSize();
		}
		return CCSizeZero;
	}

	void UILabelTTF::listenToBackground(cocos2d::CCObject *obj)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
		CCLOG("UILabelTTF::listenToBackground");
		updateRenderTexture();
#endif
	}

	CCLabelTTF* UILabelTTF::GetTTF()
	{
		return m_labelTTf;
	}

	void UILabelTTF::SetDepthTest( bool b )
	{
		if(m_labelTTf)
		{
			m_labelTTf->SetDepthTest(b);
		}
	}

	void UILabelTTF::CreateStroke( void )
	{
		return;

		if(m_labelTTf)
		{
			// TODO : 
			GLchar* pszFragSource =
				"#ifdef GL_ES \n \
				precision mediump float; \n \
				#endif \n \
				uniform float u_coef;	\n \
				uniform sampler2D u_texture; \n \
				varying vec2 v_texCoord; \n \
				varying vec4 v_fragmentColor; \n \
				void main(void) \n \
				{ \n \
					// Convert to greyscale using NTSC weightings \n \
					//calculate the fragment color based on the texture and the vertex colour \n \
					vec4 basecolor = texture2D(u_texture, v_texCoord); \n \
					basecolor *= v_fragmentColor;	\n	\
					if(basecolor.a < 0.8 && basecolor.a > 0.0) \n \
					{ \n \
					basecolor.r = 1.0f;	\n \
					basecolor.g = 0.0f;	\n \
					basecolor.b = 0.0f;	\n \
					basecolor.a = 1.0f;	\n \
						vec2 texCoordLeft = v_texCoord; \n \
						texCoordLeft.x -= 1;	\n \
						vec4 colorLeft = texture2D(u_texture, texCoordLeft); \n \
						if(colorLeft.a != 0.0)	\n \
						{	\n \
							basecolor.r = 1.0f;	\n \
							basecolor.g = 0.0f;	\n \
							basecolor.b = 0.0f;	\n \
							basecolor.a = 1.0f;	\n \
						}	\n \
						vec2 texCoordRight = v_texCoord; \n \
						texCoordRight.x += 1;	\n \
						vec4 colorRight = texture2D(u_texture, texCoordRight); \n \
						if(colorRight.a != 0.0)	\n \
						{	\n \
						basecolor.r = 1.0f;	\n \
						basecolor.g = 0.0f;	\n \
						basecolor.b = 0.0f;	\n \
						basecolor.a = 1.0f;	\n \
						}	\n \
					} \n \
					gl_FragColor = basecolor; \n \
				}";

			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
			m_labelTTf->setShaderProgram(pProgram);
			pProgram->release();
			CHECK_GL_ERROR_DEBUG();

			m_labelTTf->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			m_labelTTf->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			m_labelTTf->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			CHECK_GL_ERROR_DEBUG();


			m_labelTTf->getShaderProgram()->link();
			CHECK_GL_ERROR_DEBUG();

			m_labelTTf->getShaderProgram()->updateUniforms();
			CHECK_GL_ERROR_DEBUG();

			/*GLuint location = glGetUniformLocation(this->getShaderProgram()->getProgram(), "u_coef");
			this->getShaderProgram()->setUniformLocationWith1f(location, 0.0f);*/
			//glUniform1f(m_location, m_coef);
			CHECK_GL_ERROR_DEBUG();
		}

	}

}