--Note: 游戏的版本控制脚本
--Note: channel_id 版本ID
--Note: main_version 主版本
--Note: sub_version 子版本

local channel_id = "1001"
local main_version = "1"
local sub_version = "0"

function GetGameChannelID()
	return channel_id
end

function GetGameMainVersion()
	return main_version
end

function GetGameSubVersion()
	return sub_version
end