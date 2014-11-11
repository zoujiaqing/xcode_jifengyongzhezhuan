//
//  HtmlNode.h
//  TestHtmlLabel
//
//  Created by rickycheng  on 13-8-14.
//
//

#ifndef TestHtmlLabel_HtmlNode_h
#define TestHtmlLabel_HtmlNode_h

#include "cocos2d.h"
#include "cocos-ext.h"

class HtmlNode : public cocos2d::CCNode
{
public:
    HtmlNode();
    virtual ~HtmlNode();
    
    static HtmlNode* create(float nodeWidth);
    virtual bool init(float nodeWidth);
    
    void begin();
    void addLabel(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color);
    void addImage(const char* spriteFileName, CCObject* target, cocos2d::SEL_MenuHandler selector);
    void addHyperLink(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color, CCObject* target, cocos2d::extension::SEL_CCControlHandler touchDownAction, cocos2d::extension::SEL_CCControlHandler touchUpInsideAction);
    
    int addHyperLink(const char* strContent, int fontsize, const char* fontname, cocos2d::ccColor3B color,int touchLuaFunc,int priority,bool isButtonEnable = true);
	void addImage(cocos2d::CCSprite* sprite);
    void finish();
    
private:
    void arrangeNodePosition();
    float getOneLineMaxHeight(int startag);
    void cutString(CCNode *node, cocos2d::ccColor3B color, const char*strcont, int fontsize, const char* fontname, float bondwidth);
    void cutString2(CCNode *node, cocos2d::ccColor3B color, const char*strcont, int fontsize, const char* fontname, float bondwidth);
    float getRestOneLineLength();
    
    void initWordMap();
private:
	bool m_new_line;
    int m_childTag;
    int m_curUseWidth;
    std::map<std::string, int> m_WordMap;
    std::vector<int> m_vecWidths;
};

#endif
