// for win32

#include "HtmlStringDivider.h"
#include "GameFontManager.h"
#include "cocos2d.h"
using namespace cocos2d;

HtmlStringDivider::HtmlStringDivider()
{

}

HtmlStringDivider::~HtmlStringDivider()
{

}

std::vector<std::string> HtmlStringDivider::divideString(float width,const char* font,unsigned int fontSize,const char* content,int firstLineWidth)
{
	std::vector<std::string> ret;
	CCLabelTTF* pLabel = CCLabelTTF::create(content,KJLinXin,fontSize);
	if(pLabel->getContentSize().width <= firstLineWidth)
	{
		std::string str = content;
		ret.push_back(str);
		return ret;
	}

	unsigned short* unicodeStr = cc_utf8_to_utf16(content);
	unsigned int totalLength = cc_wcslen(unicodeStr);
	unsigned int tempLength = 0;
	unsigned int startStr = 0;
	bool isFirstLine = true;
	while(tempLength < totalLength)
	{
		long* writeErr = NULL;
		long* readErr = NULL;
		tempLength++;
		char* tempChar = cc_utf16_to_utf8(unicodeStr + startStr,tempLength - startStr,readErr,writeErr);
		CCLabelTTF* pLabel = CCLabelTTF::create(tempChar,KJLinXin,fontSize);
		int lineLength = isFirstLine ? firstLineWidth : width;
		if(pLabel->getContentSize().width > lineLength)
		{
			char* temp = cc_utf16_to_utf8(unicodeStr + startStr,tempLength - startStr -1,readErr,writeErr);
			std::string str = temp;
			ret.push_back(str);
			startStr = tempLength - 1;
			isFirstLine = false;
		}
		CC_SAFE_DELETE(tempChar);
		
	}
	if(startStr != tempLength)
	{
		long* writeErr = NULL;
		long* readErr = NULL;
		char* temp = cc_utf16_to_utf8(unicodeStr + startStr,tempLength - startStr,readErr,writeErr);
		std::string str = temp;
		ret.push_back(str);
	}
	return ret;
}