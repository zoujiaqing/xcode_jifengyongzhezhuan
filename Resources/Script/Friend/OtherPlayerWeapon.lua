require("Script/Friend/PlayerItemCenter")

ReqPlayerWeaponTable = 
{

}

local function receiveWeaponRsp()
	--CCLuaLog("receiveWeaponRsp")
	for k,v in pairs(ReqPlayerWeaponTable) do 
		local items = getPlayerItems(v)
		if items ~= nil then
			local weapon = items:getEquipItemByPos(3 + kItemHeroEquip)
			if weapon ~= nil then
				local playerId = v
				local weaponId = weapon.itemId
				--CCLuaLog("------------------------------------------weaponId = " .. weaponId)
				local levelMgr = LevelManager:sShareInstance()
				if levelMgr ~= nil then
					local levelLayer = levelMgr:getCurrentLevelLayer()
					if levelLayer ~= nil then
						levelLayer:EquipOnePlayerWeapon(playerId:getID(),weaponId)
					end
				end
			end
			
		end
		ReqPlayerWeaponTable[k] = nil
		table.remove(ReqPlayerWeaponTable,k);
		--CCLuaLog("remove player k: " .. k)
		--CCLuaLog("ReqPlayerWeaponTable.length: " .. table.getn(ReqPlayerWeaponTable))
	end
end

local function checkCallbackFunc()
	setGetUserWeaponCallback(receiveWeaponRsp)
end

function reqPlayerWeapon(id,channel,regTime)
	CCLuaLog("reqPlayerWeapon")
	--local playerId = PLAYER_ID:new()
	--playerId:setPlayId(id,regTime,channel)
	local playerId = PlayerIdMake(id,regTime,channel)
	local hasPlayer = false
	--for k,v in pairs(ReqPlayerWeaponTable) do 
		--CCLuaLog("k is " .. k .. " v type is " .. type(v))
		--if v ~= nil then
			--CCLuaLog("v: id = " .. v:getID() .. " reg = " .. v:getRegTime() .. " channel = " .. v:getChannel())
			--if playerId.isEqual(v) then
				--hasPlayer = true
				--break
			--end
		--end
	--end
	if not hasPlayer then
		table.insert(ReqPlayerWeaponTable,playerId)
		--CCLuaLog("insert new player")
		--CCLuaLog("ReqPlayerWeaponTable.length: " .. table.getn(ReqPlayerWeaponTable))
		checkCallbackFunc()
		sendGetUserItemInfo(playerId:getID(),playerId:getRegTime(),playerId:getChannel())
	end
	return hasPlayer
end

