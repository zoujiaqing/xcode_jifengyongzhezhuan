

pvpS1Layout = TXGUI.UIManager:sharedManager():getUILayout("pvp_s1");
local confirmBtn = pvpS1Layout:FindChildObjectByName("ok_btn").__UIButton__:getMenuItemSprite();
local cancleBtn  = pvpS1Layout:FindChildObjectByName("cancle_btn").__UIButton__:getMenuItemSprite();

--local Btn_1vs1 = pvpS1Layout:FindChildObjectByName("1vs1_btn").__UIButtonToggle__;
--local Btn_2vs2 = pvpS1Layout:FindChildObjectByName("2vs2_btn").__UIButtonToggle__;
--local Btn_protect = pvpS1Layout:FindChildObjectByName("protect_btn").__UIButton__:getMenuItemSprite();
--local Btn_free = pvpS1Layout:FindChildObjectByName("free_btn").__UIButton__:getMenuItemSprite();

local bPVPChoose_1vs1 = true;
local bPVPChoose_2vs2 = false;
local bPVPChoose_protect = true;
local bPVPChoose_free = false;

--1v1 or 2v2
g_PVPCount = 1

--保护 or 自由
g_PVPType = 1

-- 房间等级
g_PVPTopLevel = 40



--选择1vs1
function onBtn1vs1(pObj)
    CCLuaLog("onBtn1vs1");    
    PVPType = 1;
    bPVPChoose_1vs1 = true;
    bPVPChoose_2vs2 = false;

	g_PVPCount = 1
end

--选择2vs2
function onBtn2vs2(pObj)
    PVPType = 2;
    CCLuaLog("onBtn2vs2");
    bPVPChoose_1vs1 = false;
    bPVPChoose_2vs2 = true;

	g_PVPCount = 2
end

--选择保护
function onBtnProtect(pObj)
    CCLuaLog("onBtnProtect");
    bPVPChoose_protect = true;
    bPVPChoose_free = false;

	g_PVPType = 1
end

--选择自由
function onBtnFree(pObj)
    CCLuaLog("onBtnFree");
    bPVPChoose_protect = false;
    bPVPChoose_free = true;

	g_PVPType = 2
end

--关闭房间
function onClosePVPRoom(pObj)
    CCLuaLog("onClosePVPRoom");

    --send leam room message
	--MessageHandler:sShareInstance():pvp_exitRoom();
	--MessageHandler:sShareInstance():pvp_getRoomList();

    --if PVPType == 1 then
        --TXGUI.UIManager:sharedManager():removeUILayout("pvp_1vs1");
    --else
       -- TXGUI.UIManager:sharedManager():removeUILayout("pvp_2vs2");
    --end
    
   -- PVP_bInRoom = false;
    --pvpLayout:setVisibleInStyle(true);
   -- ShowPVPHall();
end

--确定选择
function onConfirm(pObj)

    PVP_bInRoom = true;
    PVP_bHostPlayer = true;
    --TXGUI.UIManager:sharedManager():removeUILayout("pvp_s1");
    pvpS1Layout:setVisibleInStyle(false);
    PVP_bS1 = false;
    
    local pvpRoom;
    local str_closebtn;
    if bPVPChoose_1vs1 then
        CCLuaLog("onConfirm! create room 1vs1");
        pvpRoom  = TXGUI.UIManager:sharedManager():createUILayoutFromParent("UIplist/pvp_1vs1.plist", pvpLayout, "pvp_1vs1", false);
        str_closebtn = "closeBtn_1vs1";
        MessageHandler:sShareInstance():pvp_createRoom_1vs1();
        CCLuaLog("pvp_s1 on Confirm! go to 1vs1");
        
    else
    
        CCLuaLog("onConfirm! create room 2vs2");
        pvpRoom  = TXGUI.UIManager:sharedManager():createUILayoutFromParent("UIplist/pvp_2vs2.plist", pvpLayout, "pvp_2vs2", false);
        str_closebtn = "closeBtn_2vs2";
        MessageHandler:sShareInstance():pvp_createRoom_2vs2();
        CCLuaLog("pvp_s1 on Confirm! go to 2vs2");
    end
    
    pvpRoom:setVisibleInStyle(true);
    pvpLayout:setVisible(false);
    
    --local closeBtn = pvpRoom:FindChildObjectByName(str_closebtn).__UIButton__:getMenuItemSprite();
    --closeBtn:registerScriptTapHandler(onClosePVPRoom);
    --HidePPVEButtons(); 
end

--取消选择
function onCancle(pObj)
    --TXGUI.UIManager:sharedManager():removeUILayout("pvp_s1");
    pvpS1Layout:setVisibleInStyle(false);
    PVP_bS1 = false;
end

--注册Button call back function
CCLuaLog("pvp_s1 loaded!");
confirmBtn:registerScriptTapHandler(onConfirm);
cancleBtn:registerScriptTapHandler(onCancle);

--Btn_1vs1:registerScriptTapHandler(onBtn1vs1);
--Btn_2vs2:registerScriptTapHandler(onBtn2vs2);
--Btn_protect:registerScriptTapHandler(onBtnProtect);
--Btn_free:registerScriptTapHandler(onBtnFree);





