#include "XUIPlatform.h"

XUIPlatform* CreateXUIPlatform()
{
	XUIPlatform *ui = new XUIPlatform();
    return ui; 
}

XUIPlatform::XUIPlatform()
{
	InitPlatform();
}

void XUIPlatform::InitPlatform()
{
}

void XUIPlatform::ShutDown()
{
}

void XUIPlatform::playVideo(const char* name,int tag)
{
}
    
void XUIPlatform::addCreatePlayerWithTextField()
{
}
    
const char *XUIPlatform::getPlayerName()
{
	return "Todo";
}
    
void XUIPlatform::hidePlayerTextField()
{
}
    
void XUIPlatform::showPlayerTextField()
{
}
    
void XUIPlatform::removePlayerTextField()
{
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
	return 1;
}
    
void XUIPlatform::setMaxExp(int num)
{
}
    
void XUIPlatform::addActicityIndicator()
{
}
    
void XUIPlatform::removeActicityIndicator()
{
}
    
void  XUIPlatform::onTaomeeAccountManagerTouched()
{
}
    
void XUIPlatform::onTaomeeMoreTouched()
{
}
    
void XUIPlatform::addTaomeeAdvertise()
{
}
    
void XUIPlatform::setAdvertiseVisible(bool bVisible)
{
}