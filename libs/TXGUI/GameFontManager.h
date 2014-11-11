// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#ifndef iSeer_GameFontManager_h
#define iSeer_GameFontManager_h

/*#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define KJLinXin   "STHeitiSC-Medium" 
#define KHelvetica "STHeitiSC-Medium"
#define KTongXin "STHeitiSC-Medium"
#else
// #define KJLinXin   "fonts/DroidSansFallback.ttf"
// #define KHelvetica "fonts/DroidSansFallback.ttf"
// #define KTongXin "fonts/TongXin.ttf"

#define KJLinXin   "Hevetica"
#define KHelvetica "Hevetica"
#define KTongXin "Hevetica"

#endif*/
#define KJLinXin   "fonts/Fzpc-y4.ttf"
#define KHelvetica "fonts/Fzpc-y4.ttf"
#define KTongXin "fonts/Fzpc-y4.ttf"

#define FONT_YELLOW             ccc3(153, 255, 0)
#define SPRITE_NAME_FONT_COLOR  ccc3(102,255,255)
#define FONT_COLOR_CYAN         ccc3(0,255,255)
#define BLUE                    ccc3(0, 255, 255)


class GameFontManager
{
public:
    static float superFontSize();
    static float largeFontSize();
    static float smallFontSize();
    static float miniFontSize();
    static float createPlayerLabelFontSize();
    static float smallTitleFontSize();
    static float smallTipFontSize();
    static float captureItemTipNameFontSize();
    static float captureItemTipCountFontSize();
    static float captureLayerFontSize();
};


#endif

