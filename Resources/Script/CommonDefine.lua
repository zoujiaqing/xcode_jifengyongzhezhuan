require("Script/FairyLayer/FairyLevelUpLayer")
require("Script/Instance/HeroInstanceWinLayer")

G_IsUseLuaLocal = false

KUI_BIN = "UI/ui.bin"
KUI_BIN_2 = "UI/ui2.bin"
KICON_BIN = "UI/ui_system_icon.bin"
KDailyObjective_BIN = "UI/ui_dailyobjective.bin"
KUI_VIP_BIN="UI/ui_vip.bin"
KJLinXin = "STHeitiSC-Medium"

G_CurLayerInstance = nil

--G_PvAILayerTag = 72001

function SetLayerInstance(instance)
	print("function SetLayerInstance(instance)")
	G_CurLayerInstance = instance
end

--云风oo解决方案
local _class={}
 
function class(super)
	local class_type={}
	class_type.ctor=false
	class_type.super=super
	class_type.new=function(...) 
			local obj={}
			do
				local create
				create = function(c,...)
					if c.super then
						create(c.super,...)
					end
					if c.ctor then
						c.ctor(obj,...)
					end
				end
 
				create(class_type,...)
			end
			setmetatable(obj,{ __index=_class[class_type] })
			return obj
		end
	local vtbl={}
	_class[class_type]=vtbl
 
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
		end
	})
 
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end

local enableDebugMgr = false

DebugMgr = {}

function DebugMgr:ExecDebugCmd(cmd)
	if not enableDebugMgr then
		return false
	end
	self:DoString(cmd)
	return true
end

function DebugMgr:ReloadFile(fullFileName)
	if package.loaded[fullFileName]  == nil then
		GameApi:showMessage("not loaded "..fullFileName)
		return
	end

	package.loaded[fullFileName]  = nil
	require(fullFileName)
end

function DebugMgr:DoString(debugString)
	local fairyExpSpeed = string.match(debugString, "fairy expspeed (%d+)")
	if fairyExpSpeed ~= nil then
		FairyLevelUpLayer:DebugExpSpeed(fairyExpSpeed)	
        GameApi:showMessage("精灵升级面板经验条速度设置成功")
		return	
	end

    local heroDelay = {}
    heroDelay[1], heroDelay[2], heroDelay[3], heroDelay[4], heroDelay[5], heroDelay[6], heroDelay[7] = string.match(debugString, "herodelay (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+)")
    if heroDelay[1] ~= nil then
        for i = 1, 7 do
            HeroInstanceWinLayer.delayTimes[i] = heroDelay[i] / 1000
        end
        GameApi:showMessage("勇者试炼结算面板速度设置成功")
        return
    end

    local heroRoundNum = string.match(debugString, "herorollnum (%d+)")
    if heroRoundNum ~= nil then
        HeroInstanceWinLayer.rollRoundNum = heroRoundNum
        GameApi:showMessage("勇者试炼结算面板倍率翻滚次数设置成功")
        return
    end

    local heroRollTime = string.match(debugString, "herorolltime (%d+)")
    if heroRollTime ~= nil then
        HeroInstanceWinLayer.rollSingleTime = heroRollTime / 1000
        GameApi:showMessage("勇者试炼结算面板倍率翻滚时间设置成功")
        return
    end

    GameApi:showMessage("调试命令格式错误")
	return false
	--[[
	local func = loadstring(debugString)
	if func ~= nil then
		func()
	else
		GameApi:showMessage("cmd error")
	end
	]]
end