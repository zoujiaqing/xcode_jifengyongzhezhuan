//
//  KYSDK.h
//  KYSDK
//
//  Created by MTang0589 on 14-2-24.
//  Copyright (c) 2014年 MTang0589. All rights reserved.
// sdk版本-----1.7.1

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

enum
{
    PAY_ERROR,  //支付宝回调支付出错，或者退出
    PAY_DONE    //支付宝回调支付完成，支付结果是否正确请以服务器端通告为准
}typedef ALIPAYRESULT;

enum{
    USER_UNIPAY_SUCCESS,   //用户银联支付成功
    USER_UNIPAY_FAIL,      //用户银联支付失败
    USER_UNIPAY_CANCEL,    //用户取消银联支付
    USER_UNIPAY_ERROR      //银联没有返回结果
}typedef UNIPAYTYPE;

enum{
    KYLOG_ONGAMENAME,   //显示游戏账号登录
    KYLOG_OFFGAMENAME   //不显示游戏账号登录
}typedef KYLOGOPTION;
@protocol KYSDKDelegate <NSObject>
@optional

/*------ 更新相关的回调 --------*/

//游戏取消更新回调（单独调用更新方法checkUpdate时的回调，在登录界面的更新提示，点击“取消”无回调。无论在哪个界面，点击“更新”按钮均无回调）
-(void)cancelUpdateCallBack;





/*-------- 登录 “7659账号”登录的回调 ---------*/
//设置是否只支持iphone，返回NO，则支持iPad
-(void)setOnlySupportIPhone:(BOOL)isOnlySupportIPhone;

//1.用户登录界面点击“登录”按钮的回调
-(void)loginCallBack:(NSString *)tokenKey;

//2.用户登录界面点击“快速试玩”按钮的回调
-(void)quickLogCallBack:(NSString *)tokenKey;

//3.注销方法的回调
//场景1：主动调用“注销方法”（userLogOut）
//场景2：点击“7659账号管理”界面“注销”按钮的回调。两者的回调无区分
-(void)logOutCallBack:(NSString *)guid;






/*--------- 支付的回调 ----------*/
//一切支付结果，以服务器端的通告为准
//1.支付宝完成后回调
-(void)alipayCallBack:(ALIPAYRESULT)alipayresult;

//2.银联支付回调函数
-(void)UPPayPluginResult:(UNIPAYTYPE)result;



/*!!!!!!!新接入游戏、已上架且仅使用7659账号的开发商忽略此步骤!!!!!!*/
/*-------- 登录 “游戏账号”登录的回调--------*/

//1.获取玩家登录账号和登录密码的回调
-(void)gameLoginCallback:(NSString *)username password:(NSString *)password;

//2.“游戏账号”登陆成功回调,登录成功的标志是：[[KYSDK instance]showStateGame:@"suc”];
-(void)gameLoginSuc;

//3.游戏账号忘记密码回调
-(void)callBackForgetGamePwd;

//4.游戏账号发送验证码回调
-(void)logSendSMSCallback:(NSString *)phoneNO;





/*-----移动游戏基地的接入为选接，需与移动游戏基地签订协议后，进行自主对接。快用技术支持暂不支持该服务。------*/

//#import "KYMobilePay.h"

//enum{
//    SERVICE_CHECK = -1,     //等待服务器验证
//    PAY_SUCCESS = 0,        //结果正确
//    PAY_FAILE = 1,          //结果错误
//    PAY_ERROR = 2          //验证失败
//}typedef CHECK;

/**
 *  @method - (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message
 *  中国移动支付回调
 *  @param  resultCode
 *  @param  index
 *  @param  message
 */
//- (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message;

/**
 *  @method - (void)submitMobilePay
 *  中国移动支付需用户自行调用响应方法，SDK仅通过回调方法告知用户
 */
//- (void)submitMobilePay;
@end


@interface KYSDK : NSObject
@property(assign, nonatomic)id<KYSDKDelegate> sdkdelegate;



/*------- 初始化相关的设置 ------------*/
//1.初始化SDK
+(id)instance;

//2.设置代理
+(id)setSdkdelegate:(NSObject *)str;

//3.控制登陆界面是否含有游戏账号的选项（新游戏没有老用户时，可将游戏账号关掉,
//[[KYSDK instance] changeLogOption:KYLOG_OFFGAMENAME];不显示游戏账号登录）
//场景1：在登录方法（showUserView）之前调用
//场景2：在弹出个人中心（setUpUser）之前调用，否则，点击个人中心的“注销”按钮，会再次出现“游戏账号”窗口
-(void)changeLogOption:(KYLOGOPTION)option;

//4.是否需要输入邀请码（如，游戏封测时需要输入邀请码,默认不需要）
//参数isShowVerifycdKey：YES，需要邀请码。NO，不需要邀请码
-(void)verifycdKeyOption:(BOOL)isShowVerifycdKey;

//5.（选）开发商主动调用更新方法，必须将Build和Version参数填为一致
//场景1：必须在游戏刚进入时启动更新，否则不能进游戏。则调用此方法，同时必须将游戏的后续代码放入更新的回调里，否则更新容易出错
//场景2：若非必须在游戏启动时启动更新，可以在登录界面弹出，则忽略此方法，使用SDK自带的。
-(void)checkUpdate;



/*----------“7659账号”登录相关的设置------------*/

//1.显示用户界面
-(void)showUserView;

//2.记住上次用户直接登录
-(void)logWithLastUser;

//3.显示用户中心界面，必须添加
-(void)setUpUser;

//4.注销当前用户，回调游戏,有回调
-(void)userLogOut;

//5.注销当前用户，返回登录界面
-(void)userBackToLog;



/*!!!!!新接入游戏、已上架且仅使用7659账号的开发商忽略此步骤!!!!!*/
/*----------- “游戏账号”相关的设置------------*/


//1.开启游戏账号窗口[[KYSDK instance] changeLogOption:KYLOG_ONGAMENAME];显示游戏账号登录
-(void)changeLogOption:(KYLOGOPTION)option;

//2.设置是否开启游戏账号忘记密码功能
-(void)setISShowForgetGamePwd:(BOOL)isShowForgetGamePwd;

//3.是否开启游戏账号登录发送短息功能
-(void)setISSendSMS:(BOOL)isShowSendSMS;

//4.游戏账号登陆时，传入用户名密码
-(void)gameLoginWithArray:(NSMutableArray *)array;

//5.游戏账号是否错误,登录成功，返回suc，登录界面即可关闭。失败等可以返回其他字符，比如“账号或密码不正确”，会直接显示在登录界面上
-(void)showStateGame:(NSString *)state;





/*----------支付相关的设置------------*/
/**
 1.支付信息填写
 dealseq：  订单号，唯一透传参数，最大64位，不可重复；
 fee：      金额，保留两位小数，系统默认6位小数
 game：     http://payquery.bppstore.com开发者后台查询对应的game值，一般为4位数字
 gamesvr：  多个通告地址的设置。只区分不同的通告地址，不一定是区服。若只有一个通告地址，则填空@"",若有多个，@技术支持进行后台录入
 subject:   道具名称，比如“60金币”
 md5Key：   http://payquery.bppstore.com开发者后台查询对应的"签名密钥"
 userid:    账户名，单机游戏必须传入值,网游填空@""
 appScheme：支付宝钱包客户端对应的回调参数，要与targets-》info-》url types中的 url schemes中设置的一模一样，建议使用bundle identifier
 **/
-(void)showPayWith:(NSString *)dealseq fee:(NSString *)fee game:(NSString *)game gamesvr:(NSString *)gamesvr subject:(NSString *)subject md5Key:(NSString *)md5Key userId:(NSString*)userId appScheme:(NSString *)appScheme;

//2.支付界面点击“关闭”按钮的回调,可以在此事件触发时，去服务器端请求支付结果
-(void)closePayCallback;


@end




