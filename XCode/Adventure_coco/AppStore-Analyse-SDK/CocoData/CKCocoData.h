//
//  CocoData.h
//  configSDK
//
//  Created by yinchong on 13-12-4.
//  Copyright (c) 2013年 yinchong. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 * CocoData内置的事件类型
 */
#define CKBuiltInEventIDLogin      @"coco_login"      /*登陆事件*/
#define CKBuiltInEventIDRegister   @"coco_register"   /*注册事件*/
#define CKBuiltInEventIDGuide      @"coco_guide"      /*完成用户引导事件*/
#define CKBuiltInEventIDPay        @"coco_pay"        /*完成付费事件*/
#define CKBuiltInEventIDCustom     @"coco_custom"     /*有效用户，有效的定义由开发者决定*/

/**
 * 如果你开发的是一个app，请使用CKCocoData类中的API；<br>
 * 如果你开发的是一个sdk，请使用CKCocoData4SDK类中的相关API；
 */
@interface CKCocoData : NSObject

/**
 *	@method	initWithAppId 初始化SDK
 *	@param 	appId         分配的appId
 */
+ (void) initWithAppId:(NSString *)appId;


/**
 *	@method	initWithAppId:reportPolicy:channel 初始化SDK
 *	@param 	appId         分配的appId
 *	@param 	policy        数据发送策略 可选值：BATCH,REALTIME  默认为REALTIME
 *	@param 	channel       渠道
 */
+ (void) initWithAppId:(NSString *)appId reportPolicy:(NSString *)policy channel:(NSString *)channel;


/**
 *	@method	initWithAppId:reportPolicy:channel 初始化SDK
 *	@param 	appId         分配的appId
 *	@param 	policy        数据发送策略 可选值：BATCH,REALTIME  默认为REALTIME
 *	@param 	channel       渠道
 *	@param 	channel       关闭初始化事件
 */
+ (void) initWithAppId:(NSString *)appId reportPolicy:(NSString *)policy channel:(NSString *)channel closeInitEvent:(BOOL) close;

/***********************数据跟踪相关API*************************/

/**
 *	@method	event
 *  自定义事件(只统计事件ID)
 *	@param 	eventId 	事件名称（自定义）
 */
+ (void)event:(NSString *)eventId;

/**
 *	@method	event:label:
 *  自定义事件(可用标签来区别同一个事件的不同应用场景)
 *
 *	@param 	eventId 	事件名称（自定义）
 *	@param 	eventLabel 	事件标签（自定义）
 */
+ (void)event:(NSString *)eventId label:(NSString *)eventLabel;

/**
 *	@method	event:label:parameters
 *  自定义事件(事件可带多个参数)
 *
 *	@param 	eventId 	事件名称（自定义）
 *	@param 	parameters 	事件参数 (key只支持NSString, value支持NSString和NSNumber)
 */
+ (void)event:(NSString *)eventId parameters:(NSDictionary *)parameters;

/**
 *	@method	event:label:parameters
 *  自定义事件(事件标签可带多个参数)
 *
 *	@param 	eventId 	事件名称（自定义）
 *	@param 	eventLabel 	事件标签（自定义）
 *	@param 	parameters 	事件参数 (key只支持NSString, value支持NSString和NSNumber)
 */
+ (void)event:(NSString *)eventId label:(NSString *)eventLabel parameters:(NSDictionary *)parameters;

/**
 *	@method	viewBegin
 *  开始跟踪某一页面，记录页面打开时间，建议在viewWillAppear或者viewDidAppear方法里调用
 *	@param 	viewName 	页面名称（自定义）
 */
+ (void)viewBegin:(NSString *)viewName;

/**
 *	@method	viewEnd
 *  结束某一页面的跟踪，记录页面的关闭时间，此方法与viewBegin方法结对使用，建议在viewWillDisappear或者viewDidDisappear方法里调用
 *	@param 	viewName 	页面名称，请跟viewBegin方法的页面名称保持一致
 */
+ (void)viewEnd:(NSString *)viewName;


@end
