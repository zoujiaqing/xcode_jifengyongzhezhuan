//
//  HtmlNode.cpp
//  TestHtmlLabel
//
//  Created by rickycheng  on 13-8-14.
//
//

#include "HtmlNode.h"
#include "HtmlStringDivider.h"
//#include <iconv.h>
#include "TXGUIHeader.h"
#include "ccUTF8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//utf8◊÷∑˚≥§∂»1-6£¨ø…“‘∏˘æ›√ø∏ˆ◊÷∑˚µ⁄“ª∏ˆ◊÷Ω⁄≈–∂œ’˚∏ˆ◊÷∑˚≥§∂»
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//
unsigned char utf8_look_for_table[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define UTFLEN(x)  utf8_look_for_table[(x)]

int getUtf8Length(const char *str)
{
    int clen = strlen(str);
    int len = 0;
    for(const char *ptr = str;
        *ptr!=0&&len<clen;
        len++, ptr+=UTFLEN((unsigned char)*ptr));
    
    return len;
}

char* subUtfString(const char *str, unsigned int start, unsigned int end)
{
    int len = getUtf8Length(str);
    
    if(start >= len) return NULL;
    if(end > len) end = len;
    
    const char *sptr = str;
    for(int i = 0; i < start; ++i,sptr+=UTFLEN((unsigned char)*sptr));
    
    const char *eptr = sptr;
    for(int i = start; i < end; ++i,eptr += UTFLEN((unsigned char)*eptr));
    
    int retLen = eptr - sptr;
    char *retStr = (char*)malloc(retLen+1);
    memcpy(retStr, sptr, retLen);
    retStr[retLen] = 0;
    return retStr;
}


USING_NS_CC;
USING_NS_CC_EXT;

static double float_max(double a,double b)
{
    return (b<a)?a:b;
}

HtmlNode::HtmlNode()
{
    m_childTag = 0;
    m_curUseWidth = 0;
	m_new_line = false;
}

HtmlNode::~HtmlNode()
{
}

HtmlNode* HtmlNode::create(float nodeWidth)
{
    HtmlNode* pret = new HtmlNode;
    pret->init(nodeWidth);
    pret->autorelease();
    return pret;
}

bool HtmlNode::init(float nodeWidth)
{
    CCNode* node = CCNode::create();
    addChild(node,0,100);
    node->setAnchorPoint(ccp(0,0));
    node->setContentSize(CCSize(nodeWidth,0));
    return true;
}

void HtmlNode::begin()
{
}

void HtmlNode::addLabel(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color)
{
    //CCLOG("addLabel\n");
    const char *string3 = strstr(strContent,"\n");
	if(string3 !=NULL){
		CCLOG("string has new line symbol");
		m_new_line = true;
	}
    CCNode* node = (CCNode*)this->getChildByTag(100);
	float restWidth = getRestOneLineLength();
    std::vector<std::string> strvec = HtmlStringDivider::divideString(node->getContentSize().width,fontname,fontsize,strContent,restWidth);
    if(strvec.size() > 0)
	{
		for(std::vector<std::string>::iterator it = strvec.begin();
			it != strvec.end(); it ++)
		{
			CCLabelTTF* pLabel = CCLabelTTF::create((*it).c_str(),fontname,fontsize,CCSizeZero,kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
			pLabel->setColor(color);
			pLabel->setAnchorPoint(ccp(0,0));
			if(pLabel->getContentSize().width != 0 && pLabel->getContentSize().height != 0)
			{
				node->addChild(pLabel,0,m_childTag++);
			}
		}
	}

    //cutString2(node,color,strContent,fontsize,fontname,restWidth);
};

void HtmlNode::addHyperLink(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color,
                            CCObject* target, SEL_CCControlHandler touchDownAction, SEL_CCControlHandler touchUpInsideAction)
{
    CCLabelTTF* titleButton = CCLabelTTF::create(strContent,"Thonburi",fontsize,  CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    titleButton->setColor(color);
    
    //Add the button
    CCScale9Sprite *backgroundButton = CCScale9Sprite::create("button.png");
    CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create("buttonHighlighted.png");
    CCControlButton *controlButton = CCControlButton::create(titleButton, backgroundButton);
    controlButton->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
    controlButton->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
    backgroundButton->setAnchorPoint(ccp(0.5,0.5f));
    backgroundHighlightedButton->setAnchorPoint(ccp(0.5,0.5f));
    controlButton->setAnchorPoint(ccp(0.0f, 0));
    controlButton->setOpacity(0);
    
    //Sets up event handlers
    controlButton->addTargetWithActionForControlEvents(target, touchDownAction, CCControlEventTouchDown);
    controlButton->addTargetWithActionForControlEvents(target, touchUpInsideAction, CCControlEventTouchUpInside);
    
    CCNode* node = (CCNode*)this->getChildByTag(100);
    node->addChild(controlButton,0,m_childTag++);
}

void HtmlNode::addImage(const char* spriteFileName, CCObject* target, SEL_MenuHandler selector)
{
    CCSprite* normal = CCSprite::create(spriteFileName);
	CCNode* node = (CCNode*)this->getChildByTag(100);
	if(target != NULL && selector != NULL)
	{
		CCSprite* sel = CCSprite::create(spriteFileName);
    
		normal->setAnchorPoint(ccp(0,0.5));
		sel->setAnchorPoint(ccp(0,0.5));
    
		CCMenuItemSprite* menusprite = CCMenuItemSprite::create(normal, sel,target, selector);
		menusprite->setAnchorPoint(ccp(0,0.5));
		CCMenu* pmenu = CCMenu::create(menusprite,NULL);
		pmenu->setContentSize(menusprite->getContentSize());
		pmenu->setAnchorPoint(ccp(0,0));
		pmenu->setPosition(ccp(0,0));
		node->addChild(pmenu,0,m_childTag++);
	}
	else
	{   
		node->addChild(normal,0,m_childTag++);
	}
}

void HtmlNode::finish()
{
    arrangeNodePosition();
    CCNode* node = (CCNode*)this->getChildByTag(100);
    setContentSize(node->getContentSize());
}

void HtmlNode::arrangeNodePosition()
{
    CCNode* node = (CCNode*)this->getChildByTag(100);
    if(node->getChildrenCount() == 0)
        return;
    
    float nodeHight = 0;
    CCNode* prevChild = NULL;
    CCPoint prevPos = CCPointZero;
    CCPoint firstPos = prevPos;
    CCObject* pObject;
    CCARRAY_FOREACH(node->getChildren(), pObject)
    {
        CCNode* curChild = (CCNode*)pObject;
        
        if(prevChild == NULL)
        {
            prevChild = curChild;
            prevPos = prevChild->getPosition();
            firstPos = prevPos;
            nodeHight += prevChild->getContentSize().height;
        }
        else
        {
            float curpos = prevPos.x + prevChild->getContentSize().width + curChild->getContentSize().width;
            if(curpos > node->getContentSize().width)
            {
                float maxHeight = getOneLineMaxHeight(curChild->getTag());
                curChild->setPosition(ccp(firstPos.x, prevPos.y - maxHeight));
                prevChild = curChild;
                prevPos = curChild->getPosition();
                nodeHight += prevChild->getContentSize().height;
                node->setPosition(ccp(node->getPosition().x, node->getPosition().y + maxHeight));
            }
            else
            {
                curChild->setPosition(ccp(prevPos.x + prevChild->getContentSize().width, prevPos.y));
                prevChild = curChild;
                prevPos = curChild->getPosition();
            }
        }
    }
    float nodeWidth = node->getContentSize().width;
    node->setContentSize(CCSize(nodeWidth, nodeHight));
    
    CCARRAY_FOREACH(node->getChildren(), pObject)
    {
        CCNode* curChild = (CCNode*)pObject;
        curChild->setPosition(ccp(curChild->getPosition().x - node->getContentSize().width/2,
                                  curChild->getPosition().y));
    }
}

float HtmlNode::getOneLineMaxHeight(int starTag)
{
    CCNode* node = (CCNode*)this->getChildByTag(100);
    float maxHeight = 0;
    float curWidth = 0;
    CCNode* child = (CCNode*)node->getChildByTag(starTag);
    while(child && curWidth < node->getContentSize().width)
    {
        curWidth += child->getContentSize().width;
        if(curWidth < child->getContentSize().width)
        {
            maxHeight = float_max(maxHeight, child->getContentSize().height);
            starTag += 1;
            child = (CCNode*)node->getChildByTag(starTag);
        }
        else
        {
            if(maxHeight == 0)
                maxHeight = child->getContentSize().height;
            return maxHeight;
        }
    }
    return maxHeight;
}

void HtmlNode::cutString(CCNode *node, ccColor3B color, const char*strcont, int fontsize, const char* fontname, float bondwidth)
{
     int lenght = 0;
    
     char *sub = subUtfString(strcont,0,1);
     CCLabelTTF* testTile = CCLabelTTF::create(sub, fontname, fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
     int wordWide = testTile->getContentSize().width;
     free(sub);
     sub = NULL;
     
	while(true)
	{
		char *sub = subUtfString(strcont, 0, lenght+1);
		CCLabelTTF *titleButton = CCLabelTTF::create(sub, fontname, fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
        free(sub);
        float width = titleButton->getContentSize().width;
        
		if(width >= bondwidth-1)
		{
            lenght -= 1;
			break;
		}
		lenght++;
		if(lenght >= getUtf8Length(strcont))
		{
			break;
		}
	}
    
    if(lenght >= getUtf8Length(strcont))
    {
        CCLabelTTF* pLabel = CCLabelTTF::create(strcont,fontname,fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
        pLabel->setColor(color);
        pLabel->setAnchorPoint(ccp(0,0));
        node->addChild(pLabel,0,m_childTag++);
        return;
    }
    else
    {
        char* sub = subUtfString(strcont, 0, lenght); //strcon.substr(0,lenght-1);
         char* newstrcon = subUtfString(strcont, lenght, getUtf8Length(strcont));
        CCLabelTTF* pLabel = CCLabelTTF::create(sub,fontname,fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
        pLabel->setColor(color);
        pLabel->setAnchorPoint(ccp(0,0));
        node->addChild(pLabel,0,m_childTag++);
        
        free(sub);
        sub = NULL;
        cutString(node, color, newstrcon, fontsize, fontname, node->getContentSize().width);
		free(newstrcon);
		newstrcon = NULL ;
    }
}

void HtmlNode::cutString2(CCNode *node, ccColor3B color, const char*strcont, int fontsize, const char* fontname, float bondwidth)
{
    int lenght = 0;
    
    int totalWidth = 0;
	while(true)
	{
		char *sub = subUtfString(strcont, lenght, lenght+1);
        std::string str_sub = sub;
        if(m_WordMap.find(str_sub) == m_WordMap.end())
        {
            CCLabelTTF *titleButton = CCLabelTTF::create(sub, fontname, fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
            m_WordMap[str_sub] = titleButton->getContentSize().width;
            totalWidth +=  m_WordMap[str_sub];
        }
        else
            totalWidth += m_WordMap[str_sub];
            
        free(sub);
        
		if(totalWidth >= bondwidth-1)
		{
            lenght -= 1;
			break;
		}
		lenght++;
		if(lenght >= getUtf8Length(strcont))
		{
			break;
		}
	}
    
    if(lenght >= getUtf8Length(strcont))
    {
        CCLabelTTF* pLabel = CCLabelTTF::create(strcont,fontname,fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
        pLabel->setColor(color);
        pLabel->setAnchorPoint(ccp(0,0));
        node->addChild(pLabel,0,m_childTag++);
        return;
    }
    else
    {
        char* sub = subUtfString(strcont, 0, lenght); //strcon.substr(0,lenght-1);
         char* newstrcon = subUtfString(strcont, lenght, getUtf8Length(strcont));
        CCLabelTTF* pLabel = CCLabelTTF::create(sub,fontname,fontsize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
        pLabel->setColor(color);
        pLabel->setAnchorPoint(ccp(0,0));
        node->addChild(pLabel,0,m_childTag++);
        
        free(sub);
        sub = NULL;
        cutString(node, color, newstrcon, fontsize, fontname, node->getContentSize().width);
		free(newstrcon);
		newstrcon = NULL ;
    }
}

float HtmlNode::getRestOneLineLength()
{
    CCNode* node = (CCNode*)this->getChildByTag(100);
    if(m_childTag == 0)
        return node->getContentSize().width;
    if(m_new_line){
		m_new_line = false;
		 return node->getContentSize().width;
	}
    float curWidth = 0;
    int index = m_childTag-1;
    while(curWidth < node->getContentSize().width && index >= 0)
    {
        CCNode* lastChild = (CCNode*)node->getChildByTag(index);
        curWidth += lastChild->getContentSize().width;
        index -= 1;
    }
    if(curWidth > node->getContentSize().width)
    {
        CCNode* lastChild = (CCNode*)node->getChildByTag(index+1);
        if(lastChild)
            curWidth -= lastChild->getContentSize().width;
    }
    
    float width = node->getContentSize().width - curWidth;
    if(width == 0)
        width = curWidth;
    return width;
}

//added by wesly
int HtmlNode::addHyperLink(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color,int touchLuaFunc,int priority,bool isButtonEnable)
{
	CCLabelTTF* titleButton = CCLabelTTF::create(strContent,fontname,fontsize,  CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
	titleButton->setColor(color);
    
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(titleButton);
	itemLabel->setTag(touchLuaFunc);
	itemLabel->setAnchorPoint(ccp(0,0));
	itemLabel->registerScriptTapHandler(touchLuaFunc);
    itemLabel->setEnabled(isButtonEnable);
	UIMenu* menu = UIMenu::create(itemLabel,NULL);
	menu->setAnchorPoint(ccp(0,0.5));
	menu->setInitPriority(priority);
	menu->setContentSize(itemLabel->getContentSize());
    
	CCNode* node = (CCNode*)this->getChildByTag(100);
	node->addChild(menu,0,m_childTag++);
	return touchLuaFunc;
}

void HtmlNode::addImage(CCSprite* sprite)
{
    if(sprite)
	{
		sprite->setAnchorPoint(ccp(0,0));
		CCNode* node = (CCNode*)this->getChildByTag(100);
		node->addChild(sprite,0,m_childTag++);
	}

}

void HtmlNode::initWordMap()
{
    /*
    std::string str1[] = {"a","b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "i", "s", "t", "u", "v", "w", "x", "y", "z",
        "A","B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "I", "S", "T", "U", "V", "W", "X", "Y", "Z",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    
    int val[] = {8,7,6,7,6,6,7,8,4,4,
        7,4,11,8,7,7,7,6,6,6,
        7,8,11,7,8,7,
        
        10,8,7,8,7,7,8,8,4,7,
        9,7,14,9,9,7,9,9,8,8,
        9,9,15,10,9,9,
        
        9,9,9,9,9,9,9,9,9,9,};
    
    int size = sizeof str1 / sizeof str1[0];
    for(int i=0; i<size; i++)
    {
        std::string key = str1[i];
        m_WordMap[key] = val[i];
    }*/
    
}








