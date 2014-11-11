//
//  IAPHelper.m
//  AnimationPro
//
//  Created by Yusheng Huang on 13-7-22.
//
//

#import "IAPHelper.h"
#include "NcSDKManager.h"
#include "IAPManager.h"
#import "TrackingInmobi.h"


@implementation IAPHelper

@synthesize productRequest = _productRequest;
@synthesize productIdentifiers = _productIdentifiers;
@synthesize purchasedProductIdentifiers = _purchasedProductIdentifiers;
@synthesize products = _products;


- (id)initWithProductIdentifier:(NSSet *)productIdentifier
{
    if ((self = [super init]))
    {
        _productIdentifiers = productIdentifier;
        _purchasedProductIdentifiers = [NSMutableSet set];
        for (NSString* productIdentifier in _productIdentifiers)
        {
            BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
            if (productPurchased)
            {
                [_purchasedProductIdentifiers addObject:productIdentifier];
                NSLog(@"Previously Purchased...%@",productIdentifier);
            }
            else
            {
                NSLog(@"Not Purchased...%@", productIdentifier);
            }
        }
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    
    return self;
}

- (void)requestProducts
{
    NSLog(@"=====从iTunes Connect获取产品信息======");
    _productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productRequest.delegate = self;
    [_productRequest start];
}

- (BOOL)productPurchased:(NSString *)productIndentifier {
    
    return [_purchasedProductIdentifiers containsObject:productIndentifier];
}

// 没有确认的交易订单，要先确认掉，苹果才允许下一次继续购买这个商品
- (void)CloseUnConfirmTransactions
{
    NSLog(@"=====CloseUnConfirmTransactions=====");
    NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
    NSLog(@"=====transactions count=====:%d", transactions.count);
    for(int i = 0; i < transactions.count; i++)
    {
        //检测是否有未完成的交易
        NSLog(@"=====检测是否有未完成的交易=====");
        SKPaymentTransaction* transaction = [transactions firstObject];
        if (transaction.transactionState == SKPaymentTransactionStatePurchased) {
            [self completeTransaction:transaction];
            NSLog(@"=====CloseUnConfirmTransactions补单成功=====");
        }
        else if(transaction.transactionState == SKPaymentTransactionStatePurchasing)
        {
            NSLog(@"=====SKPaymentTransactionStatePurchasing=====");
        }
        else if(transaction.transactionState == SKPaymentTransactionStateFailed)
        {
            NSLog(@"=====SKPaymentTransactionStateFailed=====");
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        }
        else if(transaction.transactionState == SKPaymentTransactionStateRestored)
        {
            NSLog(@"=====SKPaymentTransactionStateRestored=====");
        }
    }
}

- (void)buyProduct:(NSString *)productIndentifier {
    NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
    bool hasFlag = false;
    NSLog(@"=====transactions count=====:%d", transactions.count);
    for(int i = 0; i < transactions.count; i++)
    {
        //检测是否有未完成的交易
        NSLog(@"=====buyProduct=====");
        SKPaymentTransaction* transaction = [transactions firstObject];
        if (transaction.transactionState == SKPaymentTransactionStatePurchased) {
            [self completeTransaction:transaction];
            NSLog(@"=====buyProduct补单成功=====");
        }
        else if(transaction.transactionState == SKPaymentTransactionStatePurchasing)
        {
            NSLog(@"=====SKPaymentTransactionStatePurchasing=====");
        }
        else if(transaction.transactionState == SKPaymentTransactionStateFailed)
        {
            NSLog(@"=====SKPaymentTransactionStateFailed=====");
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        }
        else if(transaction.transactionState == SKPaymentTransactionStateRestored)
        {
            NSLog(@"=====SKPaymentTransactionStateRestored=====");
        }
        hasFlag = true;
    }
    if(hasFlag == true)
    {
        return;
    }

    SKPayment* payment = [SKPayment paymentWithProductIdentifier:productIndentifier];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (NSString *)encode:(const uint8_t *)input length:(NSInteger)length {
    static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    
    NSMutableData *data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t *output = (uint8_t *)data.mutableBytes;
    
    for (NSInteger i = 0; i < length; i += 3) {
        NSInteger value = 0;
        for (NSInteger j = i; j < (i + 3); j++) {
            value <<= 8;
            
            if (j < length) {
                value |= (0xFF & input[j]);
            }
        }
        
        NSInteger index = (i / 3) * 4;
        output[index + 0] =                    table[(value >> 18) & 0x3F];
        output[index + 1] =                    table[(value >> 12) & 0x3F];
        output[index + 2] = (i + 1) < length ? table[(value >> 6)  & 0x3F] : '=';
        output[index + 3] = (i + 2) < length ? table[(value >> 0)  & 0x3F] : '=';
    }
    
    return [[[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding] autorelease];
}

#pragma mark - SKProductsRequestDelegate
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    NSLog(@"Load a list of products.....");
    _productRequest = nil;
    _products = response.products;
    for (SKProduct* skProduct in _products) {
        
        NSLog(@"Found product:%@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              skProduct.price.floatValue);
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error {
    
    NSLog(@"fail to load a list of products......");
    _productRequest = nil;
}


#pragma mark - SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    
    for (SKPaymentTransaction* transaction in transactions) {

        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                [[TrackingInmobi sharedInstance] trackTransaction:transaction];
                break;
                
            case SKPaymentTransactionStateFailed:
                [self failTransaction:transaction];
                [[TrackingInmobi sharedInstance] trackTransaction:transaction];
                
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                [[TrackingInmobi sharedInstance] trackTransaction:transaction];
                
            default:
                break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"CompleteTransaction.....%@", transaction.payment.productIdentifier);
    [self provideContentForProductIdentifier:transaction.payment.productIdentifier];
    
    NSString *jsonObjectString = [self encode:(uint8_t *)transaction.transactionReceipt.bytes length:transaction.transactionReceipt.length];
    NcSDKManager::sendInAppPurchaseString([jsonObjectString UTF8String]);
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    /////////////////
    NcSDKManager::hideWaitingLayer();
}

- (void)failTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"failTransaction.....");
    if (transaction.error.code != SKErrorPaymentCancelled) {
        
        NSLog(@"Transaction error : %@", transaction.error.localizedDescription);
    }
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    /////////////////
    NcSDKManager::hideWaitingLayer();
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"RestoreTransaction.....");
    [self provideContentForProductIdentifier:transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    /////////////////
    NcSDKManager::hideWaitingLayer();
}

- (void)provideContentForProductIdentifier:(NSString *)productIdentifier
{
    //afte the sever verify the receipt, it provide the the product content
}


- (void)dealloc
{
    [_productIdentifiers release];
    _productIdentifiers = nil;
    [_products release];
    _products = nil;
    [_purchasedProductIdentifiers release];
    _purchasedProductIdentifiers = nil;
    [_productRequest release];
    _productRequest = nil;
    [super dealloc];
}

@end
