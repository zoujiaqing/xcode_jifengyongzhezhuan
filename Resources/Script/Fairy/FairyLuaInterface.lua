require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyBagLayer")
require("Script/Fairy/FairySelectLayer")
require("Script/Fairy/FairyGetwayLayer")
require("Script/FairyLayer/FairyInfoLayer")
require("Script/GameConfig/fairyunlock")
require("Script/SpriteElf/fairybasicinfo")
-- 提供给C++接口，可以获取存在lua表格中的数据
-- 获得玩家的攻击值 
function G_GetPlayerFairyQulity(pos)
	local userFairyInfo = getPlayerFairyByPos(pos ,nil)
	local ElfId = userFairyInfo.fairyId
	local fairyBasicInfo = getFairyBasicInfo(ElfId)
	if fairyBasicInfo ~= nil then
		return fairyBasicInfo.quality
	else
		return 0
	end
end
function G_GetPlayerAttackFairyByPos(pos)
	local attack = 0
	local userFairyInfo = getPlayerFairyByPos(pos ,nil)
	if userFairyInfo ~= nil then
		local orgAttrTable = userFairyInfo.fairyAttr
		attack = orgAttrTable[3]
	end
	return attack
end
function G_GetFairyIdByPos(pos)
	local fairyId = 0
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		fairyId = fairy.fairyId
	end
	return fairyId
end

function G_GetFairyIconNameByID(id)
	local sprite = ""
	local fairyInfo = getFairyBasicInfo(id)
	if fairyInfo ~= nil then
		sprite = fairyInfo.icon
	end
	return sprite
end

-- 通过精灵ID得到精灵名字
function G_GetFairyNameByID(fairyId)
	local fairyName = ""
	local fairyInfo = getFairyBasicInfo(fairyId)
	if fairyInfo ~= nil then
		fairyName = fairyInfo.name
	end
	return fairyName
end

-- 通过精灵ID得到精灵资质
function G_GetFairyQualityByID(fairyId)
	local quality = 0
	local fairyInfo = getFairyBasicInfo(fairyId)
	if fairyInfo ~= nil then
		quality = fairyInfo.quality
	end
	return quality
end

-- 选择将替换的出战/助战/小伙伴精灵
function G_SelectFairyToEquip(isAllFairy)
	MainMenuLayer:getMainMenuLayer():setFairySelectStatus(true)
	FairySelectLayer:createLayer()
	FairySelectLayer:setSelectModle(false,isAllFairy,0)
end

-- 得到选择的精灵位置（单选，直接取第一个）
function G_GetSelectSingleFairyPos()
	local pos = 0
	local fairyTable = FairySelectLayer:getSelectedFairys()
	if fairyTable ~= nil then
		for k,v in pairs(fairyTable) do
			pos = v
			break
		end
	end
	return pos
end

-- 得到精灵位的解锁等级
function G_GetFairyPosUnlockLevel(pos)
	
	local level = 100
	if fairysunlock[pos] ~= nil then
		level = tonumber(fairysunlock[pos].Unlock_level)
	end
	return level
end

-- 得到特定位置精灵的名字
function G_GetFairyNameByPos(pos)
	local name = ""
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		fairyId = fairy.fairyId
		local fairyInfo = getFairyBasicInfo(fairyId)
		if fairyInfo ~= nil then
			name = fairyInfo.name
			if fairy.fairyGrade > 0 then
				name = name .. "+" .. fairy.fairyGrade
			end
		end
	end
	return name
end

-- 得到特定位置精灵的等级
function G_GetFairyLevelByPos(pos)
	local level = 1
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		level = fairy.fairyLevel
	end
	return level
end


-- 得到特定位置精灵的品质
function G_GetFairyQualityByPos(pos)
	local quality = 0
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		fairyId = fairy.fairyId
		local fairyInfo = getFairyBasicInfo(fairyId)
		if fairyInfo ~= nil then
			quality = fairyInfo.quality
		end
	end
	return quality
end

-- 调用精灵信息界面
function G_ShowFairyInfoLayer(fairyPos,state)
	FairyInfoLayer.fairyPos = fairyPos
	FairyInfoLayer.buttonState = state
	FairyInfoLayer.infoState = 1
	FairyInfoLayer:CreateLayer()
end

-- 根据ID调用精灵信息界面
function G_ShowFairyInfoLayerById(fairyId,state)
	FairyInfoLayer.fairyId = fairyId
	FairyInfoLayer.buttonState = state
	FairyInfoLayer.infoState = 2
	FairyInfoLayer:CreateLayer()
end

-- 根据ID判断是否是精灵
function G_GetIsFairyById(id)
    if fairys[id] ~= nil then
        return true
    end
    return false
end

-- 下精灵伙伴
function G_EquipDownFairyFriend(friendPos)
	local toPos = 0
	for pos = FAIRY_POS_START_NOM,FAIRY_POS_START_NOM + FairyBagLayer.TotalFairyBagCap - 1 do
		local fairy = getUserFairyByPos(pos)
		if fairy == nil then
			toPos = pos
		end
	end
	if pos == 0 then
		GameApi:showMessage("精灵背包已满")
	else
		OnlineNetworkManager:sShareInstance():SendMoveFairyPosReq(toPos,friendPos)
	end
end

function G_GetFairyFetterIdById(fairyId,index)
	local fetterId = 0
	local fetters = getFettersByFairyId(fairyId)
	if fetters[index] ~= nil then
		fetterId = fetters[index]
	end
	return fetterId
end

function G_GetFairyFetterIsActivation(pos,fetterId,userId)
	local isActivation = false
	local fairy = getPlayerFairyByPos(pos,userId)
	if fairy ~= nil then
		if fairy.fairyFetters ~= nil and fairy.fairyFetters[fetterId] == 1 then
			isActivation = true
		end
	end
	return isActivation
end

function G_GetFetterNameByFetterId(fetterId)
	local name = getFetterNameById(fetterId)
	return name
end

function G_GetFairyEffectivenessByPos(pos)
	
	local effectiveness = 0
	local fairy = getUserFairyByPos(pos)
	if fairy ~= nil then
		effectiveness = fairy.fairyEffectiveness
	end
	return effectiveness
end

function G_hasNewFairy()
	local hasNewFairy = false
	local fairyList = getUserFairyList()
	if fairyList ~= nil then
		for k,v in pairs(fairyList) do
			if v.isNewFlag then
				hasNewFairy = true
				break
			end
		end
	end
	return hasNewFairy
end

function G_OpenFairyDecomposeLayer()
	showUILayerByTag(UITagTable["FairyBagLayer"].tag,true)
	FairySelectLayer:createLayer()
	FairySelectLayer:setDecomposeModle()
end

function G_ShowFairyGetwayLayer(itemId)
	FairyGetwayLayer:createLayer(itemId)
end



--------------------------- 精灵新怒气系统 ----------------
function GetFairyRageBasic(fairyId)
    local basicRage = 0
    if fairys[fairyId] ~= nil then
        basicRage = tonumber(fairys[fairyId].Default_Rage)
    end
    return basicRage
end

function GetFairyRageRate(fairyId)
    local rageRate = 0
    if fairys[fairyId] ~= nil then
        rageRate = tonumber(fairys[fairyId].Rage_Increaserate)
    end
    return rageRate
end