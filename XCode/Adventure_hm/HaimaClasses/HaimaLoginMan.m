//
//  HaimaLoginMan.m
//  HaimaDemo
//
//  Created by caesarhu on 14-7-18.
//  Copyright (c) 2014年 HuXiangkun. All rights reserved.
//

#import "HaimaLoginMan.h"
#import "IPAYiAppPay.h"
#import "OrderContants.h"
#import "HmcpUpdate.h"

static HaimaLoginMan *instance = nil;

@interface HaimaLoginMan() <IPAYiAppPayLoginDelegate, UIAlertViewDelegate, HmcpUpdateDelegate> {
    NSMutableData *dataUpdateInfo;
    NSString *updateURL;
}

@end

@implementation HaimaLoginMan
@synthesize delegate = _delegate;

+ (HaimaLoginMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[HaimaLoginMan alloc] init];
        }
        
        return instance;
    }
    
    return nil;
}

+ (void)purgeSharedInstance
{
    @synchronized(self){
        if (instance) {
            [instance release], instance = nil;
        }
    }
}

- (id)init
{
    if (self=[super init]) {

    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
    [updateURL release];
}



////检查更新 old way
//- (void)checkUpdate {
//    NSString *strUrl = [NSString stringWithFormat:@"http://app.haima.me/appinst/install.aspx?bundleID=%@", [[NSBundle mainBundle] bundleIdentifier]];
//    NSLog(@"url :%@", strUrl);
//    NSURL *url = [NSURL URLWithString:strUrl];
//    NSURLRequest *request = [NSURLRequest requestWithURL:url];
//    NSURLConnection *connection = [[[NSURLConnection alloc] initWithRequest:request delegate:self] autorelease];
//    if (connection) {
//        dataUpdateInfo = [[NSMutableData alloc] init];
//    }
//    [connection start];
//}
//
- (void)checkUpdateFailed {
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"更新检测失败，请重新登录游戏！" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    [alertView setTag:2];
    [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:YES];
    [alertView release];
}
//
//#pragma mark - NSURLConnectionDataDelegate
//- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
//    if (data) {
//        [dataUpdateInfo appendData:data];
//    }
//}
//
//- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
//    NSError *error = nil;
//    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:dataUpdateInfo options:NSJSONReadingAllowFragments error:&error];
//    [dataUpdateInfo release], dataUpdateInfo = nil;
//    
//    if (dict && [dict objectForKey:@"ver"] && [dict objectForKey:@"url"]) {
//        NSString *newVersion = [NSString stringWithFormat:@"%@", [dict objectForKey:@"ver"]];
//        updateURL = [[NSString stringWithFormat:@"%@", [dict objectForKey:@"url"]] retain];
//        NSString *currentVersion = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey]];
//        if ([newVersion length] > 0 && [updateURL length] > 0 && ([currentVersion compare:newVersion options:NSNumericSearch] == NSOrderedAscending)) {
//            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"获取更新！" message:@"好消息！现在可以免费下载新版本了。" delegate:self cancelButtonTitle:@"更新" otherButtonTitles:nil, nil];
//            [alertView setTag:1];
//            [alertView performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:YES];
//            [alertView release];
//        }else {
//            if ([_delegate respondsToSelector:@selector(didFinishInitSDK)]) {
//                [_delegate didFinishInitSDK];
//            }
//        }
//    }else {
//        [self checkUpdateFailed];
//    }
//
//}
//
//- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
//    [dataUpdateInfo release], dataUpdateInfo = nil;
//    [self checkUpdateFailed];
//}


// 检查更新 new way
- (void)checkUpdate
{
    [[HmcpUpdate sharedUpdate]setLogEnable:NO];
    [[HmcpUpdate sharedUpdate]checkUpdateForTest:NO delegate:self];
}

#pragma mark  ---HmcpUpdateDelegate---
- (void)checkUpdateFinish:(BOOL)isSuccess shouldUpdate:(BOOL)update isForceUpdate:(BOOL)force
{
    NSLog(@"更新检查是否成功：%d 是否有新版本：%d 是否强更：%d",isSuccess,update,force);
    if(isSuccess == NO)
    {
        [self checkUpdateFailed];
        return;
    }
//    if(update == YES)
//    {
//
//    }
//    else
//    {
        if ([_delegate respondsToSelector:@selector(didFinishInitSDK)])
        {
            [_delegate didFinishInitSDK];
        }
//    }
}


- (void)applicationHandleOpenURL:(NSURL *)url {
    [[IPAYiAppPay sharediAppPay] handleOpenurl:url];
}

- (void)applicationOpenURL:(NSURL *)url {
    [[IPAYiAppPay sharediAppPay] handleOpenurl:url];
    
}

- (void)initWithScreenOrientation:(UIInterfaceOrientation)orientation {
    [[IPAYiAppPay sharediAppPay] initializeWithAppKey:APP_KEY andAppID:APP_ID andWaresID:1 andUIOrientation:orientation andCPDelegate:nil];
    [self checkUpdate];
}


- (void)doLogin {
    [[IPAYiAppPay sharediAppPay] showLoginViewWithIsForced:YES andLoginDelegate:self];
}


#pragma mark - UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    if (alertView.tag == 1) {
        NSLog(@"==:%@",updateURL);
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:updateURL]];
    }else if (alertView.tag == 2) {
        exit(0);
    }
}


#pragma mark - 登陆回调
- (void)loginDidSuccessWithUserName:(NSString *)userName {
    if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
        [_delegate didLoginWithUserId:nil userName:userName andToken:nil];
    }
}
- (void)loginDidFail {
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:1];
    }
}
- (void)loginDidCancel {
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorCode:)]) {
        [_delegate didLoginFailedWithErrorCode:0];
    }

}


@end
