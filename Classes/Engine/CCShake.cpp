// Code by Francois Guibert
// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax


#include "CCShake.h"
#include "cocos2d.h"
USING_NS_CC;

#define CCSHAKE_EVERY_FRAME	0


// not really useful, but I like clean default constructors
CCShake::CCShake() : m_strength_x(0), m_strength_y(0), m_initial_x(0), m_initial_y(0)
{
}

CCShake* CCShake::create( float d, float strength )
{
    // call other construction method with twice the same strength
    return createWithStrength( d, strength, strength );
}

CCShake* CCShake::createWithStrength(float duration, float strength_x, float strength_y)
{
    CCShake *pRet = new CCShake();
    
    if (pRet && pRet->initWithDuration(duration, strength_x, strength_y))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

       
    return pRet;
}

bool CCShake::initWithDuration(float duration, float strength_x, float strength_y)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_strength_x = strength_x;
        m_strength_y = strength_y;
        m_Flip = false;
        return true;
    }
    
    return false;
}



CCShake* CCShake::create(float dt, cocos2d::CCPoint pamplitude, bool dampening)
{
    CCShake *pRet = new CCShake();
    
    if (pRet && pRet->initWithDuration(dt, pamplitude, dampening))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool CCShake::initWithDuration(float dt, cocos2d::CCPoint pamplitude, bool dampeningIn)
{
    if (CCActionInterval::initWithDuration(dt))
    {
        startAmplitude	= pamplitude;
		dampening	= dampeningIn;
        
        shakeInterval = 0;

        /*
		// calculate shake intervals based on the number of shakes
		if(shakeNum == CCSHAKE_EVERY_FRAME)
			shakeInterval = 0;
		else
			shakeInterval = 1.f/shakeNum;*/
        
        return true;
    }
    
    return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
static float fgRangeRand( float min, float max )
{
    float rnd = ((float)rand()/(float)RAND_MAX);
    return rnd*(max-min)+min;
}

void CCShake::update(float t)
{
    /*
    float randx = fgRangeRand( -m_strength_x, m_strength_x )*dt;
    //float randy = fgRangeRand( -m_strength_y, m_strength_y )*dt;
    // move the target to a shaked position
    m_pTarget->setPosition( ccpAdd(ccp(m_initial_x, m_initial_y),ccp( randx, 0)));*/
    
    // waits until enough time has passed for the next shake
	if(shakeInterval == CCSHAKE_EVERY_FRAME)
    {} // shake every frame!
	else if(t < nextShake)
		return; // haven't reached the next shake point yet
	else
		nextShake += shakeInterval; // proceed with shake this time and increment for next shake goal
    
	// calculate the dampening effect, if being used
	if(dampening)
    {
		float dFactor = (1-t);
		amplitude.x = dFactor * startAmplitude.x;
		amplitude.y = dFactor * startAmplitude.y;
    }
    
	CCPoint snew = ccp((CCRANDOM_0_1()*amplitude.x*2) - amplitude.x,(CCRANDOM_0_1()*amplitude.y*2) - amplitude.y);
	// simultaneously un-move the last shake and move the next shake
	//[m_pTarget->setPosition:ccpAdd(ccpSub((m_pTarget->getPosition(), last), snew)];
    
    m_pTarget->setPosition(ccpAdd(ccpSub(m_pTarget->getPosition(), last), snew));
    
	// store the current shake value so it can be un-done
	last = snew;
}

void CCShake::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget( pTarget );
    
    // save the initial position
    m_initial_x = pTarget->getPosition().x;
    m_initial_y = pTarget->getPosition().y;
    
    amplitude	= startAmplitude;
	last		= CCPointZero;
	nextShake	= 0;
}

void CCShake::stop(void)
{
    // Action is done, reset clip position
    //this->getTarget()->setPosition( ccp( m_initial_x, m_initial_y ) );
    
    // undo the last shake
	//[target_ setPosition:ccpSub(((CCNode*)target_).position, last)];
    m_pTarget->setPosition(ccpSub(m_pTarget->getPosition(), last));
    
    CCActionInterval::stop();
}