//
//  AsInfoKit.h
//  AsSDKDemo
//
//  Created by i4.cn\STM on 14-3-13.
//  Copyright (c) 2014年 i4.cn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface AsInfoKit : NSObject
@property (nonatomic, assign) int       appId;
@property (nonatomic, retain) NSString *appKey;
@property (nonatomic, assign) BOOL      logData;
@property (nonatomic, assign) BOOL      closeRecharge;
@property (nonatomic, assign) BOOL      longComet;
@property (nonatomic, retain) NSString *closeRechargeAlertMessage;

//-SDK 1.4.1 - 新增的属性
@property (nonatomic, retain) UIViewController *rootViewController; //-NSLog-2014-06-23-STM-指向根视图
//-SDK 1.5 - 新增的属性
@property (nonatomic, assign) BOOL       isHiddenCloseButtonOnAsLoginView; //-NSLog-2014-07-07-STM-是否屏蔽登录窗口右上角的 “X” 按钮
//-SDK 1.5.2 - 新增的属性
@property (nonatomic, assign) UIInterfaceOrientationMask       orientationOfGame; //-NSLog-2014-10-14-STM-游戏的方向

+ (AsInfoKit *)sharedInstance;

- (void)alixPayResult:(NSURL *)paramURL;
- (BOOL)weChatPayResult:(NSURL *)paramURL;





@end
