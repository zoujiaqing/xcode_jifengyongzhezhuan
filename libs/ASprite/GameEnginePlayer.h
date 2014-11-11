#ifndef __GameEnginePlayer_h__
#define __GameEnginePlayer_h__

#include "ASprite.h"

class GameEnginePlayer
{
public:

    enum  AS_TRANS{
        TRANS_NONE,        
        TRANS_MIRROR,        
        TRANS_MIRROR_ROT180,        
        TRANS_ROT180,        
        TRANS_ROT90,
        TRANS_MIRROR_ROT270,
        TRANS_MIRROR_ROT90,
        TRANS_ROT270, 
    };
    
	//cGame* pGame;
//////////////////////////////////////////////////////////////////////////
///   GameEnginePlayerASprite.jpp
//////////////////////////////////////////////////////////////////////////

    CCObject* m_selectorTarget;
    SEL_CallFuncND m_selector;
    void *m_data;
    
    CCObject* m_selectorTimeTarget;
    SEL_CallFunc m_selectorTime;
    
    bool m_pause;
    bool m_isLoop;
    bool m_flip;
	/// Sprite player current X position.
	int	posX;					

	/// Sprite player current Y position.
	int	posY;					

	/// Sprite player current display flags.
	int	curFlags;

	/// Sprite player current ASprite reference.
	ASprite* sprite;

	/// Sprite current animation.
	int curAnim;

	/// Sprite current frame in the current animation.
	int curFrame;

	/// Sprite current time. Used for frame that stay on screen longer than a frame.
	int curTime;

	int nbLoop;
	boolean animIsOver;

	GameEnginePlayer ();
	GameEnginePlayer(ASprite* sprite, CCSpriteBatchNode* batchnode,CCSprite* parent, int x, int y);
	~GameEnginePlayer ();
    CCSpriteBatchNode* getBatchNode() { return m_BatchNode; }
	CCSprite* getParentNode() {return m_parent;}
	void Reset();
	void ResetAnim();
	void SetPos (int x, int y);
	ASprite* GetSprite ();
	void SetSprite (ASprite* sprite);
	void SetAnim (int anim);
	void SetAnim (int anim, int nbLoop);
    void SetAnim (int anim, int nbLoop, bool isloop);
    void setLoop(bool loop);
	int GetAnim ();
	void SetFrame (int frame);
	int GetFrame ();
	void SetTransform (int transform);
	int GetTransform ();
	int GetNbanim ();
	int GetNbFrame ();
	int GetDuration ();
	boolean IsAnimOver ();
	void Render ();
	void Update ();
	void Update (int DT);
    void drawFrame(int index);
    void setActionWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector,void *data);
    void setTimeWithTarget(CCObject* pSelectorTarget, SEL_CallFunc selector);
    void runSelector();
    void runTimeSelector();
    CCRect getCurFrameRect();
    CCRect getFirstFrameRect();
    void pause();
    void resume();
    
private:
    CCSpriteBatchNode* m_BatchNode;
    CCSprite* m_parent;
};
#endif //__GameEnginePlayer_h__