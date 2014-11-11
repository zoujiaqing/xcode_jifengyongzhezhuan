require("Script/GameConfig/Localization")

LanguageLocalization = {
	LanguageFlag = "SimpleChinese"
}

function LanguageLocalization:SetLocalization(szLanguageFlag)
	self.LanguageFlag = szLanguageFlag
end

function LanguageLocalization:GetLocalization(szFlag,szLanguageFlag)
	local curLanguage = self.LanguageFlag
	if nil ~= szLanguageFlag then
		curLanguage = szLanguageFlag
	end
	
	local rst = ""
	local languageItem = Localization[szFlag]
	if nil == languageItem then
		--CCLuaLog("ERROR£º¡¡LanguageLocalization:GetLocalization key: " .. szFlag .. "Not Found");
		rst = ""
	else
		rst = languageItem[curLanguage]
		if nil == rst then
			rst = ""
		end		
	end

	return rst
end

function GetLuaLocalization(szFlag)
	return LanguageLocalization:GetLocalization(szFlag)
end