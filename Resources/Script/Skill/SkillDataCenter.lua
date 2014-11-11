require("Script/OsCommonMethod")
require("Script/Language")
require("Script/Skill/SkillBasicDataTable")
require("Script/CommonDefine")

-------------------------------------------------------------------
--Note: 技能槽数据
SkillSlotData = {
	SlotIndex = 0,
	SkillId = 0,
	IsSlotLock = false
}

function SkillSlotData:New()
	local oneTable = {}

	setmetatable(oneTable,self);
	self.__index = self;

	return oneTable
end

--Note: Skill Item Data
SkillBasicData = {
	SkillID = 0,
	SkillLevel = 0,
	SkillUnLockLevel = 0,
	SkillMaxLevel = 0
}

--Note: 技能基本数据，显示左侧列表
function SkillBasicData:New()
	local oneTable = {}

	setmetatable(oneTable,self);
	self.__index = self;

	return oneTable
end

BattleInfoData = {
	-- 物理攻击
	physical_attack = 0 ,
	-- 魔法攻击
	magic_attack = 0,
	-- 技能攻击
	skill_attack = 0,
	-- 物理防御
	physical_defence = 0,
	-- 魔法防御
	magic_defence = 0,
	-- 技能防御
	skill_defence = 0,
	-- 生命值
	health_point = 0,
	-- 精准 
	accurate	= 0,
	-- 闪避 
	dodge		= 0,
	-- 破击
	wreck		= 0, 
	-- 格挡
	parry		= 0, 
	-- 暴击
	critical_strike	= 0,
	-- 韧性
	tenacity	= 0,
	-- 必杀
	slay		= 0, 
	-- 极速
	speed		= 0, 
	-- 潜能
	proficiency	= 0,
}

function BattleInfoData:New()
	local oneTable = {}

	setmetatable(oneTable,self);
	self.__index = self;

	return oneTable
end

function BattleInfoData:ResetValue()
	self.physical_attack = 0
	self.magic_attack = 0
	self.skill_attack = 0
	self.physical_defence = 0
	self.magic_defence = 0
	self.skill_defence = 0
	self.health_point = 0
	self.accurate	= 0
	self.dodge		= 0
	self.wreck		= 0 
	self.parry		= 0 
	self.critical_strike	= 0
	self.tenacity	= 0
	self.slay		= 0
	self.speed		= 0 
	self.proficiency	= 0 
end


SkillLevelBasicData = {
	skill_id = 0,
	skill_level = 0,

	next_money = 0,
	next_exploit = 0,
	next_lv = 0,
	diff_own_cd	= 0,
	continue_cd = 0,
	diff_hurt = 0,
	
	diff_battle_info = BattleInfoData:New();
}

--Note: 右侧显示属性值
function SkillLevelBasicData:New()
	local oneTable = {}

	setmetatable(oneTable,self);
	self.__index = self;

	return oneTable
end

function SkillLevelBasicData:ResetValue()
	self.skill_id = 0
	self.skill_level = 0

	self.next_money = 0
	self.next_exploit = 0
	self.next_lv = 0
	self.diff_own_cd	= 0
	self.continue_cd = 0
	self.diff_hurt = 0

	self.diff_battle_info = BattleInfoData:New()
end

-------------------------------------------------------------------
--Note: 数据管理类
SkillDataCenter = {
	DataTable = {},	
	SlotDataTable = {},
	skillId = 0,
	skillLevel = 0,
	skillLevelBasicData = SkillLevelBasicData:New(),
	skillNextLevelBasicData = SkillLevelBasicData:New(),
	
	isInitData = false
}

--Note: Reset All Value
function SkillDataCenter:ResetValue()
	self.DataTable = {}
	self.SlotDataTable = {}
	self.skillId = 0
	self.skillLevel = 0
	self.skillLevelBasicData = SkillLevelBasicData:New()
	self.skillNextLevelBasicData = SkillLevelBasicData:New()
end

--Note: 初始化数据
function SkillDataCenter:InitValue()
	self:ResetValue()
	local heroType = self:GetHeroTypeId()
	self:GetHeroAllSkillIds(heroType)
end

function SkillDataCenter:GetHeroTypeId()
	local heroType = UserData:GetUserType()
	if heroType == 2 then
		heroType = 1
	elseif heroType == 4 then
		heroType = 3
	elseif heroType == 6 then
		heroType = 5
	end
	return heroType
end

--Note: 技能名称
function SkillDataCenter:GetSkillName(skillId)
	local item = skills_ui[skillId]
	if item == nil then
		return nil
	end

	return LanguageLocalization:GetLocalization(item.skill_name)
end

--Note: 技能描述
function SkillDataCenter:GetSkillDescription(skillId)
	local item = skills_ui[skillId]
	if item == nil then
		return nil
	end
	
	return LanguageLocalization:GetLocalization(item.skill_description)
end

--Note: 技能升级需要的消耗
function SkillDataCenter:GetSkillLevelUpValue()
	if nil ~= self.skillNextLevelBasicData then
		return self.skillNextLevelBasicData.next_money,self.skillNextLevelBasicData.next_exploit
	end
end

--Note: 解锁技能需要的玩家等级
function SkillDataCenter:GetOneSkillUnLockLevel(skillId)
	local item = skills_ui[skillId]
	if item == nil then
		return nil
	end
	
	return tonumber(item.unlock_level)
end

--Note: 判断技能是否可以学习
function SkillDataCenter:IsSkillCanLearn(skillId)
	local unlockLevel = self:GetOneSkillUnLockLevel(skillId)
	local heroLevel = UserData:GetUserLevel()
	if heroLevel >= unlockLevel then
		return true
	else
		return false
	end
end

--Note: 下次升级需要的玩家等级
function SkillDataCenter:GetLevelUpHeroLevel()
	if nil ~= self.skillNextLevelBasicData then
		return self.skillNextLevelBasicData.next_lv
	end
end

function GetOneSkillIconName(skillId)
	local skillIconName = nil
	for index,value in pairs(SkillDataCenter.DataTable) do
		if skillId == value.SkillID then
			skillIconName = SkillDataCenter:GetOneSkillIconName(skillId)
			return skillIconName
		end
	end
	return skillIconName
end

--Note: 获取技能图标
function SkillDataCenter:GetOneSkillIconName(skillId)
	local item = skills_ui[skillId]
	if item == nil then
		return nil
	end
	
	return item.skill_icon
end

--Note: 获取技能数据
function SkillDataCenter:GetOneSkillDataBySkillId(skillId)
	if self.DataTable == nil then
		return nil
	end
	
	for index,value in pairs(self.DataTable) do
		if value.SkillID == skillId then
			return value
		end
	end
	
	return nil
end

--Note: 获取某技能等级
function SkillDataCenter:GetOneSkillLevel(skillId)
	if self.DataTable == nil then
		return -1
	end
	
	local data = self:GetOneSkillDataBySkillId(skillId)
	if data == nil then
		return -1
	else
		return data.SkillLevel
	end	
end

function SkillDataCenter:IsUptoMaxLevel(skillId)
	if self.DataTable == nil then
		return false
	end	
	
	local data = self:GetOneSkillDataBySkillId(skillId)
	if data ~= nil then
		print("data.SkillMaxLevel " .. data.SkillMaxLevel)
		if 0 == data.SkillLevel then
			return false
		end
		if data.SkillMaxLevel <= data.SkillLevel then			
			print("data.SkillLevel " .. data.SkillLevel)
			return true
		end
	end		
	return false
end

--Note: 获取技能核心贡献
function SkillDataCenter:GetOneSkillAttruibte(skillId)
	local item = skills_ui[skillId]
	if item == nil then
		return nil
	end
	
	local content = item.core_attribute
	return Split(content,"/")
end

function SkillDataCenter:GetSkillIdByTableIndex(index)
	local item = self.DataTable[index]
	if item == nil then
		return nil
	end

	return item.SkillID
end

--Note: Private
--Note: 获取玩家的所有技能
function SkillDataCenter:GetHeroAllSkillIds(roleId)
	--print("roleId " .. roleId)
	for index,value in pairsByKeys(skills_ui) do
		--print("function SkillDataCenter:GetHeroAllSkillIds(roleId) " .. index)
		local roleType = tonumber(value.role_type)
		--print("function SkillDataCenter:GetHeroAllSkillIds(roleId) roleType " .. roleType)		
		if roleId == roleType then
			local basicDataTable = SkillBasicData:New()
			if nil ~= basicDataTable then
				basicDataTable.SkillID = index
				basicDataTable.SkillUnLockLevel = tonumber(value.unlock_level)
				InsertOneItemToTable(self.DataTable,basicDataTable)
			end
		end
	end

	-- 根据解锁等级排序
	local function sortByUnlockLevel(a, b)
		return a.SkillUnLockLevel < b.SkillUnLockLevel
	end

	table.sort(self.DataTable, sortByUnlockLevel)
end

--Note: 插入一条基本技能数据
function SkillDataCenter:PushOneSkillItem(skillId,levelId,maxLevel)	
	for index,value in pairs(self.DataTable) do
		if value.SkillID == skillId then
			--print("function SkillDataCenter:PushOneSkillItem(skillId,levelId,maxLevel) " .. skillId)
			value.SkillLevel = levelId
			if nil ~= maxLevel then
				value.SkillMaxLevel = maxLevel
				print("function SkillDataCenter:PushOneSkillItem(skillId,levelId,maxLevel)	" .. maxLevel)
			end
			return
		end
	end
end

function SkillDataCenter:EquipSkillToLeftSlot(slotValue)
	--SlotIndex = 0,
	--SkillId = 0,
	--检测是否重复
	for _ , value in pairs(self.SlotDataTable) do
		if value.SkillId == slotValue then
			return
		end
	end

	local isFoundLeftSlot = false
	for index,value in pairs(self.SlotDataTable) do
		if value.SkillId == nil or value.SkillId == 0 then			
			--print("------------------------------------ slot index " .. value.SlotIndex)
			--print("------------------------------------ slot slotValue " .. slotValue)
			self:PushSkillSlotInfoToLua(value.SlotIndex,slotValue)
			SkillUIManager:getInstance():sendSetSlotAttributeMessage(value.SlotIndex,slotValue)
			isFoundLeftSlot = true
			break
		end
	end
	--if false == isFoundLeftSlot then
		----Note: 用户提示
		--local languageFlag = "M_ATUO_STUDYSKILLS_TIP"
		--local content = LanguageLocalization:GetLocalization(languageFlag)
		--GameApi:showMessage(content);
	--end
end

function SkillDataCenter:PushSkillSlotInfoToLua(slotIndex,slotValue)
	local isIndexExist = false;
	for index,value in pairs(self.SlotDataTable) do
		local tmpSlotIndex = value.SlotIndex
		if tmpSlotIndex == slotIndex then
			isIndexExist = true
			value.SkillId = slotValue
			break
		end
	end

	--Note: 插入数据项
	if false == isIndexExist then	
		local slotDataTable = SkillSlotData:New()
		if nil ~= slotDataTable then
			slotDataTable.SlotIndex = slotIndex
			slotDataTable.SkillId = slotValue
			--print("-------------------------- insert one skill item to table SlotIndex " .. slotIndex)
			--print("-------------------------- insert one skill item to table slotValue " .. slotValue)
			InsertOneItemToTable(self.SlotDataTable,slotDataTable)
		end	
	end
end

function SkillDataCenter:SetOneSlotSkillId(slotIndex,skillId)
	local tableItem = self.SlotDataTable[slotIndex]
	if nil == tableItem then
		return false
	end	

	tableItem.SkillId = skillId
	return true
end

function SkillDataCenter:GetOneSlotSkillId(slotIndex)
	local tableItem = self.SlotDataTable[slotIndex]
	if nil == tableItem then
		return nil
	else
		return tableItem.SkillId
	end
end

function SkillDataCenter:GetOneSkillExistInOtherSlot(slotIndex,skillId)
	
	local isExist = false;
	local slotIndex = 0
	for index,value in pairs(self.SlotDataTable) do
		if false == value.IsSlotLock then
			if index ~= slotIndex then
				if value.SkillId == skillId then
					isExist = true
					slotIndex = index
					break
				end
			end
		end
	end
	return isExist,slotIndex
end

function SkillDataCenter:IsOneSlotLock(slotIndex)
	local tableItem = self.SlotDataTable[slotIndex]
	if nil == tableItem then
		return true
	else
		return tableItem.IsSlotLock
	end
end

function SkillDataCenter:GetOneRoleSkillCount(roleType)
	print("-------------------------------------------- roleType " .. roleType)
	local count = 0
	for index,value in pairs(skills_ui) do
		local role_type = tonumber(value.role_type)
		if role_type == roleType then
			count = count + 1
		end
	end
	print("-------------------------------------------- count " .. count)
	return count
end

function SkillDataCenter:GetOnePageIdList(pageIndex,pageItemsCount,roleType)
	local startNum = (pageIndex-1) * pageItemsCount
	local count = 0
	idList = {}
	for index,value in pairs(self.DataTable) do

			count = count + 1
			if count > startNum then
				InsertOneItemToTable(idList,value.SkillID)
				print("------------------------------------- insert " .. index)
				local listCount = table.getn(idList)
				if listCount >= pageItemsCount then
					print("------------------------------------- insert over ")
					break;
				end
			end

	end

	return idList
end

------------------------------------------接口函数接口函数-------------------------------------------
--Note: 初始化数据
function SkillDataCenterInitData()
	--print("***** function SkillDataCenter:InitData()")
	--if false == SkillDataCenter.isInitData then
		SkillDataCenter:InitValue()
		--SkillDataCenter.isInitData = true
	--end
	return true
end

function PushSkillItemToDataCenter(skillId,levelId,maxLevel)
	SkillDataCenter:PushOneSkillItem(skillId,levelId,maxLevel)
	return true
end

function PushSkillSlotInfoToLua(slotIndex,value)
	SkillDataCenter:PushSkillSlotInfoToLua(slotIndex,value)
	return true
end

function EquipSkillToLeftSlot(value)
	SkillDataCenter:EquipSkillToLeftSlot(value)
	return true
end

-- 新技能查看状态
local newSkillId = 0
function SetNewSkillId(id)
	newSkillId = id

	local function sendSetNewSkillIdReq()
		CSAttributesSetReq = {
			key = 1200010,
			value = newSkillId,
			}
		local msgname="CSAttributesSetReq"
		local ret = send_message(msgname, CSAttributesSetReq, true)
		return ret
	end

	--通知后台
	sendSetNewSkillIdReq()
	UpdateNewSkillCheckInfo()
end

function getNewSkillId()
	return newSkillId
end

function UpdateNewSkillCheckInfo()
	CCLuaLog("------- UpdateNewSkillCheckInfo ------")

	-- 更新主界面
	local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");	
	if nil ~= topLayout then
		local skillButton = topLayout:FindChildObjectByName("headShotBtn").__UIButton__

		if newSkillId ~= 0 then
			skillButton:showTipIcon()
		else
			skillButton:hideTipIcon()
		end
	end
end

function OnClickSkillButton()
	CCLuaLog("------- OnClickSkillButton ------")

	--立即刷新本地
	SetNewSkillId(0)
end	
-----------------------------------------------------------------------------------------------------