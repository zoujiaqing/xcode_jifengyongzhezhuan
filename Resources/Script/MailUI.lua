require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/DailyTask/dailyTaskDataCenter")

require("Script/protobuf")
require("Script/handleMessage")

MailUI = {
	LayerInstance = nil,
	LayoutEnter = nil,
    LayerScrollList = nil,
    LayerScrollList_Txt = nil,
    ListButtons = {},
	ListButtonsMenuItems = {},
    ListMailId = {},
    ListTipId = {},
    ListTipBtn = {},
    ListMailHeadIconTip = {},
    ListMailHeadIcon = {},
    ListMailHasTip = {},
    ListMailHasRead = {},
    ListMailTipGet = {},
	SelectItemIndex = 0,
}

function MailUI:ResetValue()
	self.LayerInstance = nil
	self.LayoutEnter = nil
    self.LayerScrollList = nil
    self.LayerScrollList_Txt = nil
    self.ListButtons = {}
	self.ListButtonsMenuItems = {}
    self.ListMailId = {}
    self.ListTipId = {}
    self.ListTipBtn = {}
    self.ListMailHeadIconTip = {}
    self.ListMailHeadIcon = {}
    self.ListMailHasTip = {}
    self.ListMailHasRead = {}
    self.ListMailTipGet = {}
	self.SelectItemIndex = 0
end

function MailUI:CreateLayer(parentLayer)

    print("MailUI:CreateLayer")
	self:ResetValue()
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayoutEnter = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/MailUI.plist",self.LayerInstance, "MailUI", true);

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["MailUI"].tag
		mainMenuNode:addChild(self.LayerInstance,0,tag)
		InitMoneyConfigLayer(self.LayerInstance)
	end	
	
    PushOneLayer(self.LayerInstance,"","")
    
    --注册响应协议
    self:setupCallback()
    
    --注册btn响应函数
    self:setupRegisterBtn()
    
    --设置scrolllist
    self:setupScrollList()
    
    self:setupScrollist_txt()
    
    self:hideMailInfo(false)
end

-----注册btn响应函数------------
function MailUI:setupRegisterBtn()
    local btn_get = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_1").__UIButton__:getMenuItemSprite();
    local btn_del = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_3").__UIButton__:getMenuItemSprite();
    local btn_close = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_CLOSE").__UIButton__:getMenuItemSprite();

    btn_get:registerScriptTapHandler(onMailGet)
    btn_del:registerScriptTapHandler(onMailDel)
    btn_close:registerScriptTapHandler(onMailClose)
    
    local label_get = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_BUTTON1").__UILabel__;
    local label_del = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_BUTTON3").__UILabel__;
end

-----设置scrolllist------------
function MailUI:setupScrollList()
    
    self.LayerScrollList = self.LayoutEnter:FindChildObjectByName("mail_scrolllist").__UIScrollList__
	if nil == self.LayerScrollList then
		return 
	end

	self.LayerScrollList:clearChildrens()
    self.ListButtons = {}
	self.ListButtonsMenuItems = {}
    self.ListMailId = {}
    self.ListTipId = {}
    self.ListTipBtn = {}
    self.ListMailHeadIconTip = {}
    self.ListMailHeadIcon = {}
    self.ListMailHasTip = {}
    self.ListMailHasRead = {}
    self.ListMailTipGet = {}
    --获取邮件列表
    sendAskMailList()
end

function MailUI:setupScrollist_txt()
    self.LayerScrollList_txt = self.LayoutEnter:FindChildObjectByName("mail_scrolllist_txt").__UIScrollList__
	if nil == self.LayerScrollList_txt then
		return 
	end
	self.LayerScrollList_txt:clearChildrens()
end

---------生成一个邮件到scrolllist-----
function MailUI:insertItemToList(title, mailid, index, hasread, hasattach)

    local strBin = "UI/ui2.bin"
    local bgListItemIconBgName_normal = "map_ui2_FRAME_MAIL_BUTTON_MAIL1";
    local bgListItemIconBgName_clicked = "map_ui2_FRAME_MAIL_BUTTON_MAIL1_SELECTED";	
	local normal = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName_normal)
	local clicked = AspriteManager:getInstance():getOneFrame(strBin,bgListItemIconBgName_clicked)
    
    local button = self.LayerScrollList:AddNewItem(normal, clicked, "", "", ccc3(255,255,255), -130);
    local parentControl = button:getAnimationNode()
    
    --设置邮件Icon
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);
    local mailIconName = "map_ui2_FRAME_MAIL_ICON_MAIL1"
	AspriteManager:getInstance():getFramesRelativePos(strBin,bgListItemIconBgName_normal,mailIconName,anchorPoint,relativePoint);
    
	--self:ScaleByScreen(relativePoint)
    
    local strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_NEW"
    if hasread then
        strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_OPEN"
    end
    local mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
    mailIconSprite:setPosition(relativePoint)
    parentControl:addChild(mailIconSprite,100)
    
    --设置邮件标题
    local mailLabelTxtName = "map_ui2_FRAME_MAIL_TEXT_MAIL1"
	AspriteManager:getInstance():getFramesRelativePos(strBin,bgListItemIconBgName_normal,mailLabelTxtName,anchorPoint,relativePoint);
    local nameLabel = TXGUI.UILabelTTF:create(title, KJLinXin, 20);
    nameLabel:setPosition(relativePoint)
    parentControl:addChild(nameLabel,100)
    
    --存储button
    self.ListButtons[index] = button
	local btnMenuItem = button:getMenuItemSprite()
	self.ListButtonsMenuItems[index] = btnMenuItem
	btnMenuItem:setTag(index)
	btnMenuItem:registerScriptTapHandler(onMailLayerOneItemClickEvent)
    
    --存储mailid
    self.ListMailId[index] = mailid
    print("mailid:"..self.ListMailId[index])
    
    --如果有附件，显示一个附件图标 
    local iconSprite = nil
    if hasattach then
       iconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", "map_ui_system_icon_FRAME_ICON_EXCALMATORY");
       mailIconSprite:addChild(iconSprite,0,10)
       iconSprite:setPosition(ccp(mailIconSprite:getContentSize().width - iconSprite:getContentSize().width*0.4,              mailIconSprite:getContentSize().height - iconSprite:getContentSize().height*0.5))
    end
    self.ListMailHeadIconTip[index] = iconSprite
    self.ListMailHeadIcon[index] = mailIconSprite
    self.ListMailHasRead[index] = hasread
end

function MailUI:ScaleByScreen(relativePoint)
	relativePoint.x = relativePoint.x * TXGUI.UIManager:sharedManager():getScaleFactor()
end

---------生成一个附件-----------
function MailUI:setupTipIconButton(id, item_id, item_num)

   local MAIL_ICON_ATTACHMENT = nil
   if id == 1 then
      MAIL_ICON_ATTACHMENT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT1").__UIPicture__;
   elseif id == 2 then
      MAIL_ICON_ATTACHMENT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT2").__UIPicture__;
   elseif id == 3 then
      MAIL_ICON_ATTACHMENT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT3").__UIPicture__;
   end
   
   local spriteSize = MAIL_ICON_ATTACHMENT:getCurrentNode():getContentSize()
   local boxLockSpriteOn = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_ALCHEMY_ICON_CLICKED")
   local boxLockSpriteOff = AspriteManager:getInstance():getOneFrame("UI/ui.bin", "map_ui_FRAME_ALCHEMY_ICON_CLICKED")   

   local pEmptyItem = CCSprite:create()
   pEmptyItem:setContentSize(spriteSize)
   
   boxBtn = IconButton:new(pEmptyItem,boxLockSpriteOff,nil)
   
   local btnMenuItem = boxBtn:getMenuItem()
   btnMenuItem:setTag(id)
   btnMenuItem:registerScriptTapHandler(onPressTipBoxButton)
   
   MAIL_ICON_ATTACHMENT:getCurrentNode():addChild(boxBtn)
   boxBtn:setPosition(ccp(MAIL_ICON_ATTACHMENT:getCurrentNode():getContentSize().width/2, 
                          MAIL_ICON_ATTACHMENT:getCurrentNode():getContentSize().height/2))
                          
   local sprite = ItemManager:Get():getIconSpriteById(item_id)
   boxBtn:SetNormalIcon(sprite);
   local num =  MainMenuLayer:GetNumByFormat(item_num)
   local numLabel = TXGUI.UILabelTTF:create(""..num, KJLinXin, 20);
   MAIL_ICON_ATTACHMENT:getCurrentNode():addChild(numLabel,100)
   numLabel:setPosition(ccp(spriteSize.width - numLabel:getContentSize().width/2,
                          numLabel:getContentSize().height/2))

   self.ListTipId[id] = item_id
   self.ListTipBtn[id] = boxBtn
end

---------------显示某个邮件的信息，from:发件人，content:发信内容  hasget:是否已获取
function MailUI:addMailContent(title,from,content,from_tm)
    local label_title = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILTITLE").__UILabel__;
    local label_from_tm = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILSENDTIME").__UILabel__;
    local label_from = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILSENDER2").__UILabel__;
    
    local label_content2 = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILCONTENT").__UILabel__;
    label_content2:setVisible(false)

    label_title:setString(title)
    label_from_tm:setString(from_tm)
    label_from:setString(from)
    
    local labelttf = label_content2:getLabelTTF()
    local size = labelttf:getContentSize()
    
    local ListItemNode = CCNode:create();
    local labelSize = CCSizeMake(size.width,0)
    local label_content = TXGUI.UILabelTTF:create(content,KJLinXin,24,labelSize,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)
    
	size = label_content:getContentSize()
    ListItemNode:addChild(label_content)
    ListItemNode:setContentSize(size)
    label_content:setPosition(ccp(ListItemNode:getContentSize().width/2, ListItemNode:getContentSize().height/2))
    
    --self.LayerScrollList_txt:clearChildrens()
    local parentControl = self.LayerScrollList_txt:AddBaseItem(size,ListItemNode);
end

function MailUI:showMailContent(title,from,content,from_tm)
         self.LayerScrollList_txt:clearChildrens()
         while( content ~= "")do
	          local x=string.find(content,"\n")
              if x == nil then
                 print("x == nil")
                 self:addMailContent(title,from,content,from_tm)
                 return
              end
	          local sub_content=string.sub(content,1,x-1)              
              self:addMailContent(title,from,sub_content,from_tm)
	          local temp_content=string.sub(content,x+1,string.len(content))
	          content=temp_content
         end         
end

----------恢复所以tips的最初状态------------------
function MailUI:recoveryAllTips(visible)
        local MAIL_ICON_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT1").__UIPicture__;
        local MAIL_ICON_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT2").__UIPicture__;
        local MAIL_ICON_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT3").__UIPicture__;
        local MAIL_ICON_ALREADYGOT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ALREADYGOT").__UIPicture__;

        MAIL_ICON_ATTACHMENT1:getCurrentNode():removeAllChildrenWithCleanup(true)
        MAIL_ICON_ATTACHMENT2:getCurrentNode():removeAllChildrenWithCleanup(true)
        MAIL_ICON_ATTACHMENT3:getCurrentNode():removeAllChildrenWithCleanup(true)
        MAIL_ICON_ALREADYGOT:getCurrentNode():removeAllChildrenWithCleanup(true)
        
        local MAIL_FRAME_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT1").__UIPicture__;
        local MAIL_FRAME_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT2").__UIPicture__;
        local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
          
        MAIL_FRAME_ATTACHMENT1:setVisible(visible)
        MAIL_FRAME_ATTACHMENT2:setVisible(visible)
        MAIL_FRAME_ATTACHMENT3:setVisible(visible)
        
        local btn_get = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_1").__UIButton__:getMenuItemSprite();
        local label_get = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_BUTTON1").__UILabel__;
        local label_attachment = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_ATTACHMENT").__UILabel__;
        
        btn_get:setVisible(visible)
        label_get:setVisible(visible)
        label_attachment:setVisible(visible)
end

-----设置tip信息
function MailUI:showTip(items, hasget)
    local count = table.getn(items)
    print("count:"..count)
        
    if count == 0 then
       self:recoveryAllTips(false)
       return
    end
    
    self:recoveryAllTips(true)
    
    if count > 3 then
        local MAIL_FRAME_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT1").__UIPicture__;
        local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
        MAIL_FRAME_ATTACHMENT1:setVisible(false)
        MAIL_FRAME_ATTACHMENT3:setVisible(false)
        self:setupTipIconButton(2, 411000, 1)
    else
    
       for index, v in ipairs(items) do
           print("\t:showTip"..v.id.." "..v.num)
           self:setupTipIconButton(index, v.id, v.num)
	   end
       
       if count == 0 then
       
          local MAIL_FRAME_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT1").__UIPicture__;
          local MAIL_FRAME_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT2").__UIPicture__;
          local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
          
          MAIL_FRAME_ATTACHMENT1:setVisible(false)
          MAIL_FRAME_ATTACHMENT2:setVisible(false)
          MAIL_FRAME_ATTACHMENT3:setVisible(false)
          
       elseif count == 1 then
       
          local MAIL_FRAME_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT2").__UIPicture__;
          local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
          
          MAIL_FRAME_ATTACHMENT2:setVisible(false)
          MAIL_FRAME_ATTACHMENT3:setVisible(false)
          
       elseif count == 2 then
          local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
          MAIL_FRAME_ATTACHMENT3:setVisible(false)
       end
       
    end
    self:setGetBtnUnEable(hasget)
end

------设者领取按钮--------------
function MailUI:setGetBtnUnEable(val)

  if val == true then
  
      local mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", "map_ui_system_icon_FRAME_ICON_ALREADYGOT")
      local MAIL_ICON_ALREADYGOT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ALREADYGOT").__UIPicture__;
  
      MAIL_ICON_ALREADYGOT:getCurrentNode():addChild(mailIconSprite)
      mailIconSprite:setPosition(ccp(MAIL_ICON_ALREADYGOT:getCurrentNode():getContentSize().width/2,MAIL_ICON_ALREADYGOT:getCurrentNode():getContentSize().height/2))
  
      local btn_get = MailUI.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_1").__UIButton__;
      btn_get:setEnabled(false)
      --local normalsprite = AspriteManager:getInstance():getOneFrame("UI/ui2.bin", "map_ui2_FRAME_MAIL_BUTTON_1_CANTCLICK")
      
  else
     local MAIL_ICON_ALREADYGOT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ALREADYGOT").__UIPicture__;
     MAIL_ICON_ALREADYGOT:getCurrentNode():removeAllChildrenWithCleanup(true);
     local btn_get = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_1").__UIButton__;
     btn_get:setEnabled(true)
  end
end

------如果邮件为空，则隐藏邮件内容----------
function MailUI:hideMailInfo(visible)

      local MAIL_FRAME_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT1").__UIPicture__;
      local MAIL_FRAME_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT2").__UIPicture__;
      local MAIL_FRAME_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_FRAME_ATTACHMENT3").__UIPicture__;
      
      local btn_get = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_1").__UIButton__:getMenuItemSprite();
      local btn_del = self.LayoutEnter:FindChildObjectByName("MAIL_BUTTON_3").__UIButton__:getMenuItemSprite();

      local label_get = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_BUTTON1").__UILabel__;
      local label_del = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_BUTTON3").__UILabel__;
      
      local label_mailsendtime = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILSENDTIME").__UILabel__;
      local label_mailtitle = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILTITLE").__UILabel__;
      local label_mailsender = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILSENDER").__UILabel__;
      local label_mailsender2 = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILSENDER2").__UILabel__;
      local label_mailcontent = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_MAILCONTENT").__UILabel__;
      local label_attachment = self.LayoutEnter:FindChildObjectByName("MAIL_TEXT_ATTACHMENT").__UILabel__;
      
      MAIL_FRAME_ATTACHMENT1:setVisible(visible)
      MAIL_FRAME_ATTACHMENT2:setVisible(visible)
      MAIL_FRAME_ATTACHMENT3:setVisible(visible)
      
      btn_get:setVisible(visible)
      btn_del:setVisible(visible)
      
      label_get:setVisible(visible)
      label_del:setVisible(visible)
      
      label_mailsendtime:setVisible(visible)
      label_mailtitle:setVisible(visible)
      label_mailsender:setVisible(visible)
      label_mailsender2:setVisible(visible)
      label_mailcontent:setVisible(visible)
      label_attachment:setVisible(visible)
      
      local MAIL_ICON_ATTACHMENT1 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT1").__UIPicture__;
      local MAIL_ICON_ATTACHMENT2 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT2").__UIPicture__;
      local MAIL_ICON_ATTACHMENT3 = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ATTACHMENT3").__UIPicture__;
        
      MAIL_ICON_ATTACHMENT1:getCurrentNode():removeAllChildrenWithCleanup(true);
      MAIL_ICON_ATTACHMENT2:getCurrentNode():removeAllChildrenWithCleanup(true);
      MAIL_ICON_ATTACHMENT3:getCurrentNode():removeAllChildrenWithCleanup(true);
      
      local MAIL_ICON_ALREADYGOT = self.LayoutEnter:FindChildObjectByName("MAIL_ICON_ALREADYGOT").__UIPicture__;
      MAIL_ICON_ALREADYGOT:setVisible(visible)
      
      self:setupScrollist_txt()
      print("hide done")
end

--------显示msg邮件信息--------
function MailUI:showoneMailmsg(mail)

    local mailid = mail.mailid
    local title = mail.title
    local from = mail.from
    local content = mail.content
    local hasget = mail.hasget
    local from_tm = mail.from_tm
    
    print("mailid:"..mailid)
    print("title:"..title)
    print("from:"..from)
    print("content:"..content)
    print("from_tm:"..from_tm)
    print(hasget)

    self.ListMailTipGet[self.SelectItemIndex] = hasget

    local items = mail.items
    print(type(items))
    
    local count = table.getn(items)
    print("count:"..count)

    for index, v in ipairs(items) do
       print("\t:"..v.id.." "..v.num)
	end

    self:showMailContent(title,from,content,from_tm)
    self:showTip(items,hasget)

    if count > 0 then
       self.ListMailHasTip[self.SelectItemIndex] = true
    else
       self.ListMailHasTip[self.SelectItemIndex] = false
    end
end

--------------------------------------邮件操作------------------------------
--点击邮件某个列表
function onMailLayerOneItemClickEvent(nTag)
	print("onMailLayerOneItemClickEvent:" .. nTag)
	local button = MailUI.ListButtons[nTag]
    button:getMenuItemSprite():selected();
    
    local length = table.getn(MailUI.ListButtons)
    for i =1,length do
        if i ~= nTag then
	       local btn = MailUI.ListButtons[i]
           if btn ~= nil then
              btn:getMenuItemSprite():unselected();
           end
        end
	end
    
    local mailid = MailUI.ListMailId[nTag];
    if nTag ~= MailUI.SelectItemIndex then
	    MailUI.SelectItemIndex = nTag
        sendMail(mailid)
    end
    
    if MailUI.ListMailHasRead[nTag] == true then
       return
    end
    MailUI.ListMailHasRead[nTag] = true

    print("--OPEN MAIL ICON")
    --OPEN MAIL ICON
    local mailIconSprite = MailUI.ListMailHeadIcon[MailUI.SelectItemIndex]
    local iconsprite = MailUI.ListMailHeadIconTip[MailUI.SelectItemIndex]
    if mailIconSprite then
       local parentControl = mailIconSprite:getParent()
       mailIconSprite:removeFromParentAndCleanup(true)
       
       local strBin = "UI/ui2.bin"
       local bgListItemIconBgName_normal = "map_ui2_FRAME_MAIL_BUTTON_MAIL1";   
           
       --设置邮件Icon
	   local relativePoint = CCPoint(0,0);
	   local anchorPoint = CCPoint(0.5,0.5);
       local mailIconName = "map_ui2_FRAME_MAIL_ICON_MAIL1"
	   AspriteManager:getInstance():getFramesRelativePos(strBin,bgListItemIconBgName_normal,mailIconName,anchorPoint,relativePoint);
       local strmailIcon = "map_ui_system_icon_FRAME_ICON_MAIL_OPEN"
       
       mailIconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", strmailIcon)
       mailIconSprite:setPosition(relativePoint)
       parentControl:addChild(mailIconSprite,100)
       
       MailUI.ListMailHeadIcon[MailUI.SelectItemIndex] = mailIconSprite

       if iconsprite then
          iconSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin", "map_ui_system_icon_FRAME_ICON_EXCALMATORY");
          mailIconSprite:addChild(iconSprite,0,10)
          iconSprite:setPosition(ccp(mailIconSprite:getContentSize().width - iconSprite:getContentSize().width*0.4,              
                                     mailIconSprite:getContentSize().height - iconSprite:getContentSize().height*0.5))
                                     
          MailUI.ListMailHeadIconTip[MailUI.SelectItemIndex] = iconSprite
       end
    end
end

--领取邮件
function onMailGet(pObj)
  print("onMailGet:"..MailUI.SelectItemIndex)

  local mailid = MailUI.ListMailId[MailUI.SelectItemIndex]
  sendGetAttach(mailid)

  local IconSprite = MailUI.ListMailHeadIconTip[MailUI.SelectItemIndex]
  if IconSprite ~= nil then
     IconSprite:removeFromParentAndCleanup(true)
     MailUI.ListMailHeadIconTip[MailUI.SelectItemIndex] = nil
     print("clean tip")
  end
end

--删除邮件
function onMailDel(pObj)
  print("onMailDel")
  local attach = MailUI.ListMailHasTip[MailUI.SelectItemIndex]
  
  local tipget = MailUI.ListMailTipGet[MailUI.SelectItemIndex]

  local strval = ""
  if attach == true then
     if tipget == true then
        strval = "M_MESSAGE_DELETED_TIPS_3"
     else
        strval = "M_MESSAGE_DELETED_TIPS_2"
     end
  else
     strval = "M_MESSAGE_DELETED_TIPS_1"
  end
  print(strval)
  MessageBox:Show(LanguageLocalization:GetLocalization(strval),DelMail_Ok,nil,MB_YESNO,ccc3(255,255,255),26)
end

function DelMail_Ok(pObj)
	sendDelMail(MailUI.ListMailId[MailUI.SelectItemIndex])
end

--tip按钮
function onPressTipBoxButton(nTag)
   print("onPressTipBoxButton:"..nTag)
   local id = nTag
   local item_id = MailUI.ListTipId[id]
   local iconbtn = MailUI.ListTipBtn[id]
   local menu = iconbtn:getMenuNode()
   local pos = menu:getTouchPoint()
   ItemManager:Get():showItemTipsById(item_id, pos)
end

function onMailClose(pObj)
  print("onMailClose")
  MailUICloseBtnClick(nil)
end

function MailUICloseBtnClick(pObj)	

    local var = false
    local length = table.getn(MailUI.ListMailHasRead)
    for i =1,length do
        local val = MailUI.ListMailHasRead[i]
        if val == false then
           var = true
        end
	end

	if nil ~= MailUI.LayerInstance then
    local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if nil ~= topLayout then
		local mailButton = topLayout:FindChildObjectByName("mailBtn").__UIButton__
		if nil ~= mailButton then
            print(var)
            if var == false then
               mailButton:hideTipIcon()
               mailButton:setVisible(false)
               
               local mainLayer = MainMenuLayer:getMainMenuLayer()
	           if nil ~= mainLayer then
		          mainLayer:resetMailUIMsg()
               end
            end
		end
	end

        SetChatBar(true,-1);
		RemoveOneLayer(MailUI.LayerInstance)
		TXGUI.UIManager:sharedManager():removeUILayout("MailUI")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		MailUI.LayerInstance:removeFromParentAndCleanup(true);
		MailUI.LayerInstance = nil
        MailUI:ResetValue()
	end
end

function MailUI:setupCallback()

    --获取消息列表
	addMsgCallBack("CSMailListRsp",   CSMailListRspCallBack)
    
    --删除邮件
    addMsgCallBack("CSDelMailRsp",    CSDelMailCallBack)
    
    --领取邮件
    addMsgCallBack("CSGetAttachRsp",  CSGetAttachCallBack)
    
    --获取某个邮件
    addMsgCallBack("CSMailRsp",       CSMailCallBack)
end

--------------------------------------------------消息发送-------------------------------
--发送请求邮件列表
function sendAskMailList()
    print("sendAskMailList")
	local msgname = "CSMailListReq"
	CSMailListReq = {
	}
	local ret = send_message(msgname, CSMailListReq, true)
	PushOneWaitingLayer("CSMailListReq")
	return ret;
end

--响应请求邮件列表
function CSMailListRspCallBack(msg)
	print("CSMailListRspCallBack")
    local listcount = table.getn(msg.maillist)
    if listcount == 0 then
        MailUI:hideMailInfo(false)
        return
    end
    
    MailUI:hideMailInfo(true)

    for index,v in ipairs(msg.maillist) do
		print("\t:"..v.mailid.." "..v.title)
        MailUI:insertItemToList(v.title, v.mailid, index, v.hasread, v.hasattach)
        MailUI.ListMailTipGet[index] = false
	end
    
    MailUI.SelectItemIndex = 1
    MailUI.ListMailHasRead[1] = true
    MailUI:showoneMailmsg(msg.mail)
    
    local button = MailUI.ListButtons[1]
    if button ~= nil then
       button:getMenuItemSprite():selected();
    end
    return true
end

--发送删除邮件
function sendDelMail(in_mailid)
    print("sendDelMail:"..in_mailid)
	local msgname="CSDelMailReq"
	CSDelMailReq = {
       mailid = in_mailid
	}
	local ret = send_message(msgname, CSDelMailReq, true)
	PushOneWaitingLayer("CSDelMailReq")
	return ret;
end

--响应删除邮件
function CSDelMailCallBack(msg)
     print("CSDelMailCallBack")
     local rsp = msg.rsp
     if rsp == true then
        MailUI:setupScrollList()
     end
     print(rsp)
end

--领取邮件
function sendGetAttach(in_mailid)
    print("sendGetAttack:"..in_mailid)
    local msgname="CSGetAttachReq"
	CSGetAttachReq = {
       mailid = in_mailid
	}
	local ret = send_message(msgname, CSGetAttachReq, true)
	PushOneWaitingLayer("CSGetAttachReq")
	return ret;
end

--响应领取邮件
function CSGetAttachCallBack(msg)
   print("CSGetAttackCallBack")
   print(rsp)
   
   local rsp = msg.rsp
   if rsp == 0 then
      MailUI:setGetBtnUnEable(true)
      MailUI.ListMailTipGet[MailUI.SelectItemIndex] = true
      MessageBox:Show(LanguageLocalization:GetLocalization("M_MESSAGE_DELETED_TIPS_3"),DelMail_Ok,nil,MB_YESNO,ccc3(255,255,255),26)
   end
end

--发送获取某个邮件
function sendMail(in_mailid)
    print("sendMail:"..in_mailid)
    local msgname="CSMailReq"
	CSMailReq = {
       mailid = in_mailid
	}
	local ret = send_message(msgname, CSMailReq, true)
	PushOneWaitingLayer("CSMailReq")
	return ret;
end

--响应获取某个邮件
function CSMailCallBack(msg)
   print("CSMailCallBack")
   MailUI:showoneMailmsg(msg)
   return true
end

function mailUI_update()
    local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if nil ~= topLayout then
		local mailButton = topLayout:FindChildObjectByName("mailBtn").__UIButton__
		if nil ~= mailButton then
            mailButton:showTipIcon()
		end
	end
end

--响应邮件tip提示
function CSMailButtonTipCallback()
   print("CSMailButtonTipCallback")
   mailUI_update()
end

addMsgCallBack("CSNewMailRsp", CSMailButtonTipCallback)
print("load mailUI success")

