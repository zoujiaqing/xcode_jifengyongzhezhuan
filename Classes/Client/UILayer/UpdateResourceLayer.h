#ifndef LOADING_LAYER_H
#define LOADING_LAYER_H


#include "cocos2d.h"
#include "TXGUIHeader.h"

using namespace std;
using namespace cocos2d;

class UpdateResourceLayer : public cocos2d::CCLayer
{
public:
	enum UPDATE_STATE{
		UPDATE_STATE_START_ANIM = 0,
		UPDATE_STATE_DOWN_RCS,
		UPDATE_STATE_UNKNOW,
	};
public:
	UpdateResourceLayer();
	virtual ~UpdateResourceLayer();

	CREATE_FUNC(UpdateResourceLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	static void SetSumFileNums(int nums)
	{
		UpdateResourceLayer::nSumFileNums = nums;
		if (nums <= 0)
		{
			UpdateResourceLayer::bDownLoadOver = true;
		}
	}
	static void SetDownLoadFileNums(int nums)
	{
		UpdateResourceLayer::nDownLoadFileNums = nums;
	}
	static void SetDownLoadOver(bool over)
	{
		UpdateResourceLayer::bDownLoadOver = over;
	}

	void ShowTipInfo();
	void ShowCurProgressInfo();
	void ShowSpeedInfo();
	void ShowCurLoadingProgress(float _progress = -1);

	void DownLoadOver();
	virtual void update(float fDelta);

	// Note: 开场动画相关
	void CheckFirstComeIntoGameFlag();
	void StartPlayStartAnimation();
	void StartContentAnimation(int pageIndex);
	void StartDownLoadResource();
	void EndStartAnimation();

	void UpdateStartAnimation(float fDelta);
	void UpdateResource(float fDelta);
	void OneAnimationOver();
	void OneUpdateRcsAnimationOver();
	void OneLabelFadOutOver();
	void ShowLoadingControls(bool bShowLoadingControls);
	void ShowRcsDownloadTips();

    void OnConfirmUpdate(CCObject* sender);
    void OnCancel(CCObject* sender);

protected:
	void InitLayer();	

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
	void skipBtnClick(CCObject* sender);
    void removeCocosActivityImageView();

	void CheckResouceVersion(float dt);
	void GotoLoginScene(float dt);

	void PopupWarnningMessageBox(float dt);
private: 
	TXGUI::UILayout* layout;
	TXGUI::UILabel*  txtProgressLabel;
	TXGUI::UILabel*  txtSpeedLabel;
	TXGUI::UILabel*  txtUpdateLabel;
	TXGUI::UIPicture* iconLoadingPic;
	TXGUI::UIPicture* barLoadingPic;
	TXGUI::UIPicture* barFrameLoading;
	TXGUI::UIPicture* decLoadingPic;

	float fRunningTimer;
	bool  bInit;

	static int		nSumFileNums;
	static int		nDownLoadFileNums;
	static bool		bDownLoadOver;

	// Note: 开场动画相关
	std::string configFileName;
	bool isFirstComeGame;
	static const int ANIMATION_COUNT = 6;
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

	// Note: 资源更新时使用的控件
	TXGUI::UILabel* tipLabel;
	TXGUI::UILabel* rcsUpdateSpeedLabel;
	TXGUI::UILabel* rcsUpdateProcessLabel_1;
	TXGUI::UILabel* rcsUpdateProcessLabel_2;
	TXGUI::UILabel* rcsUpdateCurVersionLabel;
	TXGUI::UILabel* rcsUpdateNewVersionLabel;
	TXGUI::UIPicture* rcsUpdateFrameLoading;
	TXGUI::UIPicture* rcsUpdateBarLoading;

	int curLabelPageIndex;
};

#endif