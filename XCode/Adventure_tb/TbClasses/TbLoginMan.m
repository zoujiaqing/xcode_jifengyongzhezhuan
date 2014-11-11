//
//  TbLoginMan.m
//  TbDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "TbLoginMan.h"
#import "OrderContants.h"

#define TB_SAFE_OBJ(obj)    ((obj)?(obj):(@""))

@interface TbLoginMan () {
    BOOL hasInited;
    BOOL isToolbarHidden;
}
- (void)addTbObservers;
- (void)removeTbObservers;
- (void)succCallback;
- (void)logoutCallback;
- (void)loginResult:(NSNotification *)notify;
- (void)logoutResult:(NSNotification *)notify;

@property (nonatomic, retain) NSString *userId;
@property (nonatomic, retain) NSString *userName;
@property (nonatomic, retain) NSString *token;
@property (nonatomic, retain) NSString *errorDes;
@end

static TbLoginMan *instance = nil;

@implementation TbLoginMan
@synthesize delegate = _delegate;
@synthesize userId;
@synthesize userName;
@synthesize token;
@synthesize errorDes;


+ (TbLoginMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[TbLoginMan alloc] init];
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
        hasInited = NO;
        isToolbarHidden = NO;
    }
    [self addTbObservers];
    return self;
}

- (void)dealloc
{
    self.userId = nil;
    self.userName = nil;
    self.token = nil;
    self.errorDes = nil;
    [self removeTbObservers];
    [super dealloc];
}

- (void)initWithScreenOrientation:(UIInterfaceOrientation)orientation
{
    [[TBPlatform defaultPlatform] TBInitPlatformWithAppID:APP_ID screenOrientation:orientation isContinueWhenCheckUpdateFailed:NO];
    [[TBPlatform defaultPlatform] TBSetAutoRotation:YES];
    [[TBPlatform defaultPlatform] TBSetShowSDKLog:NO];
//    [[TBPlatform defaultPlatform] TBSetUpdateDebugMode:0];
    [self setToolbarHidden:NO];
}

- (void)addTbObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginResult:) name:kTBLoginNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(logoutResult:) name:kTBUserLogoutNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didInitFinished:) name:kTBInitDidFinishNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leavePlatform:) name:kTBLeavePlatformNotification object:nil];
}

- (void)removeTbObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kTBLoginNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kTBUserLogoutNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kTBInitDidFinishNotification object:nil];
}

- (void)leavePlatform:(NSNotification *)notify {
    TBPlatformLeavedType leavedFormType = [[notify.userInfo objectForKey:TBLeavedPlatformTypeKey] intValue];
    if (TBPlatformLeavedFromLogin == leavedFormType) {
        if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:0];
        }
    }else if (TBPlatformLeavedFromUserCenter == leavedFormType) {
        if ([_delegate respondsToSelector:@selector(didLeavePlatform)]) {
            [_delegate didLeavePlatform];
        }
    }
}

- (void)didInitFinished:(NSNotification*)notify {
//    [self doLogin];
    hasInited = YES;
    if ([_delegate respondsToSelector:@selector(didFinishInitSDK)]) {
        [_delegate didFinishInitSDK];
    }
}

- (void)loginResult:(NSNotification *)notify
{
    if ([[TBPlatform defaultPlatform] TBIsLogined]) {
        TBPlatformUserInfo *userInfo = [[TBPlatform defaultPlatform] TBGetMyInfo];
        self.userId = [userInfo userID];
        self.userName = [userInfo nickName];
        self.token = [userInfo sessionID];
        [self performSelectorOnMainThread:@selector(succCallback) withObject:nil waitUntilDone:NO];
    }else{
        if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:0];
        }
    }
}

- (void)logoutResult:(NSNotification *)notify
{
    NSLog(@"logout: %@", notify);
    [self performSelectorOnMainThread:@selector(logoutCallback) withObject:nil waitUntilDone:NO];
}

- (void)doLogin
{
    if (!hasInited) {
        [self initWithScreenOrientation:UIInterfaceOrientationLandscapeLeft]; //这里只能设置一个方向
    }
    [[TBPlatform defaultPlatform] TBLogin:0];
}

- (void)doLogout
{
    [[TBPlatform defaultPlatform] TBLogout:0];
}

- (BOOL)isLogined {
    return [[TBPlatform defaultPlatform] TBIsLogined];
}

- (void)switchAccount {
    [[TBPlatform defaultPlatform] TBSwitchAccount];
}

- (void)setToolbarHidden:(BOOL)isHidden {
    if (isHidden) {
        [[TBPlatform defaultPlatform] TBHideToolBar];
    }else {
        [[TBPlatform defaultPlatform] TBShowToolBar:TBToolBarAtTopLeft isUseOldPlace:YES];
    }
    isToolbarHidden = isHidden;
}

- (BOOL)isToolBarHidden {
    return isToolbarHidden;
}

- (NSString *)getUserInfoJson {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:TB_SAFE_OBJ(self.userId) forKey:@"userId"];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:nil];
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}


#pragma mark - delegate callback in main queue
- (void)succCallback
{
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        [_delegate didLoginWithUserId:self.userId userName:self.userName andToken:self.token];
    }
}

- (void)logoutCallback
{
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        [_delegate didLogout];
    }
}

@end
