//
//  TrackingAppCPA.m
//  AppStoreAnalyse
//
//  Created by caesarhu on 14-7-15.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "TrackingAppCPA.h"
#import "TalkingDataAppCpa.h"
#import "AppStoreAnalyseConfig.h"

static TrackingAppCPA *instance = nil;

@implementation TrackingAppCPA

+ (TrackingAppCPA *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[TrackingAppCPA alloc] init];
        }
        return instance;
    }
    return instance;
}

+ (void)purgeSharedInstance {
    @synchronized(self) {
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (void)startTrackingWithChannelID:(NSString *)channelID {
    [TalkingDataAppCpa init:Tracking_AppCPA_App_Key withChannelId:channelID];
}

- (void)trackRegister:(NSString *)userId {
    [TalkingDataAppCpa onRegister:userId];
}

- (void)trackLogin:(NSString *)userId {
    [TalkingDataAppCpa onLogin:userId];
}

- (void)trackPayment:(NSString *)userId orderId:(NSString *)orderId amount:(int)amount currency:(NSString *)currency payType:(NSString *)payType{
    [TalkingDataAppCpa onPay:userId withOrderId:orderId withAmount:amount withCurrencyType:currency withPayType:payType];
}

- (void)event1 {
    [TalkingDataAppCpa onCustEvent1];
}
- (void)event2 {
    [TalkingDataAppCpa onCustEvent2];
}
- (void)event3 {
    [TalkingDataAppCpa onCustEvent3];
}
- (void)event4 {
    [TalkingDataAppCpa onCustEvent4];
}
- (void)event5 {
    [TalkingDataAppCpa onCustEvent5];
}
- (void)event6 {
    [TalkingDataAppCpa onCustEvent6];
}
- (void)event7 {
    [TalkingDataAppCpa onCustEvent7];
}
- (void)event8 {
    [TalkingDataAppCpa onCustEvent8];
}
- (void)event9 {
    [TalkingDataAppCpa onCustEvent9];
}
- (void)event10 {
    [TalkingDataAppCpa onCustEvent10];
}

@end
