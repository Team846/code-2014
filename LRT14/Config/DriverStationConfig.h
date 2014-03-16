#ifndef DRIVER_STATION_CONFIG_H_
#define DRIVER_STATION_CONFIG_H_

#include <string>

/*!
 * @brief Contains all constants for driver station configuration.
 */
namespace DriverStationConfig
{
namespace JoystickConfig
{
const uint32_t NUM_JOYSTICK_BUTTONS = 12; // Max supported by the driver station
const uint32_t NUM_JOYSTICK_AXES = 6;
const uint32_t NUM_WHEEL_BUTTONS = 11;
const uint32_t NUM_WHEEL_AXES = 3;

const uint32_t DRIVER_STICK_PORT = 1;
const uint32_t OPERATOR_STICK_PORT = 2;
const uint32_t DRIVER_WHEEL_PORT = 3;
}
namespace JoystickButtons
{
// Driver Joystick
const uint32_t COLLECT = 1;
const uint32_t DRIBBLE = 2;
// Operator Joystick
const uint32_t PASS = 1;
const uint32_t LOAD_LAUNCHER = 2;
const uint32_t SHORT_SHOT = 3;
const uint32_t LONG_SHOT = 4;
const uint32_t PURGE_LAUNCHER = 6;
const uint32_t HUMAN_LOAD = 7;
const uint32_t UNLOAD_LAUNCHER = 7;
const uint32_t PRESSURE_PLATE = 8;
const uint32_t COLLECTOR_DOWN = 9;
const uint32_t RUN_ROLLERS = 10;
const uint32_t REVERSE_ROLLERS = 11;
// Driver Wheel
const uint32_t POSITION_HOLD = 5;
const uint32_t REVERSE_DRIVE = 6;
}
namespace DigitalIns
{
const int32_t NO_DS_DI = -1;
const uint32_t DRIVETRAIN = 1;
const uint32_t COLLECTOR_ROLLERS = 2;
const uint32_t COLLECTOR_ARM = 3;
const uint32_t LAUNCHER_LOADER = 4;
const uint32_t LAUNCHER_ANGLE = 5;
const uint32_t PRESSURE_PLATE = 6;
const uint32_t COMPRESSOR = 8;
}
namespace AnalogIns
{
const uint32_t AUTONOMOUS_DELAY = 1;
const uint32_t AUTONOMOUS_SELECT = 2;
}
}

#endif
