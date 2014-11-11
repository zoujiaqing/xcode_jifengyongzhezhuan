#include "CProcessBase.h"

#define USE_FIX_FRAME_RATES

CCProcessBase::CCProcessBase(void) {
    mScale = 1;
    mIsComplete = false;
    mIsPause = false;
    mCurrentFrame = 0;
	mToFrameID = 0;

	mDuration = 0;
	mCurrentPrecent = 0;
	mLoop = true;
	mEase = 0;

	mBetweenDuration = 0;
	mTotalDuration = 0;

	mTotalFrames = 0;
	mDurationTween = 0;	

#ifdef USE_FIX_FRAME_RATES
	mAnimationInternal = 1.0f / FRAME_RATES;
#else
	mAnimationInternal = CCDirector::sharedDirector()->getAnimationInterval();
#endif	
}


CCProcessBase::~CCProcessBase(void) {
}

void CCProcessBase::resetValue()
{
	mScale = 1;
	mIsComplete = true;
	mIsPause = false;
	mCurrentFrame = 0;
	mToFrameID = 0;
}


void CCProcessBase::remove() {
    mScale = 1;
    mIsComplete = true;
    mIsPause = false;
    mCurrentFrame = 0;
}
void CCProcessBase::pause() {
    mIsPause = true;
}


void CCProcessBase::resume() {
    mIsPause = false;
}

void CCProcessBase::stop() {
    mIsComplete = true;
    mCurrentFrame = 0;
}

void CCProcessBase::playTo(void * _to, int _durationTo, int _durationTween,  bool _loop, int _ease) {
    mIsComplete = false;
    mIsPause = false;
    mCurrentFrame = 0;
    mTotalFrames = 0;
	mCurrentPrecent = 0;
    mEase = _ease;
	mLoop = _loop;

	mScale = 1;
	mToFrameID = 0;

	mDuration = 0;

	mBetweenDuration = 0;
	mTotalDuration = 0;

	mDurationTween = 0;	
}

void CCProcessBase::update(float dt) {
    if (mIsComplete || mIsPause) {
        return;
    }
	
    if( mTotalFrames == 0 ) {
        //CCLOG( "mTotalFrames is zero!" );
		mCurrentPrecent = 1;
    }
	else
	{
		mCurrentFrame += 1 * (dt/mAnimationInternal);

		mCurrentPrecent = mCurrentFrame / (mTotalFrames);

		if( mCurrentFrame > mTotalFrames ){
			mCurrentFrame = (int)(mCurrentFrame) % mTotalFrames;
		}
	}
    
    updateHandler();
}

void CCProcessBase::updateHandler() {
}