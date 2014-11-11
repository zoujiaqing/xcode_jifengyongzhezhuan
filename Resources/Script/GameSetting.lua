

require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/Mail/MailUILayer")
require("Script/Notice2")
require("Script/UILayer/FeedBackUI")
require("Script/UILayer/exchangeCodeLayer")
--number of players on the mainland(100-precent) 屏蔽的百分比，服务器好处理
local NUMBER_PLAYERS_MAX = 0;
local NUMBER_PLAYERS_MID = 50;
local NUMBER_PLAYERS_MIN = 100;

--quality setting 品质常量
local QUALITY_HIGH = 3;
local QUALITY_NORMAL = 2;
local QUALITY_LOW = 1;

GameSetting = {
	LayerInstance = nil,
    LayerSetting = nil,
    
    playeronScreenMuch_key = "playeronScreenMuch",
    playeronScreenMuch_val = false,
    
    playeronScreenNormal_key = "playeronScreenNormal",
    playeronScreenNormal_val = false,
    
    playeronScreenLow_key = "playeronScreenLow",
    playeronScreenLow_val = false,
    
    graphicsMuch_key = "graphicsMuch",
    graphicsMuch_val = false,
    
    graphicsNormal_key = "graphicsNormal",
    graphicsNormal_val = false,
    
    graphicsLow_key = "graphicsLow",
    graphicsLow_val = false,
    
    playeronScreenMuch_Pic = nil,
    playeronScreenNormal_Pic = nil,
    playeronScreenLittle_Pic = nil,
    
    graphicMuch_Pic = nil,
    graphicNormal_Pic = nil,
    graphicLow_Pic = nil,
}

function GameSetting:ResetValue()
	self.LayerInstance = nil
    self.LayerSetting = nil
    
    self.playeronScreenMuch_key = "playeronScreenMuch"
    self.playeronScreenMuch_val = false
    
    self.playeronScreenNormal_key = "playeronScreenNormal"
    self.playeronScreenNormal_val = false
    
    self.playeronScreenLow_key = "playeronScreenLow"
    self.playeronScreenLow_val = false
    
    self.graphicsMuch_key = "graphicsMuch"
    self.graphicsMuch_val = false
    
    self.graphicsNormal_key = "graphicsNormal"
    self.graphicsNormal_val = false
    
    self.graphicsLow_key = "graphicsLow"
    self.graphicsLow_val = false
    
    self.playeronScreenMuch_Pic = nil
    self.playeronScreenNormal_Pic = nil
    self.playeronScreenLittle_Pic = nil
    
    self.graphicMuch_Pic = nil
    self.graphicNormal_Pic = nil
    self.graphicLow_Pic = nil
    
end

function sendSettingAttrReq()
	CSAttributesReq = {
		keys = {
			PB_ATTR_MXA_VIEW_PLAYER,
			PB_ATTR_GAME_PIC_QUALITY,
		}
	};
	local msgName = "CSAttributesReq";
	send_message(msgName, CSAttributesReq, true)
end

function GameSetting:CreateLayer(parentLayer)

    print("GameSetting:CreateLayer")
	self:ResetValue()

	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
    
    self.LayerSetting = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/GameSetting.plist",self.LayerInstance, "GameSetting", true)
    local container =  self.LayerSetting:FindChildObjectByName("GameSettingContainer").__UIContainer__;
    
    if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["GameSetting"].tag
        mainMenuNode:addChild(self.LayerInstance,0,tag)
        InitMoneyConfigLayer(self.LayerInstance)
        
        local ccnode = container:getCurrentNode();
        local uiframePic = self.LayerSetting:FindChildObjectByName("SETTING_BACKGROUND").__UIPicture__;
        local size = uiframePic:getSpriteSize()
        
        local width = size.width
        local heigh = size.height
        
        CreateSliderCtrl(ccnode,1,width,heigh)
        CreateSliderCtrl(ccnode,2,width,heigh)
	end	
    
	PushOneLayer(self.LayerInstance,"","")
   
    self:ReadValue()
    self:setupSetting()    
end

function GameSetting:ReadValue()
    self.playeronScreenMuch_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.playeronScreenMuch_key);
    self.playeronScreenNormal_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.playeronScreenNormal_key);
    self.playeronScreenLow_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.playeronScreenLow_key);
    
    self.graphicsMuch_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.graphicsMuch_key);
    self.graphicsNormal_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.graphicsNormal_key);
    self.graphicsLow_val = CCUserDefault:sharedUserDefault():getBoolForKey(self.graphicsLow_key);
end

function GameSetting:SaveValue()

    CCUserDefault:sharedUserDefault():setBoolForKey(self.playeronScreenMuch_key, self.playeronScreenMuch_val);
    CCUserDefault:sharedUserDefault():setBoolForKey(self.playeronScreenNormal_key, self.playeronScreenNormal_val);
    CCUserDefault:sharedUserDefault():setBoolForKey(self.playeronScreenLow_key, self.playeronScreenLow_val);
    
    CCUserDefault:sharedUserDefault():setBoolForKey(self.graphicsMuch_key, self.graphicsMuch_val);
    CCUserDefault:sharedUserDefault():setBoolForKey(self.graphicsNormal_key, self.graphicsNormal_val);
    CCUserDefault:sharedUserDefault():setBoolForKey(self.graphicsLow_key, self.graphicsLow_val);

    CCUserDefault:sharedUserDefault():flush();
end

function GameSetting:setupSetting()
    

    
    local btn_playeronscreen_high = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_MUCH").__UIButton__:getMenuItemSprite();
    local btn_playeronscreen_normal = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_NORMAL").__UIButton__:getMenuItemSprite();
    local btn_playeronscreen_little = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_LITTLE").__UIButton__:getMenuItemSprite();
    
    local btn_graphics_high = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_HIGH").__UIButton__:getMenuItemSprite();
    local btn_graphics_normal = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_NORMAL").__UIButton__:getMenuItemSprite();
    local btn_graphics_low = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_LOW").__UIButton__:getMenuItemSprite();


    local btn_exit = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_EXIT").__UIButton__:getMenuItemSprite();
    local btn_customService = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_CUSTOMSERVICE").__UIButton__:getMenuItemSprite();
    
    -- if channelid is cocoappstore(32),then hide the button
    local bun_exchangeCode = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_EXCHANGE").__UIButton__:getMenuItemSprite();
    if(G_GameChannelID ~= "32") then
        self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_EXCHANGE").__UIButton__:setVisible(true);
    else
        self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_EXCHANGE").__UIButton__:setVisible(false);
    end
    
    local btn_onclose = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
    
    btn_playeronscreen_high:registerScriptTapHandler(onPlayeronScreenHighClick);
    btn_playeronscreen_normal:registerScriptTapHandler(onPlayeronScreenNormalClick);
    btn_playeronscreen_little:registerScriptTapHandler(onPlayeronScreenLittleClick);
    
    btn_graphics_high:registerScriptTapHandler(onGraphicsHighClick);
    btn_graphics_normal:registerScriptTapHandler(onGraphicsNormalClick);
    btn_graphics_low:registerScriptTapHandler(onGraphicsLowClick);
    
    btn_exit:registerScriptTapHandler(onExitClick);
    btn_customService:registerScriptTapHandler(onCustomServiceClick);
    bun_exchangeCode:registerScriptTapHandler(onExchangeCodeClick)
    btn_onclose:registerScriptTapHandler(onGameSettingClick);
    
    self.playeronScreenMuch_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_MUCH_UNSELECT").__UIPicture__;
    self.playeronScreenNormal_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_NORMAL_UNSELECT").__UIPicture__;
    self.playeronScreenLittle_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_LITTLE_UNSELECT").__UIPicture__;
    
    self.graphicMuch_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_HIGH_UNSELECT").__UIPicture__;
    self.graphicNormal_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_NORMAL_UNSELECT").__UIPicture__;
    self.graphicLow_Pic = self.LayerSetting:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_LOW_UNSELECT").__UIPicture__;


    self:SetPlayeronScreenShowVisible();
	self:SetGraphicsShowVisible();
end

function GameSetting:SetPlayeronScreenShowVisible()
	if self.LayerInstance == nil then
		CCLuaLog("--------------------------------SetPlayeronScreenShowVisible instance = nil-----------------")
		return
	end
    if  self.playeronScreenMuch_val == false and self.playeronScreenNormal_val == false and  self.playeronScreenLow_val == false then
         self:ShowVisible_playerson(2)
    elseif self.playeronScreenMuch_val == true then
        self:ShowVisible_playerson(1)
    elseif self.playeronScreenNormal_val == true then
         self:ShowVisible_playerson(2)
    elseif self.playeronScreenLow_val == true then
         self:ShowVisible_playerson(3)
    end
end

function GameSetting:SetGraphicsShowVisible()
	if self.LayerInstance == nil then
		CCLuaLog("--------------------------------SetGraphicsShowVisible instance = nil-----------------")
		return
	end
	if  self.graphicsMuch_val == false and self.graphicsNormal_val == false and  self.graphicsLow_val == false then
         self:ShowVisible_Graphic(1)
        
    elseif self.graphicsMuch_val == true then
         self:ShowVisible_Graphic(1)
         
    elseif self.graphicsNormal_val == true then
         self:ShowVisible_Graphic(2)
         
    elseif self.graphicsLow_val == true then
         self:ShowVisible_Graphic(3)
    end
end

function GameSetting:ShowVisible_playerson(index)
   if index == 1 then
      self.playeronScreenMuch_Pic:setVisible(true)
      self.playeronScreenNormal_Pic:setVisible(false)
      self.playeronScreenLittle_Pic:setVisible(false)
      print("showVisible 11111")
   elseif index == 2 then
   
      self.playeronScreenMuch_Pic:setVisible(false)
      self.playeronScreenNormal_Pic:setVisible(true)
      self.playeronScreenLittle_Pic:setVisible(false)
	  print("showVisible 2222")

   elseif index == 3 then
      self.playeronScreenMuch_Pic:setVisible(false)
      self.playeronScreenNormal_Pic:setVisible(false)
      self.playeronScreenLittle_Pic:setVisible(true)
	  print("showVisible 3333")
   end
end


function GameSetting:ShowVisible_Graphic(index)
	if index == 1 then
      self.graphicMuch_Pic:setVisible(true)
      self.graphicNormal_Pic:setVisible(false)
      self.graphicLow_Pic:setVisible(false)
      
   elseif index == 2 then
      self.graphicMuch_Pic:setVisible(false)
      self.graphicNormal_Pic:setVisible(true)
      self.graphicLow_Pic:setVisible(false)

   elseif index == 3 then
      self.graphicMuch_Pic:setVisible(false)
      self.graphicNormal_Pic:setVisible(false)
      self.graphicLow_Pic:setVisible(true)

   end
end

function onPlayeronScreenHighClick(pObj)
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  local btn_playeronscreen_high = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_MUCH").__UIButton__:getMenuItemSprite()
  local btn_playeronscreen_normal = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_NORMAL").__UIButton__:getMenuItemSprite()
  local btn_playeronscreen_little = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_LITTLE").__UIButton__:getMenuItemSprite()

  GameSetting:ShowVisible_playerson(1)

  GameSetting.playeronScreenMuch_val = true
  GameSetting.playeronScreenNormal_val = false
  GameSetting.playeronScreenLow_val = false  
  print("onPlayeronScreenHighClick")
end

function onPlayeronScreenNormalClick(pObj)

  print("onPlayeronScreenNormalClick")
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  local btn_playeronscreen_high =   player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_MUCH").__UIButton__:getMenuItemSprite();
  local btn_playeronscreen_normal = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_NORMAL").__UIButton__:getMenuItemSprite();
  local btn_playeronscreen_little = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_LITTLE").__UIButton__:getMenuItemSprite();
  
  GameSetting:ShowVisible_playerson(2)

  GameSetting.playeronScreenMuch_val = false
  GameSetting.playeronScreenNormal_val = true
  GameSetting.playeronScreenLow_val = false

  --GameSetting:SaveValue()
  
end

function onPlayeronScreenLittleClick(pObj)
  print("onPlayeronScreenLittleClick")
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  
  local btn_playeronscreen_high = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_MUCH").__UIButton__:getMenuItemSprite();
  local btn_playeronscreen_normal = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_NORMAL").__UIButton__:getMenuItemSprite();
  local btn_playeronscreen_little = player:FindChildObjectByName("SETTING_BUTTON_PLAYERONSCREEN_LITTLE").__UIButton__:getMenuItemSprite();
    
  GameSetting:ShowVisible_playerson(3)

  GameSetting.playeronScreenMuch_val = false
  GameSetting.playeronScreenNormal_val = false
  GameSetting.playeronScreenLow_val = true
  
  --GameSetting:SaveValue()
  
end

function onGraphicsHighClick(pObj)
  print("onGraphicsHighClick")
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  local btn_graphics_high = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_HIGH").__UIButton__:getMenuItemSprite();
  local btn_graphics_normal = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_NORMAL").__UIButton__:getMenuItemSprite();
  local btn_graphics_low = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_LOW").__UIButton__:getMenuItemSprite();
  
  
  GameSetting:ShowVisible_Graphic(1)
           
  GameSetting.graphicsMuch_val= true
  GameSetting.graphicsNormal_val = false
  GameSetting.graphicsLow_val = false
  
end

function onGraphicsNormalClick(pObj)
  print("onGraphicsNormalClick")
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  local btn_graphics_high = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_HIGH").__UIButton__:getMenuItemSprite();
  local btn_graphics_normal = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_NORMAL").__UIButton__:getMenuItemSprite();
  local btn_graphics_low = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_LOW").__UIButton__:getMenuItemSprite();
  
  GameSetting:ShowVisible_Graphic(2)

  GameSetting.graphicsMuch_val= false
  GameSetting.graphicsNormal_val = true
  GameSetting.graphicsLow_val = false
  
end

function onGraphicsLowClick(pObj)
  print("onGraphicsLowClick")
  
  local player = TXGUI.UIManager:sharedManager():getUILayout("GameSetting");
  local btn_graphics_high = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_HIGH").__UIButton__:getMenuItemSprite();
  local btn_graphics_normal = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_NORMAL").__UIButton__:getMenuItemSprite();
  local btn_graphics_low = player:FindChildObjectByName("SETTING_BUTTON_GRAPHICS_LOW").__UIButton__:getMenuItemSprite();
  
  GameSetting:ShowVisible_Graphic(3)

  GameSetting.graphicsMuch_val= false
  GameSetting.graphicsNormal_val = false
  GameSetting.graphicsLow_val = true
end

-- function onChangePassWordClick(pObj)
  -- print("onChangePassWordClick")
  
  -- onGameSettingClick(nil)
  -- StopHeroMove()
  -- Notice2:CreateLayer(nil)
  
-- end

-- function onSecurityClick(pObj)
  -- print("onSecurityClick")
-- end

-- function onForgetPassWordClick(pObj)
  -- print("onForgetPassWordClick")
-- end

function onExitClick(pObj)
	CCLuaLog("onExitClick")
	onGameSettingClick(nil)
	StopHeroMove()
	XLogicManager:sharedManager():fromGameBackToLogin();
end

function onCustomServiceClick(pObj)
  print("onCustomServiceClick")
  onGameSettingClick(nil)
  FeedBackUI:CreateLayer(nil)
  --FeedBackLayer:CreateLayer(GameSetting.LayerInstance)
  --GameSetting.LayerInstance:addChild(feedbacklayer, 100)
end

function onExchangeCodeClick(tag)
	onGameSettingClick(nil)
	ExchangeCodelUILayer:createLayer()
end

-- function onMailClick(pObj)
  -- print("onMailClick")
  -- onGameSettingClick(nil)
  -- StopHeroMove()
  -- MailUILayer:createLayer()
-- end

function setNumberOfPLayersSettingValue(val)
	if val == nil then
		return;
	end
	CCLuaLog("------------setNumberOfPLayersSettingValue---------------");

	if val == NUMBER_PLAYERS_MAX then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenMuch_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenMuch_key, false);
	end

	if val == NUMBER_PLAYERS_MID then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenNormal_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenNormal_key, false);
	end

	if val == NUMBER_PLAYERS_MIN then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenLow_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.playeronScreenLow_key, false);
	end

    CCUserDefault:sharedUserDefault():flush();

	GameSetting:SetPlayeronScreenShowVisible();
	
end

function setQualitySettingValue(val)
	if val == nil then
		return;
	end
	CCLuaLog("------------setQualitySettingValue---------------");

	if val == QUALITY_HIGH then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsMuch_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsMuch_key, false);
	end

	if val == QUALITY_NORMAL then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsNormal_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsNormal_key, false);
	end

	if val == QUALITY_LOW then
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsLow_key, true);
	else
		CCUserDefault:sharedUserDefault():setBoolForKey(GameSetting.graphicsLow_key, false);
	end

    CCUserDefault:sharedUserDefault():flush();

	GameSetting:SetGraphicsShowVisible();
end

local function getSettingValue()
	local numberOfPlayers = NUMBER_PLAYERS_MAX;
	local quality = QUALITY_HIGH;
    if GameSetting.playeronScreenMuch_val then
		numberOfPlayers = NUMBER_PLAYERS_MAX;
	elseif GameSetting.playeronScreenNormal_val then
		numberOfPlayers = NUMBER_PLAYERS_MID;
	elseif GameSetting.playeronScreenLow_val then
		numberOfPlayers = NUMBER_PLAYERS_MIN;
	end
    
	if GameSetting.graphicsMuch_val then
		quality = QUALITY_HIGH;
	elseif GameSetting.graphicsNormal_val then
		quality = QUALITY_NORMAL;
	elseif GameSetting.graphicsLow_val then
		quality = QUALITY_LOW;
	end

	return numberOfPlayers, quality;
end

function onGameSettingClick(pObj)

  print("onGameSettingClick")
  if nil ~= GameSetting.LayerInstance then
        print("inside")
        GameSetting:SaveValue()
		--send setting message start
		local numberOfPlayers, quality = getSettingValue();
		CCLuaLog("numberOfPlayers: " .. numberOfPlayers .. " quality: " .. quality);
		local msgname = "CSAttributesSetReq"
		CSAttributesSetReq1 = {--setting attribute
			key = PB_ATTR_MXA_VIEW_PLAYER,
			value = numberOfPlayers,
		};
		send_message(msgname, CSAttributesSetReq1, true)
		
		CSAttributesSetReq2 = {--setting quality
			key = PB_ATTR_GAME_PIC_QUALITY,
			value = quality,
		};
		send_message(msgname, CSAttributesSetReq2, true)
		--send end;

        SetChatBar(true,-1);
		RemoveOneLayer(GameSetting.LayerInstance)
		TXGUI.UIManager:sharedManager():removeUILayout("GameSetting")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		GameSetting.LayerInstance:removeFromParentAndCleanup(true);
		GameSetting.LayerInstance = nil

  end
end

print("game setting loading!")

