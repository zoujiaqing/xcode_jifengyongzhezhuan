LuaPlayerInfo =	{
	totalHp = 0,
	currentHp = 0,
	uiNode = nil,
	hpBar = nil,
	type = nil,
	name = "",
	id = nil,
	currentRagePoint = 0,
	totalRagePoint = 1000,
	secCurrentRagePoint = 0,
	secTotalRagePoint = 1000,
	rageBar = nil,
	rageBackBar = nil,
	rageBall = nil,
	rageBallFull = nil,
	secRageBar = nil,
	secRageBackBar = nil,
	secRageBall = nil,
	secRageBallFull = nil
}

function LuaPlayerInfo:createPlayerInfo(extension)
	CCLuaLog("-------------------create new LuaPLayerInfo---------------");
	local playerinfo = setmetatable(extension or {},self);
	self.__index = self;
	return playerinfo;
end

function LuaPlayerInfo:getTotalHp()
	return self.totalHp;
end

function LuaPlayerInfo:setTotalHp(hp)
	self.totalHp = hp;
end

function LuaPlayerInfo:getCurrentHp()
	return self.currentHp;
end

function LuaPlayerInfo:setCurrentHp(hp)
    CCLuaLog(string.format("LuaPlayerInfo:setCurrentHp(%d)",hp));
	self.currentHp = hp;
end

function LuaPlayerInfo:getUINode()
	return self.uiNode;
end

function LuaPlayerInfo:setUINode(node)
	self.uiNode = node;
end

function LuaPlayerInfo:getHpBar()
	return self.hpBar;
end

function LuaPlayerInfo:setHpBar(bar)
	self.hpBar = bar;
end

function LuaPlayerInfo:getPlayerType()
	return self.type;
end

function LuaPlayerInfo:setPlayerType(type)
	self.type = type;
	CCLuaLog(string.format("LuaPlayerInfo:setPlayerType(%d)",type));
end

function LuaPlayerInfo:getPlayerName()
	return self.name;
end

function LuaPlayerInfo:setPlayerName(str)
	CCLuaLog(string.format("LuaPlayerInfo:setPlayerName(%s)",str));
	self.name = str;
end

function LuaPlayerInfo:getPlayerId()
	return self.id;
end

function LuaPlayerInfo:setPlayerId(id)
	--CCLuaLog(string.format("LuaPlayerInfo:setPlayerId(%d)",id));
	self.id = id;
end

function LuaPlayerInfo:getCurrentRagePoint()
	return self.currentRagePoint;
end

function LuaPlayerInfo:setCurrentRagePoint(ragePoint)
    CCLuaLog(string.format("LuaPlayerInfo:setCurrentRagePoint(%d)",ragePoint));
	self.currentRagePoint = ragePoint;
end

function LuaPlayerInfo:getTotalRagePoint()
	return self.totalRagePoint;
end

function LuaPlayerInfo:setTotalRagePoint(ragePoint)
    CCLuaLog(string.format("LuaPlayerInfo:setTotalRagePoint(%d)",ragePoint));
	self.totalRagePoint = ragePoint;
end

function LuaPlayerInfo:getCurrentSecRagePoint()
	return self.secCurrentRagePoint;
end

function LuaPlayerInfo:setCurrenSectRagePoint(ragePoint)
    CCLuaLog(string.format("LuaPlayerInfo:setCurrentRagePoint(%d)",ragePoint));
	self.secCurrentRagePoint = ragePoint;
end

function LuaPlayerInfo:getTotalSecRagePoint()
	return self.secTotalRagePoint;
end

function LuaPlayerInfo:setTotalSecRagePoint(ragePoint)
    CCLuaLog(string.format("LuaPlayerInfo:setTotalSecRagePoint(%d)",ragePoint));
	self.secTotalRagePoint = ragePoint;
end

function LuaPlayerInfo:getRageBar()
	return self.rageBar;
end

function LuaPlayerInfo:getSecRageBar()
	return self.secRageBar;
end

function LuaPlayerInfo:setRageBar(bar)
	self.rageBar = bar;
end

function LuaPlayerInfo:setSecRageBar(bar)
	self.secRageBar = bar;
end

function LuaPlayerInfo:getRageBall()
	return self.rageBall;
end

function LuaPlayerInfo:getSecRageBall()
	return self.secRageBall;
end

function LuaPlayerInfo:setRageBall(ball)	
	self.rageBall = ball;
end

function LuaPlayerInfo:setSecRageBall(ball)	
	self.secRageBall = ball;
end

function LuaPlayerInfo:getRageBallFull()
	return self.rageBallFull;
end

function LuaPlayerInfo:getSecRageBallFull()
	return self.secRageBallFull;
end

function LuaPlayerInfo:setRageBallFull(ball)
	self.rageBallFull = ball;
end

function LuaPlayerInfo:setSecRageBallFull(ball)
	self.secRageBallFull = ball;
end

function LuaPlayerInfo:getRageBackBar()
	return self.rageBackBar;
end

function LuaPlayerInfo:getSecRageBackBar()
	return self.secRageBackBar;
end

function LuaPlayerInfo:setRageBackBar(bar)
	self.rageBackBar = bar;
end

function LuaPlayerInfo:setSecRageBackBar(bar)
	self.secRageBackBar = bar;
end

function LuaPlayerInfo:playHpAni(hp)	
	if self.currentHp ~= hp then
		local uiScale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		--CCLuaLog(string.format("LuaPlayerInfo:playHpAni: %d / %d,%f",hp,self.totalHp,uiScale));
		self.currentHp = hp;
		if 0 ~= self.totalHp then
			local scale = uiScale * hp / self.totalHp;
			local action = CCScaleTo:create(0.5,scale,uiScale);
			if self.hpBar ~= nil then
				self.hpBar:runAction(action);
			end
		end
	--else
		--CCLuaLog("LuaPlayerInfo:playHpAni" .. " self.currentHp == hp == " .. hp)
	end
end


function LuaPlayerInfo:playRageAni(ragePoint)	

	
	
	--local hero = GameManager:Get():getHero();
	local levelManager = LevelManager:sShareInstance();
	if levelManager == nil then
		return 
	end

	local levelLayer = levelManager:getCurrentLevelLayer();
	if levelLayer == nil then
		return 
	end

	local hero = levelLayer:getPlayer(self.id);
	if hero == nil then
		CCLuaLog("hero == nil");

		return;
	end

	if hero:IsHasFightElf(0) == false then	
		return;			
	end

	if self.rageBar ~= nil then
		self.rageBar:setVisible(true);
	end

	if self.rageBackBar ~= nil then
		self.rageBackBar:setVisible(true);
	end

	--CCLuaLog("playRageAni : "..ragePoint);
	if self.currentRagePoint ~= ragePoint then
		--CCLuaLog("self.currentRagePoint ~= ragePoint");
		local uiScale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		--CCLuaLog(string.format("LuaPlayerInfo:playHpAni: %d / %d,%f",hp,self.totalHp,uiScale));
		self.currentRagePoint = ragePoint;
		if 0 ~= self.totalRagePoint then
			local scale = uiScale * ragePoint / self.totalRagePoint;
			local action = CCScaleTo:create(0.01,scale,uiScale);
			if self.rageBar ~= nil then
				--CCLuaLog("self.rageBar ~= nil");
				self.rageBar:runAction(action);
			end
		end
	--else
		--CCLuaLog("LuaPlayerInfo:playHpAni" .. " self.currentHp == hp == " .. hp)
	end

	if self.currentRagePoint < self.totalRagePoint then
			if self.rageBall ~= nil then
				self.rageBall:setVisible(true);
			end

			if self.rageBallFull ~= nil then
				self.rageBallFull:setVisible(false);
			end
		else
			if self.rageBall ~= nil then
				self.rageBall:setVisible(false);
			end

			if self.rageBallFull ~= nil then
				self.rageBallFull:setVisible(true);
			end
	end
end

function LuaPlayerInfo:playSecRageAni(ragePoint)		
	

	--local hero = GameManager:Get():getHero();
	local levelManager = LevelManager:sShareInstance();
	if levelManager == nil then
		return 
	end

	local levelLayer = levelManager:getCurrentLevelLayer();
	if levelLayer == nil then
		return 
	end

	local hero = levelLayer:getPlayer(self.id);
	if hero == nil then
		CCLuaLog("hero == nil");

		return;
	end

	if hero:IsHasFightElf(1) == false then	
		return;			
	end

	if self.secRageBar ~= nil then
		print("step 1")
		self.secRageBar:setVisible(true);
	end

	if self.secRageBackBar ~= nil then
		print("step 2")
		self.secRageBackBar:setVisible(true);
	end

	CCLuaLog("^^^^^^^^^ secCurrentRagePoint : " .. self.secCurrentRagePoint);
	if self.secCurrentRagePoint ~= ragePoint then		
		--CCLuaLog("self.currentRagePoint ~= ragePoint");
		local uiScale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		--CCLuaLog(string.format("LuaPlayerInfo:playHpAni: %d / %d,%f",hp,self.totalHp,uiScale));
		self.secCurrentRagePoint = ragePoint;
		print("step 3 self.secCurrentRagePoint " .. self.secCurrentRagePoint)
		print("step 4 self.secTotalRagePoint " .. self.secTotalRagePoint)
		if 0 ~= self.secTotalRagePoint then
			local scale = uiScale * ragePoint / self.secTotalRagePoint;
			local action = CCScaleTo:create(0.01,scale,uiScale);
			if self.secRageBar ~= nil then
				--CCLuaLog("self.rageBar ~= nil");
				self.secRageBar:runAction(action);
			end
		end
	--else
		--CCLuaLog("LuaPlayerInfo:playHpAni" .. " self.currentHp == hp == " .. hp)
	end

	if self.secCurrentRagePoint < self.secTotalRagePoint then
			if self.secRageBall ~= nil then
				self.secRageBall:setVisible(true);
			end

			if self.secRageBallFull ~= nil then
				self.secRageBallFull:setVisible(false);
			end
		else
			if self.secRageBall ~= nil then
				self.secRageBall:setVisible(false);
			end

			if self.secRageBallFull ~= nil then
				self.secRageBallFull:setVisible(true);
			end
	end
end