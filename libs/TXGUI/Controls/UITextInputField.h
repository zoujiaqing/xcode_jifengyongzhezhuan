// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#ifndef iSeer_UITextInputField_h
#define iSeer_UITextInputField_h

#include "TXGUI.h"
#include "UILayerColorBase.h"
#include "UIControlBase.h"
#include "TextInputField.h"

namespace TXGUI {
	USING_NS_CC;

	class UITextInputField : public UIControlBase
	{       
		NiDeclareRTTI;
		NiDeclareStream;  
	public:
		UITextInputField(const char *placeholder,
			const char *fontName,
			float fontSize, 
			cocos2d::CCNode* parent);

		UITextInputField(const char *placeholder,
			cocos2d::CCSize dimension,
			cocos2d::CCTextAlignment alignment,
			const char *fontName,
			float fontSize, 
			cocos2d::CCNode* parent);

		UITextInputField(TextInputField *pTextInputField, cocos2d::CCNode *parent);

		UITextInputField(std::string name, int tag, float z, UIControlBase *parent, TextInputField *textInputField);

		virtual   ~UITextInputField();   

		virtual void BindNode(CCNode* pNode);

		void setString(const char *label);

		const char* getString();

		virtual void setPosition(CCPoint pt);

		virtual void setTextInputField(TextInputField *pTextInputField);

		virtual void setLengthLimit(int value);

		virtual void setIsPassword(bool value);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setIsVisible(bool isVisible);

		virtual void clearString();
        
        virtual void becomeFirstResponder();
        virtual void resignFirstResponder();

	protected:
		UITextInputField();

	protected:        
		TextInputField *m_pTextInputField;
		CCSprite *m_pWhiteBack;

	private:
		void CreateWhiteBack();
	};
}


#endif
