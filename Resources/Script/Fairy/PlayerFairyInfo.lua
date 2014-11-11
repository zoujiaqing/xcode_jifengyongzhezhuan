-- 玩家的精灵信息
PlayerFairyInfo = {
	pos			= 0,	-- 精灵位置
	fairyId		= 0,	-- 精灵ID
	fairyLevel	= 0,	-- 精灵等级
	fairyGrade  = 0,	-- 精灵阶级
	--fairyStatus = 0,	-- 精灵状态
	fairyExp	= 0,	-- 精灵经验
	fairyNextLvExp = 0,  -- 下一级需要经验
	fairyBornTime = 0,	-- 精灵获取时间
	fairyGetWay = 0,    -- 精灵获取方式
	fairyEffectiveness = 0, --精灵战斗力
	isNewFlag = false, -- 是否新精灵
	fairyAttr = nil,	-- 精灵属性
	fairyTrainAttr = nil,	-- 精灵特训属性
	fairyFetters = nil, -- 精灵缘分表
}

function PlayerFairyInfo:create(fairyInfo)
	local item = setmetatable({},self)
	self.__index = self
	item:init(fairyInfo)
	return item
end

function PlayerFairyInfo:updateBasicInfo(basicInfo)
	if basicInfo ~= nil then
		self.fairyLevel = basicInfo.fairy_lv
		self.fairyGrade = basicInfo.fairy_grade
		--self.fairyStatus = basicInfo.fairy_status
		self.fairyExp = basicInfo.fairy_exp
		self.fairyNextLvExp = basicInfo.next_lv_exp
	end
end

function PlayerFairyInfo:updateAttrInfo(attrInfo)
	if attrInfo ~= nil then
		self.fairyAttr = {}
		setmetatable(self.fairyAttr,{})
		for k,v in pairs(attrInfo) do
			self.fairyAttr[v.key] = v.value
			--local attr = {}
			--setmetatable(attr,{})
			--attr.key = v.key
			--attr.value = v.value
			--table.insert(self.fairyAttr,attr)
		end
	end
end

function PlayerFairyInfo:updateTrainAttrInfo(trainAttrInfo)
	if trainAttrInfo ~= nil then
		self.fairyTrainAttr = {}
		setmetatable(self.fairyTrainAttr,{})
		for k,v in pairs(trainAttrInfo) do
			self.fairyTrainAttr[v.key] = v.value
			--local attr = {}
			--setmetatable(attr,{})
			--attr.key = v.key
			--attr.value = v.value
			--table.insert(self.fairyTrainAttr,attr)
		end
	end
end

function PlayerFairyInfo:updateFetterInfo(fetters)
	if fetters ~= nil then
		self.fairyFetters = {}
		setmetatable(self.fairyFetters,{})
		for k,v in pairs(fetters) do
			self.fairyFetters[v] = 1
		end
		NotificationCenter:defaultCenter():broadcast(GM_ATTR_FAIRY_FETTER,nil,"","")
	end
end

function PlayerFairyInfo:updateEffectiveness(effectiveness)
	if effectiveness ~= nil then
		if self.fairyEffectiveness ~= effectiveness then
			self.fairyEffectiveness = effectiveness
			NotificationCenter:defaultCenter():broadcast(GM_ATTR_FAIRY_EFFECTIVENESS,nil,"","")
		end
	end
end

function PlayerFairyInfo:init(fairyInfo)
	--CCLuaLog("PlayerFairyInfo:init: " .. fairyInfo.fairy_pos .. " ,fairyEffectiveness: " .. fairyInfo.fairy_effectiveness)
	self.pos = fairyInfo.fairy_pos
	self.fairyId = fairyInfo.fairy_id
	self.fairyBornTime = fairyInfo.fairy_born_time
	self.fairyGetWay = fairyInfo.fairy_get_way
	self.fairyEffectiveness = fairyInfo.fairy_effectiveness
	self:updateBasicInfo(fairyInfo.fairy_basic)
	self:updateAttrInfo(fairyInfo.fairy_attrs)
	self:updateTrainAttrInfo(fairyInfo.fairy_train_attrs)
	self:updateFetterInfo(fairyInfo.fairy_rel_ids)
end

-- for test
function PlayerFairyInfo:toString()
	CCLuaLog("PlayerFairyInfo")
	CCLuaLog("-------------")
	CCLuaLog("pos: " .. self.pos)
	CCLuaLog("fairyId: " .. self.fairyId)
	CCLuaLog("fairyLevel: " .. self.fairyLevel)
	CCLuaLog("fairyGrade: " .. self.fairyGrade)
	CCLuaLog("-------------")
end