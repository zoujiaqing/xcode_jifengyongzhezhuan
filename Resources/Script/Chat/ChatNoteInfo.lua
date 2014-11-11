-- 聊天记录（单条）
ChatNoteInfo = {
	noteType = nil,		-- 聊天类型 "WORLD"世界 "PARTY"公会 "PM"私聊
	isSend   = true,	-- 记录类型 true 发送 false 接收
	content  = nil,		-- 聊天内容
	targetId = nil,		-- 聊天对象(当isSend为true时，为消息接收方.当isSend为false时，为消息发送者)
	dnd		 = false,	-- true 强制显示，不能屏蔽
	isSystem = false,	-- 是否为系统消息
	name	 = "",		-- 发言人
	noteIndex = 0,		-- 记录编号，前端使用
}

function ChatNoteInfo:create(extension)
	local playerinfo = setmetatable(extension or {},self)
	self.__index = self
	return playerinfo
end