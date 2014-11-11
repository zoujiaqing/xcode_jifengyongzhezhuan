--require("Script/Friend/FriendInfo")
require("Script/Language")

SelectedFriendContainer = {
	id				= 0,		-- 玩家ID
	headIcon		= nil,		-- 头像
	nameLabel		= nil,		-- 名字文本
	--levelLabel		= nil,		-- 等级文本
	--occupationLabel = nil,		-- 职业文本
	--reputationLabel = nil,		-- 声望文本
	rankLabel		= nil,		-- 竞技场排名
	detailBt		= nil,		-- 详细信息按键
	pvpDuelTimesLabel = nil,	-- 可发起决斗次数
	pvpDueledTimesLabel = nil,	-- 可接受决斗次数
	leftPKNumLabel = nil,		-- 剩余战斗次数
	killPlayerNumLabel = nil,		-- 杀害玩家的个数
	killedByPlayerNumLabel = nil,	-- 被玩家杀死的个数
	killPunishNumLabel = nil,		-- 玩家惩罚
	guildlabel = nil,			-- 公会
	guildDesLabel = nil,
	--redNameTimeLabel = nil,			-- 红名时间
}

--function SelectedFriendContainer:SetPvPDuelTimeNum(value)
--	CCLuaLog("SetPvPDuelTimeNum: " .. value)
--	if self.pvpDuelTimesLabel ~= nil then
--		self.pvpDuelTimesLabel:setString("" .. value)
--	end
--end
--
--function SelectedFriendContainer:SetPvPDueledTimeNum(value)
--	CCLuaLog("SetPvPDueledTimeNum: " .. value)
--	if self.pvpDueledTimesLabel ~= nil then
--		self.pvpDueledTimesLabel:setString("" .. value)
--	end
--end

function SelectedFriendContainer:create(extension)
	local item = setmetatable(extension or {},self)
	self.__index = self
	return item
end

function SelectedFriendContainer:init(uiLayout)
	self:resetValue()
	if uiLayout ~= nil then
		local widget = nil
		widget = uiLayout:FindChildObjectByName("headIcon")
		if widget ~= nil then
			self.headIcon = widget.__UIPicture__
		end
		
		widget = uiLayout:FindChildObjectByName("friendName")
		if widget ~= nil then
			self.nameLabel = widget.__UILabel__
		end
		--self.levelLabel = uiLayout:FindChildObjectByName("friendLevel").__UILabel__
		--self.occupationLabel = uiLayout:FindChildObjectByName("friendOccupation").__UILabel__
		--self.reputationLabel = uiLayout:FindChildObjectByName("friendReputation").__UILabel__
		widget= uiLayout:FindChildObjectByName("friendRankNo")
		if widget ~= nil then
			self.rankLabel = widget.__UILabel__
		end

		widget = uiLayout:FindChildObjectByName("pvpDuelTimesNumLable")
		if widget ~= nil then
			self.pvpDuelTimesLabel = widget.__UILabel__
		end

		widget = uiLayout:FindChildObjectByName("pvpDueledTimesNumLable")
		if widget ~= nil then
			self.pvpDueledTimesLabel = widget.__UILabel__
		end
		
		widget = uiLayout:FindChildObjectByName("leftPKNumLabel")
		if widget ~= nil then
			self.leftPKNumLabel = widget.__UILabel__
		end		
		
		local widget = uiLayout:FindChildObjectByName("detailBt")
		if widget ~= nil then
			self.detailBt = widget.__UIButton__
		end

		local widget = uiLayout:FindChildObjectByName("killPlayerNumLabel")
		if widget ~= nil then
			self.killPlayerNumLabel = widget.__UILabel__
		end

		local widget = uiLayout:FindChildObjectByName("killedByPlayerNumLabel")
		if widget ~= nil then
			self.killedByPlayerNumLabel = widget.__UILabel__
		end

		local widget = uiLayout:FindChildObjectByName("publishNumLabel")
		if widget ~= nil then
			self.killPunishNumLabel = widget.__UILabel__
		end

		widget = uiLayout:FindChildObjectByName("sociatyText")
		if widget ~= nil then
			self.guildDesLabel = widget.__UILabel__
			self.guildDesLabel:setVisible(false)
		end

		widget = uiLayout:FindChildObjectByName("guildLabel")
		if widget ~= nil then
			self.guildlabel = widget.__UILabel__
		end

		--local widget = uiLayout:FindChildObjectByName("redNameTimeNumLabel")
		--if widget ~= nil then
		--	self.redNameTimeLabel = widget.__UILabel__
		--end		
	end
end

function SelectedFriendContainer:resetValue()
	self.id = 0
	self.headIcon = nil
	self.nameLabel = nil
	--self.levelLabel = nil
	--self.occupationLabel = nil
	--self.reputationLabel = nil
	self.rankLabel = nil
	self.detailBt = nil
	self.pvpDuelTimesLabel = nil
	self.pvpDueledTimesLabel = nil
	self.leftPKNumLabel = nil
	self.killPlayerNumLabel = nil
	self.killedByPlayerNumLabel = nil
	self.killPunishNumLabel = nil
	self.guildlabel = nil
	self.guildDesLabel = nil
	--self.redNameTimeLabel = nil
end

function SelectedFriendContainer:unselectedFriend()
	CCLuaLog("SelectedFriendContainer:unselectedFriend")
	if self.headIcon ~= nil then
		self.headIcon:setSprite(nil)
	end
	--if self.occupationLabel ~= nil then
		--self.occupationLabel:setString("")
	--end
	--if self.levelLabel ~= nil then
	--	self.levelLabel:setString("")
	--end
	--if self.reputationLabel ~= nil then
		--self.reputationLabel:setString("")
	--end
	self:setNameLabel("")
	self:setRank("")
	if self.pvpDuelTimesLabel ~= nil then
		self.pvpDuelTimesLabel:setString("0")
	end
	if self.pvpDueledTimesLabel ~= nil then
		self.pvpDueledTimesLabel:setString("0")
	end	

	if self.leftPKNumLabel ~= nil then
		self.leftPKNumLabel:setString("0")
	end

	if self.killPlayerNumLabel ~= nil then
		self.killPlayerNumLabel:setString("0")
	end
	
	if self.killedByPlayerNumLabel ~= nil then
		self.killedByPlayerNumLabel:setString("0")
	end
	
	if self.killPunishNumLabel ~= nil then
		self.killPunishNumLabel:setString("0")
	end
	
	if self.guildlabel ~= nil then
		self.guildlabel:setString("")
	end

	if self.guildDesLabel ~= nil then
		self.guildDesLabel:setVisible(false)
	end

	--if self.redNameTimeLabel ~= nil then
	--	self.redNameTimeLabel:setString("0")
	--end
	
end

function SelectedFriendContainer:setSelectFriend(playerInfo)
	self.id = playerInfo:getID()
	if playerInfo ~= nil then
		self:setHeadIcon(playerInfo:getType())
		self:setNameLabel(playerInfo:getName())
		--self:setLevelLabel(playerInfo:getLevel())
		--self:setOccupation(playerInfo:getType())
		--self:setReputation(playerInfo:getReputation())
		self:setRank(playerInfo:getRank())
		self:SetLeftPKNum(playerInfo:getLeftPkNum())
		self:setKilledByPlayerNum(playerInfo:getKilledPlayerNum())
		self:setKillPunishNum(playerInfo:getKillPunishNum())
		self:setRedNameTime(playerInfo:getRedNameTime())
		self:setKillPlayerNum(playerInfo:getKillPlayerNum())
		self:SetGuild(playerInfo:getGuildName())
	end
end

function SelectedFriendContainer:showPlayerInfo()
	CCLuaLog("SelectedFriendContainer:showPlayerInfo")
	self:SetPvpDuelTimesNum(UserData:GetUserInfo().m_pvpDuelTimesNum)
	self:SetPvpDueledTimesNum(UserData:GetUserInfo().m_pvpDueledTimesNum)
end

function SelectedFriendContainer:getHeadSpriteByType(type)
	local asMgr = AspriteManager:getInstance(); 
	local frameRes = "";
	if type == 1 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN"
	elseif type == 2 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSWOMAN"
	elseif type == 3 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDMAN"
	elseif type == 4 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDWOMAN"
	elseif type == 5 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINMAN"
	elseif type == 6 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINWOMAN"
	elseif type == 7 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERMAN"
	elseif type == 8 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERWOMAN"
	end
	local headIcon = asMgr:getOneFrame("UI/ui_character_select.bin", frameRes);
	return headIcon;	
end

function SelectedFriendContainer:setHeadIcon(type)
	if self.headIcon ~= nil then
		local sprite = self:getHeadSpriteByType(type)
		self.headIcon:setSprite(sprite)
	end
end

function SelectedFriendContainer:setNameLabel(name)
	if self.nameLabel ~= nil then
		self.nameLabel:setString(name)
	end
end

function SelectedFriendContainer:SetLeftPKNum(value)
	if self.leftPKNumLabel ~= nil then
		self.leftPKNumLabel:setString("" .. value)
	end
end

function SelectedFriendContainer:SetGuild(guildName)
	if guildName ~= nil and guildName ~= "" then
		if self.guildlabel ~= nil then
			self.guildlabel:setString(guildName)
		end

		if self.guildDesLabel ~= nil then
			self.guildDesLabel:setVisible(true)
		end
	else
		if self.guildlabel ~= nil then
			self.guildlabel:setString("")
		end

		if self.guildDesLabel ~= nil then
			self.guildDesLabel:setVisible(false)
		end
	end
end

--function SelectedFriendContainer:setLevelLabel(level)
	--if self.levelLabel ~= nil then
		--self.levelLabel:setString("Lv" .. level)
	--end
--end

--function SelectedFriendContainer:setOccupation(type)
	--if self.occupationLabel ~= nil then
		--local occupation = getCharacterField(type,"Name");
		--self.occupationLabel:setString(occupation)
	--end
--end
--
--function SelectedFriendContainer:setReputation(reputation)
	--if self.reputationLabel ~= nil then
		--self.reputationLabel:setString("声望 " .. reputation)
	--end
--end

function SelectedFriendContainer:setRank(rank)
	if self.rankLabel ~= nil then
		self.rankLabel:setString(rank)
	end
end

function SelectedFriendContainer:SetPvpDuelTimesNum(value)
	if self.pvpDuelTimesLabel ~= nil then
		self.pvpDuelTimesLabel:setString("" .. value)
	end
end

function SelectedFriendContainer:SetPvpDueledTimesNum(value)
	if self.pvpDueledTimesLabel ~= nil then
		self.pvpDueledTimesLabel:setString("" .. value)
	end
end

	
function SelectedFriendContainer:setKilledByPlayerNum(value)
	if self.killedByPlayerNumLabel ~= nil then
		self.killedByPlayerNumLabel:setString("" .. value)
	end	
end

function SelectedFriendContainer:setKillPunishNum(value)
	if self.killPunishNumLabel ~= nil then
		local languageFlag = "M_REDNAME_PENALTY"
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.killPunishNumLabel:setString(content .. value .. "%")
	end
end

function SelectedFriendContainer:setRedNameTime(value)
	--if self.redNameTimeLabel ~= nil then
	--	self.redNameTimeLabel:setString("" .. value)
	--end
end

function SelectedFriendContainer:setKillPlayerNum(value)
	if self.killPlayerNumLabel ~= nil then
		local languageFlag = "M_REDNAME_4"
		local content = LanguageLocalization:GetLocalization(languageFlag)
		self.killPlayerNumLabel:setString("" .. value .. content)
	end
end

function SelectedFriendContainer:setDetailCallBack(func)
	if self.detailBt ~= nil then
		local menuItem = self.detailBt:getMenuItemSprite()
		menuItem:registerScriptTapHandler(func)
	end
end

function SelectedFriendContainer:setDetailCallBack(func)
	if self.detailBt ~= nil then
		local menuItem = self.detailBt:getMenuItemSprite()
		menuItem:registerScriptTapHandler(func)
	end
end

