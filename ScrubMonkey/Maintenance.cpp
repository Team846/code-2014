#include "Maintenance.h"
#include "Actuators/Actuator.h"
#include "Actuators/LRTSpeedController.h"
#include "DriverStation/LRTDriverStation.h"
#include "Actuators/Pneumatics.h"
#include "Actuators/LRTServo.h"
#include <string>

#include <Rhesus/Toolkit/IO/BufferedConsole.h>

using namespace Rhesus::Toolkit::IO;

void Maintenance::Update()
{
    BufferedConsole::Printf("----------Maintenance Mode----------\n");
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
        line += lexical_cast(button) + "\t" + Actuator::actuator_vector[i]->GetName() + "\t";
        if (dynamic_cast<LRTSpeedController*>(Actuator::actuator_vector[i]))
        {
            float val = 0.0;
           	if (stick->IsButtonDown(button))
           	{
           		if (stick->GetAxis(Joystick::kYAxis) > 0.5)
           			val = 1.0;
           		else if (stick->GetAxis(Joystick::kYAxis) < -0.5)
           			val = -1.0;
           		else
           			val = 0.0;
           	}
           	else
           	{
           		val = 0.0;
           	}
        
            dynamic_cast<LRTSpeedController*>(Actuator::actuator_vector[i])->SetDutyCycle(val);
            line += lexical_cast(dynamic_cast<LRTSpeedController*>(Actuator::actuator_vector[i])->GetDutyCycle());
            if (stick->IsButtonDown(button))
            	line += "\t*";
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
            if (stick->IsButtonJustPressed(button))
            	line += "\t*";
        }
        else if (dynamic_cast<LRTServo*>(Actuator::actuator_vector[i]))
        {
            if (stick->IsButtonDown(button))
            	dynamic_cast<LRTServo*>(Actuator::actuator_vector[i])->Set(fabs(stick->GetAxis(Joystick::kYAxis)));
            line += lexical_cast(dynamic_cast<LRTServo*>(Actuator::actuator_vector[i])->Get());
            if (stick->IsButtonDown(button))
            	line += "\t*";
        }
        line += "\n";
        BufferedConsole::Printf(line.c_str());
    }
    BufferedConsole::Printf("------------------------------------\n");
}
