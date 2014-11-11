require("Script/Login/loginLayer")
require("Script/Login/rigisterLayer")
require("Script/Login/loginError")
function enterLoginLayer()
	CCLuaLog("enterLoginLayer")
	local scene = CCDirector:sharedDirector():getRunningScene()
	local layer = nil
	if scene ~= nil then
		local parent = scene:getChildByTag(XLogicManager.E_SELECT_ZONE)
		if parent ~= nil then 
			layer = LoginLayer:create(parent)
            local frontCover = parent:getChildByTag(1)
            if frontCover ~= nil then
                frontCover:setVisible(false)
            end
		end
	end
	local ret = (layer ~= nil)
	return ret
end

function enterRigisterLayer()
	CCLuaLog("enterRigisterLayer")
	local scene = CCDirector:sharedDirector():getRunningScene()
	local layer = nil
	if scene ~= nil then
		local parent = scene:getChildByTag(XLogicManager.E_SELECT_ZONE)
		if parent ~= nil then 
			layer = RigisterLayer:create(parent)
		end
	end
	local ret = (layer ~= nil)
	return ret
end

function onReceiveLoginError(error)
	local errStr = LoginError[error]
	if errStr ~= nil then
		GameApi:showMessage(errStr)
	end
	LoginLayer:setVisible(true)
	RigisterLayer:setVisible(false)
	return true
end

function onReceiveRigisterError(error)
	local errStr = LoginError[error]
	if errStr ~= nil then
		GameApi:showMessage(errStr)
	end
	return true
end