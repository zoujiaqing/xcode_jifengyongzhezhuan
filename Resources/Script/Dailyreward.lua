


require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")
require("Script/handleMessage")
require("Script/DailyLogin")
require("Script/DailyRewardPVAI/dailyRewardPvaiLayer")

Dailyreward = {

	LayerInstance = nil,
    LayreReward = nil,
    LayerLogin = nil,
    LayerSalary = nil,
    
    currentRewardIndex = 0,
    doubleRewardIndex = 0,
    ListButtons = {},
}

function Dailyreward:ResetValue()

	self.LayerInstance = nil
    self.LayreReward = nil
    self.LayerLogin = nil
    self.LayerSalary = nil
    
    currentRewardIndex = 0
    doubleRewardIndex = 0
    self.ListButtons = {}
end

function Dailyreward:CreateLayer(parentLayer)

    print("Dailyreward:CreateLayer")
    
	self:ResetValue()
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end
    
    self.LayreReward = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/Dailyreward.plist",self.LayerInstance, "DailyReward", true)
    self.LayerLogin  = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/DailyLogin.plist",self.LayerInstance, "DailyLogin", true)
    self.LayerSalary = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/DailySalary.plist",self.LayerInstance, "DailySalary", true)
    
    self.LayerLogin:setVisible(false)
    self.LayerSalary:setVisible(false)
    
    if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["DailyReward"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	
	PushOneLayer(self.LayerInstance,"","")
    
    self.currentRewardIndex = 0
    self.doubleRewardIndex = 0
    
    self:setupScrollist()
    
    local bnt_close = self.LayreReward:FindChildObjectByName("rewardCloseBtn").__UIButton__:getMenuItemSprite();
    bnt_close:registerScriptTapHandler(onCloseClick)
    
    local label_tile = self.LayreReward:FindChildObjectByName("reward_text_tile").__UILabel__;
    label_tile:setString(LanguageLocalization:GetLocalization("M_DAILYREWARD_TITLE"))
    
    addcallbackDailyRewardRsp()
end

function Dailyreward:setupScrollist()

    local scrollList = self.LayreReward:FindChildObjectByName("rewardScrollList").__UIScrollList__
	if nil == scrollList then
		return 
	end
	scrollList:clearChildrens()
    self.ListButtons = {}
    self.currentRewardIndex = 0
    self.doubleRewardIndex = 0

    
    self:insertItemToList(scrollList)
    self:insertItemToList(scrollList)
    self:insertItemToList(scrollList)
end

function Dailyreward:insertItemToList(scrollList)

    local strBin = "UI/ui_dailyreward.bin"
    local scaleFactor = TXGUI.UIManager:sharedManager():getScaleFactor()

    local normalListItemIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_LIST";
	local clickListItemIconBgName = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_LIST";
	local normal = AspriteManager:getInstance():getOneFrame(strBin,normalListItemIconBgName)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,clickListItemIconBgName)
	local button = scrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -130);
    local parentControl = button:getAnimationNode()
    
    local loginRewardTime = UserData:GetUserInfo().m_loginRewardTime
    local vipRewardTime = UserData:GetUserInfo().m_vipRewardTime
    local payRewardTime = UserData:GetUserInfo().m_payRewardTime
    local diamondRewardTime = UserData:GetUserInfo().m_buyDiamondRewardTime
    local pviRewardTime =  UserData:GetUserInfo().m_pvaiRewardTime
    local holdExp = UserData:GetUserInfo().m_palyHoldExp
    
    diamondRewardTime = 100
    holdExp = 100
    vipRewardTime = 100
    
    --print("loginRewardTime:"..loginRewardTime)
    --print("vipRewardTime:"..vipRewardTime)
    --print("payRewardTime:"..payRewardTime)
    --print("diamondRewardTime:"..diamondRewardTime)
    --print("pviRewardTime:"..pviRewardTime)
    --print("holdExp:"..holdExp)
    
    local scrollList = self.LayreReward:FindChildObjectByName("rewardScrollList").__UIScrollList__
    
    local strnormal_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
    local strsel_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_CLICKED"
    
    local strnormal_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1"
    local strsel_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_CLICKED"
    
    if self.currentRewardIndex == 0  then
    
       if payRewardTime == 0 then 
          strnormal_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
       
       if loginRewardTime == 0 then
          strnormal_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
    
    elseif self.currentRewardIndex == 2 then 
    
       if pviRewardTime == 0 then
          strnormal_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
       
       if holdExp == 0 then
          strnormal_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
       
    elseif self.currentRewardIndex == 4 then
    
       if diamondRewardTime == 0 then
          strnormal_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_l = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
       
       if vipRewardTime == 0 then
          strnormal_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
          strsel_r = "map_ui_dailyreward_FRAME_DAILYREWARD_BUTTON_FUNCTION1_GOT"
       end
    end

    
    local normalSprite = AspriteManager:getInstance():getOneFrame(strBin,strnormal_l)
    local clickedSprite = AspriteManager:getInstance():getOneFrame(strBin,strsel_l)
    local menuItem_l = CCMenuItemSprite:create(normalSprite,clickedSprite);
    local leftsubBtn = TXGUI.UIButton:new("", 0, 0, scrollList, menuItem_l, nil,-130, "");
    
    if self.currentRewardIndex == 0 then
        menuItem_l:registerScriptTapHandler(onDailypayReward)
    elseif self.currentRewardIndex == 2 then
        menuItem_l:registerScriptTapHandler(onDailypviReward)
    elseif self.currentRewardIndex == 4 then
        menuItem_l:registerScriptTapHandler(onDailydiamondReward)
    end

    normalSprite = AspriteManager:getInstance():getOneFrame(strBin,strnormal_r)
    clickedSprite = AspriteManager:getInstance():getOneFrame(strBin,strsel_r)
    local menuItem_r = CCMenuItemSprite:create(normalSprite,clickedSprite);
    local rightsubBtn = TXGUI.UIButton:new("", 0, 0, scrollList, menuItem_r, nil,-130, "");
    
    
    if self.currentRewardIndex == 0 then
        menuItem_r:registerScriptTapHandler(onDailyloginReward)
    elseif self.currentRewardIndex == 2 then
        menuItem_r:registerScriptTapHandler(onDailyholdExp)
    elseif self.currentRewardIndex == 4 then
        menuItem_r:registerScriptTapHandler(onDailyvipReward)
    end

    local parentSize = normal:getContentSize()
    local subSize = normalSprite:getContentSize()
    
    leftsubBtn:setPosition(ccp(scaleFactor*subSize.width/2-scaleFactor*parentSize.width/2, -30))
    rightsubBtn:setPosition(ccp(scaleFactor*parentSize.width/2-scaleFactor*subSize.width/2,-30))
    
    local list_uipicture = self.LayreReward:FindChildObjectByName("FRAME_DAILYREWARD_LIST_AREA").__UIPicture__;
    local frameSize = list_uipicture:getSpriteSize()
    
    local pt_l = leftsubBtn:getPosition()
    local pt_r = rightsubBtn:getPosition()
    
    leftsubBtn:setPosition(ccp(pt_l.x,  pt_l.y + scaleFactor * frameSize.height*0.4 - self.doubleRewardIndex * parentSize.height * scaleFactor))
    rightsubBtn:setPosition(ccp(pt_r.x, pt_r.y + scaleFactor * frameSize.height*0.4 - self.doubleRewardIndex * parentSize.height * scaleFactor))
    
    self.doubleRewardIndex = self.doubleRewardIndex + 1
    
    self:setButtonInfo(leftsubBtn,subSize)
    self.currentRewardIndex = self.currentRewardIndex + 1
    self:setButtonInfo(rightsubBtn,subSize)
    self.currentRewardIndex = self.currentRewardIndex + 1
    
    local listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = leftsubBtn
    
    listBtnCount = table.getn(self.ListButtons);
	self.ListButtons[listBtnCount+1] = rightsubBtn
    
end

function Dailyreward:setButtonInfo(subButton, subSize)

         local scaleFactor = TXGUI.UIManager:sharedManager():getScaleFactor()
         local node = CCNode:create()
         local strBin = "UI/ui_system_icon.bin"
         local iconsprite = AspriteManager:getInstance():getOneFrame(strBin,"map_ui_system_icon_FRAME_ICON_REWARD")
         node:addChild(iconsprite)
         node:setContentSize(subSize)
         
         iconsprite:setPosition(ccp(scaleFactor*(iconsprite:getContentSize().width*0.7-subSize.width/2), 
                                   -scaleFactor*subSize.height/2 + scaleFactor*iconsprite:getContentSize().height*0.7))
                                   
         --label 1
          local reWardName;
          if self.currentRewardIndex == 0 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6000_name");
              
          elseif self.currentRewardIndex == 1 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6001_name");
              
          elseif self.currentRewardIndex == 2 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6002_name");
              
          elseif self.currentRewardIndex == 3 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6003_name");
              
          elseif self.currentRewardIndex == 4 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6004_name");
              
          elseif self.currentRewardIndex == 5 then
              reWardName = LanguageLocalization:GetLocalization("Dailyreceive_6005_name");
          end
                   
          local label_reWardName = TXGUI.UILabelTTF:create(reWardName,KJLinXin,24,subSize,kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
          node:addChild(label_reWardName)
          local relativePoint = ccp(scaleFactor*subSize.width*0.4,-scaleFactor*subSize.height*0.12);
          label_reWardName:setPosition(relativePoint)
          label_reWardName:setAnchorPoint(ccp(0,0))
          subButton:getAnimationNode():addChild(node,10)
          
          
          --label 2
          local strGot = LanguageLocalization:GetLocalization("M_DAILYREWARD_ALREADYGOT");
          local strWaitGot = LanguageLocalization:GetLocalization("M_DAILYREWARD_WAITFORGET");
   
          local loginRewardTime = UserData:GetUserInfo().m_loginRewardTime
          local vipRewardTime = UserData:GetUserInfo().m_vipRewardTime
          local payRewardTime = UserData:GetUserInfo().m_payRewardTime
          local diamondRewardTime = UserData:GetUserInfo().m_buyDiamondRewardTime
          local pviRewardTime =  UserData:GetUserInfo().m_pvaiRewardTime
          local holdExp = UserData:GetUserInfo().m_palyHoldExp
          
          local strVal = ""
          
          if self.currentRewardIndex == 0 then
   
             if payRewardTime == 0 then
                strVal = strGot
             else
                strVal = strWaitGot
             end
                
          elseif self.currentRewardIndex == 1 then
           
             if loginRewardTime == 0 then
                strVal = strGot
             else
                strVal = strWaitGot
             end
                
          elseif self.currentRewardIndex == 2 then
          
             if pviRewardTime == 0 then
                strVal = strGot
             else
                strVal = strWaitGot
             end
                
          elseif self.currentRewardIndex == 3 then
           
             if holdExp == 0 then
                strVal = strWaitGot
             else
                strVal = strWaitGot
             end
          
          
          elseif self.currentRewardIndex == 4 then
          
             if diamondRewardTime == 0 then
                strVal = strWaitGot
             else
                strVal = strWaitGot
             end
          
          elseif self.currentRewardIndex == 5 then
          
             if vipRewardTime == 0 then
                strVal = strWaitGot
             else
                strVal = strWaitGot
             end
          end
          
          local label_lock = TXGUI.UILabelTTF:create(strVal,KJLinXin,24,subSize,kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
          node:addChild(label_lock)
          label_lock:setAnchorPoint(ccp(0,0))
          relativePoint = ccp(scaleFactor*subSize.width*0.4,scaleFactor*subSize.height*0.25);
          label_lock:setPosition(relativePoint)
end

function onDailyloginReward(nTag)
  print("onDailyloginReward")
  local loginRewardTime = UserData:GetUserInfo().m_loginRewardTime
  
  if loginRewardTime == 0 then
  else
     Dailyreward.LayerLogin:setVisible(true)
     Dailyreward.LayreReward:setVisible(false)
     Dailyreward.LayerSalary:setVisible(false)
  end
end

function onDailypayReward(nTag)
  print("onDailypayReward")
  
  local payRewardTime = UserData:GetUserInfo().m_payRewardTime
  if payRewardTime == 0 then
  else
    Dailyreward.LayerLogin:setVisible(false)
    Dailyreward.LayreReward:setVisible(false)
    Dailyreward.LayerSalary:setVisible(true)
  end
end

--Note: 竞技场奖励领取
function onDailypviReward(nTag)

  print("onDailypviReward")
  local pviRewardTime =  UserData:GetUserInfo().m_pvaiRewardTime
  if pviRewardTime == 0 then
  else
     Dailyreward.LayerLogin:setVisible(false)
     Dailyreward.LayreReward:setVisible(false)
     Dailyreward.LayerSalary:setVisible(false)
     DailyRewardPvaiLayer:CreateLayer(nil)
  end
end

function onDailyholdExp(nTag)
  print("onDailyholdExp")
end

function onDailydiamondReward(nTag)
  print("onDailydiamondReward")
end

function onDailyvipReward(nTag)
  print("onDailyvipReward")
end

function PviBackToReward()
  Dailyreward.LayerLogin:setVisible(false)
  Dailyreward.LayreReward:setVisible(true)
  Dailyreward.LayerSalary:setVisible(false)
  Dailyreward:setupScrollist()
  print("PviBackToReward")
end

function BackToDailyReWard()
  Dailyreward.LayerLogin:setVisible(false)
  Dailyreward.LayreReward:setVisible(true)
  Dailyreward.LayerSalary:setVisible(false)
end

function onCloseClick(pObj)
  if nil ~= Dailyreward.LayerInstance then
        SetChatBar(true,-1);
		RemoveOneLayer(Dailyreward.LayerInstance)
		TXGUI.UIManager:sharedManager():removeUILayout("DailyReward")
        TXGUI.UIManager:sharedManager():removeUILayout("DailyLogin")
		TXGUI.UIManager:sharedManager():removeUILayout("DailySalary")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		Dailyreward.LayerInstance:removeFromParentAndCleanup(true);
		Dailyreward.LayerInstance = nil
  end
end

----每日领取
function sendGetDailyRewardReq(key_in)
    print("sendGetDailyRewardReq ");
	local msgname="CSGetDailyRewardReq"
	CSGetDailyRewardReq = {
       key = key_in
	}
	local ret = send_message(msgname, CSGetDailyRewardReq, true)
	return ret;
end

function CSGetDailyRewardCallBackRsp(msg)

	print("CSGetDailyRewardCallBackRsp")
    print(msg.succ)
    print(msg.key)
    
    local succ = msg.succ
    local key = msg.key
    
    if succ == true then
       if key == 68 then  --每日薪金
          PviBackToReward()
          --GameApi:showMessage(LanguageLocalization:GetLocalization("M_GARDEN_NOTIME"));
       elseif key == 66 then --每日登陆
         PviBackToReward()
       end
    end
    return false
end

function addcallbackDailyRewardRsp()
	addMsgCallBack("CSGetDailyRewardRsp", CSGetDailyRewardCallBackRsp)
	return 1
end

