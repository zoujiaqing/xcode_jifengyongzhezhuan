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
#ifndef iSeer_UIContainer_h
#define iSeer_UIContainer_h

#include "TXGUI.h"
#include "UIControlBase.h"
#include "NiStream.h"

namespace TXGUI
{
	USING_NS_CC;

	typedef enum 
	{
		NONE = 0,
		LEFT_TOP,
		LEFT_MIDDLE,
		LEFT_BOTTOM,
		MIDDLE_TOP,
		MIDDLE_MIDDLE,
		MIDDLE_BOTTOM,
		RIGHT_TOP,
		RIGHT_MIDDLE,
		RIGHT_BOTTOM
	} ReferencePoint;

	typedef enum 
	{
		NO_REFERENCE = 0,
		NORMAL_REFERENCE,
		OBJECT_REFERENCE
	} ReferenceMode;

	// a controllers container
	// always take center point as the calculation point
	class UIContainer : public UIControlBase
	{
		NiDeclareRTTI;
		NiDeclareStream;

	public:
		UIContainer(CCNode *pParentNode);
		virtual ~UIContainer();

		// position
		virtual void setPosition(CCPoint pt);

		virtual void UpdateWorldPosition();

		// scale
		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

		// reference mode
		virtual void setRefMode(ReferenceMode refMode);
		virtual ReferenceMode getRefMode();

		// reference point
		virtual void setRefPointMode(ReferencePoint refPtMode);
		virtual ReferencePoint getRefPointMode();

		// reference position
		virtual void setWidthScale(float scale);
		virtual float getWidthScale();

		virtual void setWidthOffset(float value);
		virtual float getWidthOffset();

		virtual void setHeightScale(float scale);
		virtual float getHeightScale();

		virtual void setHeightOffset(float value);
		virtual float getHeightOffset();

		// container size
		virtual void setSize(CCSize size);
		virtual void setSize(int width, int height);
		virtual CCSize getSize();

		// reference object
		virtual void setRefObjName(std::string name);
		virtual std::string getRefObjName();

		virtual void setRefObj(UIContainer *refObj);
		virtual UIContainer *getRefObj();

		// calculate position
		/** calculate world position in window size*/
		virtual void CalWorldPos();
		/** calculate world position in 1024x768*/
		virtual void preCalWorldPos();
		virtual void CalWorldPosWithSize(CCSize size);
		virtual void CalLocalPos();
		/** get center position under targetWinSize*/
		CCPoint containerCenterPosition(CCSize targetWinSize);

		// update children controllers position
		virtual void UpdateChildrenPos();

	protected:
		// for streaming
		UIContainer();

	protected:
		ReferenceMode m_refMode;
		ReferencePoint m_refPointMode;

		float m_widthScale;
		float m_widthOffset;
		float m_heightScale;
		float m_heightOffset;

		CCSize m_size;

		// the object referenced
		std::string m_refObjName;
		UIContainer *m_refObj;
	};
	
}

#endif //iSeer_UIContainer_h