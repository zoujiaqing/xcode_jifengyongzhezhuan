//
//  TrackingAppCPA.h
//  AppStoreAnalyse
//
//  Created by caesarhu on 14-7-15.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TrackingAppCPA : NSObject

+ (TrackingAppCPA *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  SDK初始化，在application: didFinishLaunchingWithOptions:中调用
 *
 *  @param channelID 渠道号
 */
- (void)startTrackingWithChannelID:(NSString *)channelID;

- (void)trackRegister:(NSString *)userId;

- (void)trackLogin:(NSString *)userId;

- (void)trackPayment:(NSString *)userId
             orderId:(NSString *)orderId
              amount:(int)amount
            currency:(NSString *)currency
             payType:(NSString *)payType;

- (void)event1;
- (void)event2;
- (void)event3;
- (void)event4;
- (void)event5;
- (void)event6;
- (void)event7;
- (void)event8;
- (void)event9;
- (void)event10;

@end
