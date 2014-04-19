--[[
SingleHot.lua

Autonomous routine for firing a single hot goal.
The routine waits until either the goal in front
of it is lit or the timeout passes.
--]]

require "common"

--[[
left: -1
none/error: 0
right: 1
--]]
STARTING_SIDE = -1 -- CHANGEME
WAIT_TIMEOUT = 8.0 -- 5.0 + 3.0 timeout

start = GetTimeMillis()

dribbleDrive(-36.0, 1.0)

--[[
-- if the side isn't the starting side or we haven't timed out, keep yielding back to the main code
while getHotGoalSide() ~= STARTING_SIDE and ( (GetTimeMillis() - start) < WAIT_TIMEOUT * 1000) do
	BufferedPrint("Side: "..getHotGoalSide())
	coroutine.yield()
end
--]]

-- go time!
fire()
print("Routine done")