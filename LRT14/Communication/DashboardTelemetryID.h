#ifndef DASHBOARD_TELEMETRY_ID_H_
#define DASHBOARD_TELEMETRY_ID_H_

namespace DashboardTelemetryID
{
	enum Enum
	{
        COLLECTOR_ROLLERS_SPEED = 0x00,
        DRIVETRAIN_LEFT_OUTPUT = 0x01,
        DRIVETRAIN_RIGHT_OUTPUT = 0x02,
        COLLECTOR_ARMS_DESIRED_POSITION = 0x03,
        COLLECTOR_ARMS_CURRENT_POSITION = 0x04,
        LAUNCHER_ANGLE_STATE = 0x05,
        LAUNCHER_ANGLE = 0x06,
        LAUNCHER_LOADER_SAFETY = 0x07,
        PRESSURE_PLATE_STATE = 0x08,
        AUTON_HOT_GOAL_LAST_SIDE = 0x09,
        HOT_GOAL_SIDE = 0x0A,
        COLLECTOR_ARM_INPUT = 0x0B,
        COLLECTOR_ROLLERS_INPUT = 0x0C,
        DRIVETRAIN_INPUTS = 0x0D,
        LAUNCHER_ANGLE_INPUT = 0x0E,
        LAUNCHER_LOADER_INPUT = 0x0F,
        PRESSURE_PLATE_INPUT = 0x10
        
        // 28,000+ reserved for sensors
	};
}

#endif
