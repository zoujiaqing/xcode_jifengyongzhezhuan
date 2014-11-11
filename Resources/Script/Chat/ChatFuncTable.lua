ChatFuncTable = {

}

ChatTagFuncTable = {

}

function getFuncTable(funIndex)
	return ChatFuncTable[funIndex]
end

function setFuncTable(funIndex, obj)
	ChatFuncTable[funIndex] = obj
end

function removeFuncTable(funIndex)
	ChatFuncTable[funIndex] = nil
end

function getTagFunc(tag)
	return ChatTagFuncTable[tag]
end

function setTagFunc(tag, obj)
	ChatTagFuncTable[tag] = obj
end

function removeTagFunc(tag)
	ChatTagFuncTable[tag] = nil
end