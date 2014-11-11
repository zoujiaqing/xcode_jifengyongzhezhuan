#pragma once
#include "cocos2d.h" 

enum JoystickDirection
{
	None = 0,
	Up,
	Down,
	Left,
	Right,
};

using namespace cocos2d;

class HSJoystick : public CCLayer
{

public:

	static HSJoystick* sharedJoystick();
    virtual bool init();  
    CCPoint getVelocity(){return velocity;}
	JoystickDirection getDirection(){return direction;}
	void setScale(float scale);
    CREATE_FUNC(HSJoystick);

	bool getIsEnable(){return isEnable;}
	void setIsEnable(bool enable);

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	bool isPointInJoystick(CCPoint point);

private:
	HSJoystick();
	virtual ~HSJoystick();

	static HSJoystick* pSharedJoystick;
	float joystickRadius;
	float thumbRadius;
	float ratio2 ;
    CCPoint kCenter;
	CCPoint joystickPos;
	CCSprite *bg;
    CCSprite *thumb;
    bool isPressed;

    CCPoint velocity;
	JoystickDirection direction;
	bool isEnable;

    void updateVelocity(CCPoint point);
    void resetJoystick();
    bool handleLastTouch();
	void refreshJoystickPosition(CCPoint position);

	
#if WIN32
	virtual void processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam);
#endif


};