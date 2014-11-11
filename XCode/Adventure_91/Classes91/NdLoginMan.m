//
//  NdLoginMan.m
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "NdLoginMan.h"
#import "OrderContants.h"

#define ND_SAFE_OBJ(obj)    ((obj)?(obj):(@""))

@interface NdLoginMan () {
    BOOL hasInitSDK;
    NdInitConfigure *cfg;
    BOOL isToolbarHidden;
}

- (void)setNdObservers;
- (void)removeNdObservers;
- (void)SNSInitResult:(NSNotification *)notify;
- (void)NdLoginResult:(NSNotification *)notify;
- (void)succCallback;

@property (nonatomic, retain) NSString *userId;
@property (nonatomic, retain) NSString *sessionId;
@property (nonatomic, retain) NSString *nickName;
@property (nonatomic, assign) int errorCode;

@end

static NdLoginMan *instance = nil;

@implementation NdLoginMan
@synthesize delegate = _delegate;
@synthesize userId;
@synthesize sessionId;
@synthesize nickName;
@synthesize errorCode;

+ (NdLoginMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[NdLoginMan alloc] init];
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
        hasInitSDK = NO;
        isToolbarHidden = YES;
    }
    [self setNdObservers];
    return self;
}

- (void)dealloc
{
    [self removeNdObservers];
    self.userId = nil;
    self.sessionId = nil;
    self.nickName = nil;
    [super dealloc];
}

- (void)setNdObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SNSInitResult:) name:(NSString *)kNdCPInitDidFinishNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(NdLoginResult:) name:(NSString *)kNdCPLoginNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leavePlatformNotification:) name:kNdCPLeavePlatformNotification object:nil];
}

- (void)removeNdObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:(NSString *)kNdCPInitDidFinishNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:(NSString *)kNdCPLoginNotification object:nil];
}

- (void)initWithScreenOrientation:(UIInterfaceOrientation)orientation
{
    cfg = [[[NdInitConfigure alloc] init] autorelease];
    [cfg setOrientation:orientation];
    cfg.appid = APP_ID;
    cfg.appKey = APP_KEY;
    [[NdComPlatform defaultPlatform] NdInit:cfg];
}

- (void)doLogin
{
    if (!hasInitSDK) {
        [self initWithScreenOrientation:UIInterfaceOrientationLandscapeRight];
    }else {
        int result = [[NdComPlatform defaultPlatform] NdLogin:0];
        if (ND_COM_PLATFORM_NO_ERROR != result && [_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:1];
        }
    }
}

- (void)doLogout
{
    int result = [[NdComPlatform defaultPlatform] NdLogout:1];
    if (ND_COM_PLATFORM_NO_ERROR == result && [_delegate respondsToSelector:@selector(didLogout)]) {
        [_delegate didLogout];
    }else {
        if ([_delegate respondsToSelector:@selector(didLogoutFailedWithErrorDescription:)]) {
            [_delegate didLogoutFailedWithErrorDescription:[NSString stringWithFormat:@"Logout error, error code:%d", result]];
        }
    }
}

- (BOOL)isLogined
{
    return [[NdComPlatform defaultPlatform] isLogined];
}

- (void)switchAccount {
    [[NdComPlatform defaultPlatform] NdSwitchAccount];
}

- (void)setNdBarHidden:(BOOL)isHidden {
    if (isHidden) {
        [[NdComPlatform defaultPlatform] NdHideToolBar];
    }else {
        [[NdComPlatform defaultPlatform] NdShowToolBar:NdToolBarAtTopLeft];
    }
    isToolbarHidden = isHidden;
}

- (void)willEnterForeground {
    [[NdComPlatform defaultPlatform] NdPause];
}

- (NSString *)getUserInfoJson {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:ND_SAFE_OBJ(self.userId) forKey:@"userId"];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:nil];
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}

- (void)SNSInitResult:(NSNotification *)notify
{
    hasInitSDK = YES;
    [self doLogin];
    NSLog(@"init completed");
}

- (void)NdLoginResult:(NSNotification *)notify
{
    NSLog(@"login callback:%@", notify);
    
    NSDictionary *dict = [notify userInfo];
    
    BOOL isLoginSucc = [[dict objectForKey:@"result"] boolValue];
    if ([[NdComPlatform defaultPlatform] isLogined] && isLoginSucc) {
        // login success.
        self.userId = [[NdComPlatform defaultPlatform] loginUin];
        self.sessionId = [[NdComPlatform defaultPlatform] sessionId];
        self.nickName = [[NdComPlatform defaultPlatform] nickName];
        [self performSelectorOnMainThread:@selector(succCallback) withObject:nil waitUntilDone:NO];
        
    }else if ([[dict objectForKey:@"error"] intValue] == ND_COM_PLATFORM_ERROR_USER_CANCEL && [[dict objectForKey:@"errorDescription"] isEqualToString:@"您已注销"]) {
        if ([_delegate respondsToSelector:@selector(didLogout)]) {
            [_delegate didLogout];
        }
    } else{
        self.errorCode = [[dict objectForKey:@"error"] intValue];
        if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:0];
        }
    }
}

- (BOOL)isToolBarHidden {
    return isToolbarHidden;
}

- (void)leavePlatformNotification:(NSNotification*)notification {
    if ([_delegate respondsToSelector:@selector(didLeavePlatform)]) {
        [_delegate didLeavePlatform];
    }
}

- (void)succCallback
{
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        [_delegate didLoginWithUserId:self.userId userName:self.nickName andToken:self.sessionId];
    }
}

@end
