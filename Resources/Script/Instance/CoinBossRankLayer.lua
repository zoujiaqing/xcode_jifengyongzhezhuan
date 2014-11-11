-- 金币副本排行页面
require("Script/DailyGoals/AwardDaily")

CoinBossRankLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,
    rewardDetailLayout = nil,

	notificationFunc = nil,	

    rankRewardList = {},
    rankPlayerList = {},
	selfRankData = {},
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if CoinBossRankLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(CoinBossRankLayer.notificationFunc);
	end

	CoinBossRankLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("CoinBossRankLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= CoinBossRankLayer.uiLayerInstance then
			if G_CurLayerInstance ~= CoinBossRankLayer.uiLayerInstance then
				CoinBossRankLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function CoinBossRankLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["CoinBossRankLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/CoinBossRankUI.plist",self.uiLayerInstance, "CoinBossRankLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function CoinBossRankLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
    self.rewardDetailLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function CoinBossRankLayer:destroyed()
	CCLuaLog("--- CoinBossRankLayer:destroyed() ---")
	if self.uiLayerInstance ~= nil then
		self.uiLayerInstance:removeFromParentAndCleanup(true)
	end

	if self.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(self.notificationFunc);
	end

	self:resetValue()
    SetChatBar(true,-1)
end

--关闭页面
local function onPressCloseBtn(tag)
    CCLuaLog("--- onPressCloseBtn ---")
    RemoveOneLayer(CoinBossRankLayer.uiLayerInstance)
	CoinBossRankLayer:destroyed()
end

-- 进入副本
local function onPressEnterInstanceBtn(tag)
	OnPressEnterCoinBossInstance()
	do return end
	local count = CoinBossRankLayer.selfRankData.remainCount
	if count ~= nil and count > 0 then
		OnPressEnterCoinBossInstance()
	else
		GameApi:showMessage(GetLuaLocalization("M_ERROR_NUMBER_LACK"))
	end
    
end

-- 设置是否隐藏其他玩家
local function onPressSettingBtn(tag)
	CallNativeFuncManager:getInstance():ShowCombatSettingLayer(true)
end

-- 隐藏奖励详情
local function onPressHideRewardLayer(tag)
    CCLuaLog("--- onPressHideAttrLayer ---")
    if CoinBossRankLayer.rewardDetailLayout then
        CoinBossRankLayer.rewardDetailLayout:setVisible(false)
    end
end

-- 显示奖励详情
local function onPressShowRewardLayer(tag)
    CCLuaLog("--- onPressShowAttrLayer ---")
    if CoinBossRankLayer.rewardDetailLayout then       
        CoinBossRankLayer.rewardDetailLayout:setVisible(true)
    end
end


-- 初始化伤害奖励列表
local function initDamageRewardList()
    -- 初始化数据
    local rewardIndex = 4300
    local rewardDataList = {}
    local itemBtns = {}

    while true do
        if AwardDaily[rewardIndex] ~= nil and rewardIndex < 4320 then
            local valueTable = AwardDaily[rewardIndex]

            rewardDataList[rewardIndex - 4299] = {}

            -- 伤害数值
            rewardDataList[rewardIndex - 4299].damageNum = tonumber(valueTable.Active)

            -- 奖励物品
            local itemIdStr, itemNumStr = string.match(valueTable.RaidDrops1, "(%d+)/(%d+)")
            rewardDataList[rewardIndex - 4299].itemId = tonumber(itemIdStr)
            rewardDataList[rewardIndex - 4299].itemNum = tonumber(itemNumStr)
            rewardIndex = rewardIndex + 1
        else
            break
        end
    end

    local function onPressDamageRewardItem(tag)
        CCLuaLog("----------onPressDamageRewardItem")
        local itemId = rewardDataList[tag].itemId
        
        local menu = itemBtns[tag]:getMenuNode()
        local pos = menu:getTouchPoint()

        ItemManager:Get():showItemTipsById(itemId, pos)
    end

    -- 添加单个奖励条目
    local function addSingleItem(itemList, itemData, index)
        local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_WORLDBOSS_GOLD_REWARD_FRAME1")
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

        -- 伤害文本
        local damageStr = GetLuaLocalization("M_HURT_NUMBER")..MainMenuLayer:GetNumByFormatK(itemData.damageNum)..""..GetLuaLocalization("M_WORLDBOSS_GET")
        addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_REWARD_TEXT1", damageStr, 20, ccc3(255,195,118), 2)

        -- 奖励物品背景
        addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_REWARD_ICON")

        -- 奖励物品图标按钮
        local normalItemIcon = ItemManager:Get():getIconSpriteById(itemData.itemId)
        local clickItemIcon = ItemManager:Get():getIconSpriteById(itemData.itemId)
        local _ , btnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_REWARD_ICON1")

        local iconBtn = IconButton:create(normalItemIcon, nil, clickItemIcon, -133)
        iconBtn:setPosition(ccpSub(btnPos, bgPos)) 
        iconBtn:setContentSize(normalItemIcon:getContentSize()) 

        
        
        local fontSize = GameFontManager:smallFontSize()
	    iconBtn:setItemLabel(""..itemData.itemNum, fontSize, KJLinXin) 

        local btnMenuItem = iconBtn:getMenuItem()
        btnMenuItem:setTag(index)

        btnMenuItem:registerScriptTapHandler(onPressDamageRewardItem)
	
	    bgControl:getCurrentNode():addChild(iconBtn, 10)

        itemBtns[index] = iconBtn
    end

    local rewardList = CoinBossRankLayer.uiLayout:FindChildObjectByName("rewardScrollList").__UIScrollList__

    for i = 1, #rewardDataList do
        addSingleItem(rewardList, rewardDataList[i], i)
    end    
end

function CoinBossRankLayer:InitLayer()
	CCLuaLog("--- CoinBossRankLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
    self.uiLayout:FindChildObjectByName("rankRewardBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressShowRewardLayer)
    self.uiLayout:FindChildObjectByName("enterBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressEnterInstanceBtn)
    --self.uiLayout:FindChildObjectByName("setBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressSettingBtn)

    self.rewardDetailLayout = self.uiLayout:FindChildObjectByName("rewardDetailLayer");
    self.rewardDetailLayout:FindChildObjectByName("hideBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressHideRewardLayer)

    self.rewardDetailLayout:setVisible(false)


    initDamageRewardList()
	--RefreshCoinBossRankList()
    --RefreshRankRewardDetailLayer()
	--RefreshSelfRankInfo()

    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end

-- 刷新玩家排名
function RefreshCoinBossRankList()

	if CoinBossRankLayer.uiLayout == nil then
		return
	end

    local function onPressViewBtn(tag)
		local playerData = CoinBossRankLayer.rankPlayerList[tag]
        local userId = PlayerIdMake(playerData.userId,playerData.regTime,playerData.channelId)

        PlayerItemCenter:setSelectedPlayer(userId)
	    showUILayerByTag(UITagTable["OtherPlayerEquipLayer"].tag,true)
    end

	local function addSingleItem(itemList, itemData, index)
		local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME1")
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
			elseif alignment == 2 then
				alignmentSetting = kCCTextAlignmentRight        
            end

            local label = TXGUI.UILabelTTF:create(contentStr, KJLinXin, fontSize,CCSizeMake(0,0), alignmentSetting,kCCVerticalTextAlignmentCenter)
            label:setPosition(ccpSub(labelPos, bgPos))
            label:setColor(color)
            bgControl:getCurrentNode():addChild(label, 11)
            return label, labelPos, label:getContentSize().width
        end

		-- 背景图
		addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME")

		-- 玩家排名
		if index <= 3 then
            local iconRankSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_PAY_RANK"..index)
            addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_NUMBER", iconRankSprite)
        else
            addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_NUMBER", ""..index, 24, ccc3(255,84,0), 1)
        end

		-- 玩家VIP等级
		local vipStr = ""
		if itemData.vip == 0 then
			vipStr = ""
		else
			vipStr = "V"..itemData.vip
		end


		local vipLabel, vipLabelPos, vipWidth = addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_VIP", vipStr, 20, ccc3(255,163,4), 1)

		-- 玩家昵称
		local nameColor = ccc3(255, 195, 118)
		if itemData.rank == 1 then
			nameColor = ccc3(250, 30, 25)
		elseif itemData.rank == 2 then
			nameColor = ccc3(201, 57, 255)
		elseif itemData.rank == 3 then
			nameColor = ccc3(17, 179, 255)
		end
		local nameLabel,nameLabelPos, nameWidth = addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_NAME", ""..itemData.name, 20, nameColor, 1)

		-- 校准位置
		local totalWidth = vipWidth + nameWidth

		local finalMidpos = ccpSub(nameLabelPos, bgPos)
		local deltaVip = ccp(vipWidth / 2 - totalWidth / 2, 0)
		local deltaName = ccp(totalWidth / 2 - nameWidth / 2, 0)

		vipLabel:setPosition(ccpAdd(finalMidpos, deltaVip))
		nameLabel:setPosition(ccpAdd(finalMidpos, deltaName))

		-- 玩家伤害数值
		addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_HURT", ""..itemData.hurtNum, 20, nameColor, 1)

		-- 查看玩家按钮
		if CoinBossRankLayer.selfRankData.rank ~= nil and itemData.rank == CoinBossRankLayer.selfRankData.rank then
			local selfMarkPic = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_MYMARK")
			addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_LOOK", selfMarkPic)
		else
			local normalItemIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL_CLICKED")
			local clickItemIcon = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_DETAIL")
			local _ , btnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin", "map_ui_FRAME_WORLDBOSS_GOLD_RANK_FRAME_LOOK")

			local iconBtn = IconButton:create(normalItemIcon, nil, clickItemIcon, -133)
			iconBtn:setPosition(ccpSub(btnPos, bgPos)) 

			local btnMenuItem = iconBtn:getMenuItem()
			btnMenuItem:setTag(index)

			btnMenuItem:registerScriptTapHandler(onPressViewBtn)
	
			bgControl:getCurrentNode():addChild(iconBtn, 10)			
		end
		

	end

	local rankList = CoinBossRankLayer.uiLayout:FindChildObjectByName("rankScrollList").__UIScrollList__
	rankList:clearChildrens()

    for i = 1, #CoinBossRankLayer.rankPlayerList do
        addSingleItem(rankList, CoinBossRankLayer.rankPlayerList[i], i)
    end   

    
end

-- 刷新排名奖励详情页面
function RefreshRankRewardDetailLayer()
    local detailLayout = CoinBossRankLayer.rewardDetailLayout
    if detailLayout == nil then
        return
    end

    local itemBtns = {}

    local function onPressRankRewardItem(tag)
        CCLuaLog("----------onPressRankRewardItem")
        local playerIndex = math.floor(tag / 10)
        local itemIndex = tag % 10

        local itemId = CoinBossRankLayer.rankRewardList[playerIndex][itemIndex].id
        
        local menu = itemBtns[tag]:getMenuNode()
        local pos = menu:getTouchPoint()

        ItemManager:Get():showItemTipsById(itemId, pos)
    end

    local function addSingleItem(itemList, itemData, index)
        local itemBg, bgPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_WORLDBOSS_RANKREWARD_REWARD_FRAME")
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

		addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_REWARD_FRAME")

        -- 前三名的排名图标
        if itemData.rank <= 3 then
			if itemData.rank == itemData.rankEnd then
				local iconRankSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin","map_ui_system_icon_FRAME_ICON_PAY_RANK"..itemData.rank)
				addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_RANK", iconRankSprite)
			else
				addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_RANK", ""..itemData.rank.."-"..itemData.rankEnd, 30, ccc3(255,84,0), 1)
			end             
        else
			if itemData.rank == itemData.rankEnd then
				addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_RANK", ""..itemData.rank, 30, ccc3(255,84,0), 1)
			elseif itemData.rank < itemData.rankEnd then
				addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_RANK", ""..itemData.rank.."-"..itemData.rankEnd, 30, ccc3(255,84,0), 1)
			else
				addLabel(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_RANK", ""..itemData.rank.."+", 30, ccc3(255,84,0), 1)
			end            
        end

        for i = 1, 2 do
            if itemData[i] == nil then
                break
            end

            addPic(bgControl, bgPos, "UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_FRAME"..i+1)

            local normalItemIcon = ItemManager:Get():getIconSpriteById(itemData[i].id)
            local clickItemIcon = ItemManager:Get():getIconSpriteById(itemData[i].id)
            local _ , btnPos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin", "map_ui_FRAME_WORLDBOSS_RANKREWARD_ICON"..i+1)

            local iconBtn = IconButton:create(normalItemIcon, nil, clickItemIcon, -136)
            iconBtn:setPosition(ccpSub(btnPos, bgPos)) 
            iconBtn:setContentSize(normalItemIcon:getContentSize())         
        
            local fontSize = GameFontManager:smallFontSize()
	        iconBtn:setItemLabel(""..MainMenuLayer:GetNumByFormat(itemData[i].num), fontSize, KJLinXin) 

            local btnMenuItem = iconBtn:getMenuItem()
            btnMenuItem:setTag(index * 10 + i)

            btnMenuItem:registerScriptTapHandler(onPressRankRewardItem)
	
	        bgControl:getCurrentNode():addChild(iconBtn, 10)

            itemBtns[index * 10 + i] = iconBtn
        end 
    end

    local rewardList = CoinBossRankLayer.uiLayout:FindChildObjectByName("rewardDetailScrollList").__UIScrollList__
	rewardList:clearChildrens()

    for i = 1, #CoinBossRankLayer.rankRewardList do
        addSingleItem(rewardList, CoinBossRankLayer.rankRewardList[i], i)
    end    

end

function RefreshSelfRankInfo()

	CCLuaLog("--------RefreshSelfRankInfo")
	if CoinBossRankLayer.uiLayout == nil or CoinBossRankLayer.selfRankData.rank == nil then
		return
	end

	local layout = CoinBossRankLayer.uiLayout
	local selfRankData = CoinBossRankLayer.selfRankData

	CCLuaLog("--------RefreshSelfRankInfo")
	-- 刷新排名
	layout:FindChildObjectByName("selfRankNum").__UILabel__:setString(""..selfRankData.rank)

	-- 刷新VIP等级
	--layout:FindChildObjectByName("selfVipText").__UILabel__:setString("V"..selfRankData.vip)
	--layout:FindChildObjectByName("selfVipText").__UILabel__:setString("")

	-- 刷新名称
	--layout:FindChildObjectByName("selfNameText").__UILabel__:setString(""..selfRankData.name)
	--layout:FindChildObjectByName("selfNameText").__UILabel__:setString("")

	-- 刷新伤害数值
	layout:FindChildObjectByName("selfHurtNum").__UILabel__:setString(""..selfRankData.hurtNum)

	-- 刷新剩余次数
	layout:FindChildObjectByName("remainText").__UILabel__:setString(GetLuaLocalization("M_HEROTREASURE_REMAINTIME")..selfRankData.remainCount)

	-- 手动添加vip及名称组合
	local orgLabel = layout:FindChildObjectByName("selfNameText").__UILabel__

	local vipLevel = UserData:GetVipLevel()

	local vipStr = ""
	if vipLevel == 0 then
		vipStr = ""
	else
		vipStr = "V"..vipLevel
	end

	local vipLabel = TXGUI.UILabelTTF:create(vipStr, KJLinXin, 20, CCSizeMake(0,0), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter)
	local nameLabel = TXGUI.UILabelTTF:create(""..selfRankData.name, KJLinXin, 20, CCSizeMake(0,0), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter)

	vipLabel:setColor(ccc3(255, 164, 4))
	nameLabel:setColor(ccc3(56, 221, 7))

	local vipWidth = vipLabel:getContentSize().width
	local nameWidth = nameLabel:getContentSize().width

	local totalWidth = vipWidth + nameWidth

	local finalMidpos = ccp(0, 0)
	local deltaVip = ccp(vipWidth / 2 - totalWidth / 2, 0)
	local deltaName = ccp(totalWidth / 2 - nameWidth / 2, 0)

	vipLabel:setPosition(ccpAdd(finalMidpos, deltaVip))
	nameLabel:setPosition(ccpAdd(finalMidpos, deltaName))

	orgLabel:getCurrentNode():removeAllChildrenWithCleanup(true)
	orgLabel:getCurrentNode():addChild(vipLabel)
	orgLabel:getCurrentNode():addChild(nameLabel)
end