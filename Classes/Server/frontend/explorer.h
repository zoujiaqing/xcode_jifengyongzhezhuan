#ifndef EXPLORER_H_
#define EXPLORER_H_

#include "../config/battleconfig.h"
#include "../common/fwd_decl.h"
#include "../msg_dispatcher/msg_handler_mgr.h"
#include "../proto/errcode.pb.h"
#include "../proto/protocodec.h"
#include "../proto/cs_core.pb.h"
#include "../proto/head.pb.h"

class Player;

typedef int (*VirtualHandlerType)(const void* const data, int len);

class Explorer {
public:
	Explorer(BasicUserInfo& info, CSGetUserSkillRsp& skill_msg, VirtualHandlerType virtual_handler);

	void set_info(BasicUserInfo& info);

	void set_skill(CSGetUserSkillRsp& skill_msg);

	~Explorer();

	void clock();

	int dispatch(const void* data, int datalen);

	void registe_handler();

	void virtual_hdlr(const void* const data, int len);

	void set_pos(uint32_t x, uint32_t y);

private:
	MsgHandlerMgr m_hdlr_mgr;   // 所有消息的处理函数保存于此

	Player* m_player;

	VirtualHandlerType m_virtual_hdlr;
};

#endif
