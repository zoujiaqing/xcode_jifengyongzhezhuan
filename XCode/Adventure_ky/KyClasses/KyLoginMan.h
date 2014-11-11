//
//  KyLoginMan.h
//  KyDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KYSDK.h"
//#import "KYMobilePay.h"

@protocol LoginManDelegate <NSObject>
@required
- (void)didLoginWithUserId:(NSString *)userId
                  userName:(NSString *)userName
                  andToken:(NSString *)token;

- (void)didLoginFailedWithErrorDescription:(NSString *)des;

- (void)didLogout;
@end

@interface KyLoginMan : NSObject<KYSDKDelegate>{
    id<LoginManDelegate> _delegate;
}

@property (nonatomic, assign) id<LoginManDelegate> delegate;

+ (KyLoginMan *)sharedInstance;
+ (void)purgeSharedInstance;
/**
 *  在didFinishLaunchWithOptions中调用该方法
 *
 *  @param orientation
 */
- (void)initWithScreenOrientation:(UIInterfaceOrientation) orientation;
/**
 *  快用登陆成功的回调只有token返回，userId和username需要用token和appkey到后台取
 */
- (void)doLogin;
- (void)doLogout;
- (BOOL)isLogined;

- (void)loginWithLastUser;

- (void)showUserCenter;

- (void)backToLog;

- (NSString *)guid;
@end
