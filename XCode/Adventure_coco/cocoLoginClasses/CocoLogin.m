//
//  CocoLogin.m
//  CocoSdkDemo
//
//  Created by Delle  on 14-1-23.
//
//

#import "CocoLogin.h"

#import "Coco.h"
#import "JSONKit.h"

@interface CocoLogin ()<CocoCallback>{
    NSString *_userName;
    NSString *_userId;
    NSString *_session;
    NSInteger _errorCode;
}

//
//  Login callbacks.
//
- (void)loginSuccessCallback;
- (void)loginFailedCallback;

//
//  Bind callbacks.
//
- (void)bindSuccessCallback;
- (void)bindFailedCallback;


@property (nonatomic, retain) NSString *userName;
@property (nonatomic, retain) NSString *userId;
@property (nonatomic, retain) NSString *session;
@property (nonatomic, assign) NSInteger errorCode;

@end

static CocoLogin *instance = nil;

@implementation CocoLogin
@synthesize delegate  = _delegate;
@synthesize userName  = _userName;
@synthesize userId    = _userId;
@synthesize session   = _session;
@synthesize errorCode = _errorCode;

+ (CocoLogin *)sharedInstance
{
    @synchronized(self){
        if (!instance) {
            instance = [[CocoLogin alloc] init];
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
        _userName  = [[NSString alloc] init];
        _userId    = [[NSString alloc] init];
        _session   = [[NSString alloc] init];
        _errorCode = -1;
    }
    
    return self;
}

- (void)dealloc
{
    if (_userName) {
        [_userName release], _userName = nil;
    }
    if (_userId) {
        [_userId release], _userId = nil;
    }
    if (_session) {
        [_session release], _session = nil;
    }
    
    [super dealloc];
}


- (void)doLogin
{
    [Coco login:self];
}

- (void)doBind
{
    [Coco bind:self];
}


#pragma mark - coco callbacks
- (void)loginCallback:(NSString *)json
{
    //TML_JSONDecoder *jsonDecoder = [TML_JSONDecoder decoder];
    NSDictionary *responseDict = [json objectFromJSONString];
    //NSData *data = [json dataUsingEncoding:NSUTF8StringEncoding];//:NSUTF8StringEncoding
    //NSDictionary *responseDict = json;//[NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:nil];
    NSLog(@"login callback json: %@", json);
    //NSLog(@"respones dict:%@", responseDict);
    if ([responseDict isKindOfClass:[NSDictionary class]]) {
        NSString* token = [responseDict objectForKey:@"tkn"];
        if (token != nil && token.length != 0) {
            //NSLog(@"login succ");
            if ([responseDict isKindOfClass:[NSDictionary class]]) {
                self.userName = [responseDict objectForKey:@"un"];
                self.userId   = [responseDict objectForKey:@"coco"];
                self.session  = [responseDict objectForKey:@"tkn"];
                [self performSelectorOnMainThread:@selector(loginSuccessCallback) withObject:nil waitUntilDone:NO];
                return;
            }
        }
        else
        {
            int status = 0;
            self.errorCode = status;
            [self performSelectorOnMainThread:@selector(loginFailedCallback) withObject:nil waitUntilDone:NO];
            return;
        }
    }
    
    self.errorCode = 1001;
    [self performSelectorOnMainThread:@selector(loginFailedCallback) withObject:nil waitUntilDone:NO];
}

- (void)bindCallback:(NSString *)json
{
    NSLog(@"bind callback json: %@", json);
    NSDictionary *responseDict = [json objectFromJSONString];
    if ([responseDict isKindOfClass:[NSDictionary class]]) {
        NSInteger status = [[responseDict objectForKey:@"state"] integerValue];
        if (status == 1) {
            //NSLog(@"bind succ");
            [self performSelectorOnMainThread:@selector(bindSuccessCallback) withObject:nil waitUntilDone:NO];
            return;
        }
        self.errorCode = status;
        [self performSelectorOnMainThread:@selector(bindFailedCallback) withObject:nil waitUntilDone:NO];
        return;
    }
    
    self.errorCode = 1001;
    [self performSelectorOnMainThread:@selector(bindFailedCallback) withObject:nil waitUntilDone:NO];
}

#pragma mark -- login callbacks
- (void)loginSuccessCallback
{
    if ([_delegate respondsToSelector:@selector(didLoginUserName:userId:andSession:)]){
        [_delegate didLoginUserName:_userName userId:_userId andSession:_session];
    }
}

- (void)loginFailedCallback
{
    if ([_delegate respondsToSelector:@selector(didFailedLoginWithErrorCode:)]) {
        [_delegate didFailedLoginWithErrorCode:_errorCode];
    }
}

- (void)bindSuccessCallback
{
    if ([_delegate respondsToSelector:@selector(didBind)]) {
        [_delegate didBind];
    }
}

- (void)bindFailedCallback
{
    if ([_delegate respondsToSelector:@selector(didFailedBindWithErrorCode:)]) {
        [_delegate didFailedBindWithErrorCode:_errorCode];
    }
}

@end
