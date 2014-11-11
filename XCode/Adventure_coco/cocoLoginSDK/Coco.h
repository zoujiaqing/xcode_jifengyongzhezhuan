//
//  Coco.h
//  Coco iOS SDK
//
//  Created by lvyile on 11/6/13.
//  Copyright (c) 2013 CocoaChina. All rights reserved.
//

@protocol CocoCallback <NSObject>

@required
- (void)loginCallback:(NSString *)json;
- (void)bindCallback:(NSString *)json;
@end

@interface Coco : NSObject

/**
 *	@brief	show coco login UI
 *
 *	@param 	(id<CocoCallback>)cb :  delegate Objc Class instance
 *
 */
+ (void)login:(id<CocoCallback>)cb;

/**
 *	@brief	show coco bind mobile UI
 *
 *	@param 	(id<CocoCallback>)cb :  delegate Objc Class instance
 *
 */
+ (void)bind:(id<CocoCallback>)cb;

+ (void)removeCocoFromRootView;
@end
