// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#ifndef iSeer_UIScrollView_h
#define iSeer_UIScrollView_h

#include "UILayerColorBase.h"
#include "cocos2d.h"

namespace TXGUI{
    
    typedef enum {
        SWUIScrollViewDirectionHorizontal,
        SWUIScrollViewDirectionVertical,
        SWUIScrollViewDirectionBoth
    } SWUIScrollViewDirection;

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.35f
#define INSET_RATIO          0.1f
    
    class UIScrollView;
    
    class UIScrollViewDelegate
    {
    public:
        virtual void scrollViewDidScroll(UIScrollView *view){}
        virtual void scrollViewDidZoom(UIScrollView *view){}
    };
    
    typedef cocos2d::CCArray TouchArray;
    
    
    
    ///
    // class UIScrollView
    //
    class UIScrollView: public UILayerColorBase
    {
    public:
        UIScrollView(bool touchEnable, bool schedule, ccColor4B color);
        UIScrollView();
        
        ~UIScrollView();
        
        /**
         * Returns an autoreleased scroll view object.
         *
         * @param size view size
         * @return autoreleased scroll view object
         */
        static UIScrollView *viewWithViewSize(cocos2d::CCSize size);
        
        /**
         * Returns an autoreleased scroll view object.
         *
         * @param size view size
         * @param container parent object
         * @return autoreleased scroll view object
         */
        static UIScrollView *viewWithViewSize(cocos2d::CCSize size,cocos2d::CCNode* container);
        
        /**
         * Returns a scroll view object
         *
         * @param size view size
         * @return scroll view object
         */
		void setisScrollList(bool val){isScrollList = val ;}
        bool initWithViewSize(cocos2d::CCSize size);
        
        /**
         * Returns a scroll view object
         *
         * @param size view size
         * @param container parent object
         * @return scroll view object
         */
        bool initWithViewSize(cocos2d::CCSize size,cocos2d::CCNode* container);
        
        
        // default implements are used to call script callback if exist
        virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent* event);
        virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        
        void registerWithTouchDispatcher();
        
        
        void setContentOffset(cocos2d::CCPoint offset);
        void setContentOffset(cocos2d::CCPoint offset,bool animated);
        void setContentOffset(cocos2d::CCPoint offset,float dt );
        void setZoomScale(float s);
        void setZoomScale(float s,bool animated);
        void setZoomScale(float s,float dt);
        float getZoomScale();
        void setViewSize(cocos2d::CCSize size);
		inline cocos2d::CCSize getViewSize() { return viewSize_;}
        
        inline void setDirection(SWUIScrollViewDirection d) {direction_ = d;}
                
        CC_SYNTHESIZE(UIScrollViewDelegate*, m_pDelegate, Delegate)
        CC_SYNTHESIZE(float,minScale_,MinScale);
        CC_SYNTHESIZE(float,maxScale_,MaxScale);
        CC_SYNTHESIZE(int,m_bPriority,Priority);
        cocos2d::CCSize getContentSize();
        
        virtual void setContentSize(cocos2d::CCSize size);

		virtual void setPosition(cocos2d::CCPoint pt);

		virtual void setContainerPos(cocos2d::CCPoint pt);

		virtual cocos2d::CCPoint getContainerPos();

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);
        
        virtual void addChild(cocos2d::CCNode *node,int z=0,int aTag=0);
        
        
        /**
         * clip this view so that outside of the visible bounds can be hidden.
         */
        virtual void beforeDraw();
        /**
         * retract what's done in beforeDraw so that there's no side effect to
         * other nodes.
         */
        virtual void afterDraw();
        /**
         * Zoom handling
         */
        //    virtual void handleZoom();
        
        virtual void visit();
        
        void setIsTouchEnabled(bool e);
        void claimTouch(cocos2d::CCTouch* pTouch);
        void cancelAndStoleTouch(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
		void sendTouchToOther(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
		void endOtherTouch(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
		void checkChildrenPos(); //by benyang: check if the child node is in view

		// 注册划动到底部的通知对象
		void registerScrollMaxCallback(CCObject* target,SEL_CallFunc handler);
		void setCanReceiveScrollCallback(bool isReceive);
    private:
        cocos2d::CCPoint getMaxContainerOffset();
        cocos2d::CCPoint getMinContainerOffset();
        void stoppedAnimatedScroll(cocos2d::CCNode *node);
        void performedAnimatedScroll(float dt);
        void deaccelerateScrolling(float dt);
        void relocateContainer(bool animated);   
        
		void checkScrollMax(float newY,float maxY);
    private:
        /**
         * Container holds scroll view contents
         */
		bool isScrollList ;
        cocos2d::CCNode *container_;
        /**
         * Determiens whether user touch is moved after begin phase.
         */
        bool touchMoved_;
        /**
         * Determiens whether the touch should be swallowed.
         */
        bool shouldSwallowTouch_;
        /**
         * max inset point to limit scrolling by touch
         */
        cocos2d::CCPoint maxInset_;
        /**
         * min inset point to limit scrolling by touch
         */
        cocos2d::CCPoint minInset_;
        /**
         * If YES, touches are being moved
         */
        bool isDragging_;
        /**
         * Determines whether the scroll view is allowed to bounce or not.
         */
        bool bounces_;
        /**
         * Determines whether it clips its children or not.
         */
        bool clipsToBounds_;
        /**
         * scroll speed
         */
        cocos2d::CCPoint scrollDistance_;
        /**
         * Touch point
         */
        cocos2d::CCPoint touchPoint_;
		 /**
         * Touch begin point
         */
		cocos2d::CCPoint touchBeginPoint_;
        /**
         * length between two fingers
         */
        float touchLength_;
        /**
         * UITouch objects to detect multitouch
         */
        TouchArray *touches_;
        /**
         * size to clip. CCNode boundingBox uses contentSize directly.
         * It's semantically different what it actually means to common scroll views.
         * Hence, this scroll view will use a separate size property.
         */
        cocos2d::CCSize viewSize_;
		/**
		 * actual center point in world scale
		 */
		cocos2d::CCPoint worldPosition_;
        /**
         * scroll direction
         */
        SWUIScrollViewDirection direction_;
        /**
         * delegate to respond to scroll event
         */
        //    id<SWScrollViewDelegate> delegate_;
        /**
         * max and min scale
         */
        //    float minScale_, maxScale_;
        
        UIScrollViewDelegate *delegate_;

		// 界面切换时回调通知
		SEL_CallFunc m_ScrollMaxHandler;
		CCObject*	 m_ScrollMaxTarget;
		bool		 m_isReceiveScrollMsg; // 滑动到底时是否响应回调
    };

}

#endif
