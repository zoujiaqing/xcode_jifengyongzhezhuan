//
//  FlurryMan.h
//  FacebookDemo
//
//  Created by caesarhu on 14-5-4.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AppStoreAnalyseConfig.h"

@interface FlurryMan : NSObject


/**
 *  初始化Flurry SDK
 *
 *  @param channel           渠道
 *  @param isTrackLocation   是否记录地理位置
 *  @param isNeedCrashReport 是否记录Crash报告
 *  @param isAppStore        是否是AppStore的包
 */
+ (void)initFlurryWithChannel:(NSString *)channel
              isTrackLocation:(BOOL)isTrackLocation
            isNeedCrashReport:(BOOL)isNeedCrashReport
                   isAppStore:(BOOL)isAppStore;

/**
 *  记录事件
 *
 *  @param eventName 事件名称
 *  @param dic       事件相关的参数，如果不需要就设为nil
 */
+ (void)trackEvent:(NSString *)eventName withParameters:(NSDictionary *)dic;


@end
