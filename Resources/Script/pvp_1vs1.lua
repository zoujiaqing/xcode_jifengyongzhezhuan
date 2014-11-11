

local topLayout_1vs1 = TXGUI.UIManager:sharedManager():getUILayout("pvp_1vs1");

local closeBtn_1vs1 = topLayout_1vs1:FindChildObjectByName("closeBtn_1vs1").__UIButton__:getMenuItemSprite();
local btnReady_1vs1 = topLayout_1vs1:FindChildObjectByName("readyBtn").__UIButton__;
local btnGo_1vs1 =   topLayout_1vs1:FindChildObjectByName("goReadyBtn").__UIButton__;

local l_roomID = -1;

--初始化房间标题
function InitRoomTitle_1vs1(roomID)
        CCLuaLog("init RoomTitle RoomID:"..roomID);
        print("roomID:"..roomID);
        if   l_roomID  ~= roomID then
            l_roomID = roomID;
   			local label = topLayout_1vs1:FindChildObjectByName("1vs1_roomTile").__UILabel__;
			label:setString("房间"..roomID);
			label:setVisible(true);
		end
end

--关闭当前ui layout
function onCloseBtnClicked_1vs1(pObj)
    CCLuaLog("onCloseBtnClicked_1vs1 begin!");
	HideChatToolLayerAllItems();
    MessageHandler:sShareInstance():pvp_exitRoom();
	MessageHandler:sShareInstance():pvp_getRoomList();
	TXGUI.UIManager:sharedManager():removeUILayout("pvp_1vs1");
    ShowPVPHall();
	
	--send leam room message
    CCLuaLog("onCloseBtnClicked_1vs1 end!");
end
closeBtn_1vs1:registerScriptTapHandler(onCloseBtnClicked_1vs1);

-- hide close buttons
local headIcon_1vs1={};

--获取玩家头像
function GetheadIcon_1vs1(character)
    
    CCLuaLog("GetheadIcon_1vs1 character:"..character);
	local asMgr = AspriteManager:getInstance(); 
	
	local frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN";
	
    if character == 1 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSMAN";
	end
		
    if character == 2 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_SWORDSWOMAN";
	end
	
    if character == 3 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDMAN";
	end
	
    if character == 4 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_WIZARDWOMAN";
	end
	
    if character == 5 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINMAN";
	end
	
    if character == 6 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_ASSASINWOMAN";
	end
	
    if character == 7 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERMAN";
	end
	
    if character == 8 then
	    frameRes =  "map_ui_character_select_FRAME_CHARACTER_SELECT_HUNTERWOMAN";
	end

	local headIcon = asMgr:getOneFrame("UI/ui_character_select.bin", frameRes);
	return headIcon;	
end

--初始化host和玩家位置信息
function  InitHostPlace_1vs1(bHost)

    CCLuaLog("InitHostPlace_1vs1!");
	ShowPlaceButtons_1vs1(1, false);
    
	--- hide host kick button	
	local iconHost  =  topLayout_1vs1:FindChildObjectByName("1vs1_host0").__UIPicture__;
	iconHost:setVisible(bHost);
    
    local btnClose = topLayout_1vs1:FindChildObjectByName("1vs1_kickGuest0").__UIButton__;
    btnClose:setVisible(false);
    
    local waitLabel =  topLayout_1vs1:FindChildObjectByName("1vs1_waitTile0").__UILabel__;
    waitLabel:setVisible(false);

	local nameLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playername0").__UILabel__;
	nameLabel:setString(pvp_RoomSeat[0]['name']);
	print("Host seat 0 : [".. pvp_RoomSeat[0]['name'].."]-"..pvp_RoomSeat[0]['Avatar']);
	
	local levelLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playerlevel0").__UILabel__;
	levelLabel:setString( "LV"..pvp_RoomSeat[0]['level']);
    
    CCLuaLog("what's the fuck0".. pvp_RoomSeat[0]['level']);

    
    local readypic = topLayout_1vs1:FindChildObjectByName("1vs1_ready0").__UILabel__;
    readypic:setVisible(false);
    
	local placePic = topLayout_1vs1:FindChildObjectByName("1vs1_occupation0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon_1vs1( pvp_RoomSeat[0]['Avatar']);
	icon:setPosition(154, 37);
    placePic:removeChildByTag(100,true);
    placePic:addChild(icon,0,100);

	
end

--显示某个位置信息
function ShowPlaceButtons_1vs1(place, bShow)

	    CCLuaLog("ShowPlaceButtons_1vs1 !");
        
        local btnClose = topLayout_1vs1:FindChildObjectByName("1v1_kickGuest"..place).__UIButton__;   		
  		if bShow == true and PVP_bHostPlayer == true then
  		         btnClose:setVisible(true);  		     
  		     else
  		         btnClose:setVisible(false);  		     
  		end 
        
   		--local btnClose = topLayout_1vs1:FindChildObjectByName("1v1_kickGuest"..place).__UIButton__;
        --btnClose:setVisible(true);
        
        local iconHost  =  topLayout_1vs1:FindChildObjectByName("1vs1_host"..place).__UIPicture__;
        iconHost:setVisible(false);

		local levelLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playerlevel"..place).__UILabel__;
		levelLabel:setVisible(bShow);
		levelLabel:setString( "LV"..pvp_RoomSeat[place]['level']);
        
		local bWait = not bShow;
		local waitLabel =  topLayout_1vs1:FindChildObjectByName("1vs1_waitTile"..place).__UILabel__;
		waitLabel:setVisible(bWait);
        
        local nameLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playername"..place).__UILabel__;
		nameLabel:setVisible(bShow);
		nameLabel:setString(pvp_RoomSeat[place]['name']);
        
        local percentLabel = topLayout_1vs1:FindChildObjectByName("1vs1_percentWin"..place).__UILabel__;
		percentLabel:setVisible(bShow);
        
        local networkLabel = topLayout_1vs1:FindChildObjectByName("1vs1_network"..place).__UILabel__;
		networkLabel:setVisible(bShow);
        
        --- hide head icon if no guest seat on this chair
		print("pvp_RoomSeat:["..place.."]:"..pvp_RoomSeat[place]['seat_id']);
		
		if (pvp_RoomSeat[place]['seat_id'] < 0) then
		    if headIcon_1vs1[place] ~=nil then 
		        print("remove icon"..place);
			 	headIcon_1vs1[place]:removeFromParentAndCleanup(true);
			 	headIcon_1vs1[place] = nil;			 	
			end	

		else
		    if (headIcon_1vs1[place] == nil) then
		    	local placePic = topLayout_1vs1:FindChildObjectByName("1vs1_occupation"..place).__UIPicture__:getCurrentNode();	    
		    	--- have one guest here!
		    	local icon = GetheadIcon_1vs1(pvp_RoomSeat[place]['Avatar']);		
				icon:setPosition(154, 37);			
		    	placePic:addChild(icon);			
				headIcon_1vs1[place] = icon;	
				
			    print("add icon"..place);
		    end	
		end

		ShowAutoChatItems(2)
		ShowPPVEChatButton()
end


--更新玩家信息
function UpdateGuestSeat_1vs1()

	CCLuaLog("UpdateGuestSeat_1vs1 !");
    
	local nameLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playername0").__UILabel__;
	nameLabel:setString(pvp_RoomSeat[0]['name']);
	
	local levelLabel = topLayout_1vs1:FindChildObjectByName("1vs1_playerlevel0").__UILabel__;
	levelLabel:setString( "LV"..pvp_RoomSeat[0]['level']);
    
    CCLuaLog("what's the fuck UpdateGuestSeat_1vs1"..pvp_RoomSeat[0]['level']);

	local placePic = topLayout_1vs1:FindChildObjectByName("1vs1_occupation0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon_1vs1(pvp_RoomSeat[0]['Avatar']);
	icon:setPosition(154,40);
    
    placePic:removeChildByTag(100,true);

    placePic:addChild(icon,0,100);
    
    local i = 1;
    if (pvp_RoomSeat[i]['seat_id'] > 0) then
    		ShowPlaceButtons_1vs1(i, true)
    else
    		ShowPlaceButtons_1vs1(i, false);
    end
    
    if (pvp_RoomSeat[i]['bReady'] == true) then
            local icon  =  topLayout_1vs1:FindChildObjectByName("1vs1_ready"..i).__UIPicture__;
            icon:setVisible(true);
    else
            local icon  =  topLayout_1vs1:FindChildObjectByName("1vs1_ready"..i).__UIPicture__;
            icon:setVisible(false);
    end		

    if (PVP_bHostPlayer == true) then
		if isAllGuestReady_1vs1() == false then
    		print("not all guests are ready!");
    	end	
    end
end

--是否准备好开始PK
function isAllGuestReady_1vs1()

    if pvp_RoomSeat[1]['seat_id'] > 0 and pvp_RoomSeat[1]['bReady'] == false then
    	return false;
    end
    
    if pvp_RoomSeat[1]['seat_id'] < 0 then
    	return false;
    end
    
	return true; 
end

local bReady = false;
--准备
function onReady_1vs1(pObj)
	bReady = not bReady;
	MessageHandler:sShareInstance():pvp_setReady(bReady);
end

--开发PK
function onGo_1vs1(pObj)
   if isAllGuestReady_1vs1() == true then
       PushOneWaitingLayer("CSPVPStartReq");
       MessageHandler:sShareInstance():pvp_startGame();
   else
       local rstString = LanguageLocalization:GetLocalization("M_PVP_NOT_READY");
       GameApi:showMessage(rstString);
   end
end

--注册Go和Ready
btnReady_1vs1:getMenuItemSprite():registerScriptTapHandler(onReady_1vs1);
btnGo_1vs1:getMenuItemSprite():registerScriptTapHandler(onGo_1vs1);        

if PVP_bHostPlayer == true then
	btnReady_1vs1:setVisible(false);
	btnGo_1vs1:setVisible(true);
else   
	btnReady_1vs1:setVisible(true);
	btnGo_1vs1:setVisible(false);
end

------------------------------------kick off button message----------------------------
function onCloseGuest1_1vs1(pObj)
	MessageHandler:sShareInstance():pvp_kickOffFromRoom(1);
end

local btKf1 = topLayout_1vs1:FindChildObjectByName("1v1_kickGuest1").__UIButton__;  
btKf1:getMenuItemSprite():registerScriptTapHandler(onCloseGuest1_1vs1);	

InitHostPlace_1vs1(true);
UpdateGuestSeat_1vs1();
CCLuaLog("pvp_1vs1 loaded!");
HideChatToolLayerAllItems();
