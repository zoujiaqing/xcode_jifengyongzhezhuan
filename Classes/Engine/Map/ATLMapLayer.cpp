#include "AppDelegate.h"
#include "ATLMapLayer.h"
#include "LevelLayer.h"

CATLMapLayer::CATLMapLayer() :
    m_mapScale(1.0f)
{
    std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();

    if (strcmp(clientVersion.c_str(), "hq") == 0)
    {
        m_mapScale = HQ_MAP_RATE;
    } else if (strcmp(clientVersion.c_str(), "nq") == 0)
    {
        m_mapScale = NQ_MAP_RATE;
    }
}

CATLMapLayer::~CATLMapLayer()
{
}

void CATLMapLayer::draw(void)
{
	cocos2d::CCSpriteBatchNode::draw();
}

void CATLMapLayer::ParseMap(CATLMapInfo* pMapInfo)
{
	if(pMapInfo == NULL)
		return;
	// 坐标系转换为OpenGL的，原点在左下角

	initWithTexture(pMapInfo->GetTexture(), 1);
	ASprite* pTileset = pMapInfo->GetTileset(); 
	assert(pTileset != NULL);
	int height = (pMapInfo->GetHeight() - 1) * pMapInfo->GetTileHeight();
	for(int j = 0; j < pMapInfo->GetHeight(); j++)
	{
		for(int i = 0; i < pMapInfo->GetWidth(); i++)
		{
			CCPoint pt = ccp(0,0);
			int frame = pMapInfo->GetFrameIndex(i, j);
			if(frame >= 0)
			{
				int moduleCount = pTileset->GetFModules(frame);
				for(int k = 0; k < moduleCount; k++)
				{
					int moduleID = pTileset->GetFrameModule(frame, k);
					CCRect frameRect(0,0,0,0);
					frameRect.size.width = (float)pTileset->GetModuleWidth(moduleID) * m_mapScale;
					frameRect.size.height = (float)pTileset->GetModuleHeight(moduleID) * m_mapScale;
					frameRect.origin.x = (float)pTileset->GetModuleX(moduleID) * m_mapScale;
					frameRect.origin.y = (float)pTileset->GetModuleY(moduleID) * m_mapScale;
					int offsetX = pTileset->GetFrameModuleX(frame, k);
					int offsetY = pTileset->GetFrameModuleY(frame, k);
					CCSprite *pSprite = new CCSprite();
					pSprite->autorelease();

					pSprite->initWithTexture(pMapInfo->GetTexture(),frameRect);
					pSprite->setScale(1 / m_mapScale);
					
					pSprite->setAnchorPoint(CCPointZero);
					//pSprite->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_TILE_BACKGROUND));

					pSprite->setPosition(ccp(i * pMapInfo->GetTileWidth() + offsetX, height - (j * pMapInfo->GetTileHeight() + offsetY)));

					bool bFlipX = false;
					bool bFlipY = false;	// 在游戏中坐标系的原点在左下角，在地图编辑器中，坐标系为左上角
					
					int flag = pTileset->GetFModuleFlags(frame, k);
					
					switch(flag)
					{
					case 1:
						// 水平翻转
						bFlipX = !bFlipX;
						break;
					case 2:
						// 竖直翻转;
						bFlipY = !bFlipY;
						break;
					case 3:
						bFlipX = !bFlipX;
						bFlipY = !bFlipY;
						break;
					default:
						break;
					}
				
					flag = pMapInfo->GetFlag(i, j);
					switch(flag)
					{
					case 1:
						// 水平翻转
						bFlipX = !bFlipX;
						break;
					case 2:
						// 竖直翻转
						bFlipY = !bFlipY;
						break;
					case 3:
						bFlipX = !bFlipX;
						bFlipY = !bFlipY;
						break;
					default:
						break;
					}
					

					pSprite->setFlipX(bFlipX);	
					pSprite->setFlipY(bFlipY);	
					
					insertQuadFromSprite(pSprite, 0);
					//
					//addChild(pSprite, 0);
				}
			}
		}
	}
}

void CATLMapLayer::addChild( CCNode * child, int zOrder )
{
	CCSpriteBatchNode::addChild(child, zOrder);
}

void CATLMapLayer::visit( void )
{
	
	CC_PROFILER_START_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");

	// CAREFUL:
	// This visit is almost identical to CocosNode#visit
	// with the exception that it doesn't call visit on it's children
	//
	// The alternative is to have a void CCSprite#visit, but
	// although this is less maintainable, is faster
	//
	if (! m_bVisible)
	{
		return;
	}

	kmGLPushMatrix();



	draw();

	kmGLPopMatrix();

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");

}
