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
#include "GameFontManager.h"
#include "cocos2d.h"
#include "CrossPlatform.h"


float GameFontManager::superFontSize() {
    static float baseSize = 35.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::largeFontSize() {
    static float baseSize = 26.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::smallFontSize() {
    static float baseSize = 20.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::miniFontSize() {
    static float baseSize = 16.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::createPlayerLabelFontSize() {
    static float baseSize = 32.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::smallTitleFontSize() {
    static float baseSize = 17.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::smallTipFontSize() {
    static float baseSize = 14.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::captureItemTipNameFontSize() {
    static float baseSize = 18.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::captureItemTipCountFontSize() {
    static float baseSize = 20.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

float GameFontManager::captureLayerFontSize() {
    static float baseSize = 20.0;
    
    return baseSize / cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}
