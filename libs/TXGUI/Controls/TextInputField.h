#ifndef iSeer_TextInputField_h
#define iSeer_TextInputField_h

#include "cocos2d.h"

USING_NS_CC;

namespace TXGUI
{
	class TextInputField: public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate
	{
	private:

		CCPoint m_beginPos;


		CCSprite *m_pCursorSprite;


		CCAction *m_pCursorAction;


		CCPoint m_cursorPos;


		std::string m_strInputText;


		bool m_bIsPassword;


		std::string m_strShowText;


		bool m_bIsScheduled;


		int m_iLengthLimit;


		int m_priority;


		CCSize m_touchInputSize;


		CCPoint m_rootPos;
        
        bool b_isSelect;
	protected:
		// CCTextFieldTTF
		virtual const char *getContentText();

	public:
		TextInputField();
		~TextInputField();

		// static
		static TextInputField* textFieldWithPlaceHolder(const char *placeholder, CCSize dimensions, 
			CCTextAlignment alignment, const char *fontName, float fontSize);
		static TextInputField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

		// CCTextFieldTTF
		virtual void onEnter();
		virtual void onExit();

		void initCursorSprite(int nHeight);

		// CCTextFieldTTF
		virtual bool attachWithIME();
		virtual bool detachWithIME();

		// CCTextFieldDelegate
		virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
		virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
		virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
		virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);

        virtual void resetInputText(const char *label);
		// CCLayer Touch
		bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

		void setIsPassword(bool value);

		void ChangeToPassword(float dt);

		void setLengthLimit(int value);

		void setInitPriority(int priority);

		CCSize getDimensions();

		virtual const char *getString();

		virtual void clearString();

		void setTouchInputSize(CCSize touchSize);

	protected:
		virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);

		virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
	private:

		bool isInTextField(CCTouch *pTouch);

		CCRect getRect();
	};
}


#endif
