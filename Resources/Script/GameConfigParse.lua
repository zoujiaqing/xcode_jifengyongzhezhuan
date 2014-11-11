require("Script/GameConfig")
require("Script/GameConfig/Recharge")
require("Script/svn_version")
function GetChannelID()
	return G_GameChannelID
end

function GetGameID()
	return G_GameID
end

function GetMainVersion()
	return G_GameMainVersion
end

function GetNetWorkFlag()
	return G_GameNetWorkFlag
end

function GetDeviceID()
	return "pc_1001"
end

function GetLaunchStatUrl()
	return "http://ck_stat.61.com/version/stat/ck_stat.php"
end

function GetNetWorkIP()
	if G_GameNetWorkFlag == "InnerNet" then
		return G_GameNetWorkInnerIP
	elseif G_GameNetWorkFlag == "OutNet" then
		return G_GameNetWorkOutIP
	elseif G_GameNetWorkFlag == "AllOutNet" then
		return G_GameNetWorkAllOutIP
	end
end

function GetNetWorkPortID()
	if G_GameNetWorkFlag == "InnerNet" then
		return G_GameNetWorkInnerPortId
	elseif G_GameNetWorkFlag == "OutNet" then
		return G_GameNetWorkOutPortId
	elseif G_GameNetWorkFlag == "AllOutNet" then
		return G_GameNetWorkAllOutPortId
	end
end

--"InnerNet" or "OutNet" or "AllOutNet"
function GetResorceDownLoadURL()
	if G_GameNetWorkFlag == "InnerNet" then
		return G_GameNetWorkUpdateResInnerUrl
	elseif G_GameNetWorkFlag == "OutNet" then
		return G_GameNetWorkUpdateResOutUrl
	elseif G_GameNetWorkFlag == "AllOutNet" then
		return G_GameNetWorkUpdateResAllOutUrl
	end
end

function GetServerListURL()
	if G_GameNetWorkFlag == "InnerNet" then
		return G_GameNetWorkGetServerLstInnerUrl
	elseif G_GameNetWorkFlag == "OutNet" then
		return G_GameNetWorkGetServerLstOutUrl
	elseif G_GameNetWorkFlag == "AllOutNet" then
		return G_GameNetWorkGetServerLstAllOutUrl
	end
end

function GetStateURL()
	if G_GameNetWorkFlag == "InnerNet" then
		return G_GameNetWorkGetStateInnerUrl
	elseif G_GameNetWorkFlag == "OutNet" then
		return G_GameNetWorkGetStateOutUrl
	elseif G_GameNetWorkFlag == "AllOutNet" then
		return G_GameNetWorkGetStateAllOutUrl
	end
end

function GetNetWorkServerConfigURL()
	return G_GameNetWorkGetServerConfigUrl
end

function GetNetWorkServerConfigURL2()
	return G_GameNetWorkGetServerOtherConfigUrl
end

function GetDownLoadShopItemInfoConfigURL()
	local index = tonumber(G_GameChannelID)
	if Recharge[index] ~= nil then
		local a = "http://wlpay.shootao.com/iap/index.php?m=ahero&a=getItemJson&category="..Recharge[index].manu_id.."&version="..getsvn()
		print(a)
		return a
	else
		local ostype = CCApplication:sharedApplication():getTargetPlatform()
		print("Recharge is null for "..index)
		if ostype == kTargetAndroid then 
			return "http://wlpay.shootao.com/iap/index.php?m=ahero&a=getItemJson&category=4".."&version="..getsvn()
		else
			return "http://wlpay.shootao.com/iap/index.php?m=ahero&a=getItemJson&category=1".."&version="..getsvn()
		end
	end
	
end
