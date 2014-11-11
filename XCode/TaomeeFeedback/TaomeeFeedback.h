//
//  TaomeeFeedback.h
//  TaomeeFeedback
//
//  Created by Michael Wong on 5/4/12.
//  Copyright (c) 2011 Taomee. All rights reserved.
//
//  Required frameworks
//  1) libz.dylib
//  2) libxml2.dylib
//  3) SystemConfiguration.framework
//  4) MobileCoreServices.framework
//  5) CoreData.framework
//  6) CFNetwork.framework
//  7) UIKit.framework
//  8) Foundation.framework (Option)
//  9) CoreGraphics.framework
/***
 // 
 //  Version : V 1.0.0 init
 //  2012.05.15 MICHAEL / YANGFAN
 //  Version : V 1.0.1 Fixed crash bug under iPad
 //  2012.05.16 MICHAEL / YANGFAN 
 //  Version : V 1.0.2 release image after select big image
 //  2012.06.16 MICHAEL / YANGFAN 
 //  Version : V 1.0.3
            1) add type for message posted
            2) change normal picture to thumb in detailed issue page
            3) fixed crash bug under invalid networking situation
 //  2012.06.19 MICHAEL / YANGFAN 
 //  Version : V 1.0.4 
            1) fixed string error in bundle
            2) fixed crash bug when send image file failed
 //  2012.07.05 YANGFAN 
 //  Version : V 1.0.5 
            1) fixed another crash bug under invalid networking situation
 //  2012.07.10 MICHAEL 
 //  Version : V 1.0.6 
            1) scale large image to screen size
 //  2012.08.2 KEVIN 
 //  Version : V 1.1.0 
            1) add forum feature
 //  2012.08.29 KEVIN 
 //  Version : V 1.1.1 
            1) change FAQ title length
 //  2012.09.20 KEVIN 
 //  Version : V 1.1.2 
            1) change UI
 //  2012.10.16 KEVIN
 //  Version : V 1.1.3 
            1) optimized memory used
            2) remove asihttprequest lib
            3) fixed namespace conflict
 //  2013.2.28 KEVIN
 //  Version : V 1.1.4
            1) update "Question" icon for iPad
 //  2013.3.28 KEVIN
 //  Version : V 1.1.5
            1) remove UDID
 //  2013.5.8 KEVIN
 //  Version : V 1.1.6
            1) contact and unread feature
 //  2013.6.25 KEVIN
 //  Version : V 1.1.7
            1) fix bug: error display
            2) replace TaomeeUDID with OpenUDID
 //  2013.8.29 KEVIN
 //  Version : V 1.1.8
            1) add new forum
 //  2014.5.27 Caesarhu
 //  Version : V 1.1.9
            1) add username, channelId, serverId

 ****/
// usage :
// 1) #import "TaomeeFeedback.h"
// 2) @interface ViewController : UIViewController <TaomeeFeedbackDelegate>
// 3) - (IBAction)openFeedbackUI:(id)sender {
//    // to open feedback UI
//    AppDelegate *_AppDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
//    [TaomeeFeedback showFeedbackWithUserId:???? password:nil forumId:forumId viewControll:_AppDelegate.viewController delegate:self];
//}
// 4) - (void)onFeedbackViewClosed
//{
//    NSLog(@"onFeedbackViewClosed");
//}

#import <UIKit/UIKit.h>

// 0x00 HI ME LO
// 00   01 00 00
#define TAOMEE_FEEDBACK_VERSION 0x00010108

typedef enum _TaomeeFeedbackModule {
    TaomeeFeedbackSubmitModule,
    TaomeeFeedbackAnswerModule,
    TaomeeFeedbackFAQModule,
    TaomeeFeedbackBBSModule,
} TaomeeFeedbackModule;

@protocol TaomeeFeedbackDelegate <NSObject>

- (void)onFeedbackViewClosed;
- (void)onFeedbackInformation;

@end

@class TFTransportManager;
@interface TaomeeFeedback : NSObject
{
    int _userId;
    NSData *_password;
    NSString *_gameId;
    NSString *_forumId;
    NSString *_sessionId;
    BOOL _showForumInApp;
    int _usedFroumVersion;
    UIViewController *_viewController;
    id<TaomeeFeedbackDelegate> _delegate;

    NSMutableDictionary *_information;
}

@property (nonatomic, assign) int userId;
@property (nonatomic, retain) NSData *password;
@property (nonatomic, retain) NSString *gameId;
@property (nonatomic, retain) NSString *forumId;
@property (nonatomic, retain) NSString *sessionId;
@property (nonatomic, assign) BOOL showForumInApp;
@property (nonatomic, assign) int usedFroumVersion;
@property (nonatomic, assign) UIViewController *viewController;
@property (nonatomic, assign) id<TaomeeFeedbackDelegate> delegate;

@property (nonatomic, retain) NSMutableDictionary *information;

@property (nonatomic, retain) NSString *channelId;
@property (nonatomic, retain) NSString *serverId;
@property (nonatomic, retain) NSString *userName;

// input with user id & view controller & delegate !!!DEPRECATED!!! IT SHOULD BE NOT USED!!!
+ (void)showFeedbackWithUserId:(int)userId viewControll:(UIViewController*)viewController delegate:(id<TaomeeFeedbackDelegate>)delegate DEPRECATED_ATTRIBUTE;

// input with user id & password & view controller & delegate !!!DEPRECATED!!! IT SHOULD BE NOT USED!!!
+ (void)showFeedbackWithUserId:(int)userId password:(NSData*)password viewControll:(UIViewController*)viewController delegate:(id<TaomeeFeedbackDelegate>)delegate;

// input with user id & password & forumId & view controller & delegate
+ (void)showFeedbackWithUserId:(int)userId password:(NSData*)password forumId:(NSString *)forumId viewControll:(UIViewController*)viewController delegate:(id<TaomeeFeedbackDelegate>)delegate;

+ (id)shared;

+ (NSString *)openUDID;

- (void)showFeedbackWithModule:(TaomeeFeedbackModule)module;

- (void)fetchInformation;

@end
