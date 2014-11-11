//
//  NcSDKManager.cpp
//  AnimationPro
//
//  Created by sean on 5/12/14.
//
//

#include "NcSDKManager.h"
#include "cocos2d.h"
#import <Foundation/Foundation.h>
#import "LoginController.h"
#import "StringUtil.h"
#include "LoginManager.h"
#include "JSONKit.h"
#include "XLogicManager.h"
#include "WaitingLayerManager.h"
#include "GlobalFunction.h"
#ifdef TargetForKy
#import "KyLoginMan.h"
#import "KyPayMan.h"
#import "KYSDK.h"
#endif

#ifdef TargetForCoCoAppStore
#import "CocoLogin.h"
#include "IAPManager.h"
#import "RageIAPHelper.h"
#endif
#import "UpdateChecker.h"

#ifdef TargetForPP
#import "PPLoginMan.h"
#import "PPPayMan.h"
#endif

#ifdef TargetForNd91
#import "NdLoginMan.h"
#import "OrderMan.h"
#import "NdPayMan.h"
#endif

#ifdef TargetForItools
#import "HXPaymentMan.h"
#import "HXLoginMan.h"
#endif

#ifdef TargetForTB
#import "TbLoginMan.h"
#import "TbPayMan.h"
#endif

#ifdef TargetForI4
#import "I4LoginMan.h"
#import "I4PayMan.h"
#endif

#ifdef TargetForHm
#import "CocoLogin.h"
#import "HaimaLoginMan.h"
#import "HaimaPayMan.h"
#endif

#ifdef TargetForXy
#import "XYPayMan.h"
#import "XYLoginMain.h"
#endif

#import "FlurryMan.h"
#import "CocoDataMan.h"
#import "HailStoneMan.h"
#import "TrackingAppCPA.h"
#import "TrackingInmobi.h"
//#import "CocoAnalyseMan.h"

using namespace cocos2d;

static std::string _productId = "";
static std::string _userId = "";
static std::string _serverId = "";
static std::string _moneyType = "";
static std::string _price = "";
static std::string _userRegistTime = "";
static std::string _level = "";
static bool _isInPurchasing = false;
static bool _isResendReceiptVerify = false;
static bool _isIAPInit = false;

NcSDKManager::NcSDKManager()
{

}

NcSDKManager::~NcSDKManager()
{
}

// for login
void NcSDKManager::doLogin()
{
#ifdef TargetForKy
    [[KyLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForCoCoAppStore
    [[CocoLogin sharedInstance] doLogin];
#endif
    
#ifdef TargetForPP
    [[PPLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForNd91
    [[NdLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForItools
    [[HXLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForTB
    [[TbLoginMan sharedInstance] doLogin];
    [[TbLoginMan sharedInstance] setToolbarHidden:true];
#endif
    
#ifdef TargetForI4
    [[I4LoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForHm
    [[CocoLogin sharedInstance] doLogin];
#endif
    
#ifdef TargetForXy
    [[XYLoginMain sharedInstance] doLogin];
#endif
}

// for repeated login
void NcSDKManager::doLastUserLogin()
{
#ifdef TargetForKy
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[KyLoginMan sharedInstance] backToLog];
#endif
    
#ifdef TargetForCoCoAppStore
    [[CocoLogin sharedInstance] doLogin];
#endif
    
#ifdef TargetForPP
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[PPLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForNd91
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[NdLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForItools
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[HXLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForTB
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[TbLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForI4
    if(XLogicManager::sharedManager()->isIOSInScene == true)
    {
        XLogicManager::sharedManager()->isIOSInScene = false;
        return;
    }
    [[I4LoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForHm
    [[CocoLogin sharedInstance] doLogin];
#endif
#ifdef TargetForXy
    [[XYLoginMain sharedInstance] doLogin];
#endif
}

// for logout, especialy user center
void NcSDKManager::doLogout()
{
    XLogicManager::sharedManager()->isIOSNeedWaitingLayer = true;
#ifdef TargetForKy
    XLogicManager::sharedManager()->isIOSInScene = false;
    [[KyLoginMan sharedInstance] doLogout];
#endif
    
#ifdef TargetForCoCoAppStore
    [[CocoLogin sharedInstance] doLogin];
#endif
    
#ifdef TargetForPP
    [[PPLoginMan sharedInstance] doLogout];
	[[PPLoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForNd91
    [[NdLoginMan sharedInstance] switchAccount];
#endif
    
#ifdef TargetForItools
    [[HXLoginMan sharedInstance] doLogout];
#endif
    
#ifdef TargetForTB
    [[TbLoginMan sharedInstance] switchAccount];
#endif
    
#ifdef TargetForI4
    [[I4LoginMan sharedInstance] doLogout];
    [[I4LoginMan sharedInstance] doLogin];
#endif
    
#ifdef TargetForHm
    [[CocoLogin sharedInstance] doLogin];
#endif
    
#ifdef TargetForXy
    [[XYLoginMain sharedInstance] switchAccount];
#endif
}

// moneyType 0 rmb 1 dollar
void NcSDKManager::doPay(const char* productid, const char* userid, const char* serverid, const char* moneyType, const char* price, const char* userRegistTime, const char* level)
{
#ifdef TargetForCoCoAppStore
    if(_isInPurchasing == true)
    {
        return;
    }
    showWaitingLayer();
#endif
    
    NSString* pid = [NSString stringWithCString:productid encoding:[NSString defaultCStringEncoding]];
    NSString* uid = [NSString stringWithCString:userid encoding:[NSString defaultCStringEncoding]];
    
    int channelx = [[LoginController sharedInstance] getChannelId];
    NSString* cid = [NSString stringWithFormat:@"%d",channelx];
    
    NSString* sid = [NSString stringWithCString:serverid encoding:[NSString defaultCStringEncoding]];
    NSString* cashType = [NSString stringWithCString:moneyType encoding:[NSString defaultCStringEncoding]];
    
    //NSDate *localDate = [NSDate date];
    //NSString *timeSp = [NSString stringWithFormat:@"%ld", (long)[localDate timeIntervalSince1970]];
    NSString* timeSp = [NSString stringWithCString:userRegistTime encoding:[NSString defaultCStringEncoding]];
    
#ifdef TargetForKy
    [KyPayMan sharedInstance].isInnerMode = NO;
    [[KyPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
    
#ifdef TargetForCoCoAppStore
    std::string productidStr = "";
    int proid = atoi(productid);
    switch (proid)
    {
        case 82001:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier1";
            break;
            
        case 82002:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier5";
            break;
            
        case 82003:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier15";
            break;
            
        case 82004:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier30";
            break;
            
        case 82005:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier50";
            break;
            
        case 82006:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.tier60";
            break;
            
        case 82007:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.card.tier5";
            break;
            
        case 82008:
            productidStr = "com.cocoentertainment.jifengyongzhezhuan.iap.card.tier4";
            break;
            
        default:
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"charge" message:@"failed, null product id!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
                [alert show];
                [alert release];
            }
            break;
    }
    _isResendReceiptVerify = false;
    IAPManager::payForProduct(productidStr);
    
    _productId = [pid UTF8String];
    _userId = [uid UTF8String];
    _serverId = [sid UTF8String];
    _moneyType = [cashType UTF8String];
    _price = price;
    _userRegistTime= [timeSp  UTF8String];
    _level = level;
    
    // appcpa(talking data) anlyse
    trackpay(userid, productid, atoi(price), moneyType);
#endif
    
#ifdef TargetForPP
    [PPPayMan sharedInstance].isInnerMode = NO;
    [[PPPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
    
#ifdef TargetForNd91
    [NdPayMan sharedInstance].isInnerMode = NO;
    [[NdPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif

#ifdef TargetForItools
    [HXPaymentMan sharedInstance].isInnerMode = NO;
    [[HXPaymentMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
    
#ifdef TargetForTB
    [TbPayMan sharedInstance].isInnerMode = NO;
    [[TbPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
    
#ifdef TargetForI4
    [I4PayMan sharedInstance].isInnerMode = NO;
    [[I4PayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
    
#ifdef TargetForHm
    [HaimaPayMan sharedInstance].isInnerMode = NO;
    [[HaimaPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
#ifdef TargetForXy
    [XYPayMan sharedInstance].isInnerMode = NO;
    [[XYPayMan sharedInstance] doPayWithProductId:pid userId:uid channelId:cid serverId:sid currency:cashType andUserCreateTime:timeSp];
#endif
}

int NcSDKManager::sendToPlatformVerify(std::string receiptstring)
{
    USERINFO mUserData = UserData::GetUserInfo();
    _userId = IntToStr(mUserData.id.getID());
    _serverId = IntToStr(mUserData.serverId);
    _userRegistTime= IntToStr(mUserData.id.getRegTime());
    _level = IntToStr(mUserData.level);
    
    // amount app_name channel_id device jailbreak level product_id receipt regtime server_id udid user_id
    int channelx = [[LoginController sharedInstance] getChannelId];
    NSString* cid = [NSString stringWithFormat:@"%d",channelx];
    
    NSString* receiptStr = [NSString stringWithCString:receiptstring.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString* appName = @"com.cocoentertainment.jifengyongzhezhuan";
    NSString* jailbreakstr = @"0"; // 0 非越狱 1 越狱
    std::string devicestr = LoginManager::Get()->getDeviceType();
    std::string deviceId = LoginManager::Get()->getDeviceId();
    NSString* deviceStr = [NSString stringWithCString:devicestr.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString* deviceIdStr = [NSString stringWithCString:deviceId.c_str() encoding:[NSString defaultCStringEncoding]];
    
    // set toSign str
    std::string tempStro = _price;
    std::string toSignStr = tempStro.append([appName UTF8String]).append([cid UTF8String]).append(devicestr.c_str()).append([jailbreakstr UTF8String]).append(_level.c_str()).append(_productId.c_str()).append(receiptstring.c_str()).append(_userRegistTime.c_str()).append(_serverId.c_str()).append(deviceId.c_str()).append(_userId.c_str()).append("a8898beb143a4b0d46fc8259a2ac6717");
    
    // get md5
    NSString* toSignStrX = [NSString stringWithCString:toSignStr.c_str() encoding:[NSString defaultCStringEncoding]];
    const char* receipt = [receiptStr UTF8String];
    const char* sign = [[StringUtil MD5:toSignStrX] UTF8String];
    const char* app_name = "com.cocoentertainment.jifengyongzhezhuan";
    const char* user_id = _userId.c_str();
    const char* regtime = _userRegistTime.c_str();
    const char* server_id = _serverId.c_str();
    const char* product_id = _productId.c_str();
    const char* level = _level.c_str();
    const char* amount = _price.c_str();
    const char* udid = [deviceIdStr UTF8String];
    const char* jailbreak = "0";
    const char* device = [deviceStr UTF8String];
    
    // post to verify url
    NSURL *url = [NSURL URLWithString:@"http://wlpay.shootao.com/paycheck/iospay/new_apple_pay.php"];
    
    //第二步，创建请求
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
    [request setHTTPMethod:@"POST"];//设置请求方式为POST，默认为GET
    
    //设置参数数据
    CCString * strdata = new CCString();
	strdata->autorelease() ;
    strdata->initWithFormat("receipt=%s&sign=%s&app_name=%s&user_id=%s&regtime=%s&server_id=%s&product_id=%s&level=%s&amount=%s&udid=%s&jailbreak=%s&device=%s&channel_id=%s", receipt, sign, app_name, user_id, regtime, server_id, product_id, level, amount, udid, jailbreak, device, [cid UTF8String]);
    NSString* str = [NSString stringWithCString:strdata->getCString() encoding:[NSString defaultCStringEncoding]];
    NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
    [request setHTTPBody:data];
    
    //第三步，连接服务器
    NSData *received = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *str1 = [[NSString alloc]initWithData:received encoding:NSUTF8StringEncoding];
    
    // decode json data of key "result": 0 success other fail
    // use jsonkit to parse jsondata ,because ios 4.5 earlyversion not support NSJSONSerialization
    NSData* jsonData = [str1 dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    id ret = nil;
    int status = 0;
    JSONDecoder *jsonDecoder = [JSONDecoder decoder];
    if ([jsonDecoder respondsToSelector:@selector(objectWithData:error:)]){
        ret = [jsonDecoder performSelector:@selector(objectWithData:error:) withObject:jsonData withObject:(id)&error];
    }else{
        ret = [jsonDecoder performSelector:@selector(parseJSONData:error:) withObject:jsonData withObject:(id)&error];
    }
    if (error==nil && [ret isKindOfClass:[NSDictionary class]]){
        status = [[ret objectForKey:@"result"] intValue];
    }
    NSLog(@"%d",status);
    
    [str1 release];
    return status;
}

// receipt main enter
void NcSDKManager::sendInAppPurchaseString(std::string receiptstring)
{
    // 先把所有小票都存起来
    bool isIn = false;
    for(int i = 0; i < 30; i++)
    {
        CCString* key = CCString::createWithFormat("receipt%d", i);
        const char* receiptKey = key->getCString();
        std::string combinedReceipt = CCUserDefault::sharedUserDefault()->getStringForKey(receiptKey);
        if(std::strcmp(deCombineReceipt(combinedReceipt).c_str(), receiptstring.c_str()) == 0)
        {
            isIn = true;
            break;
        }
    }
    
    if(isIn == false)
    {
        for(int j = 0; j < 30; j++)
        {
            CCString* key = CCString::createWithFormat("receipt%d", j);
            const char* receiptKey = key->getCString();
            std::string combinedReceipt = CCUserDefault::sharedUserDefault()->getStringForKey(receiptKey);
            if(std::strcmp(combinedReceipt.c_str(), "") == 0)
            {
                CCUserDefault::sharedUserDefault()->setStringForKey(receiptKey, combineReceipt(receiptstring));
                break;
            }
        }
    }
    
    // 如果成功的或特殊情况则删除（10000成功）（10119验证处理错误，请清理小票）
    int result = NcSDKManager::sendToPlatformVerify(receiptstring);
    if(result == 10000 || result == 10119)
    {
        // if userdefault has this receipt,then delete it
        for(int k = 0; k < 30; k++)
        {
            CCString* key = CCString::createWithFormat("receipt%d", k);
            const char* receiptKey = key->getCString();
            std::string combinedReceipt = CCUserDefault::sharedUserDefault()->getStringForKey(receiptKey);
            if(std::strcmp(deCombineReceipt(combinedReceipt).c_str(), receiptstring.c_str()) == 0)
            {
                CCUserDefault::sharedUserDefault()->setStringForKey(receiptKey, "");
            }
        }
        
        // success alert
        if(result != 10119)
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"充值成功" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
        }
    }
    else
    {
        // fail, add this receipt to userdefault for storing (format: uid + serverid + receipt)
        
        // fail alert
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"获取钻石失败,请重新登录游戏" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
    }
}

std::string NcSDKManager::combineReceipt(std::string receipt)
{
    std::string str = _userId;
    str = str.append("$").append(_serverId).append("$").append(receipt);
    return str;
}

std::string NcSDKManager::deCombineReceipt(std::string combinedReceipt)
{
    if(std::strcmp(combinedReceipt.c_str(), "") == 0)
    {
        return "";
    }
    
    // a$12$s len=6, 1
    int index = combinedReceipt.find_first_of("$");
    int len = combinedReceipt.length();
    std::string tempStr = combinedReceipt.substr(index + 1, len - index - 1);
    int index2 = tempStr.find_first_of("$");
    std::string rStr = tempStr.substr(index2 + 1, tempStr.length() - index2 - 1);
    return rStr;
}

void NcSDKManager::reSendVerifyFailedReceipt()
{
    for(int i = 0; i < 30; i++)
    {
        CCString* key = CCString::createWithFormat("receipt%d", i);
        const char* receiptKey = key->getCString();
        std::string combinedReceipt = CCUserDefault::sharedUserDefault()->getStringForKey(receiptKey);
        if(std::strcmp(combinedReceipt.c_str(), "") == 0)
        {
            continue;
        }
        
        // receipt uid
        int index = combinedReceipt.find_first_of("$");
        std::string ruid = combinedReceipt.substr(0, index);
        if(std::strcmp(ruid.c_str(), _userId.c_str()) != 0)
        {
            continue;
        }
        
        // receipt serverid
        int len = combinedReceipt.length();
        std::string tempStr = combinedReceipt.substr(index + 1, len - index - 1);
        int index1 = tempStr.find_first_of("$");
        std::string rserverId = tempStr.substr(0, index1);
        if(std::strcmp(rserverId.c_str(), _serverId.c_str()) != 0)
        {
            continue;
        }
        
        // send
        _isResendReceiptVerify = true;
        sendInAppPurchaseString(tempStr.substr(index1 + 1, tempStr.length() - index1 - 1));
    }
}

void NcSDKManager::requestIAPInitData()
{
#ifdef TargetForCoCoAppStore
    [RageIAPHelper sharedInstance];
    [[RageIAPHelper sharedInstance] requestProducts];
#endif
}

void NcSDKManager::setResendVerifyFlag(bool isResend)
{
    _isResendReceiptVerify = isResend;
}

bool NcSDKManager::getResendVerifyFlag()
{
    return _isResendReceiptVerify;
}

void NcSDKManager::setIsInPurchasing(bool flag)
{
    _isInPurchasing = flag;
}

void NcSDKManager::openWebUrl(std::string url)
{
    NSString* str = [NSString stringWithCString:url.c_str() encoding:[NSString defaultCStringEncoding]];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void NcSDKManager::NetworkDisconnect(const char* content)
{
    StringUtil* util = [[StringUtil alloc] autorelease];
    [util NetworkDisconnect:content];
}

void NcSDKManager::KillProces()
{
//    StringUtil* util = [StringUtil alloc];
//    [util KillProcessBySelf];
    CCDirector::sharedDirector()->end();
    exit(0);
}

bool NcSDKManager::isNetWorkConneted()
{
    return [[LoginController sharedInstance] isConnectionAvailable];
}


void NcSDKManager::SetFloatButtonHidden(bool isHidden)
{
#ifdef TargetForNd91
    [[NdLoginMan sharedInstance] setNdBarHidden:isHidden];
#endif
    
#ifdef TargetForTB
    [[TbLoginMan sharedInstance] setToolbarHidden:isHidden];
#endif
    
#ifdef TargetForXy
    [[XYLoginMain sharedInstance] setToolBarHidden:isHidden];
#endif

#ifdef TargetForItools
    [[HXLoginMan sharedInstance] setToolBarHidden:isHidden];
#endif
}

bool NcSDKManager::isFloatButtonHiden()
{
#ifdef TargetForNd91
    return [[NdLoginMan sharedInstance] isToolBarHidden];
#endif
    
#ifdef TargetForTB
    return [[TbLoginMan sharedInstance] isToolBarHidden];
#endif
    
#ifdef TargetForXy
    return [[XYLoginMain sharedInstance] isToolBarHidden];
#endif
    
#ifdef TargetForItools
    [[HXLoginMan sharedInstance] isToolBarHidden];
#endif
}

void NcSDKManager::checkAppstoreUpdate(const char* url)
{
    [UpdateChecker checkVersion:url];
}

void NcSDKManager::showWaitingLayer()
{
    _isInPurchasing = true;
    WaitingLayerManager::Get()->PushOneMessage("payforappstore");
}
void NcSDKManager::hideWaitingLayer()
{
    _isInPurchasing = false;
    WaitingLayerManager::Get()->CheckPopOneMessage("payforappstore");
}

// for statistic
// flurry ///////////////////////////////////////
void NcSDKManager::enterFlurry(int channelid)
{
    CCString* tempStr = CCString::createWithFormat("%d", channelid);
    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
#ifdef TargetForCoCoAppStore
    [FlurryMan initFlurryWithChannel:str isTrackLocation:NO isNeedCrashReport:YES isAppStore:YES];
#else
    [FlurryMan initFlurryWithChannel:str isTrackLocation:NO isNeedCrashReport:YES isAppStore:NO];
#endif
}
void NcSDKManager::trackFlurryEvent(const char* str)
{
    NSString* tempstr = [NSString stringWithCString:str encoding:[NSString defaultCStringEncoding]];
    [FlurryMan trackEvent:tempstr withParameters:nil];
}

// cocodata ///////////////////////////////////////
void NcSDKManager::enterCocoData(int channelid)
{
    CCString* tempStr = CCString::createWithFormat("%d", channelid);
    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
#ifdef TargetForCoCoAppStore
    [CocoDataMan didFinishLaunchingWithChannel:str];
#endif
}
void NcSDKManager::trackCocoDataEvent(const char* str)
{
    NSString* tempstr = [NSString stringWithCString:str encoding:[NSString defaultCStringEncoding]];
#ifdef TargetForCoCoAppStore
    [CocoDataMan trackEvent:tempstr label:nil params:nil];
#endif
}
void NcSDKManager::cocoDataViewBegain(const char* viewName)
{
#ifdef TargetForCoCoAppStore
    [CocoDataMan viewBegin:[NSString stringWithCString:viewName encoding:[NSString defaultCStringEncoding]]];
#endif
}
void NcSDKManager::cocoDataViewEnd(const char* viewName)
{
#ifdef TargetForCoCoAppStore
    [CocoDataMan viewEnd:[NSString stringWithCString:viewName encoding:[NSString defaultCStringEncoding]]];
#endif
}

// hailstone ///////////////////////////////////////
void NcSDKManager::enterHailStone(int channelid)
{
    CCString* tempStr = CCString::createWithFormat("%d", channelid);
    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
#ifdef TargetForCoCoAppStore
    [HailStoneMan didFinishLaunchingWithChannel:str server:@"0" isAppStore:YES];
#else
    [HailStoneMan didFinishLaunchingWithChannel:str server:@"0" isAppStore:NO];
#endif
}
void NcSDKManager::trackHailStoneEvent(const char* str)
{
    NSString* tempstr = [NSString stringWithCString:str encoding:[NSString defaultCStringEncoding]];
    [HailStoneMan customEventWithAccount:nil roleID:nil roleName:nil type:tempstr andDescription:nil];
}
void NcSDKManager::HailStoneloginWithAccout(int userId)
{
    CCString* tempStr = CCString::createWithFormat("%d", userId);
    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
    NSString* accountName = [NSString stringWithCString:XLogicManager::sharedManager()->accountName.c_str() encoding:[NSString defaultCStringEncoding]];
    [HailStoneMan loginWithAccount:accountName andAccountID:str];
}
void NcSDKManager::HailStonecreateRoleWithAccout(int userId, const char* userName, const char* charator)
{
    CCString* tempStr = CCString::createWithFormat("%d", userId);
    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
    NSString* accountName = [NSString stringWithCString:XLogicManager::sharedManager()->accountName.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString* gameName = [NSString stringWithCString:userName encoding:[NSString defaultCStringEncoding]];
    [HailStoneMan createRoleWithAccount:accountName roleID:str roleName:gameName andCharactor:nil];
}
void NcSDKManager::HailStoneroleLoginWithAccount(int userId, const char* userName, int level)
{
//    CCString* tempStr = CCString::createWithFormat("%d", userId);
//    NSString* str = [NSString stringWithCString:tempStr->getCString() encoding:[NSString defaultCStringEncoding]];
//    NSString* accountName = [NSString stringWithCString:XLogicManager::sharedManager()->accountName.c_str() encoding:[NSString defaultCStringEncoding]];
//    NSString* gameName = [NSString stringWithCString:userName encoding:[NSString defaultCStringEncoding]];
//    [HailStoneMan roleLoginWithAccount:accountName roleID:str roleName:gameName andLevel:level];
}


// inmobi (only for appstore)
void NcSDKManager::enterInmovi()
{
#ifdef TargetForCoCoAppStore
    [[TrackingInmobi sharedInstance] startTracking];
#endif
}

// appcpa (only for appstore)
void NcSDKManager::enterAppCpa(int channelid)
{
#ifdef TargetForCoCoAppStore
    CCString* str = CCString::createWithFormat("%d", channelid);
    NSString* channelStr = [NSString stringWithCString:str->getCString() encoding:[NSString defaultCStringEncoding]];
    [[TrackingAppCPA sharedInstance] startTrackingWithChannelID:channelStr];
#endif
}
void NcSDKManager::trackReg(const char* userid)
{
#ifdef TargetForCoCoAppStore
    NSString* uidStr = [NSString stringWithCString:userid encoding:[NSString defaultCStringEncoding]];
    [[TrackingAppCPA sharedInstance] trackRegister:uidStr];
#endif
}
void NcSDKManager::trackLogin(const char* userid)
{
#ifdef TargetForCoCoAppStore
    NSString* uidStr = [NSString stringWithCString:userid encoding:[NSString defaultCStringEncoding]];
    [[TrackingAppCPA sharedInstance] trackLogin:uidStr];
#endif
}
void NcSDKManager::trackpay(const char* userid, const char* orderid, int amount, const char* currency)
{
#ifdef TargetForCoCoAppStore
    NSString* uidStr = [NSString stringWithCString:userid encoding:[NSString defaultCStringEncoding]];
    NSString* oidStr = [NSString stringWithCString:orderid encoding:[NSString defaultCStringEncoding]];
    NSString* curStr = [NSString stringWithCString:currency encoding:[NSString defaultCStringEncoding]];
    [[TrackingAppCPA sharedInstance] trackPayment:uidStr orderId:oidStr amount:amount currency:curStr payType:@"APPLE_IAP"];
#endif
}

// cocoanalyse (only for escape)
void NcSDKManager::enterCocoAnalyse(int channelid)
{

}


// get bundle version
std::string NcSDKManager::getBundleVersion()
{
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];
    return [version UTF8String];
}

void NcSDKManager::setInitIAPFlag(bool flag)
{
    _isIAPInit = flag;
}

bool NcSDKManager::getInitIAPFlag()
{
    return _isIAPInit;
}




