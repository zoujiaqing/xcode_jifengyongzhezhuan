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

#include "UILayoutRenderNode.h"
#include "cocos2d.h"

using namespace TXGUI;

void UILayoutRenderNode::visit()
{
	cocos2d::CCSize size = CCDirector::sharedDirector()->getWinSize();
	cocos2d::CCPoint cameraOffset = CCDirector::sharedDirector()->getLevelRenderCameraOffset();
	float zeye = CCDirector::sharedDirector()->getZEye();
	
	// quick return if not visible
	if (!m_bVisible)
	{
		return;
	}
	kmGLPushMatrix();

	//TODO: don't need change camera everytime
	//it costs much time
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();
/*
	gluLookAt( size.width/2 + cameraOffset.x, size.height/2 + cameraOffset.y, zeye,
		size.width/2 + cameraOffset.x, size.height/2 + cameraOffset.y, 0,
		0.0f, 1.0f, 0.0f);*/

	kmVec3 eye, center, up;
	kmVec3Fill( &eye,  size.width/2 + cameraOffset.x, size.height/2 + cameraOffset.y, zeye);
	kmVec3Fill( &center, size.width/2 + cameraOffset.x, size.height/2 + cameraOffset.y, 0 );
	kmVec3Fill( &up, 0.0f, 1.0f, 0.0f);

	kmMat4 matrixLookup;
	kmMat4LookAt(&matrixLookup, &eye, &center, &up);
	kmGLMultMatrix(&matrixLookup);


 	if (m_pGrid && m_pGrid->isActive())
 	{
 		m_pGrid->beforeDraw();
 		this->transformAncestors();
 	}

	this->transform();

    CCNode* pNode = NULL;
    unsigned int i = 0;

	if(m_pChildren && m_pChildren->count() > 0)
	{
		// draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];

			if ( pNode && pNode->getZOrder() < 0 ) 
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
    }

	// self draw
	this->draw();

	// draw children zOrder >= 0
    if (m_pChildren && m_pChildren->count() > 0)
    {
        ccArray *arrayData = m_pChildren->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            if (pNode)
            {
                pNode->visit();
            }
		}		
	}

 	if (m_pGrid && m_pGrid->isActive())
 	{
 		m_pGrid->afterDraw(this);
	}
 
	kmGLPopMatrix();
}