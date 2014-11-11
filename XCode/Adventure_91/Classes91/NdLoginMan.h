//
//  NdLoginMan.h
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>
#import <NdComPlatform/NdComPlatform.h>

@protocol LoginManDelegate <NSObject>
@required
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

- (void)didLoginFailedWithErrorCode:(int)errorCode;

- (void)didLeavePlatform;

- (void)didLogout;
- (void)didLogoutFailedWithErrorDescription:(NSString *)des;
@end


@interface NdLoginMan : NSObject{
    id<LoginManDelegate> _delegate;
}

@property (nonatomic, assign) id<LoginManDelegate>delegate;

+ (NdLoginMan *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  初始化SDK，在application:didFinishLaunchingWithOptions:中调用
 *
 *  @param orientation 屏幕旋转角度
 */
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation;
- (void)doLogin;
- (void)doLogout;
- (BOOL)isLogined;

- (void)switchAccount;

- (void)setNdBarHidden:(BOOL)isHidden;

- (NSString*)getUserInfoJson;

- (BOOL)isToolBarHidden;

/**
 *  在- (void)applicationWillEnterForeground:(UIApplication *)application中调用
 */
- (void)willEnterForeground;
@end
