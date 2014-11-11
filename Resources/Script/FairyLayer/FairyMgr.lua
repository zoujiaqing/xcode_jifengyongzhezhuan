--精灵相关的辅助函数
require("Script/Fairy/FairyDataCenter")
require("Script/Fairy/FairyConfigTable")

--[[

	PB_ITEM_ATTR_PHYATK        =   1;  // 物理攻击(1)
    PB_ITEM_ATTR_MAGATK        =   2;  // 魔法攻击(2)
    PB_ITEM_ATTR_SKIATK        =   3;  // 技能攻击(3)
    PB_ITEM_ATTR_PHYDEF        =   4;  // 物理防御(4)
    PB_ITEM_ATTR_MAGDEF        =   5;  // 魔法防御(5)
    PB_ITEM_ATTR_SKIDEF        =   6;  // 技能防御(6)
    PB_ITEM_ATTR_HP            =   7;  // 生命值(7)
    PB_ITEM_ATTR_ACCU          =   8;  // 精准(8)
    PB_ITEM_ATTR_DODG          =   9;  // 闪避(9)
]]

--[[
标签属性顺序:
1. 物攻（魔攻）
2. 物防
3. 魔防
4. 技攻
5. 技防
6. 生命

]]

function RefreshAttrPanel(layout, nowAttrTable, nextAttrTable)
	if layout == nil then
		return
	end

	-- 更改物攻或魔攻显示
	local userType = UserData:GetUserType()

	--魔法师显示魔攻标签
	if userType == 3 or userType == 4 then
		layout:FindChildObjectByName("attrName1").__UILabel__:setString(GetLuaLocalization("M_FAIRY_LU_ATTRIBUTE7"))
	else
		layout:FindChildObjectByName("attrName1").__UILabel__:setString(GetLuaLocalization("M_FAIRY_LU_ATTRIBUTE1"))
	end

	-- 更新当前属性
	if nowAttrTable ~= nil then
		CCLuaLog("--- nowAttrTable size : "..#nowAttrTable)

		for key, value in pairs(nowAttrTable) do
			CCLuaLog("key : "..key.." value : "..value)
		end

		local normalAttackNum = 0
		if userType == 3 or userType == 4 then
			normalAttackNum = nowAttrTable[2]
		else
			normalAttackNum = nowAttrTable[1]
		end

		layout:FindChildObjectByName("attrNum1").__UILabel__:setString(""..(normalAttackNum or 0))
		layout:FindChildObjectByName("attrNum2").__UILabel__:setString(""..(nowAttrTable[4] or 0))
		layout:FindChildObjectByName("attrNum3").__UILabel__:setString(""..(nowAttrTable[5] or 0))
		layout:FindChildObjectByName("attrNum4").__UILabel__:setString(""..(nowAttrTable[3] or 0))
		layout:FindChildObjectByName("attrNum5").__UILabel__:setString(""..(nowAttrTable[6] or 0))
		layout:FindChildObjectByName("attrNum6").__UILabel__:setString(""..(nowAttrTable[7] or 0))
	end

	-- 更新升级后属性
	if nextAttrTable ~= nil then
		CCLuaLog("--- nextAttrTable size : "..#nextAttrTable)
		for key, value in pairs(nextAttrTable) do
			CCLuaLog("key : "..key.." value : "..value)
		end

		local normalAttackNum = 0
		if userType == 3 or userType == 4 then
			normalAttackNum = nextAttrTable[2]
		else
			normalAttackNum = nextAttrTable[1]
		end

		layout:FindChildObjectByName("attrUpNum1").__UILabel__:setString(""..(normalAttackNum or 0))
		layout:FindChildObjectByName("attrUpNum2").__UILabel__:setString(""..(nextAttrTable[4] or 0))
		layout:FindChildObjectByName("attrUpNum3").__UILabel__:setString(""..(nextAttrTable[5] or 0))
		layout:FindChildObjectByName("attrUpNum4").__UILabel__:setString(""..(nextAttrTable[3] or 0))
		layout:FindChildObjectByName("attrUpNum5").__UILabel__:setString(""..(nextAttrTable[6] or 0))
		layout:FindChildObjectByName("attrUpNum6").__UILabel__:setString(""..(nextAttrTable[7] or 0))
	end
end