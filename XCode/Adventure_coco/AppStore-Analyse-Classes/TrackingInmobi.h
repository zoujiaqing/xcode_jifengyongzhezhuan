//
//  TrackingInmobi.h
//  FacebookDemo
//
//  Created by caesarhu on 14-5-8.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IMBanner.h"

typedef enum IM_BannerSize {
    IM_BannerSize_320X48     =  IM_UNIT_320x48,
    IM_BannerSize_300X250    =  IM_UNIT_300x250,
    IM_BannerSize_728X90     =  IM_UNIT_728x90,
    IM_BannerSize_468X60     =  IM_UNIT_468x60,
    IM_BannerSize_120X600    =  IM_UNIT_120x600,
    IM_BannerSize_320X50     =  IM_UNIT_320x50
}IM_BannerSize;

@interface TrackingInmobi : NSObject

+ (TrackingInmobi *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  初始化，在application:didFinishLaunchingWithOptions:中调用
 */
- (void)startTracking;

/**
 *  记录地理位置
 */
- (void)trackLocation;

/**
 *  记录事件
 *
 *  @param event  事件名
 *  @param params 该事件相关的附加参数，如果不需要就传nil
 */
- (void)trackEvent:(NSString *)event withParamters:(NSDictionary *)params;

/**
 *  开始记录连续的事件
 *
 *  @param event  事件名
 *  @param params 该事件相关的附加参数，如果不需要就传nil
 */
- (void)beginTrackSectionEvent:(NSString *)event withParamters:(NSDictionary *)params;

/**
 *  结束记录连续的事件
 *
 *  @param event  事件名
 *  @param params 该事件相关的附加参数，如果不需要就传nil
 */
- (void)endTrackSectionEvent:(NSString *)event withParamters:(NSDictionary *)params;

/**
 *  记录交易
 *
 *  @param transacton 交易
 */
- (void)trackTransaction:(id)transacton;

/**
 *  横幅广告
 *
 *  @param rect 相对于父View的位置
 *  @param size InMobi支持的大小，在IM_BannerSize中选择
 *  @param view 父view
 *  @param sec  广告刷新间隔，单位秒。小于零关闭自动刷新，等于零是默认（60秒），大于零就是设置的刷新间隔
 */
- (void)showBannerWithRect:(CGRect)rect size:(IM_BannerSize)size inView:(UIView *)view refreshInterval:(int)sec;

- (void)showInterstitial;

@end
