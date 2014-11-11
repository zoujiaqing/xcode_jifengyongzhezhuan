require("Script/GameConfig/vip")
require("Script/GameConfig/DebugConfig")
require("Script/GameConfig/ExchangeItem")
require("Script/Language")

if kTargetAndroid == CCApplication:sharedApplication():getTargetPlatform() then 
	--加载luaJavaBridge的基础框架组件
	CCLuaLoadChunksFromZip("framework_precompiled.zip")
	require("framework.init")
end

--获取LuaJavaBridge
local luaj = nil
function GetLuaJavaBridge()
	if kTargetAndroid == CCApplication:sharedApplication():getTargetPlatform() then 
		if luaj == nil then
			--加载luaJavaBridge的基础框架组件
			CCLuaLoadChunksFromZip("framework_precompiled.zip")
			require("framework.init")

			luaj = require("Script/luaj")
		end
	end
	return luaj
end

--==============================
--【luaJavaBridge返回错误代码】
--==============================
--	-1 	不支持的参数类型或返回值类型
--	-2 	无效的签名
--	-3 	没有找到指定的方法
--	-4 	Java 方法执行时抛出了异常
--	-5 	Java 虚拟机出错
--	-6 	Java 虚拟机出错

--==============================
--【luaJavaBridge实例】
--==============================
--[[
if kTargetAndroid == CCApplication:sharedApplication():getTargetPlatform() then 

	--返回参数有两种方式，同步及异步
	--同步：在调用了callStaticMethod方法后如果调用成功返回Java方法的返回值
	--异步：需要在调用Java方法时传递一个lua回调方法，该方法含有一个string类型的返回值

    local javaClassName = "com.taomee.adventure.adventure"				--类名
    local javaMethodName = "showAlertDialog"							--方法名
    local javaParams = {												--参数：与方法签名一致，Lua Function以引用ID（int类型）方式传递
        "How are you ?",
        "I'm great !",
        function(event)
            --回调函数，event为回调时传递的返回值（string类型）
			--实例中返回值为"CLICKED"
        end
    }
    local javaMethodSig = "(Ljava/lang/String;Ljava/lang/String;I)I"	--方法签名：与参数一致，Lua Function以引用ID（int类型）方式传递

	--ok：true/false 调用是否成功
	--ret: ok为true时：ret为Java方法的返回值
		   ok为false时：ret为错误代码（具体错误代码见【luaJavaBridge返回错误代码】）
    local ok, ret = GetLuaJavaBridge().callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)

	if ok then
		--调用成功，ret为Java返回值
		--实例中返回值为123
	else
		--调用失败，ret为错误代码
		--具体错误代码见【luaJavaBridge返回错误代码】
	end

end
--]]

local visionNum = "1.3.0";
function getVisionNum()
	return LanguageLocalization:GetLocalization("M_VERSION")..visionNum;
end


local nowPowerBuyCount = 0
function RefreshPowerBuyCount(count)
	nowPowerBuyCount = count
end

function getNowPowerBuyCount()
	return nowPowerBuyCount
end

function getRemainPowerBuyCount()
	local vipLevel = UserData:GetVipLevel()
	local maxPowerBuyCount = Vip[vipLevel].Power

	return maxPowerBuyCount - nowPowerBuyCount
end

-- 开启自动战斗的相关函数
local canAutoFight = false
local autoFightVIP = 0
local autoFightLevel = 0

function GetAutoFightState()
    if GetEnableAutoFight() then
        return true
    end
	return canAutoFight
end

function GetAutoFightText()
	return ""..autoFightLevel..GetLuaLocalization("M_FINDENEMY_2")
end

local function sendAutoFightCheckReq()
	CSAutoBattleCheckReq = {
		}
	local msgname="CSAutoBattleCheckReq"
	local ret = send_message(msgname, CSAutoBattleCheckReq, true)
	return ret
end
	
local function onMsgAutoFightCheckRsp(msg)
	canAutoFight = (msg.on_or_off == 1)
	autoFightVIP = msg.vip_lv
	autoFightLevel = msg.lv
end

function InitAutoFightInfo()
	CCLuaLog("---- InitAutoFightInfo ---")

	sendAutoFightCheckReq()
	addMsgCallBack("CSAutoBattleCheckRsp", onMsgAutoFightCheckRsp)

	return true
end

--刷新卡及魂石数量
local refreshCardCount = 0
local soulStoneCount = 0

function UpdateRefreshCardCount(count)
	refreshCardCount = count
end

function UpdateSoulStoneCount(count)
	soulStoneCount = count
end

function GetRefreshCardCount()
	return refreshCardCount
end

function GetSoulStoneCount()
	return soulStoneCount
end

-- 声望商店及最强勇者积分兑换商店次数处理
local exchangeItemTimes = {}
function UpdateExchangeTimes(attrId, value)
    local exchangeId = attrId - PB_ATTR_EXCHANGE_TIMES_BEGIN + 250

    CCLuaLog("------------------------------- UpdateExchangeTimes : exchangeId "..exchangeId)
    exchangeItemTimes[exchangeId] = value
end

-- 额外返回兑换类型 ： exchangeType : 0 不限次，1 每天， 2 总共
function GetItemExchangeRemainTime(exchangeId)
    CCLuaLog("GetItemExchangeRemainTime : exchangeId "..exchangeId)
    local itemTable = ExchangeItems[exchangeId]

    local exchangeType = 0
    if tonumber(itemTable.Restriction) ~= 0 then
        exchangeType = 1
    elseif tonumber(itemTable.Lifelong_Restriction) ~= 0 then
        exchangeType = 2
    end

    local maxTime = -1
    if exchangeType == 1 then
        maxTime = tonumber(itemTable.Restriction)
    elseif exchangeType == 2 then
        maxTime = tonumber(itemTable.Lifelong_Restriction)
    end
    local nowTime = exchangeItemTimes[exchangeId] or 0

    return maxTime - nowTime, exchangeType
end

function ResetReputationExchangeTime()
    exchangeItemTimes = {}
end