//
//  ImageUtil.h
//  iSeer
//
//  Created by b a on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iSeer_ImageUtil_h
#define iSeer_ImageUtil_h



//把彩色图转化为灰度图
void convertToGray(unsigned char *imgPixel,const int &width,const int &height);

//明度转换
void convertToLight(unsigned char *imgPixel,const int &width,const int &height,int value);


#endif
