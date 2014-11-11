//
//  CCInt.h
//  iSeer
//
//  Created by arthurgong on 11-12-27.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//

#ifndef __CCINT_H__
#define __CCINT_H__
#include <stdlib.h>
#include "CCObject.h"
namespace cocos2d {
    
	class CC_DLL CCInt : public CCObject
	{
	public:
		int m_int;
	public:
		CCInt()
        :m_int(0)
		{}
		CCInt(int value)
		{
			m_int = value;
		}
		~CCInt(){;};
		
		int getValue()
		{
			return m_int;
		}
	};
}// namespace cocos2d
#endif //__CCINT_H__