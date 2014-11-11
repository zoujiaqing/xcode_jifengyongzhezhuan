//
//  ZHPayDefines.h
//  ZHAccountSDK
//
//  Created by cc on 14-4-16.
//  Copyright (c) 2014年 haima. All rights reserved.
//  SDKVer 1.0.0


#ifndef ZHAccountSDK_ZHAccountDefines_h
#define ZHAccountSDK_ZHAccountDefines_h

/**
 *  @brief 订单信息，用于支付
 */
@interface ZHPayOrderInfo : NSObject

@property (nonatomic, copy) NSString *orderId;				    /* 订单号，必须保证唯一 */
@property (nonatomic, copy) NSString *productName;				/* 商品名称，不可为空 */
@property (nonatomic, copy) NSString *gameName;                 /* 游戏或平台名，可精确到服或区，不可为空 */
@property (nonatomic, copy) NSString *userParams;               /* 您的自定义参数，不超过255个字符 可以为空*/
@property (nonatomic, assign) float productPrice;               /* 商品价格，单位：元 */
@end

#pragma mark - 界面回调协议 *************************************************************

@protocol ZHPayDelegate <NSObject>

@optional

/**
 *  @brief SDK界面出现
 *
 *  @note  如登陆，账号信息界面等出现时，均会回调此方法。建议判断是否在游戏中途并将游戏临时暂停
 */
- (void)ZHPayViewIn;

/**
 *  @brief SDK界面退出
 *
 *  @note  如登陆，账号信息界面等消失时，均会回调此方法。
 */
- (void)ZHPayViewOut;

@end


#pragma mark - 支付结果回调协议 *************************************************************

@protocol ZHPayResultDelegate <NSObject>

typedef enum {
    
    kZHPayBalanceError,      /*余额不足*/
    
    kZHPayCreateOrderError,  /*订单创建错误*/
    
    kZHPayOldOrderError,     /*重复提交订单，请尝试更换订单号*/
    
    kZHPayNetworkingError,   /*网络不通畅（有可能已购买成功但客户端已超时,建议去自己服务器进行订单查询，以自己服务器为准）*/
    
    kZHPayServerError,       /*服务器错误*/
    
    kZHPayOtherError,        /*其他错误*/
    
}ZH_PAY_ERROR;

@optional

/**
 *	@brief	支付成功
 *
 *	@param 	orderInfo 订单信息
 *  @note   收到回调后，SDK服务器会异步通知开发者服务器，建议去自己服务器查询订单是否有效，金额是否正确。
 */
- (void)ZHPayResultSuccessWithOrder:(ZHPayOrderInfo *)orderInfo;

/**
 *	@brief	支付失败
 *
 *	@param 	orderInfo  订单信息
 *	@param 	errorType  错误类型，见ZH_PAY_ERROR
 */
- (void)ZHPayResultFailedWithOrder:(ZHPayOrderInfo *)orderInfo resultCode:(ZH_PAY_ERROR)errorType;

/**
 *	@brief  用户中途取消支付
 *
 *	@param	orderInfo  订单信息
 */
- (void)ZHPayResultCancelWithOrder:(ZHPayOrderInfo *)orderInfo;

@end


#pragma mark - 手动查询订单结果回调协议 *************************************************************

@protocol ZHPayCheckDelegate <NSObject>

typedef enum{
    
    kZHPayCheckStatusReady = 0, /* 待支付（已经创建第三方充值订单，但未支付）*/
    
    kZHPayCheckStatusSuccess,   /* 成功 */
    
    kZHPayCheckStatusExpired,   /* 过期失效 */
    
    kZHPayCheckStatusNotExist,  /* 订单不存在，未发起 */
    
}ZHPayCheckStatusType;

@optional
/**
 *  @brief 查询订单完毕
 *
 *  @param orderId     订单号
 *  @param money       订单金额（单位：元）
 *  @param statusType  订单状态
 */
- (void)ZHCheckOrderFinishedWithOrder:(NSString *)orderId
                               money:(float)money
                               status:(ZHPayCheckStatusType)statusType;

/**
 *  @brief 查询订单失败（网络中断，或服务器返回错误）
 */
- (void)ZHCheckOrderDidFailed:(NSString*)orderId;

@end


#endif
