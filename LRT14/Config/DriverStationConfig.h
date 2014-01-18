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
const uint32_t NUM_JOYSTICK_BUTTONS = 12;// Max supported by the driver station software. 
										// Having more than 12 can cause a crash in very particular scenarios involving broken USB hubs. 
const uint32_t NUM_JOYSTICK_AXES = 6;
const uint32_t NUM_WHEEL_BUTTONS = 11;

const uint32_t NUM_WHEEL_AXES = 3;
const uint32_t DRIVER_STICK_PORT = 1;
const uint32_t OPERATOR_STICK_PORT = 2;
const uint32_t DRIVER_WHEEL_PORT = 3;
}
namespace JoystickButtons
{
//const uint32_t FORCE_CLIMB_ADVANCE= 4;

const uint32_t VECTOR_DRIVE = 8;
const uint32_t AUTO_AIM_BACKBOARD = 9;
//const uint32_t FEEDER_STATION_APPROACH = 3;
const uint32_t APPLY_CONFIG = 5;
const uint32_t LOAD_CONFIG = 6;
const uint32_t SAVE_CONFIG = 7;
const uint32_t RESET_ZERO = 10;
const uint32_t PAUSE_AUTON = 11;

const uint32_t STOP_ROBOT = 5;
const uint32_t REVERSE_DRIVE = 6;
}
namespace DigitalIns
{
const uint32_t DRIVETRAIN = 1;
const uint32_t COLLECTOR_ROLLERS = 2;
const uint32_t COLLECTOR_ARM = 3;
const uint32_t COMPRESSOR = 8;


}
namespace AnalogIns
{
const uint32_t AUTONOMOUS_DELAY = 1;
const uint32_t AUTONOMOUS_SELECT = 2;
const uint32_t NUM_FRISBEES = 3;
}
}

#endif
