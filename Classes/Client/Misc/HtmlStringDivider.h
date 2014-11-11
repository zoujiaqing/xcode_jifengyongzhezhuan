#ifndef HTML_STRING_DIVIDER_H
#define HTML_STRING_DIVIDER_H

#include <vector>
#include <string>

class HtmlStringDivider
{
public:
	HtmlStringDivider();
	virtual ~HtmlStringDivider();

	static std::vector<std::string> divideString(float width,const char* font,unsigned int fontSize,const char* str,int firstLineWidth);
};

#endif
