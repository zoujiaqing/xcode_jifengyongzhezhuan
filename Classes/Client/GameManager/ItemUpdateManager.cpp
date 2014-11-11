#include "ItemUpdateManager.h"
#include "SpriteMonster.h"

ItemUpdateManager::ItemUpdateManager()
{
	
}

ItemUpdateManager::~ItemUpdateManager()
{
	
}

void ItemUpdateManager::Update(float dt)
{
	CCPoint cameraOffset = CCDirector::sharedDirector()->getLevelRenderCameraOffset();
	CCSize visibleSize = CCDirector::sharedDirector()->getRenderSize();

	//Ìí¼Ó100ÏñËØ¿í¶ÈµÄ¿ò×÷Îª»º³å£¬·ÀÖ¹¶¶¶¯
	float bufDis = 100.0f;
	CCRect cameraRect = CCRectMake(cameraOffset.x,cameraOffset.y,visibleSize.width,visibleSize.height);	
	CCRect cameraOutRect = CCRectMake(cameraOffset.x - bufDis, cameraOffset.y - bufDis, visibleSize.width + 2*bufDis, visibleSize.height + 2*bufDis);	
	map<AreaItemNode *, bool>::iterator itor;
	for (itor = itemNodeMap.begin(); itor != itemNodeMap.end(); itor++)
	{
		if (!itor->first)
		{
			continue;
		}


		CCRect rect = (*itor).first->GetABBox();
		if (false == AreaItemNode::IsTwoRectsOverlapped(cameraRect,rect))
		{
			itor->first->SetUpdateOrNot(false);
			itor->second = false;
            //CCNode* pNode = dynamic_cast<CCNode*>((*itor).first);
            //if (pNode) {
            //    pNode->setVisible(false);
            //}
		}
		//else if (true == AreaItemNode::IsTwoRectsOverlapped(cameraRect,rect))
        else
		{
			itor->first->SetUpdateOrNot(true);
			itor->second = true;
            //CCNode* pNode = dynamic_cast<CCNode*>((*itor).first);
            //if (pNode) {
            //    pNode->setVisible(true);
            //}
		}
	}
}

bool ItemUpdateManager::addToUpdateManager(AreaItemNode * itemNode)
{
	bool ret = false;
	if (itemNodeMap.find(itemNode) == itemNodeMap.end())
	{
		itemNodeMap.insert(make_pair(itemNode, true));
		ret = true;
	}
	return ret;
}

bool ItemUpdateManager::removeFromUpdateManager(AreaItemNode * itemNode)
{
	bool ret = false;
	if (itemNodeMap.find(itemNode) != itemNodeMap.end())
	{
		itemNodeMap.erase(itemNode);
		ret = true;
	}
	return ret;
}

void ItemUpdateManager::resetData()
{
	itemNodeMap.clear();
}