require("Script/GameConfig/uiTagDefine")

WorldBossHPLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()

	WorldBossHPLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("WorldBossHPUI")
end

function WorldBossHPLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["WorldBossHPLayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/WorldBossHPUI.plist",self.uiLayerInstance, "WorldBossHPUI", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function WorldBossHPLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
end

-- 删除UI
function WorldBossHPLayer:destroyed()
	CCLuaLog("--- WorldBossHPLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	self:resetValue()
end

local function onMsgBossHP(msg)
	if msg.monsters ~= nil then
		for _ , info in pairs(msg.monsters) do
			SpriteMonsterMgr:Get():updateSpecialMonsterHPLayer(info.id, info.blood, info.total_hp, 1)
		end
		return true
	end	
end

local function onMsgHeroInstanceHP(msg)
	if msg.monsters ~= nil then
		for _ , info in pairs(msg.monsters) do
			SpriteMonsterMgr:Get():updateSpecialMonsterHPLayer(info.id, info.blood, info.total_hp, 2)
		end
		return true
	end	
end

local function onMsgCityHP(msg)
	if msg.monsters ~= nil then
		for _ , info in pairs(msg.monsters) do
			SpriteMonsterMgr:Get():updateSpecialMonsterHPLayer(info.id, info.blood, info.total_hp, 3)
		end
		return true
	end	
end

function WorldBossHPLayer:InitLayer()
	CCLuaLog("--- WorldBossHPLayer:InitLayer() ---")
    local isCityDefend = LevelManager:sShareInstance():isCurrentCityDefendLevel()
    if isCityDefend then
        self.uiLayout:FindChildObjectByName("bossHPBG"):setVisible(false)
        self.uiLayout:FindChildObjectByName("bossHPBar"):setVisible(false)
        self.uiLayout:FindChildObjectByName("bossName"):setVisible(false)
        self.uiLayout:FindChildObjectByName("bossLevel"):setVisible(false)
        self.uiLayout:FindChildObjectByName("bossHPNum"):setVisible(false)
    else
        self.uiLayout:FindChildObjectByName("cityHPBG"):setVisible(false)
        self.uiLayout:FindChildObjectByName("cityHPBar"):setVisible(false)
        self.uiLayout:FindChildObjectByName("cityName"):setVisible(false)
    end
	
	addMsgCallBack("CSWBMonsterBloodRsp", onMsgBossHP)
    addMsgCallBack("CSBBMonsterBloodRsp", onMsgHeroInstanceHP)
    addMsgCallBack("CSCityBloodRsp", onMsgCityHP)
end