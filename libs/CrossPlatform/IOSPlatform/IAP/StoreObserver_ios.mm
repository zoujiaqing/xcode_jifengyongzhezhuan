//
//  StoreObserver.m
//  idou
//
//  Created by razer tong on 11-11-3.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "StoreObserver_ios.h"
//#include "OnlineManager.h"
#include "StoreManager_ios.h"
#import "NSString+NSStringAdditions.h"
#include "UserData.h"


@implementation StoreObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
	{
        NSLog(@"transaction.transactionState=%d",transaction.transactionState);
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchased:
				
                [self completeTransaction:transaction];
				
                break;
				
            case SKPaymentTransactionStateFailed:
				
                [self failedTransaction:transaction];
				
                break;
				
            case SKPaymentTransactionStateRestored:
				
                [self restoreTransaction:transaction];
				
            default:
				
                break;
		}	
		NSLog(@"transaction.transactionState=%d",transaction.transactionState);
        //交易回执
        NSData *receipt= transaction.transactionReceipt;
        [receipt bytes];
	}
}
/*
 收到交易回执以后可以传送到你的服务器处理该交易。
 你的服务器可以向苹果发送交易验证信息。
 苹果的交易验证地址：https:/y.itunes.apple.comerifyReceipt
 验证数据格式：{ receipt-data : …}
 苹果的返回值如果是{ status : 0 } 那么交易是正确，如果返回任何其他值 { status : any_other_value } 交易就是不成立。
 你的服务器收到苹果的验证成功信息以后，返回给应用程序类似于解锁功能等消息
 */


- (void) failedTransaction: (SKPaymentTransaction *)transaction
{	
//    NSLog(@"failedTransaction %@ %@",transaction.JSONRepresentation,transaction.error.description);
    
    if (transaction.error.code != SKErrorPaymentCancelled)		
    {		
        // Optionally, display an error here.		
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];	
}

- (void) completeTransaction: (SKPaymentTransaction *)transaction
{	
    NSLog(@"complete Transaction");
   
    

//    NSData *receipt =transaction.transactionReceipt;

//    NSString *codeReceipt = [NSString base64StringFromData:receipt length:[receipt length]];
    
    //自己验证　for test
    /*
    if([self putStringToItunes:codeReceipt])
    {
        NSLog(@"success");
    }else
    {
        NSLog(@"false");
    }
    */
    
    [[StoreManager sharedManager] addVipGolds];
    //存档关键点
    //4.使用人名币买金斗
    
    
    //向服务器进行验证　验证出错　服务器直接拉黑
    //OnlineManager::sharedManager()->checkReceipt([codeReceipt UTF8String], [codeReceipt length],  [[StoreManager sharedManager] convertCurrencyToUSD]);

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
/*
-(BOOL)putStringToItunes:(NSString*)encodingStr
{//用户购成功的transactionReceipt
      
//         NSString*encodingStr = [iapData base64EncodedString];
      
         NSString *URL=@"https://sandbox.itunes.apple.com/verifyReceipt";
         //https://buy.itunes.apple.com/verifyReceipt
         NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];// autorelease];
         [request setURL:[NSURL URLWithString:URL]];
         [request setHTTPMethod:@"POST"];
         //设置contentType
         [request addValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
         //设置Content-Length
         [request setValue:[NSString stringWithFormat:@"%d", [encodingStr length]] forHTTPHeaderField:@"Content-Length"];  
     
        NSLog(@"%d",[encodingStr length]);
         NSDictionary* body = [NSDictionary dictionaryWithObjectsAndKeys:encodingStr, @"receipt-data", nil];
         SBJsonWriter *writer = [SBJsonWriter new];
         [request setHTTPBody:[[writer stringWithObject:body] dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES]];
         NSHTTPURLResponse *urlResponse=nil;
         NSError *errorr=nil;
         NSData *receivedData = [NSURLConnection sendSynchronousRequest:request
                                                  returningResponse:&urlResponse
                                                      error:&errorr];

         //解析
        NSString *results=[[NSString alloc]initWithBytes:[receivedData bytes] length:[receivedData length] encoding:NSUTF8StringEncoding];
         NSLog(@"-Himi- %@",results);
         NSDictionary*dic = [results JSONValue];
         if([[dic objectForKey:@"status"] intValue]==0){//注意，status=@"0" 是验证收据成功
                 return true;
             }
         return false;
     }
*/
- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{	
    NSLog(@"restoreTransaction");
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];	
}


#pragma mark restore transcation
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    
}


#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"success=%@",  [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease]);

//    [self recordTransaction:bookid];
//    [self provideContent:bookid];

    //[self.receivedData appendData:data];
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    switch([(NSHTTPURLResponse *)response statusCode]) 
    {
        case 200:
        case 206:
            break;
        case 304:
            
            break;
        case 400:
            
            break;
            
            
        case 404:
            break;
        case 416:
            break;
        case 403:
            break;
        case 401:
        case 500:
            break;
        default:
            break;
    }        
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error 
{
    NSLog(@"fail");
    NSLog(@"%@",  [error description]);
    NSLog(@"%d %@",[error code],[error domain]);
    
}
@end
