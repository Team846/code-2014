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
// Operator Joystick
// Driver Wheel
const uint32_t POSITION_HOLD = 5;
const uint32_t REVERSE_DRIVE = 6;
}
namespace DigitalIns
{
const int32_t NO_DS_DI = -1;
const uint32_t DRIVETRAIN = 1;
const uint32_t COMPRESSOR = 8;
}
namespace AnalogIns
{
const uint32_t AUTONOMOUS_DELAY = 1;
const uint32_t AUTONOMOUS_SELECT = 2;
}
}

#endif