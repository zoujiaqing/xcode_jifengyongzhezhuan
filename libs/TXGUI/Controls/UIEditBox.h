#ifndef AHERO_UIEDITERBOX_H
#define AHERO_UIEDITERBOX_H

#include "TXGUI.h"
#include "cocos2d.h"
#include "UIControlBase.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

namespace TXGUI
{
	class UIEditBox : public UIControlBase
	{
		NiDeclareRTTI;
		NiDeclareStream;  
	
		

	public:
		~UIEditBox();
		virtual void BindNode(CCNode* pNode);

		void setString(const char *label);

		const char* getString();

		virtual void setPosition(CCPoint pt);

		/*virtual void setTextInputField(TextInputField *pTextInputField);*/

		virtual void setLengthLimit(int value);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		virtual void setIsVisible(bool isVisible);

		virtual void clearString();

		virtual void becomeFirstResponder();
		virtual void resignFirstResponder();

	protected:
		UIEditBox();

	protected:        
		CCEditBox* m_pEditBox;
	};
}

#endif	