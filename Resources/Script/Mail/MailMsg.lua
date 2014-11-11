require("Script/handleMessage")

MailInfoList = {}
MailDetailList = {}

local function MailSort(a,b)
	return a.index < b.index
end

function sendGetMailListReq(startIndex,endIndex)
	CCLuaLog("sendGetMailListReq:" .. startIndex .. "," .. endIndex)
	CSGetMailListReq = {
		index_start = startIndex,
		index_over = endIndex,
	}
	local msgname = "CSGetMailListReq"
	local ret = send_message(msgname, CSGetMailListReq, true)
	return ret
end

function sendGetMailInfoReq(idHigh,idLow)
	CSGetMailInfoReq = {
		mail_id_high = idHigh,
		mail_id_low = idLow,
	}
	local msgname="CSGetMailInfoReq"
	local ret = send_message(msgname, CSGetMailInfoReq, true)
	return ret
end

--function sendSetMailStatusReq(idHigh,idLow,status)
	--CSSetMailStatusReq = {
	--	mail_id_high = idHigh,
	--	mail_id_low = idLow,
	--	new_status = status,
	--}

function sendSetSingleMailStatusReq(idHigh,idLow,status)
	local statusTable = {}
	local msgInfo = {}
	msgInfo.idHigh = idHigh
	msgInfo.idLow = idLow
	msgInfo.status = status
	table.insert(statusTable,msgInfo)
	sendSetMailStatusReq(statusTable)
end

function sendSetMailStatusReq(statusTable)
	CSSetMailStatusReq = {
		mails = {},
	}
	for k,v in pairs(statusTable) do
		MailStatus = {
			mail_id_high = v.idHigh,
			mail_id_low = v.idLow,
			new_status = v.status,
		}
		table.insert(CSSetMailStatusReq.mails,MailStatus)
	end
	local msgname="CSSetMailStatusReq"
	local ret = send_message(msgname, CSSetMailStatusReq, true)
	return ret
end

local function onReceiveMailListRsp(msg)
	CCLuaLog("onReceiveMailListRsp")
	if msg ~= nil and msg.base_mails ~= nil then
		MailInfoList = {}
		for k,v in pairs(msg.base_mails) do
			if v.index ~= nil and v.mail ~= nil then
				local mailInfo = {}
				mailInfo.mail_id_high = v.mail.mail_id_high
				mailInfo.mail_id_low = v.mail.mail_id_low
				mailInfo.mail_status = v.mail.mail_status
				mailInfo.title = v.mail.title
				mailInfo.has_attach = v.mail.has_attach
				mailInfo.is_sys_mail = v.mail.is_sys_mail
				mailInfo.index = v.index
				MailInfoList[v.index] = mailInfo
			end
		end
		table.sort(MailInfoList,MailSort)
		MailUILayer:freshMailInfoList()
	end
end

local function onReceiveMailInfoRsp(msg)
	CCLuaLog("onReceiveMailInfoRsp")
	if msg.mail ~= nil then
		local mailInfo = {}
		mailInfo.mail_id_high = msg.mail.mail_id_high
		mailInfo.mail_id_low = msg.mail.mail_id_low
		mailInfo.title = msg.mail.title
		mailInfo.from = msg.mail.from
		mailInfo.content = msg.mail.content
		mailInfo.mail_status = msg.mail.mail_status
		mailInfo.is_sys_mail = msg.mail.is_sys_mail
		if msg.mail.items ~= nil then
			mailInfo.items = {}
			for k,v in pairs(msg.mail.items) do
				local item ={}
				item.item_num = v.item_num
				item.item_id = v.item_id
				table.insert(mailInfo.items,item)
			end
		end
		table.insert(MailDetailList,mailInfo)
		MailUILayer:showMailDetail(mailInfo)
	end
end

local function setMailInfoStausById(id_high,id_low,status)
	local index = 0
	if MailDetailList ~= nil then	
		for k,v in pairs(MailInfoList) do
			if v.mail_id_high == id_high and v.mail_id_low == id_low then
				index = k
				break
			end
		end
	end
	if index ~= 0 then
		MailInfoList[index].mail_status = status
	end
end

local function setMailDetalStausById(id_high,id_low,status)
	local index = 0
	if MailDetailList ~= nil then	
		for k,v in pairs(MailDetailList) do
			if v.mail_id_high == id_high and v.mail_id_low == id_low then
				index = k
				break
			end
		end
	end
	if index ~= 0 then
		if status == 4 then
			table.remove(MailDetailList,index)
		else
			MailDetailList[index].mail_status = status
		end
	end
end

local function onReceiveMailStatusRsp(msg)
	if msg ~= nil and msg.mails ~= nil then
		local deleteFlag = false
		local getAttachFlag = false
		for k,v in pairs(msg.mails) do
			if v.new_status == PB_MAIL_STATUS_DEL then
				deleteFlag = true
			end
			if v.new_status == PB_MAIL_STATUS_GOT then
				getAttachFlag = true
			end
				-- 已删除
				--MailInfoList = {}
			if v.new_status ~= PB_MAIL_STATUS_DEL then
				setMailInfoStausById(v.mail_id_high,v.mail_id_low,v.new_status)
			end
			setMailDetalStausById(v.mail_id_high,v.mail_id_low,v.new_status)
			MailUILayer:setCurrentMailStatus(v.mail_id_high,v.mail_id_low,v.new_status)
		end
		if deleteFlag then
			MailInfoList = {}
			sendGetMailListReq(MailUILayer.startMailIndex,MailUILayer.overMailIndex)
		elseif getAttachFlag then
			MailUILayer:onReceiveAttachmentGot()
		end
	end
end

function getMailDetailById(id_high,id_low)
	local info = nil
	if MailDetailList ~= nil then	
		for k,v in pairs(MailDetailList) do
			if v.mail_id_high == id_high and v.mail_id_low == id_low then
				info = v
				break
			end
		end
	end
	return info
end

function registerMailCallBack()
	addMsgCallBack("CSGetMailListRsp",   onReceiveMailListRsp)
	addMsgCallBack("CSGetMailInfoRsp",   onReceiveMailInfoRsp)
	addMsgCallBack("CSSetMailStatusRsp",   onReceiveMailStatusRsp)
end

function unRegisterMailInfo()
	MailInfoList = {}
	MailDetailList = {}
	removeMsgCallBack("CSGetMailListRsp",   onReceiveMailListRsp)
	removeMsgCallBack("CSGetMailInfoRsp",   onReceiveMailInfoRsp)
	removeMsgCallBack("CSSetMailStatusRsp",   onReceiveMailStatusRsp)
end