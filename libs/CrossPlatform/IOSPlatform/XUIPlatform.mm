//
//  XIOSUIPlatform.cpp
//  iSeer
//
//  Created by razer tong on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "XUIPlatform.h"
#import "AppController.h"
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

#define KPLAYER_NAME_TEXT_FIELD  0


XUIPlatform* CreateXUIPlatform()
{
    XUIPlatform *ui = new XUIPlatform();
    return ui;
}

XUIPlatform::XUIPlatform()
{    
    InitPlatform();
}


///
//  InitPlatform
//  init all the devices that needed!
//
void XUIPlatform::InitPlatform()
{
    
}

///
//  ShutDown
//  clean up all context used  
//
void XUIPlatform::ShutDown()
{

}


void XUIPlatform::playVideo(const char* name,int tag)
{
    AppController* clientApp        = (AppController *)[UIApplication sharedApplication].delegate;
    //[clientApp playVideo:name withTag:tag];
}

#define KPLAYER_NAME_FONT_SIZE (isipad?32:16)

void XUIPlatform::addCreatePlayerWithTextField()
{
    bool isipad = false;
    
#ifdef UI_USER_INTERFACE_IDIOM
    isipad = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#endif
    UIApplication* clientApp        = [UIApplication sharedApplication];
    UITextField *textField;
    if (isipad) 
    {
        textField = [[[UITextField alloc] initWithFrame:CGRectMake(520, 260, 210, 50)] autorelease];
    }else
    {
        textField = [[[UITextField alloc] initWithFrame:CGRectMake(240, 105, 105, 25)] autorelease];
    }
    //    UITextField *textField          = [[[UITextField alloc] initWithFrame:CGRectMake(520, 260, 210, 50)] autorelease];
    textField.borderStyle           = UITextBorderStyleRoundedRect;
    textField.font                  = [UIFont systemFontOfSize:KPLAYER_NAME_FONT_SIZE];
    textField.placeholder           = @"";
    textField.tag                   = 0;
    textField.autocorrectionType    = UITextAutocorrectionTypeNo;
    textField.keyboardType          = UIKeyboardTypeDefault;
    textField.returnKeyType         = UIReturnKeyDone;
    textField.clearButtonMode       = UITextFieldViewModeNever;
    textField.layer.borderWidth     = 3;
    textField.layer.cornerRadius    = 10;
    textField.layer.borderColor     = [[UIColor colorWithRed:0 green:102.0/255.0 blue:153.0/255.0 alpha:1] CGColor];
    textField.delegate              = (AppController *)clientApp.delegate;
    [textField setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter];
    [[((AppController *)clientApp.delegate) getViewControllerView] addSubview:textField];
}

const char *XUIPlatform::getPlayerName()
{
    UIApplication* clientApp = [UIApplication sharedApplication];
    UITextField *textField = (UITextField *)[[((AppController *)clientApp.delegate) getViewControllerView] viewWithTag:0];
    //去除左右空格
    NSString *temp = [textField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if (temp.length ==0  || temp == nil)
    {
        return "";
    }else
    {
        return [textField.text UTF8String];
    }
}

void XUIPlatform::hidePlayerTextField()
{
    UIApplication* clientApp = [UIApplication sharedApplication];
    UITextField *textField = (UITextField *)[[((AppController *)clientApp.delegate) getViewControllerView] viewWithTag:KPLAYER_NAME_TEXT_FIELD];
    textField.hidden = YES;
}
void XUIPlatform::showPlayerTextField()
{
    UIApplication* clientApp = [UIApplication sharedApplication];
    UITextField *textField = (UITextField *)[[((AppController *)clientApp.delegate) getViewControllerView] viewWithTag:KPLAYER_NAME_TEXT_FIELD];
    textField.hidden = NO;
}

void XUIPlatform::removePlayerTextField()
{
    UIApplication* clientApp = [UIApplication sharedApplication];
    [[[((AppController *)clientApp.delegate) getViewControllerView] viewWithTag:KPLAYER_NAME_TEXT_FIELD] removeFromSuperview];
}



void XUIPlatform::showInitInput(float x, float y, float w, float h)
{
      
}

void XUIPlatform::hideInitInput()
{
     
}

void XUIPlatform::removeInitInput()
{
     
}

int XUIPlatform::getInputOffsetY()
{
    return 0;
}

void XUIPlatform::setInputFrame(float x, float y, float w, float h)
{
    
}

int XUIPlatform::getInputNum()
{
    return 0;
}

void XUIPlatform::setMaxExp(int num)
{
    
}

void XUIPlatform::addActicityIndicator() 
{
    AppController* clientApp   = (AppController *)[UIApplication sharedApplication].delegate;
    [clientApp addActicityIndicator];
}

void XUIPlatform::removeActicityIndicator()
{
    AppController* clientApp   = (AppController *)[UIApplication sharedApplication].delegate;
    [clientApp removeActicityIndicator];
}

void  XUIPlatform::onTaomeeAccountManagerTouched()
{
   AppController* clientApp   = (AppController *)[UIApplication sharedApplication].delegate;
    [clientApp onTaomeeAccountManagerTouched];
}

void  XUIPlatform::onTaomeeMoreTouched()
{
    AppController* clientApp   = (AppController *)[UIApplication sharedApplication].delegate;
    [clientApp onTaomeeMoreTouched];
}
#define TAG_ADVERTISE_VIEW  12202
void XUIPlatform::addTaomeeAdvertise()
{
     
}

void XUIPlatform::setAdvertiseVisible(bool bVisible)
{
    UIView* _TaomeeAdView = [[UIApplication sharedApplication].keyWindow viewWithTag:TAG_ADVERTISE_VIEW];
    if(bVisible)
    {
        [_TaomeeAdView setHidden:NO];
    }
    else
    {
        [_TaomeeAdView setHidden:YES];
    }
}

