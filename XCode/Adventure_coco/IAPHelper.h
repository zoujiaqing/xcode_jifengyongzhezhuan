//
//  IAPHelper.h
//  AnimationPro
//
//  Created by Yusheng Huang on 13-7-22.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#define IAP_PID_1 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier1"
#define IAP_PID_2 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier5"
#define IAP_PID_3 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier15"
#define IAP_PID_4 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier30"
#define IAP_PID_5 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier50"
#define IAP_PID_6 @"com.cocoentertainment.jifengyongzhezhuan.iap.tier60"
#define IAP_PID_7 @"com.cocoentertainment.jifengyongzhezhuan.iap.card.tier5"
#define IAP_PID_8 @"com.cocoentertainment.jifengyongzhezhuan.iap.card.tier4"

@interface IAPHelper : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    
    SKProductsRequest* _productRequest;
    NSSet* _productIdentifiers;
    NSMutableSet* _purchasedProductIdentifiers;
    NSArray* _products;
    
}

@property (nonatomic, retain) SKProductsRequest* productRequest;
@property (nonatomic, retain) NSSet* productIdentifiers;
@property (nonatomic, retain) NSMutableSet* purchasedProductIdentifiers;
@property (nonatomic, retain) NSArray* products;

- (id)initWithProductIdentifier:(NSSet *)productIdentifier;
- (BOOL)productPurchased:(NSString *)productIndentifier;

- (void)buyProduct:(NSString *)productIndentifier;
- (void)CloseUnConfirmTransactions;
- (void)requestProducts;

- (NSString *)encode:(const uint8_t *)input length:(NSInteger)length;

@end
