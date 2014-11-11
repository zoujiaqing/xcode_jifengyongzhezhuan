//
//  AsPlatformSDK.h
//  AsPlatformSDK
//
//  Created by i4.cn\STM on 14-2-20.
//  Copyright (c) 2014年 i4.cn. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 * @brief AsWeb页面关闭回调参数
 */
typedef enum{
    /**
     * 返回充值页面
     */
	AsWebViewCodeRecharge = 1,
    /**
     * 返回充值并且兑换页面
     */
    AsWebViewCodeRechargeAndExchange = 2,
}AsWebViewCode;


/**
 * @brief 错误范围，用来标识错误是接口返回的还是SDK返回的。
 */
typedef enum{
    /**
     * 关闭接口为登录页面
     */
	AsLoginViewPageCode	= 1,
    /**
     * 关闭接口为注册
     */
    AsRegisterViewPageCode = 2,
}AsPageCode;


/**
 * @brief 错误范围，用来标识错误是接口返回的还是SDK返回的。
 */
typedef enum{
    /**
     * 购买成功
     */
	AsPayResultCodeSucceed	= 0,
    /**
     * 用户离线，禁止访问
     */
    AsPayResultCodeUserOffLine = 1,
    /**
     * 非法访问，可能用户已经下线
     */
    AsPayResultCodeUserNotExist = 2,
    /**
     * 爱思币余额不足 必选参数丢失
     */
    AsPayResultCodeParamLost = 3,
    /**
     * 消费金额填写不正确
     */
    AsPayResultCodeNotSufficientFunds = 4,
    /**
     * 用户中途取消
     */
    AsPayResultCodeCancel = 5,
}AsPayResultCode;

/**
 * @protocol   AsPlatformSDKDelegate
 * @brief   SDK接口回调协议
 */
@protocol AsPlatformSDKDelegate <NSObject>
@required

//-SDK 1.5.2 - 新增的支付宝app支付的结果回调
/**
 * @brief   支付宝app的支付结果回调
 * @param   INPUT   statusCode       接口返回的结果编码
 * 9000     订单支付成功
 * 8000     正在处理
 * 4000     订单支付失败
 * 6001     用户中途取消
 * 6002     网络连接出错
 * @return  无返回
 */
- (void)asAlixPayResultCallBack:(int)statusCode;

//-SDK 1.5.2 - 新增的银联sdk支付的结果回调
/**
 * @brief   银联sdk的支付结果回调
 * @param   INPUT   result       接口返回的结果
 * success  支付成功
 * fail     支付失败
 * cancel   用户取消支付。
 * @return  无返回
 */
- (void)asUPPayPluginResultCallBack:(NSString *)result;

//-SDK 1.4.1 - 新增的关闭用户中心回调
/**
 * @brief   关闭用户中心的回调
 * @param   INPUT   paramCenterViewCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asClosedCenterViewCallBack;


/**
 * @brief   余额大于所购买道具
 * @param   INPUT   paramAsPayResultCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asPayResultCallBack:(AsPayResultCode)paramPayResultCode;


/**
 * @brief   验证更新成功后
 * @noti    分别在非强制更新点击取消更新和暂无更新时触发回调用于通知弹出登录界面
 * @return  无返回
 */
- (void)asVerifyingUpdatePassCallBack;


/**
 * @brief   登录成功回调
 * @param   INPUT   paramToken       字符串token
 * @return  无返回
 */
- (void)asLoginCallBack:(NSString *)paramToken;


/**
 * @brief   关闭Web页面后的回调
 * @param   INPUT   paramWebViewCode    接口返回的页面编码
 * @return  无返回
 */
- (void)asCloseWebViewCallBack:(AsWebViewCode)paramWebViewCode;


/**
 * @brief   关闭SDK客户端页面后的回调
 * @param   INPUT   paramAsPageCode       接口返回的页面编码
 * @return  无返回
 */
- (void)asClosePageViewCallBack:(AsPageCode)paramPageCode;


/**
 * @brief   注销后的回调
 * @return  无返回
 */
- (void)asLogOffCallBack;
@end

@interface AsPlatformSDK : NSObject


@property (nonatomic, retain) id<AsPlatformSDKDelegate> delegate;

/**
 * @brief     初始化SDK信息
 * @return    AsPlatformSDK    生成的AsPlatformSDK对象实例
 */
+ (AsPlatformSDK *)sharedInstance;


/**
 * @brief     弹出爱思登录页面
 * @return    无返回
 */
- (void)showLogin;


/**
 * @brief     弹出爱思中心页面
 * @return    无返回
 */
- (void)showCenter;


/**
 * @brief     兑换道具
 * @noti      只有余额大于道具金额时候才有客户端回调。余额不足的情况取决与LongComet参数，LongComet = YES，则为充值兑换。回调给服务端，LongComet = NO ，则只是打开充值界面
 * @param     INPUT paramPrice      商品价格，价格必须为大于等于1的int类型
 * @param     INPUT paramBillNo     商品订单号，订单号长度请勿超过30位，参有特殊符号
 * @param     INPUT paramBillTitle  商品名称
 * @param     INPUT paramRoleId     角色id，回传参数，若无请填0
 * @param     INPUT paramZoneId     开发者中心后台配置的分区id，若无请填写0
 * @return    无返回
 */
- (void)exchangeGoods:(int)paramPrice BillNo:(NSString *)paramBillNo BillTitle:(NSString *)paramBillTitle RoleId:(NSString *)paramRoleId ZoneId:(int)paramZoneId;


/**
 * @brief     SDK检查游戏版本更新
 * @return    无返回
 */
- (void)checkGameUpdate;

/**
 * @brief   注销
 * @return  无返回
 */
- (void)asLogout;


/**
 * @brief     获取当前用户id
 * @return    返回当前用户id
 */
- (u_int64_t)currentUserId;


/**
 * @brief     获取当前用户名
 * @return    返回当前用户名
 */
- (NSString *)currentUserName;

@end
