//
//  Painter.cpp
//  iSeer
//
//  Created by wuwei on 2/13/12.
//  Copyright 2012 Taomee.Inc. All rights reserved.
//

#include "Painter.h"
#include "CrossUtil.h"
using namespace cocos2d;

#define NUM_SEGMENTS    36                  // number of segments to make up the circle perimeter
#define FILL_COLOR      1.0f,0.0f,1.0f,0.2f // filling color
#define EDGE_COLOR      1.0f,0.0f,1.0f,0.8f // edge color

void Painter::drawCircle(CCPoint c, float r)
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
    // delta radian of each segment
    float dR = float( 2 * M_PI / NUM_SEGMENTS);
    
    // initialize the vertex array
    // number of vertices: 1 center + (NUM_SEGMENT + 1) endpoints
    int numVerts = 1 + (NUM_SEGMENTS + 1);
    float* pVerts = new float[numVerts * 2];
    memset(pVerts, 0, numVerts * 2);
    
    // insert the vertices
    // center and the first endpoint
    pVerts[0] = c.x;
    pVerts[1] = c.y;
    pVerts[2] = c.x + r;
    pVerts[3] = c.y;
    
    // endpoints on the perimeter
    for(int i = 0; i < NUM_SEGMENTS; ++i)
    {
        float radian    = dR * (i+1);
        pVerts[4+i*2]   = c.x + r * cosf(radian);
        pVerts[4+i*2+1] = c.y + r * sinf(radian);
    }
    
    glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
    
    // set blend mode
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // fill circle
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, pVerts);
    ccDrawColor4F(1.0f,0.0f,1.0f,0.2f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts);
    
    // highlight edges
    ccDrawColor4F(1.0f,0.0f,1.0f,0.8f);
    glDrawArrays(GL_LINE_STRIP, 1, numVerts - 1);
    
    // restore default state
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    
    delete[] pVerts;
}

void Painter::drawFan(CCPoint c, float r, float a1, float a2)
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
    // delta radian of each segment
    float dR = float(2 * M_PI / NUM_SEGMENTS);
    float radian1 = a1 * M_PI / 180;
    float radian2 = a2 * M_PI / 180;
    
    // number of segments to make up the fan arc
    int numSegments = ceilf((radian2 - radian1) / dR);
    
    // initialize the vertex array
    // number of vertices: 1 center + (numSegments + 1) endpoints + 1 close point
    int numVerts = 1 + (numSegments + 1) + 1;
    float* pVerts = new float[numVerts * 2];
    memset(pVerts, 0, numVerts * 2);
    
    // insert the vertices
    // center and the first endpoint
    pVerts[0] = c.x;
    pVerts[1] = c.y;
    pVerts[2] = c.x + r * cosf(radian1);
    pVerts[3] = c.y + r * sinf(radian1);
    
    for(int i = 0; i < numSegments; ++i)
    {
        float radian    = (i == numSegments - 1) ? radian2 : radian1 + dR * (i+1);
        pVerts[4+i*2]   = c.x + r * cosf(radian);
        pVerts[4+i*2+1] = c.y + r * sinf(radian);
    }
    
    // reinsert the center as the close point, used to draw the close edge
    pVerts[numVerts*2-2] = c.x;
    pVerts[numVerts*2-1] = c.y;
    
    glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
    
    // set blend mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	// fill the fan
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, pVerts);
	ccDrawColor4F(1.0f,0.0f,1.0f,0.2f);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts - 1);

	// highlight the edge
	ccDrawColor4F(1.0f,0.0f,1.0f,0.8f);
	glDrawArrays(GL_LINE_STRIP, 0, numVerts);
    
    // restore default state
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    
    delete[] pVerts;
}

void Painter::drawRect(CCRect rect,GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	int width = rect.size.width;
	int height = rect.size.height;
	         
	if (width == 0 || height == 0)
	{
		return ;
	}
	CCPoint upLeftPoint(rect.origin.x,rect.origin.y + height);
	CCPoint downRightPoint(rect.origin.x + width,rect.origin.y);
	{
		ccDrawColor4B(r, g, b, a);
	}
	glLineWidth( 2.0f );			
	ccDrawRect(upLeftPoint,downRightPoint);
	return ;
}