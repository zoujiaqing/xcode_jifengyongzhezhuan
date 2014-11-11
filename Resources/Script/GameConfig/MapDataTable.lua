require("Script/GameConfig/RaidBasic")
require("Script/GameConfig/MonsterBasicInfomation")
require("Script/OsCommonMethod")
require("Script/GameConfig/MonsterSkill")
--dofile("E:/cocos2d-x/Adventure/source/Resources/Script/GameConfig/RaidBasic.lua")

G_MonsterTable = {}

function GetOneMapMonsterIds(instance_id,mapId)
	print("------------- instance_id " .. instance_id);
	print("------------- mapId " .. mapId);
	local mapTable = MapMonsterInformation[instance_id]
	if nil == mapTable then
		return false
	else
		for index,value in pairsByKeys(mapTable) do
			local _mapId = value.MapId
			print("------------- map id " .. _mapId);
			if _mapId == mapId then
				G_MonsterTable = value.MonsterId
				return true
			end
		end	
	end	
	return false
end

function PreLoadOneMonsterAudio(monsterId)
	if nil ~= monsterId then
		local monsterItem = MonsterBasicInfomation[monsterId]
		if nil ~= monsterItem then
			local skillsContent = monsterItem.Skill;
			print("-------------------------- PreLoadOneMonsterAudio skillsContent: " .. skillsContent)
			if nil ~= skillsContent then
				local monsterSkills = Split(skillsContent, ";")
				if monsterSkills ~= nil then
					for index,value in pairsByKeys(monsterSkills) do
						print("----------------------- PreLoadOneMonsterAudio value " .. value)
						local skillId = tonumber(value)
						if skillId ~= nil then
							print("----------------------- PreLoadOneMonsterAudio skillId " .. skillId)
							local skillItem = MonsterSkills[skillId]
							if nil ~= skillItem then
								local soundId = skillItem.sound_id;
								if nil ~= soundId then
									GameAudioManager:sharedManager():preLoadEffect(tonumber(soundId))
								end						
							end
						end
					end
				end
			end
		end
	end	
	return true
end