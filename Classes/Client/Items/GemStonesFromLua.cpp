#include "OnlineNetworkManager.h"
#include "GemStonesFromLua.h"
#include "GameConfigFromLuaManager.h"
#include "LuaEngine.h"
#include "ItemManager.h"
#include "LuaTinkerManager.h"
static GemStonesFromLua* s_gemStonesFromLua = NULL;

GemStonesFromLua::GemStonesFromLua()
{
	m_configLuaState = NULL;
}

GemStonesFromLua::~GemStonesFromLua()
{

}

GemStonesFromLua* GemStonesFromLua::getInstance()
{
	if(s_gemStonesFromLua == 0)
	{
		s_gemStonesFromLua = new GemStonesFromLua();
		s_gemStonesFromLua->init();
	}
	return s_gemStonesFromLua;
}

void GemStonesFromLua::Destroy()
{
	CC_SAFE_DELETE(s_gemStonesFromLua);
}

void GemStonesFromLua::init()
{
	this->m_configLuaState = CCLuaEngine::defaultEngine()->getLuaState();
	checkEnchaseUILayer();
	LuaTinkerManager::Get()->checkAnyLoadFile("Script/UILayer/enchaseExchangeUILayer.lua");
}

bool GemStonesFromLua::checkEnchaseUILayer()
{
	return LuaTinkerManager::Get()->checkAnyLoadFile("Script/UILayer/enchaseUILayer.lua");
}

void GemStonesFromLua::EquipInlayGemReq(unsigned int equip,unsigned int hole,unsigned int gemId)
{
	OnlineNetworkManager::sShareInstance()->sendEquipInlayGemReq(equip,hole,gemId);
}

void GemStonesFromLua::EquipInlayGemRsp(unsigned int error)
{
	if(checkEnchaseUILayer())
	{
		lua_getglobal(this->m_configLuaState, "onReceiveEquipInlayGem");
		lua_pushnumber(this->m_configLuaState,error);
		int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
		if (ret != 0) {
			CCLOG("call onReceiveEquipInlayGem error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return ;
		}
		
	}
}

void GemStonesFromLua::GemRecastReq(unsigned int equipPos,unsigned int hole1,unsigned int hole2,unsigned int hole3,unsigned int hole4,unsigned int currencyKey)
{
	unsigned int holeLength = 4;
	unsigned int holes[4];
	holes[0] = hole1;
	holes[1] = hole2;
	holes[2] = hole3;
	holes[3] = hole4;
	OnlineNetworkManager::sShareInstance()->sendGemRecastReq(equipPos,holes,holeLength,currencyKey);
}

void GemStonesFromLua::GemRecastRsp(unsigned int error)
{
	if(checkEnchaseUILayer())
	{
		lua_getglobal(this->m_configLuaState, "onRecieveGemRecast");
		lua_pushnumber(this->m_configLuaState,error);
		int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
		if (ret != 0) {
			CCLOG("call onReceiveEquipInlayGem error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return ;
		}

	}
}

void GemStonesFromLua::GemAnimaExchangeReq(unsigned int gemId,unsigned int multi)
{
	ExchangeItem* exchangeItem = ItemManager::Get()->getEquipDrawingById(gemId);
	if(exchangeItem != NULL)
	{
		OnlineNetworkManager::sShareInstance()->sendEquipComposeReq(exchangeItem->m_exchangeId,0,multi);
	}
}

void GemStonesFromLua::GemAnimaExchangeRsp(unsigned int error)
{
	if(LuaTinkerManager::Get()->checkAnyLoadFile("Script/UILayer/enchaseExchangeUILayer.lua"))
	{
		lua_getglobal(this->m_configLuaState, "onReceivedExchangeRsp");
		lua_pushnumber(this->m_configLuaState,error);
		int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
		if (ret != 0) {
			CCLOG("call onReceivedExchangeRsp error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return ;
		}

	}
}

void GemStonesFromLua::GemHoleUnlockReq(unsigned int exchangeId,unsigned int equipPos)
{
	OnlineNetworkManager::sShareInstance()->sendExchangeParameterReq(exchangeId, &equipPos, 1);
}

void GemStonesFromLua::GemHoleUnlockRsp(bool success)
{
	if(checkEnchaseUILayer())
	{
		lua_getglobal(this->m_configLuaState, "onReceiveEquipHoleUnlock");
		lua_pushboolean(this->m_configLuaState,success);
		int ret = lua_pcall(this->m_configLuaState, 1, 0, 0);
		if (ret != 0) {
			CCLOG("call onReceiveEquipHoleUnlock error:%s", lua_tostring(this->m_configLuaState, -1));
			lua_pop(this->m_configLuaState, 1);

			return ;
		}

	}
}