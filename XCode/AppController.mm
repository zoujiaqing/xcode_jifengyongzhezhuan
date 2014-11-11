/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#import <Parse/Parse.h>
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "TaomeeLogin.h"
#import "UserData.h"
#import "XLogicManager.h"
#import "LuaTinkerManager.h"
#import "NcSDKManager.h"
#include "LoginController.h"

#import "DirectionMPMoviePlayerViewController.h"

#ifdef TargetForKy
#import "KyLoginMan.h"
#import "KyPayMan.h"
#import "KYSDK.h"
#import <AlipaySDK/AlipaySDK.h>
@interface AppController()<LoginManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForCoCoAppStore
#import "CocoLogin.h"
#import "RageIAPHelper.h"
@interface AppController()<CocoLoginDelegate>
@end
#endif

#ifdef TargetForPP
#import "PPLoginMan.h"
#import "PPPayMan.h"
@interface AppController()<LoginManDelegate>
@end
#endif

#ifdef TargetForNd91
#import "NdLoginMan.h"
#import "OrderMan.h"
#import "NdPayMan.h"
@interface AppController()<LoginManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForItools
#import "HXPaymentMan.h"
#import "HXLoginMan.h"
@interface AppController()<LoginManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForTB
#import "TbLoginMan.h"
#import "TbPayMan.h"
@interface AppController()<LoginManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForI4
#import "I4LoginMan.h"
#import "I4PayMan.h"
@interface AppController()<LoinManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForHm
#import "CocoLogin.h"
#import "HaimaLoginMan.h"
#import "HaimaPayMan.h"
@interface AppController()<CocoLoginDelegate, LoginManDelegate, PayManDelegate>
@end
#endif

#ifdef TargetForXy
#import "XYPayMan.h"
#import "XYLoginMain.h"
#import <XYPlatform/XYPlatform.h>
#import "taomeePay/OrderContants.h"
@interface AppController()<LoginManDelegate, PayManDelegate>
@end
#endif

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

@synthesize window;
static RootViewController * viewController= nil;

static bool m_supportPortrait = false;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
    [NSThread sleepForTimeInterval:2.5];
    
#ifdef TargetForXy
    [[XYPlatform defaultPlatform] initializeWithAppId:APP_ID appKey:APP_KEY isContinueWhenCheckUpdateFailed:NO];
#endif

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0 ];
    
    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    
    viewController.view = __glView;
    [__glView setMultipleTouchEnabled: YES];

#ifdef __IPHONE_4_0
  
    CGSize newSize = [__glView bounds].size;
    newSize.width  = roundf(newSize.width);
    newSize.height = roundf(newSize.height);
    
    if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)]
        )
    {
        CGFloat scaleFactor = [UIScreen mainScreen].scale;
        if (((newSize.width == 768) && (newSize.height == 1024) && (scaleFactor == 2))
            || ((newSize.width == 1024) && (newSize.height == 768) && (scaleFactor == 2)) )
        {
            [__glView setContentScaleFactor: 1];
        }
    }
#endif
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
       
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
     window.rootViewController = viewController;
    [window makeKeyAndVisible];
    [[UIApplication sharedApplication] setIdleTimerDisabled: YES];
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
    
    // TaomeeLogin
    //[TaomeeLogin initGameChannel:93 andGameId:81];
    //[[TaomeeLogin sharedInstance] loginWithRootViewController:viewController andDelegate:self];
    
    // for flurry cocodata hailstone statistic
    int channelidx = [[LoginController sharedInstance] getOuterChannelId];
    NcSDKManager::enterFlurry(channelidx);
    NcSDKManager::enterCocoData(channelidx);
    NcSDKManager::enterHailStone(channelidx);
    NcSDKManager::enterInmovi();
    NcSDKManager::enterAppCpa(channelidx);
    //NcSDKManager::enterCocoAnalyse(channelidx);
    
#ifdef TargetForKy
    [KyLoginMan sharedInstance].delegate = self;
    [KyPayMan sharedInstance].delegate = self;
#endif
    
#ifdef TargetForCoCoAppStore
    [CocoLogin sharedInstance].delegate = self;
    //[RageIAPHelper sharedInstance];
    //[[RageIAPHelper sharedInstance] requestProducts];
#endif
    
#ifdef TargetForPP
    [PPLoginMan sharedInstance].delegate = self;
    [[PPLoginMan sharedInstance] initWithScreenOrientation:UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationLandscapeRight];
    NcSDKManager::SetFloatButtonHidden(true);
    return YES;
#endif
    
#ifdef TargetForNd91
    [NdLoginMan sharedInstance].delegate = self;
    [NdPayMan sharedInstance].delegate = self;
#endif
    
#ifdef TargetForItools
    [HXLoginMan sharedInstance].delegate = self;
    [HXPaymentMan sharedInstance].delegate = self;
#endif
    
#ifdef TargetForTB
    [TbLoginMan sharedInstance].delegate = self;
    [TbPayMan sharedInstance].delegate = self;
    [[TbLoginMan sharedInstance] initWithScreenOrientation:UIInterfaceOrientationLandscapeLeft];
    NcSDKManager::SetFloatButtonHidden(true);
    return YES;
#endif
    
#ifdef TargetForI4
    [I4LoginMan sharedInstance].delegate = self;
    [I4PayMan sharedInstance].delegate = self;
    [I4PayMan sharedInstance].loginDelegate = self;
    [[I4LoginMan sharedInstance] initSDK];
    return YES;
#endif
    
#ifdef TargetForHm
    [CocoLogin sharedInstance].delegate = self;
    [HaimaLoginMan sharedInstance].delegate = self;
    [HaimaPayMan sharedInstance].delegate = self;
    [[HaimaLoginMan sharedInstance] initWithScreenOrientation:UIInterfaceOrientationLandscapeLeft];
    return YES;
#endif
    
#ifdef TargetForXy
    [XYLoginMain sharedInstance].delegate = self;
    [XYPayMan sharedInstance].delegate = self;
    [[XYLoginMain sharedInstance] initWithScreenOrientation:UIInterfaceOrientationLandscapeLeft];
    NcSDKManager::SetFloatButtonHidden(true);
    //return YES;
#endif
  
//#ifdef TargetForKy
//    // shootao logo
//    UIImageView *splashScreen = [[[UIImageView alloc] initWithFrame:self.window.bounds] autorelease];
//    if([[UIApplication sharedApplication] statusBarOrientation] == UIDeviceOrientationLandscapeRight)
//    {
//        splashScreen.image = [UIImage imageNamed:@"shootaologo_right"];
//    }
//    else
//    {
//        splashScreen.image = [UIImage imageNamed:@"shootaologo"];
//    }
//    [self.window addSubview:splashScreen];
//    [UIView animateWithDuration:2.0 animations:^{
//        CATransform3D transform = CATransform3DMakeScale(1.0, 1.0, 1.0);
//        splashScreen.layer.transform = transform;
//        splashScreen.alpha = 0.9;
//    } completion:^(BOOL finished) {
//        [splashScreen removeFromSuperview];
//    }];
//    [self performSelector:@selector(showUserInfoWithoutDelay) withObject:nil afterDelay:3.0f];
//    return YES;
//#endif
    
    // player the coco mp4
    [self performSelector:@selector(showUserInfoWithoutDelay) withObject:nil afterDelay:1.0f];
    return YES;
}

+ (RootViewController *)rootController{
    return viewController;
}

//=================

-(void)showUserInfoWithoutDelay
{
    [self playMovie:@"logo"];
}

/**
 @method 播放电影
 */
-(void)playMovie:(NSString *)fileName{
    //视频文件路径
    NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:@"mp4"];
    //视频URL
    NSURL *url = [NSURL fileURLWithPath:path];
    //视频播放对象
    DirectionMPMoviePlayerViewController *movie = [[DirectionMPMoviePlayerViewController alloc] initWithContentURL:url];
    movie.moviePlayer.controlStyle = MPMovieControlStyleNone;
    movie.moviePlayer.fullscreen = true;
    movie.moviePlayer.scalingMode = MPMovieScalingModeAspectFill;
    [movie.view setFrame:window.bounds];
    movie.moviePlayer.initialPlaybackTime = -1;
    // 注册一个播放结束的通知
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(myMovieFinishedCallback:)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:movie.moviePlayer];
    [movie.moviePlayer play];
    [viewController presentMoviePlayerViewControllerAnimated:movie];
    [movie release];
}

/*
 @method 当视频播放完毕释放对象
 */
-(void)myMovieFinishedCallback:(NSNotification*)notify
{
    //视频播放对象
    MPMoviePlayerController* theMovie = [notify object];
    //销毁播放通知
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:theMovie];
    // 释放视频对象
    [viewController dismissMoviePlayerViewControllerAnimated];
    
    // delay enter the game
    [self performSelector:@selector(showUserInfoWithoutDelay2) withObject:nil afterDelay:1.0f];
}

-(void)showUserInfoWithoutDelay2
{
    m_supportPortrait = true;
    
#ifdef TargetForNd91
    [[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIInterfaceOrientationMaskLandscape];
    [[NdComPlatform defaultPlatform] NdSetAutoRotation:YES];
#endif
    
#ifdef TargetForTB
    m_supportPortrait = false;
#endif
    
#ifdef TargetForHm
    m_supportPortrait = false;
#endif
    
#ifdef TargetForCoCoAppStore
    [Parse setApplicationId:@"kqka2l7G6GOzOPG6t7Ns0Ry6tbJdZauueOcOQwef" clientKey:@"ihLHV3Z70UPCuwEeKHVqbi4UEqTaD57BTrgGkKKU"];
#else
    // add ios push
    [Parse setApplicationId:@"y3jHYCJOdBX0aBWV6z1PQ0WkjDSHtAGkpFiPIrkC" clientKey:@"k2XPMF6ONfbcg7tO7yPgQHgbk8s9MoptMmaRL9cI"];
#endif
    
    // Register for Push Notitications, if running iOS 8
    if ([[UIApplication sharedApplication ]respondsToSelector:@selector(registerUserNotificationSettings:)]) {
        UIUserNotificationType userNotificationTypes = (UIUserNotificationTypeAlert |
                                                        UIUserNotificationTypeBadge |
                                                        UIUserNotificationTypeSound);
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:userNotificationTypes
                                                                                 categories:nil];
        [[UIApplication sharedApplication]registerUserNotificationSettings:settings];
        [[UIApplication sharedApplication]registerForRemoteNotifications];
    } else {
        // Register for Push Notifications before iOS 8
        [[UIApplication sharedApplication]registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |                                                                              UIRemoteNotificationTypeAlert |                                                                              UIRemoteNotificationTypeSound)];
    }
    // clear badges
    PFInstallation* currentInstallation = [PFInstallation currentInstallation];
    if(currentInstallation.badge != 0)
    {
        currentInstallation.badge = 0;
        [currentInstallation saveEventually];
    }

    
    // go to game
    cocos2d::CCApplication::sharedApplication()->run();
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    // Store the deviceToken in the current installation and save it to Parse.
    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    [currentInstallation setDeviceTokenFromData:deviceToken];
    [currentInstallation saveInBackground];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    [PFPush handlePush:userInfo];
}

//=================

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
#ifdef TargetForNd91
    [[NdLoginMan sharedInstance] willEnterForeground];
#endif
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [super dealloc];
}

//#####################################################################################

- (void)IndeedLoginSuccessWithUid:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token
{
    UserData::Create();
    USERINFO mUserData = UserData::GetUserInfo();
    
    int uid = [userId intValue];
    int channelx = [[LoginController sharedInstance] getChannelId];
    mUserData.id.setPlayId(uid, 0, channelx);
    const char * localsession = [token UTF8String];
    strcpy(mUserData.userSession,localsession);
    
    UserData::SetUserInfo(mUserData);
    XLogicManager::sharedManager()->connectToServer();
    if(userName != nil)
    {
        XLogicManager::sharedManager()->accountName = [userName UTF8String];
    }
}
- (void)LogoutAlert
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"login" message:@"failed" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}
//#####################################################################################

#pragma mark - ky delegate methods

#ifdef TargetForKy
- (void)didLoginFailedWithErrorDescription:(NSString *)des
{
    [self LogoutAlert];
}

- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token
{
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLogout {
    NSLog(@"已注销");
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->startXXX();
        return;
    }
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"退出成功" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
    
    XLogicManager::sharedManager()->isChangeScene = true;
}

-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
    }
    NcSDKManager::doLogin();
}

- (void)didPaid {
    NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des {
    NSLog(@"支付失败：%@", des);
}
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    if ([url.host isEqualToString:@"safepay"]) {
        [[AlipaySDK defaultService] processOderWithPaymentResult:url];
    }
    return YES;
}
-(void)alipayCallBack:(ALIPAYRESULT)alipayresult
{
}
#endif

#ifdef TargetForCoCoAppStore
- (void)didLoginUserName:(NSString *)userName userId:(NSString *)userId andSession:(NSString *)session
{
    NSLog(@"userName:%@, userId:%@, session:%@", userName, userId, session);
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:session];
}

- (void)didFailedLoginWithErrorCode:(NSInteger)errorCode
{
    NSLog(@"Lgoin error:%d", errorCode);
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didBind
{
    NSLog(@"Bind success!");
}

- (void)didFailedBindWithErrorCode:(NSInteger)errorCode
{
    NSLog(@"Bind error:%d", errorCode);
}
#endif

#ifdef TargetForPP
- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token {
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLoginFailedWithErrorCode:(int)errorCode
{
    NSLog(@"Lgoin error:%d", errorCode);
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
    }
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didLogout {
    NSLog(@"已注销");
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->startXXX();
    }
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    [[PPAppPlatformKit sharedInstance] alixPayResult:url];
    return YES;
}
#endif

//////////////// 91
#ifdef TargetForNd91
- (void)didLoginFailedWithErrorCode:(int)errorCode
{
    NSLog(@"Lgoin error:%d", errorCode);
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didLeavePlatform {
    NSLog(@"离开平台");
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
    }
    
    if([[NdLoginMan sharedInstance] isLogined] == false)
    {
        NcSDKManager::doLogin();
    }
}

- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token
{
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLogout {
    NSLog(@"已注销");
    XLogicManager::sharedManager()->isIOSInScene = true;
    XLogicManager::sharedManager()->startXXX();
}

- (void)didPaid
{
     NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des
{
    NSLog(@"支付失败：%@", des);
}
#endif

//////////////// itools
#ifdef TargetForItools
- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token {
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLoginFailedWithErrorDescription:(NSString *)des {
    [self LogoutAlert];
}

- (void)didLogout {
    NSLog(@"已注销");
    if(XLogicManager::sharedManager()->isAlreadyLogin() == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = true;
        XLogicManager::sharedManager()->startXXX();
    }
    NcSDKManager::doLogin();
}

- (void)didPaid {
    NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des {
    NSLog(@"支付失败：%@", des);
}
#endif

////////////////TB
#ifdef TargetForTB
- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token
{
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLoginFailedWithErrorCode:(int)errorCode
{
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didPaid
{
    NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des
{
    NSLog(@"支付失败: %@", des);
}

- (void)didLogout
{
    NSLog(@"已注销.");
    XLogicManager::sharedManager()->isIOSInScene = true;
    XLogicManager::sharedManager()->startXXX();
}

- (void)didLeavePlatform {
    NSLog(@"离开平台");
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
    }
    
    if([[TbLoginMan sharedInstance] isLogined] == false)
    {
        NcSDKManager::doLogin();
    }
}
#endif

/////////////////////////////////I4
#ifdef TargetForI4
#pragma mark - 登录回调
- (void)didLoginFailedWithErrorCode:(int)errorCode {
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
    }
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token {
   [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
}

- (void)didLogout {
    NSLog(@"已注销");
    if(XLogicManager::sharedManager()->isAlreadyLogin() == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = true;
        XLogicManager::sharedManager()->startXXX();
    }
    NcSDKManager::doLogout();
}

#pragma mark - 支付回调
- (void)didPaid {
    NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des {
    NSLog(@"支付失败:%@", des);
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    [[I4LoginMan sharedInstance] applicationOpenURL:url SourceApplication:sourceApplication];
    return YES;
}
#endif

#ifdef TargetForHm
- (void)didLoginUserName:(NSString *)userName userId:(NSString *)userId andSession:(NSString *)session
{
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:session];
}

- (void)didFailedLoginWithErrorCode:(NSInteger)errorCode
{
    if(errorCode == 0)
    {
        NcSDKManager::doLogin();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didBind
{
    NSLog(@"Bind success!");
}

- (void)didFailedBindWithErrorCode:(NSInteger)errorCode
{
    NSLog(@"Bind error:%d", errorCode);
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    [[HaimaLoginMan sharedInstance] applicationHandleOpenURL:url];
    return YES;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    [[HaimaLoginMan sharedInstance] applicationOpenURL:url];
    return YES;
}
#endif

#ifdef TargetForXy
- (void)didLoginWithUserId:(NSString *)userId userName:(NSString *)userName andToken:(NSString *)token {
    [self IndeedLoginSuccessWithUid:userId userName:userName andToken:token];
    NcSDKManager::SetFloatButtonHidden(true);
}

- (void)didLoginFailedWithErrorCode:(int)errorCode
{
    if(XLogicManager::sharedManager()->isAlreadyLogin() == true)
    {
        if(XLogicManager::sharedManager()->isIOSInScene == true)
        {
            XLogicManager::sharedManager()->isIOSInScene = false;
        }
        if([[XYLoginMain sharedInstance] isLogined] == false)
        {
            XLogicManager::sharedManager()->setisAlreadyLogin(false);
            NcSDKManager::doLogout();
        }
        return;
    }
    
    if(errorCode == 0)
    {
        NcSDKManager::doLogout();
    }
    else
    {
        [self LogoutAlert];
    }
}

- (void)didPaid
{
    NSLog(@"支付成功");
}

- (void)didPaidFailedWithErrorDescription:(NSString *)des
{
    NSLog(@"支付失败: %@", des);
}

- (void)didLogout
{
    if(XLogicManager::sharedManager()->isAlreadyLogin() == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = true;
        XLogicManager::sharedManager()->startXXX();
    }
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    [[XYPlatform defaultPlatform] XYHandleOpenURL:url];
    return YES;
}
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    [[XYPlatform defaultPlatform] XYHandleOpenURL:url];
    return YES;
}
#endif

- (void)didFinishInitSDK
{
    NcSDKManager::SetFloatButtonHidden(true);
    [self showUserInfoWithoutDelay];
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    if(m_supportPortrait == true)
    {
        return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
    }

}

@end

