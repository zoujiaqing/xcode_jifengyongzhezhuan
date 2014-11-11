
#include "GameEnginePlayer.h"
//--------------------------------------------------------------------------------------------------------------------
/// Empty constructor. 
/// @note It is up to user to fill all information.
//--------------------------------------------------------------------------------------------------------------------
GameEnginePlayer::GameEnginePlayer()
{
    Reset();
}

//--------------------------------------------------------------------------------------------------------------------
/// Basic constructor. Default constructor to use to Play some animations.
/// @param sprite Sprite to use by this player. The player will only keep a reference to the ASprite eg it won't be copied.
/// @param x Ininital X Position of the player.
/// @param y Ininital Y Position of the player.
//--------------------------------------------------------------------------------------------------------------------
GameEnginePlayer::GameEnginePlayer(ASprite* sprite, CCSpriteBatchNode *batchnode,CCSprite* parent, int x, int y)
{
    Reset();
    posX 	= x;
    posY 	= y;
    SetSprite(sprite);
    m_parent = parent;
    m_BatchNode = batchnode;
}

GameEnginePlayer::~GameEnginePlayer ()
{
	Reset();
}

//--------------------------------------------------------------------------------------------------------------------
/// Reset the current player. The player will be in the same state as if you would have called the empty constructor.
/// @note Usefull only on the ASprite player, no effect on the sound.
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::Reset()
{
    m_selectorTarget = NULL;
    m_selector = NULL;
    m_selectorTimeTarget = NULL;
    m_selectorTime = NULL;
    
    posX 	= 0;
    posY 	= 0;
    curAnim = -1;
    curFrame = 0;
    sprite 	= NULL;
    curFlags = 0;
    curTime = 0;
    m_isLoop = false;
    nbLoop = 1;
    animIsOver = true;
    m_flip = false;    
    m_pause = false;    
}

void GameEnginePlayer::ResetAnim()
{
    animIsOver = false;
    nbLoop = 0;
    curFrame = 0;
    curTime = 0;
}

//--------------------------------------------------------------------------------------------------------------------
/// Set new position of sprite.
/// @param x Sprite new X Position_
/// @param y Sprite new Y Position_
/// @note There are no validation of the new position, it can be outside of the screen. You can also access directly 
/// the posX and posY variables.
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetPos (int x, int y)
{
    posX = x;
    posY = y;
}



//--------------------------------------------------------------------------------------------------------------------
/// Get the current sprite
//--------------------------------------------------------------------------------------------------------------------
ASprite* GameEnginePlayer::GetSprite ()
{
    return sprite;
}

//--------------------------------------------------------------------------------------------------------------------
/// Set a new ASprite reference in the player. This call will also reset the animation number.
/// @param sprite Sprite to be used from now on. If the sprite is null, this will remove the reference to any sprite from this player.
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetSprite (ASprite* sprite)
{
    this->sprite = sprite;
    SetAnim (-1, -1);
}

//--------------------------------------------------------------------------------------------------------------------
/// Set a new animation number to play, animation will loop forever
/// @param anim Animation number to play. The animation will reset the next frame to play of this new animation to 0. 
/// @note if an animation is currently playing, and try a setanim on the same anim -> nothing is done, do SetAnim(-1, 1);
/// SetAnim(anim, loop) to force an update
/// @deprecated use SetAnim (int anim, int nbLoop) instead
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetAnim (int anim)
{
    //		WARNING("GameEnginePlayer->SetAnim(int anim) :: this function is deprecated, use GameEnginePlayer->SetAnim(int anim, int nbLoop) instead");
    SetAnim (anim, -1);
}

//--------------------------------------------------------------------------------------------------------------------
/// Set a new animation number to play.
/// @param anim Animation number to play
/// @param nbLoop how many time this animation should loop (-1 for forever)(cannot be 0)
/// @note if an animation is currently playing, and try a setanim on the same anim -> nothing is done, do SetAnim(-1, 1);
//  SetAnim(anim, loop) to force an update
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetAnim (int anim, int nbLoop)
{		
    if (	animIsOver
        ||	(anim != curAnim)
        )
    {
        curAnim = anim;
        SetFrame(0);
        this->nbLoop = nbLoop-1;
        animIsOver = false;
        m_pause = false;
    }
}

void GameEnginePlayer::SetAnim (int anim, int nbLoop, bool isLoop)
{    
    m_isLoop = isLoop;
    SetAnim(anim, nbLoop);
}

void GameEnginePlayer::setLoop(bool loop)
{
    m_isLoop = loop;
}

//--------------------------------------------------------------------------------------------------------------------
/// get animation
/// @return current selected animation, -1 if none
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetAnim ()
{
    return curAnim;
}

//--------------------------------------------------------------------------------------------------------------------
/// Set a new frame position in the current animation.
/// @param frame New frame postion.
/// @return Frame adjusted if frame was larger than the frame count. Ex: if you ask for frame 7 of a 5 frame animation, 
/// this call will place the frame at 2. return -1 if no animation
/// @note In DEBUG, there will be an assert if frame is smaller than 0, but not in RELEASE.
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetFrame (int frame)
{
    //ASSERT(sprite != null, "GameEnginePlayer::SetFrame()::sprite is not set");
    CCAssert(frame >= 0, "GameEnginePlayer::SetFrame()::frame is negative");

    int nbFrame = GetNbFrame();
    
    curFrame 	= (nbFrame <= 0 ? 0 : (frame % nbFrame));
    curTime 	= 0;
}

//--------------------------------------------------------------------------------------------------------------------
/// get current frame nb 
/// @return current frame nb in current animation, result is invalid if no animation is set
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetFrame ()
{
    return curFrame;
}

//--------------------------------------------------------------------------------------------------------------------
/// set current sprite transformation 
/// @param transform  any of GameEngine transform 
/// @see pGame->TRANS_NONE, pGame->TRANS_ROT90, pGame->TRANS_ROT180, pGame->TRANS_ROT270, pGame->TRANS_MIRROR, 
/// pGame->TRANS_MIRROR_ROT90, pGame->TRANS_MIRROR_ROT180, pGame->TRANS_MIRROR_ROT270
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::SetTransform (int transform)
{
    CCAssert(transform >= TRANS_NONE && transform <= TRANS_ROT270, "transform invalid");
    curFlags = transform;
}

//--------------------------------------------------------------------------------------------------------------------
/// get current sprite transformation
/// @return one of pGame->TRANS_NONE, pGame->TRANS_ROT90, pGame->TRANS_ROT180, pGame->TRANS_ROT270, pGame->TRANS_MIRROR,
/// pGame->TRANS_MIRROR_ROT90, pGame->TRANS_MIRROR_ROT180, pGame->TRANS_MIRROR_ROT270
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetTransform ()
{
    return curFlags;
}

//--------------------------------------------------------------------------------------------------------------------
/// Get the animation count of the current sprite.
/// @return The animation count of the current sprite.
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetNbanim ()
{
    //		ASSERT(sprite != null, "GameEnginePlayer::GetNbanim()::sprite is not set");
    return sprite->GetAnimationNum();
}

//--------------------------------------------------------------------------------------------------------------------
/// Get the frame count of the current animation.
/// @return The frame count of the current animation or -1 if the animation is not set.
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetNbFrame ()
{
    //		ASSERT(sprite != null, "GameEnginePlayer::GetNbFrame()::sprite is not set");
    if (curAnim >= 0 && sprite)
        return sprite->GetAFrames(curAnim);
    else
        return -1;
}

//--------------------------------------------------------------------------------------------------------------------
/// Get the duration of the current frame of the current animation.
/// @return The duration of the current frame of the current animation or 0 if the animation is not set.
//--------------------------------------------------------------------------------------------------------------------
int GameEnginePlayer::GetDuration ()
{
    //		ASSERT(sprite != null, "GameEnginePlayer::GetDuration()::sprite is not set");
    
    if (curAnim >= 0 && sprite)
        return sprite->GetAFrameTime(curAnim, curFrame);
    else
        return 100000;
}

//--------------------------------------------------------------------------------------------------------------------
/// Query the state of the current animation to see if its over.
/// @return True if the current animation is over.
//--------------------------------------------------------------------------------------------------------------------
boolean GameEnginePlayer::IsAnimOver ()
{
    //		ASSERT(sprite != null, "GameEnginePlayer::isAnimOver()::sprite is not set");
    
    // if no anim
    if (curAnim < 0)
        return true;
    
    // if loop forever
    if (nbLoop < 0)
        return false;
    
    return animIsOver;
    
    // if (curFrame < GetNbFrame() - 1)
    // return false;
    
    // return true;
    //return (curTime >= GetDuration());
}

//--------------------------------------------------------------------------------------------------------------------
/// Render the curreny animation in GameEngine curent graphic context. The animation will be drawn at posX,posY.
/// \sa GameEngine::setCurrentGraphics
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::Render ()
{
    //		ASSERT(sprite != null, "GameEnginePlayer::render()::sprite is not set");
    
    // if no anim, do nothing
    
    if(m_pause)
        return;
    
    if (curAnim < 0)
        return;
    
	if (sprite)
		sprite->PaintAFrame(m_parent, m_BatchNode, curAnim, curFrame, posX, posY, curFlags);
}

void GameEnginePlayer::drawFrame(int index)
{
    if(index < 0)
        return;    

    if (sprite)
		sprite->PaintFrame(m_parent, m_BatchNode, index, posX, posY, curFlags);
}

//--------------------------------------------------------------------------------------------------------------------
/// Update current animation time from 1 time unit. If the current frame time is exceeded the current frame will be increased, if the current 
/// frame reach the animation frame count, the current frame will be looped to 0.
///@deprecated use void Update(int DT) instead
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::Update ()
{
    if(m_pause)
        return;
    Update(1);
    
    CCRect rect = getCurFrameRect();
    if(rect.size.width >0 && rect.size.height > 0)
    {
        runTimeSelector();
    }
    // if no anim, do nothing
    if (animIsOver)
        runSelector();	
    
}

//--------------------------------------------------------------------------------------------------------------------
/// Update current animation time from 1 time unit. If the current frame time is exceeded the current frame will be increased, if the current 
/// frame reach the animation frame count, the current frame will be looped to 0.
/// @param DT nb of time unit elapsed since last frame
//--------------------------------------------------------------------------------------------------------------------
void GameEnginePlayer::Update (int DT)
{
    //ASSERT(DT >= 0, "GameEnginePlayer::Update::DT is negative");
	
    // if anim is over, or no anim, do nothing
    if (animIsOver || (curAnim < 0))
        return;
    
    // update current frame time
    curTime += DT;
    
    // get duration of current frame
    int duration = GetDuration();
    //		ASSERT(duration != 0, "GameEnginePlayer::Update::frame "+curFrame+" of animation "+curAnim+" has a duration of 0");
    
    // pass frame until duration is matched (or anim is over)
    while (curTime >= duration) // could cause a pb if curAnim==-1
    {
        // update current time to set time for next frame
        curTime -= duration;
        
        // pass to next frame or loop or end animation
        if (sprite && curFrame < sprite->GetAFrames(curAnim) - 1)
        {
            // pass to next frame
            curFrame++;
        }
        else
        {
            // if end of loop for this animation --> animation is over
            if(m_isLoop)
            {
                curFrame = 0;
            }
            else if (nbLoop == 0)
            {
                animIsOver = true;
                break;
            }
            else
            {
                // decrease nb of loop if not looping forever
                if (nbLoop > 0) 
                    nbLoop--;
                // reset to first frame of animation
                curFrame = 0;
            }
        }
        // get duration of new frame
        duration = GetDuration();
        //			ASSERT(duration != 0, "GameEnginePlayer::Update::frame "+curFrame+" of animation "+curAnim+" has a duration of 0");
    }
    
    //---------------- uncomment this if you want to authorise a negative DT --> and then make animation go backward
    // go backward?
    //while (curTime < 0)
    //{
    //	// get previous frame
    //	if (curFrame == 0)
    //	{
    //		// get last frame of animation
    //		curFrame = sprite->GetAFrames(curAnim) - 1;
    //		
    //		// increase loop number (if not infinite)
    //		if (nbLoop >= 0)
    //			nbLoop++;
    //	}
    //	else
    //		curFrame--;
    //		
    //	// get previous frame duration
    //	duration = GetDuration();
    //
    //	curTime += duration;
    //}
    
}

void GameEnginePlayer::runSelector()
{
    if(m_selectorTarget && m_selector)
    {
        // store the old selector
        SEL_CallFuncND prevSelector = m_selector;
        
        // run selector
        (m_selectorTarget->*m_selector)(NULL,m_data);
        
        // if no new selector is assigned, clear it
        if(m_selector == prevSelector)
        {
            m_selectorTarget = NULL;
            m_selector= NULL;   
        }
    }
}

void GameEnginePlayer::runTimeSelector()
{
    if(m_selectorTimeTarget && m_selectorTime)
    {
        (m_selectorTimeTarget->*m_selectorTime)();
        m_selectorTimeTarget = NULL;
        m_selectorTime= NULL;
    }
}

void GameEnginePlayer::setTimeWithTarget(CCObject* pSelectorTarget, SEL_CallFunc selector)
{
    m_selectorTimeTarget = pSelectorTarget;
    m_selectorTime = selector;
    
}
void GameEnginePlayer::setActionWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector,void *data)
{
    m_selectorTarget = pSelectorTarget;
    m_selector = selector;    
    m_data = data;
}

CCRect GameEnginePlayer::getCurFrameRect()
{
	if (sprite)
    {
		return sprite->getCurAframeRect(curAnim, curFrame);
	}else
	{
		return CCRectZero;
	}
}

CCRect GameEnginePlayer::getFirstFrameRect()
{
	if (sprite)
    {
		return sprite->getframeRect(0);
	}else
	{
		return CCRectZero;
	}
}

void GameEnginePlayer::pause()
{
    m_pause = true;
}

void GameEnginePlayer::resume()
{
    m_pause = false;
}
