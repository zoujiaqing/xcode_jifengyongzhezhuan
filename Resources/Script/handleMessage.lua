require("Script/protobuf")
require("Script/errcode")

function DecodeCSQueryShardRsp(msg)
	
	for _,v in ipairs(msg.shards) do
	print("\t"..v.id, v.os, v.provider)
		for _, vv in ipairs(v.svrs) do
			--print("\t\t"..vv.desc, vv.ip, vv.port, vv.online, vv.newsvr)
		end
	end
	return false;
end


--------------------------------------------------------------------------
-- 消息分发表， 在这里增加回调函数
--------------------------------------------------------------------------
MsgDispatcher =
{
	--key = 消息处理回调函数
	CSErrorCode={{cb = printErrStr, o = nil}}
	
}

function printMsgTable()
	 --print("*****Msg Dispatcher table*******")
	 for i, v in pairs(MsgDispatcher) do
	 
		--print(i.." "..type(i).." => {")
		for ii, vv in pairs(v) do
			--print("\t("..type(vv.cb).." "..type(vv.o)..")")
		end -- end inner for
		--print("}")
	end --end outer for
end

function addMsgCallBack(msgName, callbackFunc, object)
	--CCLuaLog("addMsgCallBack: " .. msgName)
	if MsgDispatcher[msgName] == nil then
		MsgDispatcher[msgName]={}
	end
	for i, v in pairs(MsgDispatcher[msgName]) do 
		----print(i, v)
		
		if(v.cb == callbackFunc) then
			--print("addMsgCallBack, already have this msg handler:"..msgName)
			return
		end
		
	end	
	table.insert(MsgDispatcher[msgName], {cb = callbackFunc, o = object})
	--printMsgTable()
end

function removeMsgCallBack(msgName, callbackFunc)
	if MsgDispatcher[msgName] == nil then
		return
	end
	for i, v in ipairs(MsgDispatcher[msgName]) do 
		----print(i, v)
		
		if(v.cb == callbackFunc) then
			table.remove(MsgDispatcher[msgName],i)
			break;
		end
		
	end	
	if #MsgDispatcher[msgName] == 0 then
	   MsgDispatcher[msgName] = nil
	end
	--printMsgTable()
end
--------------------------------------------------------------------------
-- main function
--------------------------------------------------------------------------
--check if this msg needed processed by lua. cost too much cpu? this can put it in c++ for 
function processedByLua(cmd)
	local ret = MsgDispatcher[cmd]~=nil
	
	return ret
end

--需要先注册该消息所在的proto 文件，proto 文件已经被编译为二进制格式，统一放在Script/Pbc/ 目录下
--修改了make_proto.bat脚本 ，可自动生成，如果后面服务器端proto有改变，运行一下make_proto.bat脚本即可
function registerAllfile()
	
	local pbfiles={"head.bin","cs_basic.bin", "cs_core.bin", "db.bin", "errcode.bin", "cs_battle.bin", "cs_config.bin"}
	for _, v in ipairs(pbfiles) do
		
		local path = GameResourceManager:sharedManager():storedFullPathFromRelativePath("Script/Pbc/"..v)
		print("register file ".. path)
		local addr = io.open(path,"rb")
		if addr == nil then
			return 0
		end
		local buffer = addr:read "*a"
		addr:close()
		protobuf.register(buffer)
	end
	return 1
end

function registerfile(buff)
		protobuf.register(buff)
end

local currentSendMsgname
local needCache

function encode_callback(buff, len)
	--print("lua send message:"..currentSendMsgname);
	OnlineNetworkManager:sShareInstance():sendMessage(currentSendMsgname, buff, len, needCache)
end

	--各个参数意义
	--msgName 消息名称，和.proto文件中消息名称一致
	--msgTable, 构造消息，repeat字段需要用一个表来表示，即使只有一项，如下面的param字段
	--更多如何构造消息的内容，请参考例子Script/Pbc/test.lua
	--cache 当消息发不出去时，是否缓存该消息，网络正常后再发
function send_message(msgName, msgTable, cache)
	currentSendMsgname=msgName
	needCache=cache
	--print("lua encode message:"..msgName);
	local ret = protobuf.encode(msgName, msgTable, encode_callback)
	return ret
end

--msg dispacher
function process_message()

	local msgPackage = MessageHandler:sShareInstance():getWildMsgPackage()

	local cmd = msgPackage:GetCmd();
	CCLuaLog("receive wild message: cmd = ".. cmd)
	local ret=false

	if MsgDispatcher[cmd]~=nil then
		local s = msgPackage:ReadBuff();

		decode = protobuf.decode(cmd , s)
		ret = false;
		for i, v in pairs(MsgDispatcher[cmd]) do 
			----print(i, v)
			ret = false
			if v.o == nil and v.cb ~= nil then
			    ret = v.cb(decode)
			elseif v.cb ~= nil then
			    ret = v.cb(v.o, decode)
			end
			if ret == true then
				--print("lua success process msg"..cmd)
				break
			end
		end
		--MsgDispatcher[cmd](decode)

		--print("did not find the msg func callback")

	end
	return ret;
end

