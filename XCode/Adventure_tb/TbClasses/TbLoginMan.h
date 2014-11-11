//
//  TbLoginMan.h
//  TbDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TBPlatform/TBPlatform.h>

@protocol LoginManDelegate <NSObject>
@required
//登录成功的回调
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

- (void)didLoginFailedWithErrorCode:(int)errorCode;
//从平台离开的回调
- (void)didLeavePlatform;
//注销的回调
- (void)didLogout;
//初始化完成的回调
- (void)didFinishInitSDK;
@end

@interface TbLoginMan : NSObject{
    id<LoginManDelegate> _delegate;
}

@property (nonatomic, assign) id<LoginManDelegate> delegate;

+ (TbLoginMan *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  初始化SDK，务必在applicaiton:didFinishLaunchingWithOptions中调用，调用前设置delegate
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
 *  @return 是否已登录的结果
 */
- (BOOL)isLogined;
/**
 *  切换账号
 */
- (void)switchAccount;

- (NSString*)getUserInfoJson;
/**
 *  设置工具栏是否隐藏
 *
 *  @param isHidden 是否隐藏
 */
- (void)setToolbarHidden:(BOOL)isHidden;
/**
 *  工具栏是否已隐藏
 *
 *  @return 是否已隐藏
 */
- (BOOL)isToolBarHidden;

@end
