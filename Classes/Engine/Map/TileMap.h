#ifndef _TILE_MAP_2012_12_17_H
#define _TILE_MAP_2012_12_17_H

#include "../../../cocos2dx/cocos2dx/base_nodes/CCNode.h"

class CTileMap : public cocos2d::CCNode
{
	void LoadLayer(const char* pFileName, int zOrder);
public:
	CTileMap();
	~CTileMap();

	void LoadBGLayer(const char* pFileName);
	void LoadDcorateLayer(const char* pFileName);
	void LoadPhysicsLayer(const char* pFileName);

	// draw

    /** Override this method to draw your own node.
     The following GL states will be enabled by default:
     - glEnableClientState(GL_VERTEX_ARRAY);
     - glEnableClientState(GL_COLOR_ARRAY);
     - glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     - glEnable(GL_TEXTURE_2D);

     AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR NODE

     But if you enable any other GL state, you should disable it after drawing your node.
     */
    virtual void draw(void);

    /** recursive method that visit its children and draw them */
    virtual void visit(void);


};

#endif