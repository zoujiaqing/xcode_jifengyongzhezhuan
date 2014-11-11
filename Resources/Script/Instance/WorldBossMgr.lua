require("Script/Instance/WorldBossRemainTimeLayer")
require("Script/Instance/WorldBossDamageLayer")
require("Script/Instance/WorldBossFailLayer")
require("Script/Instance/WorldBossOverLayer")

local function sendWorldBossViewPlayerReq()
	CSWBViewPlayersReq = {
		}
	local msgname="CSWBViewPlayersReq"
	local ret = send_message(msgname, CSWBViewPlayersReq, true)
	return ret
end

-- 判断世界boss活动是否已经结束
local function sendWorldBossRewardReq()
	CSWBRewardReq = {
		}
	local msgname="CSWBRewardReq"
	local ret = send_message(msgname, CSWBRewardReq, true)
	return ret
end

function GetWorldBossViewPlayer()
	sendWorldBossViewPlayerReq()
	return true
end

local function sendWorldBossBattleCreateReq()
	CSBattleCreateReq = {
		battle_id = 290000,
		map_id = 0,
		}
	local msgname="CSBattleCreateReq"
	local ret = send_message(msgname, CSBattleCreateReq, true)
	return ret;
end

function OnPressWorldBossPortal()
	if WorldBossRemainTimeLayer.isStart then
		sendWorldBossBattleCreateReq()
		PushOneWaitingLayer("CSBattleCreateReq")
	else
		GameApi:showMessage(GetLuaLocalization("M_WORLDBOSS_NOTOPEN"))
	end
	
	return true
end

local function onMsgWorldBossFail(msg)

	local isWorldBoss = LevelManager:sShareInstance():isCurrentWorldBossLevel()
	if not isWorldBoss then
		return 
	end

	WorldBossFailData.coin = msg.coin
	WorldBossFailData.reputation = msg.reputation
	WorldBossFailData.revival_time = msg.revival_time

	showUILayerByTag(UITagTable["WorldBossFailLayer"].tag,true)
end

local function onMsgWorldBossOver(msg)
	CCLuaLog("--- onMsgWorldBossOver ---")
	WorldBossOverData = {}	
	if msg.ranking ~= nil then		
		for _ , info in pairs(msg.ranking) do
			WorldBossOverData[info.rank] = {
				rank = info.rank,
				name = info.name,
				hurt = info.hurt,
				percentage = info.percentage,
				coin = info.coin,
				reputation = info.reputation,
			}
		end
	end

	WorldBossOverSelfRanking = {}
	if msg.self_ranking ~= nil then
		WorldBossOverSelfRanking = {
			rank = msg.self_ranking.rank,
			name = msg.self_ranking.name,
			hurt = msg.self_ranking.hurt,
			percentage = msg.self_ranking.percentage,
			coin = msg.self_ranking.coin,
			reputation = msg.self_ranking.reputation,
		}
	end

	WorldBossOverLastKill = {}
	if msg.last_killer ~= nil then
		WorldBossOverLastKill = {
			rank = msg.last_killer.rank,
			name = msg.last_killer.name,
			hurt = msg.last_killer.hurt,
			percentage = msg.last_killer.percentage,
			coin = msg.last_killer.coin,
			reputation = msg.last_killer.reputation,
		}
	end
	
	showUILayerByTag(UITagTable["WorldBossOverLayer"].tag,true)
end

function OnEnterWorldBoss(inBattle)
	WorldBossRemainTimeLayer.inBattle = inBattle
	WorldBossDamageLayer.inBattle = inBattle

	showUILayerByTag(UITagTable["WorldBossRemainTimeLayer"].tag,true)
	showUILayerByTag(UITagTable["WorldBossDamageLayer"].tag,true)

	--提前注册玩家死亡回调，最早在进入入口时
	addMsgCallBack("CSWBPlayerFailRsp", onMsgWorldBossFail)	
	addMsgCallBack("CSWBRewardRsp", onMsgWorldBossOver)

	--查看是否结束
	sendWorldBossRewardReq()

	if inBattle then
		CCLuaLog("inBattle")
		showUILayerByTag(UITagTable["WorldBossHPLayer"].tag,true)
		
		--隐藏回城按钮
		TXGUI.UIManager:sharedManager():getUILayout("BattleUI"):FindChildObjectByName("backToTown").__UIButton__:setVisible(false)
	end			
end



function OnWorldBossEndHideLayer()
	HideOneLayerByTag(UITagTable["GameUILayer"].tag)
	HideOneLayerByTag(UITagTable["BattleUILayer"].tag)
	HideOneLayerByTag(UITagTable["ChatToolbar"].tag)

	HideOneLayerByTag(UITagTable["WorldBossFailLayer"].tag)
	HideOneLayerByTag(UITagTable["WorldBossRemainTimeLayer"].tag)
	HideOneLayerByTag(UITagTable["WorldBossDamageLayer"].tag)
	HideOneLayerByTag(UITagTable["WorldBossHPLayer"].tag)

	HSJoystick:sharedJoystick():setIsEnable(false)
end

function GetWorldBossDamagePercentStr(percentage)
	local hurtPercentStr = ""

	if percentage < 1000 then
		hurtPercentStr = hurtPercentStr.." "
	end

	hurtPercentStr = hurtPercentStr..(percentage / 100)

	if percentage % 100 == 0 then
		hurtPercentStr = hurtPercentStr..".00"
	end

	hurtPercentStr = hurtPercentStr.."%"

	return hurtPercentStr
end

function HideWorldBossFailLayer()
	CCLuaLog("-- HideWorldBossFailLayer --")
	WorldBossFailLayer:destroyed()

	ShowOneLayerByTag(UITagTable["BattleUILayer"].tag)
	HSJoystick:sharedJoystick():setIsEnable(true)
	return true
end

function SetPortalVisible(isVisible)
	LevelManager:sShareInstance():getCurrentLevelLayer():getObjectLayer():getChildByTag(109):setVisible(isVisible)
end