//
//  PPLoginMan.h
//  PPDemo
//
//  Created by Delle  on 14-3-10.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>
#import <PPAppPlatformKit/PPAppPlatformKit.h>

@protocol LoginManDelegate <NSObject>
@required
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

/**
 *  登陆失败回调
 *
 *  @param errorCode 0是关闭了界面，其他是失败
 */
- (void)didLoginFailedWithErrorCode:(int)errorCode;

- (void)didLogout;

- (void)didFinishInitSDK;
@end

@interface PPLoginMan : NSObject<PPAppPlatformKitDelegate>{
    id<LoginManDelegate> _delegate;
}

@property (nonatomic, assign) id<LoginManDelegate> delegate;

+ (PPLoginMan *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  初始化SDK，在application:didFinishLaunchingWithOptions:中调用
 *
 *  @param orientation 屏幕旋转角度
 */
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation;

/**
 *  登录，调用前设置delegate
 */
- (void)doLogin;
/**
 *  注销，调用前设置delegate
 */
- (void)doLogout;
/**
 *  是否已登录
 *
 *  @return YES为已登录，NO为未登录
 */
- (BOOL)isLogined;
/**
 *  显示用户中心
 */
- (void)showUserCenter;

@end
