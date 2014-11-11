//
//  HXLoginMan.m
//  iToolsDemo
//
//  Created by caesarhu on 14-4-23.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "HXLoginMan.h"
#import "HXAppPlatformKitPro.h"
#import "OrderContants.h"

#define HX_SAFE_OBJ(obj)      ((obj)?(obj):(@""))

static HXLoginMan *instance = nil;

@interface HXLoginMan() {
    BOOL isLogined;
    NSString *theUserId;
    BOOL hasInited;
    BOOL isToolbarHidden;
}
@end

@implementation HXLoginMan

@synthesize delegate = _delegate;

- (id)init {
    if (self = [super init]) {
        //监听注册通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(registerNotification:) name:HX_NOTIFICATION_REGISTER object:nil];
        //监听登录通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginNotification:) name:HX_NOTIFICATION_LOGIN object:nil];
        //视图关闭通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeViewNotification:) name:HX_NOTIFICATION_CLOSEVIEW object:nil];
        //注销通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(logoutNotification:) name:HX_NOTIFICATION_LOGOUT object:nil];
        isLogined = NO;
        hasInited = NO;
        theUserId = nil;
        isToolbarHidden = YES;
    }
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [theUserId release];
    [super dealloc];
}

+ (HXLoginMan *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[HXLoginMan alloc] init];
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

- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation {
    [HXAppPlatformKitPro setAppId:APP_ID appKey:APP_KEY];
    [HXAppPlatformKitPro setSupportOrientationPortrait:YES
                                    portraitUpsideDown:NO
                                         landscapeLeft:YES
                                        landscapeRight:YES];
    [HXAppPlatformKitPro setSDKAssistiveTouchAtScreenPlace:TSAssistiveTouchViewAtMiddleLeft];
    hasInited = YES;
}

- (void)doLogin {
    if (!hasInited) {
        [self initWithScreenOrientation:UIInterfaceOrientationLandscapeRight | UIInterfaceOrientationLandscapeLeft];
        hasInited = YES;
    }
    [HXAppPlatformKitPro setLoginViewClosedEnabled:NO];
    [HXAppPlatformKitPro showLoginView];
    [HXAppPlatformKitPro setSDKAssistiveTouchAtScreenPlace:TSAssistiveTouchViewAtMiddleLeft];
}

- (void)doLogout {
    [HXAppPlatformKitPro logout];
}

- (BOOL)isLogined {
    return isLogined;
}

- (void)showPlatformView {
    [HXAppPlatformKitPro showPlatformView];
}

- (NSString *)getUserInfoJson {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:HX_SAFE_OBJ(theUserId) forKey:@"userId"];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:nil];
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}

#pragma mark - Notifications
//注册通知处理
- (void)registerNotification:(NSNotification *)notification
{
    theUserId = [[NSString alloc] initWithFormat:@"%@", [notification.object objectForKey:@"userId"]];
    NSString *userId = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"userId"]];
    NSString *sessionId = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"sessionId"]];
    NSString *userName = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"userName"]];
    
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        isLogined = YES;
        [_delegate didLoginWithUserId:userId userName:userName andToken:sessionId];
    }
}

//登录通知处理
- (void)loginNotification:(NSNotification *)notification
{
    theUserId = theUserId = [[NSString alloc] initWithFormat:@"%@", [notification.object objectForKey:@"userId"]];
    NSString *userId = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"userId"]];
    NSString *sessionId = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"sessionId"]];
    NSString *userName = [NSString stringWithFormat:@"%@", [notification.object objectForKey:@"userName"]];
    
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        isLogined = YES;
        [_delegate didLoginWithUserId:userId userName:userName andToken:sessionId];
    }
}

//关闭窗口通知处理
- (void)closeViewNotification:(NSNotification *)notification
{
    NSLog(@"SDK View Closed.");
}

//注销通知处理
- (void)logoutNotification:(NSNotification *)notification
{
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        isLogined = NO;
        [_delegate didLogout];
    }
}

/**
 *  工具条是否隐藏
 *
 *  @return 是否隐藏
 */
- (BOOL)isToolBarHidden
{
    return isToolbarHidden;
}

/**
 *  设置工具条的显示和隐藏
 *
 *  @param isHidden 是否隐藏
 */
- (void)setToolBarHidden:(BOOL)isHidden
{
    if(!isHidden)
    {
        [HXAppPlatformKitPro setSupportSDKAssistiveTouchEnbled:YES];
        isToolbarHidden = NO;
    }
    else
    {
        [HXAppPlatformKitPro setSupportSDKAssistiveTouchEnbled:NO];
        isToolbarHidden = YES;
    }
}

@end
