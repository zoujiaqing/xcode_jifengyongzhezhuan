//
//  KyPayMan.m
//  KyDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "KyPayMan.h"
#import "OrderMan.h"
#import "OrderContants.h"
#import "KyLoginMan.h"

@interface KyPayMan() {
    BOOL isPaying;  //是否正在支付，防止连续点击crash的问题
}

- (void)succCallback;
- (void)failedCallback;

@property (nonatomic, retain) NSString *errorDes;
@property (nonatomic, retain) NSString *kyUserId;
@end

static KyPayMan *instance = nil;

@implementation KyPayMan
@synthesize delegate = _delegate;
@synthesize isInnerMode = _isInnerMode;
@synthesize errorDes;
@synthesize kyUserId;

+ (KyPayMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[KyPayMan alloc] init];
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
        _isInnerMode = NO;
        isPaying = NO;
    }
    
    return self;
}

- (void)dealloc
{
    self.errorDes = nil;
    self.kyUserId = nil;
    [super dealloc];
}

- (void)doPayWithProductId:(NSString *)productId userId:(NSString *)userId channelId:(NSString *)channelId serverId:(NSString *)serverId currency:(NSString *)currency andUserCreateTime:(NSString *)createTime
{
    if (isPaying) {
        return;
    }else {
        isPaying = YES;
    }
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
    self.kyUserId = userId;
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
    isPaying = NO;
    if ([_delegate respondsToSelector:@selector(didPaid)]){
        [_delegate didPaid];
    }
}

- (void)failedCallback
{
    isPaying = NO;
    if ([_delegate respondsToSelector:@selector(didPaidFailedWithErrorDescription:)]){
        [_delegate didPaidFailedWithErrorDescription:self.errorDes];
    }
}

#pragma mark - order delegate

- (void)didFectchOrderWithProductId:(NSString *)productId amount:(NSString *)amount tradeNo:(NSString *)tradeNo productName:(NSString *)productName andAddInfo:(NSString *)addInfo
{
    [[KYSDK instance] setSdkdelegate:self];
    NSString *userId = [NSString stringWithFormat:@"%@", [[KyLoginMan sharedInstance] guid]];
    [[KYSDK instance] showPayWith:tradeNo fee:amount game:KY_GAME_ID gamesvr:@"" subject:productName md5Key:KY_SIGN userId:userId appScheme:@"com.taomee.adventure.ky"];
}

- (void)didFailedFetchOrder
{
    self.errorDes = @"Order not found.";
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
    isPaying = NO;
}


#pragma mark - kuaiyong pay delegate
/**
 *  @method -(void)UPPayPluginResult:(UNIPAYTYPE *)result
 *  银联支付回调函数
 *  @param  result 支付结果
 */
-(void)UPPayPluginResult:(UNIPAYTYPE)result
{
    isPaying = NO;
}

/**
 *  @method - (void)submitMobilePay
 *  关闭支付界面"关闭"按钮的回调
 */
-(void)closePayCallback {
    self.errorDes = @"payment view closed.";
    isPaying = NO;
    [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
}

- (void)backCheckDel:(NSMutableDictionary *)map
{
    NSLog(@"back check: %@", map);
}
/**
 *-(void)alipayCallBack
 *支付宝完成后回调
 **/
-(void)alipayCallBack:(ALIPAYRESULT)alipayresult
{
    isPaying = NO;
}

//
//- (void)checkResult:(CHECK)result
//{
//    NSLog(@"check: %d", result);
//    
//    switch (result) {
//        case PAY_SUCCESS:
//            [self performSelectorOnMainThread:@selector(succCallback) withObject:nil waitUntilDone:NO];
//            break;
//        default:
//            [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
//            break;
//    }
//}


#pragma mark - kuai yong login delegate methods
/**
 登录成功回调
 **/
-(void)userLoginWithserviceTokenKey:(NSString *)tokenKey{
    
}

/**
 快速登录成功回调
 **/
-(void)quickGameWithserviceTokenKey:(NSString *)tokenKey{
    
}

/**
 游戏账号登录回调
 **/
-(void)gameLogBack:(NSString *)username passWord:(NSString *)password{
    
}

/**
 注销成功回调
 **/
-(void)userLoginOut:(NSString *)guid{
    
}

-(void)returnKeyDidClick{
    
}

/**
 取消更新回调方法
 **/
-(void)singleLogin{
    
}

@end
