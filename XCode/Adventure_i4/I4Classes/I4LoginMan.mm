//
//  I4Man.m
//  I4Demo
//
//  Created by caesarhu on 14-7-17.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "I4LoginMan.h"
#import "AsInfoKit.h"
#import "AsPlatformSDK.h"
#import "OrderContants.h"

#define I4_SAFE_OBJ(obj)      ((obj)?(obj):(@""))


@interface I4LoginMan() <AsPlatformSDKDelegate> {
    BOOL isLogined;
    NSString *theUserId;
}

@end

static I4LoginMan *instance = nil;

@implementation I4LoginMan

@synthesize delegate = _delegate;

- (id)init {
    if (self = [super init]) {
        isLogined = NO;
    }
    return self;
}

+ (I4LoginMan *)sharedInstance {
    @synchronized(self) {
        if (!instance) {
            instance = [[I4LoginMan alloc] init];
        }
        return instance;
    }
    return nil;
}

+ (void)purgeSharedInstance {
    @synchronized(self) {
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (void)applicationOpenURL:(NSURL *)url SourceApplication:(NSString *)sourceApplication {
    if ([sourceApplication isEqualToString:@"com.alipay.iphoneclient"]) {
        [[AsInfoKit sharedInstance] alixPayResult:url];
    }else if ([sourceApplication isEqualToString:@"com.tencent.xin"]) {
        [[AsInfoKit sharedInstance] weChatPayResult:url];
    }
}

- (void)initSDK {
    [[AsInfoKit sharedInstance] setAppId:APP_ID];
    [[AsInfoKit sharedInstance] setAppKey:APP_KEY];
    [[AsInfoKit sharedInstance] setLogData:NO];
    [[AsInfoKit sharedInstance] setCloseRecharge:NO];
    //[[AsInfoKit sharedInstance] setCloseRechargeAlertMessage:@"充值功能暂时不开放"];
    [[AsInfoKit sharedInstance] setLongComet:YES];
    [[AsPlatformSDK sharedInstance] setDelegate:self];
    [[AsPlatformSDK sharedInstance] checkGameUpdate];
}


- (void)doLogin {
    [[AsPlatformSDK sharedInstance] setDelegate:self];
    [[AsPlatformSDK sharedInstance] showLogin];
}

- (void)doLogout {
    [[AsPlatformSDK sharedInstance] setDelegate:self];
    [[AsPlatformSDK sharedInstance] asLogout];
}

- (void)showUserCenter {
    [[AsPlatformSDK sharedInstance] showCenter];
}

- (BOOL)isLogined {
    return isLogined;
}

- (NSString *)getUserInfoJson {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:I4_SAFE_OBJ(theUserId) forKey:@"userId"];
    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:nil];
    return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}


#pragma mark - AS代理回调
/**
 * @brief   余额大于所购买道具
 * @param   INPUT   paramAsPayResultCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asPayResultCallBack:(AsPayResultCode)paramPPPayResultCode {

}


/**
 * @brief   验证更新成功后
 * @noti    分别在非强制更新点击取消更新和暂无更新时触发回调用于通知弹出登录界面
 * @return  无返回
 */
- (void)asVerifyingUpdatePassCallBack {
    if ([_delegate respondsToSelector:@selector(didFinishInitSDK)]) {
        [_delegate didFinishInitSDK];
    }
}


/**
 * @brief   登录成功回调
 * @param   INPUT   paramToken       字符串token
 * @return  无返回
 */
- (void)asLoginCallBack:(NSString *)paramToken {
    if (paramToken && [paramToken length] > 0) {
        isLogined = YES;
        if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
            theUserId = [NSString stringWithFormat:@"%llu", [[AsPlatformSDK sharedInstance] currentUserId]];
            [theUserId retain];
            [_delegate didLoginWithUserId:theUserId
                                 userName:[[AsPlatformSDK sharedInstance] currentUserName]
                                 andToken:paramToken];
        }
    }else {
        if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
            [_delegate didLoginFailedWithErrorCode:1];
        }
    }
}

/**
 * @brief   关闭用户中心的回调
 * @param   INPUT   paramCenterViewCode       接口返回的结果编码
 * @return  无返回
 */
- (void)asClosedCenterViewCallBack {

}


/**
 * @brief   关闭Web页面后的回调
 * @param   INPUT   paramWebViewCode    接口返回的页面编码
 * @return  无返回
 */
- (void)asCloseWebViewCallBack:(AsWebViewCode)paramWebViewCode {

}


/**
 * @brief   关闭SDK客户端页面后的回调
 * @param   INPUT   paramAsPageCode       接口返回的页面编码
 * @return  无返回
 */
- (void)asClosePageViewCallBack:(AsPageCode)paramPPPageCode {
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:0];
    }
}


/**
 * @brief   注销后的回调
 * @return  无返回
 */
- (void)asLogOffCallBack {
    isLogined = NO;
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        [_delegate didLogout];
    }
}

- (void)dealloc {
    [super dealloc];
}

@end
