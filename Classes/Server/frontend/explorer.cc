#include "explorer.h"
#include "./virtualplayer.h"
#include "../battlefields/battlehandlers.h"
	
Explorer::Explorer(BasicUserInfo& info, CSGetUserSkillRsp& skill_msg, VirtualHandlerType virtual_handler) :
	m_virtual_hdlr(virtual_handler)
{
	g_tile_config = new TileConfig(); 
	g_monster_value_config = new MonsterValueConfig;
//	g_monster_value_config->load();
	g_range_config = new RangeConfig;
	g_range_config->load();
	g_skill_config = new SkillConfig;
	g_skill_config->load();
	g_monster_config = new MonsterConfig;
	g_monster_config->load();
	g_instance_config = new InstanceConfig;
	g_instance_config->load();
	g_mapcontent_config = new MapContentConfig;
//	g_mapcontent_config->load();
	g_fairy_skill_config = new FairySkillConfig;
	g_fairy_skill_config->load();
	g_hero_skill_config = new HeroSkillConfig;
	g_hero_skill_config->load();
	g_skill_hurt_config = new SkillHurtConfig;
//	g_skill_hurt_config->load();

	m_player = new Player(info, skill_msg, this);
	registe_handler();
}
	
Explorer::~Explorer() {
	if(m_player) {
		m_player->safe_release();
	}
	g_monster_value_config->clear();
	delete	g_monster_value_config;
	g_range_config->clear();
	delete g_range_config;
	g_skill_config->clear();
	delete g_skill_config;
	g_monster_config->clear();
	delete g_monster_config;
	g_instance_config->clear();
	delete g_instance_config;
	g_mapcontent_config->clear();
	delete g_mapcontent_config;
	g_fairy_skill_config->clear();
	delete g_fairy_skill_config;
	g_hero_skill_config->clear();
	delete g_hero_skill_config;
	g_skill_hurt_config->clear();
	delete g_skill_hurt_config;
	g_tile_config->clear();
	delete g_tile_config;
}
	
void Explorer::set_info(BasicUserInfo& info) {
	if(m_player) {
		m_player->set_info(info);
	}
}
	
void Explorer::set_skill(CSGetUserSkillRsp& skill_msg) {
	if(m_player) {
		m_player->set_skill(skill_msg);
	}
}
	
void Explorer::clock() {
	sGloble.Timer.process_events();
}
	
int Explorer::dispatch(const void* data, int datalen) {
	// 分解出包头和包体
	CSMessageHeader msghdr;
	Message* msgpayload;
	if (decode(data, &msghdr, &msgpayload) == false) {
        ERROR_LOG("invalid package len=%d",
					 datalen);
		return -1;
	}
	// 通过scoped_ptr自动释放msgpayload的内存，避免到处delete。
	scoped_ptr<Message> auto_del_msg(msgpayload);
	
	DEBUG_LOG("DISPATCH :%d", msghdr.msg_id());

	// 查找对应的处理函数对消息进行处理
	MsgHandlerMgr::HandlerType hdlr = m_hdlr_mgr.find_handler(head_msg::CSMsgName_Name(msghdr.msg_id()));
	if (hdlr) {
		int ret = hdlr(m_player, msgpayload);
		return ret;
	}
	m_player->send_stderr(olerr_invalid_req);
        ERROR_LOG("invalid req=%d len=%d",
                  msghdr.msg_id().c_str(), datalen);
	return -1;
}

void Explorer::registe_handler() {
	m_hdlr_mgr.register_handler(CSBattleCreateReq::descriptor()->name(), battle_create_cmd );
	m_hdlr_mgr.register_handler(CSBattleLoadCompleteReq::descriptor()->name(), battle_load_complete_cmd );
	m_hdlr_mgr.register_handler(CSBattleLeaveReq::descriptor()->name(), battle_leave_cmd );
	m_hdlr_mgr.register_handler(CSPlayerAttackReq::descriptor()->name(), player_attack_cmd );
	m_hdlr_mgr.register_handler(CSBattleMoveReq::descriptor()->name(), battle_move_cmd );
	m_hdlr_mgr.register_handler(CSSkillBeginReq::descriptor()->name(), skill_begin_cmd );
	m_hdlr_mgr.register_handler(CSBattlePlayerRevivalReq::descriptor()->name(), battle_player_revival_cmd );
	m_hdlr_mgr.register_handler(CSBattlePrepareRsp::descriptor()->name(), battle_prepare_front_cmd );
}
	
void Explorer::set_pos(uint32_t x, uint32_t y) {
	if(m_player) {
		m_player->m_pos.x = x;
		m_player->m_pos.y = y;
	}
}

void Explorer::virtual_hdlr(const void* const data, int len) {
	m_virtual_hdlr(data, len);
}
