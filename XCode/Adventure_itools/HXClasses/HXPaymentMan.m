//
//  HXPaymentMan.m
//  iToolsDemo
//
//  Created by caesarhu on 14-4-23.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "HXPaymentMan.h"
#import "HXAppPlatformKitPro.h"
#import "OrderMan.h"

static HXPaymentMan *instance = nil;

@interface HXPaymentMan() {
    BOOL isBuying;
}

@end

@implementation HXPaymentMan

@synthesize delegate = _delegate,isInnerMode = _isInnerMode;

- (id)init {
    if (self = [super init]) {
        //支付页面关闭通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closePayViewNotification:) name:HX_NOTIFICATION_CLOSE_PAYVIEW object:nil];
        isBuying = NO;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

+ (HXPaymentMan *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[HXPaymentMan alloc] init];
        }
        return instance;
    }
    return nil;
}

+ (void)purgeSharedInstance {
    @synchronized(self) {
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (void)doPayWithProductId:(NSString *)productId
                    userId:(NSString *)userId
                 channelId:(NSString *)channelId
                  serverId:(NSString *)serverId
                  currency:(NSString *)currency
         andUserCreateTime:(NSString *)createTime
{
    if (isBuying) {
        return;
    }else {
        isBuying = YES;
    }
    [OrderMan sharedInstance].productId = productId;
    [OrderMan sharedInstance].serverId = serverId;
    [OrderMan sharedInstance].userId = userId;
    [OrderMan sharedInstance].currency = currency;
    [OrderMan sharedInstance].userCreateTime =createTime;
    [OrderMan sharedInstance].channelId = channelId;
    [OrderMan sharedInstance].isInnerMode = self.isInnerMode;
    [OrderMan sharedInstance].delegate = self;
    [[OrderMan sharedInstance] fetchOrder];
}

#pragma mark -- order delegate

- (void)didFectchOrderWithProductId:(NSString *)productId
                             amount:(NSString *)amount
                            tradeNo:(NSString *)tradeNo
                        productName:(NSString *)productName
                         andAddInfo:(NSString *)addInfo
{
    //[HXAppPlatformKitPro setPayViewAmount:[amount floatValue] orderIdCom:tradeNo];
    [HXAppPlatformKitPro payProductWithProductName:productName Amount:[amount floatValue] OrderIdCom:tradeNo];
    isBuying = NO;
}

- (void)didFailedFetchOrder
{
    NSLog(@"failed fetch order info...");
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"get tradeNo error."];
    }
    isBuying = NO;
}

- (void)closePayViewNotification:(NSNotification *)notification {
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"Payment canceled."];
    }
    isBuying = NO;
}

@end
