
require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/DailyTask/dailyTaskDataCenter")

require("Script/protobuf")
require("Script/handleMessage")

Notice1 = {
	LayerInstance = nil,
	LayoutEnter = nil,
    LayerScrollList = nil,
    maxWidth = 0,
    offsetY = 0,
}

function Notice1:ResetValue()
	self.LayerInstance = nil
	self.LayoutEnter = nil
    self.LayerScrollList = nil
    self.maxWidth = 0
    self.offsetY = 0
end

function Notice1:CreateLayer(parentLayer)

    print("Notice1:CreateLayer")
    
	self:ResetValue()
    
	if parentLayer == nil then
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	self.LayoutEnter = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/Notice1.plist",self.LayerInstance, "Notice1", true);

	--if parentLayer == nil then
	--	local mainMenuNode = GetMainMenuLayerNode()
	--	local tag = UITagTable["Notice1"].tag
	--	mainMenuNode:addChild(self.LayerInstance,0,tag)
	--	InitMoneyConfigLayer(self.LayerInstance)
	--end
    --PushOneLayer(self.LayerInstance,"","")

    self:setupCallback()
    self:setupScrollList()
    self:setupRegisterBtn()
    -----------发送服务器请求------------
end

-----注册btn响应函数------------
function Notice1:setupRegisterBtn()

    local btn_close = self.LayoutEnter:FindChildObjectByName("NOTICE1_BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
    btn_close:registerScriptTapHandler(onAnnouncementClose)
    
end

-----设置scrolllist------------
function Notice1:setupScrollList()

    self.LayerScrollList = self.LayoutEnter:FindChildObjectByName("NOTICE1_TEXT_CONTENT").__UIScrollList__
	if nil == self.LayerScrollList then
		return 
	end
    
	self.LayerScrollList:clearChildrens()
    
    local contentSize = self.LayerScrollList:getContentSize()
    local scale =  TXGUI.UIManager:sharedManager():getScaleFactor()
    
	local size = CCSizeMake(contentSize.width / scale,contentSize.height / scale)
    self.maxWidth = size.width
    
    print("self.maxWidth:"..self.maxWidth)
end

-----------插入子节点标题----------------
function Notice1:insertTitle(title)

    print("insert title :"..title)
    
    local strBin = "UI/ui.bin"
    local str_name = "map_ui_FRAME_NOTICE2_FRAME_TITLE";
    local normal = AspriteManager:getInstance():getOneFrame(strBin,str_name)
    local size = normal:getContentSize()

    local label_content = TXGUI.UILabelTTF:create(title,KJLinXin,20)
    
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
function Notice1:insertStyleNode(r, g, b, content, fontsize)

    
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

-----注册连接服务器的函数-------
function Notice1:setupCallback()
	addMsgCallBack("CSLoginAnnouncementRsp", CSLoginAnnouncementRspCallBack)
end

----------关闭按钮------------
function onNotice1Close(pObj)
  Notice1CloseBtnClick(nil)
end

function Notice1CloseBtnClick(pObj)

        SetChatBar(true,-1);
        
		RemoveOneLayer(Notice1.LayerInstance)
        
		TXGUI.UIManager:sharedManager():removeUILayout("Notice1")
        
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
        
		Notice1.LayerInstance:removeFromParentAndCleanup(true);
        
		Notice1.LayerInstance = nil
        
        Notice1:ResetValue()
end

--------------------------------------------------消息发送-------------------------------
--向服务器请求公告内容
function sendLoginAnnouncementReq()

    print("sendLoginAnnouncementReq")
    
	local msgname = "CSLoginAnnouncementReq"
    
	CSLoginAnnouncementReq = {
	}
    
	local ret = send_message(msgname, CSLoginAnnouncementReq, true)
    
	return ret;
end

----------接收公告-----------
function CSLoginAnnouncementRspCallBack(msg)

    print("CSLoginAnnouncementRspCallBack")
    
    for index, value in ipairs(msg.content) do
    
        local styles = value.context
        local title = value.title
        
        Notice2:insertTitle(title)
        
        for x, style in ipairs(styles) do
        
             local r = style.red
             local g = style.green
             local b = style.blue
             
             local content = style.text
             local fontsize = style.fontsize
             
             print("r:"..r.."g:"..g.."b:"..b)
             print("content:"..content)
             print("fontsize:"..fontsize)
             
             -----插入一个style---
             Notice2:insertStyleNode(r,g,b,content,fontsize)
        end
	end
    
    return true
end

print("load Notice1 success")

