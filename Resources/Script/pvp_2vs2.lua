

local topLayout_2vs2 = TXGUI.UIManager:sharedManager():getUILayout("pvp_2vs2");
local closeBtn_2vs2 = topLayout_2vs2:FindChildObjectByName("closeBtn_2vs2").__UIButton__:getMenuItemSprite();
local btnReady_2vs2 = topLayout_2vs2:FindChildObjectByName("readyBtn").__UIButton__;
local btnGo_2vs2 =   topLayout_2vs2:FindChildObjectByName("goReadyBtn").__UIButton__;

local l_roomID = -1;

--初始化房间标题
function InitRoomTitle_2vs2(roomID)
        CCLuaLog("InitRoomTitle_2vs2:"..roomID);
        print("roomID:"..roomID);
        if    l_roomID  ~= roomID then
            l_roomID = roomID;
   			local label = topLayout_2vs2:FindChildObjectByName("2vs2_roomTile").__UILabel__;
			label:setString("房间"..roomID);
			label:setVisible(true);
		end
end

 --关闭当前ui layout
function onCloseBtnClicked_2vs2(pObj)
	HideChatToolLayerAllItems();
    CCLuaLog("onCloseBtnClicked_2vs2 begin!");
	TXGUI.UIManager:sharedManager():removeUILayout("pvp_2vs2");
	ShowPVPHall();
	--send leam room message
	MessageHandler:sShareInstance():pvp_exitRoom();
	MessageHandler:sShareInstance():pvp_getRoomList();
	
    CCLuaLog("onCloseBtnClicked_2vs2 end!");
end

closeBtn_2vs2:registerScriptTapHandler(onCloseBtnClicked_2vs2);

-- hide close buttons
local headIcon_2vs2={};

--获取玩家头像
function GetheadIcon_2vs2(character)

    print("character:"..character);
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
function InitHostPlace_2vs2(bHost)

    CCLuaLog("InitHostPlace_2vs2!");
	ShowPlaceButtons_2vs2(1, false);
	ShowPlaceButtons_2vs2(2, false);
	ShowPlaceButtons_2vs2(3, false);

	--- hide host kick button	
	local iconHost  =  topLayout_2vs2:FindChildObjectByName("2vs2_host0").__UIPicture__;
	iconHost:setVisible(bHost);
    
    local btnClose = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest0").__UIButton__;
    btnClose:setVisible(false);
    
    local waitLabel =  topLayout_2vs2:FindChildObjectByName("2vs2_waitTile0").__UILabel__;
    waitLabel:setVisible(false);

	local nameLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playername0").__UILabel__;
	nameLabel:setString(pvp_RoomSeat[0]['name']);
    
	print("Host seat 0 : [".. pvp_RoomSeat[0]['name'].."]-"..pvp_RoomSeat[0]['Avatar']);
	
	local levelLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playerlevel0").__UILabel__;
	levelLabel:setString( "LV"..pvp_RoomSeat[0]['level']);
    
    local readypic = topLayout_2vs2:FindChildObjectByName("2vs2_ready0").__UILabel__;
    readypic:setVisible(false);

	local placePic = topLayout_2vs2:FindChildObjectByName("2vs2_occupation0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon_2vs2( pvp_RoomSeat[0]['Avatar']);
	icon:setPosition(154, 40);
    
    placePic:removeChildByTag(100,true);
    placePic:addChild(icon,0,100);	
    
    
end

--显示某个位置信息
function ShowPlaceButtons_2vs2(place, bShow)
	    CCLuaLog("ShowPlaceButtons_2vs2!");
        
        local btnClose = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest"..place).__UIButton__;   		
  		if bShow == true and PVP_bHostPlayer == true then
  		         btnClose:setVisible(true);  		     
  		     else
  		         btnClose:setVisible(false);  		     
  		end
        
        --local btnClose = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest"..place).__UIButton__;
        --btnClose:setVisible(true);
        
        local iconHost  =  topLayout_2vs2:FindChildObjectByName("2vs2_host"..place).__UIPicture__;
        iconHost:setVisible(false);
        
        local levelLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playerlevel"..place).__UILabel__;
		levelLabel:setVisible(bShow);
		levelLabel:setString( "LV"..pvp_RoomSeat[place]['level']);
        
        local bWait = not bShow;
		local waitLabel =  topLayout_2vs2:FindChildObjectByName("2vs2_waitTile"..place).__UILabel__;
		waitLabel:setVisible(bWait);

        local nameLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playername"..place).__UILabel__;
		nameLabel:setVisible(bShow);
		nameLabel:setString(pvp_RoomSeat[place]['name']);
        
        local percentLabel = topLayout_2vs2:FindChildObjectByName("2vs2_percentWin"..place).__UILabel__;
		percentLabel:setVisible(bShow);
        
        local networkLabel = topLayout_2vs2:FindChildObjectByName("2vs2_network"..place).__UILabel__;
		networkLabel:setVisible(bShow);

        
        --- hide head icon if no guest seat on this chair
		print("pvp_RoomSeat:["..place.."]:"..pvp_RoomSeat[place]['seat_id']);
        if (pvp_RoomSeat[place]['seat_id'] < 0) then

            if headIcon_2vs2[place] ~=nil then 
		        print("remove icon"..place);
			 	headIcon_2vs2[place]:removeFromParentAndCleanup(true);
			 	headIcon_2vs2[place] = nil;			 	
			end
    
		else
            if (headIcon_2vs2[place] == nil) then
            
		    	local placePic = topLayout_2vs2:FindChildObjectByName("2vs2_occupation"..place).__UIPicture__:getCurrentNode();	    
		    	---have one guest here!
		    	local icon = GetheadIcon_2vs2(pvp_RoomSeat[place]['Avatar']);		
				icon:setPosition(154, 40);			
		    	placePic:addChild(icon);			
				headIcon_2vs2[place] = icon;	
                
			    print("add icon"..place);
		    end	
		end

		ShowAutoChatItems(2)
		ShowPPVEChatButton()
end


--更新玩家信息
function UpdateGuestSeat_2vs2()

	CCLuaLog("UpdateGuestSeat_2vs2 !");
	local nameLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playername0").__UILabel__;
	nameLabel:setString(pvp_RoomSeat[0]['name']);
	
	local levelLabel = topLayout_2vs2:FindChildObjectByName("2vs2_playerlevel0").__UILabel__;
	levelLabel:setString( "LV"..pvp_RoomSeat[0]['level']);

	local placePic = topLayout_2vs2:FindChildObjectByName("2vs2_occupation0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon_2vs2(pvp_RoomSeat[0]['Avatar']);
	icon:setPosition(154, 40);
    
    placePic:removeChildByTag(100,true);
    placePic:addChild(icon,0,100);
    
    for i=1,3,1 do
		if (pvp_RoomSeat[i]['seat_id'] > 0) then
    		ShowPlaceButtons_2vs2(i, true);
        else
    		ShowPlaceButtons_2vs2(i, false);
		end
	end
    
	for i=1,3,1 do
		if ( pvp_RoomSeat[i]['bReady'] == true) then
                CCLuaLog("UpdateGuestSeat_2vs2  show ready pic");
    			local icon = topLayout_2vs2:FindChildObjectByName("2vs2_ready"..i).__UIPicture__;
	            icon:setVisible(true);
    		else
                CCLuaLog("UpdateGuestSeat_2vs2  hide ready pic");
    			local icon  =  topLayout_2vs2:FindChildObjectByName("2vs2_ready"..i).__UIPicture__;
	            icon:setVisible(false);
		end		
	end	
    
    if (PVP_bHostPlayer == true) then
		if isAllGuestReady_2vs2() == false then
    		print("not all guests are ready!");
    	end	
    end
end

--是否准备好开始PK
function isAllGuestReady_2vs2()

    if pvp_RoomSeat[1]['seat_id'] > 0 and  pvp_RoomSeat[1]['bReady'] == false then
    	return false;
    end
    
    if  pvp_RoomSeat[2]['seat_id'] > 0 and pvp_RoomSeat[2]['bReady'] == false  then
    	return false;
    end
    
    if  pvp_RoomSeat[3]['seat_id'] > 0 and pvp_RoomSeat[3]['bReady'] == false then
    	return false;
    end

    if pvp_RoomSeat[1]['seat_id'] < 0  then
    	return false;
    end
    
    if pvp_RoomSeat[2]['seat_id'] < 0  then
    	return false;
    end
    
    if pvp_RoomSeat[3]['seat_id'] < 0  then
    	return false;
    end
    
    return true;
end

local bReady = false;
--准备
function onReady_2vs2(pObj)
	bReady = not bReady;
	MessageHandler:sShareInstance():pvp_setReady(bReady);
end

--开发PK
function onGo_2vs2(pObj)
   if isAllGuestReady_2vs2() == true then
         PushOneWaitingLayer("CSPVPStartReq");
       MessageHandler:sShareInstance():pvp_startGame();
   else
       local rstString = LanguageLocalization:GetLocalization("M_PVP_NOT_READY");
       GameApi:showMessage(rstString);
   end
end

--注册Go和Ready
btnReady_2vs2:getMenuItemSprite():registerScriptTapHandler(onReady_2vs2);
btnGo_2vs2:getMenuItemSprite():registerScriptTapHandler(onGo_2vs2);        

if PVP_bHostPlayer == true then
	btnReady_2vs2:setVisible(false);
	btnGo_2vs2:setVisible(true);
else   
	btnReady_2vs2:setVisible(true);
	btnGo_2vs2:setVisible(false);
end

-----------------------------------change seat of button message---------------------
function onChangeSeat0_2vs2(pObj)
end

function onChangeSeat1_2vs2(pObj)

    if pvp_RoomSeat[1]['seat_id'] > 0  or PVP_bHostPlayer == true then
    	return;
    end
    
    CCLuaLog("onChangeSeat1_2vs2");
    MessageHandler:sShareInstance():pvp_changeSeat(1);
end

function onChangeSeat2_2vs2(pObj)

    if pvp_RoomSeat[2]['seat_id'] > 0 or PVP_bHostPlayer == true then
    	return;
    end
    
    CCLuaLog("onChangeSeat2_2vs2");
    MessageHandler:sShareInstance():pvp_changeSeat(2);
end

function onChangeSeat3_2vs2(pObj)

    if pvp_RoomSeat[3]['seat_id'] > 0 or PVP_bHostPlayer == true then
    	return;
    end

    CCLuaLog("onChangeSeat3_2vs2");
    MessageHandler:sShareInstance():pvp_changeSeat(3);
end

local btnChangeSeat0_2vs2 = topLayout_2vs2:FindChildObjectByName("2vs2_waitButton0").__UIButton__;
btnChangeSeat0_2vs2:getMenuItemSprite():registerScriptTapHandler(onChangeSeat0_2vs2);	

local btnChangeSeat1_2vs2 = topLayout_2vs2:FindChildObjectByName("2vs2_waitButton1").__UIButton__;
btnChangeSeat1_2vs2:getMenuItemSprite():registerScriptTapHandler(onChangeSeat1_2vs2);	

local btnChangeSeat2_2vs2 = topLayout_2vs2:FindChildObjectByName("2vs2_waitButton2").__UIButton__;
btnChangeSeat2_2vs2:getMenuItemSprite():registerScriptTapHandler(onChangeSeat2_2vs2);	

local btnChangeSeat3_2vs2 = topLayout_2vs2:FindChildObjectByName("2vs2_waitButton3").__UIButton__;
btnChangeSeat3_2vs2:getMenuItemSprite():registerScriptTapHandler(onChangeSeat3_2vs2);	

------------------------------------kick off button message----------------------------
function onCloseGuest1_2vs2(pObj)
	MessageHandler:sShareInstance():pvp_kickOffFromRoom(1);
end

function onCloseGuest2_2vs2(pObj)
	MessageHandler:sShareInstance():pvp_kickOffFromRoom(2);
end

function onCloseGuest3_2vs2(pObj)
	MessageHandler:sShareInstance():pvp_kickOffFromRoom(3);
end

-- init Host Place 
local btKf1 = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest1").__UIButton__;
btKf1:getMenuItemSprite():registerScriptTapHandler(onCloseGuest1_2vs2);	

local btKf2 = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest2").__UIButton__;
btKf2:getMenuItemSprite():registerScriptTapHandler(onCloseGuest2_2vs2);

local btKf3 = topLayout_2vs2:FindChildObjectByName("2vs2_kickGuest3").__UIButton__;
btKf3:getMenuItemSprite():registerScriptTapHandler(onCloseGuest3_2vs2);
------------------------------------kick off button message----------------------------

InitHostPlace_2vs2(true);
UpdateGuestSeat_2vs2();
HideChatToolLayerAllItems();

CCLuaLog("pvp_2vs2 loaded!");

