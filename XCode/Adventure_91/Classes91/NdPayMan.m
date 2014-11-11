//
//  NdPayMan.m
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "NdPayMan.h"
#import "OrderMan.h"
#import <NdComPlatform/NdComPlatform.h>
#import <NdComplatform/NdCPNotifications.h>
#import <NdComPlatform/NdComPlatformError.h>

@interface NdPayMan () {
    BOOL isBuying;
}

- (void)addNdPayObservers;
- (void)removeNdPayObservers;
- (void)NdPayAysnResult:(NSNotification *)notify;
- (void)succCallback;
- (void)failedCallback;

@property (nonatomic, retain) NSString *errorDes;
@property (nonatomic, retain) NSDictionary *valueDict;
@end

static NdPayMan *instance = nil;

@implementation NdPayMan
@synthesize isInnerMode = _isInnerMode;
@synthesize delegate    = _delegate;
@synthesize errorDes;
@synthesize valueDict;

+ (NdPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[NdPayMan alloc] init];
        }
        
        return instance;
    }
    
    return nil;
}

+ (void)purgeSharedInstance
{
    @synchronized(self){
        if (instance) {
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
    }
    
    return self;
}

- (void)dealloc
{
    self.errorDes = nil;
    self.valueDict = nil;
    [super dealloc];
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

- (void)addNdPayObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(NdPayAysnResult:) name:kNdCPBuyResultNotification object:nil];
}

- (void)removeNdPayObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kNdCPBuyResultNotification object:nil];
}

- (void)NdPayAysnResult:(NSNotification *)notify
{
    NSLog(@"pay callback:%@", notify);
    
    NSDictionary *resultDic = [notify userInfo];
    BOOL paySuccess = [[resultDic objectForKey:@"result"] boolValue];
    if (paySuccess) {
        NSLog(@"pay succ.");
        [self performSelectorOnMainThread:@selector(succCallback) withObject:nil waitUntilDone:NO];
    }else{
        int errorCode = [[resultDic objectForKey:@"error"] intValue];
        switch (errorCode) {
            case ND_COM_PLATFORM_ERROR_USER_CANCEL:
                self.errorDes = @"User cancel.";
                break;
            case ND_COM_PLATFORM_ERROR_NETWORK_ERROR:
                self.errorDes = @"Network error.";
                break;
            case ND_COM_PLATFORM_ERROR_SERVER_RETURN_ERROR:
                self.errorDes = @"Server return error.";
                break;
            case ND_COM_PLATFORM_ERROR_ORDER_SERIAL_SUBMITTED:
                self.errorDes = @"Order serial submitted.";
                break;
            default:
                self.errorDes = @"Error in process.";
                break;
        }
        NSLog(@"pay failed");
        [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
        
    }
    isBuying = NO;
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
        [_delegate didPaidFailedWithErrorDescription:self.errorDes];
    }
    isBuying = NO;
}

#pragma mark -- order delegate

- (void)didFectchOrderWithProductId:(NSString *)productId
                             amount:(NSString *)amount
                            tradeNo:(NSString *)tradeNo
                        productName:(NSString *)productName
                         andAddInfo:(NSString *)addInfo
{
    NdBuyInfo *buyInfo = [[NdBuyInfo new]autorelease];
    buyInfo.cooOrderSerial = tradeNo;
    buyInfo.productId = productId;
    buyInfo.productName = productName;
    buyInfo.productPrice = [amount floatValue];
    buyInfo.productOrignalPrice = [amount floatValue];
    buyInfo.productCount = 1;
    buyInfo.payDescription = @"18";
    
    int res = [[NdComPlatform defaultPlatform] NdUniPay:buyInfo];
    NSLog(@"buy res: %d", res);
    [self addNdPayObservers];
    
}

- (void)didFailedFetchOrder
{
    NSLog(@"failed fetch order info...");
    isBuying = NO;
}

@end
