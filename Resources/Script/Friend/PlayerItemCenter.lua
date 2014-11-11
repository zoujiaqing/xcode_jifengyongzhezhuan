require("Script/Friend/PlayerTotalItem")
require("Script/handleMessage")
PlayerItemCenter = {
	playerItemList = {},
	selectedId = nil,
	receiveItemCallBack = nil,
	recieveAttrCallBack = nil,
	recieveWeaponCallBack = nil,
	updateItemFunc = 0,
	updateAttrFunc = 0,
	reqMsgIndex = 0,
}

function PlayerItemCenter:setSelectedPlayer(id)
	self.selectedId = id
end

function PlayerItemCenter:getSelectedPlayer()
	return self.selectedId
end

function PlayerItemCenter:clearOtherPlayerItems()
	self.selectedId = nil
	for k,v in pairs(self.playerItemList) do
		if v ~= nil then
			v:resetValue()
			self.playerItemList[k] = nil
		end
	end
	self.playerItemList = {}
end

local function updateItemInfo()
	CCLuaLog("updateItemInfo")
	PlayerItemCenter.reqMsgIndex = PlayerItemCenter.reqMsgIndex - 1
	if PlayerItemCenter.reqMsgIndex == 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(PlayerItemCenter.updateItemFunc)
		PlayerItemCenter.updateItemFunc = 0
	end
	if PlayerItemCenter.receiveItemCallBack ~= nil and PlayerItemCenter.selectedId ~= nil then
		PlayerItemCenter.receiveItemCallBack()
	end
	if PlayerItemCenter.recieveWeaponCallBack ~= nil then
		PlayerItemCenter.recieveWeaponCallBack()
	end
end

local function updateAttrInfo()
	CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(PlayerItemCenter.updateAttrFunc)
	PlayerItemCenter.updateAttrFunc = 0
	if PlayerItemCenter.recieveAttrCallBack ~= nil then
		PlayerItemCenter.recieveAttrCallBack()
	end
end

function sendGetUserItemInfo(id,reg,channel)
	CCLuaLog("sendGetUserItemInfo")
	CSGetUserItemReq = {
		userid = id,
		reg_time = reg,
		channel_id = channel,
	}
	local msgname="CSGetUserItemReq"
	WaitingLayerManager:Get():PushOneMessage(msgname)
	local ret = send_message(msgname, CSGetUserItemReq, true)
	return ret
end

function sendCSGetUserInfoReq(id,reg,channel)
	CSGetUserInfoReq = {
		userid = id,
		reg_time = reg,
		channel_id = channel,
	}
	local msgname="CSGetUserInfoReq"
	WaitingLayerManager:Get():PushOneMessage(msgname)
	local ret = send_message(msgname, CSGetUserInfoReq, true)
	return ret
end

local function setUserEffectiveness(id,num)
	if PlayerItemCenter.playerItemList ~= nil then
		local playerItems = nil
		for k,v in pairs(PlayerItemCenter.playerItemList) do
			if id:isEqual(k) then
				playerItems = v
				break;
			end
		end

		if playerItems ~= nil then
			playerItems:SetEffectiveness(num)
		else
			playerItems = PlayerTotalItem:create()
			playerItems:setId(id)
			playerItems:SetEffectiveness(num)
			PlayerItemCenter.playerItemList[id] = playerItems
		end
	end
end

local function setUserExp(id,exp)
	if PlayerItemCenter.playerItemList ~= nil then
		local playerItems = nil
		for k,v in pairs(PlayerItemCenter.playerItemList) do
			if id:isEqual(k) then
				playerItems = v
				break;
			end
		end

		if playerItems ~= nil then
			playerItems:setExp(exp)
		else
			playerItems = PlayerTotalItem:create()
			playerItems:setId(id)
			playerItems:setExp(exp)
			PlayerItemCenter.playerItemList[id] = playerItems
		end
	end
end

function sendCSGetUserAttributesReq(id,regTime,channel)
	CSGetUserAttributesReq = {
		userid = id,
		reg_time = regTime,
		channel_id = channel,
	}
	local msgname="CSGetUserAttributesReq"
	WaitingLayerManager:Get():PushOneMessage(msgname)
	local ret = send_message(msgname, CSGetUserAttributesReq, true)
	return ret
end

local function receiveCSGetUserAttributesRsp(msg)
	-- to do PLAYER_ID
	local uid = msg.userid
	local regTime = msg.reg_time
	local channelId = msg.channel_id
	--local playerId = PLAYER_ID:new()
	--playerId:setPlayId(uid,regTime,channelId)
	local playerId = PlayerIdMake(uid,regTime,channelId)
	if msg.attributes ~= nil then
		local values = msg.attributes.values
		for k,v in pairs(values) do
			if v.key == PB_ATTR_EXP and v.value ~= nil then
				setUserExp(playerId,v.value)
			elseif v.key == PB_ATTR_EFFECTIVENESS and v.value ~= nil then
				setUserEffectiveness(playerId,v.value)
				local userInfo = UserData:Get():GetUserInfo(playerId) 
				if userInfo ~= nil then
					userInfo.effectiveness = v.value
					UserData:Get():SetUserInfo(playerId,userInfo)
				end
			end
		end
	end
	-- 下一帧更新UI响应
	if PlayerItemCenter.selectedId ~= nil and (playerId:isEqual(PlayerItemCenter.selectedId)) and (PlayerItemCenter.updateAttrFunc == 0) then
        --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun receiveCSGetUserAttributesRsp")
		PlayerItemCenter.updateAttrFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateAttrInfo, 0.01, false)
	end
	return true
end

local function receiveCSGetUserItemInfoRsp(msg)
	--s to do PLAYER_ID
	local playerItems = nil
	--local itemId = PLAYER_ID:new()
	--itemId:setPlayId(msg.userid,msg.reg_time,msg.channel_id)
	local itemId = PlayerIdMake(msg.userid,msg.reg_time,msg.channel_id)
	for k,v in pairs(PlayerItemCenter.playerItemList) do
		if itemId:isEqual(k) then
			playerItems = v
			break;
		end
	end
	if playerItems ~= nil then
		playerItems:resetItemList()
	else
		playerItems = PlayerTotalItem:create()
		playerItems:setId(itemId)
	end

	if msg.items ~= nil then
		for k,v in pairs(msg.items) do
			local itemInfo = msg.items[k]
			playerItems:addItem(itemInfo.pos,itemInfo)
		end
	end
	PlayerItemCenter.playerItemList[itemId] = playerItems		

	-- 下一帧更新UI响应
	--if (itemId:isEqual(PlayerItemCenter.selectedId)) and (PlayerItemCenter.updateItemFunc == 0) then
	if PlayerItemCenter.reqMsgIndex == 0 then
        --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun receiveCSGetUserItemInfoRsp")
		PlayerItemCenter.updateItemFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateItemInfo, 0.01, false)
	end
	PlayerItemCenter.reqMsgIndex = PlayerItemCenter.reqMsgIndex + 1
	return true
end

function setGetUserWeaponCallback(callbackFunc)
	if PlayerItemCenter.receiveItemCallBack == nil then
		addMsgCallBack("CSGetUserItemRsp", receiveCSGetUserItemInfoRsp)
		PlayerItemCenter.recieveWeaponCallBack = callbackFunc
	end
end

function setGetUserItemCallback(callbackFunc)
	if PlayerItemCenter.receiveItemCallBack == nil then
		addMsgCallBack("CSGetUserItemRsp", receiveCSGetUserItemInfoRsp)
		PlayerItemCenter.receiveItemCallBack = callbackFunc
	end
end

function setGetUserAttrCallback(func)
	if PlayerItemCenter.recieveAttrCallBack == nil then
		addMsgCallBack("CSGetUserAttributesRsp", receiveCSGetUserAttributesRsp)
		PlayerItemCenter.recieveAttrCallBack = func
	end
end

function getPlayerItems(playerId)
	-- to do PLAYER_ID
	local items = nil
	for k,v in pairs(PlayerItemCenter.playerItemList) do
		if playerId:isEqual(k) then
			items = v
			break;
		end
	end
	--if PlayerItemCenter.playerItemList[playerId] ~= nil then
		--items = PlayerItemCenter.playerItemList[playerId]
	--end
	return items
end