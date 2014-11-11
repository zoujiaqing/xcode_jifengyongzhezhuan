--[[ 
@file:ppveUILayer.lua
@description 
    ppve game hall, show the list of all the rooms, click one room to join.

@author:
     jameshuang 
@date
     2013/2/21
--]]
-- bind create room button
require("Script/GameConfig/InstanceList")
require("Script/Chat/ChatDataCenter")

---------------------------------
--
--  init all Guild map
--
function InitGuildInstance()
     local i, val
     for i =295001, 296000 do

		print(i)
		if instances[i] == nil then
			print("break")
			break
		end

       	if instances[i]['Type'] == '15' then	
       	    local reqLevel = instances[i]['RequiredLevel']
			GameApi:InsertGuildInstance(i, reqLevel)
		end  
     end          
end

