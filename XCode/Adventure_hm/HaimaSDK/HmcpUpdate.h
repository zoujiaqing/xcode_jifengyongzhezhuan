//
//  cpupdate.h
//  cpupdate
//
//  Created by rong on 14-7-25.
//  Copyright (c) 2014年 rong. All rights reserved.
//  ver：2.0

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol HmcpUpdateDelegate <NSObject>
@optional
/**
 *  @brief 检查更新回调
 *
 *  @param isSuccess 请求成功时返回YES
 *  @param update 有新版本时为YES
 *  @param force 本次是否是强制更新，强制时为YES
 *
 *  @note  请求成功但无更新时，sdk不会提示。其他情况均会提示用户
 */
- (void)checkUpdateFinish:(BOOL)isSuccess shouldUpdate:(BOOL)update isForceUpdate:(BOOL)force;

@end

@interface HmcpUpdate : NSObject

+ (HmcpUpdate *)sharedUpdate;

/**
 *	@brief	通过海马服务器检查更新
 *
 *	@param 	test 	是否是测试模式。 测试模式下，若海马服务器中有该应用，则不论版本号，肯定提示更新。
 *
 */
- (void)checkUpdateForTest:(BOOL)test delegate:(id<HmcpUpdateDelegate>)delegate;

/**
 *  设置是否打印log日志
 *
 */
- (void)setLogEnable:(BOOL)log;

@end
