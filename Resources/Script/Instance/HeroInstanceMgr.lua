-- 勇者试炼相关函数
require("Script/Instance/HeroInstanceListLayer")
require("Script/Instance/HeroInstanceWinLayer")

HeroInstanceMgr = {}

function SendHeroInstanceCreateReq(instanceId)
	CSBattleCreateReq = {
		battle_id = instanceId,
		map_id = 0,
		}
	local msgname="CSBattleCreateReq"
	local ret = send_message(msgname, CSBattleCreateReq, true)
	return ret
end

local function onMsgHeroInstanceWinRsp(msg)
	CCLuaLog("--- onMsgHeroInstanceWinRsp ---")
    if msg ~= nil then
        HeroInstanceWinLayer.rewardItems = {}
        if msg.rewards == nil then
            CCLuaLog("--- msg.rewards == nil ---")
        end
        for _ , item in pairs(msg.rewards) do
            local itemData = {
                id = item.item_id,
                count = item.item_num,
                factor = item.factor / 100,
            }
            table.insert(HeroInstanceWinLayer.rewardItems, itemData)
        end

        HeroInstanceWinLayer:CreateLayer()
    end
end

function sendHeroInstanceFactorReq()
	CSBabelFactorReq = {
		}
	local msgname="CSBabelFactorReq"
	local ret = send_message(msgname, CSBabelFactorReq, true)
	return ret
end

local function onMsgHeroInstanceFactorRsp(msg)
	CCLuaLog("--- onMsgHeroInstanceFactorRsp ---")
    if msg ~= nil then
        HeroInstanceWinLayer.factors = {}
        for _ , factor in pairs(msg.factors) do
            table.insert(HeroInstanceWinLayer.factors, factor / 100)
        end
    end
end

function HeroInstanceMgr:Init()
    sendHeroInstanceFactorReq()

    addMsgCallBack("CSBabelRewardRsp", onMsgHeroInstanceWinRsp)	
    addMsgCallBack("CSBabelFactorRsp", onMsgHeroInstanceFactorRsp)	
end

function OnEnterHeroInstance()
    showUILayerByTag(UITagTable["WorldBossHPLayer"].tag,true)

    --隐藏回城按钮
    --TXGUI.UIManager:sharedManager():getUILayout("BattleUI"):FindChildObjectByName("backToTown").__UIButton__:setVisible(false)
end