require("Script/Language")
require("Script/CommonDefine")
require("Script/DeathTip/deathTip")
require("Script/DeathTip/deathTipNetWorkHandler")

DeathTipSystem = {
	isBackToTown = false,
	id = nil
}

function DeathTipSystem:SetBackToTown()
	self.isBackToTown = true
end

function DeathTipSystem_SetBackToTown()
	DeathTipSystem:SetBackToTown()
	return true
end

function DeathTipSystem:GetLookFace(id)
	local item = End_Prompt[id]
	if item == nil then
		return nil
	end

	local lookFace = "map_ui_system_icon_FRAME_" .. string.upper(item.LookFace)
	return lookFace
end

function DeathTipSystem:GetDisplayText(id)
	local item = End_Prompt[id]
	if item == nil then
		return nil
	end

	local flag = item.Text
	return flag
end

function DeathTipSystem:GetOkBtnText()
	local flag = "M_CONFIRM"
	return flag
end

local function DeathTipSystem_OkBtnClick()
	local id = DeathTipSystem.id
	if id~= nil then
		local item = End_Prompt[id]
		local openLayerName = item.OpenLayerName
		if openLayerName ~= nil and openLayerName ~= "" then
			local isAutoCreate = true;
			if openLayerName == "ElfExploerLayer" or openLayerName == "DailyTaskLayer" or openLayerName == "SkillSelectLayer" then
				print("layer " .. openLayerName .. "close")
			else
				local layer = showUILayerByName(openLayerName,isAutoCreate)
				if openLayerName == "NormalFBTranspoint" or openLayerName == "EliteFBTranspoint" or openLayerName == "EnterUnderCity" then
					PushOneLayer(nil,"","")	
				end
			end
		end
	end
end

function DeathTipSystem:ShowTipDialog(id)
	self.id = id
	local iconName = self:GetLookFace(id)
	local content = self:GetDisplayText(id)
	local okBtnContent = self:GetOkBtnText()
	PopupRewardLayer:ShowByIconName(content,iconName,okBtnContent,false,false,DeathTipSystem_OkBtnClick,nil,6)
end

local function updateNotification(message)
	if message == GM_E_ENTER_MAINLAND_COLOR_LAYER_OVER then
		print("***************************** enter main land")
		--Note: get player health		
		local health_point = UserData:getHeroHp()
		local inWorldBossEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_WORLDBOSS)
		local inBestFighterEntrance = LevelManager:sShareInstance():getCurMapId() == InstanceManager:Get():getSpecialEnteranceMapId(ENTERANCE_BESTFIGHTER)
		if 0 == health_point and not inWorldBossEntrance and not inBestFighterEntrance then
			--print("***************************** hero health point 0")	
			--print("------------------------------------------- send player may improve req")
			if DeathTipSystem.isBackToTown then
				DeathTipNetWorkHandler:SendCSPlayerMayImproveReq()
			end			
		end
		DeathTipSystem.isBackToTown = false
	end	
end

local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);