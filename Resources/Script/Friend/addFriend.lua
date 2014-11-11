require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/Friend/FriendInfoDataCenter")
AddFriendUILayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
	uiTextInput = nil,
	callBackFlag = false,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	AddFriendUILayer:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("addFriendLayout")
end

function AddFriendUILayer:CreateLayer()
	CCLuaLog("AddFriendUILayer:CreateLayer()!");
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["AddFriendUILayer"].tag
			parentNode:addChild(self.uiLayerInstance,0,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/addFriend.plist",self.uiLayerInstance, "addFriendLayout", true)
			self:InitLayer();
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function AddFriendUILayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.uiTextInput = nil
end

-- 删除UI
function AddFriendUILayer:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	self:resetValue()
end

-- 添加好友回包
local function receiveAddFriendRsp(msg)
	CCLuaLog("receiveAddFriendRsp ")
	local ret = false;
	if msg.rsp ~= nil then
		CCLuaLog("rsp is: " .. msg.rsp)
		if msg.rsp == "succ" then
			local text = Localization['M_FRIEND_SUCCESS']["SimpleChinese"];
			GameApi:showMessage(text);
			if AddFriendUILayer.uiLayerInstance ~= nil then
				sendCSPartnerInfoReq()
				AddFriendUILayer:destroyed()
			end
		elseif msg.rsp == "existed" then
			local text = Localization['M_FRIEND_ERROR3']["SimpleChinese"];
			GameApi:showMessage(text);
		elseif msg.rsp == "nofound" then
			local text = Localization['M_FRIEND_ERROR2']["SimpleChinese"];
			GameApi:showMessage(text);
		elseif msg.rsp == "overflow" then
			local text = Localization['M_FRIEND_ERROR1']["SimpleChinese"];
			GameApi:showMessage(text);
		elseif msg.rsp == "self" then
			GameApi:showMessage("不能添加玩家自己为好友");
		end
	else
		CCLuaLog("rsp is nil")
	end
	ret = true;
	return ret
end

-- 发送添加好友消息
function sendAddFriendReq(playerName)
	CCLuaLog("sendAddFriendReq " .. playerName)
	if AddFriendUILayer.callBackFlag ~= true then
		addMsgCallBack("CSAddPartnerRsp", receiveAddFriendRsp)
		AddFriendUILayer.callBackFlag = true
	end
	CSAddPartnerReq = {
		name	= playerName;
	}
	local msgname="CSAddPartnerReq"
	local ret = send_message(msgname, CSAddPartnerReq, true)
	return ret;
end

-- 添加好友
local function onConfirmButtonClicked(pObj)
	-- add friend to do
	if AddFriendUILayer.uiTextInput ~= nil then
		local name = AddFriendUILayer.uiTextInput:getString()
		if name ~= nil then
			sendAddFriendReq(name)
		end
		AddFriendUILayer.uiTextInput:resignFirstResponder()
	end
	
end

-- 取消
local function onCancellButtonClicked(pObj)
	AddFriendUILayer:destroyed()
end

function AddFriendUILayer:InitLayer()
	local confirmBt = self.uiLayout:FindChildObjectByName("confirmBt").__UIButton__:getMenuItemSprite();
	confirmBt:registerScriptTapHandler(onConfirmButtonClicked);
	local cancellBt = self.uiLayout:FindChildObjectByName("cancellBt").__UIButton__:getMenuItemSprite();
	cancellBt:registerScriptTapHandler(onCancellButtonClicked);

	self.uiTextInput = self.uiLayout:FindChildObjectByName("NameInput").__UITextInputField__;
end