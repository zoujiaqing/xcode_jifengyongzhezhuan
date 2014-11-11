#ifndef BATTLEHANDLERS_H_
#define BATTLEHANDLERS_H_

#include <google/protobuf/message.h>
#include "battlefield.h"
#include "instance.h"
#ifndef FRONT
#include "wb_instance.h"
#include "babel_instance.h"
#endif

int battle_prepare_cmd(Player*, const google::protobuf::Message* msg);

int battle_prepare_front_cmd(Player*, const google::protobuf::Message* msg);

int battle_create_cmd(Player*, const google::protobuf::Message* msg);

int battle_load_complete_cmd(Player*, const google::protobuf::Message* msg);

int battle_move_cmd(Player*, const google::protobuf::Message* msg);

int player_attack_cmd(Player*, const google::protobuf::Message* msg); 

int skill_begin_cmd(Player*, const google::protobuf::Message* msg); 

int battle_leave_cmd(Player*, const google::protobuf::Message* msg); 

int player_hit_player_cmd(Player*, const google::protobuf::Message* msg); 

int battle_player_revival_cmd(Player*, const google::protobuf::Message* msg); 

int player_hp_cmd(Player*, const google::protobuf::Message* msg); 

int monster_die_cmd(Player*, const google::protobuf::Message* msg); 

int wb_view_players_cmd(Player*, const google::protobuf::Message* msg); 

int wb_btl_player_revival_cmd(Player* pPlayer, const google::protobuf::Message* msg);

int wb_battle_leave_cmd(Player* pPlayer, const google::protobuf::Message* msg);
int wb_btl_reward_cmd(Player* pPlayer, const google::protobuf::Message* msg);
int bb_btl_leave_cmd(Player* pPlayer, const google::protobuf::Message* msg);
#endif
