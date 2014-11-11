//
//  StoreManager.m
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "ProductManager.h"
#include "StoreManager_ios.h"
static void static_buyProduct(KProductType type)
{
    [[StoreManager sharedManager] productPurchased:type];
}


//static void static_checkTrancation()
//{
//    [[StoreManager sharedManager] checkTrancation];
//}

static void static_addVipGolds()
{
    [[StoreManager sharedManager]addVipGolds];
}

const char *static_getFormattedPriceAtIndex(int index)
{
    return[[StoreManager sharedManager]getFormattedPriceAtIndex:index];
}

static void static_requesetProductData()
{
    [[StoreManager sharedManager] requestProductData];
}
namespace CocosIAP
{
    static ProductManager *pStoreManager = NULL;
    
    ProductManager::ProductManager()
    {
    
    }
    
    ProductManager::~ProductManager()
    {
        
    }
    
    ProductManager* ProductManager::sharedManager()
    {
        if (pStoreManager)
        {
            pStoreManager = new ProductManager();
        }
        return pStoreManager;
    }
    
    void ProductManager::buyProduct(KProductType type)
    {

        static_buyProduct(type);
    }
    
//    void ProductManager::checkTrancation()
//    {
//        static_checkTrancation();
//    }
    
    void ProductManager::addVipGolds()
    {
        static_addVipGolds();
    }
    
    void ProductManager::requesetProductData()
    {
        static_requesetProductData();
    }
    
    const char *ProductManager::getFormattedPriceAtIndex(int index)
    {
       return  static_getFormattedPriceAtIndex(index);
    }
 
}


