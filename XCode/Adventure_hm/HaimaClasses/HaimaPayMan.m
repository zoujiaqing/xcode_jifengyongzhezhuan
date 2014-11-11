//
//  HaimaPayMan.m
//  HaimaDemo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "HaimaPayMan.h"
#import "OrderMan.h"
#import "IPAYiAppPay.h"
#import "IPAYiAppPayOrder.h"

static HaimaPayMan *instance = nil;

@interface HaimaPayMan() <OrderManDelegate, IPAYiAppPayPaymentDelegate> {
    BOOL isBuying;
}

@end

@implementation HaimaPayMan

+ (HaimaPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[HaimaPayMan alloc] init];
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
        isBuying = NO;
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}


- (void)doPayWithProductId:(NSString *)productId userId:(NSString *)userId channelId:(NSString *)channelId serverId:(NSString *)serverId currency:(NSString *)currency andUserCreateTime:(NSString *)createTime
{
//    if (isBuying) {
//        return;
//    }else {
//        isBuying = YES;
//    }
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

#pragma mark - order delegate methods

- (void)didFectchOrderWithProductId:(NSString *)productId amount:(NSString *)amount tradeNo:(NSString *)tradeNo productName:(NSString *)productName andAddInfo:(NSString *)addInfo
{
    
}

- (void)didFectchOrderWithProductId:(NSString *)productId
                             amount:(NSString *)amount
                            tradeNo:(NSString *)tradeNo
                        productName:(NSString *)productName
                        callbackUrl:(NSString *)url
                            thirdId:(NSString *)thirdId
                         andAddInfo:(NSString *)addInfo {
    int cost = (int)([amount floatValue]);
    
    IPAYiAppPayOrder *order = [[IPAYiAppPayOrder alloc] init];
    order.exorderno = [NSString stringWithString:tradeNo];
    order.price = cost;
    order.waresid = [thirdId intValue];
    if (url && [url length] > 0) {
        NSString *newUrl =  (NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault, (CFStringRef)url, CFSTR(""),kCFStringEncodingUTF8);
        order.notifyurl = [NSString stringWithFormat:@"%@", newUrl];
        [newUrl autorelease];
    }
    
    NSString *signature = [[IPAYiAppPay sharediAppPay] getOrderSignature:order withAppID:nil andAppKey:nil];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[IPAYiAppPay sharediAppPay] checkoutWithOrder:order andOrderSignature:signature andPaymentDelegate:self];
    });
    
    
}

- (void)didFailedFetchOrder
{
    
}

#pragma mark - SDK支付回调
- (void)paymentStatusCode: (IPAYiAppPayPaymentStatusCodeType)statusCode
                signature: (NSString *)signature
               resultInfo: (NSString *)resultInfo {
    
}

@end
