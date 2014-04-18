--[[
SingleHot.lua

Autonomous routine for firing a single hot goal.
The routine waits until either the goal in front
of it is lit or the timeout passes.
--]]

Common = require "common"

--[[
left: -1
none/error: 0
right: 1
--]]
STARTING_SIDE = -1 -- CHANGEME
WAIT_TIMEOUT = 8.0 -- 5.0 + 3.0 timeout

side = Common.getHotGoalSide()
start = os.clock()

-- if the side isn't the starting side or we haven't timed out, keep yielding back to the main code
while side ~= STARTING_SIDE and ( (os.clock() - start) < WAIT_TIMEOUT) do
	coroutine.yield()
end

-- go time!
dribble(36.0, 1.0)
wait(0.5)
fire()