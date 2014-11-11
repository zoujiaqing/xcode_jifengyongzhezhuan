//
//  HaimaPayMan.h
//  HaimaDemo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol PayManDelegate <NSObject>
@required
- (void)didPaid;
- (void)didPaidFailedWithErrorDescription:(NSString *)des;
@end

@interface HaimaPayMan : NSObject {
    id<PayManDelegate> _delegate;
    BOOL _isInnerMode;
}

@property (nonatomic, assign) id<PayManDelegate> delegate;
@property (nonatomic, assign) BOOL isInnerMode;

+ (HaimaPayMan *)sharedInstance;
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
