#include "CCScale9Sprite.h"

namespace cocos2d
{
	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithFile(const char* file) {
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithFile(file))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithFile(const char *file, int widthDelta, int heightDelta)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithFile(file, widthDelta, heightDelta))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithTexture(CCTexture2D *texture)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithTexture(texture))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithTexture(CCTexture2D *texture, CCRect rect)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithTexture(texture, rect))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithTexture(CCTexture2D *texture, CCRect rect, int widthDelta, int heightDelta)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithTexture(texture, rect, widthDelta, heightDelta))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithSprite(CCSprite *sprite)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithSprite(sprite))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite *CCScale9Sprite::scale9SpriteWithSprite(CCSprite *sprite, int widthDelta, int heightDelta)
	{
		CCScale9Sprite *pRet = new CCScale9Sprite();
		if(pRet && pRet->initWithSprite(sprite, widthDelta, heightDelta))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_RELEASE(pRet);
		return NULL;
	}

	CCScale9Sprite::CCScale9Sprite()
	{
		init();
	}

	CCScale9Sprite::~CCScale9Sprite()
	{

	}

	bool CCScale9Sprite::init()
	{
		m_pScale9Image = NULL;
		m_pTopLeft = NULL;
		m_pTop = NULL;
		m_pTopRight = NULL;
		m_pLeft = NULL;
		m_pCenter = NULL;
		m_pRight = NULL;
		m_pBottomLeft = NULL;
		m_pBottom = NULL;
		m_pBottomRight = NULL;
		m_fScaleX = m_fScaleY = 1.0f;
		m_fWidthCut = m_fHeightCut = (float)DEFAULT_CUT;
		return true;
	}

	bool CCScale9Sprite::initWithFile(const char *file)
	{
		assert(file != NULL);

		return initWithFile(file, DEFAULT_CUT, DEFAULT_CUT);
	}

	bool CCScale9Sprite::initWithFile(const char *file, int widthDelta, int heightDelta)
	{
		assert(file != NULL);
		assert(widthDelta >= 0);
		assert(heightDelta >= 0);

		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(file);
		if(pTexture)
		{
			CCRect rect = CCRectZero;
			rect.size = pTexture->getContentSize();
			return initWithTexture(pTexture, rect, widthDelta, heightDelta);
		}

		return false;
	}

	bool CCScale9Sprite::initWithTexture(CCTexture2D *texture)
	{
		assert(texture != NULL);

		CCRect rect = CCRectZero;
		rect.size = texture->getContentSize();
		return initWithTexture(texture, rect);
	}

	bool CCScale9Sprite::initWithTexture(CCTexture2D *texture, CCRect rect)
	{
		assert(texture != NULL);

		return initWithTexture(texture, rect, DEFAULT_CUT, DEFAULT_CUT);
	}

	bool CCScale9Sprite::initWithTexture(CCTexture2D *texture, CCRect rect, int widthDelta, int heightDelta)
	{
		assert(texture != NULL);
		assert(widthDelta >= 0);
		assert(heightDelta >= 0);

		bool bRet = false;

		do
		{
			m_tOriginSize = rect.size;
			this->setContentSize(rect.size);
			m_fWidthCut = (float)widthDelta;
			m_fHeightCut = (float)heightDelta;

			m_pScale9Image = CCSpriteBatchNode::createWithTexture(texture, 9);
			CC_BREAK_IF(m_pScale9Image == NULL);

			// calculate the 9 rectangles
			// in image (0, 0) is the left top corner
			CCRect topLeftRect = CCRectMake(
				rect.origin.x,
				rect.origin.y,
				widthDelta,
				heightDelta
				);

			CCRect topRect = CCRectMake(
				rect.origin.x + widthDelta,
				rect.origin.y,
				rect.size.width - widthDelta * 2,
				heightDelta
				);

			CCRect topRightRect = CCRectMake(
				rect.origin.x + rect.size.width - widthDelta,
				rect.origin.y,
				widthDelta,
				heightDelta
				);

			CCRect leftRect = CCRectMake(
				rect.origin.x,
				rect.origin.y + heightDelta,
				widthDelta,
				rect.size.height - heightDelta * 2
				);

			CCRect centerRect = CCRectMake(
				rect.origin.x + widthDelta,
				rect.origin.y + heightDelta,
				rect.size.width - widthDelta * 2,
				rect.size.height - heightDelta * 2
				);

			CCRect rightRect = CCRectMake(
				rect.origin.x + rect.size.width - widthDelta,
				rect.origin.y + heightDelta,
				widthDelta,
				rect.size.height - heightDelta * 2
				);

			CCRect bottomLeftRect = CCRectMake(
				rect.origin.x,
				rect.origin.y + rect.size.height - heightDelta,
				widthDelta,
				heightDelta
				);

			CCRect bottomRect = CCRectMake(
				rect.origin.x + widthDelta,
				rect.origin.y + rect.size.height - heightDelta,
				rect.size.width - widthDelta * 2,
				heightDelta
				);

			CCRect bottomRightRect = CCRectMake(
				rect.origin.x + rect.size.width - widthDelta,
				rect.origin.y + rect.size.height - heightDelta,
				widthDelta,
				heightDelta
				);

			// create 9 sprite with the batch node
			m_pTopLeft = CCSprite::spriteWithBatchNode(m_pScale9Image, topLeftRect);
			CC_BREAK_IF(m_pTopLeft == NULL);

			m_pTop = CCSprite::spriteWithBatchNode(m_pScale9Image, topRect);
			CC_BREAK_IF(m_pTop == NULL);

			m_pTopRight = CCSprite::spriteWithBatchNode(m_pScale9Image, topRightRect);
			CC_BREAK_IF(m_pTopRight == NULL);

			m_pLeft = CCSprite::spriteWithBatchNode(m_pScale9Image, leftRect);
			CC_BREAK_IF(m_pLeft == NULL);

			m_pCenter = CCSprite::spriteWithBatchNode(m_pScale9Image, centerRect);
			CC_BREAK_IF(m_pCenter == NULL);

			m_pRight = CCSprite::spriteWithBatchNode(m_pScale9Image, rightRect);
			CC_BREAK_IF(m_pRight == NULL);

			m_pBottomLeft = CCSprite::spriteWithBatchNode(m_pScale9Image, bottomLeftRect);
			CC_BREAK_IF(m_pBottomLeft == NULL);

			m_pBottom = CCSprite::spriteWithBatchNode(m_pScale9Image, bottomRect);
			CC_BREAK_IF(m_pBottom == NULL);

			m_pBottomRight = CCSprite::spriteWithBatchNode(m_pScale9Image, bottomRightRect);
			CC_BREAK_IF(m_pBottomRight == NULL);

			m_pScale9Image->addChild(m_pTopLeft, 2);
			m_pScale9Image->addChild(m_pTop, 1);
			m_pScale9Image->addChild(m_pTopRight, 2);
			m_pScale9Image->addChild(m_pLeft, 1);
			m_pScale9Image->addChild(m_pCenter, 0);
			m_pScale9Image->addChild(m_pRight, 1);
			m_pScale9Image->addChild(m_pBottomLeft, 2);
			m_pScale9Image->addChild(m_pBottom, 1);
			m_pScale9Image->addChild(m_pBottomRight, 2);

			this->addChild(m_pScale9Image, 0);
			// update position
			this->rearrangePosition();

			bRet = true;
		} while (0);

		return bRet;
	}

	bool CCScale9Sprite::initWithSprite(CCSprite *sprite)
	{
		assert(sprite != NULL);

		return initWithSprite(sprite, DEFAULT_CUT, DEFAULT_CUT);
	}

	bool CCScale9Sprite::initWithSprite(CCSprite *sprite, int widthDelta, int heightDelta)
	{
		assert(sprite != NULL);
		assert(widthDelta >= 0);
		assert(heightDelta >= 0);

		CCTexture2D *texture = sprite->getTexture();
		CCRect rect = sprite->getTextureRect();

		return initWithTexture(texture, rect, widthDelta, heightDelta);
	}

	void CCScale9Sprite::setScale(float scale)
	{
		m_fScaleX = scale;
		m_fScaleY = scale;

		CCSize newContentSize = CCSizeMake(m_tOriginSize.width * m_fScaleX, m_tOriginSize.height * m_fScaleY);
		this->setContentSize(newContentSize);

		// calculate the corresponding scale
		float scaleX = (newContentSize.width - m_fWidthCut * 2) / (m_tOriginSize.width - m_fWidthCut * 2);
		float scaleY = (newContentSize.height - m_fHeightCut * 2) / (m_tOriginSize.height - m_fHeightCut * 2);

		// set scale of the corresponding sprites
		m_pTop->setScaleX(scaleX);
		m_pBottom->setScaleX(scaleX);

		m_pLeft->setScaleY(scaleY);
		m_pRight->setScaleY(scaleY);

		m_pCenter->setScaleX(scaleX);
		m_pCenter->setScaleY(scaleY);

		// update the position of the 9 sprites
		this->rearrangePosition();
	}

	void CCScale9Sprite::setScaleX(float var)
	{
		m_fScaleX = var;

		CCSize newContentSize = CCSizeMake(m_tOriginSize.width * m_fScaleX, m_tOriginSize.height * m_fScaleY);
		this->setContentSize(newContentSize);

		// calculate the corresponding scale
		float scaleX = (newContentSize.width - m_fWidthCut * 2) / (m_tOriginSize.width - m_fWidthCut * 2);

		// set scale of the corresponding sprites
		m_pTop->setScaleX(scaleX);
		m_pCenter->setScaleX(scaleX);
		m_pBottom->setScaleX(scaleX);

		// update the position of the 9 sprites
		this->rearrangePosition();
	}

	void CCScale9Sprite::setScaleY(float var)
	{
		m_fScaleY = var;

		CCSize newContentSize = CCSizeMake(m_tOriginSize.width * m_fScaleX, m_tOriginSize.height * m_fScaleY);
		this->setContentSize(newContentSize);

		// calculate the corresponding scale
		float scaleY = (newContentSize.height - m_fHeightCut * 2) / (m_tOriginSize.height - m_fHeightCut * 2);

		// set scale of the corresponding sprites
		m_pLeft->setScaleY(scaleY);
		m_pCenter->setScaleY(scaleY);
		m_pRight->setScaleY(scaleY);

		// update the position of the 9 sprites
		this->rearrangePosition();
	}

	void CCScale9Sprite::setContentSize(const CCSize &size)
	{
		CCSprite::setContentSize(size);
	}

	void CCScale9Sprite::rearrangePosition()
	{
		CCSize contentSize = this->getContentSize();

		// calculate all 9 sprites' position
		CCPoint topLeft = ccp(m_fWidthCut / 2, contentSize.height - m_fHeightCut / 2);
		CCPoint top = ccp(contentSize.width / 2, contentSize.height - m_fHeightCut / 2);
		CCPoint topRight = ccp(contentSize.width - m_fWidthCut / 2, contentSize.height - m_fHeightCut / 2);
		CCPoint left = ccp(m_fWidthCut / 2, contentSize.height / 2);
		CCPoint center = ccp(contentSize.width / 2, contentSize.height / 2);
		CCPoint right = ccp(contentSize.width - m_fWidthCut / 2, contentSize.height / 2);
		CCPoint bottomLeft = ccp(m_fWidthCut / 2, m_fHeightCut / 2);
		CCPoint bottom = ccp(contentSize.width / 2, m_fHeightCut / 2);
		CCPoint bottomRight = ccp(contentSize.width - m_fWidthCut / 2, m_fHeightCut / 2);

		m_pTopLeft->setPosition(topLeft);
		m_pTop->setPosition(top);
		m_pTopRight->setPosition(topRight);
		m_pLeft->setPosition(left);
		m_pCenter->setPosition(center);
		m_pRight->setPosition(right);
		m_pBottomLeft->setPosition(bottomLeft);
		m_pBottom->setPosition(bottom);
		m_pBottomRight->setPosition(bottomRight);
	}

	void CCScale9Sprite::draw() {
		// override the draw method of CCSprite
		// draw nothing
		CCNode::draw();
	}
}
