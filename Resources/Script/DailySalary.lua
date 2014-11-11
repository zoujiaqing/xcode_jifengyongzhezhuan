

require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")

local function init()
    local player = TXGUI.UIManager:sharedManager():getUILayout("DailySalary");
    
    local label_title       = player:FindChildObjectByName("DAILYSALARY_TEXT_TITLE").__UILabel__;
    local icon              =  player:FindChildObjectByName("DAILYSALARY_ICON_COIN").__UIPicture__;
    local icon_num          = player:FindChildObjectByName("DAILYSALARY_NUM_COIN").__UILabel__;
    
    local label_discripton  = player:FindChildObjectByName("DAILYSALARY_TEXT_DESCRIPTION").__UILabel__;
    
    local btn_get           = player:FindChildObjectByName("DAILYSALARY_BUTTON_GET").__UIButton__:getMenuItemSprite();
    local label_get         = player:FindChildObjectByName("DAILYSALARY_TEXT_GET").__UILabel__;
    
    local btn_colse         = player:FindChildObjectByName("DAILYSALARY_BUTTON_CLOSE").__UIButton__:getMenuItemSprite();
    
    local strval = LanguageLocalization:GetLocalization("Dailyreceive_6000_name");
    label_title:setString(strval)
    
    strval = LanguageLocalization:GetLocalization("Dailytask_WaitReward");
    label_get:setString(strval)
    
    strval = LanguageLocalization:GetLocalization("M_DAILYSALARY_DESCRIPTION");
    label_discripton:setString(strval)
    
    btn_get:registerScriptTapHandler(onSalaryGetCloic);
    btn_colse:registerScriptTapHandler(onSalaryClosesClick);
end

function onSalaryGetCloic(pObj)
   print("onSalaryGetCloic")
   sendGetDailyRewardReq(68)
end

function onSalaryClosesClick(pObj)
   print("onSalaryClosesClick")
   BackToDailyReWard()
end

init()
print("DailySalary load!!");
