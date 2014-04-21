--[[
DoubleHot.lua

Autonomous routine for firing a two hot goals.
The robot starts in the center and drives into
the colored zone for mobility points, turns
towards the first hot goal, fires, reloads, then
turns to the other hot goal and fires.
--]]

require "common"

-- == CONFIGURABLE CONSTANTS == --
GOAL_SIDE_PADDING = 48.0 -- inches, distance from center of goals
SHOOTING_RANGE = 138.0 -- inches, distance to shoot


-- == Routine == --
routineStartTime = GetTimeMillis()

TURN_ANGLE = math.asin(GOAL_SIDE_PADDING / SHOOTING_RANGE) * 180.0 / math.pi -- calculate turn angle from params
DRIVE_DISTANCE = 216.0 - math.sqrt(SHOOTING_RANGE * SHOOTING_RANGE - GOAL_SIDE_PADDING * GOAL_SIDE_PADDING)

-- -1: Left
--  0: Center
--  1: Right
heading = 0 -- neutral position

function defaultTwoBall()
	turn(TURN_ANGLE)
	fire(true)
	collect()
	loadLauncher()
	collectorMove(true)
	pause(0.5)
	fire()
end

function shootHotGoal()
	local side = getHotGoalSide()
	local holdDown = true

	if(heading == 0) then
		if side == 0 then
			return false
		end
	elseif heading == -1 then
		side = 1
		holdDown = false
	elseif heading == 1 then
		side = -1
		holdDown = false
	end

	print("Hot goal detected")
	local turnAngle = side * TURN_ANGLE * -1

	turn(turnAngle)
	fire(holdDown)
	BufferedPrint("FIRED")

	heading = side

	return true
end

function centerRobot()
	local turnAngle = (0 - heading) * TURN_ANGLE * -1

	turn(turnAngle)
end

--[[
beginActionGroup("collect")
	collect()
endActionGroup()

beginActionGroup("drive6")
	drive(6.0)
endActionGroup()

beginActionGroup("drive-6")
	drive(-6.0)
endActionGroup()
--]]

turn(0.0)
dribbleDrive(-1 * DRIVE_DISTANCE, 1.0)
collectorMove(true)
--turn(TURN_ANGLE)
--pause(1.0)
--fire(true)
if shootHotGoal() then
	collectorMove(true)
	--turn(-1 * TURN_ANGLE)
	centerRobot() -- back to neutral pos to pick up ball
	collect()
	loadLauncher()
	collectorMove(true)
	pause(0.5)
	--turn(-1 * TURN_ANGLE)
	--fire()
	shootHotGoal()
else
	defaultTwoBall()
end

BufferedPrint("Routine completed in "..((GetTimeMillis() - routineStartTime) / 1000.0).."s.")
