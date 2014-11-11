//
//  UpdateChecker.h
//  AnimationPro
//
//  Created by Yusheng on 13-9-11.
//
//

#import <Foundation/Foundation.h>


@interface UpdateChecker : NSObject <UIAlertViewDelegate>

+ (void)checkVersion:(const char*)url;

@end
