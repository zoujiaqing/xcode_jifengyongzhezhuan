

require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/GameConfig/uiTagDefine")
require("Script/protobuf")

function DailyLogin_init()

    local player = TXGUI.UIManager:sharedManager():getUILayout("DailyLogin");
    if player == nil then
       return 
    end
    
    local strday1 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY1");
    local strday2 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY2");
    local strday3 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY3");
    local strday4 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY4");
    local strday5 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY5");
    local strday6 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY6");
    local strday7 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY7");
    local strday8 = LanguageLocalization:GetLocalization("M_LOGINREWARD_DAY7");
    
    -------1------
    local label_day1 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY1").__UILabel__;
    local label_11 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD1").__UILabel__;
    local label_12 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD2").__UILabel__;
    
    local label_13 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD3").__UILabel__;
    local label_14 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD4").__UILabel__;
    local label_btntext1 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD1").__UILabel__;
    
    local uipictureHead_1 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY1").__UIPicture__;
    local uipicture11 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD1").__UIPicture__;
    local uipicture12 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD2").__UIPicture__;
    local uipicture13 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD3").__UIPicture__;
    local uipicture14 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD4").__UIPicture__;

    -------2------
    local label_day2 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY2").__UILabel__;
    local label_21 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD1").__UILabel__;
    local label_22 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD2").__UILabel__;
    local label_23 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD3").__UILabel__;
    local label_24 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD4").__UILabel__;
    local label_btntext2 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD2").__UILabel__;
    
    local uipictureHead_2 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY2").__UIPicture__;
    local uipicture21 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD1").__UIPicture__;
    local uipicture22 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD2").__UIPicture__;
    local uipicture23 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD3").__UIPicture__;
    local uipicture24 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD4").__UIPicture__;
    
    -------3------
    local label_day3 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY3").__UILabel__;
    local label_31 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD1").__UILabel__;
    local label_32 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD2").__UILabel__;
    local label_33 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD3").__UILabel__;
    local label_34 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD4").__UILabel__;
    local label_btntext3 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD3").__UILabel__;
    
    local uipictureHead_3 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY3").__UIPicture__;
    local uipicture31 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD1").__UIPicture__;
    local uipicture32 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD2").__UIPicture__;
    local uipicture33 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD3").__UIPicture__;
    local uipicture34 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD4").__UIPicture__;
    
    -------4------
    local label_day4 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY4").__UILabel__;
    local label_41 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD1").__UILabel__;
    local label_42 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD2").__UILabel__;
    local label_43 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD3").__UILabel__;
    local label_44 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD4").__UILabel__;
    local label_btntext4 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD4").__UILabel__;

    local uipictureHead_4 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY4").__UIPicture__;
    local uipicture41 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD1").__UIPicture__;
    local uipicture42 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD2").__UIPicture__;
    local uipicture43 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD3").__UIPicture__;
    local uipicture44 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD4").__UIPicture__;
    
    -------5------
    local label_day5 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY5").__UILabel__;
    local label_51 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD1").__UILabel__;
    local label_52 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD2").__UILabel__;
    local label_53 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD3").__UILabel__;
    local label_54 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD4").__UILabel__;
    local label_btntext5 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD5").__UILabel__;
    
    local uipictureHead_5 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY5").__UIPicture__;
    local uipicture51 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD1").__UIPicture__;
    local uipicture52 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD2").__UIPicture__;
    local uipicture53 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD3").__UIPicture__;
    local uipicture54 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD4").__UIPicture__;

    -------6------
    local label_day6 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY6").__UILabel__;
    local label_61 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD1").__UILabel__;
    local label_62 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD2").__UILabel__;
    local label_63 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD3").__UILabel__;
    local label_64 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD4").__UILabel__;
    local label_btntext6 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD6").__UILabel__;
    
    local uipictureHead_6 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY6").__UIPicture__;
    local uipicture61 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD1").__UIPicture__;
    local uipicture62 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD2").__UIPicture__;
    local uipicture63 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD3").__UIPicture__;
    local uipicture64 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD4").__UIPicture__;

    -------7------
    local label_day7 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY7").__UILabel__;
    local label_71 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD1").__UILabel__;
    local label_72 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD2").__UILabel__;
    local label_73 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD3").__UILabel__;
    local label_74 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD4").__UILabel__;
    local label_btntext7 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD7").__UILabel__;
    
    local uipictureHead_7 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY7").__UIPicture__;
    local uipicture71 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD1").__UIPicture__;
    local uipicture72 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD2").__UIPicture__;
    local uipicture73 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD3").__UIPicture__;
    local uipicture74 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD4").__UIPicture__;

    -------8------
    local label_day8 = player:FindChildObjectByName("LOGINREWARD_TEXT_REWARDDAY8").__UILabel__;
    local label_81 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD1").__UILabel__;
    local label_82 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD2").__UILabel__;
    local label_83 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD3").__UILabel__;
    local label_84 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD4").__UILabel__;
    local label_btntext8 = player:FindChildObjectByName("LOGINREWARD_TEXT_GETREWARD8").__UILabel__;
    
    local uipictureHead_8 = player:FindChildObjectByName("LOGINREWARD_ICON_REWARDDAY8").__UIPicture__;
    local uipicture81 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD1").__UIPicture__;
    local uipicture82 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD2").__UIPicture__;
    local uipicture83 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD3").__UIPicture__;
    local uipicture84 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD4").__UIPicture__;
    
    local bntDay1 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD1").__UIButton__:getMenuItemSprite();
    local bntDay2 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD2").__UIButton__:getMenuItemSprite();
    local bntDay3 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD3").__UIButton__:getMenuItemSprite();
    local bntDay4 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD4").__UIButton__:getMenuItemSprite();
    local bntDay5 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD5").__UIButton__:getMenuItemSprite();
    local bntDay6 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD6").__UIButton__:getMenuItemSprite();
    local bntDay7 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD7").__UIButton__:getMenuItemSprite();
    local bntDay8 = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD8").__UIButton__:getMenuItemSprite();
    
    local btnCloseBtn = player:FindChildObjectByName("loginCloseBtn").__UIButton__:getMenuItemSprite();

    bntDay1:registerScriptTapHandler(onDailyLoginGotDay1Click);
    bntDay2:registerScriptTapHandler(onDailyLoginGotDay2Click);
    bntDay3:registerScriptTapHandler(onDailyLoginGotDay3Click);
    bntDay4:registerScriptTapHandler(onDailyLoginGotDay4Click);
    bntDay5:registerScriptTapHandler(onDailyLoginGotDay5Click);
    bntDay6:registerScriptTapHandler(onDailyLoginGotDay6Click);
    bntDay7:registerScriptTapHandler(onDailyLoginGotDay7Click);
    bntDay8:registerScriptTapHandler(onDailyLoginGotDay8Click);
    btnCloseBtn:registerScriptTapHandler(onDailyLoginClosesClick);
    
    label_day1:setString(strday1)
    label_day2:setString(strday2)
    label_day3:setString(strday3)
    label_day4:setString(strday4)
    label_day5:setString(strday5)
    label_day6:setString(strday6)
    label_day7:setString(strday7)
    label_day8:setString(strday8)
    
    label_day1:setColor(ccc3(255,255,255))
    label_day2:setColor(ccc3(255,255,255))
    label_day3:setColor(ccc3(255,255,255))
    label_day4:setColor(ccc3(255,255,255))
    label_day5:setColor(ccc3(255,255,255))
    label_day6:setColor(ccc3(255,255,255))
    label_day7:setColor(ccc3(255,255,255))
    label_day8:setColor(ccc3(255,255,255))
    
    label_btntext1:setColor(ccc3(255,255,255))
    label_btntext2:setColor(ccc3(255,255,255))
    label_btntext3:setColor(ccc3(255,255,255))
    label_btntext4:setColor(ccc3(255,255,255))
    label_btntext5:setColor(ccc3(255,255,255))
    label_btntext6:setColor(ccc3(255,255,255))
    label_btntext7:setColor(ccc3(255,255,255))

    
    setDimandAndCoin(uipicture13, uipicture14)
    setDimandAndCoin(uipicture23, uipicture24)
    setDimandAndCoin(uipicture33, uipicture34)
    setDimandAndCoin(uipicture43, uipicture44)
    setDimandAndCoin(uipicture53, uipicture54)
    setDimandAndCoin(uipicture63, uipicture64)
    setDimandAndCoin(uipicture73, uipicture74)
    setDimandAndCoin(uipicture83, uipicture84)
    
    
    label_11:setVisible(false)
    label_12:setVisible(false)
    
    label_21:setVisible(false)
    label_22:setVisible(false)

    label_31:setVisible(false)
    label_32:setVisible(false)

    label_41:setVisible(false)
    label_42:setVisible(false)
    
    label_51:setVisible(false)
    label_52:setVisible(false)

    label_61:setVisible(false)
    label_62:setVisible(false)

    label_71:setVisible(false)
    label_72:setVisible(false)
    
    label_81:setVisible(false)
    label_82:setVisible(false)

    setIconReward(uipictureHead_1, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_2, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_3, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_4, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_5, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_6, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_7, "map_ui_system_icon_FRAME_ICON_REWARD")
    setIconReward(uipictureHead_8, "map_ui_system_icon_FRAME_ICON_VIP")

    local strGot = LanguageLocalization:GetLocalization("M_DAILYREWARD_ALREADYGOT");
    local strWait = LanguageLocalization:GetLocalization("M_DAILYREWARD_WAITFORGET");

    local continueloginTime = UserData:GetUserInfo().m_continueloginTime
    
    continueloginTime = continueloginTime % 7 + 1

    
    local bntDay1Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD1").__UIButton__
    local bntDay2Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD2").__UIButton__
    local bntDay3Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD3").__UIButton__
    local bntDay4Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD4").__UIButton__
    local bntDay5Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD5").__UIButton__
    local bntDay6Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD6").__UIButton__
    local bntDay7Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD7").__UIButton__
    local bntDay8Btn = player:FindChildObjectByName("LOGINREWARD_BUTTON_GETREWARD8").__UIButton__

    if continueloginTime == 1 then
       label_btntext1:setString(strWait)
       label_btntext2:setString(strWait)
       label_btntext3:setString(strWait)
       label_btntext4:setString(strWait)
       label_btntext5:setString(strWait)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(true)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(false)

       
    elseif continueloginTime == 2 then
    
       label_btntext1:setString(strGot)
       label_btntext2:setString(strWait)
       label_btntext3:setString(strWait)
       label_btntext4:setString(strWait)
       label_btntext5:setString(strWait)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(true)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(false)
       
       
    elseif continueloginTime == 3 then
    
       label_btntext1:setString(strGot)
       label_btntext2:setString(strGot)
       label_btntext3:setString(strWait)
       label_btntext4:setString(strWait)
       label_btntext5:setString(strWait)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(true)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(false)
    
    elseif continueloginTime == 4 then

       label_btntext1:setString(strGot)
       label_btntext2:setString(strGot)
       label_btntext3:setString(strGot)
       label_btntext4:setString(strWait)
       label_btntext5:setString(strWait)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(true)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(false)
       
    elseif continueloginTime == 5 then
    
       label_btntext1:setString(strGot)
       label_btntext2:setString(strGot)
       label_btntext3:setString(strGot)
       label_btntext4:setString(strGot)
       label_btntext5:setString(strWait)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(true)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(false)
    
    elseif continueloginTime == 6 then
    
       label_btntext1:setString(strGot)
       label_btntext2:setString(strGot)
       label_btntext3:setString(strGot)
       label_btntext4:setString(strGot)
       label_btntext5:setString(strGot)
       label_btntext6:setString(strWait)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(true)
       bntDay7Btn:setEnabled(false)
    
    elseif continueloginTime == 7 then
    
       label_btntext1:setString(strGot)
       label_btntext2:setString(strGot)
       label_btntext3:setString(strGot)
       label_btntext4:setString(strGot)
       label_btntext5:setString(strGot)
       label_btntext6:setString(strGot)
       label_btntext7:setString(strWait)
       
       bntDay1Btn:setEnabled(false)
       bntDay2Btn:setEnabled(false)
       bntDay3Btn:setEnabled(false)
       bntDay4Btn:setEnabled(false)
       bntDay5Btn:setEnabled(false)
       bntDay6Btn:setEnabled(false)
       bntDay7Btn:setEnabled(true)
    end
    
    setDailyLogin_DC()
end

function setDimandAndCoin(daimaond, icon)
    local diamondSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local iconSprite = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    daimaond:setSprite(diamondSprite)
    icon:setSprite(iconSprite)
end

function setIconReward(reward, frameRes)
    local sprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",frameRes)
    reward:setSprite(sprite)
end

function setDailyLogin_DC()

    local player = TXGUI.UIManager:sharedManager():getUILayout("DailyLogin");
    local uipicture11 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD1").__UIPicture__;
    local uipicture12 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD2").__UIPicture__;
    local uipicture13 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD3").__UIPicture__;
    local uipicture14 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY1_REWARD4").__UIPicture__;
    
    local label_11 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD1").__UILabel__;
    local label_12 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD2").__UILabel__;
    local label_13 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD3").__UILabel__;
    local label_14 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY1_REWARD4").__UILabel__;
    
    local uipicture21 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD1").__UIPicture__;
    local uipicture22 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD2").__UIPicture__;
    local uipicture23 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD3").__UIPicture__;
    local uipicture24 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY2_REWARD4").__UIPicture__;
    
    local label_21 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD1").__UILabel__;
    local label_22 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD2").__UILabel__;
    local label_23 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD3").__UILabel__;
    local label_24 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY2_REWARD4").__UILabel__;
    
    local uipicture31 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD1").__UIPicture__;
    local uipicture32 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD2").__UIPicture__;
    local uipicture33 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD3").__UIPicture__;
    local uipicture34 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY3_REWARD4").__UIPicture__;
    
    local label_31 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD1").__UILabel__;
    local label_32 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD2").__UILabel__;
    local label_33 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD3").__UILabel__;
    local label_34 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY3_REWARD4").__UILabel__;
    
    local uipicture41 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD1").__UIPicture__;
    local uipicture42 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD2").__UIPicture__;
    local uipicture43 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD3").__UIPicture__;
    local uipicture44 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY4_REWARD4").__UIPicture__;
    
    local label_41 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD1").__UILabel__;
    local label_42 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD2").__UILabel__;
    local label_43 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD3").__UILabel__;
    local label_44 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY4_REWARD4").__UILabel__;
    
    local uipicture51 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD1").__UIPicture__;
    local uipicture52 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD2").__UIPicture__;
    local uipicture53 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD3").__UIPicture__;
    local uipicture54 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY5_REWARD4").__UIPicture__;
    
    local label_51 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD1").__UILabel__;
    local label_52 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD2").__UILabel__;
    local label_53 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD3").__UILabel__;
    local label_54 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY5_REWARD4").__UILabel__;
    
    local uipicture61 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD1").__UIPicture__;
    local uipicture62 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD2").__UIPicture__;
    local uipicture63 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD3").__UIPicture__;
    local uipicture64 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY6_REWARD4").__UIPicture__;
    
    local label_61 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD1").__UILabel__;
    local label_62 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD2").__UILabel__;
    local label_63 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD3").__UILabel__;
    local label_64 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY6_REWARD4").__UILabel__;
    
    local uipicture71 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD1").__UIPicture__;
    local uipicture72 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD2").__UIPicture__;
    local uipicture73 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD3").__UIPicture__;
    local uipicture74 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY7_REWARD4").__UIPicture__;
    
    local label_71 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD1").__UILabel__;
    local label_72 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD2").__UILabel__;
    local label_73 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD3").__UILabel__;
    local label_74 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY7_REWARD4").__UILabel__;
    
    local uipicture81 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD1").__UIPicture__;
    local uipicture82 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD2").__UIPicture__;
    local uipicture83 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD3").__UIPicture__;
    local uipicture84 = player:FindChildObjectByName("LOGINREWARD_ICON_DAY8_REWARD4").__UIPicture__;
    
    local label_81 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD1").__UILabel__;
    local label_82 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD2").__UILabel__;
    local label_83 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD3").__UILabel__;
    local label_84 = player:FindChildObjectByName("LOGINREWARD_NUM_DAY8_REWARD4").__UILabel__;

    local framesRes1 = 416001 -- 金币
    local framesRes2 = 416007 -- 公新
    local framesRes3 = 416004 -- 声望
    local framesRes4 = 416004 -- 钻石
    
    --50000    500   20   nil
    --80000    1000  30   nil
    --120000   2000  50   nil
    --160000   5000  80   nil
    --200000   7000  100  5
    --250000   8000  150  15
    --4500000  10000 200  20

    local m00 = MainMenuLayer:GetNumByFormat(50000)
    local m01 = MainMenuLayer:GetNumByFormat(500)
    local m02 = MainMenuLayer:GetNumByFormat(20)
    --local m03 = MainMenuLayer:GetNumByFormat(nil)
    
    label_11:setString(m00)
    label_12:setString(m01)
    label_13:setString(m02)
    --label_14:setVisible(false)

    
    local m10 = MainMenuLayer:GetNumByFormat(80000)
    local m11 = MainMenuLayer:GetNumByFormat(1000)
    local m12 = MainMenuLayer:GetNumByFormat(30)
    --local m13 = MainMenuLayer:GetNumByFormat(nil)
    
    label_21:setString(m10)
    label_22:setString(m11)
    label_23:setString(m12)
    
    --label_24:setVisible(false)
    
    local m20 = MainMenuLayer:GetNumByFormat(120000)
    local m21 = MainMenuLayer:GetNumByFormat(2000)
    local m22 = MainMenuLayer:GetNumByFormat(50)
    --local m23 = MainMenuLayer:GetNumByFormat(nil)
    
    
    label_31:setString(m20)
    label_32:setString(m21)
    label_33:setString(m22)
    
    --label_34:setVisible(false)

    
    local m30 = MainMenuLayer:GetNumByFormat(160000)
    local m31 = MainMenuLayer:GetNumByFormat(5000)
    local m32 = MainMenuLayer:GetNumByFormat(80)
    --local m33 = MainMenuLayer:GetNumByFormat(nil)
    
    label_41:setString(m30)
    label_42:setString(m31)
    label_43:setString(m32)
    --label_44:setVisible(false)
    
    local m40 = MainMenuLayer:GetNumByFormat(200000)
    local m41 = MainMenuLayer:GetNumByFormat(7000)
    local m42 = MainMenuLayer:GetNumByFormat(100)
    local m43 = MainMenuLayer:GetNumByFormat(5)
    
    label_51:setString(m40)
    label_52:setString(m41)
    label_53:setString(m42)
    label_54:setString(m43)
    
    local m50 = MainMenuLayer:GetNumByFormat(250000)
    local m51 = MainMenuLayer:GetNumByFormat(8000)
    local m52 = MainMenuLayer:GetNumByFormat(150)
    local m53 = MainMenuLayer:GetNumByFormat(15)
    
    label_61:setString(m50)
    label_62:setString(m51)
    label_63:setString(m52)
    label_64:setString(m53)
    
    local m60 = MainMenuLayer:GetNumByFormat(4500000)
    local m61 = MainMenuLayer:GetNumByFormat(10000)
    local m62 = MainMenuLayer:GetNumByFormat(200)
    local m63 = MainMenuLayer:GetNumByFormat(20)
    
    label_71:setString(m60)
    label_72:setString(m61)
    label_73:setString(m62)
    label_74:setString(m63)
    

    local ui00 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui01 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui02 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui03 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    
    uipicture11:setSprite(ui00)
    uipicture12:setSprite(ui01)
    uipicture13:setSprite(ui02)
    
    local ui10 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui11 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui12 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui13 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    
    uipicture21:setSprite(ui10)
    uipicture22:setSprite(ui11)
    uipicture23:setSprite(ui12)

    local ui20 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui21 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui22 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui23 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")

    uipicture31:setSprite(ui20)
    uipicture32:setSprite(ui21)
    uipicture33:setSprite(ui22)

    local ui30 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui31 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui32 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui33 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")

    uipicture41:setSprite(ui30)
    uipicture42:setSprite(ui31)
    uipicture43:setSprite(ui32)
    
    local ui40 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui41 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui42 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui43 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    
    uipicture51:setSprite(ui40)
    uipicture52:setSprite(ui41)
    uipicture53:setSprite(ui42)
    uipicture54:setSprite(ui43)

    local ui50 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui51 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui52 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui53 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")

    uipicture61:setSprite(ui50)
    uipicture62:setSprite(ui51)
    uipicture63:setSprite(ui52)
    uipicture64:setSprite(ui53)

    local ui60 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui61 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui62 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui63 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    
    uipicture71:setSprite(ui60)
    uipicture72:setSprite(ui61)
    uipicture73:setSprite(ui62)
    uipicture74:setSprite(ui63)

    local ui70 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_COIN_INALL")
    local ui71 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui72 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    local ui73 = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_SHOP_ICON_DIAMOND_INALL")
    
    uipicture81:setSprite(ui70)
    uipicture82:setSprite(ui71)
    uipicture83:setSprite(ui72)
    uipicture84:setSprite(ui73)
end

function onDailyLoginGotDay1Click(pObj)
   print("onDailyLoginGotDay1Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay2Click(pObj)
   print("onDailyLoginGotDay2Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay3Click(pObj)
   print("onDailyLoginGotDay3Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay4Click(pObj)
   print("onDailyLoginGotDay4Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay5Click(pObj)
   print("onDailyLoginGotDay5Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay6Click(pObj)
   print("onDailyLoginGotDay6Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay7Click(pObj)
   print("onDailyLoginGotDay7Click")
   sendGetDailyRewardReq(66)
end

function onDailyLoginGotDay8Click(pObj)
   print("onDailyLoginGotDay8Click")
end

function onDailyLoginClosesClick(pObj)
   print("onDailyLoginClosesClick")
   BackToDailyReWard()
   	XLogicManager:sharedManager():fromGameBackToLogin();
end

DailyLogin_init()
print("DailyLogin Load!!!")


