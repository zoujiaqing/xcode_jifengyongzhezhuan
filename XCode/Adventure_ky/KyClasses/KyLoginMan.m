//
//  KyLoginMan.m
//  KyDemo
//
//  Created by Delle  on 14-3-13.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "KyLoginMan.h"
#import <CommonCrypto/CommonDigest.h>
#import "OrderContants.h"

@interface KyLoginMan ()<NSURLConnectionDataDelegate, NSURLConnectionDelegate> {
    BOOL islogined;
    NSMutableData *dataUserInfo;
    NSMutableData *dataUid;
    NSString *tokenKey;
    NSURLConnection *connectionUserInfo;
    NSURLConnection *connectionUid;
    NSString *username;
    NSString *guid;
    
    BOOL isLogining;
    BOOL isLogouting;
}

@end

static KyLoginMan *instance = nil;

@implementation KyLoginMan
@synthesize delegate = _delegate;

+ (KyLoginMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[KyLoginMan alloc] init];
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
        islogined = NO;
        isLogining = NO;
        isLogouting = NO;
    }
    return self;
}

- (void)dealloc
{
    [guid release], guid = nil;
    [super dealloc];
}

- (void)initWithScreenOrientation:(UIInterfaceOrientation)orientation {
    [[KYSDK instance] checkUpdate];
}

- (void)doLogin
{
    if (isLogining) {
        return;
    }else {
        isLogining = YES;
    }
    [[KYSDK instance] changeLogOption:KYLOG_OFFGAMENAME];
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    [[KYSDK instance] setSdkdelegate:self];
    [[KYSDK instance] showUserView];
}

- (void)loginWithLastUser {
    [[KYSDK instance] setSdkdelegate:self];
    [[KYSDK instance] logWithLastUser];
}

- (void)backToLog {
    [[KYSDK instance] setSdkdelegate:self];
    [[KYSDK instance] userBackToLog];
}

- (void)doLogout
{
    [[KYSDK instance] setSdkdelegate:self];
    if (isLogouting) {
        return;
    }else {
        isLogouting = YES;
    }
    [[KYSDK instance] userLogOut];
}

- (BOOL)isLogined {
    return islogined;
}

- (void)showUserCenter {
    [[KYSDK instance] setSdkdelegate:self];
    [[KYSDK instance] setUpUser];
}

#pragma mark - ky usersystem delegate
/**
 *  @method-(void)logCallBack:(NSString *)tokenKey
 *  用户登录回调
 *  @param  tokenKey
 **/
-(void)loginCallBack:(NSString *)token {
    tokenKey = token;
    [self requestUserInfo];
}

/**
 *  @method-(void)quickLogCallBack:(NSString *)tokenKey
 *  用户快速登录回调
 *  @param  tokenKey
 **/
-(void)quickLogCallBack:(NSString *)token {
    tokenKey = token;
    [self requestUserInfo];
}

/**
 *游戏账号登陆成功回调
 **/
-(void)gameLoginSuc{

}

/**
 *  @method-(void)logOutCallBack:(NSString *)guid
 *  注销方法，userLogOut该方法回调
 *  @param  guid
 **/
-(void)logOutCallBack:(NSString *)guid {
    isLogouting = NO;
    isLogining = NO;
    if ([_delegate respondsToSelector:@selector(didLogout)]) {
        islogined = NO;
        [_delegate didLogout];
    }
}


- (void)closeWindow
{
    isLogouting = NO;
    isLogining = NO;
    NSLog(@"close login windows");
}

//请求uid和username
- (void)requestUserInfo {
    NSString *sign = [[NSString stringWithFormat:@"%@%@", APP_KEY, tokenKey] lowercaseString];
    NSString *strUrl = [NSString stringWithFormat:@"http://f_signin.bppstore.com/loginCheck.php?tokenKey=%@&sign=%@", [tokenKey lowercaseString], [[self md5:sign] lowercaseString]];
    NSLog(@"url :%@", strUrl);
    NSURL *url = [NSURL URLWithString:strUrl];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    connectionUserInfo = [[[NSURLConnection alloc] initWithRequest:request delegate:self] autorelease];
    if (connectionUserInfo) {
        dataUserInfo = [[NSMutableData alloc] init];
    }
    [connectionUserInfo start];
}

#pragma mark - NSURLConnectionDataDelegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    if ([connection isEqual:connectionUserInfo]) {
        [dataUserInfo appendData:data];
    }else if ([connection isEqual:connectionUid]) {
        [dataUid appendData:data];
    }
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    if ([connection isEqual:connectionUserInfo]) {
        NSError *error = nil;
        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:dataUserInfo options:NSJSONReadingAllowFragments error:&error];
        [dataUserInfo release], dataUserInfo = nil;
        if (dict && [[dict objectForKey:@"code"] intValue] == 0) {
            guid = [[NSString alloc] initWithFormat:@"%@", [[dict objectForKey:@"data"] objectForKey:@"guid"]];
            username = [[NSString alloc] initWithFormat:@"%@", [[dict objectForKey:@"data"] objectForKey:@"username"]];
//
//            NSString *sign = [NSString stringWithFormat:@"channel_id=26&key=%@&sign=b0ae68ca0275cdc89a2614c13251a5e4", guid];
//            NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"http://account.shootao.com/accountMap/account_map.php?channel_id=26&key=%@&sign=%@", guid, [self md5:sign]]]];
//            connectionUid = [[NSURLConnection alloc] initWithRequest:request delegate:self];
//            if (connectionUid) {
//                dataUid = [[NSMutableData alloc] init];
//            }
//            [connectionUid start];
            
            //直接返回UID
            if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
                [_delegate didLoginWithUserId:guid userName:username andToken:tokenKey];
            }
        }else {
            if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorDescription:)]) {
                [_delegate didLoginFailedWithErrorDescription:@"Sever error"];
            }
        }
    }else if ([connection isEqual:connectionUid]) {
//        NSLog(@"映射后的返回：%@", [[NSString alloc] initWithData:dataUid encoding:NSUTF8StringEncoding]);
//        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:dataUid options:NSJSONReadingAllowFragments error:nil];
//        if (dict && [[dict objectForKey:@"result"] intValue] == 0) {
//            NSString *userId = [NSString stringWithFormat:@"%@", [dict objectForKey:@"value"]];
//            if ([_delegate respondsToSelector:@selector(didLoginWithUserId:userName:andToken:)]) {
//                [_delegate didLoginWithUserId:userId userName:username andToken:tokenKey];
//                [username release], username = nil;
//            }
//        }else {
//            if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorDescription:)]) {
//                [_delegate didLoginFailedWithErrorDescription:@"Sever error"];
//            }
//        }
    }
    isLogining = NO;
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    isLogining = NO;
    if ([_delegate respondsToSelector:@selector(didLoginFailedWithErrorDescription:)]) {
        [_delegate didLoginFailedWithErrorDescription:error.localizedDescription];
    }
}

- (NSString *)guid {
    return [NSString stringWithFormat:@"%@", guid];
}

//md5 32位 加密
- (NSString *)md5:(NSString *)str
{
    const char *cStr = [str UTF8String];
    unsigned char result[16];
    CC_MD5(cStr, strlen(cStr), result); // This is the md5 call
    return [NSString stringWithFormat:
            @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
}











/**
 *  @method -(void)UPPayPluginResult:(UNIPAYTYPE *)result
 *  银联支付回调函数
 *  @param  result 支付结果
 */
-(void)UPPayPluginResult:(UNIPAYTYPE)result {
    
}

/**
 *  @method - (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message
 *  中国移动支付回调
 *  @param  resultCode
 *  @param  index
 *  @param  message
 */
//- (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message{
//
//}

/**
 *  @method - (void)submitMobilePay
 *  中国移动支付需用户自行调用响应方法，SDK仅通过回调方法告知用户
 */
- (void)submitMobilePay{

}

/**
 *  @method - (void)submitMobilePay
 *  关闭支付界面"关闭"按钮的回调
 */
-(void)closePayCallback{

}



/**
 *  @method-(void)cancelUpdateCallBack
 *  游戏取消更新回调（单独使用更新时）
 **/
-(void)cancelUpdateCallBack{

}

/**
 *  @method-(void)gameLoginCallback:(NSString *)username password:(NSString *)password
 *  游戏账号登陆回调
 **/
-(void)gameLoginCallback:(NSString *)username password:(NSString *)password{

}

/**
 *-(void)callBackForgetGamePwd
 *游戏账号忘记密码回调
 **/
-(void)callBackForgetGamePwd{

}

@end
