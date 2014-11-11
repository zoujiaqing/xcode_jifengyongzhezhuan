#pragma once
#include "cs_battle.pb.h"
using namespace std;

class BATTLEINFO;
class MonsterData
{
public:
	MonsterData();
	virtual ~MonsterData();
	static MonsterData* GetMonsterInfo();
	static void   Destroy();
	void reset();
	void addMonser(MonsterInfo info);
	std::vector<MonsterInfo>* getMonsterList();

private:
	std::vector< MonsterInfo>* pMonsterList;
};