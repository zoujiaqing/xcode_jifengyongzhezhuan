
MarqueeLabel = {
	updateFunc = 0,
	rootNode   = nil,
	--textLabel = nil,
	animationNode = nil,
	rootScale	= 1,
	labelStatus = 0,   -- 0: 等待状态 1: 强制显示状态 2: 非强制显示状态 3: 强制显示状态结束 4：非强制显示结束 5：收缩UI 6：收缩UI结束
}

function MarqueeLabel:create(root)
	if root == nil then
		CCLuaLog("root == nil1")
	end
	self:init(root)
	return self
end

local function scrollCallback()
	--CCLuaLog("scrollCallback")
	MarqueeLabel.labelStatus = 6
end

local function forceShowCallback()
	--CCLuaLog("forceShowCallback")
	MarqueeLabel.labelStatus = 3
end

local function pauseCallback()
	--CCLuaLog("pauseCallback")
	MarqueeLabel.labelStatus = 4
end

local function updateLabel(dt)
	if MarqueeLabel.labelStatus == 3 then
		MarqueeLabel:showLabelView()
	elseif MarqueeLabel.labelStatus == 4 then
		MarqueeLabel:zoomContainer()
	elseif MarqueeLabel.labelStatus == 6 then
		MarqueeLabel:stopCurrentMarquee()
	end
end

function MarqueeLabel:showLabelView()
	--CCLuaLog("MarqueeLabel:showLabelView")
	if self.animationNode ~= nil then
		local delayAction =	CCDelayTime:create(2.0)
		local callback = CCCallFunc:create(pauseCallback)
		local action = CCSequence:createWithTwoActions(delayAction,callback)
		self.animationNode:runAction(action)
		self.labelStatus = 2
	end
end

function MarqueeLabel:zoomContainer()
	--CCLuaLog("MarqueeLabel:zoomContainer")
	self.labelStatus = 5
	if self.rootNode ~= nil then
		local scaleAction = CCScaleTo:create(0.5,self.rootScale,self.rootScale * 0.1) 
		local callback = CCCallFunc:create(scrollCallback)
		local action = CCSequence:createWithTwoActions(scaleAction,callback)
		self.rootNode:runAction(action)
	end
end

function MarqueeLabel:stopCurrentMarquee()
	--CCLuaLog("MarqueeLabel:stopCurrentMarquee")
	if self.labelStatus ~= 0 then
		if self.animationNode ~= nil then
			self.animationNode:stopAllActions()
			self.animationNode:removeFromParentAndCleanup(true)
		end
	end
	self.labelStatus = 0
	self.animationNode = nil
	if self.rootNode ~= nil then
		self.rootNode:stopAllActions()
		self.rootNode:setVisible(false)
	end
	--CCLuaLog("stopCurrentMarquee:over")
end

function MarqueeLabel:isReadyForMarquee()
	local flag = false
	if self.labelStatus ~= 1 then
		flag = true
	end
	return flag
end

function MarqueeLabel:setLabel(msg)
	--CCLuaLog("MarqueeLabel:setLabel")
	if self.labelStatus ~= 1 then
		local isNeedOpen = false
		if self.labelStatus == 0 or self.labelStatus == 5 then
			isNeedOpen = true
		end
		self:stopCurrentMarquee()
		self.animationNode = msg:getNode()
		if self.rootNode ~= nil and self.animationNode ~= nil then
			self.rootNode:setVisible(true)	
			self.rootNode:addChild(self.animationNode)
			local rootSize = self.rootNode:getContentSize()
			self.animationNode:setPosition(ccp(rootSize.width * 0.5 - self.animationNode:getContentSize().width * 0.5,rootSize.height * 0.5))
			if isNeedOpen then
				self.rootNode:setScaleX(self.rootScale)
				self.rootNode:setScaleY(0)
				local rootAction = CCScaleTo:create(0.2,self.rootScale,self.rootScale)
				self.rootNode:runAction(rootAction)
			end

			--local scrollAction = CCMoveBy:create(1.5,ccp(length * -1,0))
			local delayAction =	CCDelayTime:create(2.0)
			local callback = CCCallFunc:create(forceShowCallback)
			local action = CCSequence:createWithTwoActions(delayAction,callback)
			self.animationNode:runAction(action)
			self.labelStatus = 1
		end
	end
	
end

function MarqueeLabel:destroy()
	if self.updateFunc ~= 0 then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(self.updateFunc)
		self.updateFunc = 0
	end
	self.rootNode = nil
	self.animationNode = nil
	self.labelStatus = 0
	self.rootScale = 1
end

function MarqueeLabel:init(root)
--print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 2")
	self.updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateLabel, 0.1, false)
	self.rootNode = root
	--self.textLabel = label
	self.rootScale = self.rootNode:getScale()
end