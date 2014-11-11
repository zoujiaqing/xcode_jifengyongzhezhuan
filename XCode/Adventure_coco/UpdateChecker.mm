

#import "UpdateChecker.h"
#import "cocos2d.h"

//#define kMyAppID                @"802851368"
#define kUpdateAlertViewTitle   @"获取更新!"
#define kUpdateMessage          @"好消息!现在可以免费下载新版本了。"
#define kCancelButtonTitle      @"更新"
#define kUpdateButtonTitle      @""

@interface UpdateChecker()

@end

static std::string m_url = "";
@implementation UpdateChecker

#pragma mark - Public Methods
+ (void)checkVersion:(const char*)url
{
    m_url = url;
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:kUpdateAlertViewTitle
                                              message:kUpdateMessage
                                              delegate:self
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

#pragma mark - UIAlertViewDelegate Methods
+ (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSString *iTunesString = [NSString stringWithFormat:@"%s", m_url.c_str()];
    NSURL *iTunesURL = [NSURL URLWithString:iTunesString];
    [[UIApplication sharedApplication] openURL:iTunesURL];
    
    [self checkVersion:m_url.c_str()];
}

@end


