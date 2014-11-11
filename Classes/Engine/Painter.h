//
//  Painter.h
//  iSeer
//
//  Created by wuwei on 2/13/12.
//  Copyright 2012 Taomee.Inc. All rights reserved.
//

#ifndef _PAINTER_H_
#define _PAINTER_H_

#include "cocos2d.h"

class Painter
{
public:
    static void drawCircle(cocos2d::CCPoint c, float r);
    static void drawFan(cocos2d::CCPoint c, float r, float a1, float a2);
	static void drawRect(cocos2d::CCRect rect,GLubyte r, GLubyte g, GLubyte b, GLubyte a);
};

#endif