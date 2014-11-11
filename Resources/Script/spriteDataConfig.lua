require("Script/GameConfig/spriteDataList")

function GetASpriteData(index)
	return spriteDataList[index]
end

function GetAspriteNum()
	return table.getn(spriteDataList) / 2
end