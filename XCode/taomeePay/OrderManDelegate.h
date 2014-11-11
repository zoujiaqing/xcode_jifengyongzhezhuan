//
//  OrderManDelegate.h
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol OrderManDelegate <NSObject>

@required
- (void)didFectchOrderWithProductId:(NSString *)productId
                             amount:(NSString *)amount
                            tradeNo:(NSString *)tradeNo
                        productName:(NSString *)productName
                         andAddInfo:(NSString *)addInfo;
- (void)didFailedFetchOrder;

@optional
- (void)didFectchOrderWithProductId:(NSString *)productId
                             amount:(NSString *)amount
                            tradeNo:(NSString *)tradeNo
                        productName:(NSString *)productName
                        callbackUrl:(NSString *)url
                            thirdId:(NSString *)thirdId
                         andAddInfo:(NSString *)addInfo;

@end
