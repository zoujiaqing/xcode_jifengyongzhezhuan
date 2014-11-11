require("Script/GameConfig/InstanceList")
require("Script/MainMenuUILayer")
require("Script/Instance/UnderBoxLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/GameConfig/RaidMusic")

NextUnderInstanceId = 0

function SetNextUnderInstanceId(id)
	NextUnderInstanceId = id
	return true
end

function GetNextUnderInstanceId()
	return NextUnderInstanceId
end

function OnPressUndergroundNpc(npcId)
	showUILayerByTag(UITagTable["UnderBoxLayer"].tag,true)
	UnderBoxLayer.UnderBoxNpcId = npcId
	RefreshUnderBoxLayer()
end

function OnPressCityDefendNpc(npcId)
	showUILayerByTag(UITagTable["CityDefendChatLayer"].tag,true)
	CityDefendChatLayer.CityDefendNpcId = npcId
	RefreshCityDefendChatLayer()
end

function GetInstanceBornCell(curRaidId, curMapId, cellIndex)
	print("cur "..curRaidId.." "..curMapId)
	local ret = 0
	local cell = {}
	for index,value in pairs(Map_Basic) do
		local raidId = Map_Basic[index]["RaidID"]
		local mapId = Map_Basic[index]["MapID"]
		if raidId == ""..curRaidId and mapId == ""..curMapId then
			local bornCell = Map_Basic[index]["HeroBornCoordinate"]
			for num in string.gmatch(bornCell, "%d+") do
				cell[#cell + 1] = num
			end
		end
	end

	if #cell >= 2 then
		ret = cell[cellIndex]
	end
	return ret
end