//
//  NcSDKManager.h
//  AnimationPro
//
//  Created by tianTian on 6/24/13.
//
//

#ifndef __AnimationPro__NcSDKManager__
#define __AnimationPro__NcSDKManager__

#include <iostream>

#define kFlurryStatisticEnterGame    "FlurryStartGame"
#define kCocoDataStatisticEnterGame  "CocoDataStartGame"
#define kHailStoneStatisticEnterGame "HailStoneStartGame"

class NcSDKManager
{
public:
    NcSDKManager();
    virtual ~NcSDKManager();
    
    // for login
    static void doLogin();
    static void doLogout();
    static void doLastUserLogin();
    
    // for pay
    static void doPay(const char* productid, const char* userid, const char* serverid, const char* moneyType, const char* price, const char* userRegistTime, const char* level);
    
    // for appstore pay verify
    static void sendInAppPurchaseString(std::string receiptstring);
    static int sendToPlatformVerify(std::string receiptstring);
    static std::string combineReceipt(std::string receipt);
    static std::string deCombineReceipt(std::string combinedReceipt);
    static void requestIAPInitData();
    static void reSendVerifyFailedReceipt();
    static void setResendVerifyFlag(bool isResend);
    static bool getResendVerifyFlag();
    static void setIsInPurchasing(bool flag);
    static void checkAppstoreUpdate(const char* url);
    static void setInitIAPFlag(bool flag);
    static bool getInitIAPFlag();
    
    // for web url
    static void openWebUrl(std::string url);
    
    // networkdisconnect
    static void NetworkDisconnect(const char* content);
    static void KillProces();
    static bool isNetWorkConneted();
    
    // float button
    static void SetFloatButtonHidden(bool isHidden);
    static bool isFloatButtonHiden();
    
    // show waiting layer
    static void showWaitingLayer();
    static void hideWaitingLayer();
    
    // for statistic
    // flurry (appstore and escape)
    static void enterFlurry(int channelid);
    static void trackFlurryEvent(const char* str);
    
    // cocodata (only for appstore)
    static void enterCocoData(int channelid);
    static void trackCocoDataEvent(const char* str);
    static void cocoDataViewBegain(const char* viewName);
    static void cocoDataViewEnd(const char* viewName);
    
    // hailstone (appstore and escape)
    static void enterHailStone(int channelid);
    static void trackHailStoneEvent(const char* str);
    static void HailStoneloginWithAccout(int userId);
    static void HailStonecreateRoleWithAccout(int userId, const char* userName, const char* charator);
    static void HailStoneroleLoginWithAccount(int userId, const char* userName, int level);

    // inmobi (only for appstore)
    static void enterInmovi();
    
    // appcpa (only for appstore)
    static void enterAppCpa(int channelid);
    static void trackReg(const char* userid);
    static void trackLogin(const char* userid);
    static void trackpay(const char* userid, const char* orderid, int amount, const char* currency);
    
    // cocoanalyse (only for escape)
    static void enterCocoAnalyse(int channelid);
    
    // get version
    static std::string getBundleVersion();
};

#endif /* defined(__AnimationPro__NcSDKManager__) */
