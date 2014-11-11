#import "StringUtil.h"
#import "GTMBase64.h"
#import "LoginController.h"

static int disconnet_count = 0;
@implementation StringUtil
+ (NSString *)MD5:(NSString *)str {
    const char *cStr = [str UTF8String];
    unsigned char result[16];
    CC_MD5( cStr, strlen(cStr), result ); // This is the md5 call
    return [NSString stringWithFormat:
            @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
}

+ (NSString * )encodeBase64:(NSString * )input
{
    NSData * data = [input dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
    // 转换到base64
    data = [GTMBase64 encodeData:data];
    NSString * base64String = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return base64String;
}

+ (NSString * )decodeBase64:(NSString * )input
{
    NSData * data = [input dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
    // 转换到base64
    data = [GTMBase64 decodeData:data];
    NSString * base64String = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return base64String;
}

- (void)NetworkDisconnect:(const char*)content
{
    if(disconnet_count >= 1)
    {
        return;
    }
    disconnet_count++;
    NSString* tempStr = [[NSString alloc] initWithUTF8String:content];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:tempStr delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
   // NcSDKManager::KillProces();
    [[LoginController sharedInstance] killProces];
}

- (void)KillProcessBySelf
{
    [[UIApplication sharedApplication] performSelector:@selector(terminates)];
    exit(0);
}


@end