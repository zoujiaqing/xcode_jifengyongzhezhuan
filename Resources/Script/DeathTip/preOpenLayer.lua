
local layerName = ""

function setPreOpenLayerName(name)
	layerName = name
	return true
end

function getPreOpenLayerName()
	return layerName
end