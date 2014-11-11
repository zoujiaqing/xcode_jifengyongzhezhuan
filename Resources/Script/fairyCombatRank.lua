require("Script/SpriteElf/fairybasicinfo")

function getFairyNameById(fairyId)
	local name = ""
	if fairys[fairyId] ~= nil then
	    name = LanguageLocalization:GetLocalization(fairys[fairyId].Name)
	end
	return name
end

function getFairyIconById(fairyId)
	local icon = ""
	if fairys[fairyId] ~= nil then
		icon = "map_ui_system_icon_FRAME_" .. string.upper(fairys[fairyId].icon)
	end
	return icon
end