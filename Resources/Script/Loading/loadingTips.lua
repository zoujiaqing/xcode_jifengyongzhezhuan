require("Script/Language")

LoadingTips = {
	minIndex = nil,
	maxIndex = nil,
	isCalRange = false,
	TipPreSuffix = "M_TIPS_",
}

function LoadingTips:CalRange()
	if self.isCalRange then
		return
	end
	self.isCalRange = true

	local index = 1
	while true do
		local flag = self.TipPreSuffix .. index
		local content = LanguageLocalization:GetLocalization(flag)
		if content ~= "" and content ~= nil then
			if self.minIndex == nil then
				self.minIndex = index
			end
			self.maxIndex = index
		else
			break
		end

		index = index + 1
	end
end


function LoadingTips:GetRandomTxt()
	self:CalRange()

	if self.minIndex ~= nil and self.maxIndex ~= nil then
		print("******************** minIndex " .. self.minIndex)
		print("******************** maxIndex " .. self.maxIndex)

		math.randomseed(tostring(os.time()):reverse():sub(1, 6))
		local index = math.random(self.minIndex,self.maxIndex)
		print("******************** index " .. index)

		local flag = self.TipPreSuffix .. index
		local content = LanguageLocalization:GetLocalization(flag)
		return content
	else
		return ""
	end	
end

function LoadingTips_CalRandomTxt()
	return LoadingTips:GetRandomTxt()
end