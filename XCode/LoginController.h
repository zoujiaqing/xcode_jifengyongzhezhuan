//
//  LoginController.h
//  HelloLua
//
//  Created by Wesley Yin on 8/6/13.
//
//

#ifndef HelloLua_LoginController_h
#define HelloLua_LoginController_h

#import "TaomeeLogin.h"


@interface LoginController : NSObject<TaomeeLoginDelegate> {
    int m_gameId;
    int m_channelId;
    int m_outChannelId;
}

+(LoginController *)sharedInstance;

-(void)loginWithUserName:(const char *)userName
                password:(const char *)password;

-(void)doRegisterWithUserName:(const char *)userName
                     password:(const char *)password
              confirmPassword:(const char *)confirmPassword;

- (void)changePasswordWithUserId:(unsigned int)userId
                        userName:(const char *)userName
                     oldPassword:(const char *)oldPassword
                     newPassword:(const char *)newPassword
                 confirmPassword:(const char *)confirmPassword;

- (bool)doRecentLogin;

- (void)initValue;

- (int)getChannelId;
- (int)getOuterChannelId;

- (int)getGameId;

- (void)killProces;
- (BOOL)isConnectionAvailable;

@end

#endif
