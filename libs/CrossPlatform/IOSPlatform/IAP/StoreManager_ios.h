//
//  StoreManager.h
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "StoreObserver_ios.h"
#import "ProductManager.h"




@interface StoreManager : NSObject<SKProductsRequestDelegate>
{
    NSMutableArray *purchasableObjects;
    StoreObserver *storeObserver;
    

    
    
    KProductInfo m_bProductInfo;
}
@property (nonatomic, retain) NSMutableArray *purchasableObjects;
@property (nonatomic, retain) StoreObserver *storeObserver;

//一次获取所有product的信息
- (void) requestProductData;

//获取当前物品的价格
- (float) convertCurrencyToUSD;

- (void) productPurchased:(KProductType) type;
+(StoreManager*)sharedManager;
- (void) buyFeature:(NSString*) featureId;

//检查是否有付了钱未完成的交易
//-(void)checkTrancation;

//购买成功增加金豆
-(void) addVipGolds;

-(const char*)getFormattedPriceAtIndex:(int) index;
@end
