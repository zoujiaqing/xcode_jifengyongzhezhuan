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
#ifndef iSeer_UILabelAtlas_h
#define iSeer_UILabelAtlas_h

#include "TXGUI.h"
#include "UILayerColorBase.h"
#include "UIControlBase.h"

namespace TXGUI {
	USING_NS_CC;

	class UILabelAtlas : public UIControlBase
	{       
		NiDeclareRTTI;
		NiDeclareStream;  
	public:
		UILabelAtlas(const char *label,
			const char *atlasFile,
			int itemWith,
			int itemHeight,
			unsigned int startChar, 
			cocos2d::CCNode* parent);

		UILabelAtlas(cocos2d::CCLabelAtlas *pLabel, cocos2d::CCNode *parent);

		UILabelAtlas(std::string name, int tag, float z, UIControlBase *parent, CCLabelAtlas *label);

		virtual   ~UILabelAtlas();

		void setString(const char *label);

		const char* getString();

		virtual void setPosition(CCPoint pt);

		virtual void setLabelAtlas(CCLabelAtlas *pLabel);

		virtual void setScale(float scale);

		virtual void setScale(float xScale, float yScale);

	protected:
		UILabelAtlas();

	protected:        
		CCLabelAtlas *m_pAtlasLabel;
	};
}


#endif
