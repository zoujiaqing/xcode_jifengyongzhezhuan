//
//  IPAYAppPay.h
//  IAppPayDemo1
//
//  Created by iapppay on 13-6-25.
//  Copyright (c) 2013年 Kong. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IPAYiAppPayOrder.h"

@protocol IPAYiAppPayContentProviderDelegate <NSObject>
@optional
- (void)getInitStatus: (BOOL)isSucceeded;

@end


@protocol IPAYiAppPayLoginDelegate <NSObject>
@optional
- (void)loginDidSuccessWithUserName: (NSString *)userName;
- (void)loginDidFail;
- (void)loginDidCancel; // 游戏自行根据用户登录返回状态，确定强制用户登录的逻辑和UI。

@end

typedef enum IPAYiAppPayPaymentStatusCodeType: NSInteger {
    IPAY_PAYMENT_CANCELED = 99,
    IPAY_PAYMENT_FAILED = -1,
    IPAY_PAYMENT_SUCCESS = 0
} IPAYiAppPayPaymentStatusCodeType;

@protocol IPAYiAppPayPaymentDelegate <NSObject>
@optional
- (void)paymentStatusCode: (IPAYiAppPayPaymentStatusCodeType)statusCode
                signature: (NSString *)signature
               resultInfo: (NSString *)resultInfo;

@end

@interface IPAYiAppPay : NSObject

@property (nonatomic, weak) id<IPAYiAppPayContentProviderDelegate> cpDelegate;
@property (nonatomic, weak) id<IPAYiAppPayLoginDelegate> loginDelegate;
@property (nonatomic, weak) id<IPAYiAppPayPaymentDelegate> payDelegate;

// called in - (BOOL)application: didFinishLaunchingWithOptions:
+ (IPAYiAppPay *)sharediAppPay;

- (void)initializeWithAppKey: (NSString *) iappKey
                    andAppID: (NSString *) iappID
                  andWaresID: (NSUInteger) waresid
            andUIOrientation: (UIInterfaceOrientation) supportedOrientation
               andCPDelegate: (id<IPAYiAppPayContentProviderDelegate>) delegate;


// called in - (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
- (void)handleOpenurl: (NSURL *) url;

- (void)showLoginViewWithLoginDelegate:(id<IPAYiAppPayLoginDelegate>)delegate;

- (NSString *)getOrderSignature: (IPAYiAppPayOrder *) order
                      withAppID: (NSString *)iappID
                      andAppKey: (NSString *)iappKey;

- (void)checkoutWithOrder: (IPAYiAppPayOrder *)order
        andOrderSignature: (NSString *) orderSign
       andPaymentDelegate: (id<IPAYiAppPayPaymentDelegate>)delegate;

- (BOOL)verifyPaymentSignature: (NSString *) paySign withAppKey: (NSString *)iappKey;

@end
