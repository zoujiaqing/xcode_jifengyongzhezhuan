//
//  device.m
//  idou
//
//  Created by razer tong on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "IOSUtil.h"
#import "SimpleAudioEngine.h"



@implementation device

+(void)playEffect:(const char*)name delay:(float)delay
{
    [self performSelector:@selector(playByName:) withObject:[NSString stringWithUTF8String:name] afterDelay:delay];
}

+(void)playByName:(NSObject*)name
{
    NSString *effectName = ( NSString *)name;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect([effectName UTF8String]);
}

+(void)playBackgroundMusic:(const char* )name
{
    [self performSelectorInBackground:@selector(playBackgroundMusicByObject) withObject:[NSString stringWithUTF8String:name]]; 
}

+(void)playBackgroundMusicByObject:(NSObject*)name
{
     NSString *bgmName = ( NSString *)name;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic([bgmName UTF8String]);
}





@end
