require("Script/Chat/ChatDataCenter")
require("Script/Chat/ShieldPlayerCenter")
require("Script/Friend/PlayerItemCenter")
require("Script/Friend/FriendInfoDataCenter")
require("Script/Friend/addFriend")
require("Script/Chat/ShieldPlayerInfo")
require("Script/Chat/ChatFuncTable")
require("Script/Chat/ChatLayer")
require("Script/Language")

ChatPlayerOption = {
	uiLayerInstance = nil,
	uiLayout = nil,
	selectedPlayer = nil,
	id = 0 ,
	regtime = 0 ,
	channel = 0 ,
	levelNum = 0,
	strengthNum = 0,
	backpic = nil ;
	playerName = "",
	nameLabel = nil,
	lvLabel = nil ,
	lv = "" ,
	strengthLabel = nil ,
	shieldLabel = nil,
	strength = "",
	layerType = nil,

}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	ChatPlayerOption:resetValue()
	TXGUI.UIManager:sharedManager():removeUILayout("chatPlayerOptionLayout")
end

function ChatPlayerOption:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["ChatPlayerOption"].tag
			parentNode:addChild(self.uiLayerInstance,81,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/chatPopListUI.plist",self.uiLayerInstance, "chatPlayerOptionLayout", true)
			self:InitLayer()
			print("ChatPlayerOption:CreateLayer")
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function ChatPlayerOption:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.nameLabel = nil
	self.selectedPlayer = nil
	self.playerName = ""
	self.levelNum = 0
	self.strengthNum = 0
end

-- 删除UI
function ChatPlayerOption:destroyed()
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end
	self:resetValue()
end
function setPlayerId( id )
	ChatPlayerOption.id = id 

end
function setRegtime(regtime)
	ChatPlayerOption.regtime = regtime 
end
function setChannel(channel)
   ChatPlayerOption.channel = channel
end

function setPlayerLevel(value)
	ChatPlayerOption.levelNum = value	
end

function setPlayerStrength(value)
	ChatPlayerOption.strengthNum = value
end

function setPlayerName(playerName)
   ChatPlayerOption.playerName = playerName 
end

function ChatPlayerOption:EnableModeless(bModel)
	if self.uiLayout ~= nil then
		self.uiLayout:EnableModeless(bModel)
	end
end

function setModelLess(bModel)
	ChatPlayerOption:EnableModeless(bModel)
end

function ChatPlayerOption:setSelectedPlayer(playerId,playerName)
	self.selectedPlayer = playerId
	self.playerName = playerName
	if self.nameLabel ~= nil then
		self.nameLabel:setString(self.playerName)
	end
end
function setInitPlayer(layerType)
   ChatPlayerOption.layerType = layerType
    --local playerInfo = ShieldPlayerInfo:create()
		--playerInfo:setplay(ChatPlayerOption.id ,ChatPlayerOption.regtime ,ChatPlayerOption.channel)
		local playerInfo = PlayerIdMake(ChatPlayerOption.id ,ChatPlayerOption.regtime ,ChatPlayerOption.channel)
	    ChatPlayerOption.selectedPlayer = playerInfo 

		local userInfo =  UserData:Get():GetUserInfo(playerInfo)

		if nil ~= userInfo then
			ChatPlayerOption.lv = "Lv"..userInfo.level;
			print(ChatPlayerOption.lv);
			ChatPlayerOption.strength = "战斗力"..userInfo.effectiveness
			print(ChatPlayerOption.strength)
		else
			ChatPlayerOption.lv = "Lv" .. ChatPlayerOption.levelNum
			ChatPlayerOption.strength = "战斗力" .. ChatPlayerOption.strengthNum
			print("++++++++++++++++++++++++++++++++++++++++++++ " .. ChatPlayerOption.strength)
		end				
	
	showUILayerByTag(UITagTable["ChatPlayerOption"].tag,true)
	if ChatPlayerOption.nameLabel ~= nil then
	    
		ChatPlayerOption.nameLabel:setString(ChatPlayerOption.playerName)
	end
	if ChatPlayerOption.lvLabel ~= nil then
	    
		ChatPlayerOption.lvLabel:setString(ChatPlayerOption.lv)
	end
	if ChatPlayerOption.strengthLabel ~= nil then
	    
		ChatPlayerOption.strengthLabel:setString(ChatPlayerOption.strength)
	end

	if 1 == ChatPlayerOption.layerType then
		if ChatPlayerOption.shieldLabel ~= nil then
			local languageFlag = "M_CHATPLAYER_REMOVE"
			local content = LanguageLocalization:GetLocalization(languageFlag)
			ChatPlayerOption.shieldLabel:setString(content)					
		end
	end

end

function setLayerPostion(valx ,valy)
    if ChatPlayerOption.uiLayout ~=nil then
	
	 local  dlayer = ChatPlayerOption.uiLayout
	 --local dlayer1 = dlayer:getParent()
	-- local parentNode = MainMenuLayer:getMainMenuLayer()
	-- local dp = parentNode:convertToNodeSpace(ccp(valx,valy))
	
       dlayer:setPosition(ccp(valx,valy));

	  
	 end
	 
end
function closeChatPlayerOption()
	ChatPlayerOption:destroyed()
end
function  getChatPlayerOpen()
   local  val = 0
   if ChatPlayerOption.uiLayout ~=nil then
      val =1 
   end
   return val 
end
function  getWinH()
	local val = 0.0
	 if ChatPlayerOption.uiLayout ~=nil then
	  --local layNode = ChatPlayerOption.uiLayout:getCurrentNode()
		val =ChatPlayerOption.backpic:getSpriteSize().height
	 end
	 return val
end
function  getWinW()
   local val = 0.0
	 if ChatPlayerOption.uiLayout ~=nil then
	    --local layNode = ChatPlayerOption.uiLayout:getCurrentNode()
		val =  ChatPlayerOption.backpic:getSpriteSize().width
	 end
	 return  val 
end
function getWinPostionX()
    local val = 0.0
   if ChatPlayerOption.uiLayout ~=nil then
    --local layNode = ChatPlayerOption.uiLayout:getCurrentNode()
     val = ChatPlayerOption.backpic:getSpritePos().x
   end
   return val
end
function getWinPostionY()
    local val = 0.0
   if ChatPlayerOption.uiLayout ~=nil then
    --local layNode = ChatPlayerOption.uiLayout:getCurrentNode()
     val = ChatPlayerOption.backpic:getSpritePos().y
   end
   return val
end
-- 关闭按钮回调
local function onCloseBtClicked(tag)
	ChatPlayerOption:destroyed()
end

-- 私聊按钮回调
local function onChatBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil then
		reqToPrivateChat(ChatPlayerOption.selectedPlayer,ChatPlayerOption.playerName)
	end
	ChatPlayerOption:destroyed()
end

-- 查看装备按钮回调
local function onEquipBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil then
		PlayerItemCenter:setSelectedPlayer(ChatPlayerOption.selectedPlayer)
		showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
	end
	ChatPlayerOption:destroyed()
	--关掉聊天面板，防止太多的界面层次
	ChatLayer:destroyed()
end

-- 添加好友按钮回调
local function onFriendBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil and ChatPlayerOption.playerName ~= nil then
		sendAddFriendReq(ChatPlayerOption.playerName)
	end
	ChatPlayerOption:destroyed()
end

-- 切磋按钮回调
local function onFightBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil then
	   
		--CancelBestFighter()
		--sendPvaiReq(ChatPlayerOption.selectedPlayer:getID(),ChatPlayerOption.selectedPlayer:getRegTime(),ChatPlayerOption.selectedPlayer:getChannel())
		--sendFriendSkill(ChatPlayerOption.selectedPlayer:getID(),ChatPlayerOption.selectedPlayer:getRegTime(),ChatPlayerOption.selectedPlayer:getChannel())
		--PushOneWaitingLayer("CSPVAIReq")
		sendCPVPLearnPrepare(ChatPlayerOption.selectedPlayer:getID(),ChatPlayerOption.selectedPlayer:getRegTime(),ChatPlayerOption.selectedPlayer:getChannel(),false)
		 ChatPlayerOption:destroyed()
	end
end

-- 屏闭按钮回调
local function onShieldBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil then
		if 1 == ChatPlayerOption.layerType then
			sendCSDelEnemyReq(ChatPlayerOption.selectedPlayer)
		else
			sendCSAddDndReq(ChatPlayerOption.selectedPlayer)
		end		
	end
	ChatPlayerOption:destroyed()
end
-- 屏闭按钮回调
local function onEnforceBtClicked(tag)
	if ChatPlayerOption.selectedPlayer ~= nil then
		sendCPVPLearnPrepare(ChatPlayerOption.selectedPlayer:getID(),ChatPlayerOption.selectedPlayer:getRegTime(),ChatPlayerOption.selectedPlayer:getChannel(),true)
	end
	ChatPlayerOption:destroyed()
end
function sendCPVPLearnPrepare(id,reg,channel,ismandatory)

	CSPVPLearnPrepareReq = {
		uid	= id,
		reg_time = reg,
		channel_id = channel,
		mandatory = ismandatory,
		}
	local msgname="CSPVPLearnPrepareReq"
	local ret = send_message(msgname, CSPVPLearnPrepareReq, true)


	return ret;
end
function ChatPlayerOption:InitLayer()
	self.nameLabel = self.uiLayout:FindChildObjectByName("playNameLable").__UILabel__
	self.lvLabel = self.uiLayout:FindChildObjectByName("playLvLable").__UILabel__
	self.strengthLabel = self.uiLayout:FindChildObjectByName("playStrengthLable").__UILabel__
	local closeBt = self.uiLayout:FindChildObjectByName("closeBt").__UIButton__:getMenuItemSprite()
	closeBt:registerScriptTapHandler(onCloseBtClicked)
	local chatBt = self.uiLayout:FindChildObjectByName("chatBt").__UIButton__:getMenuItemSprite()
	chatBt:registerScriptTapHandler(onChatBtClicked)
	local equipBt = self.uiLayout:FindChildObjectByName("equipBt").__UIButton__:getMenuItemSprite()
	equipBt:registerScriptTapHandler(onEquipBtClicked)
	local addFriendBt = self.uiLayout:FindChildObjectByName("addFriendBt").__UIButton__:getMenuItemSprite()
	addFriendBt:registerScriptTapHandler(onFriendBtClicked)
	local fightBt = self.uiLayout:FindChildObjectByName("fightBt").__UIButton__:getMenuItemSprite()
	fightBt:registerScriptTapHandler(onFightBtClicked)
	local shieldBt = self.uiLayout:FindChildObjectByName("enforceFightBt").__UIButton__:getMenuItemSprite()
	shieldBt:registerScriptTapHandler(onEnforceBtClicked)
	local shieldBt = self.uiLayout:FindChildObjectByName("shieldBt").__UIButton__:getMenuItemSprite()
	shieldBt:registerScriptTapHandler(onShieldBtClicked)
	self.backpic =self.uiLayout:FindChildObjectByName("backPic").__UIPicture__
	self.shieldLabel = self.uiLayout:FindChildObjectByName("shieldLable").__UILabel__
	 print("ChatPlayerOption:InitLayer")
	if self.backpic ~= nil then
	 print("has backContainer")
	end
	
end