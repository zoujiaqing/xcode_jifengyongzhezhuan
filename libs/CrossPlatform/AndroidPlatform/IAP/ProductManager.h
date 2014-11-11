//
//  StoreManager.h
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef idou_StoreManager_h
#define idou_StoreManager_h

// Disable
//#include "GameDataManager.h"

typedef enum
{
    KProductA=0,//com.taomee.seer.vipgold1 
    KProductB,
    KProductC,
    KProductD,
    KProductE,
    KProductF,
    KProductUnknow,
}KProductType;

typedef  struct  
{
    KProductType type;
    int num;//赛尔金豆的数量
    float cost;
}KProductInfo;

namespace CocosIAP 
{
class  ProductManager
    {
 
    public:
        ProductManager();
        ~ProductManager();
		static ProductManager *sharedManager();
        
        //购买金斗
		void buyProduct(KProductType type);
       
        //增加用户已购买到的金斗
		void addVipGolds();
        
        //请求服务器的数据
		void requesetProductData();
        
        //本地化价格
        const char *getFormattedPriceAtIndex(int index);
    };
}

#endif
