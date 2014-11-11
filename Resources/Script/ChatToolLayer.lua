--Note: 通用ChatToolLayer 用户需要自己定义隐藏隐藏代码代码和和显示显示代码代码
--require("Script/MainMenuUILayer")
require("Script/Chat/ChatLayer")
-- 1. ppve 2. pvp 3. 主城防守
g_AutoChatType = 0

G_ChatToolLayerItems = {

}

ChatToolLayer = {
	channelPage = nil,
}

function CreateChatToolLayer(parentNode)
	if nil ~= parentNode then
		local uiLayout = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar");
		--local uiLayout = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/chatToolbar.plist",parentNode, "chatToolbar", true);
		HideChatToolLayerAllItems()
		return uiLayout
	end
	return nil
end

function SetChatBar(isExtraVisible,order)
	SetChatToolLayer(true,true,isExtraVisible,order)
end

function SetChatToolLayer(isChatVisible,isFuncVisible,isExtraVisible,order)
	local mainLayer = MainMenuLayer:getMainMenuLayer()
	if mainLayer ~= nil then
		mainLayer:setChatToolbar(isChatVisible,isFuncVisible,isExtraVisible,order)
		HideChatToolLayerAllItems()
	end
end	

local function onChatBtClicked(tag)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["ChatUILayer"].tag,true)
end

local function onPpveChatBtClicked(tag)
	GameAudioManager:sharedManager():playEffect(350001,false);
	StopHeroMove()
	showUILayerByTag(UITagTable["ChatUILayer"].tag,true)
	ChatLayer:SetPPVEModle()
end

function InitChatToolLayer()
	local chatToolbarLayerOut = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar");
	
	G_ChatToolLayerItems = {}
	--if chatToolbarLayerOut ~= nil then
		--Note:PPVE Layer
		g_ppveChatButton = chatToolbarLayerOut:FindChildObjectByName("ppveChatButton").__UIButton__;
		g_ppveChatButton:getMenuItemSprite():registerScriptTapHandler(onPpveChatBtClicked)
		InsertOneItemToChatToolLayer(g_ppveChatButton)
		g_createButton = chatToolbarLayerOut:FindChildObjectByName("createButton").__UIButton__;
		InsertOneItemToChatToolLayer(g_createButton)
		g_EnterButton = chatToolbarLayerOut:FindChildObjectByName("EnterButton").__UIButton__;
		InsertOneItemToChatToolLayer(g_EnterButton)
		g_createBtnLabel = chatToolbarLayerOut:FindChildObjectByName("createLabel").__UILabel__;
		InsertOneItemToChatToolLayer(g_createBtnLabel)
		g_enterBtnLabel = chatToolbarLayerOut:FindChildObjectByName("joinLabel").__UILabel__;
		InsertOneItemToChatToolLayer(g_enterBtnLabel)
		g_autoChatBtn = chatToolbarLayerOut:FindChildObjectByName("autoChatBtn").__UIButton__
		InsertOneItemToChatToolLayer(g_autoChatBtn)
		g_autoChatLabel = chatToolbarLayerOut:FindChildObjectByName("autoChatLabel").__UILabel__;
		InsertOneItemToChatToolLayer(g_autoChatLabel)
		--Note:Elf Layer
		--g_TrainButton = chatToolbarLayerOut:FindChildObjectByName("trainButton").__UIButton__;
		--InsertOneItemToChatToolLayer(g_TrainButton)
		--g_TrainLabel = chatToolbarLayerOut:FindChildObjectByName("trainTextLabel").__UILabel__;
		--InsertOneItemToChatToolLayer(g_TrainLabel)
		--g_TrainAllPotentialLabel = chatToolbarLayerOut:FindChildObjectByName("trainTextAllPotentialLabel").__UILabel__;
		--InsertOneItemToChatToolLayer(g_TrainAllPotentialLabel)
		--g_TrainNumLabel = chatToolbarLayerOut:FindChildObjectByName("trainNumPotentialLabel").__UILabel__;
		--InsertOneItemToChatToolLayer(g_TrainNumLabel)
		
		--local chatBt = chatToolbarLayerOut:FindChildObjectByName("chatBt").__UIButton__:getMenuItemSprite()
		--chatBt:registerScriptTapHandler(onChatBtClicked)
	--end
end

local function onReceiveDestructorHandler()
	if ChatToolLayer.channelPage ~= nil then
		ChatToolLayer.channelPage:destroy()
		ChatToolLayer.channelPage = nil
	end
end

function InitSmallChatArea()
	
	local chatToolbarLayerOut = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar")
	if chatToolbarLayerOut ~= nil then
		local layer = getUILayerByTag(UITagTable["ChatToolbar"].tag,false)
		if layer ~= nil then
			layer:registerDestructorScriptHandler(onReceiveDestructorHandler)
		end

		local chatBt = chatToolbarLayerOut:FindChildObjectByName("chatBt").__UIButton__:getMenuItemSprite()
		chatBt:registerScriptTapHandler(onChatBtClicked)
		local chatBt2 = chatToolbarLayerOut:FindChildObjectByName("chatBt2").__UIButton__:getMenuItemSprite()
		chatBt2:registerScriptTapHandler(onChatBtClicked)
		
		ChatToolLayer.channelPage = ChatShowPage:create()
		ChatToolLayer.channelPage:addChatType("WORLD")
		ChatToolLayer.channelPage:addChatType("PARTY")
		ChatToolLayer.channelPage:addChatType("PM")
		ChatToolLayer.channelPage:init(chatToolbarLayerOut,"chatContainer","chatList",16,-128)
		ChatToolLayer.channelPage:setButtonEnable(false)
	end
	return true
end

function InsertOneItemToChatToolLayer(oneItem)
	if nil~= oneItem then
		local tableCount = table.getn(G_ChatToolLayerItems)
		G_ChatToolLayerItems[tableCount+1] = oneItem
	end
end

function HideChatToolLayerAllItems()
	InitChatToolLayer()
	
	for index,value in pairs(G_ChatToolLayerItems) do 
		value:setVisible(false)
	end
end

function ShowPPVEChatButton()
	g_ppveChatButton:setVisible(true)
end

function ShowPPVELayerItems()
	g_EnterButton:setVisible(true)
	g_createButton:setVisible(true)
	g_createBtnLabel:setVisible(true)
	g_enterBtnLabel:setVisible(true)
	g_ppveChatButton:setVisible(true)
end

--自动喊话

-- ppve 文本
local function getPPVEAutoChatContent()
	local level = g_topPPVELevel
	math.randomseed(tostring(os.time()):reverse():sub(1, 6)) 
	local temp = math.random(3)
	local content = ""
	if temp < 2 then
		content = level .. GetLuaLocalization("M_SPEAK_DETAIL_1")
	elseif temp < 3 then
		content = level .. GetLuaLocalization("M_SPEAK_DETAIL_2")
	else
		content = GetLuaLocalization("M_SPEAK_DETAIL_3_1") .. level .. GetLuaLocalization("M_SPEAK_DETAIL_3_2")
	end
	return content
end

-- pvp 文本
local function getPVPAutoChatContent()
	local content = "pvp auto chat nil "
	if g_PVPType == 1 then
		local level = g_PVPTopLevel - math.fmod(g_PVPTopLevel, 10)
		if g_PVPCount == 1 then
			content = level .. GetLuaLocalization("M_PVP_SPEAK_DETAIL_1")
		else
			content = level .. GetLuaLocalization("M_PVP_SPEAK_DETAIL_2")
		end
	else
		if g_PVPCount == 1 then
			content = GetLuaLocalization("M_PVP_SPEAK_DETAIL_3")
		else
			content = GetLuaLocalization("M_PVP_SPEAK_DETAIL_4")
		end
	end
	return content
end

-- 主城防守文本
local function getCityDefendAutoChatContent()
    local roomFightNum = CityDefendRoomData.fightNum

	local temp = math.random(1, 3)
    return ""..roomFightNum..GetLuaLocalization("M_TD_SPEAK_DETAIL_"..temp)
end

local function sendAutoChat()
	local content = "auto chat nil"
	if g_AutoChatType == 1 then
		content = getPPVEAutoChatContent()
	elseif g_AutoChatType == 2 then
		content = getPVPAutoChatContent()
	elseif g_AutoChatType == 3 then
		content = getCityDefendAutoChatContent()
	end
	sendWorldNote(content,true)
end

local function onAutoChatClicked(tag)
	local cd = GameApi:GetPPVEAutoChatCD()
	if cd == 0 then
		sendAutoChat()
	else
		local content = LanguageLocalization:GetLocalization("M_PPVE_TIPS_1") .. (10 - cd) .. LanguageLocalization:GetLocalization("M_PPVE_TIPS_2")
		GameApi:showMessage(content)
	end
end

function ShowAutoChatItems(type)
	g_autoChatBtn:setVisible(true)
	g_autoChatLabel:setVisible(true)
	if g_autoChatBtn ~= nil then
		g_autoChatBtn:getMenuItemSprite():registerScriptTapHandler(onAutoChatClicked)
	end

	g_AutoChatType = type
end

function ShowElfLayerItems()
	--g_TrainButton:setVisible(true)
	--g_TrainLabel:setVisible(true)
	--g_TrainAllPotentialLabel:setVisible(true)
	--g_TrainNumLabel:setVisible(true)
end

function ShowMoneyConfigLayer()

end

-----------------------------------------------------Init UI Layer---------------------------------------------
function InitPPVELayer(layerParent)	
	print("InitPPVELayer")
	--CreateChatToolLayer(layerParent)
	SetChatBar(false,10)
	ShowPPVELayerItems()
end

function InitPPVERoom(layerParent)
	print("InitPPVERoom")
	SetChatBar(false,10)
	ShowAutoChatItems(1)
	ShowPPVEChatButton()
end

function InitElfLayer(layerParent)
	print("InitElfLayer")
	--CreateChatToolLayer(layerParent)
	SetChatBar(false,-1)
	ShowElfLayerItems()
end

function InitMoneyConfigLayer(layerParent)
	--CreateChatToolLayer(layerParent)
	SetChatBar(false,-1)
	ShowMoneyConfigLayer()
end

---------------------------------------------------------------------------------------------------------------