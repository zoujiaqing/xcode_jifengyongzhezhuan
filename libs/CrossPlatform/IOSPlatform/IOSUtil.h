//
//  device.h
//  idou
//
//  Created by razer tong on 11-11-1.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef idou_device_h
#define idou_device_h

#import  <Foundation/Foundation.h>



@interface device : NSObject <UITextFieldDelegate>
{
 
}

+(void)playEffect:(const char*)name delay:(float)delay;
+(void)playByName:(NSObject*)name;

+(void)playBackgroundMusic:(const char* )name; 
+(void)playBackgroundMusicByObject:(NSObject*)name;
@end

#endif
