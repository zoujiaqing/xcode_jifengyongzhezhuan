-- 称号页面
require("Script/Talent/TalentMgr")

TitleLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
    attrLayout = nil,

	notificationFunc = nil,	

	sheetIndex = 1,
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if TitleLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(TitleLayer.notificationFunc);
	end

	TitleLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("TitleLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= TitleLayer.uiLayerInstance then
			if G_CurLayerInstance ~= TitleLayer.uiLayerInstance then
				TitleLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function TitleLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["TitleLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/TitleLayer.plist",self.uiLayerInstance, "TitleLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function TitleLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
    self.attrLayout = nil
	self.notificationFunc = nil
	sheetIndex = 1
end

-- 删除UI
function TitleLayer:destroyed()
	CCLuaLog("--- TitleLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
    --SetChatBar(true,-1)
end

-- 刷新详细属性页面
local function refreshAttrDetailLayer(titleId)
    
    if titleId == 0 then
        TitleLayer.attrLayout:FindChildObjectByName("detailTitlePic").__UIPicture__:setVisible(false)
    else
        local titleIcon = GetTitleIconById(titleId)    
        if titleIcon then 
            TitleLayer.attrLayout:FindChildObjectByName("detailTitlePic").__UIPicture__:setSprite(titleIcon)
        end
    end
    
    local attrTable
    local titleStr
    if titleId == 0 then
        titleStr = GetLuaLocalization("M_TITLE_ALLATTRIBUTE")
        attrTable = GetTitleTotalAttrTable()
    else
        titleStr = GetLuaLocalization("M_TITLE_ADDATTRIBUTE")
        attrTable = GetAttrTableByTitleId(titleId)
    end

    TitleLayer.attrLayout:FindChildObjectByName("detailText").__UILabel__:setString(titleStr)

    local attrStrTable = {}
    for i = 1, 6 do
        if attrTable[i] ~= 0 or titleId == 0 then
            local attrStr = GetTalentAttrStr(i, attrTable[i])
            attrStrTable[#attrStrTable + 1] = attrStr
         end
    end

    for i = 1, 6 do
        local contentStr
        if attrStrTable[i] ~= nil then
            contentStr = attrStrTable[i]
        else
            contentStr = ""
        end
        TitleLayer.attrLayout:FindChildObjectByName("detailAttr"..i).__UILabel__:setString(contentStr)
    end
end

--关闭页面
local function onPressCloseBtn(tag)
    CCLuaLog("--- onPressCloseBtn ---")
    --RemoveOneLayer(TitleLayer.uiLayerInstance)
	TitleLayer:destroyed()
end

-- 隐藏详细属性
local function onPressHideAttrLayer(tag)
    CCLuaLog("--- onPressHideAttrLayer ---")
    if TitleLayer.attrLayout then
        TitleLayer.attrLayout:setVisible(false)
    end
end

local curTitleList = {}
local needTitleList = {}
local needTitleContent = {}
local costIdList = {}
local costCountList = {}

local learnBtns = {}
local grayPics = {}
local learnLabels = {}
local costTexts = {}
local costIcons = {}
local costNums = {}

-- 其他称号相关数据
local geniusTitleList = {}
local otherTitleList = {}

-- 显示详细属性
local function onPressShowAttrLayer(tag)
    CCLuaLog("--- onPressShowAttrLayer ---")
    if TitleLayer.attrLayout then
        refreshAttrDetailLayer(curTitleList[tag].titleId)
        TitleLayer.attrLayout:setVisible(true)
    end
end

local function onPressShowAllAttr(tag)
    CCLuaLog("--- onPressShowAttrLayer ---")
    if TitleLayer.attrLayout then
        refreshAttrDetailLayer(0)
        TitleLayer.attrLayout:setVisible(true)
    end
end

-- 点击装备或者学习按钮，根据对应称号状态判断
local function onPressLearnBtn(tag)
    CCLuaLog("--- onPressLearnBtn ---"..tag)
    local btnStatus = GetTitleStatus(curTitleList[tag].titleId)
    if btnStatus == 2 then
        -- 判断需要学习的是否已学习
        local needTitleId = needTitleList[tag]
        if needTitleId ~= 0 then
            local titleLearned = GetTitleLearned(needTitleId)
            if not titleLearned then
                GameApi:showMessage(needTitleContent[tag])
                return
            end
        end

        -- 检测花费物品是否足够
		if costIdList[tag] ~= nil then
			local remainNum = ItemManager:Get():getItemNumberById(tonumber(costIdList[tag]))
			local needNum = tonumber(costCountList[tag])
			if remainNum < needNum then 
				GameApi:showMessage(GetLuaLocalization("M_TITLE_ERROR1"))
				return 
			end
		end        

        local function sendMsg()
	        msgTable = {
                title_id = curTitleList[tag].titleId
		        }
	        local msgname="CSTitleLearnReq"
	        local ret = send_message(msgname, msgTable, true)
	        return ret
        end
        PushOneWaitingLayer("CSTitleLearnReq")
        sendMsg()
    elseif btnStatus == 3 then
        local function sendMsg()
	        msgTable = {
                title_id = curTitleList[tag].titleId
		        }
	        local msgname="CSTitleWearReq"
	        local ret = send_message(msgname, msgTable, true)
	        return ret
        end
        PushOneWaitingLayer("CSTitleWearReq")
        sendMsg()
    end
end

local function onMsgTitleLearn(msg)
    GameApi:showMessage(GetLuaLocalization("M_TITLE_LEARNSUCCESS"))
    SendQueryTitleReq()
end

local function onMsgTitleWear(msg)
    GameApi:showMessage(GetLuaLocalization("M_TITLE_EQUIPSUCCESS"))
    SendQueryTitleReq()
	SendUserInfoReqTitle()
end

-- 添加一条称号
local function addSingleTitleItem(itemList, index, itemData)
    local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_TITLE_FRAME_TITLE")
	local bgListItemSize = itemBg:getContentSize()		
	local ListItemNode = CCNode:create()

	local bgControl = itemList:AddBaseItem(bgListItemSize, ListItemNode)

    local function addPic(bgControl, bgPos, resName, frameName, sprite)
        local icon, iconPos = AspriteManager:getInstance():getOneFrameAndPosition(resName,frameName)

        if sprite then 
            sprite:setPosition(ccpSub(iconPos, bgPos))
            bgControl:getCurrentNode():addChild(sprite, 10)
            return sprite
        else 
            icon:setPosition(ccpSub(iconPos, bgPos))
            bgControl:getCurrentNode():addChild(icon, 10)
            return icon
        end
	    	    
    end

    local function addLabel(bgControl, bgPos, resName, frameName, contentStr, fontSize, color, alignment)
        local labelFrame, labelPos = AspriteManager:getInstance():getOneFrameAndPosition(resName,frameName)
        local alignmentSetting = kCCTextAlignmentCenter
        if alignment ~= nil and alignment == 0 then
            alignmentSetting = kCCTextAlignmentLeft        
        end

        local label = TXGUI.UILabelTTF:create(contentStr, KJLinXin, fontSize,labelFrame:getContentSize(), alignmentSetting,kCCVerticalTextAlignmentCenter)
        label:setPosition(ccpSub(labelPos, bgPos))
        label:setColor(color)
        bgControl:getCurrentNode():addChild(label, 11)
        return label
    end

    local function addBtn(bgControl, bgPos, resName, normalName, clickedName,priority, tag, tapFunc)
        local normalSprite, btnPos = AspriteManager:getInstance():getOneFrameAndPosition(resName, normalName)
        local clickedSprite = AspriteManager:getInstance():getOneFrameAndPosition(resName, clickedName)

        local iconBtn = IconButton:create(normalSprite, nil, clickedSprite, priority)
        iconBtn:setPosition(ccpSub(btnPos, bgPos))   

        local btnMenuItem = iconBtn:getMenuItem()
        btnMenuItem:setTag(tag)

        btnMenuItem:registerScriptTapHandler(tapFunc)
	
	    bgControl:getCurrentNode():addChild(iconBtn, 10)
        return iconBtn
    end

    -- 背景图
    addPic(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_FRAME_TITLE1")

    -- 称号图片
    local titleIcon = GetTitleIconById(itemData.titleId)
    addPic(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_ICON_TITLE1", titleIcon)

    -- 附加战力提示
    local addFightText = GetLuaLocalization("M_TITLE_ADDFIGHT")
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_TITLEFIGHT", addFightText, 20, ccc3(255,195,118), 0)

    -- 附加战力值
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_NUM_TITLEFIGHT", ""..itemData.fightNum, 20, ccc3(255,195,118), 0)

    -- "解锁条件"
    local unlockText = GetLuaLocalization("M_TITLE_UNLOCKCONDITION")
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_UNLOCKCONDITION", unlockText, 20, ccc3(251,158,38), 0)

	if tonumber(itemData.unlockType) == 1 then
		-- 解锁条件内容
		local unlockContent = GetLuaLocalization("M_TITLE_UNLOCKTYPE_"..itemData.unlockType)
		addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_UNLOCKCONDITION1", unlockContent, 20, ccc3(255,255,255), 0)

		-- 解锁条件数量
	
		local nowTalentNum = TalentMgr.nowTalentCount
		local needTalentNum = tonumber(itemData.unlockData)

		local numStr, numColor
		if nowTalentNum < needTalentNum then
			numStr = "("..nowTalentNum.."/"..needTalentNum..")"
			numColor = ccc3(250,30,25)
		else
			numStr = "("..needTalentNum.."/"..needTalentNum..")"
			numColor = ccc3(56,221,7)
		end
		addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_NUM_UNLOCKCONDITION1", numStr, 20, numColor, 0)
	else
		addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_UNLOCKCONDITION3", GetLuaLocalization(itemData.text), 20, ccc3(255,255,255), 0)
	end


    -- “学习条件”
    local studyConditionText = GetLuaLocalization("M_TITLE_STUDYCONDITION")
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_STUDYCONDITION", studyConditionText, 20, ccc3(251,158,38), 0)

    -- 学习条件内容
    local needTitleId = tonumber(Title[itemData.titleId].Learn_ID)
    local studyContent
    if needTitleId ~= 0 then
        local needName = GetLuaLocalization(Title[needTitleId].Learn_name)
        studyContent = GetLuaLocalization("M_TITLE_NEEDSTUDY")..needName
    else
        studyContent = GetLuaLocalization("M_TITLE_NONE")
    end

    needTitleList[index] = needTitleId
    needTitleContent[index] = studyContent
    
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_STUDYCONDITION1", studyContent, 20, ccc3(17,179,255), 0)

    -- 属性按钮
    addBtn(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_BUTTON_ATTRIBUTE", "map_ui2_FRAME_TITLE_BUTTON_ATTRIBUTE_CLICKED", -139, index, onPressShowAttrLayer)

    -- 属性文本
    addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_ATTRIBUTE", GetLuaLocalization("M_TITLE_ATTRIBUTE"), 20, ccc3(255,255,255))

    -- 装备按钮
    learnBtns[index] = addBtn(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_BUTTON_EQUIP", "map_ui2_FRAME_TITLE_BUTTON_EQUIP_CLICKED", -139, index, onPressLearnBtn)

    -- 灰色装备图片
    local grayIcon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin", "map_ui2_FRAME_TITLE_BUTTON_EQUIP_UNLOCK")
    grayPics[index] = addPic(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_BUTTON_EQUIP_UNLOCK", grayIcon)


    -- 装备文本
    learnLabels[index] = addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_EQUIP", GetLuaLocalization("M_TITLE_EQUIP"), 20, ccc3(255,255,255))


	local costId, costNum = GetTitleCostData(itemData.titleId)
    costIdList[index] = costId
    costCountList[index] = costNum

	if costId ~= nil then
		    -- 花费文本
		costTexts[index] = addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_TEXT_COST", GetLuaLocalization("M_TITLE_COST"), 16, ccc3(251,158,38))

    
		-- 花费图标，直接从本图中读
		local costIcon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin", "map_ui2_FRAME_TITLE_ICON_"..costId)
		costIcons[index] = addPic(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_ICON_COST", costIcon)

		-- 花费数值
		costNums[index] = addLabel(bgControl, bgPos, "UI/ui2.bin", "map_ui2_FRAME_TITLE_NUM_COST", ""..costNum, 16, ccc3(255,255,255), 0)
	else
		costTexts[index] = nil
		costIcons[index] = nil
		costNums[index] = nil
	end
end

local function refreshTitleScrollList()
	local index = TitleLayer.sheetIndex

	TitleLayer.uiLayout:FindChildObjectByName("sheetBtn"..(3-index)).__UIButton__:getMenuItemSprite():unselected()
	TitleLayer.uiLayout:FindChildObjectByName("sheetBtn"..index).__UIButton__:getMenuItemSprite():selected()

	if index == 1 then
		curTitleList = geniusTitleList
	else
		curTitleList = otherTitleList
	end

	local itemList = TitleLayer.uiLayout:FindChildObjectByName("titleScrollList").__UIScrollList__
	itemList:setStartOffset(ccp(0, 0))
	itemList:clearChildrens()

    for i = 1, #curTitleList do
		--CCLuaLog("-------addSingleTitleItem")
        addSingleTitleItem(itemList, i, curTitleList[i])
    end

end

local function initTitleList()
    geniusTitleList = {}
	otherTitleList = {}
    for k, v in pairs(Title) do
        local itemData = {}
        itemData.titleId = k
        itemData.fightNum = GetTitleFightNumById(k)
        itemData.unlockType, itemData.unlockData = GetTitleUnlockCondition(k)
		itemData.text = v.Unlock_text

		--titleList[#titleList + 1] = itemData  
		if tonumber(itemData.unlockType) == 1 then
			geniusTitleList[#geniusTitleList + 1] = itemData  
		else
			otherTitleList[#otherTitleList + 1] = itemData
		end              
    end

    local function compareFunc(a, b)
        return a.titleId < b.titleId
    end
    table.sort(geniusTitleList, compareFunc)
	table.sort(otherTitleList, compareFunc)

    refreshTitleScrollList()
end

-- 点击标签按钮
local function onPressSheetBtn(tag)
	TitleLayer.sheetIndex = tag

	

	refreshTitleScrollList()

	SendQueryTitleReq()
	PushOneWaitingLayer("CSTitleReq")
end

function TitleLayer:InitLayer()
	CCLuaLog("--- TitleLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
    self.uiLayout:FindChildObjectByName("checkTotalBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressShowAllAttr)

	self.uiLayout:FindChildObjectByName("sheetBtn1").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressSheetBtn)
	self.uiLayout:FindChildObjectByName("sheetBtn2").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressSheetBtn)
	self.uiLayout:FindChildObjectByName("sheetBtn1").__UIButton__:getMenuItemSprite():setTag(1)
	self.uiLayout:FindChildObjectByName("sheetBtn2").__UIButton__:getMenuItemSprite():setTag(2)


    self.attrLayout = self.uiLayout:FindChildObjectByName("detailLayer");
    self.attrLayout:FindChildObjectByName("hideBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressHideAttrLayer)

    self.attrLayout:setVisible(false)

    initTitleList()
    RefreshTitleLayerStatus()

    addMsgCallBack("CSTitleLearnRsp", onMsgTitleLearn)
    addMsgCallBack("CSTitleWearRsp", onMsgTitleWear)

    --PushOneLayer(self.uiLayerInstance,"","")
    --SetChatBar(false,-1)
end

--------------------------------------
function RefreshTitleLayerStatus()
    if TitleLayer.uiLayout ~= nil then
        local costId = GetTitleCostData(geniusTitleList[1].titleId)
        local remainNum = ItemManager:Get():getItemNumberById(tonumber(costId))
        TitleLayer.uiLayout:FindChildObjectByName("resNum1").__UILabel__:setString(""..remainNum)

        for i = 1, #curTitleList do
            local titleStatus = GetTitleStatus(curTitleList[i].titleId)
            if titleStatus == 1 then
				--CCLuaLog("--- 1 ---")
                learnBtns[i]:setVisible(false)
                grayPics[i]:setVisible(true)
                learnLabels[i]:setString(GetLuaLocalization("M_TITLE_UNLOCK"))
                learnLabels[i]:setColor(ccc3(255,255,255))
				if costTexts[i] ~= nil then
					costTexts[i]:setVisible(false)
					costIcons[i]:setVisible(false)
					costNums[i]:setVisible(false)
				end
                

            elseif titleStatus == 2 then
                learnBtns[i]:setVisible(true)
                learnBtns[i]:showTipIcon(ccp(1, 1))
                grayPics[i]:setVisible(false)
                learnLabels[i]:setString(GetLuaLocalization("M_TITLE_STUDY"))
                learnLabels[i]:setColor(ccc3(255,255,255))

				if costTexts[i] ~= nil then
					costTexts[i]:setVisible(costIdList[i] ~= nil)
					costIcons[i]:setVisible(costIdList[i] ~= nil)
					costNums[i]:setVisible(costIdList[i] ~= nil)
				end                

            elseif titleStatus == 3 then
                learnBtns[i]:setVisible(true)
                learnBtns[i]:hideTipIcon()
                grayPics[i]:setVisible(false)
                learnLabels[i]:setString(GetLuaLocalization("M_TITLE_EQUIP"))
                learnLabels[i]:setColor(ccc3(255,255,255))

				if costTexts[i] ~= nil then
					costTexts[i]:setVisible(false)
					costIcons[i]:setVisible(false)
					costNums[i]:setVisible(false)
				end


            elseif titleStatus == 4 then
                learnBtns[i]:setVisible(false)
                grayPics[i]:setVisible(false)
                learnLabels[i]:setString(GetLuaLocalization("M_TITLE_EQUIPNOW"))
                learnLabels[i]:setColor(ccc3(56,221,7))

				if costTexts[i] ~= nil then
					costTexts[i]:setVisible(false)
					costIcons[i]:setVisible(false)
					costNums[i]:setVisible(false)
				end
			else
				CCLuaLog("titleStatus error : "..curTitleList[i].titleId)
            end            
        end
    end
end