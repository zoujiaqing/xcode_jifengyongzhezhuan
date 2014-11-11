local NetWorkConfigTable = {
	OutNet = {"211.151.105.176",15711,"http://211.151.105.176/ahero_svc_list/updateversion.php","http://211.151.105.176/ahero_svc_list/listserver.php","211.151.105.176/ahero_svc_list/frontdetect.php"},
	InnerNet = {"10.1.1.155",5015,"http://10.1.1.155/server/updateversion.php","http://10.1.1.155/server/listserver.php","http://10.1.1.155/detect/frontstat.php"},
	AllOutNet = {"211.151.105.169",9002,"http://211.151.105.169/server/updateversion.php","http://211.151.105.169/server/listserver.php","211.151.105.169/server/frontstat.php"}
}


local netType = "InnerNet"
--local netType = "OutNet"
--local netType = "AllOutNet"

function GetServerIp()
	return NetWorkConfigTable[netType][1]
end

function GetServerPort()
	return NetWorkConfigTable[netType][2]
end

function GetUpdateResourceURL()
	return NetWorkConfigTable[netType][3]
end

function GetServerListURL()
	return NetWorkConfigTable[netType][4]
end

function GetStatURL()
	return NetWorkConfigTable[netType][5]
end