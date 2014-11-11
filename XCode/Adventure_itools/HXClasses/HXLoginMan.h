//
//  HXLoginMan.h
//  iToolsDemo
//
//  Created by caesarhu on 14-4-23.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol LoginManDelegate <NSObject>
@required
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

- (void)didLoginFailedWithErrorDescription:(NSString *)des;

- (void)didLogout;
@end

@interface HXLoginMan : NSObject {
    id<LoginManDelegate> _delegate;
}

@property (assign, nonatomic) id<LoginManDelegate> delegate;

+ (HXLoginMan *)sharedInstance;
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

/**
 *  显示支付中心，必须调用，否则无法修改密码、查看购买记录
 */
- (void)showPlatformView;

- (NSString*)getUserInfoJson;

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

@end
