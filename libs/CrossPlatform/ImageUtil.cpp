//
//  ImageUtil.cpp
//  iSeer
//
//  Created by b a on 12-5-14.
//  Copyright (c) 2012дЙ __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ImageUtil.h"


void convertToGray(unsigned char *imgPixel,const int &width,const int &height)
{
    int wOff = 0;
	int pixOff = 0;
    for(int y = 0;y< height;y++)
	{
		pixOff = wOff;
		
		for (int x = 0; x<width; x++) 
		{
			//int alpha = (unsigned char)imgPixel[pixOff];
			int red = (unsigned char)imgPixel[pixOff];
			int green = (unsigned char)imgPixel[pixOff+1];
			int blue = (unsigned char)imgPixel[pixOff+2];
			
			int bw = (int)((red+green+blue)/3.0);
			
			imgPixel[pixOff] = bw;
			imgPixel[pixOff+1] = bw;
			imgPixel[pixOff+2] = bw;
			
			pixOff += 4;
		}
		wOff += width * 4;
	}
}

#define RGB(c,v) (c) + (0 -(c))*(v)/255 //╨зи╚узуж
#define RGBW(c,v) (c) + (255 -(c))*(v)/255  //╟ви╚узуж
//reference http://blog.csdn.net/maozefa/article/details/1772418
void convertToLight(unsigned char *imgPixel,const int &width,const int &height,int value)
{
    int wOff = 0;
	int pixOff = 0;
    for(int y = 0;y< height;y++)
	{
		pixOff = wOff;
		
		for (int x = 0; x<width; x++) 
		{
			//int alpha = (unsigned char)imgPixel[pixOff+3];
			int r = (unsigned char)imgPixel[pixOff];
			int g = (unsigned char)imgPixel[pixOff+1];
			int b = (unsigned char)imgPixel[pixOff+2];
			
			imgPixel[pixOff]    =  RGB(r,value);
			imgPixel[pixOff+1]  =  RGB(g,value);
			imgPixel[pixOff+2]  =  RGB(b,value);
			
			pixOff += 4;
		}
		wOff += width * 4;
	}
}