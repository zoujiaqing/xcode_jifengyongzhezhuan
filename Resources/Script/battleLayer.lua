require("Script/UILayer/PlayerInfo") 
require("Script/GameConfig/characterString")
CCLuaLog("load battleLayer.lua");

local battleLayout = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
local backBtn = battleLayout:FindChildObjectByName("backToTown").__UIButton__:getMenuItemSprite();

local playerInfo = {};
local battleLayerUpdateFun = nil;
local notificationFunc = nil;
local heroInfo;

local function initHeroInfo()
	heroInfo = LuaPlayerInfo:createPlayerInfo();
	heroInfo:setPlayerId(UserData:getUserId());
	heroInfo:setCurrentHp(UserData:getHeroHp());
	heroInfo:setTotalHp(UserData:getHeroTotalHp());

	heroInfo:setCurrentRagePoint(UserData:getRagePoint());
	heroInfo:setTotalRagePoint(UserData:getTotalRagePoint());

	local heroHpBar = battleLayout:FindChildObjectByName("hpBar").__UIPicture__:getCurrentNode();
	if heroHpBar ~= nil then
		local ptx,pty = heroHpBar:getPosition();
		heroHpBar:setAnchorPoint(ccp(0,0.5));
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		heroHpBar:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * heroHpBar:getContentSize().width * 0.5,0)));
		heroInfo:setHpBar(heroHpBar);
		local hpScale = scale * UserData:getHeroHp() / UserData:getHeroTotalHp();
		heroHpBar:setScaleX(hpScale)
	end

	
	local heroRageBar = battleLayout:FindChildObjectByName("rageBar").__UIPicture__:getCurrentNode();
	if heroRageBar ~= nil then
		heroRageBar:setVisible(false);
		local ptx,pty = heroRageBar:getPosition();
		heroRageBar:setAnchorPoint(ccp(0,0.5));
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		heroRageBar:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * heroRageBar:getContentSize().width * 0.5,0)));
		heroInfo:setRageBar(heroRageBar);
		local ragePointScale = scale * UserData:getRagePoint() / UserData:getTotalRagePoint();
		heroRageBar:setScaleX(ragePointScale)
	end

	local heroRageBackBar = battleLayout:FindChildObjectByName("rageBackBar").__UIPicture__:getCurrentNode();
	if heroRageBackBar ~= nil then
		heroRageBackBar:setVisible(false);
		local ptx,pty = heroRageBackBar:getPosition();
		heroRageBackBar:setAnchorPoint(ccp(0,0.5));
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		heroRageBackBar:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * heroRageBackBar:getContentSize().width * 0.5,0)));
		heroInfo:setRageBackBar(heroRageBackBar);
	end

	local iconRage = battleLayout:FindChildObjectByName("iconRage").__UIPicture__:getCurrentNode();
	if iconRage ~= nil then
		iconRage:setVisible(false);		
		local ptx,pty = iconRage:getPosition();
		iconRage:setAnchorPoint(ccp(0,0.5));
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		iconRage:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * iconRage:getContentSize().width * 0.5,0)));
		heroInfo:setRageBall(iconRage);
		
	end

	local iconRageFull = battleLayout:FindChildObjectByName("iconRageFull").__UIPicture__:getCurrentNode();
	if iconRageFull ~= nil then
		iconRageFull:setVisible(false);		
		local ptx,pty = iconRageFull:getPosition();
		iconRageFull:setAnchorPoint(ccp(0,0.5));
		local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
		iconRageFull:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * iconRageFull:getContentSize().width * 0.5,0)));
		heroInfo:setRageBallFull(iconRageFull);
		
	end
	
end

local function onEventHeroHpChange(lastHeroHp, nowHeroHp)
	--local isPVE = LevelManager:sShareInstance():isCurrentPVELevel()
	--local isUnder = LevelManager:sShareInstance():isCurrentUndergroundLevel()
	--local isElite = LevelManager:sShareInstance():isCurrentEliteLevel()
	--if isPVE or isUnder or isElite then
		--CSPlayerHPReq = {
			--blood = nowHeroHp - lastHeroHp,
		--}
		--print("------------------------------- Send Hero HP nowHeroHp " .. nowHeroHp)
		--print("------------------------------- Send Hero HP lastHeroHp " .. lastHeroHp)
		----print("------------------------------- Send Hero HP blood " .. blood)
		----local msgname="CSPlayerHPReq"
		----ret = send_message(msgname, CSPlayerHPReq, true)
	--end
end

local function updateHeroHp()
	local lastHeroHp = heroInfo.currentHp
	local nowHeroHp = UserData:getHeroHp()
	heroInfo:setTotalHp(UserData:getHeroTotalHp());
	if nowHeroHp >0  and lastHeroHp <= 0 then
		heroInfo:playHpAni(UserData:getHeroHp());
		--print("+++++++++++++++++++++++++++++++++++++++++++++++++++++ nowHeroHp " .. nowHeroHp)
		--print("+++++++++++++++++++++++++++++++++++++++++++++++++++++ lastHeroHp " .. lastHeroHp)
		return ;
	end
	--if lastHeroHp ~= nowHeroHp then
		--onEventHeroHpChange(lastHeroHp, nowHeroHp)
	--end

	local layout = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
	if (layout ~= nil) and (heroInfo ~= nil) then
		heroInfo:playHpAni(UserData:getHeroHp());
	end
end

local function updateHeroRagePoint()
	local layout = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
	if (layout ~= nil) and (heroInfo ~= nil) then
		heroInfo:playRageAni(UserData:getRagePoint());
	end
end

initHeroInfo();

local function getTableSize(aTable)
	local count = 0;
	for k,v in pairs(aTable) do
 	   count = count + 1;
 	end
 	return count;
end

local function setTeammeatStatus(player)
	CCLuaLog("**************************************************************************");
	if (battleLayout ~= nil)then
		local playerNum = getTableSize(playerInfo);
		local container = battleLayout:FindChildObjectByName(string.format("TeammeatContainer%d",playerNum)).__UIContainer__;
		if container ~= nil then
			container:setVisible(true);
		end
		if player.type ~= nil then
			local headIconName = getCharacterField(player.type,"HeadPicture");			
	
			--local playerStatus = battleLayout:FindChildObjectByName(string.format("teammeatHpFrame%d",playerNum)).__UIPicture__;
		---	if playerStatus ~= nil then
		--		playerStatus:setVisible(true);
		--	end		
			local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()	
			local headIcon = battleLayout:FindChildObjectByName(string.format("teammeatHead%d",playerNum)).__UIPicture__;
			if headIcon ~= nil then
				--headIcon:setVisible(true);
				local headFrame = AspriteManager:getInstance():getOneFrame(KUI_BIN,"map_ui_FRAME_FRAME_HEADSHOTINBATTLE");
				if headFrame ~= nil then 
					headIcon:setSprite(headFrame);
					headFrame:setScale(0.8);
					local playerIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",headIconName);
					if playerIcon ~= nil then 
						playerIcon:setScale(0.6);
						headIcon:addExtraPic(playerIcon,ccp(0,0));
					end
				end
			end			
			local hpBar = battleLayout:FindChildObjectByName(string.format("teammeatHpBar%d",playerNum)).__UIPicture__:getCurrentNode();
			if hpBar ~= nil then
				local ptx,pty = hpBar:getPosition();
				local point = hpBar:getAnchorPoint();
				if point.x == 0.5 then
					hpBar:setAnchorPoint(ccp(0,0.5));
					hpBar:setPosition(ccpSub(ccp(ptx,pty),ccp(scale * hpBar:getContentSize().width * 0.5,0)));
					local hpScale = scale * player.currentHp / player.totalHp;
					hpBar:setScaleX(hpScale)
				end
			end
			
			local nameLabel = battleLayout:FindChildObjectByName(string.format("teammeatNameLabel%d",playerNum)).__UILabel__;
			if (nameLabel ~= nil) and (player.name ~= nil) then
	--			nameLabel:setVisible(true);
				nameLabel:setString(player.name);
			end			
			
		--	winSize = CCDirector:sharedDirector():getVisibleSize(); 
		--	CCLuaLog(string.format("winSize: %f,%f,%d",winSize.width,winSize.height,playerNum));
		--	node:setPosition(ccp(0,winSize.height * (0.5) * scale - playerNum * heightPos * 1.3- heightPos * 2));
			player:setHpBar(hpBar);
			player:setUINode(container:getCurrentNode());
		end
	end
end 

function showTeammeatsInfo()
	
end

function onBackTown(obj)
	CCLuaLog(" battleLayer.lua : onBackTown");
	--CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(battleLayerUpdateFun);
end

backBtn:registerScriptTapHandler(onBackTown);

local function unRegisterBattleHandlers()
	CCLuaLog("battleLayer.lua : unRegisterBattleHandlers");
	--if notificationFunc ~= nil then
		--NotificationCenter:defaultCenter():unregisterScriptObserver(notificationFunc)	
	--end
	if battleLayerUpdateFun ~= nil then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(battleLayerUpdateFun);
	end
end

local function updatePPVEPlayerHP()
	local isPPVE = LevelManager:sShareInstance():isCurrentPPVELevel();
	local isCityDefend = LevelManager:sShareInstance():isCurrentCityDefendLevel()
	if isPPVE or isCityDefend then
		local players = GameApi:getOtherPlayerInfo();
		--CCLuaLog(string.format("players.count = %d",table.getn(players)));
		for index,value in pairs(players) do
			if value ~= nil then
				local info = nil
				for k,v in pairs(playerInfo) do
					if value.uid:isEqual(k) then
						info = v
					end
				end
				if info == nil then
					local player = LuaPlayerInfo:createPlayerInfo();
					player:setPlayerType(value.type);
					player:setPlayerName(value.name);
					player:setPlayerId(value.uid);
					player:setCurrentHp(value.currentHp);
					player:setTotalHp(value.totalHp);
					player:setCurrentRagePoint(value.currentRagePoint);
					player:setTotalRagePoint(value.totalRagePoint);
					playerInfo[player.id] = player;
					setTeammeatStatus(player);
				else
					local otherPlayer = playerInfo[value.uid];
					--CCLuaLog("playerId: %d,hp = %d / %d ",otherPlayer:getPlayerId(),otherPlayer:getCurrentHp(),otherPlayer:getTotalHp());
					if info:getCurrentHp() ~= value.currentHp then		
						info:playHpAni(value.currentHp);
					end

					if otherPlayer:getCurrentRagePoint() ~= value.currentRagePoint then		
						otherPlayer:playRageAni(value.currentRagePoint);
					end


				end
				--if playerInfo[value.uid] == nil then
					--local player = LuaPlayerInfo:createPlayerInfo();
					--player:setPlayerType(value.type);
					--player:setPlayerName(value.name);
					--player:setPlayerId(value.uid);
					--player:setCurrentHp(value.currentHp);
					--player:setTotalHp(value.totalHp);
					--playerInfo[player.id] = player;
					--setTeammeatStatus(player);
				--else
					--local otherPlayer = playerInfo[value.uid];
					----CCLuaLog("playerId: %d,hp = %d / %d ",otherPlayer:getPlayerId(),otherPlayer:getCurrentHp(),otherPlayer:getTotalHp());
					--if otherPlayer:getCurrentHp() ~= value.currentHp then		
						--otherPlayer:playHpAni(value.currentHp);
					--end
				--end
			end
		end
	
		for index,value in pairs(playerInfo) do
			local isRelease = true;
			for k,v in pairs(players) do
				if v.uid:isEqual(value:getPlayerId()) then
					isRelease = false;
					break;
				end
			end
			if isRelease then
				local layout = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
				local node = value:getUINode();
				if (layout ~= nil) and (node ~= nil) then
					node:setVisible(false);
					playerInfo[index] = nil;
				end
			end
		end
	end
end

local function updatePVPPlayerHP()
	local isPVP = LevelManager:sShareInstance():isCurrentPVPLevel() or LevelManager:sShareInstance():isCurrentMandPKLevel() or LevelManager:sShareInstance():isCurrentCommonPKLevel()
	local isBestFighter = LevelManager:sShareInstance():isCurrentBestFighterLevel()
	if isPVP or isBestFighter then
		local players = GameApi:getOtherPlayerInfo()
		local userInfo = UserData:GetUserInfo()
		for index,value in pairs(players) do
			if value ~= nil then
				local info = nil
				for k,v in pairs(playerInfo) do
					if value.uid:isEqual(k) then
						info = v
					end
				end
				if info == nil then
					local battleSide = value.battleSide;
					if battleSide % 2 == userInfo.battleSide % 2 then
						local player = LuaPlayerInfo:createPlayerInfo();
						player:setPlayerType(value.type);
						player:setPlayerName(value.name);
						player:setPlayerId(value.uid);
						player:setCurrentHp(value.currentHp);
						player:setTotalHp(value.totalHp);
						player:setCurrentRagePoint(value.currentRagePoint);
						player:setTotalRagePoint(value.totalRagePoint);
						playerInfo[player.id] = player;
						setTeammeatStatus(player);
					end
				else
					local otherPlayer = playerInfo[value.uid];
					--CCLuaLog("playerId: %d,hp = %d / %d ",otherPlayer:getPlayerId(),otherPlayer:getCurrentHp(),otherPlayer:getTotalHp());
					if otherPlayer:getCurrentHp() ~= value.currentHp then		
						otherPlayer:playHpAni(value.currentHp);
					end

					if otherPlayer:getCurrentRagePoint() ~= value.currentRagePoint then		
						otherPlayer:playRageAni(value.currentRagePoint);
					end

				end
				--if playerInfo[value.uid] == nil then
					--local battleSide = value.battleSide;
					--if battleSide % 2 == userInfo.battleSide % 2 then
						--local player = LuaPlayerInfo:createPlayerInfo();
						--player:setPlayerType(value.type);
						--player:setPlayerName(value.name);
						--player:setPlayerId(value.uid);
						--player:setCurrentHp(value.currentHp);
						--player:setTotalHp(value.totalHp);
						--playerInfo[player.id] = player;
						--setTeammeatStatus(player);
					--end
				--else
					--local otherPlayer = playerInfo[value.uid];
					----CCLuaLog("playerId: %d,hp = %d / %d ",otherPlayer:getPlayerId(),otherPlayer:getCurrentHp(),otherPlayer:getTotalHp());
					--if otherPlayer:getCurrentHp() ~= value.currentHp then		
						--otherPlayer:playHpAni(value.currentHp);
					--end
				--end
			end
		end

		for index,value in pairs(playerInfo) do
			local isRelease = true;
			for k,v in pairs(players) do
				if v.uid:isEqual(value:getPlayerId()) then
					isRelease = false;
					break;
				end
			end
			if isRelease then
				local layout = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
				local node = value:getUINode();
				if (layout ~= nil) and (node ~= nil) then
					node:setVisible(false);
					playerInfo[index] = nil;
				end
			end
		end

	end
end

local function updateTeammeat(dt)
	--CCLuaLog("battleLayer.lua : updateTeammeat() " .. dt)
	updateHeroHp();
	updateHeroRagePoint();	
	updatePPVEPlayerHP();
	updatePVPPlayerHP();
	
	--CCLuaLog(string.format("battleLayer.lua update: %f, %f",dt,sum));
end

local function updateNotification(message)
	CCLuaLog("battleLayer: updateNotification")
	--if (message == GM_E_ENTER_MAINLAND) or (message == GM_E_ENTER_FB) then
		--unRegisterBattleHandlers()
		--return 1 -- 在notification中不能调unregister取消对消息的注册，返回非零值表示取消对消息的注册
	--end
end



local function initLayer()
print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 9")
	battleLayerUpdateFun = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateTeammeat, 1.0, false);
	--notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
	local mainLayer = MainMenuLayer:getMainMenuLayer()
	--local battleLayer = mainLayer:getChildByTag(20001)
	if mainLayer ~= nil then
		CCLuaLog("initLayer ainLayer ~= nil")
		mainLayer:registerDestructorScriptHandler(unRegisterBattleHandlers)
	else
		CCLuaLog("initLayer ainLayer == nil")
	end
	--local uiLayer = TXGUI.UIManager:sharedManager():getUILayout("BattleUI");
	--if uiLayer ~= nil then
		--local layer = uiLayer:getCurrentNode()
		--if layer ~= nil then
			--layer:registerDestructorScriptHandler(unRegisterBattleHandlers)
		--end
	--end
end

initLayer()