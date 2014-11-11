//
//  ZHPayPlatform.h
//  AccountTestDemo
//
//  Created by 吕昌业 on 14-4-14.
//  Copyright (c) 2014年 haima. All rights reserved.
//  Ver:1.0.0

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ZHPayDefines.h"

#pragma mark - 平台初始化 *******************************************************

@interface ZHPayPlatform : NSObject

/**
 *  @brief 初始化sdk
 *
 *  @param appid 注册时分配的appId
 */
+ (void)initWithAppId:(NSString *)appid;

/**
 *  @brief 获取AppId
 *
 *  @note  初始化成功后才有值
 */
+ (NSString *)getAppId;

/**
 *  @brief 设置ZHPayDelegate
 *
 *  @note  设置后才可收到登录相关回调
 */
+ (void)setZHPayDelegate:(id<ZHPayDelegate>)delegate;

/**
 *  @brief 设置日志输出
 *
 *  @param enable 默认为NO,不打印日志
 */
+ (void)setLogEnable:(BOOL)enable;

/**
 *  @brief 设置支持设备方向
 *
 *  @param orientation 默认为UIInterfaceOrientationMaskLandscape 
 */
+ (void)setSupportOrientation:(UIInterfaceOrientationMask)orientation;


/**
 *  @brief 获取促销公告
 *
 *  @note  没有活动时longString，shortString均为nil
 */
+ (void)getAdStringWithCallBack:(void (^)(NSString *longString,NSString *shortString))callBack;

/**
 *  @brief 获取海马支付是否可用
 *
 *  @note  available为YES表示sdk可用
 */
+ (void)getSDKIsAvailableWithCallBack:(void (^)(BOOL available))callBack;

@end


#pragma mark - 支付API *****************************************************

@interface ZHPayPlatform(ZHPayOrder)

/**
 *  @brief  发起订单请求，未登录时会自动弹出登陆框让用户先登录
 *
 *  @param  orderInfo 订单信息
 *  @result 若订单格式有误，返回NO. 无误返回YES.
 */
+ (BOOL)startPay:(ZHPayOrderInfo *)orderInfo delegate:(id<ZHPayResultDelegate>)payResultDelegate;

/**
 *  @brief  查询订单是否成功
 *
 *  @param  orderId	支付订单号
 */
+ (void)checkPayOrder:(NSString *)orderId delegate:(id<ZHPayCheckDelegate>)checkOrderDelegate;

@end



