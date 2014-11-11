#include "MonsterData.h"
#include "UserData.h"

static MonsterData* stMonsterData = NULL;

MonsterData::MonsterData()
{
	pMonsterList = new std::vector<MonsterInfo>();
}

MonsterData::~MonsterData()
{
	if(pMonsterList)
	{
		pMonsterList->clear();
		delete pMonsterList;
		pMonsterList = NULL;
	}
}

void MonsterData::Destroy()
{
	if(stMonsterData)
	{
		delete stMonsterData;
		stMonsterData = 0;
	}
}

MonsterData* MonsterData::GetMonsterInfo()
{
	if(!stMonsterData)
	{
		stMonsterData = new MonsterData();
	}
	return stMonsterData;
}

void MonsterData::reset()
{
	if(pMonsterList)
	{
		pMonsterList->clear();
	}
}

void MonsterData::addMonser(MonsterInfo info)
{
	pMonsterList->push_back(info);
}

std::vector<MonsterInfo>* MonsterData::getMonsterList()
{
	return pMonsterList;
}