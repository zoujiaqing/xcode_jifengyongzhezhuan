require("Script/GameConfig/MonsterBasic")

function CreateRaidCacheMonster(raidId)
	for	i = 1 , #Monster_Basic do
		if tonumber(Monster_Basic[i].RaidID) == raidId then
			if tonumber(Monster_Basic[i].Group) > 0 then
				for turnCount = 1, tonumber(Monster_Basic[i].Turn) do
					SpriteMonsterMgr:Get():CreateCacheMonster(tonumber(Monster_Basic[i].MonsterID))
				end				
			end
		end
	end
	return true
end