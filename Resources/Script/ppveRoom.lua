--[[
@file:ppveRoom.lua
@description 
    manager all the guest players and the host data, show the kick button, or ready information of guest
 
@author:
      jameshuang 
@date
--     2013/2/21
--]]

local topLayout = TXGUI.UIManager:sharedManager():getUILayout("ppveRoom");
closeBtn = topLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();
local btnReady = topLayout:FindChildObjectByName("readyBtn").__UIButton__;
local btnGo =   topLayout:FindChildObjectByName("goReadyBtn").__UIButton__;

local l_roomID = -1;
local l_topLevel = -1;

function InitRoomTitle(roomID, topLevel)
        print("roomID:"..roomID.."topLevel:"..topLevel);
        if   l_roomID  ~= roomID then
            l_roomID = roomID;
   			local label = topLayout:FindChildObjectByName("title").__UILabel__;
			label:setString( "房间"..roomID);
			label:setVisible(true);
		end
		
		if l_topLevel ~=topLevel then
		    l_topLevel = topLevel;
   			local label = topLayout:FindChildObjectByName("roomlevel").__UILabel__;
			label:setString( topLevel.."级副本");
			label:setVisible(true);    
		end
end

function onCloseBtnClicked(pObj)
	TXGUI.UIManager:sharedManager():removeUILayout("ppveRoom");
	HideChatToolLayerAllItems()
	ShowPPVEHall();
	MessageHandler:sShareInstance():exitRoom();
	MessageHandler:sShareInstance():getRoomList();
end
closeBtn:registerScriptTapHandler(onCloseBtnClicked);

-- hide close buttons
local headIcon={};

function GetheadIcon(character)
    
    print("character :"..character);
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

function ShowPlaceButtons(place, bShow)
		--- hide host kick button
   		local btnClose = topLayout:FindChildObjectByName("kickPlaceBtn"..place).__UIButton__;   		
  		if bShow == true and g_PPveHostPlayer == true then
  		         btnClose:setVisible(true);  		     
  		     else
  		         btnClose:setVisible(false);  		     
  		end 		
		
		local levelLabel = topLayout:FindChildObjectByName("levellabel"..place).__UILabel__;
		levelLabel:setVisible(bShow);
		levelLabel:setString( "LV  "..RoomSeat[place]['level']);
				
		-- wait label text
		local bWait = not bShow;
		local waitLabel =  topLayout:FindChildObjectByName("labelwait"..place).__UILabel__;
		waitLabel:setVisible(bWait);
		
		local nameLabel = topLayout:FindChildObjectByName("namelabel"..place).__UILabel__;
		nameLabel:setVisible(bShow);
		nameLabel:setString( RoomSeat[place]['name']);
		
		local percentLabel = topLayout:FindChildObjectByName("percentlabel"..place).__UILabel__;
		percentLabel:setVisible(false);
		
		local networkLabel = topLayout:FindChildObjectByName("networklabel"..place).__UILabel__;
		networkLabel:setVisible(false);
		
		--- hide head icon if no guest seat on this chair
		print("RoomSeat:["..place.."]:"..RoomSeat[place]['seat_id']);
		
		if ( RoomSeat[place]['seat_id'] < 0) then
		    if headIcon[place] ~=nil then 
		        print("remove icon"..place);
			 	headIcon[place]:removeFromParentAndCleanup(true);
			 	headIcon[place] = nil;			 	
			end	
					
		else
		    if (headIcon[place] == nil) then
            
		    	local placePic = topLayout:FindChildObjectByName("place"..place).__UIPicture__:getCurrentNode();	    
		    	--- have one guest here!
		    	local icon = GetheadIcon( RoomSeat[place]['Avatar']);		
				icon:setPosition(154, 146);			
		    	placePic:addChild(icon);			
				headIcon[place] = icon;	
				
			    print("add icon"..place);
		    end	
		end
end

function  InitHostPlace(bHost)
	ShowPlaceButtons(1, false);
	ShowPlaceButtons(2, false);
	ShowPlaceButtons(3, false);
	
	--- hide host kick button	
	local iconHost  =  topLayout:FindChildObjectByName("iconHost").__UIPicture__;
	iconHost:setVisible(bHost);
	
	local nameLabel = topLayout:FindChildObjectByName("namelabel").__UILabel__;
	nameLabel:setString( RoomSeat[0]['name']);
	print("Host seat 0 : [".. RoomSeat[0]['name'].."]-"..RoomSeat[0]['Avatar']);	
    
	local levelLabel = topLayout:FindChildObjectByName("levellabel").__UILabel__;
	levelLabel:setString( "LV  "..RoomSeat[0]['level']);


		
	local placePic = topLayout:FindChildObjectByName("place0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon( RoomSeat[0]['Avatar']);		
	icon:setPosition(154, 146);	

	--去除房主的网络和胜率提示
	local percentLabel = topLayout:FindChildObjectByName("percentlabel").__UILabel__;
	percentLabel:setVisible(false);
		
	local networkLabel = topLayout:FindChildObjectByName("networklabel").__UILabel__;
	networkLabel:setVisible(false);
   
    placePic:removeChildByTag(100,true);		
    placePic:addChild(icon,0,100);		
end

--- update guest seat information
function UpdateGuestSeat()

    CCLuaLog("PPVE UpdateGuestSeat!");
	local nameLabel = topLayout:FindChildObjectByName("namelabel").__UILabel__;
	nameLabel:setString( RoomSeat[0]['name']);
	
	local levelLabel = topLayout:FindChildObjectByName("levellabel").__UILabel__;
	levelLabel:setString( "LV  "..RoomSeat[0]['level']);  

	local placePic = topLayout:FindChildObjectByName("place0").__UIPicture__:getCurrentNode();
	local icon = GetheadIcon( RoomSeat[0]['Avatar']);		
	icon:setPosition(154, 146);
    
    placePic:removeChildByTag(100,true);
    placePic:addChild(icon,0,100);

	for i=1,3,1 do
 		if RoomSeat[i]['seat_id'] > 0 then
    		ShowPlaceButtons(i, true);
    	else
    		ShowPlaceButtons(i, false);
		end
	end
    
	for i=1,3,1 do
		if ( RoomSeat[i]['bReady'] == true) then
		        
    			local icon  =  topLayout:FindChildObjectByName("readyPic"..i).__UIPicture__;
	            icon:setVisible(true);
    		else
    
    			local icon  =  topLayout:FindChildObjectByName("readyPic"..i).__UIPicture__;
	            icon:setVisible(false);
		end		
	end	

	
	if (g_PPveHostPlayer == true) then
		tryEnableGoButton();
    end

    UpdatePPVEFightNum()
end

function UpdatePPVEFightNum()
    CCLuaLog("-------------------- UpdatePPVEFightNum -------------------")
    local roomLayout = TXGUI.UIManager:sharedManager():getUILayout("ppveRoom")

    if roomLayout == nil then
        return
    end

    local hostfightLabel = roomLayout:FindChildObjectByName("hostFightNum").__UILabel__
    hostfightLabel:setString(""..GetLuaLocalization("M_MSROOM_FIGHT")..RoomSeat[0]['fightNum'])


    for i = 1, 3 do
        local fightLabel = roomLayout:FindChildObjectByName("guestFightNum"..i).__UILabel__
        if RoomSeat[i]['seat_id'] > 0 then
            fightLabel:setString(""..GetLuaLocalization("M_MSROOM_FIGHT")..RoomSeat[i]['fightNum'])
        else
            fightLabel:setString("")
        end        
    end
end


--[[
 @function, try to enable or disable go button
 @param,-void
 @note,
       try to enable or disable the go button, when only host , disable go button
 
]]--

function getGuestCount()
    local people = 0;
    for i=1, 3,1  do 
    	if (RoomSeat[i]['seat_id'] > 0) then
    		people = people + 1;
    	end
    end
	return people;
end

function tryEnableGoButton()
    -- 修改 一个人也能玩ppve 
   	--if (getGuestCount() > 0) and (isAllGuestReady()) then

	if isAllGuestReady() then
   		btnGo:setEnabled(true);
   	else
   	    btnGo:setEnabled(true);
   	end
end

function isAllGuestReady()

    if RoomSeat[1]['seat_id'] > 0 and  RoomSeat[1]['bReady'] == false then
    	return false;
    end
    
    if  RoomSeat[2]['seat_id'] > 0 and RoomSeat[2]['bReady'] == false  then
    	return false;
    end
    
    if  RoomSeat[3]['seat_id'] > 0 and RoomSeat[3]['bReady'] == false then
    	return false;
    end
	return true; 
end 


local bReady = false;

function onReady(pObj)
	-- send ready room 
	bReady = not bReady;
	MessageHandler:sShareInstance():setReady(bReady);
end

function onPPVEGo(pObj)
   CCLuaLog("ppveRoom onGo")
   if isAllGuestReady() == true then
   		-- send start game 
        print("start go")
		PushOneWaitingLayer("CSPVEStartReq");
		MessageHandler:sShareInstance():startGame();
    else
       CCLuaLog("start not go")
       local rstString = LanguageLocalization:GetLocalization("M_PVP_NOT_READY");
       GameApi:showMessage(rstString);
   end
end

------------------------------------kick off button message----------------------------
function onCloseGuest1(pObj)
	MessageHandler:sShareInstance():kickOffFromRoom(1);
end

function onCloseGuest2(pObj)
	MessageHandler:sShareInstance():kickOffFromRoom(2);
end

function onCloseGuest3(pObj)
	MessageHandler:sShareInstance():kickOffFromRoom(3);
end

-- init Host Place 
local btKf1 = topLayout:FindChildObjectByName("kickPlaceBtn1").__UIButton__;  
btKf1:getMenuItemSprite():registerScriptTapHandler(onCloseGuest1);	

local btKf2 = topLayout:FindChildObjectByName("kickPlaceBtn2").__UIButton__;  
btKf2:getMenuItemSprite():registerScriptTapHandler(onCloseGuest2);

local btKf3 = topLayout:FindChildObjectByName("kickPlaceBtn3").__UIButton__;  
btKf3:getMenuItemSprite():registerScriptTapHandler(onCloseGuest3);
------------------------------------kick off button message----------------------------

InitHostPlace( true);
        
btnReady:getMenuItemSprite():registerScriptTapHandler(onReady);
btnGo:getMenuItemSprite():registerScriptTapHandler(onPPVEGo);
        
if g_PPveHostPlayer == true then
	btnReady:setVisible(false);
	btnGo:setVisible(true);
else   
	btnReady:setVisible(true);
	btnGo:setVisible(false);
end

UpdateGuestSeat();

--------------------------------------- called by C++---------------------------------------------
function closePPVERoom()
   --- exit current room and go back to the hall
   -- this may happen when host close the room 
   CCLuaLog("close ppve room now!");   
   
   ---go to game hall 
   if (g_PPveHostPlayer == false) then
   		-- get current UI Layout and remove it from the GUI system
		TXGUI.UIManager:sharedManager():removeUILayout("ppveRoom");
        HideChatToolLayerAllItems()
		ShowPPVEHall();	
	    MessageHandler:sShareInstance():getRoomList();
	end
end

CCLuaLog("PPVE ROOM!");

