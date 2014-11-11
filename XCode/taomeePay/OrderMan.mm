//
//  OrderMan.m
//  Demo91
//
//  Created by Delle  on 14-3-4.
//  Copyright (c) 2014年 Delle . All rights reserved.
//

#import "OrderMan.h"
#import "JSONKit.h"
#import "OrderContants.h"

@interface OrderMan ()
- (void)succCallback;
- (void)failedCallback;
- (void)doFetch;
- (void)parseData:(NSData *)data;
- (NSString *)generateExtraData;
- (NSString *)generateAddInfo;
@end

static NSString *const kInnerUrl = @"http://10.1.1.101/iap/index.php?";
static NSString *const kUrl = @"http://wlpay.shootao.com/iap/index.php?";

static OrderMan *instance = nil;

@implementation OrderMan
@synthesize url            = _url;
@synthesize thirdId        = _thirdId;

@synthesize productId      = _productId;
@synthesize userId         = _userId;
@synthesize channelId      = _channelId;
@synthesize serverId       = _serverId;
@synthesize userCreateTime = _userCreateTime;
@synthesize currency       = _currency;
@synthesize tradeNo        = _tradeNo;
@synthesize productName    = _productName;
@synthesize addInfo        = _addInfo;
@synthesize isInnerMode    = _isInnerMode;

+ (OrderMan *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[OrderMan alloc] init];
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
        //
        _isInnerMode = NO;
    }
    return self;
}

- (void)dealloc
{
    if (_productId) {
        [_productId release], _productId = nil;
    }
    if (_userCreateTime) {
        [_userCreateTime release], _userCreateTime = nil;
    }
    if (_userId) {
        [_userId release], _userId = nil;
    }
    if (_channelId) {
        [_channelId release], _channelId = nil;
    }
    if (_currency) {
        [_currency release], _currency = nil;
    }
    if (_request) {
        [_request release], _request = nil;
    }
    if (_tradeNo) {
        [_tradeNo release], _tradeNo = nil;
    }
    if (_addInfo) {
        [_addInfo release], _addInfo = nil;
    }
    if (_productName) {
        [_productName release], _productName = nil;
    }
    [super dealloc];
}

- (void)fetchOrder
{
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        [self doFetch];
    });
}

- (void)doFetch
{
    NSMutableString *urlStr = nil;
    
    if (_isInnerMode) {
        urlStr = [NSMutableString stringWithString:kInnerUrl];
    }else{
        urlStr = [NSMutableString stringWithString:kUrl];
    }
    [urlStr appendFormat:@"m=%@",GAME_NAME];
    [urlStr appendFormat:@"&a=%@",PAY_ACTION];
    [urlStr appendFormat:@"&channel=%@",PAY_CHANNEL];
    [urlStr appendFormat:@"&product_id=%@",_productId];
    [urlStr appendFormat:@"&userid=%@",_userId];
    [urlStr appendFormat:@"&exdata=%@", [self generateExtraData]];
    
    NSLog(@"url str:%@", urlStr);
    NSURL *url = [NSURL URLWithString:[NSString stringWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]];
    _request = [[NSMutableURLRequest alloc] initWithURL:url cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:15.0f];
    NSURLResponse *response = nil;
    NSError *error = nil;
    if (_request==nil) {
        NSLog(@"request error");
    }
    
    NSData *data = [[NSData alloc]initWithData:[NSURLConnection sendSynchronousRequest:_request
                                                                     returningResponse:&response
                                                                                 error:&error]];
    NSLog(@"order info:%s", [data bytes]);
    NSLog(@"respo9s:%@", response);
    if (!data || [data length]==0) {
        [data release];
        [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
        return;
    }
    
    [self parseData:data];
    [data autorelease];
}

- (void)parseData:(NSData *)data
{
    NSLog(@"order info:%s", [data bytes]);
    NSAssert(data!=nil, @"NIL: response data!");
    NSError *error = nil;
    id result = nil;
    
    JSONDecoder *jsonDecoder = [JSONDecoder decoder];
    
    if ([jsonDecoder respondsToSelector:@selector(objectWithData:error:)]){
        result = [jsonDecoder performSelector:@selector(objectWithData:error:) withObject:data withObject:(id)&error];
    }else{
        result = [jsonDecoder performSelector:@selector(parseJSONData:error:) withObject:data withObject:(id)&error];
    }
    
    if (error==nil && [result isKindOfClass:[NSDictionary class]]){
        int status = [[result objectForKey:@"result"] intValue];
        NSLog(@"%d",status);
        if (status == 0) {
            self.tradeNo = [result objectForKey:@"tradeNo"];
            self.amount = [result objectForKey:@"fee"];
            self.productName = [result objectForKey:@"name"];
            //海马渠道有用
            if ([result objectForKey:@"url"]) {
                self.url = [NSString stringWithFormat:@"%@", [result objectForKey:@"url"]];
            }else {
                self.url = nil;
            }
            
            if ([result objectForKey:@"thirdId"]) {
                self.thirdId = [NSString stringWithFormat:@"%@", [result objectForKey:@"thirdId"]];
            }else {
                self.thirdId = nil;
            }
            //=========
            [self performSelectorOnMainThread:@selector(succCallback) withObject:nil waitUntilDone:NO];
        }else{
            [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
        }
    
    }else{
        [self performSelectorOnMainThread:@selector(failedCallback) withObject:nil waitUntilDone:NO];
    }

}

- (NSString *)generateExtraData
{
    NSString *str = @"{";
    NSMutableString *jsonStr = [NSMutableString stringWithString:str];
    [jsonStr appendFormat:@"\"pd\":\"%@\",",_productId];
    [jsonStr appendFormat:@"\"sd\":%@,", _serverId];
    [jsonStr appendFormat:@"\"gd\":%@,", GAME_ID];
    [jsonStr appendFormat:@"\"ud\":%@,",_userId];
    [jsonStr appendFormat:@"\"ut\":%@,", _userCreateTime];
    [jsonStr appendFormat:@"\"cu\":%@,",_currency];
    [jsonStr appendFormat:@"\"cd\":%@", _channelId];
    [jsonStr appendFormat:@"%@", @"}"];
    
    return jsonStr;
}

- (NSString *)generateAddInfo
{
    NSString *str = @"{";
    NSMutableString *jsonStr = [NSMutableString stringWithString:str];
    [jsonStr appendFormat:@"\"pd\":\"%@\",",_productId];
    [jsonStr appendFormat:@"\"sd\":%@,", _serverId];
    [jsonStr appendFormat:@"\"gd\":%@,", GAME_ID];
    [jsonStr appendFormat:@"\"ud\":%@,",_userId];
    [jsonStr appendFormat:@"\"ut\":%@,", _userCreateTime];
    [jsonStr appendFormat:@"\"cu\":%@,",_currency];
    [jsonStr appendFormat:@"\"cd\":%@", _channelId];
    [jsonStr appendFormat:@"\"od\":\"%@\"", _tradeNo];
    [jsonStr appendFormat:@"\"m\":%@",_amount];
    [jsonStr appendFormat:@"%@", @"}"];
    
    return jsonStr;
}

- (void)succCallback
{
    NSLog(@"succ callback.");
    self.addInfo = [self generateAddInfo];
    
    if (self.thirdId && [self.thirdId length] > 0) {
        if ([_delegate respondsToSelector:@selector(didFectchOrderWithProductId:amount:tradeNo:productName:callbackUrl:thirdId:andAddInfo:)]) {
            [_delegate didFectchOrderWithProductId:_productId
                                            amount:_amount
                                           tradeNo:self.tradeNo
                                       productName:self.productName
                                       callbackUrl:self.url
                                           thirdId:self.thirdId
                                        andAddInfo:[_addInfo stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
        }
    }else if ([_delegate respondsToSelector:@selector(didFectchOrderWithProductId:amount:tradeNo:productName:andAddInfo:)]) {
        [_delegate didFectchOrderWithProductId:_productId
                                        amount:_amount
                                       tradeNo:self.tradeNo
                                   productName:self.productName
                                    andAddInfo:[_addInfo stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    }
}

- (void)failedCallback
{
    NSLog(@"failed callback.");
    if ([_delegate respondsToSelector:@selector(didFailedFetchOrder)]) {
        [_delegate didFailedFetchOrder];
    }
}

@end
