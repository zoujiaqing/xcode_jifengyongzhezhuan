//
//  CocoDataMan.h
//  AppStoreAnalyse
//
//  Created by caesarhu on 14-6-4.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CocoDataMan : NSObject

/**
 *  初始化方法，在application: didFinishLaunchingWithOptions:中调用
 *
 *  @param channel 渠道
 */
+ (void)didFinishLaunchingWithChannel:(NSString *)channel;

/**
 *  进入页面
 *
 *  @param viewName 页面名称
 */
+ (void)viewBegin:(NSString *)viewName;

/**
 *  离开页面
 *
 *  @param viewName 页面名称
 */
+ (void)viewEnd:(NSString *)viewName;

/**
 *  追踪事件
 *
 *  @param eventName 事件名称（自定义）
 *  @param label     事件标签（自定义），如果不需要就传nil
 *  @param params    事件参数 (key只支持NSString, value支持NSString和NSNumber)，如果不需要就传nil
 */
+ (void)trackEvent:(NSString *)eventName label:(NSString *)label params:(NSDictionary *)params;

@end
