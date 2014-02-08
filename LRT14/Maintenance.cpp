#include "Maintenance.h"
#include "Actuators/Actuator.h"
#include "Actuators/LRTSpeedController.h"
#include "DriverStation/LRTDriverStation.h"
#include "Utils/AsyncPrinter.h"
#include "Utils/Util.h"
#include "Actuators/Pneumatics.h"
#include "Actuators/LRTServo.h"
#include <string>

void Maintenance::Update()
{
    AsyncPrinter::Printf("----------Maintenance Mode----------\n");
    for (unsigned int i = 0; i < Actuator::actuator_vector.size(); i++)
    {
        std::string line;
        DebouncedJoystick* stick;
        int button = (i + 1) % 13;
        if (i <= 12)
        {
            stick = LRTDriverStation::Instance()->GetDriverStick();
            line = "D";
        }
        else
        {
            stick = LRTDriverStation::Instance()->GetOperatorStick();
            line = "O";
        }
        line += Util::ToString(button) + "\t" + std::string(Actuator::actuator_vector[i]->GetName()) + "\t";
        if (dynamic_cast<LRTSpeedController*>(Actuator::actuator_vector[i]))
        {
            float val;
            val = stick->IsButtonDown(button) ? stick->GetAxis(Joystick::kYAxis) : 0.0;
            	dynamic_cast<LRTSpeedController*>(Actuator::actuator_vector[i])->SetDutyCycle(val);
            line += Util::ToString(val);
        }
        else if (dynamic_cast<Pneumatics*>(Actuator::actuator_vector[i]))
        {
            if (stick->IsButtonJustPressed(button))
            {
                if (dynamic_cast<Pneumatics*>(Actuator::actuator_vector[i])->Get() == Pneumatics::FORWARD)
                {
                	dynamic_cast<Pneumatics*>(Actuator::actuator_vector[i])->Set(Pneumatics::REVERSE);
                }
                else
                {
                	dynamic_cast<Pneumatics*>(Actuator::actuator_vector[i])->Set(Pneumatics::FORWARD);
                }
            }
            line += dynamic_cast<Pneumatics*>(Actuator::actuator_vector[i])->Get() == Pneumatics::FORWARD ? "F" : "R";
        }
        else if (dynamic_cast<LRTServo*>(Actuator::actuator_vector[i]))
        {
            if (stick->IsButtonDown(button))
            	dynamic_cast<LRTServo*>(Actuator::actuator_vector[i])->Set(fabs(stick->GetAxis(Joystick::kYAxis)));
            line += Util::ToString(fabs(stick->GetAxis(Joystick::kYAxis)));
        }
        line += "\n";
        AsyncPrinter::Printf(line.c_str());
    }
    AsyncPrinter::Printf("------------------------------------\n");
}
