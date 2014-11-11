#pragma once

#include "CFrameNode.h"


//补间控制点
class CCTweenNode : public CCFrameNode
{
public:
	static CCTweenNode *create();

public:
	CCTweenNode();
	~CCTweenNode(void);

	/* 让 TweenNode 从两个关键点中取值 */
	void betweenValue(CCFrameNode* _from, CCFrameNode* _to);
	/**
	* 让 TweenNode 从两个关键点中取值
	* param _k 0~1 的值，当此值为 0，TweenNode 取 from 点值，当此值为 1，TweenNode 取 to 点值
	*/
	void tweenTo(float _k);

	void remove();

public:
	void reset();


public:
	/* 是否为序列帧动画*/
	CC_SYNTHESIZE(bool, mIsFrames, IsFrames);
	CC_SYNTHESIZE(bool, mIsImageChange, IsImageChange);
	CC_SYNTHESIZE(bool, mIsSameFrameFlag, IsSameFrameFlag);
protected:
	float sR;
	float sX;
	float sY;
	float sSX;
	float sSY;
	float sSKX;
	float sSKY;
	float sA;

	float dR;
	float dX;
	float dY;
	float dSX;
	float dSY;
	float dSKX;
	float dSKY;
	float dA;
};

