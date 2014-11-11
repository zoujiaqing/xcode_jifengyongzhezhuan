require("Script/Chat/ChatNoteInfo")
require("Script/handleMessage")
ChatDataCenter = {
	WorldNoteList = {}, -- 世界频道消息
	PartyNoteList = {},	-- 公会频道消息
	PersonalNoteList = {}, -- 私聊消息
	TempSendNoteList = {},	-- 发送给服务器，但没有收到确认的消息暂存
	noteIndex = 0,		-- 消息编号，从0开始
	isCallbackFuncFlag = fasle,	-- 是否注册接收函数
	uiWorldNoticeFunc = {},
	uiPersonNoticeFunc = {},
	uiPartyNoticeFunc = {},
	recentChatPlayerId = nil,
	recentChatPlayerName = "",
}

local function getCurrentNoteIndex()
	local index = ChatDataCenter.noteIndex
	ChatDataCenter.noteIndex = index + 1
	return index
end

local function createNoteByMsg(msg)
	if msg.userid ~= nil and msg.reg_tm ~= nil and msg.channel_id ~= nil then
		local userName = UserData:getUserName()
		local note = ChatNoteInfo:create()
		note.noteType = msg.type
		if msg.name == userName then
			note.name = msg.tname
			note.isSend = true
		else
			note.name= msg.name
			note.isSend = false
		end
		note.content = msg.content
		note.targetId = PlayerIdMake(msg.userid,msg.reg_tm,msg.channel_id)
		--note.targetId = PLAYER_ID:new()
		--note.targetId:setPlayId(msg.userid,msg.reg_tm,msg.channel_id)
		if msg.system ~= nil then
			note.isSystem = msg.system
		end
		note.noteIndex = getCurrentNoteIndex()
		--ChatDataCenter.WorldNoteList[note.noteIndex] = note
		-- to do ui
		return note
	end

	return nil
end

local function checkNoteListSize(noteList)
	local length = table.getn(noteList)
	if length > 10 then
		table.remove(noteList,1)
	end
	return length
end

local function receiveWorldNote(msg)
	local note = createNoteByMsg(msg)
	if note ~= nil then
		--ChatDataCenter.WorldNoteList[note.noteIndex] = note
		table.insert(ChatDataCenter.WorldNoteList,note)
		-- notice ui
		for k,v in pairs(ChatDataCenter.uiWorldNoticeFunc) do 
			v.fun(v.ob,note)
		end
	end
	checkNoteListSize(ChatDataCenter.WorldNoteList)
end

local function receivePartyNote(msg)
	local note = createNoteByMsg(msg)
	if note ~= nil then
		table.insert(ChatDataCenter.PartyNoteList,note)
		-- notice ui
		for k,v in pairs(ChatDataCenter.uiPartyNoticeFunc) do 
			v.fun(v.ob,note)
		end
	end
	checkNoteListSize(ChatDataCenter.PartyNoteList)
end

local function receivePersonalNote(msg)
	local note = createNoteByMsg(msg)
	if note ~= nil then
		table.insert(ChatDataCenter.PersonalNoteList,note)
		-- notice ui
		for k,v in pairs(ChatDataCenter.uiPersonNoticeFunc) do 
			v.fun(v.ob,note)
		end
	end
	checkNoteListSize(ChatDataCenter.PersonalNoteList)
	--checkNoteListSize(ChatDataCenter.PersonalNoteList)
end

local function onReceiveNoteMessage(msg)
	--CCLuaLog("onReceiveNoteMessage")
	if msg.type == "WORLD" then
		receiveWorldNote(msg)
	elseif msg.type == "PARTY" then
		receivePartyNote(msg)
	elseif msg.type == "PM" then
		receivePersonalNote(msg)
	end
end 

local function onReceiveNoteRsp(msg)
	--CCLuaLog("onReceiveNoteRsp")

	if msg.seq ~= nil then
		local temp = ChatDataCenter.TempSendNoteList[msg.seq]
		if  temp~= nil then
			if msg.succ == "SUCC" then
				--if temp.noteType == "PM" then
					--table.insert(ChatDataCenter.PersonalNoteList,temp)
					---- notice ui
					--for k,v in pairs(ChatDataCenter.uiPersonNoticeFunc) do 
						--v.fun(v.ob,temp)
					--end
					--checkNoteListSize(ChatDataCenter.PersonalNoteList)
				--end
			elseif msg.succ == "NOTALLOW" then
				local content = LanguageLocalization:GetLocalization("M_BAN_CHAT")
				GameApi:showMessage(content)
			end
			ChatDataCenter.TempSendNoteList[msg.seq] = nil
		end
	end
end

local function checkReceiveFunc()
	if not ChatDataCenter.isCallbackFuncFlag then
		addMsgCallBack("CSNoteRsp", onReceiveNoteRsp)
		addMsgCallBack("CSNoteShow", onReceiveNoteMessage)
		ChatDataCenter.isCallbackFuncFlag = true
	end
end

local function sendNote(note)
	checkReceiveFunc()
	CSNoteReq = {
		type = note.noteType,
		content = note.content,
		dnd = note.dnd,
		seq = note.noteIndex,
		name = note.name,
		}
	if note.targetId ~= nil then
		CSNoteReq.userid = note.targetId:getID()
		CSNoteReq.reg_tm = note.targetId:getRegTime()
		CSNoteReq.channel_id = note.targetId:getChannel()
	end
	ChatDataCenter.TempSendNoteList[note.noteIndex] = note
	local msgname="CSNoteReq"
	local ret = send_message(msgname, CSNoteReq, true)
	return ret
end

function sendWorldNote(content ,dnd)
	--CCLuaLog("sendWorldNote")
	local note = ChatNoteInfo:create()
	note.noteType = "WORLD"
	note.isSend = true
	note.content = content
	note.targetId = nil
	note.dnd = dnd
	note.name = UserData:getUserName()
	note.noteIndex = getCurrentNoteIndex()
	return sendNote(note)
end

function sendGuildNote(content ,dnd)
	--CCLuaLog("sendGuildNote")
	local note = ChatNoteInfo:create()
	note.noteType = "PARTY"
	note.isSend = true
	note.content = content
	note.targetId = nil
	note.dnd = dnd
	note.name = UserData:getUserName()
	note.noteIndex = getCurrentNoteIndex()
	return sendNote(note)
end

function sendPersonalNote(content ,dnd,playerId,playerName)
	--CCLuaLog("sendPersonalNote")
	ChatDataCenter.recentChatPlayerId = playerId
	ChatDataCenter.recentChatPlayerName = playerName
	local note = ChatNoteInfo:create()
	note.noteType = "PM"
	note.isSend = true
	note.content = content
	note.targetId = playerId
	note.name = playerName
	note.dnd = dnd
	note.noteIndex = getCurrentNoteIndex()
	return sendNote(note)
end

function getNotesStore(notetype)
	local noteTable = nil
	if notetype == "WORLD" then
		noteTable = ChatDataCenter.WorldNoteList
	elseif notetype == "PM" then
		noteTable = ChatDataCenter.PersonalNoteList
	elseif notetype == "PARTY" then
		noteTable = ChatDataCenter.PartyNoteList
	end
	return noteTable
end

function registerUINoteNotice(t,func,object)
	checkReceiveFunc()
	if t == "WORLD" then
		ChatDataCenter.uiWorldNoticeFunc[object] = {ob = object, fun = func,}
	elseif t == "PM" then
		ChatDataCenter.uiPersonNoticeFunc[object] = {ob = object, fun = func,}
	elseif t == "PARTY" then
		ChatDataCenter.uiPartyNoticeFunc[object] = {ob = object, fun = func,}
	end
end

function unregisterUINoteNotice(object)
	ChatDataCenter.uiWorldNoticeFunc[object] = nil
	ChatDataCenter.uiPersonNoticeFunc[object] = nil
	ChatDataCenter.uiPartyNoticeFunc[object] = nil
end

function reqToPrivateChat(playerId,playerName)
	showUILayerByTag(UITagTable["ChatUILayer"].tag,true)
	ChatLayer:startPrivateChat(playerId,playerName)
end

function getRecentChatPlayerId()
	return ChatDataCenter.recentChatPlayerId
end

function getRecentChatPlayerName()
	return ChatDataCenter.recentChatPlayerName
end