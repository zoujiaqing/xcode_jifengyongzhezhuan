#ifndef GEM_STONES_FROM_LUA_H
#define GEM_STONES_FROM_LUA_H

#include "cocos2d.h"

class GemStonesFromLua
{
public:
	GemStonesFromLua();
	virtual ~GemStonesFromLua();
	static GemStonesFromLua* getInstance(void);
	static void Destroy();

	void init();
	//宝石镶嵌请求
	void EquipInlayGemReq(unsigned int equip,unsigned int hole,unsigned int gemId);
	//宝石镶嵌回包
	void EquipInlayGemRsp(unsigned int error);

	void GemRecastReq(unsigned int equipPos,unsigned int hole1,unsigned int hole2,unsigned int hole3,unsigned int hole4,unsigned int currencyKey);

	void GemRecastRsp(unsigned int error);

	void GemAnimaExchangeReq(unsigned int gemId,unsigned int multi);

	void GemAnimaExchangeRsp(unsigned int error);

	void GemHoleUnlockReq(unsigned int exchangeId,unsigned int equipPos);

	void GemHoleUnlockRsp(bool success);
private:

	bool checkEnchaseUILayer();
private:
	lua_State *m_configLuaState;
};

#endif