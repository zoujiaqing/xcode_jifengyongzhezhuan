#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>

@interface StringUtil : NSObject<UIAlertViewDelegate> {
}
+ (NSString *)MD5:(NSString *)str;
+ (NSString * )encodeBase64:(NSString * )input;
+ (NSString * )decodeBase64:(NSString * )input;
- (void)NetworkDisconnect:(const char*)content;
- (void)KillProcessBySelf;
@end
