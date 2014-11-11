-- 好友玩家基本信息
FriendInfo =	{
	name			= "",		-- 玩家姓名
	id				= nil,		-- 玩家ID
	regTime			= 0,		-- 玩家注册时间
	channelId		= 0,		-- 渠道号
	level			= 0,		-- 玩家等级
	strength		= 0,		-- 战斗力
	type			= 0,		-- 玩家职业
	reputation		= 0,		-- 玩家声望
	rank			= 0,		-- 玩家竞技排名
	leftPkNum		= 0,		-- 剩余PK次数
	onlineFlag		= 1,		-- 1表示在线 0表示离线
	redNameTime		= 0,		-- 红名时间
	killedByPlaerNum	= 0,	-- 被杀死的次数
	killPlayerNum		= 0,	-- 杀死玩家的次数
	killPunishNum		= 0,	-- 红名惩罚
	guildName		= "",		-- 公会名
}

function FriendInfo:createFriendInfo(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	return playerinfo
end

function FriendInfo:getName()
	return self.name
end

function FriendInfo:setName(friendName)
	CCLuaLog("FriendInfo:setName: " .. friendName)
	self.name = friendName
end

function FriendInfo:getStrength()
	return self.strength
end

function FriendInfo:setStrength(value)
	self.strength = value
end

function FriendInfo:setKilledPlayerNum(value)
	self.killedByPlaerNum = value
end

function FriendInfo:getKilledPlayerNum()
	return self.killedByPlaerNum
end

function FriendInfo:setKillPlayerNum(value)
	self.killPlayerNum = value
end

function FriendInfo:getKillPlayerNum()
	return self.killPlayerNum
end

function FriendInfo:setKillPunishNum(value)
	self.killPunishNum = value
end

function FriendInfo:getKillPunishNum()
	return self.killPunishNum
end

function FriendInfo:getRedNameTime()
	return self.redNameTime
end

function FriendInfo:setRedNameTime(time)
	self.redNameTime = time
end

function FriendInfo:getOnLineFlag()
	return self.onlineFlag
end

function FriendInfo:setOnLineFlag(flag)	
	if flag == 0 then
		print("---------------------------------------------------------------------------")
	end
	self.onlineFlag = flag
	print("**************************************** setFlag Inner value " .. self.onlineFlag)
end

function FriendInfo:getRegTime()
	return self.regTime
end

function FriendInfo:setRegTime(time)
	self.regTime = time
end

function FriendInfo:getChannelId()
	return self.channelId
end

function FriendInfo:setChannelId(id)
	self.channelId = id
end

function FriendInfo:getID()
	return self.id
end

function FriendInfo:setID(uid)
	self.id = uid
end

function FriendInfo:getLeftPkNum()
	return self.leftPkNum
end

function FriendInfo:setLeftPkNum(value)
	self.leftPkNum = value
end

--function FriendInfo:getRegTime()
	--return self.regTime
--end
--
--function FriendInfo:setRegTime(time)
	--self.regTime = time
--end

function FriendInfo:getLevel()
	return self.level
end

function FriendInfo:setLevel(lv)
	self.level = lv
end

function FriendInfo:getType()
	return self.type
end

function FriendInfo:setType(type)
	self.type = type
end

function FriendInfo:getReputation()
	return self.reputation
end

function FriendInfo:setReputation(reputation)
	self.reputation = reputation
end

function FriendInfo:getRank()
	return self.rank
end

function FriendInfo:setRank(rank)
	self.rank = rank
end

function FriendInfo:getGuildName()
	return self.guildName
end

function FriendInfo:setGuildName(guildName)
	self.guildName = guildName
end

