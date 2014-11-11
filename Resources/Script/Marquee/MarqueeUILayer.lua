require("Script/Marquee/MarqueeLabel")
require("Script/Marquee/MarqueeMessage")
MarqueeUILayer = 
{
	uiLayerInstance = nil,
	uiLayout = nil,
	messageTable = {},
	backPickSize   = CCSizeMake(0,0) ,
	updateFunc = nil,
	marqueeLabel = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if MarqueeUILayer.updateFunc ~= nil then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(MarqueeUILayer.updateFunc)
		MarqueeUILayer.updateFunc = nil
	end
	
	for k,v in pairs(MarqueeUILayer.messageTable) do 
		if v ~= nil then
			v:destroy()
			MarqueeUILayer.messageTable[k] = nil
		end
	end
	MarqueeUILayer.messageTable = {}

	if MarqueeUILayer.marqueeLabel ~= nil then
		MarqueeUILayer.marqueeLabel:destroy()
	end
	MarqueeUILayer.marqueeLabel = nil
	MarqueeUILayer.uiLayerInstance = nil
	MarqueeUILayer.uiLayout = nil
	
end

function MarqueeUILayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		--CCLuaLog("MarqueeUILayer:CreateLayer()")
		local tag = UITagTable["MarqueeUILayer"].tag
		self.uiLayerInstance = showUILayerByTag(tag,true)
		if self.uiLayerInstance ~= nil then
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self:InitLayer()
		end
	end

	return self
end

local function marqueeSort(a,b)
	if a.priority == b.priority then
		return a.index < b.index
	else
		return a.priority > b.priority
	end
end

local function marqueeLayerUpdate(dt)
	local count = table.getn(MarqueeUILayer.messageTable)
	if count > 0 and MarqueeUILayer.marqueeLabel:isReadyForMarquee() then
		table.sort(MarqueeUILayer.messageTable,marqueeSort)
		local msg = MarqueeUILayer.messageTable[1]
		if msg ~= nil then
			MarqueeUILayer.marqueeLabel:setLabel(msg)
			msg:destroy()
			MarqueeUILayer.messageTable[1] = nil
		end
		table.remove(MarqueeUILayer.messageTable,1)
	end
end

function MarqueeUILayer:checkMarqueeStatus()
	if self.uiLayerInstance == nil or self.marqueeLabel == nil then
		return false
	end
	return self.marqueeLabel:isReadyForMarquee()
end

function MarqueeUILayer:ShowMarquee(marquee)
	table.insert(self.messageTable,marquee)
end

function MarqueeUILayer:InitLayer()
	--backPickSize = CCSizeMake(600,400);
	self.uiLayout = TXGUI.UIManager:sharedManager():getUILayout("MarqueeLayout")
	local pic = self.uiLayout:FindChildObjectByName("frameBG").__UIPicture__
	--local label = self.uiLayout:FindChildObjectByName("marqueeLabel").__UILabel__
	local rootNode = self.uiLayout:FindChildObjectByName("marqueeBG").__UIPicture__
	local baseNode = pic:getCurrentNode()
	backPickSize = baseNode:getContentSize()
    print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 3")
	self.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(marqueeLayerUpdate, 1.0, false)

	self.marqueeLabel = MarqueeLabel:create(baseNode)
	--self.marqueeLabel:init(container)
end