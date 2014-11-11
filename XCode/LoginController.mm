//
//  LoginController.mm
//  HelloLua
//
//  Created by Wesley Yin on 8/6/13.
//
//

#include "LoginController.h"
#include "TaomeeLogin.h"
#include "LoginManager.h"
#import <SystemConfiguration/SystemConfiguration.h>
#include "NetWorkConfigInfoFromServer.h"

enum ChannelId
{
    
};

static LoginController* s_shardLoginController;

@implementation LoginController

+(LoginController *)sharedInstance
{
    if (s_shardLoginController == nil)
    {
        s_shardLoginController = [LoginController new];
        [s_shardLoginController initValue];
    }
    return s_shardLoginController;
}

- (void)initValue
{
    // for all mainland agency, other oversea agency the gameid is difference
    m_gameId = 82;
    
#ifdef TargetForNw
    m_channelId = 93;
#endif
    
#ifdef TargetForNd91
    m_channelId = 24;
    m_outChannelId = 500001;
#endif
    
#ifdef TargetForPP
    m_channelId = 25;
    m_outChannelId = 500003;
#endif
    
#ifdef TargetForKy
    m_channelId = 26;
    m_outChannelId = 500015;
#endif
    
#ifdef TargetForTB
    m_channelId = 27;
    m_outChannelId = 500002;
#endif
    
#ifdef TargetForCoCoAppStore
    m_channelId = 32;
    m_outChannelId = 500026;
#endif
    
#ifdef TargetForItools
    m_channelId = 38;
    m_outChannelId = 500004;
#endif
    
#ifdef TargetForHm
    m_channelId = 60;
    m_outChannelId = 500017;
#endif
    
#ifdef TargetForI4
    m_channelId = 61;
    m_outChannelId = 500020;
#endif
    
#ifdef TargetForXy
    m_channelId = 64;
    m_outChannelId = 500030;
#endif
    
    NSString *gameStr = [[[NSString alloc] initWithFormat:@"%d",m_gameId] autorelease];
     NSString *channelIdStr = [[[NSString alloc] initWithFormat:@"%d",m_channelId] autorelease];
    [TaomeeLogin initGameId:gameStr andChannel:channelIdStr];
    [TaomeeLogin isInnerMode:NO];
}

- (bool)doRecentLogin
{
    bool hasLoginInfo = false;
    NSDictionary* infoDic = [TaomeeLogin  getRecentLoginInfo];
    NSArray *recordArr = [TaomeeLogin getAllLoginInfo];
    NSLog(@"record is:%@", recordArr);
    if (infoDic != nil)
    {
        hasLoginInfo = true;
        NSString *userName = [infoDic objectForKey:@"userName"];
        NSString *password = [infoDic objectForKey:@"password"];
        [[TaomeeLogin sharedInstance] loginWithUserName:userName password:password isLocalRecord:YES andDelegate:self];
    }
    return hasLoginInfo;
}

-(void)loginWithUserName:(const char *) userName
                password:(const char *)password
{
    NSString *nameStr = [[[NSString alloc] initWithUTF8String:userName] autorelease];
    NSString *passwordStr = [[[NSString alloc] initWithUTF8String:password] autorelease];
    [[TaomeeLogin sharedInstance] loginWithUserName:nameStr password:passwordStr isLocalRecord:NO andDelegate:self];
}

-(void)doRegisterWithUserName:(const char *)userName
                     password:(const char *)password
              confirmPassword:(const char *)confirmPassword
{
    NSString *nameStr = [[[NSString alloc] initWithUTF8String:userName] autorelease];
    NSString *passwordStr = [[[NSString alloc] initWithUTF8String:password] autorelease];
    NSString *confirmStr = [[[NSString alloc] initWithUTF8String:confirmPassword] autorelease];
    [[TaomeeLogin sharedInstance] doRegisterWithUserName:nameStr password:passwordStr confirmPassword:confirmStr andDelegate:self];
}

- (void)changePasswordWithUserId:(unsigned int)userId
                        userName:(const char *)userName
                     oldPassword:(const char *)oldPassword
                     newPassword:(const char *)newPassword
                 confirmPassword:(const char *)confirmPassword
{
    NSString *idStr = [[[NSString alloc] initWithFormat:@"%d",userId] autorelease];
    NSString *nameStr = [[[NSString alloc] initWithUTF8String:userName] autorelease];
    NSString *oldPassStr = [[[NSString alloc] initWithUTF8String:oldPassword] autorelease];
    NSString *newPassStr = [[[NSString alloc] initWithUTF8String:newPassword] autorelease];
    NSString *confirmStr = [[[NSString alloc] initWithUTF8String:confirmPassword] autorelease];
    [[TaomeeLogin sharedInstance] changePasswordWithUserId:idStr userName:nameStr oldPassword:oldPassStr newPassword:newPassStr confirmPassword:confirmStr andDelegate:self];
}

// delegate
- (void)didRegistWithUserId:(NSString *)userId andUserName:(NSString *)userName
{
    LoginManager::Get()->onRegisterSuccess(userId.intValue,[userName UTF8String]);
}

- (void)registFailed:(NSInteger)errorCode
{
    LoginManager::Get()->onRegisterFailed(errorCode);
}

- (void)loginSuccWithUserId:(NSString *)userId
                   userName:(NSString *)userName
                 andSession:(NSString *)session
{
    LoginManager::Get()->onLoginSuccess(userId.intValue, [session UTF8String], [userName UTF8String]);
}

- (void)loginFailed:(NSInteger)errorCode
{
    LoginManager::Get()->onLoginFailed(errorCode);
}

- (void)changedPasswordSucc
{
    LoginManager::Get()->onChangePasswordSuccess();
}

- (void)changedPasswordFailed:(NSInteger)errorCode
{
    LoginManager::Get()->onChangePasswordFailed(errorCode);
}

-(int)getChannelId
{
    return m_channelId;
}
-(int)getOuterChannelId
{
    return m_outChannelId;
}

-(int)getGameId
{
    return m_gameId;
}

- (void)killProces
{
    CCDirector::sharedDirector()->end();
    exit(0);
}

- (BOOL)isConnectionAvailable
{
    SCNetworkReachabilityFlags flags;
    BOOL receivedFlags;
    
    const char* url = NetWorkConfigInfoFromServer::sharedInstance()->GetServerListURL();
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(CFAllocatorGetDefault(), url);
    receivedFlags = SCNetworkReachabilityGetFlags(reachability, &flags);
    CFRelease(reachability);
    
    if (!receivedFlags || (flags == 0) )
    {
        return FALSE;
    } else {
        return TRUE;
    }
}

@end