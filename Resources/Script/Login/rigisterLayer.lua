require("Script/GameConfig/uiTagDefine")
RigisterLayer = {
	layerInstance		= nil,
	uiLayout			= nil,
	nameTextInput		= nil,
	passwordTextInput	= nil,
	configTextInput		= nil,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	RigisterLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("registerUILayout")
end

function RigisterLayer:create(parentNode)
	CCLuaLog("RigisterLayerLoginLayer:create")
	if parentNode == nil then 
		CCLuaLog("RigisterLayer:create  parentNode is nil")
		return nil
	end

	if self.layerInstance == nil then 
		self.layerInstance = CCLayer:create()
		if parentNode ~= nil then
			local tag = UITagTable["RigisterLayer"].tag
			parentNode:addChild(self.layerInstance,2,tag)
			self.layerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
            
            local visibleSize = CCDirector:sharedDirector():getVisibleSize()
            local origin = CCDirector:sharedDirector():getVisibleOrigin()
            local backPic = CCSprite:create("UI/frontCover.jpg")
            local scaleX = visibleSize.width/backPic:getTexture():getPixelsWide()
            local scaleY = visibleSize.height/backPic:getTexture():getPixelsHigh()
            local scale = scaleX
            if scaleY > scaleX then
                scale = scaleY
            end
            backPic:setScaleX(scale)
            backPic:setScaleY(scale)
            backPic:setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y))

			self.layerInstance:addChild(backPic,0)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/registerUI.plist",self.layerInstance, "registerUILayout", true)
			self:InitLayer()
		end
	else
		self:setVisible(true)
	end

	return self.layerInstance
end

-- 重置参数
function RigisterLayer:resetValue()
	self.layerInstance = nil
	self.uiLayout = nil
	self.nameTextInput = nil
	self.passwordTextInput = nil
	self.configTextInput = nil
end

function RigisterLayer:setVisible(isVisible)
	if self.layerInstance ~= nil then
		self.layerInstance:setVisible(isVisible)
	end
end

local function onCloseBtClicked(tag)
	RigisterLayer:setVisible(false)
	enterLoginLayer()
end

local function onConfirmBtClicked(tag)
	CCLuaLog("onConfirmBtClicked")
    if RigisterLayer.nameTextInput ~= nil and RigisterLayer.passwordTextInput ~= nil and RigisterLayer.configTextInput ~= nil then
        local userName = RigisterLayer.nameTextInput:getString()
        local password = RigisterLayer.passwordTextInput:getString()
        local confirmText = RigisterLayer.configTextInput:getString()
        LoginManager:Get():registerWithUserName(userName,password,confirmText)
    end
end

function RigisterLayer:InitLayer()
	local registerBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite()
	registerBt:registerScriptTapHandler(onConfirmBtClicked)
	local loginBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	loginBt:registerScriptTapHandler(onCloseBtClicked)
	self.nameTextInput = self.uiLayout:FindChildObjectByName("NameInput").__UIEditBox__
	self.passwordTextInput = self.uiLayout:FindChildObjectByName("passwordInput").__UIEditBox__
	self.configTextInput = self.uiLayout:FindChildObjectByName("confirmInput").__UIEditBox__
end