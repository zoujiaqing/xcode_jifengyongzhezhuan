//
//  HaimaLoginMan.h
//  HaimaDemo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import <Foundation/Foundation.h>

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

//初始化完成的回调
- (void)didFinishInitSDK;
@end

@interface HaimaLoginMan : NSObject {
    id<LoginManDelegate> delegate;
}

@property(nonatomic, assign) id<LoginManDelegate> delegate;

+ (HaimaLoginMan *)sharedInstance;
+ (void)purgeSharedInstance;

- (void)applicationHandleOpenURL:(NSURL *)url;

- (void)applicationOpenURL:(NSURL *)url;

/**
 *  初始化SDK，在application:didFinishLaunchingWithOptions:中调用
 *
 *  @param orientation 屏幕旋转角度
 */
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation;

- (void)doLogin;


@end
