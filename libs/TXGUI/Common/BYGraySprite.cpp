//
// BYGraySprite.cpp
//

#include "BYGraySprite.h"

BYGraySprite::BYGraySprite()
{
}


BYGraySprite::~BYGraySprite()
{
}


BYGraySprite* BYGraySprite::create( const char* pszFileName ){
	BYGraySprite* graySprite = new BYGraySprite;
	if (graySprite && graySprite->initWithFile(pszFileName)){
		graySprite->autorelease();
		return graySprite;
	}else{
		CC_SAFE_RELEASE(graySprite);
		return NULL;
	}
}

BYGraySprite* BYGraySprite::create()
{
	BYGraySprite *pSprite = new BYGraySprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

BYGraySprite* BYGraySprite::createWithTexture(CCTexture2D *pTexture)
{
	BYGraySprite *pobSprite = new BYGraySprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

bool BYGraySprite::initWithTexture(CCTexture2D *pTexture)
{
	return CCSprite::initWithTexture(pTexture);
}

bool BYGraySprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect ){
	do{
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));


		GLchar* pszFragSource =
			"#ifdef GL_ES \n \
			precision mediump float; \n \
			#endif \n \
			uniform sampler2D u_texture; \n \
			varying vec2 v_texCoord; \n \
			varying vec4 v_fragmentColor; \n \
			void main(void) \n \
			{ \n \
			// Convert to greyscale using NTSC weightings \n \
			//calculate the fragment color based on the texture and the vertex colour \n \
			vec4 basecolor = texture2D(u_texture, v_texCoord); \n \
			vec4 frameColor = basecolor; \n \
			frameColor.r = (basecolor.r + basecolor.g + basecolor.b) / 3.0; \n \
			frameColor.g = (basecolor.r + basecolor.g + basecolor.b) / 3.0; \n \
			frameColor.b = (basecolor.r + basecolor.g + basecolor.b) / 3.0; \n \
			if(basecolor.a == 0.0) \n \
			{ \n \
			//throw this fragment away \n \
			discard; \n \
			} \n \
			gl_FragColor = frameColor; \n \
			}";


		CCGLProgram* pProgram = new CCGLProgram();
		pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
		this->setShaderProgram(pProgram);
		pProgram->release();
		CHECK_GL_ERROR_DEBUG();


		this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();


		this->getShaderProgram()->link();
		CHECK_GL_ERROR_DEBUG();


		this->getShaderProgram()->updateUniforms();
		CHECK_GL_ERROR_DEBUG();


		return true;
	} while (0);
	return false;
}


void BYGraySprite::draw(){
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );


	this->getShaderProgram()->use();
	this->getShaderProgram()->setUniformForModelViewProjectionMatrix();


	ccGLBindTexture2D( this->getTexture()->getName() );


#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;


	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));


	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));


	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CC_INCREMENT_GL_DRAWS(1);
}

//根据现有CCSprite，变亮和变灰
//http://blog.csdn.net/chenli0741/article/details/8860036
CCSprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight)
{
    //CCSprite转成CCimage
    CCPoint p = oldSprite->getAnchorPoint();
    oldSprite->setAnchorPoint(ccp(0,0));
    CCRenderTexture *outTexture = CCRenderTexture::create((int)oldSprite->getContentSize().width,(int)oldSprite->getContentSize().height);
    outTexture->begin();
    oldSprite->visit();
    outTexture->end();
    oldSprite->setAnchorPoint(p);
    #if 0
	//use shader to change gray
    CCImage* finalImage = outTexture->newCCImage();
 
    CCTexture2D *texture = new CCTexture2D;
    texture->initWithImage(finalImage);
    BYGraySprite* newSprite = BYGraySprite::createWithTexture(texture);
    delete finalImage;
    texture->release();
	#else
	//not use shader
	 CCImage* finalImage = outTexture->newCCImage();
    unsigned char *pData = finalImage->getData();
    int iIndex = 0;
    
    if(isLight)
    {
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // highlight
                int iHightlightPlus = 50;
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++;  //原来的示例缺少
                iB = (iB + iHightlightPlus > 255 ? 255 : iB + iHightlightPlus);
                iG = (iG + iHightlightPlus > 255 ? 255 : iG + iHightlightPlus);
                iR = (iR + iHightlightPlus > 255 ? 255 : iR + iHightlightPlus);
                //            iR = (iR < 0 ? 0 : iR);
                //            iG = (iG < 0 ? 0 : iG);
                //            iB = (iB < 0 ? 0 : iB);
                pData[iBPos] = (unsigned char)iB;
                pData[iBPos + 1] = (unsigned char)iG;
                pData[iBPos + 2] = (unsigned char)iR;
            }
        }
    }else{
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // gray
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++; //原来的示例缺少
                unsigned int iGray = 0.3 * iR + 0.4 * iG + 0.2 * iB;
                pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
            }
        }
    }
    
    CCTexture2D *texture = new CCTexture2D;
    texture->initWithImage(finalImage);
    CCSprite* newSprite = CCSprite::createWithTexture(texture);
    delete finalImage;
    texture->release();
    
	#endif
    return newSprite;
}
