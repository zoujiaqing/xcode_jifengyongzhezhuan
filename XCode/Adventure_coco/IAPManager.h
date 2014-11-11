//
//  IAPManager.h
//  AnimationPro
//
//  Created by Yusheng on 13-8-19.
//
//
/*
#import <Foundation/Foundation.h>

@interface IAPManager : NSObject

@end
*/
#ifndef __AnimationPro__IAPManager__
#define __AnimationPro__IAPManager__

#include <iostream>

class IAPManager
{
public:
    
    IAPManager();
    virtual ~IAPManager();
    
    static void payForProduct(std::string productId);
    static void CloseUnConfirmTransactions();
};


#endif /* defined(__AnimationPro__IAPManager__) */