--[[
common.lua

Common functions.
--]]

-- Returns -1 for left, 0 for none (or error), and 1 for right
function getHotGoalSide()
	local left = getHotLeftStatus()
	local right = getHotRightStatus()
	
	if left and right then -- clearly something is wrong...
		return 0
	elseif left then
		return -1
	elseif right then
		return 1
	end
	
	-- neither are lit
	return 0
end