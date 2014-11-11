
ShieldPlayerInfo = {
	playerId = nil,
	name	 = "",
}

function ShieldPlayerInfo:create(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	return playerinfo
end

function ShieldPlayerInfo:init(dndInfo)
	--self.playerId = PLAYER_ID:new()
	--self.playerId:setPlayId(dndInfo.userid,dndInfo.reg_tm,dndInfo.channel_id)
	self.playerId = PlayerIdMake(dndInfo.userid,dndInfo.reg_tm,dndInfo.channel_id)
	self.name = dndInfo.name
end
function ShieldPlayerInfo:setplay(id ,regtime ,channelId)
 self.playerId =  PlayerIdMake(id,regtime,channelId)
end