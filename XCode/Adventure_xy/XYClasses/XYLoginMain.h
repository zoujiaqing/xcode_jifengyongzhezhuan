//
//  XYLoginMain.h
//  XYDemo
//
//  Created by caesarhu on 14-8-5.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol LoginManDelegate <NSObject>
@required
//登录成功的回调
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

- (void)didLoginFailedWithErrorCode:(int)errorCode;
//注销的回调
- (void)didLogout;
//初始化完成的回调
- (void)didFinishInitSDK;
@end

@interface XYLoginMain : NSObject{
    id<LoginManDelegate> _delegate;
}

@property (nonatomic, assign) id<LoginManDelegate> delegate;

+ (XYLoginMain *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  初始化SDK，务必在applicaiton:didFinishLaunchingWithOptions中调用，调用前设置delegate
 *
 *  @param orientation 屏幕旋转角度
 */
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation;

/**
 * @brief 需要在以下Application Delegate回调方法中调用
 *- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
 *- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
 *
 */
- (void)applicationHandleOpenURL:(NSURL*)url;
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
 *  工具条是否隐藏
 *
 *  @return 是否隐藏
 */
- (BOOL)isToolBarHidden;

/**
 *  设置工具条的显示和隐藏
 *
 *  @param isHidden 是否隐藏
 */
- (void)setToolBarHidden:(BOOL)isHidden;


/**
 *  切换账号
 */
- (void)switchAccount;

/**
 *  显示用户中心
 */
- (void)showUserCenter;

- (NSString*)getUserInfoJson;

@end
