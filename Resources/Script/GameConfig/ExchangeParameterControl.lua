require("Script/GameConfig/ExchangeParameter")

function getExchangeParameterCostNum(exchangeId)
        local str = ExchangeParameter[exchangeId]["FromItems"]
        local beganPos = string.find(str,"/") + 1
        local endPos = string.len(str)
        local num = string.sub(str,beganPos,endPos)
        if num == nil then
                num = 0
        end
        return num
end