//
//  IAPManager.m
//  AnimationPro
//
//  Created by Yusheng on 13-8-19.
//
//


#import "IAPManager.h"
#import "RageIAPHelper.h"

IAPManager::IAPManager()
{
    
}

IAPManager::~IAPManager()
{
    
}

void IAPManager::payForProduct(std::string productId)
{
    NSLog(@"==== NcSDKManager::payForProduct =====");
    NSString* productIdentifier = [NSString stringWithCString:productId.c_str() encoding:NSUTF8StringEncoding];
    [[RageIAPHelper sharedInstance] buyProduct:productIdentifier];
}

void IAPManager::CloseUnConfirmTransactions()
{
    [[RageIAPHelper sharedInstance] CloseUnConfirmTransactions];
}