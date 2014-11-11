//
//  TaomeeLogin.h
//  TaomeeLogin
//
//  Created by Delle  on 13-5-6.
//  Copyright (c) 2013年 Delle . All rights reserved.
//
//  Required frameworks
//  1) Foundation.framework
//  2) CoreLocation.framework
//  3) CoreGraphics.framework

/***
 // Version: V1.0.0 Init.
 ***/

//  0x00 HI ME LO
//  00   01 00 00

#define TAOMEE_LOGIN_VERSION  0x010000

#import <Foundation/Foundation.h>

@protocol TaomeeLoginDelegate <NSObject>

@optional

//
//  didRegistWithUserId:userName:andPassword:
//  注册成功时，回调接口。返回米米号，用户名和密码.
//
//  Param userId: 米米号.
//  Param userName: 用户名.
//
//  Returns:
//  void.
//
- (void)didRegistWithUserId:(NSString *)userId andUserName:(NSString *)userName;

//
//  注册失败.
//
- (void)registFailed:(NSInteger)errorCode;

//
//  loginSuccWithUserId:andSession:
//  成功登录时，回调接口。返回米米号，用户名，密码和session.
//
//  Param userId:  米米号.
//  Param userName: 用户名.
//  Param session: session.
//
//  Returns:
//  void.
//
- (void)loginSuccWithUserId:(NSString *)userId
                   userName:(NSString *)userName
                 andSession:(NSString *)session;

//
//  登录失败.
//
- (void)loginFailed:(NSInteger)errorCode;

//
//  修改密码成功的回调接口.
//
- (void)changedPasswordSucc;

//
//  修改密码失败的回调接口.
//
- (void)changedPasswordFailed:(NSInteger)errorCode;

@end

@interface TaomeeLogin : NSObject{
    NSInteger _errorCode;
    id<TaomeeLoginDelegate> _delegate;
}

//
//  创建单体.
//
+ (TaomeeLogin *)sharedInstance;

//
//  销毁单体
//
+ (void)purgeSharedInstance;

//
//  设置内网模式.
//
+ (void)isInnerMode:(BOOL)isInner;

//
//  初始化gameid & channel
//
//  Returns:
//  void.
//
+ (void)initGameId:(NSString *)gameId andChannel:(NSString *)channel;

//
//  设置超时时间（单位：秒）,默认24秒
//
+ (void)setTimeout:(float)timeout;

//
//  设置保存用户登录信息，默认记住用户名、密码。
//
+ (void)setRemmemberFlag:(BOOL)isRemmember;

//
//  获取最近登录记录.
//
+ (NSDictionary *)getRecentLoginInfo;

//
//  获取游戏的用户登录记录.
//
+ (NSArray *)getAllLoginInfo;

//
//  通过用户名删除用户登录记录.
//
+ (BOOL)deleteUserInfoByUserName:(NSString *)userName;

//
//  loginWithUserName:password:andDelegate:
//  用户自定义用户名、密码进行登录.
//
//  Param userName: 自定义用户名.
//  Param password: 密码.
//  Param isRecord: 是否本地记录
//  Param delegate: 登录回调delegate.
//
//  Returns:
//  void.
//
- (void)loginWithUserName:(NSString *)userName
                 password:(NSString *)password
            isLocalRecord:(BOOL)isRecord
              andDelegate:(id)delegate;

//
//  doRegisterWithUserName:password:andDelegate:
//  用户自定义用户名、密码进行注册.
//
//  Param userName: 自定义用户名.
//  Param password: 密码.
//  Param confirmPassword: 确认密码
//  delegate: 登录回调delegate.
//
//  Returns:
//  void.
//
- (void)doRegisterWithUserName:(NSString *)userName
                      password:(NSString *)password
               confirmPassword:(NSString *)confirmPassword
                   andDelegate:(id)delegate;

//
//  changePassword:oldPassword:andNewPassword:
//  用户修改密码.
//
//  Param userId: 米米号
//  Param userName: 用户名.
//  Param oldPassword: 老密码.
//  Param newPassword: 新密码.
//  Param delegate : 回调delegate.
//
//  Returns:
//  void.
//
- (void)changePasswordWithUserId:(NSString *)userId
                        userName:(NSString *)userName
                     oldPassword:(NSString *)oldPassword
                     newPassword:(NSString *)newPassword
                 confirmPassword:(NSString *)confirmPassword
                     andDelegate:(id)delegate;


@end
