require("Script/OsCommonMethod")

BuyDiamondDataCenter = {
	Items = {},
}

function BuyDiamondDataCenter:ResetValue()
	self.Items = {}
end

function BuyDiamondDataCenter:InsertOneItem(index)
	local count = table.getn(self.Items)
	self.Items[count+1] = index
end

function BuyDiamondDataCenter:IsFirstChange()
	return false
end

function BuyDiamondDataCenter:GetFirstChangeMagnification()
	return 2
end

----------------------------------------------
function BuyDiamondDataCenterResetValue()
	BuyDiamondDataCenter:ResetValue()
	return true
end

function BuyDiamondDataCenterInsertOneItem(index)
	BuyDiamondDataCenter:InsertOneItem(index)
	return true
end