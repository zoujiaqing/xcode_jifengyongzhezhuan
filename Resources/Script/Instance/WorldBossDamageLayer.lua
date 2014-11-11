require("Script/GameConfig/uiTagDefine")

WorldBossDamageLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	inBattle = false,
}

local WorldBossDamageData = {}
local MyDamageData = nil

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()

	WorldBossDamageLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossDamageUI")
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossDamageUI2")
end

function WorldBossDamageLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["WorldBossDamageLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			if self.inBattle then
				self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossDamageUI.plist",self.uiLayerInstance, "WorldBossDamageUI", true)
			else
				self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossDamageUI2.plist",self.uiLayerInstance, "WorldBossDamageUI2", true)
			end
			
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function WorldBossDamageLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.inBattle = false

	WorldBossDamageData = {}
	MyDamageData = nil
end

-- 删除UI
function WorldBossDamageLayer:destroyed()
	CCLuaLog("--- WorldBossDamageLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	self:resetValue()
end

local function refreshWorldBossDamageLayer()
	if WorldBossDamageLayer.uiLayout == nil then
		return
	end

	for index = 1, 5 do
		local damageNameLabel = WorldBossDamageLayer.uiLayout:FindChildObjectByName("damageName"..index).__UILabel__
		local damageNumLabel = WorldBossDamageLayer.uiLayout:FindChildObjectByName("damageNum"..index).__UILabel__

		if WorldBossDamageData[index] ~= nil then			

			damageNameLabel:setString(""..WorldBossDamageData[index].rank.."."..WorldBossDamageData[index].name)

			local hurtPercentStr = GetWorldBossDamagePercentStr(WorldBossDamageData[index].percentage)
			damageNumLabel:setString(""..MainMenuLayer:GetNumByFormatK(WorldBossDamageData[index].hurt).."".."["..hurtPercentStr.."]")
		else
			damageNameLabel:setString("")
			damageNumLabel:setString("")
		end		
	end

	local myNameLabel = WorldBossDamageLayer.uiLayout:FindChildObjectByName("damageNameMy").__UILabel__
	local myNumLabel = WorldBossDamageLayer.uiLayout:FindChildObjectByName("damageNumMy").__UILabel__
	if MyDamageData ~= nil and MyDamageData.rank ~= nil and MyDamageData.rank > 0 then
		myNameLabel:setString(""..MyDamageData.rank.."."..MyDamageData.name)

		local hurtPercentStr = GetWorldBossDamagePercentStr(MyDamageData.percentage)
		myNumLabel:setString(""..MainMenuLayer:GetNumByFormatK(MyDamageData.hurt).."".."["..hurtPercentStr.."]")
	else
		myNameLabel:setString("")
		myNumLabel:setString("")
	end
end

local function onMsgHurtRank(msg)
	CCLuaLog("onMsgHurtRank")
	if msg.ranking ~= nil then
		index = 1
		WorldBossDamageData = {}
		for _, info in pairs(msg.ranking) do
			WorldBossDamageData[index] = {
				rank = info.rank,
				name = info.name,
				hurt = info.hurt,
				percentage = info.percentage,
			}
			index = index + 1
		end
	end

	MyDamageData = nil
	if msg.self_ranking ~= nil then
		MyDamageData = {
			rank = msg.self_ranking.rank,
			name = msg.self_ranking.name,
			hurt = msg.self_ranking.hurt,
			percentage = msg.self_ranking.percentage,
		}
	end

	refreshWorldBossDamageLayer()
	return true
end

function WorldBossDamageLayer:InitLayer()
	CCLuaLog("--- WorldBossDamageLayer:InitLayer() ---")

	refreshWorldBossDamageLayer()
	addMsgCallBack("CSWBRTHurtRankRsp", onMsgHurtRank)
end