
TargetInfo = {
	targetID = 0,	-- 目标ID
	currentProgress = 0, -- 当前进度
	tatalProgress = 0, -- 完成需要的进度
	status = 2,		-- 活动状态,2未完成，3未领取，4，已领取
}

function TargetInfo:create(targetID,currentProgress,tatalProgress,status)
	local item = setmetatable({},self)
	self.__index = self
	item:init(targetID,currentProgress,tatalProgress,status)
	return item
end

function TargetInfo:init(targetID,currentProgress,tatalProgress,status)
	self.targetID = targetID
	self.currentProgress = currentProgress
	self.tatalProgress = tatalProgress
	self.status = status
end

function TargetInfo:update(currentProgress,tatalProgress,status)
	self.currentProgress = currentProgress
	self.tatalProgress = tatalProgress
	self.status = status
end

GrowthTargetDataList = {}

function setTargetDataList(targets)
	GrowthTargetDataList = {}
	for k,v in pairs(targets) do
		local curRate = 0
		local totalRate = 0
		if v.rate ~= nil then
			for _index,value in pairs(v.rate) do
				curRate = value.cur_rate
				totalRate = value.total_rate
				break
			end
		end
		--CCLuaLog("targetData: " .. v.target_id)
		local targetInfo = TargetInfo:create(v.target_id,curRate,totalRate,v.status)
		GrowthTargetDataList[v.target_id] = targetInfo
	end
end

function updateTargetInfo(targets)
	for k,v in pairs(targets) do
		local curRate = 0
		local totalRate = 0
		if v.rate ~= nil then
			for _index,value in pairs(v.rate) do
				curRate = value.cur_rate
				totalRate = value.total_rate
				break
			end
		end
		if GrowthTargetDataList[v.target_id] ~= nil then
			GrowthTargetDataList[v.target_id]:update(curRate,totalRate,v.status)
		else
			local targetInfo = TargetInfo:create(v.target_id,curRate,totalRate,v.status)
			GrowthTargetDataList[v.target_id] = targetInfo
		end
	end
end

