#pragma once

#include "Singleton_t.h"
#include "AreaItemNode.h"

using namespace std;

class ItemUpdateManager : public TSingleton<ItemUpdateManager>
{
public:
	ItemUpdateManager();
	virtual ~ItemUpdateManager();

	bool addToUpdateManager(AreaItemNode * itemNode);
	bool removeFromUpdateManager(AreaItemNode * itemNode);

	void Update(float dt);

	void resetData();
private:
	map<AreaItemNode *, bool> itemNodeMap;
	CCPoint midPos;
	float length;
};