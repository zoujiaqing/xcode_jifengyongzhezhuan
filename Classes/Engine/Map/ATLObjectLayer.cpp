
#include "AppDelegate.h"
#include "ATLObjectLayer.h"
#include "AspriteManager.h"
#include "AnimSprite.h"
#include "EnginePlayerManager.h"
#include "LevelLayer.h"

// ------------------------------------------------------------------------

AlphaTestBatchNode::AlphaTestBatchNode()
	: m_coef(0.0f)
	, m_location(0)
    , m_isNeedUpdteTransformEveryFrame(true)
    , m_updateTransformCount(0)
	, m_bDepthTest(true)
{

}

AlphaTestBatchNode::~AlphaTestBatchNode()
{

}

AlphaTestBatchNode* AlphaTestBatchNode::create( const char* pszFileName )
{
	AlphaTestBatchNode* graySprite = new AlphaTestBatchNode();
	if (graySprite && graySprite->initWithFile(pszFileName, 100))
	{
		graySprite->autorelease();
		return graySprite;
	}
	else
	{
		CC_SAFE_RELEASE(graySprite);
		return NULL;
	}
}

bool AlphaTestBatchNode::initWithFile(const char* fileImage, unsigned int capacity)
{
	CCTexture2D *pTexture2D = CCTextureCache::sharedTextureCache()->addImage(fileImage);
	return initWithTexture(pTexture2D, capacity);
}
//地图遮挡优化 
bool AlphaTestBatchNode::initWithTexture(CCTexture2D* pTexture, unsigned int capacity )
{
	do{
		CC_BREAK_IF(!CCSpriteBatchNode::initWithTexture(pTexture, capacity));

		GLchar* pszFragSource =
			"#ifdef GL_ES \n \
			precision mediump float; \n \
			#endif \n \
			uniform float u_coef;	\n \
			uniform sampler2D u_texture; \n \
			varying vec2 v_texCoord; \n \
			varying vec4 v_fragmentColor; \n \
			void main(void) \n \
			{ \n \
			// Convert to greyscale using NTSC weightings \n \
			//calculate the fragment color based on the texture and the vertex colour \n \
			vec4 basecolor = texture2D(u_texture, v_texCoord); \n \
			basecolor *= v_fragmentColor;	\n	\
			if(basecolor.a <= 0.0) \n \
			{ \n \
			//throw this fragment away \n \
			discard; \n \
			} \n \
			gl_FragColor = basecolor; \n \
			}";

		CCGLProgram* pProgram = new CCGLProgram();
		pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
		this->setShaderProgram(pProgram);
		pProgram->release();
		CHECK_GL_ERROR_DEBUG();

		this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();

	
		this->getShaderProgram()->link();
		CHECK_GL_ERROR_DEBUG();

		this->getShaderProgram()->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		m_location = glGetUniformLocation(this->getShaderProgram()->getProgram(), "u_coef");
		this->getShaderProgram()->setUniformLocationWith1f(m_location, m_coef);
		//glUniform1f(m_location, m_coef);
		CHECK_GL_ERROR_DEBUG();

//		this->getShaderProgram()->use();

		return true;
	} while (0);
	return false;
}

void AlphaTestBatchNode::draw()
{
//	if (false == m_isNeedUpdteTransformEveryFrame) {
//        return;
//    }

	//	this->getShaderProgram()->use();
	//this->getShaderProgram()->setUniformLocationWith1f(m_location, m_coef);


	//CCSpriteBatchNode::draw();
    
	/*if(!!m_bDepthTest)
	{
		glDepthMask(GL_TRUE);	
	}*/

    CC_PROFILER_START("CCSpriteBatchNode - draw");
    
    // Optimization: Fast Dispatch
    if( m_pobTextureAtlas->getTotalQuads() == 0 )
    {
        return;
    }
    
    CC_NODE_DRAW_SETUP();
    
    if (m_isNeedUpdteTransformEveryFrame) {
        arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);
    }
    else{
        if(m_updateTransformCount < 2){
            arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCSprite*);
            m_updateTransformCount ++;
        }
    }
    
//    do {                                                                  
//        if(m_pChildren && m_pChildren->count() > 0)                                 
//        {                                                                 
//            CCObject* child;                                              
//            CCARRAY_FOREACH(m_pChildren, child)                                
//            {                                                             
//                CCSprite* pNode = (CCSprite*) child;
//                if(pNode)                                                 
//                {                                                         
//                    pNode->updateTransform();                                        
//                }                                                         
//            }                                                             
//        }                                                                 
//    }                                                                     
//    while(false);
    
    ccGLBlendFunc( m_blendFunc.src, m_blendFunc.dst );
    
    m_pobTextureAtlas->drawQuads();
    
	/*if(!m_bDepthTest)
	{
		glDepthMask(GL_FALSE);	
	}*/

    CC_PROFILER_STOP("CCSpriteBatchNode - draw");
}

// ------------------------------------------------------------------------


RoleShadowBatchNode::RoleShadowBatchNode()
{

}


RoleShadowBatchNode::~RoleShadowBatchNode()
{

}


RoleShadowBatchNode* RoleShadowBatchNode::create( const char* pszFileName )
{
	RoleShadowBatchNode* graySprite = new RoleShadowBatchNode();
	if (graySprite && graySprite->initWithFile(pszFileName, 100))
	{
		graySprite->autorelease();
		return graySprite;
	}
	else
	{
		CC_SAFE_RELEASE(graySprite);
		return NULL;
	}
}

void RoleShadowBatchNode::visit()
{
	int i,j,length = m_pChildren->data->num;
	CCNode ** x = (CCNode**)m_pChildren->data->arr;
	CCNode *tempItem;

	// insertion sort
	for(i=0; i<length; i++)
	{
		tempItem = x[i];
		if(tempItem)
		{
			CCNode* pRole = (CCNode*)(tempItem->getUserData());
			if(pRole)
			{
				BoneSpriteBase* pSprite = dynamic_cast<BoneSpriteBase*>(pRole);
				if(pSprite)
				{
					if(pSprite->getShadowVisible())
					{
						if(!pSprite->isVisible())
						{
							tempItem->setVisible(false);
						}
						else if(pSprite->GetRoleOpacity() == 0)
						{
							tempItem->setVisible(false);
						}
						else
						{
							tempItem->setVisible(true);
						}
					}
					else
					{
						tempItem->setVisible(false);
					}
					
				}
				tempItem->setPosition(pRole->getPosition());
			}
		}
	}

	//m_bReorderChildDirty = true;
	CCSpriteBatchNode::visit();
}

void RoleShadowBatchNode::draw()
{
    CCSpriteBatchNode::draw();
}

void RoleShadowBatchNode::RemoveShadow( CCNode* child )
{
	int i,j,length = m_pChildren->data->num;
	CCNode ** x = (CCNode**)m_pChildren->data->arr;
	CCNode *tempItem;

	// insertion sort
	for(i=0; i<length; i++)
	{
		tempItem = x[i];
		if(tempItem)
		{
			CCNode* pRole = (CCNode*)(tempItem->getUserData());
			if(pRole == child)
			{
				removeChild(tempItem, true);
			}
		}
	}
}

// ------------------------------------------------------------------------


CATLObjectLayer::CATLObjectLayer()
	:m_height(0)
	, m_pRoleShadow(NULL)
    , m_objectScale(1.0f)
{
	m_name = "ATLObjectLayer";

    std::string clientVersion = AppDelegate::GetShareAppDelegate()->getClientVersion();

    if (strcmp(clientVersion.c_str(), "hq") == 0)
    {
        m_objectScale = HQ_MAP_RATE;
    } else if (strcmp(clientVersion.c_str(), "nq") == 0)
    {
        m_objectScale = NQ_MAP_RATE;
    }
}

CATLObjectLayer::~CATLObjectLayer()
{
	RemoveLayerObjects();
}

void CATLObjectLayer::draw(void)
{
	cocos2d::CCLayer::draw();
}



void CATLObjectLayer::ParseLayer(CATLObjectInfo::LAYER::BASE* pLayer, CATLObjectInfo::VECTOR2D parentPos)
{
	CATLObjectInfo::VECTOR2D pos = parentPos;
	CATLObjectInfo::LAYER::OBJECT* pObject = dynamic_cast<CATLObjectInfo::LAYER::OBJECT*>(pLayer);
	if(pObject != NULL)
	{
		ASprite* pAsprite = NULL;
		CCSpriteBatchNode* pBatchNode = NULL;
		cocos2d::CCTexture2D* pTexture = NULL;

		std::map<std::string, OBJECT_BATCHNODE>::iterator iter = m_objectBatchNode.find(pObject->spriteFile);
		if(iter != m_objectBatchNode.end())
		{
			pAsprite = iter->second.pAsprite;
			pTexture = iter->second.pTexture;
			pBatchNode = iter->second.pBatchNode;
		}
		else
		{
			pAsprite = AspriteManager::getInstance()->getAsprite((std::string("Decoration/") + pObject->spriteFile).c_str());

			if(pAsprite)
			{
				pBatchNode = AlphaTestBatchNode::create(pAsprite->getImageName().c_str());
                ((AlphaTestBatchNode*)pBatchNode)->SetIsUpdateEveryFrame(false);
				pTexture = pBatchNode->getTexture();
				float coef = 240.0f / 255.0f;
				switch(pObject->zOrder)
				{
				case MAP_Z_OBJ_SHADOW:
					coef = 0.0f;
					 ((AlphaTestBatchNode*)pBatchNode)->SetDepthTest(false);
					break;
				case MAP_Z_LIGHT:
					coef = 0.0f;
					break;
				case MAP_Z_ROLE_SHADOW:
					coef = 1.0f;
					break;
				default:
					break;
				}
				((AlphaTestBatchNode*)pBatchNode)->SetCoef( coef );

			}

			OBJECT_BATCHNODE ob;
			ob.pAsprite = pAsprite;
			ob.pTexture = pTexture;
			ob.pBatchNode = pBatchNode;
			m_objectBatchNode.insert(std::make_pair(pObject->spriteFile, ob));

			if(pObject->zOrder == MAP_Z_LIGHT)
			{
				cocos2d::ccBlendFunc bf;
				bf.src = GL_SRC_ALPHA;
				bf.dst = GL_ONE;
				pBatchNode->setBlendFunc(bf);
			}
			else
			{
				cocos2d::ccBlendFunc bf;
				bf.src = GL_SRC_ALPHA;
				bf.dst = GL_ONE_MINUS_SRC_ALPHA;
				pBatchNode->setBlendFunc(bf);
			}


			// 决定画的顺序。
			addChild(pBatchNode, pObject->zOrder);

			// 父节点的ZDepth也会影响到子节点的ZDepth。
			//pBatchNode->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_MIN));
			// ????
			pBatchNode->setVertexZ(0.0f);


		}

		if(pAsprite == NULL || pBatchNode == NULL || pTexture == NULL)
		{
			CCLOG("Error : parse object layer : %s", pObject->spriteFile.c_str());
			return;
		}

		pos.x += pObject->pos.x;
		pos.y += pObject->pos.y;
		cocos2d::CCPoint cp(pos.x, pos.y );

		if(pAsprite->GetAFrames(pObject->animID) > 1)
		{
			ParseAnimationLayer(pObject, cp, pAsprite, pTexture);
		}
		else
		{
			ParseImmovableLayer(pObject, cp, pBatchNode, pAsprite, pTexture);
		}

	}
	else
	{
        size_t count = pLayer->childrens.size();
		for(size_t i = 0; i < count; i++)
		{
			ParseLayer(pLayer->childrens[i], pos);
		}
	}
}

void CATLObjectLayer::ParseObject(CATLObjectInfo* pObjectInfo, int levelID, LevelLayer* pLevelLayer)
{
	if(pObjectInfo == NULL)
		return;

	// TODO : 区分Level？？
	for(size_t i = 0; i < pObjectInfo->m_levels.size(); i++)
	{
		int id = atoi(pObjectInfo->m_levels[i]->name.c_str());
		if(id == levelID)
		{
			m_height = pLevelLayer->getMapSizeInPixel().height;// - pLevelLayer->getMapTileSizeInPixel().height;
			for(size_t j = 0; j < pObjectInfo->m_levels[i]->layers.size(); j++)
			{

				ParseLayer(pObjectInfo->m_levels[i]->layers[j], pObjectInfo->m_levels[i]->pos);
			}
		}		
	}
}

void CATLObjectLayer::reorderChild(cocos2d::CCNode *child, int zOrder)
{
	child->setVertexZ(LevelLayer::sCalcDepth(zOrder));
	CCLayer::reorderChild(child, zOrder);
}

void CATLObjectLayer::addChild( CCNode * child, int zOrder )
{
	child->setVertexZ(LevelLayer::sCalcDepth(zOrder));
	CCLayer::addChild(child, zOrder);
}


void CATLObjectLayer::RemoveLayerObjects()
{
	for(size_t i = 0; i < m_lstAnimSprites.size(); i++)
	{
		AnimSprite* anim = m_lstAnimSprites[i];
		EnginePlayerManager::getInstance()->removeAsprite(anim, true);
	}

	m_lstAnimSprites.clear();
}

size_t CATLObjectLayer::getBatchNodeNumber()
{
	return 0;
}

void CATLObjectLayer::ParseImmovableLayer( CATLObjectInfo::LAYER::OBJECT* pObject, const cocos2d::CCPoint& pos, CCSpriteBatchNode* pBatchNode, ASprite* pAsprite, cocos2d::CCTexture2D* pTexture )
{
	if(pAsprite != NULL)
	{
		int animID = pObject->animID;
		int frame = pAsprite->GetAnimFrame(animID, 0);
		if(frame >= 0)
		{
			int animOffsetX = pAsprite->GetAFrameOffsetX(animID, 0, 0);
			int animOffsetY = pAsprite->GetAFrameOffsetY(animID, 0, 0);

			CCSprite *pAnimSprite = new CCSprite();
			pAnimSprite->autorelease();
			pAnimSprite->initWithTexture(pTexture, CCRectZero);
			pAnimSprite->setAnchorPoint(ccp(0.0f, 1.0f));

			CCPoint animPos(pos.x + animOffsetX, pos.y + animOffsetY);
			animPos.y = m_height - animPos.y;

			int zOrder = pObject->zOrder; 
			if(zOrder == MAP_Z_OBJECT || zOrder == MAP_Z_OBJ_SHADOW)
			{
				zOrder = LevelLayer::sCalcZorder(ccp(pos.x, m_height - pos.y));
			}

			pAnimSprite->setVertexZ(LevelLayer::sCalcDepth(zOrder));

			int moduleCount = pAsprite->GetFModules(frame);
			for(int k = 0; k < moduleCount; k++)
			{
				int moduleID = pAsprite->GetFrameModule(frame, k);
				CCRect frameRect(0,0,0,0);
				frameRect.size.width = (float)pAsprite->GetModuleWidth(moduleID) * m_objectScale;
				frameRect.size.height = (float)pAsprite->GetModuleHeight(moduleID) * m_objectScale;
				frameRect.origin.x = (float)pAsprite->GetModuleX(moduleID) * m_objectScale;
				frameRect.origin.y = (float)pAsprite->GetModuleY(moduleID) * m_objectScale;
				int frameOffsetX = pAsprite->GetFrameModuleX(frame, k);
				int frameOffsetY = pAsprite->GetFrameModuleY(frame, k);
				CCSprite *pSprite = new CCSprite();
				pSprite->autorelease();

				pSprite->initWithTexture(pTexture,frameRect);
				pSprite->setScale(1 / m_objectScale);

				pSprite->setAnchorPoint(ccp(0.0f, 1.0f));

				// 因为在引擎中坐标的原点为左下角，在ATL编辑器中，坐标系的原点为左上角，所以要加上frameRect.size.height
				//CCPoint pt(pos.x + animOffsetX + frameOffsetX, pos.y + animOffsetY + frameOffsetY + frameRect.size.height );
				CCPoint pt(frameOffsetX, -frameOffsetY);

				bool bFlipX = false;
				bool bFlipY = false;	// 在游戏中坐标系的原点在左下角，在地图编辑器中，坐标系为左上角

				int flag = pAsprite->GetFModuleFlags(frame, k);

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

				pSprite->setFlipX(bFlipX);	
				pSprite->setFlipY(bFlipY);	
				pSprite->setPosition(pt);
				pSprite->setVertexZ(LevelLayer::sCalcDepth(zOrder));

				pAnimSprite->addChild(pSprite);

			}

			
			bool bFlipX = false;
			bool bFlipY = false;	// 在游戏中坐标系的原点在左下角，在地图编辑器中，坐标系为左上角
			int flag = pObject->flags;
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

			switch(pObject->renderFlag)
			{
			case CATLObjectInfo::LAYER::OBJECT::E_FLIP_X:
				//pSprite->setRotationY(180.0f);
				// pParentNode->setScaleX(-1.0f * pParentNode->getScaleX());
				bFlipX = !bFlipX;
				break;
			case CATLObjectInfo::LAYER::OBJECT::E_FLIP_Y:
				//pSprite->setRotationX(180.0f);
				bFlipY = !bFlipY;
				break;
			case CATLObjectInfo::LAYER::OBJECT::E_FLIP_XY:
				/*pSprite->setRotationX(180.0f);
				pSprite->setRotationY(180.0f);*/
				bFlipX = !bFlipX;
				bFlipY = !bFlipY;
				break;
			default:
				break;
			}

			if(bFlipX)
				pAnimSprite->setRotationY(180.0f);

			if(bFlipY)
				pAnimSprite->setRotationX(180.0f);

			/*pAnimSprite->setFlipX(bFlipX);	
			pAnimSprite->setFlipY(bFlipY);	*/

			// 必须要先旋转后再设置坐标
			
			pAnimSprite->setPosition(animPos);

			pBatchNode->addChild(pAnimSprite, zOrder);

		}
	}
}

void CATLObjectLayer::ParseAnimationLayer( CATLObjectInfo::LAYER::OBJECT* pObject, const cocos2d::CCPoint& pos, ASprite* pAsprite, cocos2d::CCTexture2D* pTexture )
{
	AnimSprite *anim = EnginePlayerManager::getInstance()->addAnimSprite(pAsprite, 0.0f, 0.0f);			
	if(anim != NULL)
	{
		anim->SetAnim(pObject->animID,1,true);	

		/// store animsprites
		m_lstAnimSprites.push_back(anim);

		/// bind to the bathNode
		CCSprite * pParentNode = anim->getParentNode();	

		CCSpriteBatchNode* pBatchNode = anim->getBatchNode();

#if _DEBUG
		if(pBatchNode)
		{
			pBatchNode->m_name = pObject->spriteFile;
		}
#endif
		pParentNode->setAnchorPoint(ccp(0.5f, 0.5f));				
		switch(pObject->renderFlag)
		{
		case CATLObjectInfo::LAYER::OBJECT::E_FLIP_X:
			pParentNode->setRotationY(180.0f);
			// pParentNode->setScaleX(-1.0f * pParentNode->getScaleX());

			break;
		case CATLObjectInfo::LAYER::OBJECT::E_FLIP_Y:
			pParentNode->setRotationX(180.0f);
			break;
		case CATLObjectInfo::LAYER::OBJECT::E_FLIP_XY:
			pParentNode->setRotationX(180.0f);
			pParentNode->setRotationY(180.0f);
			break;
		default:
			break;
		}
		
		// 必须要先旋转后再设置坐标
		CCPoint cp(pos);
		cp.y = m_height - cp.y;
		pParentNode->setPosition(cp);

		int zOrder = pObject->zOrder; 
		if(zOrder == MAP_Z_LIGHT)
		{
			cocos2d::ccBlendFunc bf;
			bf.src = GL_SRC_ALPHA;
			bf.dst = GL_ONE;
			pBatchNode->setBlendFunc(bf);
		}
		else if(zOrder == MAP_Z_OBJECT)
		{
			zOrder = LevelLayer::sCalcZorder(cp);
		}

		addChild(pBatchNode, zOrder);
	}
}

void CATLObjectLayer::AddRoleShadow( cocos2d::CCNode* pRole )
{
	if(m_pRoleShadow == NULL)
	{
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("spriteBin/shadow.bin");
		if(pAsprite != NULL)
		{
			m_pRoleShadow = RoleShadowBatchNode::create(pAsprite->getImageName().c_str());
			addChild(m_pRoleShadow, MAP_Z_ROLE_SHADOW);
			m_pRoleShadow->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_ROLE_SHADOW));
		}
	}

	if(m_pRoleShadow)
	{
		CCPoint cp;
		ASprite* pAsprite = AspriteManager::getInstance()->getAsprite("spriteBin/shadow.bin");

		CCSprite* pCSprite = pAsprite->getSpriteFromFrame(0, 0, cp);
		if(pCSprite != NULL)
		{
			pCSprite->setTexture(m_pRoleShadow->getTexture());
			cp = pRole->getPosition();
			pCSprite->setPosition(cp);
			pCSprite->setTag(SHADOW_TAG);
			pCSprite->setUserData(pRole);
			int zOrder = LevelLayer::sCalcZorder(cp);
			pCSprite->setVertexZ(LevelLayer::sCalcDepth(MAP_Z_ROLE_SHADOW));
			m_pRoleShadow->addChild(pCSprite, MAP_Z_ROLE_SHADOW);


		}
	}
}

void CATLObjectLayer::removeChild( CCNode* child, bool cleanup )
{
	if(m_pRoleShadow)
	{
		m_pRoleShadow->RemoveShadow(child);
	}
	CCLayer::removeChild(child, cleanup);
}
