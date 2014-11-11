//
//  StoreManager.m
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//

#include "ProductManager.h"

namespace CocosIAP
{
    static ProductManager *pStoreManager = 0;
    
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

    }
   
    void ProductManager::addVipGolds()
    {
        
    }
    
    void ProductManager::requesetProductData()
    {
        
    }
    
    const char *ProductManager::getFormattedPriceAtIndex(int index)
    {
       return  "0.99";
    }
 
}


