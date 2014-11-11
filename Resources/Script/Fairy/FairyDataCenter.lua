require("Script/Fairy/PlayerFairyInfo")
require("Script/Fairy/FairyMessage")
require("Script/SpriteElf/fairybasicinfo")
require("Script/Fairy/LuaDeepCopyFunc")

local userFairyList = nil -- 用户的精灵列表
local otherPlayerFairyList = nil	-- 其它玩家的精灵
local collectionFairyList = nil -- 已获得精灵列表

-- 初始化精灵信息（主要用于帐号切换时清除缓存）
function initFairyInfo()
	userFairyList = nil 
	otherPlayerFairyList = nil
	collectionFairyList = nil
	--initCollectionFairy()
end

-- 玩家的精灵列表
function getUserFairyList()
	return deepcopy(userFairyList)
	--return userFairyList
end

-- 得到玩家特定位置的某个精灵 playerId 为 nil表示玩家
function getPlayerFairyByPos(pos,playerId)
	if playerId == nil then
		return getUserFairyByPos(pos)
	else
		local fairy = nil
		local fairyList = getOtherPlayerFairyList(playerId)
		if fairyList ~= nil then
			fairy = fairyList[pos]
		end
		return fairy
	end
end

-- 得到其它玩家精灵列表
function getOtherPlayerFairyList(userId)
	local fairyList = nil
	if otherPlayerFairyList ~= nil and userId ~= nil then
		for k,v in pairs(otherPlayerFairyList) do
			if k:isEqual(userId) then
				fairyList = v
				break
			end
		end
	end
	return fairyList
end

function initCollectionFairy()
	--CCLuaLog("initCollectionFairy")
	if collectionFairyList == nil then
		local startIndex = PB_ATTR_FAIRY_COLLECTION_BEGIN
		local totalFairyNum = 0
		while fairys[2001 + totalFairyNum] ~= nil do
			totalFairyNum = totalFairyNum + 1
		end
		local endIndex = PB_ATTR_FAIRY_COLLECTION_BEGIN + math.ceil(totalFairyNum/32)
		if endIndex > PB_ATTR_FAIRY_COLLECTION_END then
			endIndex = PB_ATTR_FAIRY_COLLECTION_END
		end
		sendFairyCollectionReq(startIndex,endIndex)
	end
end

function setCollectionFairyStatus(index,status)
	--CCLuaLog("setCollectionFairyStatus: index(" .. index .. "),status("..status..")")
	if collectionFairyList == nil then
		collectionFairyList = {}
	end
	collectionFairyList[index] = status
end

function checkCollectionFairyStatus(fairyId)
	local hasFairy = false
	local index =  PB_ATTR_FAIRY_COLLECTION_BEGIN + math.modf((fairyId - 2000) / 32)
	if collectionFairyList ~= nil and collectionFairyList[index] ~= nil then
		local modeIndex = math.mod(fairyId - 2000 , 32)
		local shiftIndex = modeIndex
		if shiftIndex > 1 then
			shiftIndex = bit:_lshift(1,modeIndex-1)
		end
		hasFairy =  bit:_and(collectionFairyList[index],shiftIndex) > 0
	end
	--CCLuaLog("checkCollectionFairyStatus: " .. fairyId .. "" .. tostring(hasFairy))
	return hasFairy
end

-- 精灵背包中精灵数量
function getUserFairyNum()
	local sum = 0
	if userFairyList ~= nil then
		for k,v in pairs(userFairyList) do
			sum = sum + 1
		end
	end
	return sum
end

-- 得到位置为pos的精灵，没有时返回nil
function getUserFairyByPos(pos)
	--CCLuaLog("getUserFairyByPos(pos)" .. pos)
	if userFairyList ~= nil then
		return deepcopy(userFairyList[pos])
	end
	return nil
end

-- 重置玩家的精灵列表
function resetUserFairyList(infos)
	--CCLuaLog("resetUserFairyList(infos)")
	if infos ~= nil then
		userFairyList = {}
		for k,v in pairs(infos) do
			if v ~= nil then
				local fairy = PlayerFairyInfo:create(v)
				userFairyList[v.fairy_pos] = fairy
			end
		end
	end
end

function resetOtherPlayerFairys(playerId)
	if otherPlayerFairyList ~= nil and playerId ~= nil then
		local index = nil
		for k,v in pairs(otherPlayerFairyList) do
			if k:isEqual(playerId) then
				index = k
				break
			end
		end
		if index ~= nil then
			otherPlayerFairyList[index] = nil
		end
	end
end

-- 重置玩家的精灵列表
function setOtherUserFairyList(playerId,infos)
	if infos ~= nil then
		if otherPlayerFairyList == nil then
			otherPlayerFairyList = {}
		end
		local fairylist = {}
		for k,v in pairs(infos) do
			if v ~= nil then
				local fairy = PlayerFairyInfo:create(v)
				fairylist[v.fairy_pos] = fairy
			end
		end
		resetOtherPlayerFairys(playerId)
		otherPlayerFairyList[playerId] = fairylist
	end
end

-- 更新玩家的精灵信息
function updateUserFairys(pos,basicInfo,attrs,trainAttrs,fetters,fairy_effectiveness)
	if userFairyList ~= nil and userFairyList[pos] ~= nil then
		if basicInfo ~= nil then
			userFairyList[pos]:updateBasicInfo(basicInfo)
		end
		if attrs ~= nil then
			userFairyList[pos]:updateAttrInfo(attrs)
		end
		if trainAttrs ~= nil then
			userFairyList[pos]:updateTrainAttrInfo(trainAttrs)
		end

		if fetters ~= nil then
			userFairyList[pos]:updateFetterInfo(fetters)
		end

		if fairy_effectiveness ~= nil then
			userFairyList[pos]:updateEffectiveness(fairy_effectiveness)
		end
	end
end

-- 删除精灵,返回值表示是否删除了一只新精灵
function deleteUserFairy(pos)
	local deleteNew = false
	if userFairyList ~= nil then
		if userFairyList[pos] ~= nil and userFairyList[pos].isNewFlag then
			deleteNew = true
		end
		userFairyList[pos] = nil
	end
	return deleteNew
end

-- 添加新的精灵
function addUserFairy(info)
	if userFairyList == nil then
		userFairyList = {}
	end
	if info then
		local fairy = PlayerFairyInfo:create(info)
		userFairyList[info.fairy_pos] = fairy
	end
end

-- 设置是否新精灵
function setUserFairyNewFlag(fairyPos,isNew)
	if userFairyList ~= nil and userFairyList[fairyPos] ~= nil then
		if userFairyList[fairyPos].isNewFlag ~= isNew then
			userFairyList[fairyPos].isNewFlag = isNew
			NotificationCenter:defaultCenter():broadcast(GM_ATTR_NEW_FAIRY_INFO,nil,"","")
		end
	end
end

-- 交换精灵位置
function exchangeFairyPos(from_pos,to_pos)
	--CCLuaLog("exchangeFairyPos(from_pos,to_pos): " .. from_pos .. " , " .. to_pos)
	if userFairyList ~= nil then
		local fromFairy = userFairyList[from_pos]
		local toFairy = userFairyList[to_pos]
		if fromFairy ~= nil then
			fromFairy.pos = to_pos
		end
		if toFairy ~= nil then
			toFairy.pos = from_pos
		end
		userFairyList[from_pos] = toFairy
		userFairyList[to_pos] = fromFairy

		if userFairyList[from_pos] ~= nil and userFairyList[from_pos].isNewFlag then
			setUserFairyNewFlag(from_pos,false)
		end
		if userFairyList[to_pos] ~= nil and userFairyList[to_pos].isNewFlag then
			setUserFairyNewFlag(to_pos,false)
		end
		if isBattleFairy(from_pos) or isAssistFairy(from_pos) or isFriendlyFairy(from_pos) then
			MainMenuLayer:getMainMenuLayer():setFairyMovedIcon(from_pos)
		end
		if isBattleFairy(to_pos) or isAssistFairy(to_pos) or isFriendlyFairy(to_pos) then
			MainMenuLayer:getMainMenuLayer():setFairyMovedIcon(to_pos)
		end
	end
end

-- 获得某个id的精灵个数
function GetFairyCountById(fairyId, exceptPos)
	local fairyList = getUserFairyList()
	local count = 0

	if fairyList ~= nil then
		for k,v in pairs(fairyList) do
			if v.fairyId == fairyId then
				if v.pos ~= exceptPos then
					count = count + 1
				end				
			end
		end
	end

	return count
end

-- 获得可供进阶的精灵数量
function GetAdvanceFairyCountById(fairyId, exceptPos)
	local fairyList = getUserFairyList()
	local count = 0

	if fairyList ~= nil then
		for k,v in pairs(fairyList) do
			if v.fairyId == fairyId and v.fairyLevel == 1 and v.fairyGrade == 0 then
				if v.pos ~= exceptPos then
					count = count + 1
				end				
			end
		end
	end

	return count
end

-- 获得可供兑换的精灵数量
function GetExchangeFairyCountById(fairyId)
	fairyId = tonumber(fairyId)
	local fairyList = getUserFairyList()
	local count = 0

	if fairyList ~= nil then
		for k,v in pairs(fairyList) do
			if v.fairyId == fairyId and v.fairyLevel == 1 and v.fairyGrade == 0 and isNormalFairy(v.pos) then
				count = count + 1			
			end
		end
	end

	return count
end