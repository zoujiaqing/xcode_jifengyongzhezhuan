//
//  StoreManager.m
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "StoreManager_ios.h"
#include "UserData.h"


@implementation StoreManager

@synthesize purchasableObjects;
@synthesize storeObserver;


static NSString *KProduct[] =
{
    @"com.taomee.seer.vipgold1",
    @"com.taomee.seer.vipgold2",
    @"com.taomee.seer.vipgold3",
    @"com.taomee.seer.vipgold4",
    @"com.taomee.seer.vipgold5",
    @"com.taomee.seer.vipgold6",
};
static StoreManager *pSharedStoreManager;

- (void)productPurchased:(KProductType) type
{
    m_bProductInfo.type = type;
    [self buyFeature:KProduct[type]];
}

-(void)dealloc
{
    [pSharedStoreManager release];
    [purchasableObjects release];
    [storeObserver release];
    [super dealloc];
}

+(StoreManager*)sharedManager
{
    @synchronized(self) 
    {
        if (pSharedStoreManager == nil)
        {
            [[self alloc] init];
            pSharedStoreManager.purchasableObjects = [[NSMutableArray alloc] init];
            pSharedStoreManager.storeObserver  = [[StoreObserver alloc] init];
            [[SKPaymentQueue defaultQueue] addTransactionObserver:pSharedStoreManager.storeObserver];
        }
    }
    return pSharedStoreManager;
}
- (void) requestProductData
{
    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObjects:KProduct[KProductA],KProduct[KProductB], KProduct[KProductC],KProduct[KProductD],KProduct[KProductE],KProduct[KProductF],nil]];
    request.delegate = self;
    [request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    [purchasableObjects addObjectsFromArray:response.products];
 /*   
    for (int i=0; i<[purchasableObjects count]; i++)
    {
        SKProduct *product = [purchasableObjects objectAtIndex:i];
		NSLog(@"Feature: %@, Cost: %f, ID: %@",[product localizedTitle],
			  [[product price] doubleValue], [product productIdentifier]);
    }
  */
    [request release];
}

-(const char*)getFormattedPriceAtIndex:(int) index
{
    if ([purchasableObjects count]>0)
    {
        SKProduct *product = [purchasableObjects objectAtIndex:index];
        NSNumberFormatter *moneyFormatter = [[NSNumberFormatter alloc]init];    
        [moneyFormatter setNumberStyle:kCFNumberFormatterCurrencyStyle];
        [moneyFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];    
        [moneyFormatter setLocale:product.priceLocale];
        
        NSString *defaultString = [moneyFormatter stringFromNumber:product.price];
        [moneyFormatter release];
        return [defaultString UTF8String];
    }
    return "";
  
}

-(void) addVipGolds
{
    //UserData::addVipGoldFromShop(m_bProductInfo.type);
    m_bProductInfo.type = KProductUnknow;
    m_bProductInfo.num  = 0;
    m_bProductInfo.cost = 0.0f;
}
//转换成美分
- (float) convertCurrencyToUSD
{
    //ShopInfo_t *bInfo = GameItemDataManager::sharedManager()->getShopAtPage(3, (int)m_bProductInfo.type);
    //m_bProductInfo.cost = bInfo->price;
    return  m_bProductInfo.cost;
}

- (void) buyFeature:(NSString*) featureId
{           
    if([SKPaymentQueue canMakePayments])
    {
        SKPaymentQueue *queue= [SKPaymentQueue defaultQueue];
        NSArray *transactions = [queue transactions];
        int count = [transactions count];
        
        //only one pay in queue at the same time
        if(count == 0)
        {
            SKPayment *payment = [SKPayment paymentWithProductIdentifier:featureId];
            [queue addPayment:payment];     
        }          
        
    }else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"MyApp" message:@"You are not authorized to purchase from AppStore on Simulator"
													   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
		[alert show];
		[alert release];
    }
}
/*
-(void)checkTrancation
{
    SKPaymentQueue *queue= [SKPaymentQueue defaultQueue];
    NSArray *transactions = [queue transactions];
    NSLog(@"transactions count = %d",[transactions count]);
    NSString *str = [NSString stringWithFormat:@"%d",[transactions count]];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"队列交易数量" message:str delegate:nil cancelButtonTitle:@"cancel" otherButtonTitles: nil];
    [alert show];
    [alert release];
   
    for (SKPaymentTransaction *transaction in transactions)
    {
        NSString *msg = [NSString stringWithFormat:@"%d",transaction.transactionState];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"购买状态" message:msg delegate:nil cancelButtonTitle:@"cancel" otherButtonTitles: nil];
        [alert show];
        [alert release];
     
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                break;
                
            case SKPaymentTransactionStateFailed:
                break;
            case SKPaymentTransactionStateRestored:    
            default:
                break;
        }
    }
}
 */

#pragma mark Singleton Methods

+ (id)allocWithZone:(NSZone *)zone

{	
    @synchronized(self) {
		
        if (pSharedStoreManager == nil) {
			
            pSharedStoreManager = [super allocWithZone:zone];			
            return pSharedStoreManager;  // assignment and return on first allocation
        }
    }
	
    return nil; //on subsequent allocation attempts return nil	
}


- (id)copyWithZone:(NSZone *)zone
{
    return self;	
}

- (id)retain
{	
    return self;	
}

- (unsigned)retainCount
{
    return UINT_MAX;  //denotes an object that cannot be released
}

- (oneway void)release
{
    //do nothing
}

- (id)autorelease
{
    return self;	
}
@end
