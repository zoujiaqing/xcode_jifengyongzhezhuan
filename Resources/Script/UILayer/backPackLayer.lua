require("Script/GameConfig/characterString")
require("Script/GameConfig/Localization")
require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")

--CCLuaLog("backPackLayer.lua is run")

local b_attributeListInit = false;
local updateFunc = nil;
local notificationFunc = nil;
local backPackLayout = nil
local attributeBtn = nil

local attributeControl = {};

local function addOneAttribute(attri,name,number,des)
	local layout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
	if layout ~= nil then 
		local attributeList = layout:FindChildObjectByName("attributeList").__UIScrollList__;
		if attributeList ~= nil then
			local nameText = Localization[name]["SimpleChinese"];
			local desText = Localization[des]["SimpleChinese"];
			if (nameText ~= nil) and (desText ~= nil) and (number ~= nil)then
				local attrName,namePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_TEXT_ATTRIBUTE");
				local attrNumber,numberPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_NUMBER_ATTRIBUTE");
				local attrDes,desPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_ATTRIBUTE_TEXT_ATTRIBUTE_DESCRIPTION");
				local nodeSize = CCSize(attrDes:getContentSize().width,namePt.y - desPt.y +
								 attrDes:getContentSize().height * 0.5 + attrName:getContentSize().height * 0.5);
	 
				local baseNode = CCNode:create();
				local base = attributeList:AddBaseItem(nodeSize,baseNode);
			
				local nameLabel = TXGUI.UILabelTTF:create(nameText,KJLinXin,24,attrName:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
				baseNode:addChild(nameLabel);
				nameLabel:setPosition(ccp(attrName:getContentSize().width * 0.5 - nodeSize.width * 0.5,nodeSize.height * 0.5 - attrName:getContentSize().height * 0.5));
				nameLabel:setColor(ccc3(238,166,26));

				local numberLabel = TXGUI.UILabelTTF:create(number,KJLinXin,24,attrNumber:getContentSize(),kCCTextAlignmentRight,kCCVerticalTextAlignmentCenter);
				baseNode:addChild(numberLabel);
				numberLabel:setColor(ccc3(238,166,26));
				numberLabel:setPosition(ccp(nodeSize.width * 0.5 - attrNumber:getContentSize().width * 0.5,nodeSize.height * 0.5 - attrNumber:getContentSize().height * 0.5));

				local desLabel = TXGUI.UILabelTTF:create(desText,KJLinXin,18,attrDes:getContentSize(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
				baseNode:addChild(desLabel);
				desLabel:setPosition(ccp(attrDes:getContentSize().width * 0.5 -nodeSize.width * 0.5,attrDes:getContentSize().height * 0.5 - nodeSize.height * 0.5));
			
				if attributeControl[attri] == nil then
					attributeControl[attri] = {};
					attributeControl[attri].name = nameLabel;
					attributeControl[attri].numbel = numberLabel;
					attributeControl[attri].des = desLabel;
				end
			end
		end
	end
end

local function addAllAttribute()
	local layout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
	if layout ~= nil then 
		local attributeList = layout:FindChildObjectByName("attributeList").__UIScrollList__;
		if attributeList ~= nil then
			attributeList:clearChildrens();
			for k in pairs(attributeControl) do
				attributeControl[k] = nil
			end
		end
	end
	local userInfo = UserData:GetUserInfo();
	addOneAttribute(GM_ATTR_ABILITY_ALL_UPDATE,"M_ATTRIBUTE_ALL",userInfo.effectiveness,"M_ATTRIBUTE_ALL_DESCRIPTION");
	addOneAttribute(GM_ATTR_PHYSICAL_ATTACK_UPDATE,"M_ATTRIBUTE_PHYATTACK",userInfo.physical_attack,"M_ATTRIBUTE_PHYATTACK_DESCRIPTION");
	addOneAttribute(GM_ATTR_MAGIC_ATTACK_UPDATE,"M_ATTRIBUTE_MAGATTACK",userInfo.magic_attack,"M_ATTRIBUTE_MAGATTACK_DESCRIPTION");
	addOneAttribute(GM_ATTR_SKILL_ATTACK_UPDATE,"M_ATTRIBUTE_SKIATTACK",userInfo.skill_attack,"M_ATTRIBUTE_SKIATTACK_DESCRIPTION");
	addOneAttribute(GM_ATTR_PHYSICAL_DEFENCE_UPDATE,"M_ATTRIBUTE_PHYDEFENCE",userInfo.physical_defence,"M_ATTRIBUTE_PHYDEFENCE_DESCRIPTION");
	addOneAttribute(GM_ATTR_MAGIC_DEFENCE_UPDATE,"M_ATTRIBUTE_MAGDEFENCE",userInfo.magic_defence,"M_ATTRIBUTE_MAGDEFENCE_DESCRIPTION");
	addOneAttribute(GM_ATTR_SKILL_DEFENCE_UPDATE,"M_ATTRIBUTE_SKIDEFENCE",userInfo.skill_defence,"M_ATTRIBUTE_SKIDEFENCE_DESCRIPTION")	
	addOneAttribute(GM_ATTR_HEALTH_POINT_UPDATE,"M_ATTRIBUTE_LIFE",userInfo.total_hp,"M_ATTRIBUTE_LIFE_DESCRIPTION");	
	addOneAttribute(GM_ATTR_ACCURATE_UPDATE,"M_ATTRIBUTE_ACCURATE",userInfo.accurate,"M_ATTRIBUTE_ACCURATE_DESCRIPTION");	
	addOneAttribute(GM_ATTR_DODGE_UPDATE,"M_ATTRIBUTE_DODGE",userInfo.dodge,"M_ATTRIBUTE_DODGE_DESCRIPTION");
	addOneAttribute(GM_ATTR_WRECK_UPDATE,"M_ATTRIBUTE_WRECK",userInfo.wreck,"M_ATTRIBUTE_WRECK_DESCRIPTION");
	addOneAttribute(GM_ATTR_PARRY_UPDATE,"M_ATTRIBUTE_BLOCK",userInfo.parry,"M_ATTRIBUTE_BLOCK_DESCRIPTION");
	addOneAttribute(GM_ATTR_CRITICAL_STRIKE_UPDATE,"M_ATTRIBUTE_CRITICAL",userInfo.critical_strike,"M_ATTRIBUTE_CRITICAL_DESCRIPTION");
	addOneAttribute(GM_ATTR_TENACITY_UPDATE,"M_ATTRIBUTE_TENACITY",userInfo.tenacity,"M_ATTRIBUTE_TENACITY_DESCRIPTION");	
	addOneAttribute(GM_ATTR_SLAY_UPDATE,"M_ATTRIBUTE_SLAY",userInfo.slay,"M_ATTRIBUTE_SLAY_DESCRIPTION");
	addOneAttribute(GM_ATTR_SPEED_UPDATE,"M_ATTRIBUTE_SPEED",userInfo.speed,"M_ATTRIBUTE_SPEED_DESCRIPTION");
end

--local function showUserPic()
--	local layout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
--	if layout ~= nil then 
--		local picFrame = layout:FindChildObjectByName("attributeMC").__UIPicture__;
--		if picFrame ~= nil then	
--			local picName = getCharacterField(UserData:GetUserType(),"attrPic");
--			CCLuaLog(string.format("picName:%s",picName));
--			local pic = CCSprite:create(picName);
--			if pic ~= nil then
--				pic:setAnchorPoint(ccp(0,0));
--				local oldPointX,oldPointY = picFrame:getCurrentNode():getPosition();
--				local x = picFrame:getCurrentNode():getContentSize().width * picFrame:getCurrentNode():getScaleX();
--				local y = picFrame:getCurrentNode():getContentSize().height * picFrame:getCurrentNode():getScaleY();
--				picFrame:setSprite(pic);
--				pic:setPosition(ccp(oldPointX - x * 0.5,oldPointY - y * 0.5));
--			else
--				CCLuaLog("pic is nil");
--			end
--		end
--	end
--end

function showUserAttributes()
	if b_attributeListInit == false then
		addAllAttribute();
		--showUserPic();
		b_attributeListInit = true;
	end

	return true
end

local function hideUserAttributes()

end

local function unRegisterHandlers()
	if updateFunc ~= nil then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFunc);
	end
	if notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notificationFunc);
	end
end

local function updateAttributes(dt)
	local layout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
	if layout == nil then 
		unRegisterHandlers();
	end
end

local function getUserDataByMessage(message)
	local data = nil;
	local userInfo = UserData:GetUserInfo();
	if message == GM_ATTR_PHYSICAL_ATTACK_UPDATE then
		data = userInfo.physical_attack
	elseif message == GM_ATTR_MAGIC_ATTACK_UPDATE then
		data = userInfo.magic_attack
	elseif message == GM_ATTR_SKILL_ATTACK_UPDATE then
		data = userInfo.skill_attack
	elseif message == GM_ATTR_PHYSICAL_DEFENCE_UPDATE then
		data = userInfo.physical_defence
	elseif message == GM_ATTR_MAGIC_DEFENCE_UPDATE then
		data = userInfo.magic_defence
	elseif message == GM_ATTR_SKILL_DEFENCE_UPDATE then
		data = userInfo.skill_defence
	elseif message == GM_ATTR_SKILL_ATTACK_UPDATE then
		data = userInfo.skill_attack
	elseif message == GM_ATTR_HEALTH_POINT_UPDATE then
		data = userInfo.total_hp
	elseif message == GM_ATTR_ACCURATE_UPDATE then
		data = userInfo.accurate
	elseif message == GM_ATTR_DODGE_UPDATE then
		data = userInfo.dodge
	elseif message == GM_ATTR_WRECK_UPDATE then
		data = userInfo.wreck
	elseif message == GM_ATTR_PARRY_UPDATE then
		data = userInfo.parry
	elseif message == GM_ATTR_CRITICAL_STRIKE_UPDATE then
		data = userInfo.critical_strike
	elseif message == GM_ATTR_TENACITY_UPDATE then
		data = userInfo.tenacity
	elseif message == GM_ATTR_SLAY_UPDATE then
		data = userInfo.slay
	elseif message == GM_ATTR_SPEED_UPDATE then
		data = userInfo.speed
	elseif message == GM_ATTR_ABILITY_ALL_UPDATE then
		data = userInfo.effectiveness
	else
		data = nil;
	end 
	return data;
end

local function updateNotification(message)
	--CCLuaLog("backPackLayer:updateNotification");
	local layout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
	if layout ~= nil then 
		local str = getUserDataByMessage(message);
		if (attributeControl[message] ~= nil) and (attributeControl[message].numbel ~= nil) then
			attributeControl[message].numbel:setString(str);
		end
	--else
		--CCLuaLog("-------------backPackLayer:updateNotification error-----------------");
	end
end

local function onReceiveDestructorHandler()
	--CCLuaLog("BackPack:onReceiveDestructorHandler")
	backPackLayout = nil
	attributeBtn = nil
	if updateFunc ~= nil then
		CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFunc)
		updateFunc = nil
	end
	if notificationFunc ~= nil then
		NotificationCenter:defaultCenter():unregisterScriptObserver(notificationFunc)
		notificationFunc = nil
	end
	b_attributeListInit = false
	attributeControl = {}
end


function InitBackPack()
	--CCLuaLog("InitBackPack")
	local tag = UITagTable["BackPackLayer"].tag
	local layer = getUILayerByTag(tag,false)
	if layer ~= nil then
		layer:registerDestructorScriptHandler(onReceiveDestructorHandler)
	end
	backPackLayout = TXGUI.UIManager:sharedManager():getUILayout("BackPack");
	--attributeBtn = backPackLayout:FindChildObjectByName("playerAttributeBt").__UIButton__:getMenuItemSprite();
	--attributeBtn:registerScriptTapHandler(showUserAttributes);
    --print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 8")
	updateFunc = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(updateAttributes, 1.0, false);
	notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification)
	return true
end

