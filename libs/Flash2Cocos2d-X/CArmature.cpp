#include "AppDelegate.h"
#include "CArmature.h"
#include "CConnectionData.h"
#include "SASpriteDefine.h"

CCBoneDisplayFactory::~CCBoneDisplayFactory()
{	
	//for(size_t i = 0; i < m_vecDisplay.size(); i++)
	//{
	//	CCSprite* sprite = *(m_vecDisplay[i]);
	//	if (sprite)
	//	{
	//		sprite->release();
	//	}
	//}
	//m_vecDisplay.clear();
}


void CCBoneDisplayFactory::RecordBoneDisplay(CCSprite** sprite)
{
	//m_vecDisplay.push_back(sprite);
}


CCArmature *CCArmature::create(const char* _name, 
	const char* _animationName,
	CCSpriteBatchNode *_batchNode, 
	int _index, 
	bool _isRadian, 
	const char* _eachImageSuffix) {
    //CCXLOG("Enter CCArmature::createArmature");

    CCArmature *_armature = new CCArmature();
    if (_armature && _armature->init(_name, _animationName, _batchNode, _index, _isRadian, _eachImageSuffix)) {
        _armature->autorelease();
        return _armature;
    }
    CC_SAFE_DELETE(_armature);
    return NULL;

    //CCXLOG("Finish CCArmature::createArmature");
}


CCSprite* CCArmature::createBoneDisplay(const char* _boneName) {
	std::string _boneNameStr = _boneName;
	_boneNameStr.append(mEachImageSuffix);
	return getTextureDisplay(_boneNameStr.c_str() );
}

CCSprite* CCArmature::getTextureDisplay(const char* _boneName) {
    CCSprite *_display = CCSprite::createWithSpriteFrameName(_boneName);
	
	/// name current bone sprite
	if (_display)
	{
		_display->m_name = _boneName;
	}else
	{
		CCError("CCArmature::getTextureDisplay error! %s", _boneName);
	}

    return _display;
}

CCSprite* CCArmature::createArmatureDisplay(const char* _armatureName) {
    return CCSprite::create();
}




CCArmature::CCArmature() {
    mAnimation				= NULL;
    mDisplay				= NULL;
    mIsRadian				= false;
    mArmaureDisplayFactory	= NULL;
    mBoneDisplayFactory		= NULL;
	mBoneDic				= NULL;
	mBoneList				= NULL;
	mDisPlayBatchNode		= NULL;
	mEachImageSuffix		= SKELETON_IMAGE_FILE_SUFFIX;
	mbUpdate = true;
}


CCArmature::~CCArmature(void) {
	remove();
	CC_SAFE_DELETE(mBoneDic);
	CC_SAFE_DELETE(mBoneList);
	if (mCurBoneVec)
	{
		mCurBoneVec->clear();
		CC_SAFE_DELETE(mCurBoneVec);
	}
	if (mNotVisibleBoneVec)
	{
		mNotVisibleBoneVec->clear();
		CC_SAFE_DELETE(mNotVisibleBoneVec);
	}
	unscheduleUpdate();
}

void CCArmature::clearAllData()
{

}

void CCArmature::getCurBoneLst()
{
	clearCurBoneLst();

	CCObject *_object = NULL;
	CCARRAY_FOREACH(mBoneList, _object){
		CCBone *_bone = (CCBone*)_object;
		if (_bone->getTweenNode()->active)
		{
			mCurBoneVec->push_back(_bone);
		}
		else
		{
			mNotVisibleBoneVec->push_back(_bone);
		}
	}
}

void CCArmature::clearCurBoneLst()
{	
	if (mCurBoneVec)
	{
		mCurBoneVec->clear();
	}
	if (mNotVisibleBoneVec)
	{
		mNotVisibleBoneVec->clear();
	}
}

bool CCArmature::init(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index, bool _isRadian, const char* _eachImageSuffix) 
{   
	CCObject::m_name = _name;
	CCObject::m_name = CCObject::m_name + _animationName;
	
	bool bRet = false;
    do {

		// init attributes
        mAnimation	= CCArmatureAnimation::create();
        CCAssert(mAnimation, "create CCArmature::mAnimation fail!");
        addChild(mAnimation);

		mIsRadian	= _isRadian;

		mBoneDic	= new CCDictionary();
		CCAssert(mBoneDic, "create CCArmature::mBoneDic fail!");

		mBoneList	= new CCArray();
		CCAssert(mBoneList->init(), "create CCArmature::boneList fail!");

		mCurBoneVec	= new std::vector<CCBone*>();
		mNotVisibleBoneVec = new std::vector<CCBone*>();

        mEachImageSuffix = _eachImageSuffix;

		mDisPlayBatchNode = _batchNode;
		mDisPlayBatchNode->retain();

		mName = _name;
		

		// create armature display , which is the top parent of the bone display
        if( !mArmaureDisplayFactory ) {
            mArmaureDisplayFactory = this;
        }

        do {
            mDisplay = mArmaureDisplayFactory->createArmatureDisplay(_name);

			mDisplay->m_name = _name;


            CCAssert(mDisplay, "create CCArmature::init::mDisplay fail.");
			if( _batchNode ){
				if( static_cast<CCSprite*>(mDisplay) ){
					CCSprite *_sprite = (CCSprite*)mDisplay;
					_sprite->initWithTexture(mDisPlayBatchNode->getTexture(), CCRectZero);
				}

				mDisPlayBatchNode->addChild(mDisplay, _index);
			}

        } while (0);


        CCArmatureAniData* _animationData = CCConnectionData::sharedConnectionData()->getAnimationData(_animationName);
		if (_animationData == NULL)
		{
			SkeletonAnimRcsManager::getInstance()->LoadOneRcs(_animationName);
		}
        CCAssert(_animationData, "CCArmatureAniData not exist! ");
		mAnimation->setData(_animationData);


		CCArray *_armatureDatas = CCConnectionData::sharedConnectionData()->getArmatureData(_name);
		CCAssert(_armatureDatas->count()>0, "_armatureData->count() should not <=0");
		
		/// create bones 
		if (false == createBones(_armatureDatas))
			return false;
        
        scheduleUpdate();		
		pauseAllAnimation();
		mDisPlayBatchNode->setVisible(false);
		mDisplay->setVisible(false);

		setArmaturetOpacity(255);

        bRet = true;		
    } while (0);

    return bRet;
}

bool CCArmature::createBones(CCArray *_armatureDatas)
{

	CCBone			*_bone;
	PCCArmatureData _boneData;
	const char		*_boneName;
	const char		*_parentName;
	const char		*_imageName;
	CCSprite		*_boneDisplay;
	int				_indexZ;
	int				_length = _armatureDatas->count();

	for(int indexI = 0; indexI < _length; indexI++) {
		_boneData = (PCCArmatureData)_armatureDatas->objectAtIndex(indexI);
		_boneName = _boneData->name.c_str();
		_parentName = _boneData->parent.c_str();
		_imageName = _boneData->boneImage.c_str();
		_indexZ = _boneData->z;

		if( !mBoneDisplayFactory ) 
		{
			mBoneDisplayFactory = this;
		}

		do 
		{
			_boneDisplay = mBoneDisplayFactory->createBoneDisplay(_imageName);
			mBoneDisplayFactory->RecordBoneDisplay(&_boneDisplay);
			CCAssert(_boneDisplay, "create CCArmature::init::_boneDisplay fail.");
			if (NULL == _boneDisplay)
				return false;
		} while (0);

        std::string name = std::string(_imageName);
		_bone = addBone(_boneName, _parentName, _boneDisplay, _indexZ);
        _bone->setImageName(name);

		float _anchorPointX = _boneData->pivotX / _boneData->width;
		float _anchorPointY = (_boneData->height - _boneData->pivotY) / _boneData->height;

		if (_bone->getDisplay())
			_bone->getDisplay()->setAnchorPoint(ccp(_anchorPointX, _anchorPointY));

        // client version
        float f = 1.0f;
        std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();
        if (strcmp(clientVersion.c_str(), "hq") == 0)
        {
            // 根据资源来匹配
            if (name.find("swordswoman") == 0 ||
                name.find("swordsman") == 0 ||
                name.find("mage") == 0 ||
                name.find("mageman") == 0 ||
                name.find("assassinwoman") == 0 ||
                name.find("assassin") == 0 ||
                name.find("skill") == 0)
            {
                f = HQ_SKELETON_CHARACTER;
            } else if (name.find("npc") == 0)
            {
                f = HQ_SKELETON_NPC;
            } else if (name.find("monster") == 0)
            {
                f = HQ_SKELETON_MONSTER;
            } else if (name.find("fairy") == 0)
            {
                f = HQ_SKELETON_FAIRY;
            } else if (name.find("effect") == 0)
            {
                f = HQ_SKELETON_EFFECT;
            }
        } else if (strcmp(clientVersion.c_str(), "nq") == 0)
        {
            f = NQ_SKELETON;
        }

        _bone->setLockPosition( _boneData->x * f, _boneData->y * f, _boneData->skewX, _boneData->skewY);
    }

	resetBonesParent();

	return true;
}

void CCArmature::resetBonesParent()
{
	CCDictElement *_element = NULL;
	CCDICT_FOREACH(mBoneDic, _element){
		CCBone *_bone = (CCBone *)_element->getObject();
		CCBone *_boneParent = (CCBone *)mBoneDic->objectForKey(_bone->getParentName());
		if(_boneParent) {
			_bone->setParent(_boneParent);
		}
	}
}


void CCArmature::update(float dt) 
{
	if (mbAnimationPause)
	{
		if (this->getDisplay()->isVisible())
		{
			this->getDisPlayBatchNode()->setVisible(false);
			this->getDisplay()->setVisible(false);
		}
		return ;
	}
	if(false == this->getDisplay()->isVisible())
	{
		this->getDisPlayBatchNode()->setVisible(true);
		this->getDisplay()->setVisible(true);
	}

	mAnimation->update( dt );

	if (mNotVisibleBoneVec->size() != 0)
	{
		for (std::vector<CCBone*>::iterator iter = mNotVisibleBoneVec->begin();iter != mNotVisibleBoneVec->end(); iter++)
		{
			CCBone *_bone = (CCBone*)(*iter);
			if (_bone->getDisplay())
				_bone->getDisplay()->setVisible(false);
		}
		mNotVisibleBoneVec->clear();
	}

	bool bSingleAnimationOver = mAnimation->GetSingleAnimationOverOrNot();
	bSingleAnimationOver = false;
	if (mbUpdate && false == bSingleAnimationOver)
	{
		for (std::vector<CCBone*>::iterator iter = mCurBoneVec->begin();iter != mCurBoneVec->end(); iter++)
		{
			CCBone *_bone = (CCBone*)(*iter);
			mAnimation->updateTween(_bone->getTween(), dt);				
			_bone->update( dt );
		}
	}	

	mAnimation->SendAnimationHandleMessage();

	if (mbUpdate == false)
	{
		if (this->getDisplay()->isVisible())
		{
			this->getDisPlayBatchNode()->setVisible(false);
			this->getDisplay()->setVisible(false);
		}
	}
	else
	{
		if (false == this->getDisplay()->isVisible())
		{
			this->getDisPlayBatchNode()->setVisible(true);
			this->getDisplay()->setVisible(true);
		}
	}

	//if (bSingleAnimationOver)
	//{
	//	if (this->getDisplay()->isVisible())
	//	{
	//		this->getDisPlayBatchNode()->setVisible(false);
	//		this->getDisplay()->setVisible(false);
	//	}
	//}
}

void CCArmature::remove() {

	CCObject *_object = NULL;
	CCBone *_bone;
	CCARRAY_FOREACH(mBoneList, _object){
		_bone = (CCBone*)_object;
		CCSprite* pSprite = (CCSprite*)_bone->getDisplay();
		pSprite->getParent()->removeChild(pSprite,true);
		_bone->remove();
	}

	mAnimation->remove();
    mAnimation = NULL;

	if( mDisPlayBatchNode ){
		mDisPlayBatchNode->removeChild(mDisplay, true);
		mDisPlayBatchNode->release();
	}
	mDisPlayBatchNode = NULL;
	mDisplay = NULL;

    mBoneDic->removeAllObjects();
    mBoneList->removeAllObjects();
}


CCBone* CCArmature::addBone(const char* _name, const char* _parentName, CCNode* _display, int _index) {
	
    CCBone *_bone = (CCBone*)mBoneDic->objectForKey(_name);
    if(!_bone) {
        _bone = CCBone::create(mIsRadian,this);
        _bone->setName(_name);		
        mBoneList->addObject(_bone);
        mBoneDic->setObject(_bone, _name);
        mAnimation->addTween(_bone);
		_bone->isBoneFrames();
		_bone->setParentName(_parentName);
    }
	else
	{
		CCLOG("bone exist");
		_bone->reset();
	}

    if(_display) {
        CCNode* _displayOld = _bone->getDisplay();
        _bone->setDisplay(_display);
        if(_displayOld) {
			_displayOld->removeFromParentAndCleanup(true);
            mDisplay->addChild(_display, _index);
        } else if (_index == -1) {
            mDisplay->addChild(_display);
        } else if (_index >= 0) {
            mDisplay->addChild(_display, _index);
        }
    }
    return _bone;
}

CCBone *CCArmature::getBone(const char* _name) {
    return (CCBone*)mBoneDic->objectForKey(_name);
}

void CCArmature::removeBone(const char* _name) {
    CCBone *_bone = (CCBone*)mBoneDic->objectForKey(_name);
    if(_bone) {
        if(_bone->getDisplay() /*&& display->contains(_bone->display)*/) {
            mDisplay->removeChild(_bone->getDisplay(), false);
        }
        mAnimation->removeTween(_bone);
        _bone->remove();
    }
}

// stop
void CCArmature::pauseAllAnimation()
{
	this->getAnimation()->stop();
	mbAnimationPause = true;
}

// resume
void CCArmature::resumeAllAnimation()
{
	this->getAnimation()->resume();
	mbAnimationPause = false;
}

void CCArmature::setArmaturetOpacity(GLubyte var)
{
	//CCLOG("------------------- setArmaturetOpacity ");
	std::vector<CCBone*> vecTmpBones = (*mCurBoneVec);

	bool bCalOpacity = false;
	GLubyte opacity = 0;
	CCObject *_object = NULL;
	CCARRAY_FOREACH(mBoneList, _object){
		CCBone *_bone = (CCBone*)_object;
		CCSprite* _pSprite = (CCSprite*)_bone->getDisplay();
		opacity = _pSprite->getOpacity();
		if (bCalOpacity == false)
		{
			bCalOpacity = true;
			if (var == opacity)
			{
				break;
			}
		}
		(_pSprite)->setOpacity(var);
	}
}

void CCArmature::setArmatureColor(ccColor3B color)
{
	CCObject *_object = NULL;
	CCARRAY_FOREACH(mBoneList, _object){
		CCBone *_bone = (CCBone*)_object;
		((CCSprite*)_bone->getDisplay())->setColor(color);
	}
}