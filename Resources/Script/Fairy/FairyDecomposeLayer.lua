require("Script/GameConfig/uiTagDefine")
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")
require("Script/Fairy/FairyMessage")

FairyDecomposeLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	fairyPos = 0,
	itemIdTable = {},
	fairyPosList = {},
    fragmentIds = {},
    fragmentList = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	TXGUI.UIManager:sharedManager():removeUILayout("FairyDecomposeLayout")
	FairyDecomposeLayer:resetValue()
end

function FairyDecomposeLayer:createLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayerColor:create(ccc4(0,0,0,200))
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["FairyDecomposeLayer"].tag
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/FairyDecomposeUI.plist",self.uiLayerInstance, "FairyDecomposeLayout", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function FairyDecomposeLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.fairyPos = 0
	self.itemIdTable = {}
	self.fairyPosList = {}
    self.fragmentIds = {}
    self.fragmentList = {}
end
local function onCloseButtonClicked(tag)
	if FairyDecomposeLayer.uiLayerInstance ~= nil then
		FairyDecomposeLayer.uiLayerInstance:removeFromParentAndCleanup(true)
	end
end

local function onMaterialButtonClicked(tag)
	if FairyDecomposeLayer.uiLayout ~= nil then
		local materialBt = FairyDecomposeLayer.uiLayout:FindChildObjectByName("materialBt" .. tag).__UIButton__
		local pos = materialBt:getMenuNode():getTouchPoint()
		if FairyDecomposeLayer.itemIdTable[tag] ~= nil then
			ItemManager:Get():showItemTipsById(FairyDecomposeLayer.itemIdTable[tag],pos)
		end
	end
end

local function onFragmentButtonClicked(tag)
	if FairyDecomposeLayer.uiLayout ~= nil then
		local materialBt = FairyDecomposeLayer.fragmentList[tag]
		local pos = materialBt:getMenuNode():getTouchPoint()
		if FairyDecomposeLayer.fragmentIds[tag] ~= nil then
			ItemManager:Get():showItemTipsById(FairyDecomposeLayer.fragmentIds[tag],pos)
		end
	end
end

local function onConfirmButtonClicked(tag)
	sendFairyDecomposeReq(FairyDecomposeLayer.fairyPosList,false,false)
end

local function onFragmentsConfirmButtonClicked(tag)
	sendFairyDecomposeReq(FairyDecomposeLayer.fairyPosList,false,true)
end

function FairyDecomposeLayer:hideSingleMaterial(index)
	local materialIcon = self.uiLayout:FindChildObjectByName("materialBt" .. index)
	if 	materialIcon ~= nil then
		local bt = materialIcon.__UIButton__
		bt:setVisible(false)
	end
end

function FairyDecomposeLayer:setSingleMaterial(itemId,itemNum,index)
	local materialIcon = self.uiLayout:FindChildObjectByName("materialBt" .. index)
	if 	materialIcon ~= nil then
		local bt = materialIcon.__UIButton__
		local icon = ItemManager:Get():getIconSpriteById(itemId)
		bt:setIconSprite(icon)
		bt:setVisible(true)
		local size = bt:getMenuItemSprite():getContentSize()
		bt:setTextLabel(itemNum,18,ccc3(255,171,0),KJLinXin, ccp(size.width * (0.4),size.height * (-0.4)),ccp(1,0))
		self.itemIdTable[index] = itemId
	end
end

function FairyDecomposeLayer:setMaterial(itemTable)
	local index = 1
	for k,v in pairs(itemTable) do
		local itemId = v.id
		local itemNum = v.num
		self:setSingleMaterial(itemId,itemNum,index)
		index = index + 1
	end
	if index < 4 then
		for i = index , 4 do 
			self:hideSingleMaterial(i)
		end
	end
end

function FairyDecomposeLayer:setFragments(itemTable)
	local scale =  TXGUI.UIManager:sharedManager():getScaleFactor();
	local page = self.uiLayout:FindChildObjectByName("FRAGMENT_FRAME").__UIScrollPage__
    page:setScale(scale)
    page:getScrollLayer():setShowPagesIndicator(false)

    local index = 0
	local currentLayer = nil
    if table.getn(itemTable) <= 4 then
        page:getScrollLayer():setTouchEnabled(false)
    end

    for k,v in pairs(itemTable) do
        if index % 4 == 0 then
			currentLayer = CCLayer:create()
			currentLayer:setAnchorPoint(ccp(0,0))
			page:addPage(currentLayer)
        end
        
		local buttonFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_SPLIT_MATERIAL21")
		local clickFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_SPLIT_MATERIAL21_CLICKED")
        local icon = ItemManager:Get():getIconSpriteById(v.id)

        local button = IconButton:create(buttonFrame,nil,clickFrame,-137)
        button:setContentSize(buttonFrame:getContentSize())
        button:getMenuItem():setTag(index)
		button:getMenuItem():registerScriptTapHandler(onFragmentButtonClicked)
        button:SetNormalIcon(icon)

		local size = button:getContentSize()
        local label_name = TXGUI.UILabelTTF:create(""..v.num, KJLinXin, 20)
		label_name:setPosition(ccp(size.width/2 - 5*scale, - size.height/2 + 5*scale));
        label_name:setAnchorPoint(ccp(1,0));
		button:addChild(label_name,100)
        button:setPosition(ccp((size.width + 28) / 2 * (2 * (index % 4) - 3) , 0));
		currentLayer:addChild(button)

        self.fragmentIds[index] = v.id
        self.fragmentList[index] = button

		index = index + 1
	end




end

function FairyDecomposeLayer:onReceiveFairyDecomposeRsp(msg)
	if msg.req_info then
		if msg.to_items ~= nil then
			self:setMaterial(msg.to_items)
		end
		if msg.fragments ~= nil then
			self:setFragments(msg.fragments)
		end
	else
		FairyBagLayer:removeOneFairyInBag(msg.fairy_pos)
        if msg.is_fragments then
		    GameApi:showMessage(GetLuaLocalization("M_FAIRY_BAG_2"))
            FairyBagLayer:ResetFragData()
        else
		    GameApi:showMessage(GetLuaLocalization("M_FAIRY_BAG_1"))
        end
		onCloseButtonClicked(nil)
	end
end


function FairyDecomposeLayer:InitLayer()
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseButtonClicked)
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	confirmBt:registerScriptTapHandler(onConfirmButtonClicked)
	local fragmentsConfirmBt = self.uiLayout:FindChildObjectByName("fragmentsConfirmBt").__UIButton__:getMenuItemSprite()
	fragmentsConfirmBt:registerScriptTapHandler(onFragmentsConfirmButtonClicked)
	for i = 1,4 do
		local materialBt = self.uiLayout:FindChildObjectByName("materialBt" .. i).__UIButton__
		materialBt:setVisible(false)
		local materialItem = materialBt:getMenuItemSprite()
		materialItem:registerScriptTapHandler(onMaterialButtonClicked)
		materialItem:setTag(i)
	end
end

function FairyDecomposeLayer:setFairyPos(fairyList)
	self.fairyPosList = fairyList
	sendFairyDecomposeReq(self.fairyPosList,true,false)
end