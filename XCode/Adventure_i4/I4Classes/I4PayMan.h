//
//  I4PayMan.h
//  I4Demo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "I4LoginMan.h"

@protocol PayManDelegate <NSObject>
@required
- (void)didPaid;
- (void)didPaidFailedWithErrorDescription:(NSString *)des;
@end

@interface I4PayMan : NSObject {
    id<PayManDelegate> _delegate;
    id<LoinManDelegate> _loginDelegate;
    BOOL _isInnerMode;
}

@property (nonatomic, assign) id<PayManDelegate> delegate;
@property (nonatomic, assign) id<LoinManDelegate> loginDelegate;
@property (nonatomic, assign) BOOL isInnerMode;

+ (I4PayMan *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  支付，调用前先设置delegate
 *
 *  @param productId  产品的id
 *  @param userId     userId
 *  @param channelId  渠道号
 *  @param serverId   服务器ID
 *  @param currency   货币类型，"0"是人民币，"1"是美元
 *  @param createTime 角色创建时间
 */
- (void)doPayWithProductId:(NSString *)productId
                    userId:(NSString *)userId
                 channelId:(NSString *)channelId
                  serverId:(NSString *)serverId
                  currency:(NSString *)currency
         andUserCreateTime:(NSString *)createTime;

@end
