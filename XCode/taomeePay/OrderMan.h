//
//  OrderMan.h
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OrderManDelegate.h"

@interface OrderMan : NSObject{
    //针对海马渠道
    NSString *_url;         //支付回调地址
    NSString *_thirdId;     //海马商店里的商品编号
    //==============
    
    NSString *_productId;
    NSString *_userId;
    NSString *_channelId;
    NSString *_serverId;
    NSString *_userCreateTime;
    NSString *_currency;
    NSString *_tradeNo;
    NSString *_amount;
    NSString *_productName;
    NSString *_addInfo;
    NSMutableURLRequest *_request;
    BOOL _isInnerMode;
    id<OrderManDelegate> _delegate;
}

@property (nonatomic, retain) NSString *url;
@property (nonatomic, retain) NSString *thirdId;

@property (nonatomic, retain) NSString *productId;
@property (nonatomic, retain) NSString *userId;
@property (nonatomic, retain) NSString *channelId;
@property (nonatomic, retain) NSString *serverId;
@property (nonatomic, retain) NSString *userCreateTime;
@property (nonatomic, retain) NSString *currency;
@property (nonatomic, retain) NSString *tradeNo;
@property (nonatomic, retain) NSString *amount;
@property (nonatomic, retain) NSString *productName;
@property (nonatomic, retain) NSString *addInfo;
@property (nonatomic, assign) BOOL isInnerMode;
@property (nonatomic, assign) id<OrderManDelegate> delegate;

+ (OrderMan *)sharedInstance;
+ (void)purgeSharedInstance;
- (void)fetchOrder;

@end
