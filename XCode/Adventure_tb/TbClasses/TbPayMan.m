//
//  TbPayMan.m
//  TbDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "TbPayMan.h"
#import "OrderMan.h"

@interface TbPayMan () {
    BOOL isBuying;
}

- (void)succCallback;
- (void)failedCallback;

@property (nonatomic, retain) NSString *strTradeNo;

@end

static TbPayMan *instance = nil;

@implementation TbPayMan
@synthesize delegate = _delegate;
@synthesize isInnerMode = _isInnerMode;
@synthesize strTradeNo;

+ (TbPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[TbPayMan alloc] init];
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
    }
    
    return self;
}

- (void)dealloc
{
    self.strTradeNo = nil;
    [super dealloc];
}

- (void)doPayWithProductId:(NSString *)productId userId:(NSString *)userId channelId:(NSString *)channelId serverId:(NSString *)serverId currency:(NSString *)currency andUserCreateTime:(NSString *)createTime
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
    self.strTradeNo = tradeNo;
    [[TBPlatform defaultPlatform] TBUniPayForCoin:tradeNo needPayRMB:[amount intValue] payDescription:productName delegate:self];
    isBuying = NO;
}

- (void)didFailedFetchOrder
{
    NSLog(@"Network error.");
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
    isBuying = NO;
}

#pragma mark tbpay delegate methods
- (void)TBBuyGoodsDidSuccessWithOrder:(NSString *)order
{
    NSLog(@"order succes: %@", order);
    isBuying = NO;
}

- (void)TBBuyGoodsDidCancelByUser:(NSString *)order
{
    NSLog(@"cancel order: %@", order);
    isBuying = NO;
}

- (void)TBBuyGoodsDidStartRechargeWithOrder:(NSString *)order
{
    NSLog(@"recharge order: %@", order);
    self.strTradeNo = order;
    //[[TBPlatform defaultPlatform] TBCheckPaySuccess:order delegate:self];
    isBuying = NO;
}

- (void)TBBuyGoodsDidFailedWithOrder:(NSString *)order resultCode:(TB_BUYGOODS_ERROR)errorType
{
    NSLog(@"error...");
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
    isBuying = NO;

}

#pragma mark - tbpaycheck delegate

- (void)TBCheckOrderSuccessWithResult:(NSDictionary *)dict
{
    NSLog(@"check order: %@", dict);
    isBuying = NO;
}

- (void)TBCheckOrderDidFailed:(NSString *)order
{
    NSLog(@"check failed....");
    isBuying = NO;
}

@end
