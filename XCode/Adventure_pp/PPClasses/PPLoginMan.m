//
//  PPLoginMan.m
//  PPDemo
//
//  Created by Delle  on 14-3-10.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "PPLoginMan.h"
#import "OrderContants.h"


@interface PPLoginMan () {
	BOOL isLoginLayer;
    BOOL hasInited;
}

- (void)failedCallback;

@property (nonatomic, retain) NSString *errorDes;
@end

static PPLoginMan *instance = nil;

@implementation PPLoginMan


+ (PPLoginMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[PPLoginMan alloc] init];
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
        isLoginLayer = NO;
        hasInited = NO;
    }
    
    return self;
}

- (void)dealloc
{
    self.errorDes = nil;
    [super dealloc];
}

- (void)initWithScreenOrientation:(UIInterfaceOrientation)orientation
{
    //    NSLog(@"appid:%@, appkey:%@", APP_ID, APP_KEY);
    [[PPAppPlatformKit sharedInstance] setAppId:APP_ID AppKey:APP_KEY];
    [[PPAppPlatformKit sharedInstance] setIsNSlogData:NO];
    [[PPAppPlatformKit sharedInstance] setRechargeAmount:1];
    [[PPAppPlatformKit sharedInstance] setIsLongComet:YES];
    [[PPAppPlatformKit sharedInstance] setIsLogOutPushLoginView:YES];
    [[PPAppPlatformKit sharedInstance] setIsOpenRecharge:YES];
    [[PPAppPlatformKit sharedInstance] setCloseRechargeAlertMessage:@"关闭充值提示语"];
    [PPUIKit sharedInstance];
    
    [[PPUIKit sharedInstance] checkGameUpdate];
    [PPUIKit setIsDeviceOrientationLandscapeLeft:UIInterfaceOrientationLandscapeLeft & orientation];
    [PPUIKit setIsDeviceOrientationLandscapeRight:UIInterfaceOrientationLandscapeRight & orientation];
    [PPUIKit setIsDeviceOrientationPortrait:UIInterfaceOrientationPortrait & orientation];
    [PPUIKit setIsDeviceOrientationPortraitUpsideDown:UIInterfaceOrientationPortraitUpsideDown & orientation];
    [PPAppPlatformKit sharedInstance].delegate = self;
}

- (void)doLogin
{
    [[PPAppPlatformKit sharedInstance] setDelegate:self];
    [[PPAppPlatformKit sharedInstance] showLogin];
    //    [[PPUIKit sharedInstance] checkGameUpdate];
}

- (void)doLogout
{
    [PPAppPlatformKit sharedInstance].delegate = self;
    [[PPAppPlatformKit sharedInstance] PPlogout];
}

- (void)showUserCenter {
    [PPAppPlatformKit sharedInstance].delegate = self;
    [[PPAppPlatformKit sharedInstance] showCenter];
	isLoginLayer = NO;
}

- (BOOL)isLogined {
    if ([[PPAppPlatformKit sharedInstance] loginState] == 1) {
        return YES;
    }else {
        return NO;
    }
}

- (void)failedCallback
{
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:1];
    }
}

#pragma mark - pp delegate methods

/**
 * @brief   余额大于所购买道具
 * @param   INPUT   paramPPPayResultCode       接口返回的结果编码
 * @return  无返回
 */
- (void)ppPayResultCallBack:(PPPayResultCode)paramPPPayResultCode
{
    
}
/**
 * @brief   验证更新成功后
 * @noti    分别在非强制更新点击取消更新和暂无更新时触发回调用于通知弹出登录界面
 * @return  无返回
 */
- (void)ppVerifyingUpdatePassCallBack
{
    //[[PPAppPlatformKit sharedInstance] showLogin];
	isLoginLayer = YES;
    
    hasInited = YES;
    if ([_delegate respondsToSelector:@selector(didFinishInitSDK)]) {
        [_delegate didFinishInitSDK];
    }
}
/**
 * @brief   登录成功回调【任其一种验证即可】
 * @param   INPUT   paramStrToKenKey       字符串token
 * @return  无返回
 */
- (void)ppLoginStrCallBack:(NSString *)paramStrToKenKey
{
    NSLog(@"token: %@", paramStrToKenKey);
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        [_delegate didLoginWithUserId:nil userName:nil andToken:paramStrToKenKey];
    }
}


- (void)ppLoginCallback:(NSString *)paramPPTokenKey
{
    NSLog(@"token taoke:%@", paramPPTokenKey);
}

/**
 * @brief   关闭Web页面后的回调
 * @param   INPUT   paramPPWebViewCode    接口返回的页面编码
 * @return  无返回
 */
- (void)ppCloseWebViewCallBack:(PPWebViewCode)paramPPWebViewCode
{
    
}
/**
 * @brief   关闭SDK客户端页面后的回调
 * @param   INPUT   paramPPPageCode       接口返回的页面编码
 * @return  无返回
 */
- (void)ppClosePageViewCallBack:(PPPageCode)paramPPPageCode
{
	if (isLoginLayer == NO && [[PPAppPlatformKit sharedInstance] loginState] == 1)
		return;
    
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:0];
    }
}
/**
 * @brief   注销后的回调
 * @return  无返回
 */
- (void)ppLogOffCallBack
{
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        [_delegate didLogout];
    }
}

- (BOOL)getUserIdAndNameWithToken:(NSString *)token {
    NSURL *url = [NSURL URLWithString:@"http://passport_i.25pp.com/index?tunnel-command=2852126756"];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:url];
    request.timeoutInterval = 25;
    [request setHTTPMethod:@"POST"];
    NSData *data = [token dataUsingEncoding:NSUTF8StringEncoding];
    [request setHTTPBody:data];
    NSData *dataRecived = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    if (dataRecived) {
        NSString *str = [[NSString alloc] initWithData:dataRecived encoding:NSUTF8StringEncoding];
        NSLog(@"=========    %@", str);
        return YES;
    }else {
        return NO;
    }
}

@end
