//
//  XYLoginMain.m
//  XYDemo
//
//  Created by caesarhu on 14-8-5.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "XYLoginMain.h"
#import <XYPlatform/XYPlatform.h>
#import "OrderContants.h"


#define XY_SAFE_OBJ(obj)    ((obj)?(obj):(@""))

static XYLoginMain *instance = nil;

@interface XYLoginMain() {
    BOOL isLogined;
    BOOL isToolbarHidden;
}

@end

@implementation XYLoginMain

+ (XYLoginMain *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[XYLoginMain alloc] init];
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
        isLogined = NO;
        isToolbarHidden = YES;
    }
    [self addTbObservers];
    return self;
}

- (void)dealloc
{
    [self removeTbObservers];
    [super dealloc];
}

- (void)addTbObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginResult:) name:kXYPlatformLoginNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(logoutResult:) name:kXYPlatformLogoutNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didInitFinished:) name:kXYPlatformInitDidFinishedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leavePlatform:) name:kXYPlatformLeavedNotification object:nil];
}

- (void)removeTbObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kXYPlatformLoginNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kXYPlatformLogoutNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kXYPlatformInitDidFinishedNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kXYPlatformLeavedNotification object:nil];
}



#pragma mark - Notifications
- (void)didInitFinished:(NSNotification*)notification {
    if ([_delegate respondsToSelector:@selector(didFinishInitSDK)]) {
        //[_delegate didFinishInitSDK];
    }
}

- (void)loginResult:(NSNotification *)notification {
    NSDictionary *userInfo = notification.userInfo;
    
    int errCode = [userInfo[kXYPlatformErrorKey] intValue];
    if (errCode == XY_PLATFORM_ERROR_ACCOUNT_EMPTY || errCode == XY_PLATFORM_ERROR_UID_INVALID || errCode == XY_PLATFORM_ERROR_PASSWORD_INVALID) {
        return;
    }
    
    if (errCode == XY_PLATFORM_NO_ERROR) {
        isLogined = YES;
        NSString *token = [[XYPlatform defaultPlatform] XYToken];
        NSString *openUid = [[XYPlatform defaultPlatform] XYOpenUID];
        NSString *userName = [[XYPlatform defaultPlatform] XYLoginUserAccount];
        if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
            [_delegate didLoginWithUserId:openUid userName:userName andToken:token];
            [[XYPlatform defaultPlatform] XYShowToolBar:XYToolBarAtMiddleLeft isUseOldPlace:YES];
            isToolbarHidden = NO;
        }
    }else {
        if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:1];
        }
    }
}

- (void)logoutResult:(NSNotification *)notification {
    isLogined = NO;
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        [_delegate didLogout];
        [[XYPlatform defaultPlatform] XYHideToolBar];
    }
}

- (void)leavePlatform:(NSNotification *)notification {
    [self leave];
    //[NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(leave) userInfo:nil repeats:NO];
}

- (void)leave {
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:0];
    }
}

#pragma mark - 接口
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation {
    //[[XYPlatform defaultPlatform] initializeWithAppId:APP_ID appKey:APP_KEY isContinueWhenCheckUpdateFailed:NO];
    [[XYPlatform defaultPlatform] XYSetDebugModel:NO];
    [[XYPlatform defaultPlatform] XYSetScreenOrientation:orientation];
}

- (void)doLogin {
    //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leavePlatform:) name:kXYPlatformLeavedNotification object:nil];
    [[XYPlatform defaultPlatform] XYAutoLogin:0];
}

- (void)doLogout {
    [[XYPlatform defaultPlatform] XYLogout:0];
}

- (BOOL)isLogined {
    return isLogined;
}

- (void)setToolBarHidden:(BOOL)isHidden {
    if (!isHidden) {
        [[XYPlatform defaultPlatform] XYShowToolBar:XYToolBarAtMiddleLeft isUseOldPlace:YES];
        isToolbarHidden = NO;
    }else {
        [[XYPlatform defaultPlatform] XYHideToolBar];
        isToolbarHidden = YES;
    }
}

- (BOOL)isToolBarHidden {
    return isToolbarHidden;
}

- (void)switchAccount {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leavePlatform:) name:kXYPlatformLeavedNotification object:nil];
    [[XYPlatform defaultPlatform] XYSwitchAccount];
}

- (void)showUserCenter {
    [[XYPlatform defaultPlatform] XYEnterUserCenter:0];
}

- (void)applicationHandleOpenURL:(NSURL *)url {
    [[XYPlatform defaultPlatform] XYHandleOpenURL:url];
}


- (NSString*)getUserInfoJson {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:XY_SAFE_OBJ([[XYPlatform defaultPlatform] XYOpenUID]) forKey:@"userId"];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:nil];
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}


@end
