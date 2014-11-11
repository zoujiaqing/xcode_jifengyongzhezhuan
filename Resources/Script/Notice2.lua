
require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/DailyTask/dailyTaskDataCenter")

require("Script/protobuf")
require("Script/handleMessage")

-- CCLayer 析构的回调
local function onReceiveLayerDestructor()
    --CCLuaLog("onReceiveLayerDestructor")
    --AnnouncementCloseBtnClick(nil)
    Notice2.IsDisConnect = true
	TXGUI.UIManager:sharedManager():removeUILayout("Notice2")
    Notice2:ResetValue()
end

Notice2 = {
	LayerInstance = nil,
	LayoutEnter = nil,
    LayerScrollList = nil,
    maxWidth = 0,
    offsetY = 0,
    IsLoad = false,
    IsDisConnect = false,
}

function Notice2:ResetValue()
	--CCLuaLog("Notice2:ResetValue()")
	self.LayerInstance = nil
	self.LayoutEnter = nil
    self.LayerScrollList = nil
    self.maxWidth = 0
    self.offsetY = 0
    self.IsLoad = false
    self.IsDisConnect = false
end

function Notice2:IsLoadAlready()
   return self.IsLoad;
end

function Notice2:CreateLayer(parentLayer)

    --CCLuaLog("Notice2:CreateLayer")
    
	self:ResetValue()
    
    local mainMenuNode = GetMainMenuLayerNode()
    if mainMenuNode == nil then
       return
    end
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayoutEnter = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/Notice2.plist",self.LayerInstance, "Notice2", true);

	if parentLayer == nil then
		local tag = UITagTable["Notice2"].tag
		mainMenuNode:addChild(self.LayerInstance,75,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
        --print("success add layer as child")
	end	
    
    --PushOneLayer(self.LayerInstance,"","")
    
    self.IsLoad = true
    
    self.LayerInstance:registerDestructorScriptHandler(onReceiveLayerDestructor)
    
    self:setupCallback()
    self:setupScrollList()
    self:setupRegisterBtn()
    
    -----------发送服务器请求------------
    sendAnnouncementReq()
end

-----设置scrolllist------------
function Notice2:setupScrollList()

    self.LayerScrollList = self.LayoutEnter:FindChildObjectByName("NOTICE2_TEXT_CONTENT").__UIScrollList__
	if nil == self.LayerScrollList then
		return 
	end
    
	self.LayerScrollList:clearChildrens()
    
    local contentSize = self.LayerScrollList:getContentSize()
    local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
    
	local size = CCSizeMake(contentSize.width / scale,contentSize.height / scale)
    self.maxWidth = size.width
    
    --CCLuaLog("self.maxWidth:"..self.maxWidth)
end

-----注册btn响应函数------------
function Notice2:setupRegisterBtn()
    local btn_close = self.LayoutEnter:FindChildObjectByName("NOTICE2_BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
    btn_close:registerScriptTapHandler(onAnnouncementClose)
end

-----------插入子节点标题----------------
function Notice2:insertTitle(title)

    --CCLuaLog("insert title :"..title)
        
    local strBin = "UI/ui.bin"
    local str_name = "map_ui_FRAME_NOTICE2_FRAME_TITLE";
    local normal = AspriteManager:getInstance():getOneFrame(strBin,str_name)
    local size = normal:getContentSize()

    local label_content = TXGUI.UILabelTTF:create(title,KJLinXin,25)
    
    local node = CCNode:create()
    node:addChild(normal)
    
    local color = ccc3(255, 255, 255)
    label_content:setColor(color)
    
    normal:addChild(label_content)
    label_content:setPosition(ccp(normal:getContentSize().width/2, normal:getContentSize().height/2))

    node:setContentSize(normal:getContentSize())
    normal:setPosition(ccp(node:getContentSize().width/2, node:getContentSize().height/2))
    local base = self.LayerScrollList:AddBaseItem(node:getContentSize(),node)
end

------插入一个子节点到一个子公告------
function Notice2:insertStyleNode(r, g, b, content, fontsize)

    --CCLuaLog("insertStyleNode" .. content)
    
	local worldColor = ccc3(r,g,b)
	local node = HtmlNode:create(self.maxWidth)
	node:begin()
	node:addLabel(content,fontsize,KJLinXin,worldColor)
	node:finish()
    node:setAnchorPoint(ccp(0.5,0.5))
    local base = self.LayerScrollList:AddBaseItem(node:getContentSize(),node)
    
    local oldPos_y = node:getPositionY()
    node:setPosition((0 + self.LayerScrollList:getContentSize().width ) * 0.5,oldPos_y)
    --local size = self.LayerScrollList:getContentSize()
    --local viewSize = self.LayerScrollList:getViewSize()
    --if size.height > viewSize.height then
        --local offset = ccp(0,size.height - viewSize.height)
        --self.LayerScrollList:setStartOffset(offset)
        --print("offset x:"..offset.x.."offset y:"..offset.y)
        --self.offsetY = self.offsetY + offset.y
    --end
end

function Notice2:resetScrolllistOffset()
    local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
    --self.LayerScrollList:setStartOffset(ccp(25*scale,0))    
end

------------插入子公告分节点-----------------
function Notice2:insertCutline()
	--CCLuaLog("insertCutline")
    local strBin = "UI/ui.bin"
    local str_name = "map_ui_FRAME_NOTICE2_FRAME_CUTLINE";
	local normal = AspriteManager:getInstance():getOneFrame(strBin,str_name)
    
    local node = CCNode:create()
    node:addChild(normal)
    node:setContentSize(normal:getContentSize())
    normal:setPosition(ccp(node:getContentSize().width/2, node:getContentSize().height/2))
    local base = self.LayerScrollList:AddBaseItem(node:getContentSize(),node)
end

-----注册连接服务器的函数-------
function Notice2:setupCallback()
	addMsgCallBack("CSAnnouncementRsp", CSAnnouncementRspCallBack)
end

----------关闭按钮------------
function onAnnouncementClose(pObj)
	--CCLuaLog("onAnnouncementClose")
  AnnouncementCloseBtnClick(nil)
end

function AnnouncementCloseBtnClick(pObj)
        --SetChatBar(true,-1);
		--RemoveOneLayer(Notice2.LayerInstance)
		--TXGUI.UIManager:sharedManager():removeUILayout("Notice2")
		--CCTextureCache:sharedTextureCache():removeUnusedTextures()
		if Notice2.LayerInstance ~= nil then
			Notice2.LayerInstance:removeFromParentAndCleanup(true);
		end
		--Notice2.LayerInstance = nil
        --Notice2:ResetValue()
end

--------------------------------------------------消息发送-------------------------------
--向服务器请求公告内容
function sendAnnouncementReq()

	local msgname = "CSAnnouncementReq"
	CSAnnouncementReq = {
	}

    --CCLuaLog("sendAnnouncementReq 1")
	local ret = send_message(msgname, CSAnnouncementReq, true)
    --CCLuaLog("sendAnnouncementReq 2")
	return ret;
end

----------接收公告-----------
function CSAnnouncementRspCallBack(msg)

    if Notice2.IsDisConnect == true then
       return
    end
    
    --CCLuaLog("CSAnnouncementRspCallBack")
    for index, value in ipairs(msg.content) do
    
        local styles = value.context
        local title = value.title
        
        Notice2:insertTitle(title)
        
		local isEmptyNode = true
	    local node = HtmlNode:create(Notice2.maxWidth)
	    node:begin()

        for index, style in ipairs(styles) do
        
             local r = style.red
             local g = style.green
             local b = style.blue
             
             local content = style.text
             local fontsize = style.fontsize
   
             local worldColor = ccc3(r,g,b)
			 if fontsize ~= 0 then
				isEmptyNode = false
				node:addLabel(content,fontsize,KJLinXin,worldColor)
			 else
				node:finish()
				if node:getContentSize().width ~= 0 and node:getContentSize().height ~= 0 then
					node:setAnchorPoint(ccp(0.5,0.5))
					local base = Notice2.LayerScrollList:AddBaseItem(node:getContentSize(),node)
					local oldPos_y = node:getPositionY()
					node:setPosition((0 + Notice2.LayerScrollList:getContentSize().width ) * 0.5,oldPos_y)
				end
				isEmptyNode = true
				node = HtmlNode:create(Notice2.maxWidth)
			    node:begin()
			 end
        end
        
        node:finish()
		if not isEmptyNode then
			node:setAnchorPoint(ccp(0.5,0.5))
			local base = Notice2.LayerScrollList:AddBaseItem(node:getContentSize(),node)
			local oldPos_y = node:getPositionY()
			node:setPosition((0 + Notice2.LayerScrollList:getContentSize().width ) * 0.5,oldPos_y)
        end
        --local size = Notice2.LayerScrollList:getContentSize()
        --local viewSize = Notice2.LayerScrollList:getViewSize()
        --
        --if size.height > viewSize.height then
           --local offset = ccp(0,size.height - viewSize.height)
           --Notice2.LayerScrollList:setStartOffset(offset)
        --end
        
        
        ----插入子节点分割线------
        Notice2:insertCutline()
	end
    
     Notice2:resetScrolllistOffset()
     
    return true
end

--CCLuaLog("load Notice2 success")

