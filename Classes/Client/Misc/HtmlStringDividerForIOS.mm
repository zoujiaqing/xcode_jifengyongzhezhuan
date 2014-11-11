// for win32

#include "HtmlStringDivider.h"
#include "GameFontManager.h"
//#include "IosStringDivider.h"
#include "cocos2d.h"
using namespace cocos2d;

HtmlStringDivider::HtmlStringDivider()
{

}

HtmlStringDivider::~HtmlStringDivider()
{

}

std::vector<std::string> HtmlStringDivider::divideString(float width,const char* fontName,unsigned int fontSize,const char* content,int firstLineWidth)
{
	//std::vector<std::string> ret = IosStringDivider::DivideStringByWidth(font, content, fontSize, width, width);
	
	//return ret;
    
    std::vector<std::string> ret;
    //NSArray* tempArray = [[[NSArray alloc] init] autorelease];
    NSString *nsFontName = [[[NSString alloc] initWithUTF8String:fontName] autorelease];
    NSString *nsContent = [[[NSString alloc] initWithUTF8String:content] autorelease];
    id font;
    font = [UIFont fontWithName:nsFontName size:fontSize];
    if (! font)
    {
        font = [UIFont systemFontOfSize:fontSize];
    }
    if (font)
    {
        CGSize textRect = CGSizeZero;
        textRect.width = 0x7fffffff;
        textRect.height = 0x7fffffff;
        CGSize tmp = [nsContent sizeWithFont:font constrainedToSize:textRect];
        
        if (tmp.width <= firstLineWidth)
        {
            // this text can be draw in the first line
            std::string temp = content;
            ret.push_back(temp);
            return  ret;
        }
        
        bool isFirstLine = true;
        int startIndex = 0;
        int subStringLength = 1;
        int stringLength = [nsContent length];
        while(startIndex + subStringLength < stringLength)
        {
            int lineLength = isFirstLine ? firstLineWidth : width;
            NSRange range = NSMakeRange (startIndex, subStringLength);
            NSString *tempString = [nsContent substringWithRange:range];
            tmp = [tempString sizeWithFont:font constrainedToSize:textRect];
            if (tmp.width > lineLength)
            {
                NSRange subRange = NSMakeRange (startIndex, subStringLength - 1);
                NSString *contentString = [nsContent substringWithRange:subRange];
                std::string temp = [contentString UTF8String];
                ret.push_back(temp);
                isFirstLine = false;
                
                startIndex += subStringLength - 1;
                subStringLength = 1;
            }
            else
            {
                subStringLength++;
            }
        }
        
        if(startIndex < stringLength - 1)
        {
            NSRange subRange = NSMakeRange (startIndex, stringLength - startIndex);
            NSString *contentString = [nsContent substringWithRange:subRange];
            std::string temp = [contentString UTF8String];
            ret.push_back(temp);
        }
    }
    return ret;
}