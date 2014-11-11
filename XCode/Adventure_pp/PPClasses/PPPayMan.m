//
//  PPPayMan.m
//  PPDemo
//
//  Created by Delle  on 14-3-10.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "PPPayMan.h"
#import "OrderMan.h"

@interface PPPayMan () {
    BOOL isBuying;
}

- (void)succCallback;
- (void)failedCallback;

@property (nonatomic, retain) NSString *errorDes;

@end

static PPPayMan *instance = nil;

@implementation PPPayMan
@synthesize delegate = _delegate;
@synthesize isInnerMode = _isInnerMode;

+ (PPPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[PPPayMan alloc] init];
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
    self.errorDes = nil;
    [super dealloc];
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

#pragma mark - order delegate methods

- (void)didFectchOrderWithProductId:(NSString *)productId amount:(NSString *)amount tradeNo:(NSString *)tradeNo productName:(NSString *)productName andAddInfo:(NSString *)addInfo
{
    [PPAppPlatformKit sharedInstance].delegate = self;
//    [[PPAppPlatformKit sharedInstance] exchangeGoods:(int)([amount floatValue]*100)  BillNo:tradeNo BillTitle:productName RoleId:productId ZoneId:0];
    int cost = (int)([amount floatValue]);
    [[PPAppPlatformKit sharedInstance] exchangeGoods:cost  BillNo:tradeNo BillTitle:productName RoleId:@"0" ZoneId:0];
}

- (void)didFailedFetchOrder
{
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
}

#pragma mark - pp delegate methods

/**
 * @brief   余额大于所购买道具
 * @param   INPUT   paramPPPayResultCode       接口返回的结果编码
 * @return  无返回
 */
- (void)ppPayResultCallBack:(PPPayResultCode)paramPPPayResultCode
{
    NSLog(@"resultcode: %d", paramPPPayResultCode);
    isBuying = NO;
}
/**
 * @brief   验证更新成功后
 * @noti    分别在非强制更新点击取消更新和暂无更新时触发回调用于通知弹出登录界面
 * @return  无返回
 */
- (void)ppVerifyingUpdatePassCallBack
{
    NSLog(@"verify update succ.");
}
/**
 * @brief   登录成功回调【任其一种验证即可】
 * @param   INPUT   paramStrToKenKey       字符串token
 * @return  无返回
 */
- (void)ppLoginStrCallBack:(NSString *)paramStrToKenKey
{
    
}
/**
 * @brief   登录成功回调【任其一种验证即可】
 * @param   INPUT   paramHexToKen          2进制token
 * @return  无返回
 */
- (void)ppLoginHexCallBack:(char *)paramHexToKen
{
    
}

/**
 * @brief   关闭Web页面后的回调
 * @param   INPUT   paramPPWebViewCode    接口返回的页面编码
 * @return  无返回
 */
- (void)ppCloseWebViewCallBack:(PPWebViewCode)paramPPWebViewCode
{
    isBuying = NO;
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"Payment canceled."];
    }
}
/**
 * @brief   关闭SDK客户端页面后的回调
 * @param   INPUT   paramPPPageCode       接口返回的页面编码
 * @return  无返回
 */
- (void)ppClosePageViewCallBack:(PPPageCode)paramPPPageCode
{
    isBuying = NO;
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]) {
        [_delegate didPaidFailedWithErrorDescription:@"Payment canceled."];
    }
}
/**
 * @brief   注销后的回调
 * @return  无返回
 */
- (void)ppLogOffCallBack
{
    
}


@end
