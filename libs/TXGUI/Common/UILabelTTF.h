//
//  UILabelTTF.h
//  HelloLua
//
//  Created by Wesley Yin on 3/22/13.
//
//

#ifndef HelloLua_UILabelTTF_h
#define HelloLua_UILabelTTF_h

#include "cocos2d.h"
using namespace cocos2d;

namespace TXGUI {
    class UILabelTTF : public cocos2d::CCNode ,public CCRGBAProtocol
    {
    public:
        UILabelTTF();
        virtual ~UILabelTTF();
        
        static UILabelTTF * create(const char *string, const char *fontName, float fontSize);
        
        /** creates a CCLabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
         @since v2.0.1
         */
        static UILabelTTF * create(const char *string, const char *fontName, float fontSize,
                                   const CCSize& dimensions, CCTextAlignment hAlignment);
        
        /** creates a CCLabel from a fontname, alignment, dimension in points and font size in points
         @since v2.0.1
         */
        static UILabelTTF * create(const char *string, const char *fontName, float fontSize,
                                   const CCSize& dimensions, CCTextAlignment hAlignment,
                                   CCVerticalTextAlignment vAlignment);
        
        bool initWithString(const char *string, const char *fontName, float fontSize,
                            const CCSize& dimensions, CCTextAlignment hAlignment,
                            CCVerticalTextAlignment vAlignment);
        
        virtual void setString(const char *label);
		virtual void setStringWithColor(const char *label,const ccColor3B& color);
        virtual const char* getString(void);
		virtual void setOpacity(GLubyte opacity);
		virtual GLubyte getOpacity(void);
		virtual void setOpacityModifyRGB(bool bValue);
		virtual bool isOpacityModifyRGB(void);
		virtual void setColor(const ccColor3B& color);
		virtual ccColor3B getColor(void);
        virtual void visit(void);

		virtual CCSize getContentSize();

		virtual void setAnchorPoint(const CCPoint &anchorPoint);

        CCRenderTexture* createStroke(CCLabelTTF* label, float size ,ccColor3B cor);

		void listenToBackground(CCObject *obj);

		CCLabelTTF* GetTTF();

		void SetDepthTest(bool b);

    private:
        virtual void updateRenderTexture();

		void CreateStroke(void);

    private:
        CCLabelTTF* m_labelTTf;
        CCRenderTexture* m_renderTexture;
        ccColor3B   m_renderColer;
        float       m_renderSize;
    };
}

#endif
