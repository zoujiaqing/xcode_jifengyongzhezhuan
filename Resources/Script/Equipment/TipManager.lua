require("Script/Equipment/EquipConfigTable")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyLuaInterface")
require("Script/Fairy/FairyDataCenter")
require("Script/Equipment/TipColor")
local m_tipPos = 0
local m_exchangeId = 0
local b_isEnough = false

local function onClickFragLeftBtCallBack(tag)

end

local function onClickEquipMiddleCallBack(tag)
	if m_tipPos ~= 0 then
		local item = ItemManager:Get():findItemByPos(m_tipPos)
		if item ~= nil and not item.isEmpty and not item.isLocked then
			G_ShowFairyGetwayLayer(item:getItemId())
		end
	end
end

local function onClickFragMixBtCallBack(tag)
	if m_tipPos ~= 0 and m_exchangeId ~= 0 then
		if b_isEnough then
			OnlineNetworkManager:sShareInstance():sendEquipComposeReq(m_exchangeId,m_tipPos,1)
		else
			GameApi:showMessage("碎片不足")
		end
	end
end

local function getEquipHoleTipIcon(id)
	local icon = nil
	local iconName = nil
	if id == 417001 then
		iconName = "map_ui_system_icon_FRAME_ICON_ARMORY_GREEN"
	elseif id == 417002 then
		iconName = "map_ui_system_icon_FRAME_ICON_ARMORY_BLUE"
	elseif id == 417003 then
		iconName = "map_ui_system_icon_FRAME_ICON_ARMORY_PURPLE"
	elseif id == 417004 then
		iconName = "map_ui_system_icon_FRAME_ICON_ARMORY_YELLOW"
	elseif id == 417005 then
		iconName = "map_ui_system_icon_FRAME_ICON_ARMORY_ORANGE"
	end

	if iconName ~= nil then
		icon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",iconName)
	end
	return icon
end

function getFairyPosByEquipPos(equipPos)
	local fairyPos = 0
	local normalization = math.modf(equipPos/100) * 100
	if normalization == kItemHeroEquip + 100 then
		fairyPos = FAIRY_POS_START_BTL
	elseif normalization == kItemHeroEquip + 200 then
		fairyPos = FAIRY_POS_START_BTL + 1
	elseif normalization == kItemHeroEquip + 300 then
		fairyPos = FAIRY_POS_START_AST
	elseif normalization == kItemHeroEquip + 400 then
		fairyPos = FAIRY_POS_START_AST + 1
	end
	return fairyPos
end

function getFairyDescripetionByQuality(quality)
	local des = ""
	if quality == 1 then
		des = LanguageLocalization:GetLocalization("M_FAIRYTIPS_WHITE")
	elseif quality == 2 then
		des = LanguageLocalization:GetLocalization("M_FAIRYTIPS_GREEN")
	elseif quality == 3 then
		des = LanguageLocalization:GetLocalization("M_FAIRYTIPS_BLUE")
	else
		des = LanguageLocalization:GetLocalization("M_FAIRYTIPS_PURPLE")
	end
	return des
end

function showTipNotItem(id,touch_x,touch_y)
	if fairys[id] ~= nil then
		local fairyInfo = getFairyBasicInfo(id)
		if fairyInfo ~= nil then
			local icon = fairyInfo:getFairyIcon()
			TipMgr:Get():createTip(icon,ccp(touch_x,touch_y))
			TipMgr:Get():add(fairyInfo.name,ItemManager:Get():getFairyLabelColorByQuality(fairyInfo.quality),22)
			TipMgr:Get():add(getFairyQualityStr(fairyInfo.quality),ItemManager:Get():getFairyLabelColorByQuality(fairyInfo.quality),18)
			TipMgr:Get():add(getFairyDescripetionByQuality(fairyInfo.quality),ccc3(255,255,255),18)
			TipMgr:Get():addNodeDone()
		end
	end
end

function showEquipTip(equipId,equipLevel,playerLevel,pt,equipHoles,otherEquipHoles,relativeFairy,suitActivationTable)
	--CCLuaLog("showEquipTip: " .. equipId .. "," .. equipLevel)
	local icon = ItemManager:Get():getIconSpriteById(equipId)
	local itemInfo = ItemManager:Get():getItemInfoById(equipId)
	if itemInfo ~= nil then
		--if itemInfo.m_quality > 0 then
		--	local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality)
		--	if framePic ~= nil then
		--		icon:addChild(framePic)
		--		framePic:setPosition(ccp(icon:getContentSize().width * 0.5,icon:getContentSize().height * 0.5))
		--	end
		--end
		TipMgr:Get():createTip(icon,pt)
		TipMgr:Get():add(itemInfo.m_name,ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality),22)
		TipMgr:Get():add("品质 " .. itemInfo.m_order,ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality),18);
		local levelStr = ""
		if playerLevel ~= 0 then
			if getEquipTypeById(equipId) == 2 then
				levelStr = "+" .. equipLevel
			else
				levelStr = LanguageLocalization:GetLocalization("M_EQUIPMENT_UPGRADELEVEL") .. "  " .. equipLevel .. "/" .. (playerLevel * 2)
			end
		end
		TipMgr:Get():add(levelStr,TipColorOrange,18)
		local equipInfo = getEquipInfo(equipId)
		if equipInfo.basicInfo ~= nil then
			--CCLuaLog(" basicInfo: " .. table.getn(equipInfo.basicInfo))
			for k,v in pairs(equipInfo.basicInfo) do
				local index = v.index
				local currentValue = v.value
				local addValue = 0
				if equipInfo.intensityInfo ~= nil then
					for _index,_add in pairs(equipInfo.intensityInfo) do 
						if _add.index == index then
							addValue = _add.value
							break
						end
					end
				end
				local str = getEquipAttrStrByTableIndex(index) .. "+" .. getEquipAttrFormatStr(index,currentValue + addValue * equipLevel)
				TipMgr:Get():add(str,TipColorGreen,18)
			end
			if equipInfo.lockedInfo ~= nil then
				for k,v in pairs(equipInfo.lockedInfo) do
					local index = v.index
					local value = v.value
					local level = v.level
					if tonumber(level) > equipLevel then 
						local str = getEquipAttrStrByTableIndex(index) .. "+" .. getEquipAttrFormatStr(index,value) .. "(" .. level .. "级解锁)"
						TipMgr:Get():add(str,TipColorGray,18)
					else
						local str = getEquipAttrStrByTableIndex(index) .. "+" .. getEquipAttrFormatStr(index,value)
						TipMgr:Get():add(str,TipColorGreen,18)
					end
				end
			end
			if equipHoles ~= nil then
				for i = 1,4 do 
					if equipHoles[i] ~= nil then
						if equipHoles[i].item_gem_id ~= 0 then
							local icon = getEquipHoleTipIcon(equipHoles[i].item_gem_id)
							local attrText = ItemManager:Get():getAttributionText(equipHoles[i].attr_key,equipHoles[i].attr_value)
							if icon ~= nil and attrText ~= nil then
								local color = ItemManager:Get():getLabelColorByQuality(Items[equipHoles[i].item_gem_id]["Quality"])
								TipMgr:Get():add(icon,"  " .. attrText,color,18)
							end
						end
					end
				end
			end
			if otherEquipHoles ~= nil then
				for i = 1,4 do 
					if otherEquipHoles[i] ~= nil then
						if otherEquipHoles[i].gemId ~= 0 then
							local icon = getEquipHoleTipIcon(otherEquipHoles[i].gemId)
							local attrText = ItemManager:Get():getAttributionText(otherEquipHoles[i].attrKey,otherEquipHoles[i].attrValue)
							if icon ~= nil and attrText ~= nil and otherEquipHoles[i].gemId ~= nil and Items[otherEquipHoles[i].gemId] ~= nil then
								local color = ItemManager:Get():getLabelColorByQuality(Items[otherEquipHoles[i].gemId]["Quality"])
								TipMgr:Get():add(icon,"  " .. attrText,color,18)
							end
						end
					end
				end
			end
			
			if IsSuitEquip(equipId) then
				TipMgr:Get():add(LanguageLocalization:GetLocalization("M_SET"),TipColorOrange,18)	
				for i = 2,4 do 
					local suitId = getSuitIdByEquip(equipId,i)
					if suitId ~= 0 then
						local color = TipColorGray
						if suitActivationTable ~= nil and suitActivationTable[suitId] ~= nil then
							color = TipColorGreen
						end
						TipMgr:Get():add(getSuitDes(suitId),color,18)
					end
				end
			end

			local fetters = getFetterRelativeFairyByFairyId(equipId)
			if fetters ~= nil then
				local node = nil
				local fairyNum = table.getn(fetters)
				local index = 1
				if fairyNum > 0 then
					TipMgr:Get():add(LanguageLocalization:GetLocalization("M_TIPS_EQUIPMENT_4"),TipColorOrange,18)	
				end
				for k,v in pairs(fetters) do
					if v ~= equipId then
						local color
						if v == relativeFairy then
							color = TipColorGreen
						else
							color = TipColorGray
						end
						local fairyInfo = getFairyBasicInfo(v) 
						if fairyInfo ~= nil and fairyInfo.name ~= nil then
							if fairyNum == 1 then
								TipMgr:Get():add(fairyInfo.name,color,18)
							else
								if node == nil then
									node =  TipMgr:Get():addPart(fairyInfo.name, color, 18)
								else
									TipMgr:Get():addPart(node,",",TipColorGray,18,false)
									TipMgr:Get():addPart(node,fairyInfo.name,color,18,index == fairyNum)
								end
							end
							index = index + 1
						end
					end
				end
			end
		end
		--TipMgr:Get():add(LanguageLocalization:GetLocalization("M_EQUIPMENT_DESCRIPTION"),ccc3(255,127,0) ,18)
		TipMgr:Get():addNodeDone()
	end
end

function showEquipmentTipByPos(equipPos,touch_x,touch_y)
	--CCLuaLog("showEquipmentTipByPos: " .. equipPos)
	local  backItem = ItemManager:Get():findItemByPos(equipPos)
	if backItem ~= nil then
		if not backItem.isEmpty then
			local equipId = backItem:getItemId()
			local equipLevel = backItem.itemLevel
			local holeEquip = ItemManager:Get():findItemByPos(equipPos + kItemEnchaseEquip - kItemHeroEquip)
			local holes = nil
			if holeEquip ~= nil and holeEquip.m_equipInfo ~= nil then
				holes = holeEquip.m_equipInfo:getEquipHole()
			end
			local fairyPos = getFairyPosByEquipPos(equipPos)
			local fairyInfo = getUserFairyByPos(fairyPos)
			local fairyId = nil
			if fairyInfo ~= nil then
				fairyId = fairyInfo.fairyId
			end
			local userLevel = UserData:GetUserLevel()
			local suitActivationTable = nil
			if backItem.m_equipInfo ~= nil then
				suitActivationTable = backItem.m_equipInfo:getSuitEquip()
			end
			showEquipTip(equipId,equipLevel,userLevel,ccp(touch_x,touch_y),holes,nil,fairyId,suitActivationTable)
		end
	end
end

function showEquipmentTipById(equipId,touch_x,touch_y)
	showEquipTip(equipId,0,0,ccp(touch_x,touch_y),nil,nil,nil,nil)
end

local function showEquipFragTip(fragId,pt,hasButton)
	--CCLuaLog("backItem:getItemId(): " .. fragId)
	local icon = ItemManager:Get():getIconSpriteById(fragId)
	local itemInfo = ItemManager:Get():getItemInfoById(fragId)
	if itemInfo ~= nil then
		--if itemInfo.m_quality > 0 then
		--	local framePic = ItemManager:Get():getIconFrame(itemInfo.m_quality)
		--	if framePic  ~= nil then
		--		icon:addChild(framePic)
		--		framePic:setPosition(ccp(icon:getContentSize().width * 0.5,icon:getContentSize().height * 0.5))
		--	end
		--end
		TipMgr:Get():createTip(icon,pt)
		TipMgr:Get():add(itemInfo.m_name,ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality),22)
		TipMgr:Get():add("品质 " .. itemInfo.m_order,ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality),18);
		local node =  TipMgr:Get():addPart("集齐", ccc3(255,255,255), 18)
		local needNum,equipId, exchangeId = checkSingleEquipfrag(fragId)
		local amount = ItemManager:Get():getItemNumberById(fragId)
		local numColor = ccc3(0,255,0)
		if needNum > amount then 
			numColor = ccc3(255,0,0)
		end
		TipMgr:Get():addPart(node,needNum ,numColor,18,false)
		TipMgr:Get():addPart(node,"个碎片可以合成",ccc3(255,255,255),18,false)
		local equipName = ""
		local equipQuality = itemInfo.m_quality
		if equipId ~= nil then
			equipNum = tonumber(equipId)
			if EquipmentItems[equipNum] ~= nil and EquipmentItems[equipNum]["Name"] ~= nil then
				equipName = LanguageLocalization:GetLocalization(EquipmentItems[equipNum]["Name"])
				equipQuality = EquipmentItems[equipNum]["Quality"]
			end
		end
		TipMgr:Get():addPart(node,equipName,ItemManager:Get():getLabelColorByQuality(equipQuality),18,true)

		local equipInfo = getEquipInfo(equipId)
		if equipInfo ~= nil then
			if equipInfo.basicInfo ~= nil then
				--CCLuaLog(" basicInfo: " .. table.getn(equipInfo.basicInfo))
				for k,v in pairs(equipInfo.basicInfo) do
					local index = v.index
					local currentValue = v.value
					local addValue = 0
					if equipInfo.intensityInfo ~= nil then
						for _index,_add in pairs(equipInfo.intensityInfo) do 
							if _add.index == index then
								addValue = _add.value
								break
							end
						end
					end
					local str = getEquipAttrStrByTableIndex(index) .. "+" .. getEquipAttrFormatStr(index,currentValue) .. "每级加成+" .. getEquipAttrFormatStr(index,addValue)
					TipMgr:Get():add(str,TipColorGreen,18)
				end
			end
			if equipInfo.lockedInfo ~= nil then
				for k,v in pairs(equipInfo.lockedInfo) do
					local index = v.index
					local value = v.value
					local level = v.level
					local str = getEquipAttrStrByTableIndex(index) .. "+" .. getEquipAttrFormatStr(index,value) .. "(" .. level .. ")级解锁"
					TipMgr:Get():add(str,TipColorGray,18)
				end
			end
		end
		local numNode =  TipMgr:Get():addPart("当前个数  ", TipColorOrange, 22)
		TipMgr:Get():addPart(numNode, amount .. "/" .. needNum,numColor,22,true)
		TipMgr:Get():addNodeDone()
		if hasButton then
			m_exchangeId = exchangeId
			ItemManager:Get():setTipLuaHander(onClickEquipMiddleCallBack,LanguageLocalization:GetLocalization("M_FAIRY_GET_WAY"),true,onClickFragMixBtCallBack,LanguageLocalization:GetLocalization("M_BUTTON_TIPS2"),true)
			--ItemManager:Get():setMiddleLuaHander(onClickEquipMiddleCallBack,LanguageLocalization:GetLocalization("M_FAIRY_GET_WAY"),true)
		end
		if needNum <= amount then
			b_isEnough = true
		else
			b_isEnough = false
		end 
	end
end


function  showEquipFragTipById(equipId,touch_x,touch_y)
	showEquipFragTip(equipId,ccp(touch_x,touch_y),false)
end

function showEquipFragTipByPos(equiPos,touch_x,touch_y)
	CCLuaLog("showEquipFragTipByPos: " .. equiPos)
	local  backItem = ItemManager:Get():findItemByPos(equiPos)
	if backItem ~= nil then
		if not backItem.isEmpty then
			m_tipPos = equiPos
			showEquipFragTip(backItem:getItemId(),ccp(touch_x,touch_y),true)
		end
	end
end

function showOtherAlchemyTip(alchemyId,attrInfo,level,ss_info,touch_x,touch_y)
	local icon = ItemManager:Get():getIconSpriteById(alchemyId)
	local itemInfo = ItemManager:Get():getItemInfoById(alchemyId)
	if itemInfo ~= nil then
		TipMgr:Get():createTip(icon,ccp(touch_x,touch_y))
		TipMgr:Get():add(itemInfo.m_name,ItemManager:Get():getLabelColorByQuality(itemInfo.m_quality),22)
		TipMgr:Get():add(LanguageLocalization:GetLocalization("M_EQUIPMENT_LEVEL") .. level,TipColorOrange,20)
		if attrInfo ~= nil then
			for k,v in pairs(attrInfo) do
				local str = ItemManager:Get():getAttributionText(v.attrKey,v.basicValue+v.intensifyValue)
				TipMgr:Get():add(str,TipColorOrange ,20)
			end
		end
		TipMgr:Get():add(itemInfo.m_szDescription,ccc3(255,255,255) ,20)
		local priceStr = ""
		if ss_info ~= nil then
			priceStr = LanguageLocalization:GetLocalization("M_ELITE_NEXTLEVEL") .. ss_info.consume
		else
			priceStr = LanguageLocalization:GetLocalization("M_REACH_HIGHESTLEVEL")
		end
		TipMgr:Get():add(priceStr,TipColorGreen ,20)
		TipMgr:Get():addNodeDone()
	end
end