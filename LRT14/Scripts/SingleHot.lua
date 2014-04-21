--[[
SingleHot.lua

Autonomous routine for firing a single hot goal.
The routine waits until either the goal in front
of it is lit or the timeout passes.
--]]

require "common"

--[[
left: -1
none/error: 0 <-- don't set STARTING_SIDE to this
right: 1
--]]
STARTING_SIDE = 1
WAIT_TIMEOUT = 8.0 -- 5.0 + 3.0 timeout
FORWARD_DIST = 78.0

start = GetTimeMillis()
drive(-1 * FORWARD_DIST)

-- if the side isn't the starting side or we haven't timed out, keep yielding back to the main code
while ( getHotGoalSide() ~= STARTING_SIDE and (GetTimeMillis() - start) < (WAIT_TIMEOUT * 1000)) do
	coroutine.yield()
end

print("Time: "..(GetTimeMillis() - start))

-- go time!
fire()

print("Routine done")