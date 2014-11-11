//
//  I4Man.h
//  I4Demo
//
//  Created by caesarhu on 14-7-17.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol LoinManDelegate <NSObject>

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

@interface I4LoginMan : NSObject {
    id<LoinManDelegate> delegate;
}

@property (assign, nonatomic) id<LoinManDelegate> delegate;

+ (I4LoginMan *)sharedInstance;
+ (void)purgeSharedInstance;

- (void)initSDK;

- (void)applicationOpenURL:(NSURL *)url SourceApplication:(NSString *)sourceApplication;

- (void)doLogin;

- (void)doLogout;

- (BOOL)isLogined;

- (void)showUserCenter;

- (NSString*)getUserInfoJson;

@end
