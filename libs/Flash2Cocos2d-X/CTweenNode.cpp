#include "CTweenNode.h"
#include <limits>
#include "SASpriteDefine.h"

CCTweenNode *CCTweenNode::create(){
	CCTweenNode *pTweenNode = new CCTweenNode();
	if (pTweenNode && pTweenNode->init()){
		pTweenNode->autorelease();
		return pTweenNode;
	}
	CC_SAFE_DELETE(pTweenNode);
	return NULL;
} 

CCTweenNode::CCTweenNode( ){
	reset();
}


CCTweenNode::~CCTweenNode(void){
	
}

void CCTweenNode::reset()
{
	CCFrameNode::reset();
	sR = sX = sY = sSKX = sSKY = dR = dX = dY = dSX = dSY = dSKX = dSKY = dA = 0;
	sSX = sSY =1;
	sA = 255;
	img = "";
	mIsImageChange = true;
	mIsSameFrameFlag = false;
}

void CCTweenNode::betweenValue(CCFrameNode* _from, CCFrameNode* _to)
{
	sR = _from->rotation;
	sX = _from->x;
	sY = _from->y;
	sSX = _from->scaleX;
	sSY = _from->scaleY;
	sSKX = _from->skewX;
	sSKY = _from->skewY;
	sA = _from->alpha;

	//CCXLOG("%f, %f", sX, sY);

	std::string preImage = img;
	if( _from->img.compare("") != 0 )
	{
		if (std::string::npos != _from->img.find(SKELETON_IMAGE_FILE_SUFFIX))
		{
			img = _from->img;
		}
		else
		{
			img = _from->img + SKELETON_IMAGE_FILE_SUFFIX;
		}		

		if (preImage == img)
		{
			mIsImageChange = false;
		}
		else
		{
			mIsImageChange = true;
		}
	}
	else
	{
		img = _from->img;
		mIsImageChange = false;
	}

	if (mIsSameFrameFlag)
	{
		mIsImageChange = true;
		mIsSameFrameFlag = false;
	}

	this->z = _from->z;

	if (_to->offR) {
		dR = _to->rotation + _to->offR * 360 - sR;
	}else {
		dR = _to->rotation - sR;
	}
	
	// ÐÞ¸´¶ªÖ¡µÄbug
	visible = _from->visible;// && _to->visible;

	if( !_to->visible ){
		dR = dX = dY = dSX = dSY = dSKX = dSKY = dA = 0;
		return;
	}  


	dX = _to->x - sX;	
	dY = _to->y - sY;	
	dSX = _to->scaleX - sSX;
	dSY = _to->scaleY - sSY;
	dSKX = _to->skewX - sSKX;
	dSKY = _to->skewY - sSKY;
	dA = _to->alpha - sA;

	dSKX = fmod(dSKX,360.0f);
	if (fabs(dSKX) > 180)
	{
		if (dSKX > 0)
		{
			dSKX = dSKX - 360;
		}
		else
		{
			dSKX = dSKX + 360;
		}
	}

	dSKY = fmod(dSKY,360.0f);
	if (fabs(dSKY) > 180)
	{
		if (dSKY > 0)
		{
			dSKY = dSKY - 360;
		}
		else
		{
			dSKY = dSKY + 360;
		}
	}

	if (mIsFrames)
	{
		dX = dY = dSX = dSY = dSKX = dSKY = dA = 0;
	}	
}


void CCTweenNode::tweenTo(float _k) {
	
	rotation = sR + dR * _k;
	x = sX + dX * _k;
	y = sY + dY * _k;
	skewX = sSKX + dSKX * _k;
	skewY = sSKY + dSKY * _k;

	scaleX = sSX + dSX * _k;
	scaleY = sSY + dSY * _k;
	alpha = sA + dA * _k;

	

	/*if (dSX) {
		scaleX = sSX + dSX * _k;
	}else {
		scaleX = std::numeric_limits<float>::quiet_NaN();
	}

	if (dSY) {
		scaleY = sSY + dSY * _k;
	}else {
		scaleY = std::numeric_limits<float>::quiet_NaN();
	}

	if (dA) {
		alpha = sA + dA * _k;
	}else {
		alpha = std::numeric_limits<float>::quiet_NaN();
	}*/
}

void CCTweenNode::remove(){
	reset();
}