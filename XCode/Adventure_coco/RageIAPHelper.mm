//
//  RageIAPHelper.m
//  AnimationPro
//
//  Created by Yusheng Huang on 13-7-22.
//
//

#import "RageIAPHelper.h"

@implementation RageIAPHelper

+ (RageIAPHelper *)sharedInstance
{
    static dispatch_once_t once;
    static RageIAPHelper* sharedIAPHelper;
    dispatch_once(&once, ^{
        NSSet* productIdentifier = [NSSet setWithObjects:
                                    IAP_PID_1,
                                    IAP_PID_2,
                                    IAP_PID_3,
                                    IAP_PID_4,
                                    IAP_PID_5,
                                    IAP_PID_6,
                                    IAP_PID_7,
                                    IAP_PID_8,nil];
        
        sharedIAPHelper = [[RageIAPHelper alloc] initWithProductIdentifier:productIdentifier];
    });
    
    return sharedIAPHelper;
}

@end
