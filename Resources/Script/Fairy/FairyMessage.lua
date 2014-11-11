require("Script/handleMessage")
require("Script/GameConfig/uiTagDefine")

function sendGetFairyListReq(userid,reg_time,channel_id)
	--CCLuaLog("sendGetFairyListReq")
	CSGetFairyListReq = {
		userid = userid,
		reg_time = reg_time,
		channel_id = channel_id,
	}
	local msgname = "CSGetFairyListReq"
	local ret = send_message(msgname, CSGetFairyListReq, true)
	return ret
end

function sendMoveFairyPosReq(fromPos,toPos)
	--CCLuaLog("sendMoveFairyPosReq")
	CSMoveFairyPosReq = {
		from_pos = fromPos,
		to_pos = toPos,
	}
	local msgname = "CSMoveFairyPosReq"
	local ret = send_message(msgname, CSMoveFairyPosReq, true)
	WaitingLayerManager:Get():PushOneMessage(msgname)
	return ret
end

function sendCALBattleFairyReq()
	CSCALBattleFairyReq = {
	}
	local msgname = "CSCALBattleFairyReq"
	local ret = send_message(msgname, CSCALBattleFairyReq, true)
	return ret
end

-- 分解精灵请求,isPreview 是否预览
function sendFairyDecomposeReq(fairyPosList,isPreview,isFragments)
	CSFairyDecomposeReq = {
		fairy_pos = {},
		req_info = isPreview,
        is_fragments = isFragments,
	}
	for k,v in pairs(fairyPosList) do
		table.insert(CSFairyDecomposeReq.fairy_pos,v)
	end
	local msgname = "CSFairyDecomposeReq"
	local ret = send_message(msgname, CSFairyDecomposeReq, true)
	return ret
end

local function onReceiveFairyDecomposeRsp(msg)
	if msg ~= nil then
		FairyDecomposeLayer:onReceiveFairyDecomposeRsp(msg)
	end
end

-- 分解精灵请求,isPreview 是否预览
function sendFairyRebirthReq(fairyPosList,isPreview)
	CSFairyRebirthReq = {
		fairy_pos = {},
		req_info = isPreview,
	}
	for k,v in pairs(fairyPosList) do
		table.insert(CSFairyRebirthReq.fairy_pos,v)
	end
	local msgname = "CSFairyRebirthReq"
	local ret = send_message(msgname, CSFairyRebirthReq, true)
	return ret
end

-- 请求精灵是否已获取状态
function sendFairyCollectionReq(startIndex,endIndex)
	--CCLuaLog("sendFairyCollectionReq: " .. startIndex .. "," .. endIndex)
	CSAttributesReq = {
		keys = {},
	}
	for i = startIndex,endIndex do
		table.insert(CSAttributesReq.keys,i)
	end
	local msgname = "CSAttributesReq"
	local ret = send_message(msgname, CSAttributesReq, true)
	return ret
end

local function onReceiveFairyCollectionRsp(msg)
	if msg ~= nil and msg.values ~= nil then
		for k,v in pairs(msg.values) do
			--CCLuaLog("onReceiveFairyCollectionRsp: " .. v.key .. "," .. v.value)
			if v.key >= PB_ATTR_FAIRY_COLLECTION_BEGIN and v.key <= PB_ATTR_FAIRY_COLLECTION_END then
				setCollectionFairyStatus(v.key,v.value)
			end
		end
	end
end

local function onReceiveFairyRebirthRsp(msg)
	--CCLuaLog("onReceiveFairyRebirthRsp")
	if msg ~= nil then
		FairyRebirthLayer:onReceiveFairyRebirthRsp(msg)
	end
end


local function onReceiveGetFairyListRsp(msg)
	CCLuaLog("onReceiveGetFairyListRsp")
	if msg ~= nil then
		local userid = msg.userid
		local reg_time = msg.reg_time
		local channel_id = msg.channel_id
		local playerId = PlayerIdMake(userid,reg_time,channel_id)

		if playerId:isEqual(UserData:getUserId()) then
			resetUserFairyList(msg.fairy_infos)
		else
			setOtherUserFairyList(playerId,msg.fairy_infos)
		end
	end
end

local function onReceiveMoveFairyPosRsp(msg)
	if msg ~= nil then
		exchangeFairyPos(msg.from_pos,msg.to_pos)
	end
end

local function onReceiveAddFairyRsp(msg)
	if msg ~= nil then
		addUserFairy(msg.fairy_info)
	end
end

local function onReceiveUpdateFairyRsp(msg)
	if msg ~= nil then
		updateUserFairys(msg.fairy_pos,msg.fairy_basic,msg.fairy_attrs,msg.fairy_train_attrs,msg.fairy_rel_ids,msg.fairy_effectiveness)
	end
end

local function onReceiveDelFairyRsp(msg)
	if msg ~= nil then
		if msg.fairy_pos ~= nil then
			local isDeleteNewFairy = false
			for k,v in pairs(msg.fairy_pos) do
				local deleteNew = deleteUserFairy(v)
				isDeleteNewFairy = isDeleteNewFairy or deleteNew
			end
			if isDeleteNewFairy then
				NotificationCenter:defaultCenter():broadcast(GM_ATTR_NEW_FAIRY_INFO,nil,"","")
			end
		end
	end
end

local function onReceiveFairyComposeRsp(msg)
	if msg ~= nil then
		if msg.result == 0 then
			FairyBagLayer:onReceiveFairyComposeRsp(msg.exchange_item_id)
		end
	end
end

local function onReceiveCALBattleFairyRsp(msg)
	if msg ~= nil then
		FairyBagLayer:onReceiveFairyEquipedRsp()
	end
end

local function onConfirmGoToRebirth(tag)
	showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
	FairySelectLayer:createLayer()
	FairySelectLayer:setRebirthModle()
end

local function onReceiveShowRebirthNotify(msg)
	if msg ~= nil then
		if msg.show then
			local str = LanguageLocalization:GetLocalization("M_EXCHANGE_UPGRADEFAIRY_HINT")
			MessageBox:Show(str,onConfirmGoToRebirth,nil,MB_YESNO,ccc3(255,255,255),24)
		end
	end
end

local function onReceiveNewFairyNote(msg)
	if msg ~= nil and msg.fairys ~= nil then
		for k,v in pairs(msg.fairys) do
			CCLuaLog("onReceiveNewFairyNote: " .. v)
			setUserFairyNewFlag(v,true)
		end
	end
end

function registerFairyCallBack()
	addMsgCallBack("CSGetFairyListRsp",   onReceiveGetFairyListRsp)
	addMsgCallBack("CSMoveFairyPosRsp",   onReceiveMoveFairyPosRsp)
	addMsgCallBack("CSNotifyAddFairyRsp",   onReceiveAddFairyRsp)
	addMsgCallBack("CSNotifyUpdateFairyRsp",   onReceiveUpdateFairyRsp)
	addMsgCallBack("CSNotifyDelFairyRsp",   onReceiveDelFairyRsp)
	addMsgCallBack("CSEquipmentComposeRsp",   onReceiveFairyComposeRsp)
	addMsgCallBack("CSFairyDecomposeRsp",   onReceiveFairyDecomposeRsp)
	addMsgCallBack("CSFairyRebirthRsp",   onReceiveFairyRebirthRsp)
	addMsgCallBack("CSCALBattleFairyRsp",   onReceiveCALBattleFairyRsp)
	addMsgCallBack("CSShowRebirthNotify",   onReceiveShowRebirthNotify)
	addMsgCallBack("CSNewFairyNotify",   onReceiveNewFairyNote)
	addMsgCallBack("CSAttributesRsp",   onReceiveFairyCollectionRsp)

	local userid = UserData:getUserId()
	sendGetFairyListReq(userid:getID(),userid:getRegTime(),userid:getChannel())
end

function unRegisterFairyCallBack()
	removeMsgCallBack("CSGetFairyListRsp",   onReceiveGetFairyListRsp)
	removeMsgCallBack("CSMoveFairyPosRsp",   onReceiveMoveFairyPosRsp)
	removeMsgCallBack("CSNotifyAddFairyRsp",   onReceiveAddFairyRsp)
	removeMsgCallBack("CSNotifyUpdateFairyRsp",   onReceiveUpdateFairyRsp)
	removeMsgCallBack("CSNotifyDelFairyRsp",   onReceiveDelFairyRsp)
	removeMsgCallBack("CSFairyDecomposeRsp",   onReceiveFairyDecomposeRsp)
	removeMsgCallBack("CSFairyRebirthRsp",   onReceiveFairyRebirthRsp)
	removeMsgCallBack("CSCALBattleFairyRsp",   onReceiveCALBattleFairyRsp)
	removeMsgCallBack("CSShowRebirthNotify",   onReceiveShowRebirthNotify)
	removeMsgCallBack("CSNewFairyNotify",   onReceiveNewFairyNote)
	removeMsgCallBack("CSAttributesRsp",   onReceiveFairyCollectionRsp)
end
