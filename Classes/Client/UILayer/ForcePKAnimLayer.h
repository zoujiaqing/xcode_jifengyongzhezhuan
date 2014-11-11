#ifndef _FORCE_PK_ANIM_LAYER_H_
#define _FORCE_PK_ANIM_LAYER_H_

#include "ASprite.h"
#include "UILayout.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIPicture.h"
#include "vector"
#include "EffectSprite.h"

using namespace std;
using namespace cocos2d;

class ForcePKAnimLayer : public cocos2d::CCLayer , public CCAnimationEventListener
{
public:
	ForcePKAnimLayer();
	virtual ~ForcePKAnimLayer();

	CREATE_FUNC(ForcePKAnimLayer);
	virtual bool init();

	void SetAnimPauseOnEnd(bool bPauseOnEnd);
	void SetAnimEndCallBack(CCObject* pListerner,SEL_CallFunc callBack);
protected:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);
private:
	UIPicture* m_pBackGroundPic;
	EffectSprite* m_pPKEffect;
	bool m_isAnimPlayOver;
	bool m_isPlaySound;
	unsigned int m_nPlaySoundFrame;

	CCObject* m_pAnimEndListerner;
	SEL_CallFunc m_pfnAnimEndSelector;
};

#endif	//_FORCE_PK_ANIM_LAYER_H_