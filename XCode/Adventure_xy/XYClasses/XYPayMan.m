//
//  XYPayMan.m
//  XYDemo
//
//  Created by caesarhu on 14-8-5.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "XYPayMan.h"
#import "OrderMan.h"
#import <XYPlatform/XYPlatform.h>
#import "XYLoginMain.h"

@interface XYPayMan ()<XYPayDelegate> {
    BOOL isBuying;
    NSString *serverID;
}

- (void)succCallback;
- (void)failedCallback;

@end

static XYPayMan *instance = nil;

@implementation XYPayMan
@synthesize delegate = _delegate;
@synthesize isInnerMode = _isInnerMode;

+ (XYPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[XYPayMan alloc] init];
        }
        
        return instance;
    }
    
    return nil;
}

+ (void)purgeSharedInstance
{
    @synchronized(self){
        if (!instance) {
            [instance release], instance = nil;
        }
    }
}

- (id)init
{
    if (self=[super init]) {
        //
        _isInnerMode = NO;
        isBuying = NO;
        [XYPlatform defaultPlatform].appScheme = APP_SCHEME;
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)doPayWithProductId:(NSString *)productId userId:(NSString *)userId channelId:(NSString *)channelId serverId:(NSString *)serverId currency:(NSString *)currency andUserCreateTime:(NSString *)createTime
{
    //[[NSNotificationCenter defaultCenter] removeObserver:[XYLoginMain sharedInstance] name:kXYPlatformLeavedNotification object:nil];
    if (isBuying) {
        return;
    }else {
        isBuying = YES;
    }
    serverID = serverId;
    
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

- (void)succCallback
{
    if ([_delegate respondsToSelector:@selector(didPaid)]) {
        [_delegate didPaid];
    }
    isBuying = NO;
}

- (void)failedCallback
{
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:nil];
    }
    isBuying = NO;
}

#pragma mark - order delegate

- (void)didFectchOrderWithProductId:(NSString *)productId amount:(NSString *)amount tradeNo:(NSString *)tradeNo productName:(NSString *)productName andAddInfo:(NSString *)addInfo
{
    int count = (int)[amount floatValue];
    [[XYPlatform defaultPlatform] XYPayWithAmount:[NSString stringWithFormat:@"%d", count] appServerId:serverID appExtra:tradeNo delegate:self];
    isBuying = NO;
}

- (void)didFailedFetchOrder
{
    NSLog(@"Network error.");
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
    isBuying = NO;
}

#pragma mark - xypaydelegate
- (void)XYPaySuccessWithOrder:(NSString *)orderId payAmount:(NSString *)amount {
    [self succCallback];
}

- (void)XYPayFailedWithOrder:(NSString *)orderId payAmount:(NSString *)amount {
    [self failedCallback];
}


@end
