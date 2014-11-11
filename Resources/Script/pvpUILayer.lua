
-- bind create room button
pvpLayout = TXGUI.UIManager:sharedManager():getUILayout("pvpUI");

local chatToolBarLayout = TXGUI.UIManager:sharedManager():getUILayout("chatToolbar");
local createBtn = chatToolBarLayout:FindChildObjectByName("createButton").__UIButton__:getMenuItemSprite();
local enterBtn  = chatToolBarLayout:FindChildObjectByName("EnterButton").__UIButton__:getMenuItemSprite();

local timeLabel = pvpLayout:FindChildObjectByName("timeremain").__UILabel__;
local closeButton = pvpLayout:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();

PVPType = 1;
PVP_bHostPlayer = false;
PVP_bInRoom = false;
PVP_bS1 = false;

-----------------------------------------------------update function ------------------------------------------
local lastUpdateTime = 0;
local myRemain_time = 3600;
local myCountdown = 20;
function  pvpUILayerUpdate(dt)
    --print("update script now!!!!");   
    
    if TXGUI.UIManager:sharedManager():getUILayout("pvpUI") == nil then
        CCLuaLog("pvpUILayerUpdate!");
        onPVPClose(nil);
        return;
    end      
    
    myRemain_time = myRemain_time -1;
    --- update remain time 
    local h =math.floor(myRemain_time / 3600);
    local m = math.floor((myRemain_time - h * 3600) / 60);
    local s = myRemain_time - h * 3600 - m * 60;

    local timeStr = ""..h;
    
    if (m < 10) then
       timeStr = timeStr..":0"..m;
    else
       timeStr = timeStr..":"..m;
    end
    
    if (s < 10) then
       timeStr = timeStr..":0"..s;
      else
       timeStr = timeStr..":"..s;    
    end

    --timeLabel:setString(timeStr);

    lastUpdateTime = lastUpdateTime + 1;    
    
    -- update list
    if (lastUpdateTime > 5) then
        -- query room list every 10 seconds
        MessageHandler:sShareInstance():pvp_getRoomList();
        lastUpdateTime = 0;
    end 
end

--- register update function 
local updateFun = nil --CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(pvpUILayerUpdate, 1.0, false);
--------------------------------------------------------------------------------------------------------------------

--关闭按钮
function onPVPClose(pObj)
    if updateFun ~= nil then
        CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFun);
    end
    
    -- send leam room message
    MessageHandler:sShareInstance():pvp_exitRoom();
    CCLuaLog("onPVPClose!");
end

--创建房间
function onPVPCreateRoom(pObj)

    local pvp1vs1_plist = TXGUI.UIManager:sharedManager():getUILayout("pvp_1vs1");
    local pvp2vs2_plist = TXGUI.UIManager:sharedManager():getUILayout("pvp_2vs2");
    
    if pvp1vs1_plist ~= nil or pvp2vs2_plist ~= nil then
       return;
    end
    
    if PVP_bS1 ~= true then
       CCLuaLog("onPVPCreateRoom!");
       if updateFun ~= nil then
            CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFun);
       end
       
       local pvps1_plist = TXGUI.UIManager:sharedManager():getUILayout("pvp_s1");
       if pvps1_plist == nil then
            pvps1_plist = TXGUI.UIManager:sharedManager():createUILayoutFromParent("UIplist/pvp_s1.plist", pvpLayout, "pvp_s1", false);
       end
       pvps1_plist:setVisible(true);
       PVP_bS1 = true;
    end
end

--快速加入
function onPVPEnterRoom(pObj)    
   if PVP_bS1 ~= true then
     MessageHandler:sShareInstance():pvp_joinRoom(0);
   end
end

--获取房间列表
MessageHandler:sShareInstance():pvp_getRoomList();

--
pvp_roomData = {};
pvp_roomData[0] ={ room_id = 1, top_level = 1, current_online = 2, max_online = 4};

------------------------------------------------------ called by C++--------------------------------------
function UpdatePVPPage()
    CCLuaLog("UpdatePVPPage!");
   	pvp_roomData = MessageHandler:sShareInstance():pvp_getRoomData();
end

---pvp座位信息, 全局
pvp_RoomSeat={};

pvp_RoomSeat[0]={seat_id= -1, level= 0, name="", Avatar=0, bReady=false,};
pvp_RoomSeat[1]={seat_id= -1, level= 0, name="", Avatar=0, bReady=false,};
pvp_RoomSeat[2]={seat_id= -1, level= 0, name="", Avatar=0, bReady=false,};
pvp_RoomSeat[3]={seat_id= -1, level= 0, name="", Avatar=0, bReady=false,};

--设置玩家座位信息
function pvp_PlayerOnSeat(seatID, playerdata)
    print("pvp PlayerOnSeat:"..seatID.."Avatar:"..playerdata['Avatar']);
    
    pvp_RoomSeat[seatID]['seat_id'] = playerdata['seat_id'];
    pvp_RoomSeat[seatID]['level']   = playerdata['level'];
    pvp_RoomSeat[seatID]['name']    = playerdata['name'];
    pvp_RoomSeat[seatID]['Avatar']  = playerdata['Avatar'];
    pvp_RoomSeat[seatID]['bReady']  = playerdata['bReady'];   
    
    print("name:"..pvp_RoomSeat[seatID]['name']);
end

--玩家离开座位
function  pvp_PlayerLeaveSeat(seatID)
     print("PlayerLeaveSeat:"..seatID);
     pvp_RoomSeat[seatID]['seat_id'] = -1;
     pvp_RoomSeat[seatID]['bReady'] = false;
end

---显示当前的UI layout
function ShowPVPHall()
    pvpLayout:setVisibleInStyle(true);   
    PVP_bInRoom = false;
    ShowPPVELayerItems();
    if nil ~= updateFun then
        CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFun);
    end
    print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 7")
    updateFun = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(pvpUILayerUpdate, 1.0, false);
end 

--------------------------------------------------- called by c++ ----------------------------------------
---- when guest leave or set ready or not, this funciton will be called by C++
function pvp_EnterRoom(roomID, topLevel, type) 
    CCLuaLog("pvp_EnterRoom!");
    local pvpRoom;
    PVPType = type;
    g_PVPTopLevel = topLevel
    if PVP_bInRoom == false then
        CCLuaLog("pvp_EnterRoom! PVP_bInRoom == false");
        PVP_bInRoom = true;
        if nil ~= updateFun then
            CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(updateFun);
        end
        
        -- set myself guest
   		PVP_bHostPlayer = false;
        
        if PVPType == 1 then
            CCLuaLog("pvp_EnterRoom 1vs1!");
    	    pvpRoom = TXGUI.UIManager:sharedManager():createUILayoutFromParent("UIplist/pvp_1vs1.plist",pvpLayout,"pvp_1vs1",false);
            InitRoomTitle_1vs1(roomID, topLevel);
        else
            CCLuaLog("pvp_EnterRoom 2vs2!");
            pvpRoom = TXGUI.UIManager:sharedManager():createUILayoutFromParent("UIplist/pvp_2vs2.plist",pvpLayout,"pvp_2vs2",false);
            InitRoomTitle_2vs2(roomID, topLevel);
        end

    	pvpRoom:setVisibleInStyle(true);
    	pvpLayout:setVisible(false);
    	--HidePPVEButtons();  
        
    else
        CCLuaLog("pvp_EnterRoom!  PVP_bInRoom == true");
        if PVPType == 1 then
            CCLuaLog("pvp_EnterRoom! PVP_bInRoom == true  go to 1vs1");
            UpdateGuestSeat_1vs1();
            InitRoomTitle_1vs1(roomID);
        else
            CCLuaLog("pvp_EnterRoom! PVP_bInRoom == true  go to 2vs2");
            UpdateGuestSeat_2vs2();
            InitRoomTitle_2vs2(roomID); 
        end
    end
end

CCLuaLog("pvpUILayer loaded!");
closeButton:registerScriptTapHandler(onPVPClose);
createBtn:registerScriptTapHandler(onPVPCreateRoom);
enterBtn:registerScriptTapHandler(onPVPEnterRoom);

--------------------------------------- called by C++---------------------------------------------
function closePVPRoom()
   CCLuaLog("lua closePVPRoom");   
   ---go to game hall 
   if (PVP_bHostPlayer == false) then
   
        if PVPType == 1 then 
           TXGUI.UIManager:sharedManager():removeUILayout("pvp_1vs1");
        else 
           TXGUI.UIManager:sharedManager():removeUILayout("pvp_2vs2");
        end

		ShowPVPHall();	
	    MessageHandler:sShareInstance():pvp_getRoomList();
	end
end

---- kick off by Host
function pvp_kickOffFromRoom()
     CCLuaLog("pvp_kickOffFromRoom!");
     
     if PVPType == 1 then
        onCloseBtnClicked_1vs1("");
     else
        onCloseBtnClicked_2vs2("");
     end
end






