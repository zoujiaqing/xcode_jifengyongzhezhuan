//
//  AlipaySDK.h
//  AlipaySDK
//
//  Created by 方彬 on 14-4-28.
//  Copyright (c) 2014年 Alipay. All rights reserved.
//

typedef  BOOL(^SecurityBlock) (void);

FOUNDATION_EXPORT BOOL setSecurityBlock(SecurityBlock block);

#import <Foundation/Foundation.h>

typedef void(^CompletionBlock)(NSDictionary *resultDic);

@interface AlipaySDK : NSObject

/**
 *  创建支付单例服务
 *
 *  @return 返回单例对象
 */
+ (AlipaySDK *)defaultService;


/**
 *  支付接口
 *
 *  @param orderStr       订单信息
 *  @param schemeStr      调用支付的app注册在info。plist中的scheme
 *  @param compltionBlock 支付结果回调Block
 */
- (void)payOrder:(NSString *)orderStr
      fromScheme:(NSString *)schemeStr
        callback:(CompletionBlock)completionBlock;

/**
 *  处理钱包或者独立快捷app支付跳回商户app携带的支付结果Url
 *
 *  @param resultUrl 支付结果url，传入后由SDK解析，统一在上面的pay方法的callback中回调
 */
- (void)processOderWithPaymentResult:(NSURL *)resultUrl;

/**
 *  是否支付宝授权
 *
 *  @return YES为授权，NO反之
 */
- (BOOL)hasAuthorized;

/**
 *  当前版本号
 *
 *  @return 当前版本字符串
 */
- (NSString *)currentVersion;

/**
 *  測試所用，realse包无效
 *
 *  @param url  测试环境
 */
- (void)setUrl:(NSString *)url;



@end
