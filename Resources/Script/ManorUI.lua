

require("Script/ChatToolLayer")
require("Script/MainMenuUILayer")
require("Script/CommonDefine")
require("Script/SpriteElfExplore/ElfExploreDataCenter")
require("Script/SpriteElfExplore/ExchangeElfLayer")
require("Script/GameConfig/uiTagDefine")
require("Script/DailyTask/dailyTaskDataCenter")

require("Script/protobuf")
require("Script/handleMessage")

ManorUI = {
	LayerInstance = nil,
	LayoutEnter = nil,
    LayoutScene = nil,
    
    --5个开拓的lock button
    lockBtnpicture1 = nil,
    lockBtnpicture2 = nil,
    lockBtnpicture3 = nil,
    lockBtnpicture4 = nil,
    lockBtnpicture5 = nil,
    
     --5个开拓字体
    btn1_label = nil,
    btn2_label = nil,
    btn3_label = nil,
    btn4_label = nil,
    btn5_label = nil,
    
    rstString0 = nil,
    rstString1 = nil,
    rstString2 = nil,
    rstString3 = nil,
    rstString4 = nil,
    
    register_id1 = -1,
    register_id2 = -1,
    register_id3 = -1,
    register_id4 = -1,
    register_id5 = -1,
    
    enterManor_id1 = false,
    enterManor_id2 = false,
    enterManor_id3 = false,
    enterManor_id4 = false,
    enterManor_id5 = false,
    
    l_index = 1,
    
    expand_index = 100,
    
    enter_times = 10,
    
    l_remainTime1 = 0,
    l_remainTime2 = 0,
    l_remainTime3 = 0,
    l_remainTime4 = 0,
    l_remainTime5 = 0,
    
    l_manor_pos = 10001,
    
    l_scene_coin = 0,
    l_scene_exp = 0,
}

function ManorUI:ResetValue()
	self.LayerInstance = nil
	self.LayoutEnter = nil
    self.LayoutScene = nil
    
    --5个开拓的lock button
    self.lockBtnpicture1 = nil
    self.lockBtnpicture2 = nil
    self.lockBtnpicture3 = nil
    self.lockBtnpicture4 = nil
    self.lockBtnpicture5 = nil
    
     --5个开拓字体
    self.btn1_label = nil
    self.btn2_label = nil
    self.btn3_label = nil
    self.btn4_label = nil
    self.btn5_label = nil
    
    self.rstString0 = nil
    self.rstString1 = nil
    self.rstString2 = nil
    self.rstString3 = nil
    self.rstString4 = nil
    
    self.register_id1 = -1
    self.register_id2 = -1
    self.register_id3 = -1
    self.register_id4 = -1
    self.register_id5 = -1
    
    self.enterManor_id1 = false
    self.enterManor_id2 = false
    self.enterManor_id3 = false
    self.enterManor_id4 = false
    self.enterManor_id5 = false
    
    self.l_index = 1
    
    self.expand_index = 100
    
    self.enter_times = 10
    
    self.l_remainTime1 = 0
    self.l_remainTime2 = 0
    self.l_remainTime3 = 0
    self.l_remainTime4 = 0
    self.l_remainTime5 = 0
    
    self.l_manor_pos = 10001
    self.l_scene_coin = 0
    self.l_scene_exp = 0
end

-- CCLayer 析构的回调
local function onReceiveLayerDestructor()
	ManorUI:unRegisterAll()
	TXGUI.UIManager:sharedManager():removeUILayout("ManorA")
    TXGUI.UIManager:sharedManager():removeUILayout("ManorB")
	ManorUI:ResetValue()
end

function ManorUI:CreateLayer(parentLayer)
    
    --print("ManorUI:CreateLayer")
	--self:ResetValue()
    --
	--if parentLayer == nil then
		--self.LayerInstance = CCLayer:create()
	--else
		--self.LayerInstance = parentLayer
	--end
	--
	--local uiLayoutEnter = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ManorUIA.plist",self.LayerInstance, "ManorA", true);
    --local uiLayoutScene = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/ManorUIB.plist",self.LayerInstance, "ManorB", true);
--
	--self.LayoutEnter = uiLayoutEnter
    --self.LayoutScene = uiLayoutScene
    --uiLayoutScene:setVisible(false)
	--
	--if parentLayer == nil then
		--local mainMenuNode = GetMainMenuLayerNode()
		--local tag = UITagTable["ManorUI"].tag
		--mainMenuNode:addChild(self.LayerInstance,0,tag)
		--InitMoneyConfigLayer(self.LayerInstance)
	--end	
	--PushOneLayer(self.LayerInstance,"","")	
--
	--self.LayerInstance:registerDestructorScriptHandler(onReceiveLayerDestructor)
    --
    --self:initManor()
    --self:setupTitle()
    --self:setupTitle_Scene()
    --self:setCoinandDimond()
    --self:setupManorABoardUp()
    --
    ----注册4个接收包函数
    --addcallbackAddManorRsp()
    --addcallbackExchangeParameterRsp_Manor()
    --addcallbackManorOptRsp();
    --addcallbackWarmSportLvUpRsp();
end

function ManorUI:initManor()
   self.btnBuy = self.LayoutEnter:FindChildObjectByName("garden_btn_buytime").__UIButton__:getMenuItemSprite();
   self.btncloseManorA = self.LayoutEnter:FindChildObjectByName("manorUIA_CloseBtn").__UIButton__:getMenuItemSprite();
   
   self.lockBtnpicture1 = self.LayoutEnter:FindChildObjectByName("garden_button_lock1").__UIPicture__;
   self.lockBtnpicture2 = self.LayoutEnter:FindChildObjectByName("garden_button_lock2").__UIPicture__;
   self.lockBtnpicture3 = self.LayoutEnter:FindChildObjectByName("garden_button_lock3").__UIPicture__;
   self.lockBtnpicture4 = self.LayoutEnter:FindChildObjectByName("garden_button_lock4").__UIPicture__;
   self.lockBtnpicture5 = self.LayoutEnter:FindChildObjectByName("garden_button_lock5").__UIPicture__;

   self.btn1_label = self.LayoutEnter:FindChildObjectByName("garden_text_collect1").__UILabel__;
   self.btn2_label = self.LayoutEnter:FindChildObjectByName("garden_text_collect2").__UILabel__;
   self.btn3_label = self.LayoutEnter:FindChildObjectByName("garden_text_collect3").__UILabel__;
   self.btn4_label = self.LayoutEnter:FindChildObjectByName("garden_text_collect4").__UILabel__;
   self.btn5_label = self.LayoutEnter:FindChildObjectByName("garden_text_collect5").__UILabel__;

   self.rstString0 = LanguageLocalization:GetLocalization("M_GARDEN_ENTER");
   self.rstString1 = LanguageLocalization:GetLocalization("M_GARDEN_OPEN");
   self.rstString2 = LanguageLocalization:GetLocalization("M_GARDEN_BUYTIME");
   self.rstString3 = LanguageLocalization:GetLocalization("M_GARDEN_REMAINTIME");
   self.rstString4 = LanguageLocalization:GetLocalization("M_GARDEN_TITLE");
  
   --5个Picture Button
   local btnpicture1 = self.LayoutEnter:FindChildObjectByName("garden_grid_col1").__UIButton__:getMenuItemSprite();
   local btnpicture2 = self.LayoutEnter:FindChildObjectByName("garden_grid_col2").__UIButton__:getMenuItemSprite();
   local btnpicture3 = self.LayoutEnter:FindChildObjectByName("garden_grid_col3").__UIButton__:getMenuItemSprite();
   local btnpicture4 = self.LayoutEnter:FindChildObjectByName("garden_grid_col4").__UIButton__:getMenuItemSprite();
   local btnpicture5 = self.LayoutEnter:FindChildObjectByName("garden_grid_col5").__UIButton__:getMenuItemSprite();

   --5个开拓Button
   local btnButton1 = self.LayoutEnter:FindChildObjectByName("garden_btn_click1").__UIButton__:getMenuItemSprite();
   local btnButton2 = self.LayoutEnter:FindChildObjectByName("garden_btn_click2").__UIButton__:getMenuItemSprite();
   local btnButton3 = self.LayoutEnter:FindChildObjectByName("garden_btn_click3").__UIButton__:getMenuItemSprite();
   local btnButton4 = self.LayoutEnter:FindChildObjectByName("garden_btn_click4").__UIButton__:getMenuItemSprite();
   local btnButton5 = self.LayoutEnter:FindChildObjectByName("garden_btn_click5").__UIButton__:getMenuItemSprite();
  
   --购买按钮
   local btnBuy = self.LayoutEnter:FindChildObjectByName("garden_btn_buytime").__UIButton__:getMenuItemSprite();
   
   --关闭按钮
   local btncloseManorA = self.LayoutEnter:FindChildObjectByName("manorUIA_CloseBtn").__UIButton__:getMenuItemSprite();
   
   btnpicture1:registerScriptTapHandler(onEnter1)
   btnpicture2:registerScriptTapHandler(onEnter2)
   btnpicture3:registerScriptTapHandler(onEnter3)
   btnpicture4:registerScriptTapHandler(onEnter4)
   btnpicture5:registerScriptTapHandler(onEnter5)

   btnButton1:registerScriptTapHandler(onEnter1)
   btnButton2:registerScriptTapHandler(onEnter2)
   btnButton3:registerScriptTapHandler(onEnter3)
   btnButton4:registerScriptTapHandler(onEnter4)
   btnButton5:registerScriptTapHandler(onEnter5)
  
   btnBuy:registerScriptTapHandler(OnBuyCounts)
   btncloseManorA:registerScriptTapHandler(onCloseEnter)

   --注册Scene按钮事件
   local btncloseManorB = self.LayoutScene:FindChildObjectByName("manorUIB_CloseBtn").__UIButton__:getMenuItemSprite();
   
   --2个主图片对应的button
   local btnHunt = self.LayoutScene:FindChildObjectByName("garden_hunt_btn").__UIButton__:getMenuItemSprite();
   local btnCollect = self.LayoutScene:FindChildObjectByName("garden_collect_btn").__UIButton__:getMenuItemSprite();
   
   --热身运动
   local btnHotSport = self.LayoutScene:FindChildObjectByName("garden_button_update_btn").__UIButton__:getMenuItemSprite();
   
    --注册函数回调
   btncloseManorB:registerScriptTapHandler(onCloseScene);
   btnHotSport:registerScriptTapHandler(onHotSport);
   btnHunt:registerScriptTapHandler(onHuntBtn);
   btnCollect:registerScriptTapHandler(onCollectBtn);
   
   print("ManorUI:initManor")
end

function ManorUI:setupTitle()

     print("setup title")
     local tile1_label = self.LayoutEnter:FindChildObjectByName("garden_text_remainTime").__UILabel__;
     tile1_label:setString(self.rstString3);
     tile1_label:setColor(ccc3(255,255,255));
     
     local tile0_label = self.LayoutEnter:FindChildObjectByName("garden_text_title").__UILabel__;
     tile0_label:setString(self.rstString4);
     
     local btnBuy_label = self.LayoutEnter:FindChildObjectByName("garden_text_buytime").__UILabel__;
     btnBuy_label:setString(self.rstString2);
     btnBuy_label:setColor(ccc3(255,255,255));
end

function ManorUI:setCoinandDimond()

    if self.LayoutEnter == nil then
       return
    end
    
    local gold_label = self.LayoutEnter:FindChildObjectByName("garden_number_coin_inall").__UILabel__;
    local diamond_label = self.LayoutEnter:FindChildObjectByName("garden_number_diamond_inall").__UILabel__;

    local money = UserData:GetUserInfo().m_gold
    local moneyContent = MainMenuLayer:GetNumByFormat(money)
    
    gold_label:setString(""..moneyContent)   
    print("moneyContent:"..moneyContent)
    print("money:"..money)
    
    local diamond = UserData:GetUserInfo().m_diamond
    --local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
    
    diamond_label:setString(""..diamond)
    
    self:freshEnterTimes()
end

function ManorUI:freshEnterTimes()
    self.enter_times = UserData:GetUserInfo().m_enterManorTimes
    local tile2_label = self.LayoutEnter:FindChildObjectByName("garden_number_remainTime").__UILabel__;
    tile2_label:setString(""..self.enter_times)
end

function ManorUI:setupTitle_Scene()
    local tile0_label = self.LayoutScene:FindChildObjectByName("garden_text_title").__UILabel__;
    local rst = LanguageLocalization:GetLocalization("M_GARDEN_EVENT_TITLE");
    tile0_label:setString(rst);
    tile0_label:setColor(ccc3(255,255,0));
    
    rst = LanguageLocalization:GetLocalization("M_GARDEN_UPGRADE_LEVEL");
    local btnUpdate_label = self.LayoutScene:FindChildObjectByName("garden_text_update").__UILabel__;
    btnUpdate_label:setString(rst);
    btnUpdate_label:setColor(ccc3(255,255,255));
    
    local text_label = self.LayoutScene:FindChildObjectByName("garden_text_level").__UILabel__;
    rst = LanguageLocalization:GetLocalization("M_GARDEN_LEVEL");
    text_label:setString(rst);
    text_label:setColor(ccc3(0,0,255));
end

function ManorUI:setupSecen()
       self:setHuntCollectPic_Scene()
       self:setBoardCoinAndDimaond_Scene()
       self:setCoinAndExp_Scene()
end

function ManorUI:setHuntCollectPic_Scene()
    local sprite_hunt = CCSprite:create("UI/ui_garden_hunt.png");
    local sprite_collect = CCSprite:create("UI/ui_garden_collect.png");
    
    local uipicture1 = self.LayoutScene:FindChildObjectByName("garden_pic_hunt").__UIPicture__;
    local uipicture2 = self.LayoutScene:FindChildObjectByName("garden_pic_collect").__UIPicture__;
    
    uipicture1:setSprite(sprite_hunt);
    uipicture2:setSprite(sprite_collect);
end

function ManorUI:setBoardCoinAndDimaond_Scene()

    if self.LayoutScene == nil then
       return
    end
    
    local gold_label = self.LayoutScene:FindChildObjectByName("garden_number_coin_inall").__UILabel__;
    local diamond_label = self.LayoutScene:FindChildObjectByName("garden_number_diamond_inall").__UILabel__;
    local money = UserData:GetUserInfo().m_gold
    local moneyContent = MainMenuLayer:GetNumByFormat(money)
    gold_label:setString("".. moneyContent)
    local diamond = UserData:GetUserInfo().m_diamond
    --local diamondContent = MainMenuLayer:GetNumByFormat(diamond)
    diamond_label:setString(""..diamond)
end

function ManorUI:setCoinAndExp_Scene()
   local backItem = ItemManager:Get():findItemByPos(self.l_manor_pos);
   if backItem ~= nil then
      local manorBase = backItem.m_manorBaseInfo;
      if manorBase ~= nil then
        local coin = manorBase.hurtGetCoin;
        local exp = manorBase.pickGetExp;
        local item_level = backItem.itemLevel;
        print("item_level"..item_level);
        
        local coin_label = self.LayoutScene:FindChildObjectByName("garden_hunt_coin_number").__UILabel__;
        local strcoin = LanguageLocalization:GetLocalization("S_Coin_content");
        strcoin = strcoin..":";
        strcoin = strcoin..coin;
        coin_label:setString(""..coin);
        coin_label:setColor(ccc3(255,127,0));
        
        local exp_label = self.LayoutScene:FindChildObjectByName("garden_collect_exp_number").__UILabel__;
        local strexp = LanguageLocalization:GetLocalization("S_Exp_content");
        strexp = strexp..": "..exp;
        exp_label:setString(""..exp);
        self:setupCharacterLevel_Scene(item_level);
        
        self.l_scene_coin = coin;
        self.l_scene_exp = exp;
      end
   end
end

function ManorUI:setupCharacterLevel_Scene(index)
    --E e D d C c B b A a
    local uipicture_E = self.LayoutScene:FindChildObjectByName("garden_icon_e").__UIPicture__;
    local uipicture_e = self.LayoutScene:FindChildObjectByName("garden_icon_e_selected").__UIPicture__;

    local uipicture_D = self.LayoutScene:FindChildObjectByName("garden_icon_d").__UIPicture__;
    local uipicture_d = self.LayoutScene:FindChildObjectByName("garden_icon_d_selected").__UIPicture__;

    local uipicture_C = self.LayoutScene:FindChildObjectByName("garden_icon_c").__UIPicture__;
    local uipicture_c = self.LayoutScene:FindChildObjectByName("garden_icon_c_selected").__UIPicture__;

    local uipicture_B = self.LayoutScene:FindChildObjectByName("garden_icon_b").__UIPicture__;
    local uipicture_b = self.LayoutScene:FindChildObjectByName("garden_icon_b_selected").__UIPicture__;

    local uipicture_A = self.LayoutScene:FindChildObjectByName("garden_icon_a").__UIPicture__;
    local uipicture_a = self.LayoutScene:FindChildObjectByName("garden_icon_a_selected").__UIPicture__;
    
	local asMgr = AspriteManager:getInstance(); 
    local frameRes_a = "map_ui_system_icon_FRAME_ICON_GARDEN_A";
    local frameRes_A = "map_ui_system_icon_FRAME_ICON_GARDEN_A_CLICKED";
    
    local frameRes_b = "map_ui_system_icon_FRAME_ICON_GARDEN_B";
    local frameRes_B = "map_ui_system_icon_FRAME_ICON_GARDEN_B_CLICKED";
    
    local frameRes_c = "map_ui_system_icon_FRAME_ICON_GARDEN_C";
    local frameRes_C = "map_ui_system_icon_FRAME_ICON_GARDEN_C_CLICKED";
    
    local frameRes_d = "map_ui_system_icon_FRAME_ICON_GARDEN_D";
    local frameRes_D = "map_ui_system_icon_FRAME_ICON_GARDEN_D_CLICKED";
    
    local frameRes_e = "map_ui_system_icon_FRAME_ICON_GARDEN_E";
    local frameRes_E = "map_ui_system_icon_FRAME_ICON_GARDEN_E_CLICKED";
    
    local icon = nil;
    if index == 5 then
    
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_A);
      uipicture_A:setSprite(icon);
      uipicture_a:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_b);
      uipicture_b:setSprite(icon);
      uipicture_B:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_c);
      uipicture_c:setSprite(icon);
      uipicture_C:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_d);
      uipicture_d:setSprite(icon);
      uipicture_D:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_e);
      uipicture_e:setSprite(icon);
      uipicture_E:setVisible(false);
      
    elseif index == 4 then
    
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_B);
      uipicture_B:setSprite(icon);
      uipicture_b:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_a);
      uipicture_a:setSprite(icon);
      uipicture_A:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_c);
      uipicture_c:setSprite(icon);
      uipicture_C:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_d);
      uipicture_d:setSprite(icon);
      uipicture_D:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_e);
      uipicture_e:setSprite(icon);
      uipicture_E:setVisible(false);
      
    elseif index == 3 then
    
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_C);
      uipicture_C:setSprite(icon);
      uipicture_c:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_a);
      uipicture_a:setSprite(icon);
      uipicture_A:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_b);
      uipicture_b:setSprite(icon);
      uipicture_B:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_d);
      uipicture_d:setSprite(icon);
      uipicture_D:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_e);
      uipicture_e:setSprite(icon);
      uipicture_E:setVisible(false);
      
    elseif index == 2 then
    
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_D);
      uipicture_D:setSprite(icon);
      uipicture_d:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_a);
      uipicture_a:setSprite(icon);
      uipicture_A:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_b);
      uipicture_b:setSprite(icon);
      uipicture_B:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_c);
      uipicture_c:setSprite(icon);
      uipicture_C:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_e);
      uipicture_e:setSprite(icon);
      uipicture_E:setVisible(false);
      
    elseif index == 1 then
    
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_E);
      uipicture_E:setSprite(icon);
      uipicture_e:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_a);
      uipicture_a:setSprite(icon);
      uipicture_A:setVisible(false);

      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_b);
      uipicture_b:setSprite(icon);
      uipicture_B:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_c);
      uipicture_c:setSprite(icon);
      uipicture_C:setVisible(false);
      
      icon = asMgr:getOneFrame("UI/ui_system_icon.bin", frameRes_d);
      uipicture_d:setSprite(icon);
      uipicture_D:setVisible(false);
    end
end

function ManorUI:hideLock(index)

   if index == 2 then
     local uipicture = self.LayoutEnter:FindChildObjectByName("garden_lock_collect2").__UIPicture__;
     uipicture:setVisible(false);
     
   elseif index == 3 then
     local uipicture = self.LayoutEnter:FindChildObjectByName("garden_lock_collect3").__UIPicture__;
     uipicture:setVisible(false);
     
   elseif index == 4 then
      local uipicture = self.LayoutEnter:FindChildObjectByName("garden_lock_collect4").__UIPicture__;
      uipicture:setVisible(false);

   elseif index == 5 then
       local uipicture = self.LayoutEnter:FindChildObjectByName("garden_lock_collect5").__UIPicture__;
       uipicture:setVisible(false);
   end
end

function ManorUI:setUiPictureSprite(sprite, index)

    if index == 1 then
        local uipicture1 = self.LayoutEnter:FindChildObjectByName("garden_pic_col1").__UIPicture__;
        uipicture1:setSprite(sprite);
                                                  
    elseif index == 2 then
        local uipicture2 = self.LayoutEnter:FindChildObjectByName("garden_pic_col2").__UIPicture__;
        uipicture2:setSprite(sprite);

    elseif index == 3 then
        local uipicture3 = self.LayoutEnter:FindChildObjectByName("garden_pic_col3").__UIPicture__;
        uipicture3:setSprite(sprite);
                         
    elseif index == 4 then
        local uipicture4 = self.LayoutEnter:FindChildObjectByName("garden_pic_col4").__UIPicture__;
        uipicture4:setSprite(sprite);
                         
    elseif index == 5 then
        local uipicture5 = self.LayoutEnter:FindChildObjectByName("garden_pic_col5").__UIPicture__;
        uipicture5:setSprite(sprite);
    end
end


function ManorUI:unRegisterAll()

    if self.register_id1 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id1, TIMER_MANOUR_1);
        self.register_id1 = -1;
    end
    
    if self.register_id2 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id2, TIMER_MANOUR_2);
        self.register_id2 = -1;
    end
    
    if self.register_id3 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id3, TIMER_MANOUR_3);
        self.register_id3 = -1;
    end
    
    if self.register_id4 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id4, TIMER_MANOUR_4);
        self.register_id4 = -1;
    end
    
    if self.register_id5 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id5, TIMER_MANOUR_5);
        self.register_id5 = -1;
    end
    
    self.enterManor_id1 = false;
    self.enterManor_id2 = false;
    self.enterManor_id3 = false;
    self.enterManor_id4 = false;
    self.enterManor_id5 = false;
end

function xxxxxonUpdateCDTime1(remainTime)
    
    if  ManorUI.enterManor_id1 == true then
        ManorUI.btn1_label:setString(ManorUI.rstString0)
    else
       ManorUI.l_remainTime1 = remainTime;
	   local remainStr = TimeManager:Get():secondsToString(remainTime)
       if remainTime ~= 0 then
          ManorUI.btn1_label:setString(remainStr)
       else
          ManorUI.enterManor_id1 = true;
          ManorUI.btn1_label:setString(ManorUI.rstString0)
       end
    end
end

function onUpdateCDTime2(remainTime)

    if  ManorUI.enterManor_id2 == true then
        ManorUI.btn2_label:setString( ManorUI.rstString0)
    else
       ManorUI.l_remainTime2 = remainTime;
	   local remainStr = TimeManager:Get():secondsToString(remainTime)
       if remainTime ~= 0 then
          ManorUI.btn2_label:setString(remainStr)
       else
          ManorUI.enterManor_id2 = true;
          ManorUI.btn2_label:setString(ManorUI.rstString0)
       end
    end
end

function onUpdateCDTime3(remainTime)

    if  ManorUI.enterManor_id3 == true then
        ManorUI.btn3_label:setString( ManorUI.rstString0)
    else
       ManorUI.l_remainTime3 = remainTime;
	   local remainStr = TimeManager:Get():secondsToString(remainTime)
       if remainTime ~= 0 then
          ManorUI.btn3_label:setString(remainStr)
       else
          ManorUI.enterManor_id3 = true;
          ManorUI.btn3_label:setString(ManorUI.rstString0)
       end
    end
end

function onUpdateCDTime4(remainTime)

    if  ManorUI.enterManor_id4 == true then
        ManorUI.btn4_label:setString( ManorUI.rstString0)
    else
       ManorUI.l_remainTime4 = remainTime;
	   local remainStr = TimeManager:Get():secondsToString(remainTime)
       if remainTime ~= 0 then
          ManorUI.btn4_label:setString(remainStr)
       else
          ManorUI.enterManor_id4 = true;
          ManorUI.btn4_label:setString(ManorUI.rstString0)
       end
    end
end

function onUpdateCDTime5(remainTime)

    if  ManorUI.enterManor_id5 == true then
        ManorUI.btn5_label:setString( ManorUI.rstString0)
    else
       ManorUI.l_remainTime5 = remainTime;
	   local remainStr = TimeManager:Get():secondsToString(remainTime)
       if remainTime ~= 0 then
          ManorUI.btn5_label:setString(remainStr)
       else
          ManorUI.enterManor_id5 = true;
          ManorUI.btn5_label:setString(ManorUI.rstString0)
       end
    end
end

function ManorUI:initCDTime1(seconds)

    print("ManorUI:initCDTime1:second.."..seconds)
    
    local nowTime = TimeManager:Get():getCurServerTime()
    if seconds <= 0 then 
       seconds = 0
    end
    
    if self.register_id1 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id1, TIMER_MANOUR_1);
        self.register_id1 = TimeManager:Get():registerLuaTimer(xxxxxonUpdateCDTime1, TIMER_MANOUR_1, nowTime + seconds)
        print("A first register l_registerid_1:"..self.register_id1)

    else
        self.register_id1 = TimeManager:Get():registerLuaTimer(xxxxxonUpdateCDTime1, TIMER_MANOUR_1, nowTime + seconds)
        local remainStr = TimeManager:Get():secondsToString(seconds);
        self.btn1_label:setString(remainStr);
        print("B first register l_registerid_1:"..self.register_id1.."remainStr:"..remainStr)
    end
end

function ManorUI:initCDTime2(seconds)

    local nowTime = TimeManager:Get():getCurServerTime()
    if self.register_id2 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id2, TIMER_MANOUR_2);
        self.register_id2 = TimeManager:Get():registerLuaTimer(onUpdateCDTime2, TIMER_MANOUR_2, nowTime + seconds)
        print("first register l_registerid_2:"..self.register_id2)
        
    else
        self.register_id2 = TimeManager:Get():registerLuaTimer(onUpdateCDTime2, TIMER_MANOUR_2, nowTime + seconds)
        print("first register l_registerid_2:"..self.register_id2)
        local remainStr = TimeManager:Get():secondsToString(seconds);
        self.btn2_label:setString(remainStr);
        print("B first register l_registerid_2:"..self.register_id2.."remainStr:"..remainStr)
    end
end

function ManorUI:initCDTime3(seconds)
    
    local nowTime = TimeManager:Get():getCurServerTime()
    if self.register_id3 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id3, TIMER_MANOUR_3);
        self.register_id3 = TimeManager:Get():registerLuaTimer(onUpdateCDTime3, TIMER_MANOUR_3, nowTime + seconds)
        print("first register l_registerid_3:"..self.register_id3)
    else
        self.register_id3 = TimeManager:Get():registerLuaTimer(onUpdateCDTime3, TIMER_MANOUR_3, nowTime + seconds)
        print("first register l_registerid_3:"..self.register_id3)
        local remainStr = TimeManager:Get():secondsToString(seconds);
        self.btn3_label:setString(remainStr);
    end
end

function ManorUI:initCDTime4(seconds)
    
    local nowTime = TimeManager:Get():getCurServerTime()
    if self.register_id4 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id4, TIMER_MANOUR_4);
        self.register_id4 = TimeManager:Get():registerLuaTimer(onUpdateCDTime4, TIMER_MANOUR_4, nowTime + seconds)
        print("first register l_registerid_4:"..self.register_id4)
    else
        self.register_id4 = TimeManager:Get():registerLuaTimer(onUpdateCDTime4, TIMER_MANOUR_4, nowTime + seconds)
        print("first register l_registerid_4:"..self.register_id4)
        local remainStr = TimeManager:Get():secondsToString(seconds);
        self.btn4_label:setString(remainStr);
    end
end

function ManorUI:initCDTime5(seconds)
    
    local nowTime = TimeManager:Get():getCurServerTime()
    if self.register_id5 ~= -1 then
        TimeManager:Get():unregisterLuaTimer(self.register_id5, TIMER_MANOUR_5);
        self.register_id5 = TimeManager:Get():registerLuaTimer(onUpdateCDTime5, TIMER_MANOUR_5, nowTime + seconds)
        print("first register l_registerid_5:"..self.register_id5)
        
    else
        self.register_id5 = TimeManager:Get():registerLuaTimer(onUpdateCDTime5, TIMER_MANOUR_5, nowTime + seconds)
        print("first register l_registerid_5:"..self.register_id5)
        local remainStr = TimeManager:Get():secondsToString(seconds);
        self.btn5_label:setString(remainStr);
    end
end

function ManorUI:initCDTime(endTime, index)
	local nowTime = TimeManager:Get():getCurServerTime()    
    local val;
    if endTime < nowTime then
       val = 0;
    else
       val = endTime - nowTime;
    end
    if index == 1 then
       self:initCDTime1(val);
       
    elseif index == 2 then
       self:initCDTime2(val);
       
    elseif index == 3 then
       self:initCDTime3(val);

    elseif index == 4 then
       self:initCDTime4(val);
       
    elseif index == 5 then
       self:initCDTime5(val);
    end
end

function ManorUI:setBoardUnlock(backItem, index)
        local ret = false;
		if backItem ~= nil then
            local itemInfo = backItem:getItemInfo()
            local sprite;
            if itemInfo ~= nil then
                sprite = CCSprite:create("UI/ui_garden_light.png");
                local item_num = backItem.amount;
                local item_level = backItem.itemLevel;
                local nowTime = TimeManager:Get():getCurServerTime()  

                print("itemInfo ~= nil")
                if index == 1 then
                    self.lockBtnpicture1:setVisible(false);
                    self:initCDTime(item_num,index);
                    
                elseif index == 2 then
                    self.lockBtnpicture2:setVisible(false);
                    self:initCDTime(item_num,index);
                    
                elseif index == 3 then
                    self.lockBtnpicture3:setVisible(false);
                    self:initCDTime(item_num,index);
                    
                elseif index == 4 then
                    self.lockBtnpicture4:setVisible(false);
                    self:initCDTime(item_num,index);

                elseif index == 5 then
                    self.lockBtnpicture5:setVisible(false);
                    self:initCDTime(item_num,index);
                end
                
                self:hideLock(index);
                ret = true;
            else
                sprite = CCSprite:create("UI/ui_garden_dark.png");
                if index == 1 then
                    self.lockBtnpicture1:setVisible(true);
                    self.btn1_label:setString(self.rstString1);
                    
                elseif index == 2 then
                    self.lockBtnpicture2:setVisible(true);
                    self.btn2_label:setString(self.rstString1);
                    
                elseif index == 3 then
                    self.lockBtnpicture3:setVisible(true);
                    self.btn3_label:setString(self.rstString1);
                    
                elseif index == 4 then
                    self.lockBtnpicture4:setVisible(true);
                    self.btn4_label:setString(self.rstString1);
                    
                elseif index == 5 then
                    self.lockBtnpicture5:setVisible(true);
                    self.btn5_label:setString(self.rstString1);
                end
            end
            self:setUiPictureSprite(sprite,index);
		end
        return ret;
end

function ManorUI:setupManorABoardUp()

   print("setupManorABoardUp:")
   
   local pos1 = 10001;
   local pos2 = 10002;
   local pos3 = 10003;
   local pos4 = 10004;
   local pos5 = 10005;
   
   local backItem1 = ItemManager:Get():findItemByPos(pos1);
   local backItem2 = ItemManager:Get():findItemByPos(pos2);
   local backItem3 = ItemManager:Get():findItemByPos(pos3);
   local backItem4 = ItemManager:Get():findItemByPos(pos4);
   local backItem5 = ItemManager:Get():findItemByPos(pos5);
   
   local ret1 = self:setBoardUnlock(backItem1, 1);
   local ret2 = self:setBoardUnlock(backItem2, 2);
   local ret3 = self:setBoardUnlock(backItem3, 3);
   local ret4 = self:setBoardUnlock(backItem4, 4);
   local ret5 = self:setBoardUnlock(backItem5, 5);
   
   if ret1 == false then
        self.lockBtnpicture1:setVisible(false);
        sprite = CCSprite:create("UI/ui_garden_light.png");
        self:setUiPictureSprite(sprite,1);
        self.btn1_label:setString(LanguageLocalization:GetLocalization("M_GARDEN_ENTER"))
        self.enterManor_id1 = true
   end

   if ret1 == false then
      --self.lockBtnpicture1:setVisible(false);
      --sprite = CCSprite:create("UI/ui_garden_light.png");
      --self:setUiPictureSprite(sprite,1);
      --self.expand_index = 1;
      
   elseif ret2 == false then
        self.lockBtnpicture2:setVisible(false);
        self.expand_index = 2;
        
   elseif ret3 == false then
        self.lockBtnpicture3:setVisible(false);
        self.expand_index = 3;
        
   elseif ret4 == false then
        self.lockBtnpicture4:setVisible(false);
        self.expand_index = 4;
        
   elseif ret5 == false then
        self.lockBtnpicture5:setVisible(false);
        self.expand_index = 5;
   else
        self.expand_index = 100;
   end
   print("self expand_index:"..self.expand_index)
end

--开拓庄园
function ManorUI:DoManor(index)
    if index == 1 then 
       self.l_manor_pos = 10001
       self:DoManor_core(1,self.enterManor_id1)
       
    elseif index == 2 then
       self.l_manor_pos = 10002
       self:DoManor_core(2,self.enterManor_id2)
       
    elseif index == 3 then
       self.l_manor_pos = 10003
       self:DoManor_core(3,self.enterManor_id3)
       
    elseif index == 4 then
       self.l_manor_pos = 10004
       self:DoManor_core(4,self.enterManor_id4)
       
    elseif index == 5 then
       self.l_manor_pos = 10005
       self:DoManor_core(5,self.enterManor_id5)
    end
end

function ManorUI:DoManor_core(index, val)
    print("index..expand_index:"..index.."__"..self.expand_index)
    print(val)
    self.l_index = index;
    
    if val == true then --倒计时结束
        if self.enter_times == 0 then  --进入次数为0
           --MessageBox:Show(LanguageLocalization:GetLocalization("M_GARDEN_NOTIME"),nil,nil,MB_YESNO,ccc3(255,255,255),26)
           GameApi:showMessage(LanguageLocalization:GetLocalization("M_GARDEN_NOTIME"));
        else --进入庄园
           ManorUI:turnManorScene()
        end
        
    elseif index < self.expand_index then --倒计时没有结束
        print("index < self.expand_index")
        local diamondNum = computeEnterManorDimand(index);
        local languageFlag =  'M_SKIP_CD' --'M_RENOVATESHOP_COSTDIAMOND'
        local content = LanguageLocalization:GetLocalization(languageFlag)
        DiamondWarningLayer:Show(content,diamondNum,sendExchangeParameterReq_Manor,nil,ccc3(255,255,255),"","")
                                
    elseif index == self.expand_index then --可以开拓庄园
        print("index == self.expand_index")
        
        local param_id = 40000;
        local diamond_need = 0;
        
        if index == 1 then
          param_id = 40000;
          diamond_need = 0;
       
        elseif index == 2 then
          param_id = 40010;
          diamond_need = 100;
       
        elseif index == 3 then
          param_id = 40020;
          diamond_need = 200;
       
        elseif index == 4 then
          param_id = 40030;
          diamond_need = 500;
       
        elseif index == 5 then
          param_id = 40040;
          diamond_need = 1000;
        end
        
        local diamond_mine = UserData:GetUserInfo().m_diamond        
        if diamond_need == 0 then
           expandManor_Ok()
        elseif diamond_mine < diamond_need then
            MessageBox:Show(LanguageLocalization:GetLocalization("S_Gh_Diamond_Content"),nil,nil,MB_YESNO,ccc3(255,255,255),26)
        else
            local content = LanguageLocalization:GetLocalization("M_GARDEN_OPENNEED")
            DiamondWarningLayer:Show(content,diamond_need,expandManor_Ok,nil,ccc3(255,255,255),"","")
        end
    end
end

function expandManor_Ok()
   ManorUI:sendManorReq()
end

function ManorUI:sendManorReq()
   print("sendManorReq")
   if self.expand_index == 1 then
      sendAddManorReq(419000);
      
   elseif self.expand_index == 2 then
      sendAddManorReq(419001);
      
   elseif self.expand_index == 3 then
      sendAddManorReq(419002);
      
   elseif self.expand_index == 4 then
      sendAddManorReq(419003);
      
   elseif self.expand_index == 5 then
      sendAddManorReq(419004);
   end
end

function ManorUI:turnManorScene()
	self.LayoutEnter:setVisible(false)
    self.LayoutScene:setVisible(true)
    ManorUI:setupSecen()
    ManorUI:setCoinandDimond()
end

function ManorUI:turnManorEnter()
	self.LayoutEnter:setVisible(true)
    self.LayoutScene:setVisible(false)
end

function ManorUI:resetEnterManorId()

  if self.l_manor_pos == 10001 then
     ManorUI.enterManor_id1 = false;
     
  elseif self.l_manor_pos == 10002 then
     ManorUI.enterManor_id2 = false;
     
  elseif self.l_manor_pos == 10003 then
     ManorUI.enterManor_id3 = false;
     
  elseif self.l_manor_pos == 10004 then
     ManorUI.enterManor_id4 = false;
     
  elseif self.l_manor_pos == 10005 then
     ManorUI.enterManor_id5 = false;
  end
  
end

--进入界面入口
function onEnter1(pObj)
    ManorUI:DoManor(1)
end

function onEnter2(pObj)
    ManorUI:DoManor(2)
end

function onEnter3(pObj)
    ManorUI:DoManor(3)
end

function onEnter4(pObj)
     ManorUI:DoManor(4);
end

function onEnter5(pObj)
    ManorUI:DoManor(5);
end

function onCloseEnter(pObj)
    print("onCloseEnter")
    GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);    
    ManorUICloseBtnClick(nil)
end

function onCloseScene(pObj)
    print("onCloseScene")
    ManorUI:turnManorEnter()
end

function onHotSport(pObj)
   print("onHotSport");
   local diamondNum = dailyTaskDataCenter:GetDailyTaskNeedDiamondByExchangeID(21100)
   local languageFlag = 'Dailytask_4011_Name'
   local content = LanguageLocalization:GetLocalization(languageFlag)
   DiamondWarningLayer:Show(content,diamondNum,sendWarmSportLvUpReq,nil,ccc3(255,255,255),"","")
end

function onHuntBtn(pObj)
   print("onHuntBtn");
   print("l_manor_pos:"..ManorUI.l_manor_pos)
   sendManorOptReq(ManorUI.l_manor_pos, 1);
end

function onCollectBtn(pObj)
   print("onCollectBtn");
   print("l_manor_pos:"..ManorUI.l_manor_pos)
   sendManorOptReq(ManorUI.l_manor_pos, 2);
end

--购买次数
function OnBuyCounts(pObj)

   print("OnBuyCounts")
   local languageFlag = 'M_GARDEN_BUY_TIME_NEED'
   local content = LanguageLocalization:GetLocalization(languageFlag)
   local buytimes = UserData:GetUserInfo().m_buyManorTimes
   
   print("buytimes:"..buytimes)
   
   if buytimes >= 5 then
      GameApi:showMessage(LanguageLocalization:GetLocalization("M_GARDEN_BUYTIME_MAX"));
      return
   end

   local diamonNum;
   
   if buytimes == 0 then
       diamonNum = 2
    elseif buytimes == 1 then
       diamonNum = 5
    elseif buytimes == 2 then
       diamonNum = 10
    elseif buytimes == 3 then
       diamonNum = 20
    else
       diamonNum = 50
   end

   DiamondWarningLayer:Show(content,diamonNum,sendExchangeParameterBuyManorTimesReq_Manor,nil,ccc3(255,255,255),"","")
   
   print("OnBuyCounts end")
end

function ManorUICloseBtnClick(pObj)	
	if nil ~= ManorUI.LayerInstance then
        SetChatBar(true,-1);
		ManorUI:unRegisterAll()
		RemoveOneLayer(ManorUI.LayerInstance)
		TXGUI.UIManager:sharedManager():removeUILayout("ManorA")
        TXGUI.UIManager:sharedManager():removeUILayout("ManorB")
		CCTextureCache:sharedTextureCache():removeUnusedTextures()
		ManorUI.LayerInstance:removeFromParentAndCleanup(true);
		ManorUI.LayerInstance = nil
        ManorUI:ResetValue()
	end
end

function computeEnterManorDimand(index)
   local remain;
   if index == 1 then
      remain = ManorUI.l_remainTime1;
      
   elseif index == 2 then
         remain = ManorUI.l_remainTime2;
         
   elseif index == 3 then
         remain = ManorUI.l_remainTime3;
         
   elseif index == 4 then
         remain = ManorUI.l_remainTime4;
         
   elseif index == 5 then
         remain = ManorUI.l_remainTime5;
   end
   
   print("computer dimand remaintime:"..remain)
   
   if remain < 1800 then
      return 10;
   else
      remain = remain / 1800;
      remain = remain * 10;
      return remain;
   end
end

--------------------------------------------------消息发送-------------------------------
--发送开拓庄园包
function sendAddManorReq(manorId)
    print("sendAddManorReq "..manorId);
	local msgname="CSAddManorReq"
	CSAddManorReq = {
		manor_id = manorId,
	}
	local ret = send_message(msgname, CSAddManorReq, true)
	return ret;
end

--接收开拓庄园包
function CSAddManorRspCallBack(msg)
	print("CSAddManorRspCallBack")
	print(msg.pos)
    print("l_index:"..ManorUI.l_index)
    
    if msg.pos == 0 then
       MessageBox:Show("开拓失败",nil,nil,MB_YESNO,ccc3(255,255,255),26)
    else 
       ManorUI.l_manor_pos = msg.pos;
       --ManorUI:turnManorScene()
       
       local ret1 = false;
       local ret2 = false;
       local ret3 = false;
       local ret4 = false;
       local ret5 = false;
       
       if msg.pos == 10001 then
          ManorUI.enterManor_id1 = true
          
          sprite = CCSprite:create("UI/ui_garden_light.png");
          ManorUI:setUiPictureSprite(sprite,1);
          ManorUI.lockBtnpicture1:setVisible(false);
          ManorUI:hideLock(1)
          ManorUI.btn1_label:setString(ManorUI.rstString0)
          ret2 = true
          
       elseif msg.pos == 10002 then
          ManorUI.enterManor_id2 = true
          
          sprite = CCSprite:create("UI/ui_garden_light.png");
          ManorUI:setUiPictureSprite(sprite,2);
          ManorUI.lockBtnpicture1:setVisible(false);
          ManorUI:hideLock(2)
          ManorUI.btn2_label:setString(ManorUI.rstString0)
          ret3 = true

       elseif msg.pos == 10003 then
          ManorUI.enterManor_id3 = true
          
          sprite = CCSprite:create("UI/ui_garden_light.png");
          ManorUI:setUiPictureSprite(sprite,3);
          ManorUI.lockBtnpicture1:setVisible(false);
          ManorUI:hideLock(3)
          ManorUI.btn3_label:setString(ManorUI.rstString0)
          ret4 = true

       elseif msg.pos == 10004 then
          ManorUI.enterManor_id4 = true
          
          sprite = CCSprite:create("UI/ui_garden_light.png");
          ManorUI:setUiPictureSprite(sprite,4);
          ManorUI.lockBtnpicture1:setVisible(false);
          ManorUI:hideLock(4)
          ManorUI.btn4_label:setString(ManorUI.rstString0)
          ret5 = true
          
       elseif msg.pos == 10005 then
          ManorUI.enterManor_id5 = true
          
          sprite = CCSprite:create("UI/ui_garden_light.png");
          ManorUI:setUiPictureSprite(sprite,5);
          ManorUI.lockBtnpicture1:setVisible(false);
          ManorUI:hideLock(5)
          ManorUI.btn5_label:setString(ManorUI.rstString0)
          ret5 = true

       end
       
       if ret2 == true then
          ManorUI.lockBtnpicture2:setVisible(false);
          ManorUI.expand_index = 2;
        
       elseif ret3 == true then
          ManorUI.lockBtnpicture3:setVisible(false);
          ManorUI.expand_index = 3;
        
       elseif ret4 == true then
          ManorUI.lockBtnpicture4:setVisible(false);
          ManorUI.expand_index = 4;
          
       elseif ret5 == true then
          ManorUI.lockBtnpicture5:setVisible(false);
          ManorUI.expand_index = 5;

       else
          ManorUI.expand_index = 100;
       end

       print("self expand_index:"..ManorUI.expand_index)
       ManorUI:setCoinandDimond()
       ManorUI:setBoardCoinAndDimaond_Scene()
    end
    return true
end

---注册接收包函数
function addcallbackAddManorRsp()
	addMsgCallBack("CSAddManorRsp", CSAddManorRspCallBack)
	return 1
end

--发送购买次数消费
function sendExchangeParameterBuyManorTimesReq_Manor()
    local id_buyId
    local buytimes = UserData:GetUserInfo().m_buyManorTimes
    
    if buytimes == 0 then
       id_buyId = 30100
    elseif buytimes == 1 then
       id_buyId = 30200
    elseif buytimes == 2 then
       id_buyId = 30300
    elseif buytimes == 3 then
       id_buyId = 30400
    else
       id_buyId = 30500
    end
    
	local msgname="CSExchangeParameterReq"
	ExchangeParameterReq = {
		id = id_buyId,
		param = {
            ManorUI.l_manor_pos,
		}
	}
    print("sendExchangeParameterBuyManorTimesReq_Manor"..id_buyId)
	local ret = send_message(msgname, ExchangeParameterReq, true)
	return ret;
end

--发送钻石消耗包
function sendExchangeParameterReq_Manor(pos)
	local msgname="CSExchangeParameterReq"
	ExchangeParameterReq = {
		id = 10700,
		param = {
            ManorUI.l_manor_pos,
		}
	}
	local ret = send_message(msgname, ExchangeParameterReq, true)
	return ret;
end

--接收钻石消耗包
function CSExchangeParameterRspCallBack_Manor(msg)
    print("CSExchangeParameterRspCallBack_Manor")
	print(msg.succ, msg.id)
    
    if msg.id == 10700 or 
       msg.id == 30100 or 
       msg.id == 30200 or 
       msg.id == 30300 or 
       msg.id == 30400 or 
       msg.id == 30500 then
       
       if msg.succ == true and msg.id == 10700 then
        if ManorUI.l_index == 1 then
           ManorUI.enterManor_id1 = true;
         
        elseif ManorUI.l_index == 2 then
           ManorUI.enterManor_id2 = true;
         
        elseif ManorUI.l_index == 3 then
           ManorUI.enterManor_id3 = true;
         
        elseif ManorUI.l_index == 4 then
           ManorUI.enterManor_id4 = true;
         
        elseif ManorUI.l_index == 5 then
           ManorUI.enterManor_id5 = true;
        end
      
        ManorUI:setCoinandDimond()
        ManorUI:setBoardCoinAndDimaond_Scene()
      end
    
      if msg.succ == true and msg.id ~= 10700 then
         print("if msg.succ == true and msg.id ~= 10700 then")
         GameApi:showMessage(LanguageLocalization:GetLocalization("M_GARDEN_BUYTIME_SUCCESS"));
         ManorUI:freshEnterTimes()
       end
    end
	return false
end

---注册接收包函数
function addcallbackExchangeParameterRsp_Manor()
	addMsgCallBack("CSExchangeParameterRsp", CSExchangeParameterRspCallBack_Manor)
	return 1
end

----庄园操作请求发包
function sendManorOptReq(pos_in, opt_in)
    print("sendManorOptReq "..pos_in..".."..opt_in);
	local msgname="CSManorOptReq"
	CSManorOptReq = {
       pos = pos_in;
       opt = opt_in;
	}
	local ret = send_message(msgname, CSManorOptReq, true)
	return ret;
end

----庄园操作请求收包
function CSManorOptRspCallBack(msg)
	print("CSManorOptRspCallBack")
	print(msg.pos)
    print(msg.opt)
    
    if msg.pos == 0 then  --协议失败
        MessageBox:Show(LanguageLocalization:GetLocalization("M_GARDEN_INCD"),nil,nil,MB_YESNO,ccc3(255,255,255),26)
    else
    
       if msg.opt == 1 then
           GameApi:showMessage("获得金币:"..ManorUI.l_scene_coin);
       else
           GameApi:showMessage("获得经验:"..ManorUI.l_scene_exp);
       end
       
       ManorUI:turnManorEnter()
       ManorUI:resetEnterManorId()
       ManorUI:setupManorABoardUp()
       ManorUI:freshEnterTimes()
    end 
    return true
end

---庄园操作请求收包
function addcallbackManorOptRsp()
	addMsgCallBack("CSManorOptRsp", CSManorOptRspCallBack)
	return 1
end

---注册接收包函数
function sendWarmSportLvUpReq(pos_in, to_max_lv_in)
	local msgname="CSWarmSportLvUpReq"
	local body = {
       pos = ManorUI.l_manor_pos,
       to_max_lv=false;
	}
	local ret = send_message(msgname, body, true)
	return ret;
end

------刷新操作接收包
function CSWarmSportLvUpRspCallBack(msg)
	print("CSWarmSportLvUpRsp")
	print(msg.pos)
    ManorUI:setCoinAndExp_Scene()
    ManorUI:setBoardCoinAndDimaond_Scene()
end

---注册接收包函数
function addcallbackWarmSportLvUpRsp()
	addMsgCallBack("CSWarmSportLvUpRsp", CSWarmSportLvUpRspCallBack)
	return 1
end

local function closeCallBack(message)
	if nil ~= ManorUI.LayerInstance then
		if G_CurLayerInstance ~= ManorUI.LayerInstance then
			ManorUICloseBtnClick(nil)
		end			
	end
end

local function updateNotification(message)
	if message == GM_NETWORK_DISCONNCT then
		ManorUICloseBtnClick(nil)
	elseif message == GM_LUA_LAYER_CLOSE then
		closeCallBack(message)
	end	
end

--local notificationFunc = NotificationCenter:defaultCenter():registerScriptObserver(updateNotification);
--print("load manur success!")



function test_cd(pos)
   local returnVal = false;
   local backItem = ItemManager:Get():findItemByPos(pos);
   if backItem ~= nil then
      local itemInfo = backItem:getItemInfo()
      if itemInfo ~= nil then
         local item_num = backItem.amount;
         local nowTime = TimeManager:Get():getCurServerTime()
         if nowTime > item_num then
            returnVal = true
         end  
      end
   end
   return returnVal
end


function manor_update()
  local topLayout = TXGUI.UIManager:sharedManager():getUILayout("MainMenu");
	if nil ~= topLayout then
		local manorButton = topLayout:FindChildObjectByName("button_garden").__UIButton__
		if nil ~= manorButton then
           local pos1 = 10001;
           local pos2 = 10002;
           local pos3 = 10003;
           local pos4 = 10004;
           local pos5 = 10005;
           
           if test_cd(pos1) or test_cd(pos2) or test_cd(pos3) or test_cd(pos4) or test_cd(pos5) then
               manorButton:showTipIcon()
           else
               manorButton:hideTipIcon()
           end
		end
	end
end

local updateFun;

function addUIUpdateTimer()
	print("add update ui fun for ppve")
    print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> scheduleScriptFunc : fun 4")
	updateFun = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(manor_update, 5.0, false);
	return 1;
end

--addUIUpdateTimer()

