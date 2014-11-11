require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/UILayer/ActivityCentre")
require("Script/UILayer/ShowActivity")
KUI_OPERAION_BIN="UI/ui_operation.bin"

ShowQueryResultUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	AddFriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("showQueryResultLayout")
end

function ShowQueryResultUILayer:CreateLayer(id)
	CCLuaLog("ShowQueryResultUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ShowQueryResultUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/showQueryResult.plist",self.uiLayerInstance, "showQueryResultLayout", true)
			self:InitLayer(id);
		end
	end

	return self.uiLayerInstance
end

function ShowQueryResultUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
end

function ShowQueryResultUILayer:InitLayer(id)
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite();
	confirmBt:registerScriptTapHandler( 
        function(tag)
            if self.uiLayerInstance ~= nil then
	    	    self.uiLayerInstance:removeFromParentAndCleanup(true)
	        end
	        self:resetValue()
        end
    );

    local btLabel = self.uiLayout:FindChildObjectByName("buttonLabel").__UILabel__:setString(GetLuaLocalization("G_BUTTON_OK"))

    local btLabel = self.uiLayout:FindChildObjectByName("titleLable").__UILabel__:setString(GetLuaLocalization("M_SPEND_TIP"))

    local sjparam

    for i, v in pairs(ActivityCentre) do
        if v.Activity_Show == id then
  	        if string.len(v.Activity_Qualification) > 1 then
		        sjparam = v.Activity_Qualification          

                --print("*****Activity_Qualification: "..sjparam)

                for index, goods_id, goods_num in string.gfind(sjparam, "(%d+)\/(%d+)\/(%d+)") do     
                    local sprite = ItemManager:Get():getIconSpriteById(goods_id)
                    if sprite ~= nil then
                        self.uiLayout:FindChildObjectByName("xiaohaowupinIcon").__UIPicture__:setSprite(sprite)	
                    end    
                    --print("******cost_num: "..v.cost_num)
                    self.uiLayout:FindChildObjectByName("xiaohaowupinshuliangIcon").__UILabel__:setString("   "..tostring(v.cost_num))	  
                end
            end
        end
    end
end


