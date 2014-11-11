
EquipEnchaseLockTable ={}

function EquipEnchaseLockTable:createEnchaseLockInfo(equipId)
	local equipTable = {}
	setmetatable(equipTable,{})
	for i=1,4 do
		equipTable[i] = false
	end
	EquipEnchaseLockTable[equipId] = equipTable
	return table
end

function EquipEnchaseLockTable:setEquipEnchaseLock(equipid,holeIndex,isLock)
	if EquipEnchaseLockTable[equipid] == nil then
		self:createEnchaseLockInfo(equipid)
	end

	local holeTable = EquipEnchaseLockTable[equipid]
	if holeTable ~= nil and holeTable[holeIndex] ~= nil then
		holeTable[holeIndex] = isLock
	
	end
end

function EquipEnchaseLockTable:getEquipEnchaseLock(equipid,holeIndex)
	if EquipEnchaseLockTable[equipid] == nil then
		self:createEnchaseLockInfo(equipid)
	end

	local isLock = false
	local holeTable = EquipEnchaseLockTable[equipid]
	if holeTable ~= nil and holeTable[holeIndex] ~= nil then
		isLock = holeTable[holeIndex]
	end
	return isLock
end