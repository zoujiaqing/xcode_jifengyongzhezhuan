require("Script/Language")
local svn="45000"
function getsvn()
return svn
end

function getresversion()
	return LanguageLocalization:GetLocalization("M_RESOURS")..svn
end