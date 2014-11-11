#ifndef iSeer_CCScale9Sprite_h
#define iSeer_CCScale9Sprite_h

#include "cocos2d.h"

#define DEFAULT_CUT 5

namespace cocos2d 
{
	class CCScale9Sprite : 	public CCSprite
	{
	private:
		CCSpriteBatchNode *m_pScale9Image;
		CCSprite *m_pTopLeft;
		CCSprite *m_pTop;
		CCSprite *m_pTopRight;
		CCSprite *m_pLeft;
		CCSprite *m_pCenter;
		CCSprite *m_pRight;
		CCSprite *m_pBottomLeft;
		CCSprite *m_pBottom;
		CCSprite *m_pBottomRight;

		float m_fScaleX;
		float m_fScaleY;
		
		float m_fWidthCut;
		float m_fHeightCut;

		CCSize m_tOriginSize;

	public:
		/**
		* create a scale9Sprite with file name
		* using default cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithFile(const char *file);
		/**
		* create a scale9Sprite with file name
		* using widthDelta and heightDelta as the cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithFile(const char *file, int widthDelta, int heightDelta);
		/**
		* create a scale9Sprite with a texture
		* using whole texture
		* using default cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithTexture(CCTexture2D *texture);
		/**
		* create a scale9Sprite with a texture
		* using the rect of texture
		* using default cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithTexture(CCTexture2D *texture, CCRect rect);
		/**
		* create a scale9Sprite with a texture
		* using the rect of texture
		* using widthDelta and heightDelta as the cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithTexture(CCTexture2D *texture, CCRect rect, int widthDelta, int heightDelta);
		/**
		* create a scale9Sprite with a sprite
		* using default cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithSprite(CCSprite *sprite);
		/**
		* create a scale9Sprite with a sprite
		* using widthDelta and heightDelta as the cut parameter
		*/
		static CCScale9Sprite *scale9SpriteWithSprite(CCSprite *sprite, int widthDelta, int heightDelta);

	public:
		virtual void draw();

	public:
		CCScale9Sprite();
		~CCScale9Sprite();

		virtual bool init();
		bool initWithFile(const char *file);
		bool initWithFile(const char *file, int widthDelta, int heightDelta);
		bool initWithTexture(CCTexture2D *texture);
		bool initWithTexture(CCTexture2D *texture, CCRect rect);
		bool initWithTexture(CCTexture2D *texture, CCRect rect, int widthDelta, int heightDelta);
		bool initWithSprite(CCSprite *sprite);
		bool initWithSprite(CCSprite *sprite, int widthDelta, int heightDelta);

		virtual void setScale(float scale);
		virtual void setScaleX(float var);
		virtual void setScaleY(float var);

	protected:
		// not allowed to change content size
		virtual void setContentSize(const CCSize &size);

	private:
		void rearrangePosition();

	};
}
#endif // iSeer_CCScale9Sprite_h