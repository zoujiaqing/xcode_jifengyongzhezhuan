require("Script/Marquee/MarqueeUILayer")
require("Script/handleMessage")
MarqueeDataCenter = {
	index = 0,
	maxMarqueeNum = 10,
	isInit = false,
	updateFunc = 0,
	marqueeMsgTable = {},
	formatTable = {},
}

local function marqueeSort(a,b)
	return a.index < b.index
end

local function checkMarqueeNum(num)
	if num > MarqueeDataCenter.maxMarqueeNum then
		local count = table.getn(MarqueeDataCenter.marqueeMsgTable)
		table.sort(MarqueeDataCenter.marqueeMsgTable,marqueeSort)
		while count > MarqueeDataCenter.maxMarqueeNum do
			table.remove(MarqueeDataCenter.marqueeMsgTable,1)
			count = table.getn(MarqueeDataCenter.marqueeMsgTable)
		end
	end
end

local function updataMarqueeMessage()
	local count = table.getn(MarqueeDataCenter.marqueeMsgTable)
	checkMarqueeNum(count)
	local marqueeLayer = MarqueeUILayer:CreateLayer()
	if count > 0 and marqueeLayer:checkMarqueeStatus() then
		table.sort(MarqueeDataCenter.marqueeMsgTable,marqueeSort)
		local msg = MarqueeDataCenter.marqueeMsgTable[1]
		marqueeLayer:ShowMarquee(msg)
		table.remove(MarqueeDataCenter.marqueeMsgTable,1)
	end
end

function clearMarqueeData()
	if MarqueeDataCenter.isInit then
		if MarqueeDataCenter.updateFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(MarqueeDataCenter.updateFunc)
			MarqueeDataCenter.updateFunc = 0
		end
		for k,v in pairs(MarqueeDataCenter.marqueeMsgTable) do 
			if v ~= nil then
				v:destroy()
				MarqueeDataCenter.marqueeMsgTable[k] = nil
			end
		end
		MarqueeDataCenter.marqueeMsgTable = {}
		MarqueeDataCenter.index = 0
		MarqueeDataCenter.isInit = false
	end
end

local function onReceiveCSMaAdvertisingRsp(msg)
	CCLuaLog("onReceiveCSMaAdvertisingRsp")
	if msg.content ~= nil then
		MarqueeDataCenter.index = MarqueeDataCenter.index + 1
		local marMsg = MarqueeMessage:create(MarqueeDataCenter.index,msg.opt)
		for k,v in pairs(msg.content) do
			local color = ccc3(v.red,v.green,v.blue)
			local str = v.text
			marMsg:addText(color,str)
		end
		table.insert(MarqueeDataCenter.marqueeMsgTable,marMsg)
	end
end

local function getMarqueeFormatById(marquee_id)
	if MarqueeDataCenter.formatTable[marquee_id] == nil then
		MarqueeDataCenter.formatTable[marquee_id] = MarqueeFormat:createByID(marquee_id)
	end
	return MarqueeDataCenter.formatTable[marquee_id]
end

local function onReceiveCSMarqueeRsp(msg)
	--CCLuaLog("onReceiveCSMarqueeRsp")
	if msg ~= nil then
		local marquee_id = msg.marquee_id
		local priority = msg.priority
		local text1 = msg.player1
		local text2 = msg.player2
		local item_id = msg.item_id
		local num = msg.number
		MarqueeDataCenter.index = MarqueeDataCenter.index + 1
		local marFormat = getMarqueeFormatById(marquee_id)
		local marMsg = MarqueeMessage:create(MarqueeDataCenter.index,priority)
		marMsg:addTextByFormat(marFormat,text1,text2,item_id,num)
		--local marMsg = MarqueeMessage:createByID(MarqueeDataCenter.index,priority,marquee_id,text1,text2,item_id,num)
		if marMsg ~= nil then
			table.insert(MarqueeDataCenter.marqueeMsgTable,marMsg)
		end
	end
end

function initMarqueeData()
	if not MarqueeDataCenter.isInit then
		addMsgCallBack("CSMaAdvertisingRsp", onReceiveCSMaAdvertisingRsp)
		addMsgCallBack("CSMarqueeRsp", onReceiveCSMarqueeRsp)
		MarqueeDataCenter.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updataMarqueeMessage, 0.1, false)
		MarqueeDataCenter.index = 0
		MarqueeDataCenter.isInit = true
	end
	return true
end
