require("Script/GameConfig/uiTagDefine")
LoginLayer = {
	layerInstance		= nil,
	uiLayout			= nil,
	nameTextInput		= nil,
	passwordTextInput	= nil,
}

local function onReceiveDestructorHandler()
	LoginLayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("loginUILayout")
end

function LoginLayer:create(parentNode)
	CCLuaLog("LoginLayer:create")
	if parentNode == nil then 
		return nil
	end

	if self.layerInstance == nil then 
		self.layerInstance = CCLayer:create()
		if parentNode ~= nil then
			local tag = UITagTable["LoginLayer"].tag
			parentNode:addChild(self.layerInstance,1,tag)
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
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/loginUI.plist",self.layerInstance, "loginUILayout", true)
			self:InitLayer()
		end
	else
		self:setVisible(true)
	end

	return self.layerInstance
end

-- ÷ÿ÷√≤Œ ˝
function LoginLayer:resetValue()
	self.layerInstance = nil
	self.uiLayout = nil
	self.nameTextInput = nil
	self.passwordTextInput = nil
end

function LoginLayer:setVisible(isVisible)
	if self.layerInstance ~= nil then
		self.layerInstance:setVisible(isVisible)
	end
end

local function onLoginBtClicked(tag)
	CCLuaLog("onLoginBtClicked")
    if LoginLayer.nameTextInput ~= nil and LoginLayer.passwordTextInput ~= nil then
        local userName = LoginLayer.nameTextInput:getString()
        local password = LoginLayer.passwordTextInput:getString()
		
        LoginManager:Get():loginWithUserName(userName,password)
    end
    
end

local function onRegisterBtClicked(tag)
	CCLuaLog("onRegisterBtClicked")
	LoginLayer:setVisible(false)
	enterRigisterLayer()
end

function LoginLayer:InitLayer()
	local registerBt = self.uiLayout:FindChildObjectByName("registerBt").__UIButton__:getMenuItemSprite()
	registerBt:registerScriptTapHandler(onRegisterBtClicked)
	local loginBt = self.uiLayout:FindChildObjectByName("loginBt").__UIButton__:getMenuItemSprite()
	loginBt:registerScriptTapHandler(onLoginBtClicked)
	self.nameTextInput = self.uiLayout:FindChildObjectByName("NameInput").__UIEditBox__
	self.passwordTextInput = self.uiLayout:FindChildObjectByName("passwordInput").__UIEditBox__
    if	XLogicManager:sharedManager():getAutoLogin() then
		local hasLoginHistroy = LoginManager:Get():doRecentLogin()
		self:setVisible(false)
		if not hasLoginHistroy then
			enterRigisterLayer()    
		end
	end
end