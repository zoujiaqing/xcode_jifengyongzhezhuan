//
//  CocoLogin.h
//  CocoSdkDemo
//
//  Created by Delle  on 14-1-23.
//
//

#import <Foundation/Foundation.h>
@protocol CocoLoginDelegate <NSObject>

@optional

- (void)didLoginUserName:(NSString *)userName
                  userId:(NSString *)userId
              andSession:(NSString *)session;
- (void)didFailedLoginWithErrorCode:(NSInteger)errorCode;

- (void)didBind;
- (void)didFailedBindWithErrorCode:(NSInteger)errorCode;

@end


@interface CocoLogin : NSObject {
    id<CocoLoginDelegate> _delegate;
}

@property (nonatomic, assign) id<CocoLoginDelegate> delegate;

+ (CocoLogin *)sharedInstance;
+ (void)purgeSharedInstance;

/**
 *  Coco账号登陆，调用前设置delegate
 */
- (void)doLogin;
/**
 *  绑定手机号，调用前设置delegate
 */
- (void)doBind;

@end
