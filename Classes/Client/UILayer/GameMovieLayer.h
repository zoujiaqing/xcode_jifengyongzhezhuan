#ifndef _GAME_MOVIE_LAYER_H_
#define _GAME_MOVIE_LAYER_H_

#include "cocos2d.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

class GameMovieLayer : public cocos2d::CCLayerColor
{
public:
	enum UPDATE_STATE{
		UPDATE_STATE_START_ANIM = 0,
		UPDATE_STATE_UNKNOW,
	};
public:
	GameMovieLayer();
	virtual ~GameMovieLayer();

	CREATE_FUNC(GameMovieLayer);
	virtual bool init();
	virtual void update(float fDelta);

	// Note: 开场动画相关
	void StartPlayStartAnimation();
	void StartContentAnimation(int pageIndex);
	void EndStartAnimation();

	void UpdateStartAnimation(float fDelta);
	void OneAnimationOver();
	void OneUpdateRcsAnimationOver();
	void OneLabelFadOutOver();
	void ShowLoadingControls(bool bShowLoadingControls);
protected:
	void InitLayer();	

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void skipBtnClick(CCObject* sender);
private: 
	TXGUI::UILayout* layout;
	// Note: 开场动画相关
	static const int ANIMATION_COUNT = 7;
	cocos2d::CCSprite *animationSprites[ANIMATION_COUNT];
	UPDATE_STATE EUpdateState;
	unsigned int animationFrame;
	float animationTimer[ANIMATION_COUNT];
	float animatonLastTime;
	bool  isAnimPause;
	float oneAnimationFadTime;

	CCPoint ContentDisplayAreaMidPt;
	CCPoint ContentDownAreaPt;
	CCPoint ContentUpAreaPt;
	CCSize  ContentDisplayAreaSize;
	CCSize	OneLabelAreaSize;

	struct OneLabelInfo 
	{
		CCLabelTTF* pLabel;
		int state;
		float moveTime;
		OneLabelInfo(){
			pLabel = NULL;
			// Note: 0 未开始 1 淡入 2 淡出
			state = 0;
			moveTime = 0;
		}
	};
	static const int LABEL_COUNTS = 21;
	OneLabelInfo displayLabels[LABEL_COUNTS];
	float labelDistance;
	float labelRealDistance;
	float labelMoveSpeed;
	float labelMoveTime;
	bool  isShowStartAnimationText;
	TXGUI::UIButton* skipAnimationBtn;

	int curLabelPageIndex;
};

#endif // _GAME_MOVIE_LAYER_H_