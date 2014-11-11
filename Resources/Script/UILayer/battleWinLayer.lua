CCLuaLog("load battleWinLayer.lua");

local battleWinLayout = TXGUI.UIManager:sharedManager():getUILayout("BattleWin");
local backBtn = battleWinLayout:FindChildObjectByName("backToTown").__UIButton__:getMenuItemSprite();

function onWinToBack(obj)
	CCLuaLog(" battleWinLayer.lua : onWinToBack");
	--CCDirector:sharedDirector():getScheduler():unscheduleScriptEntry(battleLayerUpdateFun);
end

backBtn:registerScriptTapHandler(onWinToBack);