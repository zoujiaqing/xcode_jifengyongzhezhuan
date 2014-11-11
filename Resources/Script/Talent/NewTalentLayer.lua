-- 新天赋页面
require("Script/Talent/TalentMgr")

NewTalentLayer = {
	uiLayerInstance = nil,
	uiLayout = nil,

	notificationFunc = nil,	

    
}

-- layer析构时的通知消息(layer已经被析构)，此时重置数据，删除UILayout.
local function onReceiveDestructorHandler()
	if NewTalentLayer.notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(NewTalentLayer.notificationFunc);
	end

	NewTalentLayer:destroyed()
	TXGUI.UIManager:sharedManager():removeUILayout("NewTalentLayer")
end

-- 游戏内广播通知接收函数
local function updateNotification(message)
	local ret = 0;
	if message == GM_LUA_LAYER_CLOSE then
		if nil ~= NewTalentLayer.uiLayerInstance then
			if G_CurLayerInstance ~= NewTalentLayer.uiLayerInstance then
				NewTalentLayer:destroyed()
				ret = 1
			end
		end
	end

	return ret
end

function NewTalentLayer:CreateLayer()
	if self.uiLayerInstance == nil then 
		self.uiLayerInstance = CCLayer:create()
		local parentNode = MainMenuLayer:getMainMenuLayer()
		if parentNode ~= nil then
			local tag = UITagTable["NewTalentLayer"].tag
			parentNode:addChild(self.uiLayerInstance,10,tag)
			self.uiLayerInstance:registerDestructorScriptHandler(onReceiveDestructorHandler)
			self.uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/NewTalentLayer.plist",self.uiLayerInstance, "NewTalentLayer", true)
			self:InitLayer()
		end
	end

	return self.uiLayerInstance
end

-- 重置参数
function NewTalentLayer:resetValue()
	self.uiLayerInstance = nil
	self.uiLayout = nil
	self.notificationFunc = nil
end

-- 删除UI
function NewTalentLayer:destroyed()
	CCLuaLog("--- NewTalentLayer:destroyed() ---")
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
    RemoveOneLayer(NewTalentLayer.uiLayerInstance)
	NewTalentLayer:destroyed()
end

-- 查看称号
local function onPressCheckTitleBtn(tag)
    CCLuaLog("--- onPressCheckTitleBtn ---")
    showUILayerByTag(UITagTable["TitleLayer"].tag,true)
end

-- 点亮
local function onPressLightBtn(tag)
    CCLuaLog("--- onPressLightBtn ---")

    local _, costStarNum, costCoinNum = GetNextTalentInfo()
    local remainStarNum = TalentMgr.remainStarCount
    local remainCoinNum = UserData:GetUserInfo().m_gold

    if remainStarNum < costStarNum then
        GameApi:showMessage(GetLuaLocalization("M_GENIUS_STARNOTENOUGH"))
        return 
    end

    if remainCoinNum < costCoinNum then
        GameApi:showMessage(GetLuaLocalization("M_GENIUS_COINNOTENOUGH"))
        return 
    end

    local function sendDowerLightReq()
	    msgTable = {
		    }
	    local msgname="CSNewDowerReq"
	    local ret = send_message(msgname, msgTable, true)
	    return ret
    end

    sendDowerLightReq()
end

-- 一键点亮
local function onPressQuickLightBtn(tag)
    CCLuaLog("--- onPressQuickLightBtn ---")

    local costStarNum, costCoinNum = GetLightTalentTotalCost(TalentMgr.nextTitleRemain)
    local remainStarNum = TalentMgr.remainStarCount
    local remainCoinNum = UserData:GetUserInfo().m_gold

    if remainStarNum < costStarNum then
        GameApi:showMessage(GetLuaLocalization("M_GENIUS_STARNOTENOUGH"))
        return 
    end

    if remainCoinNum < costCoinNum then
        GameApi:showMessage(GetLuaLocalization("M_GENIUS_COINNOTENOUGH"))
        return 
    end

    local function sendDowerOneKeyLightReq()
	    msgTable = {
		    }
	    local msgname="CSNewDowerOneKeyReq"
	    local ret = send_message(msgname, msgTable, true)
	    return ret
    end

    sendDowerOneKeyLightReq()
    
end

-- 在某个icon上播放例子特效
local function playParticleOnTalent(index)
    if NewTalentLayer.uiLayout ~= nil then
        local talentFrame = NewTalentLayer.uiLayout:FindChildObjectByName("talentCover"..index).__UIPicture__
        local frameSize = talentFrame:getCurrentNode():getContentSize()

        local particle = ParticleManagerX:sharedManager():getParticles_uieffect("particle_effect_upgradesuccess")
        if nil ~= particle then
            particle:setPosition(ccp(frameSize.width * 0.5, frameSize.height * 0.5));
            talentFrame:getCurrentNode():removeChildByTag(1001, true)
            talentFrame:getCurrentNode():addChild(particle, 10, 1001)
        end
    end
    
end

-- 收到点亮回包
local function onMsgLightRsp(msg)
    GameAudioManager:sharedManager():playEffect(350006,false,0)

    local talentIndex = GetTalentIndexById(msg.id)
    CCLuaLog("--------------onMsgLightRsp--------"..talentIndex)
    playParticleOnTalent(talentIndex)

    GameApi:showMessage(GetLuaLocalization("M_GENIUS_LIGHTSUCCESS"))
    SendQueryDowerReq()
end

-- 收到一键点亮回包
local function onMsgQuickLightRsp(msg)
    GameAudioManager:sharedManager():playEffect(350006,false,0)
    
    for i = 1, 6 do
        playParticleOnTalent(i)
    end
        
    GameApi:showMessage(GetLuaLocalization("M_GENIUS_LIGHTSUCCESS"))
    SendQueryDowerReq()
end



-- 称号滚动效果
local titlePicList = {}
local titlePicPos = {}
local titleDeltaPos = {}

local function initTitlePicList()
    local scale = TXGUI.UIManager:sharedManager():getScaleFactor()
    for i = 1, 5 do
        titlePicList[i] = NewTalentLayer.uiLayout:FindChildObjectByName("titleIcon"..i).__UIPicture__

        local _, pos = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui2.bin","map_ui2_FRAME_GENIUS_ICON_TITLE"..i)
        titlePicPos[i] = pos
    end

    for i = 1, 4 do
        titleDeltaPos[i] = ccp(0, (titlePicPos[i].y - titlePicPos[i+1].y) * scale)
    end
end

local function refreshTitleEffect(nowTitleId, nextTitleId)



    local moveTime = 0.5

    local scale = TXGUI.UIManager:sharedManager():getScaleFactor()

    -- 重置所有位置，缩放，action
    local function resetAll()
        for i = 1, 5 do
            titlePicList[i]:getCurrentNode():setPosition(titlePicPos[i])
            titlePicList[i]:getCurrentNode():setScale(scale)
            titlePicList[i]:getCurrentNode():stopAllActions()
        end
    end

    resetAll()

    local titlePicFlag = {}
    for i = 1, 5 do
        titlePicFlag[i] = false
    end

    -- 直接设置称号图标
    local function setTitlePics(titleId)     

        if Title[titleId - 1] ~= nil then
            local icon_2 = GetTitleIconById(titleId - 1)            
            titlePicList[2]:setSprite(icon_2)
            titlePicList[2]:getCurrentNode():setScale(0.7 * scale)
            titlePicFlag[2] = true
        else
            titlePicList[2]:getCurrentNode():setScale(0)
        end

        if Title[titleId] ~= nil then
            local icon_3 = GetTitleIconById(titleId)
            titlePicList[3]:setSprite(icon_3)

            local titlePicSize = titlePicList[3]:getCurrentNode():getContentSize()

            local lockIcon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_GENIUS_ICON_TITLE1LOCK")
            lockIcon:setPosition(ccp(titlePicSize.width/2, titlePicSize.height/2))
            titlePicList[3]:getCurrentNode():addChild(lockIcon, 10, 1001)
            titlePicList[3]:getCurrentNode():setScale(1 * scale)
            titlePicFlag[3] = true
        else
            titlePicList[3]:getCurrentNode():setScale(0)
        end

        if Title[titleId + 1] ~= nil then
            local icon_4 = GetTitleIconById(titleId + 1)           
            titlePicList[4]:setSprite(icon_4)

            local titlePicSize = titlePicList[4]:getCurrentNode():getContentSize()

            local lockIcon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_GENIUS_ICON_TITLE1LOCK")
            lockIcon:setPosition(ccp(titlePicSize.width/2, titlePicSize.height/2))
            titlePicList[4]:getCurrentNode():addChild(lockIcon)
            titlePicList[4]:getCurrentNode():setScale(0.7 * scale)
            titlePicFlag[4] = true
        else
            titlePicList[4]:getCurrentNode():setScale(0)
        end

        if Title[titleId + 2] ~= nil then
            local icon_5 = GetTitleIconById(titleId + 2)            
            titlePicList[5]:setSprite(icon_5)

            local titlePicSize = titlePicList[5]:getCurrentNode():getContentSize()

            local lockIcon = AspriteManager:getInstance():getOneFrame("UI/ui2.bin","map_ui2_FRAME_GENIUS_ICON_TITLE1LOCK")
            lockIcon:setPosition(ccp(titlePicSize.width/2, titlePicSize.height/2))
            titlePicList[5]:getCurrentNode():addChild(lockIcon)       
            titlePicFlag[5] = true 
        else
            titlePicList[5]:getCurrentNode():setScale(0)    
        end

        titlePicList[1]:getCurrentNode():setScale(0)
        titlePicList[5]:getCurrentNode():setScale(0)
    end

    -- 播放解锁动画
    local function unlockNextTitle()
        titlePicList[3]:getCurrentNode():removeChildByTag(1001, true)

        local frameSize = titlePicList[3]:getCurrentNode():getContentSize()

        local particle = ParticleManager:Get():createEffectSpriteAndPlay(448,"", 100000, 1, false)
		particle:setPosition(ccp(frameSize.width/2, frameSize.height/2))
		titlePicList[3]:getCurrentNode():addChild(particle)
    end

    -- 放缩中间称号
    local function scaleNextTitle()
        

        local array = CCArray:create()

        local scaleBig = CCScaleTo:create(0.2, 2 * scale)
        local resetScale = CCScaleTo:create(0.2, 1 * scale)

        array:addObject(scaleBig)
        array:addObject(resetScale)

        local seq = CCSequence:create(array)
        titlePicList[3]:getCurrentNode():runAction(seq)
    end

    -- 移动称号图标
    local function moveTitlePics()
        if titlePicFlag[5] then
            titlePicList[5]:getCurrentNode():setScale(0.5 * scale)
        else
            titlePicList[5]:getCurrentNode():setScale(0)
        end

        titlePicList[2]:getCurrentNode():runAction(CCMoveBy:create(moveTime, titleDeltaPos[1]))
        titlePicList[2]:getCurrentNode():runAction(CCScaleTo:create(moveTime, 0))

        titlePicList[3]:getCurrentNode():runAction(CCMoveBy:create(moveTime, titleDeltaPos[2]))
        titlePicList[3]:getCurrentNode():runAction(CCScaleTo:create(moveTime, 0.7 * scale))

        if titlePicFlag[4] then
            titlePicList[4]:getCurrentNode():runAction(CCMoveBy:create(moveTime, titleDeltaPos[3]))
            titlePicList[4]:getCurrentNode():runAction(CCScaleTo:create(moveTime, 1 * scale))
        else
            titlePicList[4]:getCurrentNode():setScale(0)
        end
        
        if titlePicFlag[5] then
            titlePicList[5]:getCurrentNode():runAction(CCMoveBy:create(moveTime, titleDeltaPos[4]))
            titlePicList[5]:getCurrentNode():runAction(CCScaleTo:create(moveTime, 0.7 * scale))
        else
            titlePicList[5]:getCurrentNode():setScale(0)
        end

        for i = 1, 4 do
            CCLuaLog("DeltaPos : "..titleDeltaPos[i].x..","..titleDeltaPos[i].y)
        end
    end

    -- 重置图标位置及icon
    local function setNextTitle()
        setTitlePics(nextTitleId)
    end

    local backNode = NewTalentLayer.uiLayout:FindChildObjectByName("backPic").__UIPicture__:getCurrentNode()

    if nowTitleId == 0 then
        setTitlePics(nextTitleId)
    else
        setTitlePics(nowTitleId)

        local arrayOfActions = CCArray:create()

        local unlockFunc = CCCallFunc:create(unlockNextTitle)
        local delayScale = CCDelayTime:create(1)
        local scaleFunc = CCCallFunc:create(scaleNextTitle)
        local delayMove = CCDelayTime:create(0.5)
        local moveFunc = CCCallFunc:create(moveTitlePics)
        local delayReset = CCDelayTime:create(moveTime)
        local resetFunc = CCCallFunc:create(resetAll)
        local setNextFunc = CCCallFunc:create(setNextTitle)

        arrayOfActions:addObject(unlockFunc)
        arrayOfActions:addObject(delayScale)
        arrayOfActions:addObject(scaleFunc)
        arrayOfActions:addObject(delayMove)
        arrayOfActions:addObject(moveFunc)
        arrayOfActions:addObject(delayReset)
        arrayOfActions:addObject(resetFunc)
        arrayOfActions:addObject(setNextFunc)

        backNode:stopAllActions()
        backNode:runAction(CCSequence:create(arrayOfActions))

    end
end

-- 收到称号解锁消息
local function onMsgTitleUnlockRsp(msg)
    CCLuaLog("--------------------------onMsgTitleUnlockRsp")

	if NewTalentLayer.uiLayout == nil then
		return
	end

    GameAudioManager:sharedManager():playEffect(350015,false,0)
    GameApi:showMessage(GetLuaLocalization("M_TITLE_UNLOCKTITLE"))
    if msg ~= nil then
        local titleId = msg.title
        if Title[titleId] ~= nil then
            refreshTitleEffect(titleId, titleId + 1)
        elseif Title[titleId + 1] ~= nil then
            refreshTitleEffect(0, titleId + 1)
        end
        SendQueryTitleReq()
    end
end


local talentEffectList = {}

-- 初始化属性图标
local function initTalentIcon()
    for i = 1, 6 do
        local icon = GetTalentIconByIndex(i)
        if icon then
            NewTalentLayer.uiLayout:FindChildObjectByName("talentIcon"..i).__UIPicture__:setSprite(icon)
        end      
        
        -- 添加环绕特效
        local coverIcon = NewTalentLayer.uiLayout:FindChildObjectByName("talentFrame"..i).__UIPicture__
        local coverSize = coverIcon:getCurrentNode():getContentSize()

        local particle = ParticleManager:Get():createEffectSpriteAndPlay(449,"", 100000, 1, true)
        talentEffectList[i] = particle
        particle:setPosition(ccp(coverSize.width/2, coverSize.height/2))

        coverIcon:getCurrentNode():addChild(particle, 10)
    end    
end

function NewTalentLayer:InitLayer()
	CCLuaLog("--- NewTalentLayer:InitLayer() ---")
	self.notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)

	--注册回调函数
	self.uiLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCloseBtn)
    self.uiLayout:FindChildObjectByName("checkTitleBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressCheckTitleBtn)
    self.uiLayout:FindChildObjectByName("lightBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressLightBtn)
    self.uiLayout:FindChildObjectByName("quickLightBtn").__UIButton__:getMenuItemSprite():registerScriptTapHandler(onPressQuickLightBtn)

    initTalentIcon()
    RefreshTalentLayer()

    initTitlePicList()

    local nextTitleId = GetNextTitleId()

    refreshTitleEffect(0, nextTitleId)

    SendQueryDowerReq()
    SendQueryTitleReq()

    addMsgCallBack("CSNewDowerRsp", onMsgLightRsp)
    addMsgCallBack("CSNewDowerOneKeyRsp", onMsgQuickLightRsp)
    addMsgCallBack("CSTitleUnlockRsp", onMsgTitleUnlockRsp)

    CCLuaLog(TalentMgr.wearTitle)
    for k, v in pairs(TalentMgr.unlockTitles) do
        CCLuaLog("TalentMgr.unlockTitles")
        CCLuaLog(v)
    end

    for k, v in pairs(TalentMgr.learnTitles) do
        CCLuaLog("TalentMgr.learnTitles")
        CCLuaLog(v)
    end

    PushOneLayer(self.uiLayerInstance,"","")
    SetChatBar(false,-1)
end


-- 刷新天赋面板
function RefreshTalentLayer()
    CCLuaLog("--- RefreshTalentLayer ---")

    if NewTalentLayer.uiLayout == nil then
        return
    end

    -- 剩余星数及金币数
    local remainStarNum = TalentMgr.remainStarCount
    local remainCoinNum = UserData:GetUserInfo().m_gold

    NewTalentLayer.uiLayout:FindChildObjectByName("remainStarNum").__UILabel__:setString(""..remainStarNum)
    NewTalentLayer.uiLayout:FindChildObjectByName("remainCoinNum").__UILabel__:setString(""..MainMenuLayer:GetNumByFormat(remainCoinNum))


    -- 6个天赋对应等级及遮罩
    for i = 1, 6 do
        local levelStr = TalentMgr.talentLevels[i] ~= 0 and "Lv."..(TalentMgr.talentLevels[i] + 1) or "Lv.1"
        NewTalentLayer.uiLayout:FindChildObjectByName("talentLevel"..i).__UILabel__:setString(levelStr)
        local coverIcon = NewTalentLayer.uiLayout:FindChildObjectByName("talentCover"..i).__UIPicture__
        if TalentMgr.nextTalentIndex == i then
            coverIcon:setVisible(false)
            talentEffectList[i]:setVisible(true) 
        else
            coverIcon:setVisible(true)
            talentEffectList[i]:setVisible(false) 
        end
    end

    --下一个天赋icon
    local nextTalentIcon = GetTalentIconByIndex(TalentMgr.nextTalentIndex)
    NewTalentLayer.uiLayout:FindChildObjectByName("talentSelectIcon").__UIPicture__:setSprite(nextTalentIcon)
    
    --下一个天赋等级
    NewTalentLayer.uiLayout:FindChildObjectByName("talentSelectLevel").__UILabel__:setString("Lv."..(TalentMgr.talentLevels[TalentMgr.nextTalentIndex] + 1))

    local attrNum, costStarNum, costCoinNum = GetNextTalentInfo()
    local attrStr = GetTalentAttrStr(TalentMgr.nextTalentIndex, attrNum)

    -- 下一个天赋附加属性
    NewTalentLayer.uiLayout:FindChildObjectByName("talentSelectContent").__UILabel__:setString(attrStr)


    --消耗
    NewTalentLayer.uiLayout:FindChildObjectByName("needStarNum").__UILabel__:setString(""..costStarNum)
    NewTalentLayer.uiLayout:FindChildObjectByName("needCoinNum").__UILabel__:setString(""..MainMenuLayer:GetNumByFormat(costCoinNum))

    -- 总属性
    for i = 1, 6 do
        local attrStr = GetTalentAttrStr(i, TalentMgr.totalAttrNum[i])
        NewTalentLayer.uiLayout:FindChildObjectByName("totalAttr"..i).__UILabel__:setString(attrStr)
    end
end
