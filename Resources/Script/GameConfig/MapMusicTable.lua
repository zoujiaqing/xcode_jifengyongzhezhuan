require("Script/GameConfig/RaidMusic")
CCLuaLog("Load MapMusicTable");
instanceMusic = 
{
}

function initInstanceMusic()
	for index,value in pairs(Map_Basic) do
		local raidId = Map_Basic[index]["RaidID"]
		local mapId = Map_Basic[index]["MapID"]
		local music = Map_Basic[index]["BGMID"]
		local raidNum = tonumber(raidId)
		local mapNum = tonumber(mapId)
		if instanceMusic[raidNum] == nil then
			instanceMusic[raidNum] = {};
		end
		instanceMusic[raidNum][mapNum] = music
	end
end

function getInstanceMusicId(raidId,mapId)
	if (instanceMusic[raidId] ~= nil) and (instanceMusic[raidId][mapId] ~= nil) then
		return instanceMusic[raidId][mapId];
	else
		return 0;
	end
end

function getInstanceId(id)
	--print("id = " ..id);
	for index,value in pairs(Map_Basic) do
		local raidId = Map_Basic[index]["RaidID"]
		--print(raidId)
		if tonumber(raidId) == id then
			return index;
		end
	end
end