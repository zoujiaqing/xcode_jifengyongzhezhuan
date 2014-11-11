
-- UI帮忙系统UI面板和ID映射
HelpUIDefine = {
	
}

local recentUIHelpIndex = 0

HelpUIDefine["AlchemyUILayer"] = { ID = 1}
HelpUIDefine["ElfExploerLayer"] = { ID = 2}
HelpUIDefine["CardUILayer"] = { ID = 3}
HelpUIDefine["EnchaseUILayer"] = { ID = 4}
HelpUIDefine["SpriteElfLayer"] = { ID = 5}


function setRecentHelpIndex(index)
	local isSuccess = false
	if HelpUIDefine[index] ~= nil then
		recentUIHelpIndex = HelpUIDefine[index].ID
		isSuccess = true
	end
	return isSuccess
end

function getRecentHelpIndex()
	return recentUIHelpIndex
end