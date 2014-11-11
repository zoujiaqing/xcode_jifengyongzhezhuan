//
//  HailStoneMan.m
//  AppStoreAnalyse
//
//  Created by caesarhu on 14-6-4.
//  Copyright (c) 2014年 caesarhu. All rights reserved.
//

#import "HailStoneMan.h"
#import "HailstoneSDK.h"


@implementation HailStoneMan

+ (void)didFinishLaunchingWithChannel:(NSString *)channel server:(NSString *)server isAppStore:(BOOL)isAppStore {
    if (isAppStore) {
        [HailstoneSDK sharedInstance].game = HailStone_AppStore_AppId;
    }else {
        [HailstoneSDK sharedInstance].game = HailStone_JailBreak_AppId;
    }
    
    if (channel) {
        [HailstoneSDK sharedInstance].channel = channel;
    }
    if (server) {
        [HailstoneSDK sharedInstance].server = server;
    }
    [[HailstoneSDK sharedInstance] initGame];
}

+ (void)registerWithAccount:(NSString *)account andAccountID:(NSString *)accountID {
    [[HailstoneSDK sharedInstance] registerWithAccount:account andAccountID:accountID];
}

+ (void)loginWithAccount:(NSString *)account andAccountID:(NSString *)accountID {
    [[HailstoneSDK sharedInstance] loginWithAccount:account andAccountID:accountID];
}

+ (void)createRoleWithAccount:(NSString *)account roleID:(NSString *)roleID
                     roleName:(NSString *)roleName andCharactor:(NSString *)charactor {
    [[HailstoneSDK sharedInstance] createRoleWithAccount:account roleID:roleID roleName:roleName andCharactor:charactor];
}

+ (void)roleLoginWithAccount:(NSString *)account roleID:(NSString *)roleID
                    roleName:(NSString *)roleName andLevel:(NSUInteger)level {
    [[HailstoneSDK sharedInstance] roleLoginWithAccount:account roleID:roleID roleName:roleName andLevel:level];
}

+ (void)roleLogoutWithAccount:(NSString *)account roleID:(NSString *)roleID
                     roleName:(NSString *)roleName level:(NSUInteger)level
                  andInterval:(NSUInteger)interval {
    [[HailstoneSDK sharedInstance] roleLogoutWithAccount:account roleID:roleID roleName:roleName level:level andInterval:interval];
}

+ (void)addCashWithAccount:(NSString *)account payType:(NSUInteger)payType
                   cashAdd:(NSUInteger)cashAdd andDelta:(NSUInteger)delta {
    [[HailstoneSDK sharedInstance] addCashWithAccount:account payType:payType cashAdd:cashAdd andDelta:delta];
}

+ (void)shopTradeWithAccount:(NSString *)account orderID:(NSString *)orderID
                      itemID:(NSString *)itemID itemCount:(NSUInteger)itemCount
                     buyType:(NSString *)buyType cashLeft:(NSUInteger)cashLeft
                    andDelta:(NSUInteger)delta {
    [[HailstoneSDK sharedInstance] shopTradeWithAccount:account orderID:orderID itemID:itemID itemCount:itemCount buyType:buyType cashLeft:cashLeft andDelta:delta];
}

+ (void)customEventWithAccount:(NSString *)account roleID:(NSString *)roleID
                      roleName:(NSString *)roleName type:(NSString *)type
                andDescription:(NSString *)description {
    [[HailstoneSDK sharedInstance] customEventWithAccount:account roleID:roleID roleName:roleName type:type andDescription:description];
}

@end
