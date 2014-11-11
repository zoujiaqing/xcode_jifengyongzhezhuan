//
//  I4PayMan.m
//  I4Demo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "I4PayMan.h"
#import "OrderMan.h"
#import "OrderManDelegate.h"
#import "AsPlatformSDK.h"

static I4PayMan *instance = nil;

@interface I4PayMan() <OrderManDelegate, AsPlatformSDKDelegate> {
    BOOL isBuying;
}

@end

@implementation I4PayMan
@synthesize delegate = _delegate,loginDelegate = _loginDelegate;

- (id)init {
    if (self = [super init]) {
        isBuying = NO;
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

+ (I4PayMan *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[I4PayMan alloc] init];
        }
        return instance;
    }
}

+ (void)purgeSharedInstance {
    @synchronized(self) {
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (void)doPayWithProductId:(NSString *)productId userId:(NSString *)userId channelId:(NSString *)channelId serverId:(NSString *)serverId currency:(NSString *)currency andUserCreateTime:(NSString *)createTime {
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
    isBuying = NO;
}

#pragma mark - order delegate methods

- (void)didFectchOrderWithProductId:(NSString *)productId amount:(NSString *)amount tradeNo:(NSString *)tradeNo productName:(NSString *)productName andAddInfo:(NSString *)addInfo
{
    [AsPlatformSDK sharedInstance].delegate = self;
    [[AsPlatformSDK sharedInstance] exchangeGoods:[amount integerValue] BillNo:tradeNo BillTitle:productName RoleId:@"0" ZoneId:0];
    isBuying = NO;
}

- (void)didFailedFetchOrder
{
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"Server error"];
    }
    isBuying = NO;
}

//-SDK 1.5.2 - 新增的银联支付的回调
- (void)asUPPayPluginResultCallBack:(NSString *)result
{
    //    * success  支付成功
    //    * fail     支付失败
    //    * cancel   用户取消支付
    if ([result isEqualToString:@"success"]) {
        NSLog(@"银联支付的回调 - 支付成功 : %@", result);
    }
    if ([result isEqualToString:@"fail"]) {
        NSLog(@"银联支付的回调 - 支付失败 : %@", result);
    }
    if ([result isEqualToString:@"cancel"]){
        NSLog(@"银联支付的回调 - 用户取消支付 : %@", result);
    }
    
}

/**
 * @brief   余额大于所购买道具
 * @param   INPUT   paramAsPayResultCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asPayResultCallBack:(AsPayResultCode)paramPPPayResultCode {

}


/**
 * @brief   验证更新成功后
 * @noti    分别在非强制更新点击取消更新和暂无更新时触发回调用于通知弹出登录界面
 * @return  无返回
 */
- (void)asVerifyingUpdatePassCallBack {

}


/**
 * @brief   登录成功回调
 * @param   INPUT   paramToken       字符串token
 * @return  无返回
 */
- (void)asLoginCallBack:(NSString *)paramToken {

}


/**
 * @brief   关闭Web页面后的回调
 * @param   INPUT   paramWebViewCode    接口返回的页面编码
 * @return  无返回
 */
- (void)asCloseWebViewCallBack:(AsWebViewCode)paramWebViewCode {
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"Payment Canceled."];
    }
    isBuying = NO;
}


/**
 * @brief   关闭SDK客户端页面后的回调
 * @param   INPUT   paramAsPageCode       接口返回的页面编码
 * @return  无返回
 */
- (void)asClosePageViewCallBack:(AsPageCode)paramPPPageCode {

}

/**
 * @brief   注销后的回调
 * @return  无返回
 */
- (void)asLogOffCallBack {
    if ([_loginDelegate respondsToSelector:@selector(didLogout)]) {
        [_loginDelegate didLogout];
    }
}

/**
 * @brief   关闭用户中心的回调
 * @param   INPUT   paramCenterViewCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asClosedCenterViewCallBack {
    
}


@end
