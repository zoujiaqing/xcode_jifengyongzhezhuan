#include "HSJoystick.h" 
#include "UIManager.h"

#define	BACKGROUND_FILE "UI/cont1.png"
#define THUMB_FILE "UI/cont2.png"

#define JOYSTICK_OFFSET_X 60.0f
#define JOYSTICK_OFFSET_Y 40.0f

#define THUMB_RADIUS 40.0f

//静摩擦系数
#define START_VEL 0.0f

//多点触摸最小距离，用来稳定摇杆操作，值越大越稳定，同时也可能导致左右手指过近时摇杆还原
#define MULTI_TOUCH_DIS 240.0f

//稳定速度，不定义则为有上限的线性速度
#define STABLE_SPEED

//固定摇杆位置
#define FIXED_POSITION

//摇杆显示类型 1：总是可见 2：点击可见 3：总是不可见
#define SHOW_TYPE 1

HSJoystick* HSJoystick::pSharedJoystick = NULL;


static bool isPointInCircle(CCPoint point, CCPoint center, float radius){
    float dx = (point.x - center.x);
    float dy = (point.y - center.y);
    return (radius * radius >= dx*dx + dy*dy);
}

static CCPoint convertCoordinate(CCPoint point){
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    return ccp(point.x, screenSize.height - point.y);
}

bool HSJoystick::isPointInJoystick(CCPoint point)
{
	bool ret = false;
	if (isEnable)
	{
		ret = isPointInCircle(point,kCenter,joystickRadius);
	}
	return ret;
}

HSJoystick * HSJoystick::sharedJoystick()
{
	if(!pSharedJoystick)
	{
		pSharedJoystick = HSJoystick::create();

	}
	return pSharedJoystick;
}

HSJoystick::HSJoystick()
	:isEnable(true)
{

}

HSJoystick::~HSJoystick()
{
	this->removeAllChildren();
	//CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	pSharedJoystick = NULL;

#if WIN32
	CCDirector::sharedDirector()->getOpenGLView()->RemovWin32KeyLayer(this);
#endif
};

bool HSJoystick::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

        this->setTouchEnabled(true);
		
        velocity = CCPointZero;		
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		float ratio = screenSize.width/960.0 ;
		
		ratio2 = ratio*ratio ;
        bg = CCSprite::create(BACKGROUND_FILE);
		joystickRadius = bg->getContentSize().width / 2;		
		kCenter=CCPoint(joystickRadius + JOYSTICK_OFFSET_X,
						joystickRadius + JOYSTICK_OFFSET_Y);
        bg->setPosition(kCenter);
        this->addChild(bg,0);

        thumb = CCSprite::create(THUMB_FILE);
        thumb->setPosition(kCenter);
		
        this->addChild(thumb,1);

		float ballRadius = thumb->getContentSize().width / 2;

		//最后的数字为修正
		this->thumbRadius = joystickRadius - ballRadius + 15;
		this->resetJoystick();

#if WIN32
		CCDirector::sharedDirector()->getOpenGLView()->AddWin32KeyLayer(this);
#endif

        bRet=true;

    }while(0);

    return bRet;
}

void HSJoystick::updateVelocity(CCPoint point)
{	
	
	this->joystickPos = point;

    float dx = point.x - kCenter.x;
    float dy = point.y - kCenter.y;
	
    float distance = ccpDistance(point, kCenter);   

    if(distance > joystickRadius){
        dx = joystickRadius * dx / distance;
        dy = joystickRadius * dy / distance;
    }
	
    if(distance >= START_VEL * joystickRadius)
	{		
#ifdef STABLE_SPEED
		if(dx == 0 && dy == 0)
		{
			this->velocity = CCPointZero;
		}
		else
		{
			this->velocity = ccpNormalize(ccp(dx/joystickRadius, dy/joystickRadius));	
		}
#else
		this->velocity = ccp(dx/joystickRadius, dy/joystickRadius);
#endif
	}
	else
	{
		this->velocity = CCPointZero;
	}

	if(velocity.x == 0 && velocity.y == 0)
	{
		this->direction = None;
	}
	else if (velocity.y > fabs(velocity.x))
	{
		this->direction = Up;
	}
	else if (velocity.y < 0 && fabs(velocity.y) > fabs(velocity.x))
	{
		this->direction = Down;
	}
	else if (velocity.x >= fabs(velocity.y))
	{
		this->direction = Right;
	}
	else
	{
		this->direction = Left;
	}

    if(distance > thumbRadius)
    {
        point.x = kCenter.x + thumbRadius * dx / joystickRadius;
        point.y = kCenter.y + thumbRadius * dy / joystickRadius;
    }

    thumb->setPosition(point);
}

void HSJoystick::resetJoystick()
{
    this->updateVelocity(kCenter);
	this->isPressed = false;

#if SHOW_TYPE != 1
	this->bg->setVisible(false);
	this->thumb->setVisible(false);
#endif
	
}

bool HSJoystick::handleLastTouch()
{
    bool wasPressed = this->isPressed;
	
    if(wasPressed){
        this->resetJoystick();        
    }
    return (wasPressed ? true : false);
}

void HSJoystick::refreshJoystickPosition(CCPoint position)
{	
	this->bg->setPosition(position);
	this->thumb->setPosition(position);
	this->kCenter = position;
	this->joystickPos = position;
}

void HSJoystick::setScale(float scale)
{
	bg->setScale(scale);
	thumb->setScale(scale);
	thumbRadius *= scale;

	//修正位置
	float radius2 = scale * bg->getContentSize().width / 2;
	CCPoint newPos = CCPoint(radius2 + scale * JOYSTICK_OFFSET_X,radius2 + scale * JOYSTICK_OFFSET_Y);
	refreshJoystickPosition(newPos);

	return;
}

void HSJoystick::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint point = touch->getLocationInView();
	point = convertCoordinate(point);

#if SHOW_TYPE == 2
	#ifdef FIXED_POSITION
	if(isPointInCircle(point,kCenter,joystickRadius)){
	#endif		
		this->bg->setVisible(true);
		this->thumb->setVisible(true);		
	#ifdef FIXED_POSITION
	}
	#endif
#endif

#ifdef FIXED_POSITION
	
	//CCLOG("HSJoystick ccTouchesBegan [%f ,%f] ,[%f ,%f] ,%f ,[%f,%f] ,%d",joystickPos.x,joystickPos.y ,point.x ,point.y ,joystickRadius ,kCenter.x ,kCenter.y , this->isPressed );
	if(isPointInCircle(point,kCenter,joystickRadius + 200)){
		//CCLOG("HSJoystick ccTouchesBegan ");
        this->isPressed = true;		
        this->updateVelocity(point);
    }
	
#else
	if(!isPressed){
		this->refreshJoystickPosition(point);
		this->isPressed = true;
	}
#endif
}

void HSJoystick::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    if(this->isPressed){
        CCTouch *touch = (CCTouch*)pTouches->anyObject();
        CCPoint point = touch->getLocationInView();		
		point = convertCoordinate(point);
	//	CCLOG("HSJoystick ccTouchesBegan [%f ,%f] ,[%f ,%f] ,%f ,[%f,%f] ,%d",joystickPos.x,joystickPos.y ,point.x ,point.y ,joystickRadius ,kCenter.x ,kCenter.y , this->isPressed );
		if((ccpDistanceSQ(joystickPos, point) < MULTI_TOUCH_DIS * MULTI_TOUCH_DIS*ratio2))
			this->updateVelocity(point);
    }
}

void HSJoystick::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint point = pTouch->getLocationInView();
	point = convertCoordinate(point);

	if(ccpDistanceSQ(joystickPos, point) < MULTI_TOUCH_DIS * MULTI_TOUCH_DIS)
		this->handleLastTouch();
}

void HSJoystick::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	for (CCSetIterator iter = pTouches->begin(); iter != pTouches->end(); iter++)
	{
		CCTouch *touch =  (cocos2d::CCTouch*)(*iter);
		CCPoint point = touch->getLocationInView();
		point = convertCoordinate(point);
	//	CCLOG("HSJoystick ccTouchesEnded [%f ,%f] ,[%f ,%f] ,%f ,[%f,%f] ,%d",joystickPos.x,joystickPos.y ,point.x ,point.y ,joystickRadius ,kCenter.x ,kCenter.y,this->isPressed );
		if(ccpDistanceSQ(joystickPos, point) < MULTI_TOUCH_DIS * MULTI_TOUCH_DIS*ratio2)
			this->handleLastTouch();
	}
}

void HSJoystick::setIsEnable(bool enable)
{
	this->isEnable = enable;
	if (enable)
	{
		this->bg->setVisible(true);
		this->thumb->setVisible(true);
		this->setTouchEnabled(true);
	}
	else
	{
		this->bg->setVisible(false);
		this->thumb->setVisible(false);
		this->setTouchEnabled(false);
	}

	resetJoystick();
}

#if WIN32

void HSJoystick::processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_LEFT:
			{
				CCPoint point = kCenter;
				point.x -= joystickRadius;
				updateVelocity(point);
			}
			break;
		case VK_RIGHT:
			{
				CCPoint point = kCenter;
				point.x += joystickRadius;
				updateVelocity(point);
			}
			break;
		case VK_UP:
			{
				CCPoint point = kCenter;
				point.y += joystickRadius;
				updateVelocity(point);
			}
			break;
		case VK_DOWN:
			{
				CCPoint point = kCenter;
				point.y -= joystickRadius;
				updateVelocity(point);
			}
			break;
		default:
				break;
		}
		break;
	case WM_KEYUP:
		switch( wParam )
		{
		case VK_LEFT:
			{
				CCPoint point = kCenter;
				updateVelocity(point);
			}
			break;
		case VK_RIGHT:
			{
				CCPoint point = kCenter;
				updateVelocity(point);
			}
			break;
		case VK_UP:
			{
				CCPoint point = kCenter;
				updateVelocity(point);
			}
			break;
		case VK_DOWN:
			{
				CCPoint point = kCenter;
				updateVelocity(point);
			}
			break;
		default:
			break;
		}
		break;
	default:
			break;
	}
}

#endif