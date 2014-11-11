//
//  HailstoneSDK.h
//  HailstoneSDK
//
//  Created by CocoaChina_yangjh on 13-11-25.
//  Copyright (c) 2013年 CocoaChina. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HailstoneSDK : NSObject

// 建议在应用启动完成时赋值
@property (nonatomic, copy) NSString *game;
@property (nonatomic, copy) NSString *server;
@property (nonatomic, copy) NSString *channel;
@property (nonatomic, assign) BOOL showLog;

/**
 *	@brief	HailstoneSDK单例
 *
 *	@return	HailstoneSDK对象
 */
+ (HailstoneSDK *)sharedInstance;

/**
 *	@brief	设备激活
 */
- (void)initGame;

/**
 *	@brief	获取SDK版本号
 *
 *	@return	返回SDK版本号
 */
+ (NSString *)sdkVersion;

/**
 *	@brief	帐号注册
 *
 *	@param 	account 	帐号名
 *	@param 	accountID 	帐号ID
 */
- (void)registerWithAccount:(NSString *)account andAccountID:(NSString *)accountID;

/**
 *	@brief	帐号登陆
 *
 *	@param 	account 	帐号名
 *	@param 	accountID 	帐号ID
 */
- (void)loginWithAccount:(NSString *)account andAccountID:(NSString *)accountID;

/**
 *	@brief	创建角色
 *
 *	@param 	account 	帐号名
 *	@param 	roleID 	角色ID
 *	@param 	roleName 	角色名
 *	@param 	charactor 	特点或职业
 */
- (void)createRoleWithAccount:(NSString *)account roleID:(NSString *)roleID
                     roleName:(NSString *)roleName andCharactor:(NSString *)charactor;

/**
 *	@brief	角色登陆
 *
 *	@param 	account 	帐号名
 *	@param 	roleID 	角色ID
 *	@param 	roleName 	角色名
 *	@param 	level 	角色等级
 */
- (void)roleLoginWithAccount:(NSString *)account roleID:(NSString *)roleID
                    roleName:(NSString *)roleName andLevel:(NSUInteger)level;

/**
 *	@brief	角色登出
 *
 *	@param 	account 	帐号名
 *	@param 	roleID 	角色ID
 *	@param 	roleName 	角色名
 *	@param 	level 	角色等级
 *	@param 	interval 	在线时长
 */
- (void)roleLogoutWithAccount:(NSString *)account roleID:(NSString *)roleID
                     roleName:(NSString *)roleName level:(NSUInteger)level
                  andInterval:(NSUInteger)interval;

/**
 *	@brief	充值
 *
 *	@param 	account 	帐号名
 *	@param 	payType 	充值方式
 *	@param 	cashAdd 	本次充值后身上剩余钱数（分）
 *	@param 	delta 	本次充值数（分）
 */
- (void)addCashWithAccount:(NSString *)account payType:(NSUInteger)payType
                   cashAdd:(NSUInteger)cashAdd andDelta:(NSUInteger)delta;

/**
 *	@brief	商城购买
 *
 *	@param 	account 	帐号名
 *	@param 	orderID 	订单ID
 *	@param 	itemID 	物品ID
 *	@param 	itemCount 	物品数量
 *	@param 	buyType 	购买方式
 *	@param 	cashLeft 	本次消费后身上剩余的钱数（分）
 *	@param 	delta 	本次消费钱数（分）
 */
- (void)shopTradeWithAccount:(NSString *)account orderID:(NSString *)orderID
                      itemID:(NSString *)itemID itemCount:(NSUInteger)itemCount
                     buyType:(NSString *)buyType cashLeft:(NSUInteger)cashLeft
                    andDelta:(NSUInteger)delta;

/**
 *	@brief	自定义事件
 *
 *	@param 	account 	帐号名
 *	@param 	roleID 	角色ID
 *	@param 	roleName 	角色名
 *	@param 	type 	自定义事件类型
 *	@param 	description 	描述
 */
- (void)customEventWithAccount:(NSString *)account roleID:(NSString *)roleID
                      roleName:(NSString *)roleName type:(NSString *)type
                andDescription:(NSString *)description;

@end
