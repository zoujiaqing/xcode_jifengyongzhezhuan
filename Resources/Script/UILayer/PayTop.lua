require("Script/CommonDefine")
require("Script/OsCommonMethod")
require("Script/GameConfig/uiTagDefine")
require("Script/handleMessage")
require("Script/MainMenuUILayer")
--require("Script/UILayer/ActivityCentre")
--require("Script/UILayer/ShowActivity")

--加载需要的文件.
KUI_PAYRANK_BIN="UI/ui2.bin"

--
--Edit by andyhuang - 2013/12/24
--

PayTopUI = {
	LayerInstance = nil,		--该UI层的拥有者.
	LayoutEnter = nil,			--当前UI层
	ClosePayTop_btn	= nil,		--关闭界面的按钮.

	--UI布局.
	Player_Cur_Top_Label = nil,	--玩家当前排名字符串标签.
	Title_Label	= nil,			--UI界面的标题栏目.
	Top_Label = nil,			--排名 字符串标签.
	Player_Label = nil,			--玩家 字符串标签.
	Pay_diamond_Label = nil,	--已付钻石 字符串标签.

	Top_Player_scrollList = nil,		--所有上了排行的玩家列表,可以滑动.

	PayRank_data = nil,			--保存 所有玩家排行数据.

	player_rank = nil,			--玩家自己的排名.

	player_pay_count = nil,		--玩家的充值数量.

	Rank_active_id = nil, 			--活动的结束时间.

	day_recharge_active = {}
};

function PayTopUI:ResetValue()			--初始化UI内的值.
	self.LayerInstance = nil
	self.LayoutEnter = nil
	self.ClosePayTop_btn = nil

	self.Player_Cur_Top_Label = nil
	self.Title_Label = nil
	self.Top_Label = nil
	self.Player_Label = nil
	self.Pay_diamond_Label = nil

	self.Top_Player_scrollList = nil

	--self.PayRank_data = nil

	--self.player_rank = nil

end;


local function onReceiveLayerDestructor()		--析构时调用的函数.
	PayTopUI:unRegisterAll()											--todo,如果需要使用定时器,就在这里取消并释放掉定时器.
	TXGUI.UIManager:sharedManager():removeUILayout("PayTopUI")			--todo,清除图层资源.
	PayTopUI:ResetValue()												--清空当前UI的值.
end;

function PayTopUI:CreateLayer(parentLayer)	--构造函数.

	print("PayTopUI:CreateLayer")	--写入日志,方便调试.
	self:ResetValue();	--初始化UI.

	if parentLayer == nil then		--确定父窗口.
		self.LayerInstance = CCLayer:create()
	else
		self.LayerInstance = parentLayer
	end

	local uiLayoutScene = TXGUI.UIManager:sharedManager():createUILayoutFromFile("UIplist/PayRankUI.plist",self.LayerInstance, "PayRankUI", true);	--todo载入资源文件.

	self.LayoutEnter = uiLayoutScene	--保存资源文件句柄,以便后面通过该句柄加载UI控件.

	if parentLayer == nil then
		local mainMenuNode = GetMainMenuLayerNode()
		local tag = UITagTable["PayRankUI"].tag				--确定PayTopUI的tag id
		mainMenuNode:addChild(self.LayerInstance,0,tag)		--将UI层挂到主菜单层上.
		InitMoneyConfigLayer(self.LayerInstance)			--分配内存?
	end

	--PushOneLayer(self.LayerInstance,"","")				--调用这个函数以后,无法隐藏.

	self.LayerInstance:registerDestructorScriptHandler(onReceiveLayerDestructor)	--注册析构函数.


	self:initPayTopUI()	--初始化UI布局.



	return self.LayerInstance;

end;



function PayTopUI:unRegisterAll()	--todo,如果需要释放定时器

end;


function PayTopUI:initPayTopUI()	--初始化界面布局todo.
	self.ClosePayTop_btn = self.LayoutEnter:FindChildObjectByName("closeBtn").__UIButton__:getMenuItemSprite();	--设定关闭按钮.

	self.Player_Cur_Top_Label = self.LayoutEnter:FindChildObjectByName("yourRankLabel").__UILabel__;	--todo 玩家当前排名字符串标签.
	self.Title_Label = 			self.LayoutEnter:FindChildObjectByName("titleLabel").__UILabel__; --todo UI界面的标题栏目.

	self.Top_Label = 			self.LayoutEnter:FindChildObjectByName("rankLabel").__UILabel__; --todo 排名 字符串标签.
	self.Player_Label =		 	self.LayoutEnter:FindChildObjectByName("playerNameLabel").__UILabel__; --todo 玩家 字符串标签.
	self.Pay_diamond_Label = 	self.LayoutEnter:FindChildObjectByName("payInAllLabel").__UILabel__; --todo 已付钻石 字符串标签.


	--[[添加一个活动时间判定.
	local Cur_time = os.date("%m-%d-%Y");
	local month = string.sub(Cur_time,1,2);
	local day   = string.sub(Cur_time,4,5);
	local year  = string.sub(Cur_time,7,10);
	local now_time = year*10000 + month*100 + day;
	--print("xxxxxxxxxxxxxxxxxxx" .. now_time);
	--local active_id = 20030;				--活动id

	local active = string.sub(ActivityCentre[self.Rank_active_id].Award_Time,1,4) * 10000 + string.sub(ActivityCentre[self.Rank_active_id].Award_Time,6,7) * 100 + string.sub(ActivityCentre[self.Rank_active_id].Award_Time,9,10);

	if (tonumber(string.sub(ActivityCentre[self.Rank_active_id].Award_Time,12,13)) == 24) then	--如果是24点结束,需要在此基础上加1天.
		active = active + 1;
	end
	--]]
	if(ActivityCentre[PayTopUI.Rank_active_id].status < 5) then	--活动结束
	--	print("andyxxxxxxxxxxxxxxxxxxxxxxxx" .. PayTopUI.Rank_active_id .. " == 5");
	--end

	--if now_time < active then			--当前是否是活动时间.

		--if self.player_rank <= 50 then
			self.Player_Cur_Top_Label:setString(GetLuaLocalization("M_LATEST_RANK") .. tostring(self.player_rank));
		--else
		--	self.Player_Cur_Top_Label:setString("您当前没有进入排行榜");
		--end
	else
		--if self.player_rank<=50 then
			self.Player_Cur_Top_Label:setString(GetLuaLocalization("M_ENDING_RANK") .. tostring(self.player_rank));
		--else
		--	self.Player_Cur_Top_Label:setString("活动已结束,您没有进入排行榜.");
		--end
	end;

	self.Title_Label:setString(GetLuaLocalization("M_RECHANGE_RANK"));
	self.Top_Label:setString(GetLuaLocalization("M_RANK"));
	self.Player_Label:setString(GetLuaLocalization("M_PALYER"));
	self.Pay_diamond_Label:setString(GetLuaLocalization("M_FILLED_DIAMOND"));

	self:setupbuttonAreaScrollist();	--填充排行榜.


	self.ClosePayTop_btn:registerScriptTapHandler(PayRank_onCloseEnter);	--注册关闭按钮的响应函数.

end;

function PayRank_onCloseEnter(pObj)
    print("PayTopUI:onCloseEnter.")

   TXGUI.UIManager:sharedManager():removeUILayout("PayRankUI")
   --SetChatBar(true,-1)
   CCTextureCache:sharedTextureCache():removeUnusedTextures()

   RemoveOneLayer(PayTopUI.LayerInstance)
   --NotificationCenter:defaultCenter():unregisterScriptObserver(lotteryUI.closeCallBackFun)

   PayTopUI.LayerInstance:removeFromParentAndCleanup(true);
   PayTopUI.LayerInstance = nil
   PayTopUI:ResetValue()

   operationUI:ShowOrNot(true);		--显示活动中心.
  --GameAudioManager:sharedManager():playEffect(SOUND_EFFECT_CLICK,false);
--    ManorUICloseBtnClick(nil)
end

--关闭UI窗口.
--[[
function ManorUICloseBtnClick(pObj)
	if nil ~= PayTopUI.LayerInstance then
		PayTopUI:unRegisterAll()
		RemoveOneLayer(PayTopUI.LayerInstance)						--从场景中删除这个层.
		TXGUI.UIManager:sharedManager():removeUILayout("PayRankUI")	--使用这个层所引用到的资源.
		CCTextureCache:sharedTextureCache():removeUnusedTextures()	--使用这个UI所引用到的字符串资源.
		PayTopUI.LayerInstance:removeFromParentAndCleanup(true);	--从父窗口中删除.
		PayTopUI.LayerInstance = nil								--将父窗口置空.
        PayTopUI:ResetValue()										--清空UI窗口的值.
	end
end
--]]

--初始化玩家排行榜.
function PayTopUI:setupbuttonAreaScrollist()

	self.ListItemButtons = {};

	self.Top_Player_scrollList = self.LayoutEnter:FindChildObjectByName("rankScrollList").__UIScrollList__;		--todo 所有上了排行的玩家列表,可以滑动.

	if nil == self.Top_Player_scrollList then
		return
	end

	self.Top_Player_scrollList:clearChildrens();		--清空排行榜.

	--开始填充排行榜单.

	for i, value in pairs(self.PayRank_data) do
		self:InsertButtonAreaOneItem(self.Top_Player_scrollList,value, i);		--循环插入数据.
	end

	self.payrank_offsize_height = self.Top_Player_scrollList:getViewSize().height;					--得到单条记录的高度.

	local rank_top_count = table.getn(self.PayRank_data);		--排行榜玩家总数.

	if self.player_rank <= 3 or rank_top_count <= 5 or self.player_rank > rank_top_count then		--玩家排名小于等于前3名.或者玩家没有上排行榜单.从第1名开始显示.
		self.Top_Player_scrollList:setStartOffset(ccp(0,0));
	elseif self.player_rank >= rank_top_count-3 then	--玩家排名为倒数前3名.从第倒数前5名开始显示.
		self.Top_Player_scrollList:setStartOffset(ccp(0, (self.payrank_offsize_height)/5 * (rank_top_count-5)));
	else							--玩家正常上榜.
		self.Top_Player_scrollList:setStartOffset(ccp(0, (self.payrank_offsize_height)/5 * (self.player_rank-3)));		--设置初始偏移值,计算方式为单条记录 * 玩家当前的排名 - 前面2个玩家..todo
	end

	if self.player_rank > rank_top_count then
		self.ListItemButtons[1]:selected();
		self.SelectItemIndex = 1;
		self.SelectedListItem = self.ListItemButtons[1];
	else
		self.ListItemButtons[self.player_rank]:selected();
		self.SelectItemIndex = self.player_rank
		self.SelectedListItem = self.ListItemButtons[self.player_rank]
	end
	--self:setupContentAreaScrollist(1)
end

function PayTopUI:InsertButtonAreaOneItem(scrollList,value, index)		--添加一个玩家的排行信息.
	if nil == scrollList then
		return
	end
	--print("PayTopUI Insert One Item " .. index)

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	--Note: 定义背景框大小
	local backGroundFrameName = "map_ui2_FRAME_PAYRANK_FRAME_PLAYERRANK";

	local normalListItemIconBgName = "map_ui2_FRAME_PAYRANK_FRAME_PLAYERRANK";
	local clickListItemIconBgName = "map_ui2_FRAME_PAYRANK_FRAME_PLAYERRANK_YOURRANK";
	local normal = AspriteManager:getInstance():getOneFrame(KUI_PAYRANK_BIN,normalListItemIconBgName)		--todo
	local clicked = AspriteManager:getInstance():getOneFrame(KUI_PAYRANK_BIN,clickListItemIconBgName)		--todo


	--self.payrank_offsize_height = AspriteManager:getInstance():getOneFrame(KUI_PAYRANK_BIN,backGroundFrameName):getContentSize().height;
	--print("******************height:" .. self.payrank_offsize_height);

	local button = scrollList:AddNewItem(normal,
		clicked,
		"",
		"",
		ccc3(255,255,255),
		-130);

	local childFrameName1 = "map_ui2_FRAME_PAYRANK_TEXT_PLAYERRANK1"		--添加按钮的字符串 排名 标签.
	local childFrameName2 = "map_ui2_FRAME_PAYRANK_ICON_PLAYERNAME1"		--添加按钮的字符串 名称 标签.
	local childFrameName3 = "map_ui2_FRAME_PAYRANK_ICON_PLAYERPAY1"			--添加按钮的字符串 钻石数量 标签.

	local pay_color = ccc3(255,195,118);
	local pay_font_size = 24;

	if index == 1 then	--前3名,用不同颜色的字体显示.
		pay_color = ccc3(255,78,0);
	elseif index == 2 then
		pay_color = ccc3(255,120,0);
	elseif index == 3 then
		pay_color = ccc3(255,156,0);
	else
		--默认的颜色和字体大小.
	end

	self:addButtonLabel(backGroundFrameName,childFrameName1,childFrameName2,childFrameName3,value,pay_font_size,pay_color,button:getAnimationNode(),10,index,kCCTextAlignmentCenter);

	local menuItem = button:getMenuItemSprite();
	menuItem:registerScriptTapHandler(onPayRankListItemClick);				--todo设置点击其中一项的响应函数,类似按钮.

	local listItemCount = table.getn(self.ListItemButtons);					--todo获得列表中按钮的数量,并将当前创建的这个放在最后面.
	self.ListItemButtons[listItemCount+1] = menuItem;

--	local listBtnCount = table.getn(self.ListButtons);						--todo获得列表中按钮的数量,并将它保存在按钮列表中.
--	self.ListButtons[listBtnCount+1] = button
end



--Note: 显示button文本函数
function PayTopUI:addButtonLabel(bgName,uiName1,uiName2,uiName3,value,fontSize,color,parentNode,zOrder,nTag,HorAligment,VerAligment)

	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

--添加排行榜名次.
	local CupFrameName = {"map_ui_system_icon_FRAME_ICON_PAY_RANK1","map_ui_system_icon_FRAME_ICON_PAY_RANK2","map_ui_system_icon_FRAME_ICON_PAY_RANK3"};
	local pLabelSprite = AspriteManager:getInstance():getFramesRelativePos(KUI_PAYRANK_BIN,bgName,uiName1,anchorPoint,relativePoint);
	local labelSize = pLabelSprite:getContentSize();
	local contentSize = CCSize(labelSize.width,labelSize.height)

	if(nTag <= 3) then
		local pChildSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",CupFrameName[nTag]);
		pChildSprite:setPosition(relativePoint);
		parentNode:addChild(pChildSprite,zOrder);
	else
		local nameLabel = parentNode:getChildByTag(nTag)				--todo,前3名需要特殊处理.加入金银铜标签图.
		if nil == nameLabel then
			nameLabel=self:newLabel(tostring(nTag),fontSize,contentSize,color,HorAligment,VerAligment)
			nameLabel:setTag(nTag)
			parentNode:addChild(nameLabel,zOrder)
		end
		nameLabel:setPosition(relativePoint)
	end

--添加排行榜名称.
	local pLabelSprite1 = AspriteManager:getInstance():getFramesRelativePos(KUI_PAYRANK_BIN,bgName,uiName2,anchorPoint,relativePoint);
	labelSize = pLabelSprite1:getContentSize();
	contentSize = CCSize(labelSize.width,labelSize.height)
	--
	nameLabel = parentNode:getChildByTag(nTag+1)
	if nil == nameLabel then
		nameLabel=self:newLabel(value.player_name,fontSize,contentSize,color,HorAligment,VerAligment)
		nameLabel:setTag(nTag+1)
		parentNode:addChild(nameLabel,zOrder)
	end
	nameLabel:setPosition(relativePoint)

--添加排行榜钻石数量.

	local pLabelSprite2 = AspriteManager:getInstance():getFramesRelativePos(KUI_PAYRANK_BIN,bgName,uiName3,anchorPoint,relativePoint);
	labelSize = pLabelSprite2:getContentSize();
	contentSize = CCSize(labelSize.width,labelSize.height)
	--
	nameLabel = parentNode:getChildByTag(nTag+2)
	if nil == nameLabel then
		nameLabel=self:newLabel(value.pay_count,fontSize,contentSize,color,HorAligment,VerAligment)
		nameLabel:setTag(nTag+2)
		parentNode:addChild(nameLabel,zOrder)
	end
	nameLabel:setPosition(relativePoint)

end



--Note: 显示文本函数
function PayTopUI:newLabel(content,fontSize,contentSize,color,HorAligment,VerAligment)
	--local relativePoint = CCPoint(0,0);
	--local contentSize = CCSize(labelSize.width,labelSize.height)
	if HorAligment == nil then
		HorAligment = kCCTextAlignmentLeft
	end
	if VerAligment == nil then
		VerAligment = kCCVerticalTextAlignmentCenter
	end

	nameLabel = TXGUI.UILabelTTF:create(content, KJLinXin, fontSize,contentSize,HorAligment,VerAligment)
	nameLabel:setColor(color)

	return nameLabel
end

--Note: 显示奖杯的代码.
function PayTopUI:addNewIcon(button,childFrameName,zOrder)
	local relativePoint = CCPoint(0,0);
	local anchorPoint = CCPoint(0.5,0.5);

	local pChildSprite = AspriteManager:getInstance():getOneFrame("UI/ui_system_icon.bin",childFrameName);
	--pChildSprite:setPosition(relativePoint);
	button:addChild(pChildSprite,zOrder);
end


--单击了排行榜中的某一项.
function onPayRankListItemClick(nTag)
--	print("************************************ ononPayRankListItemClick " .. nTag)

	local listItemIndex = nTag+1
	PayTopUI.SelectItemIndex = listItemIndex

	local item = PayTopUI.ListItemButtons[listItemIndex]

	if nil ~= PayTopUI.SelectedListItem then
		PayTopUI.SelectedListItem:unselected()
	end

	item:selected()					--高亮显示选中的这项.
	PayTopUI.SelectedListItem = item

end

