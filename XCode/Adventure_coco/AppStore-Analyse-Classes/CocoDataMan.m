//
//  CocoDataMan.m
//  AppStoreAnalyse
//
//  Created by caesarhu on 14-6-4.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "CocoDataMan.h"
#import "CKCocoData.h"
#import "AppStoreAnalyseConfig.h"

@implementation CocoDataMan

+ (void)didFinishLaunchingWithChannel:(NSString *)channel {
    [CKCocoData initWithAppId:CocoData_AppId reportPolicy:@"REALTIME" channel:channel];
}

+ (void)viewBegin:(NSString *)viewName {
    [CKCocoData viewBegin:viewName];
}

+ (void)viewEnd:(NSString *)viewName {
    [CKCocoData viewEnd:viewName];
}

+ (void)trackEvent:(NSString *)eventName label:(NSString *)label params:(NSDictionary *)params {
    if (label && params) {
        [CKCocoData event:eventName label:label parameters:params];
    }else if (label && !params) {
        [CKCocoData event:eventName label:label];
    }else if (!label && params) {
        [CKCocoData event:eventName parameters:params];
    }else {
        [CKCocoData event:eventName];
    }
}

@end
