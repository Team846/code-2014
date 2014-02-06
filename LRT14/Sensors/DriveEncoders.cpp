#include "DriveEncoders.h"
#include "SensorFactory.h"
#include "../Config/RobotConfig.h"
#include "../Config/ConfigRuntime.h"
#include "../Config/ConfigPortMappings.h"
#include "../Utils/Util.h"

double DriveEncoders::MAX_ENCODER_RATE = 1;
double DriveEncoders::MAX_TURNING_RATE = 1;
double DriveEncoders::PULSES_PER_REVOLUTION = 1;
double DriveEncoders::TICKS_PER_FULL_TURN = 1;
double DriveEncoders::WHEEL_DIAMETER = 1;
double DriveEncoders::GEAR_RATIO = 1;

DriveEncoders *DriveEncoders::m_instance = NULL;

DriveEncoders::DriveEncoders(UINT32 leftSourceA, UINT32 leftSourceB, UINT32 rightSourceA, UINT32 rightSourceB) :
	Configurable("DriveEncoders"),
	Loggable("DriveEncoders")
{
	m_encoders[LEFT] = SensorFactory::GetLRTEncoder("LeftDriveEncoder", leftSourceA, leftSourceB);
	m_encoders[RIGHT] = SensorFactory::GetLRTEncoder("RightDriveEncoder", rightSourceA, rightSourceB);

	m_encoders[LEFT]->SetDistancePerPulse(1);
	m_encoders[RIGHT]->SetDistancePerPulse(1);
	
	if (m_instance == NULL)
		m_instance = this;
}

DriveEncoders::~DriveEncoders()
{	
	DELETE(m_encoders[LEFT]);
	DELETE(m_encoders[RIGHT]);
	
	DELETE(m_instance);
}

DriveEncoders* DriveEncoders::Get()
{
	return m_instance;
}

double DriveEncoders::GetRawForwardSpeed()
{
	return (m_encoders[LEFT]->GetRate() + m_encoders[RIGHT]->GetRate()) / 2;
}

double DriveEncoders::GetNormalizedForwardSpeed()
{
	return GetRawForwardSpeed() / GetMaxEncoderRate();
}

double DriveEncoders::GetRawTurningSpeed()
{
	return m_encoders[RIGHT]->GetRate() - m_encoders[LEFT]->GetRate();
}

double DriveEncoders::GetNormalizedTurningSpeed()
{
	return GetRawTurningSpeed() / MAX_TURNING_RATE;
}

double DriveEncoders::GetRobotDist()
{
	return (GetWheelDist(LEFT) + GetWheelDist(RIGHT)) / 2.0;
}

int DriveEncoders::GetTurnTicks()
{
	return m_encoders[RIGHT]->Get() - m_encoders[LEFT]->Get();
}

double DriveEncoders::GetTurnRevolutions()
{
	return GetTurnTicks() / TICKS_PER_FULL_TURN;
}

double DriveEncoders::GetTurnAngle()
{
	return GetTurnRevolutions() * 360.0;
}

double DriveEncoders::GetWheelDist(Side side)
{
	LRTEncoder * e = m_encoders[side];
	double dist = (double) ((e->Get() * 1.0) / PULSES_PER_REVOLUTION
			* GEAR_RATIO * WHEEL_DIAMETER * PI); // pulses / ( pulses / encoder revolution ) * encoder to wheel gear ratio * distance / wheel revolution = inch distance
	return dist;
}

double DriveEncoders::GetNormalizedSpeed(Side side)
{
	return m_encoders[side]->GetRate() / MAX_ENCODER_RATE;
}

LRTEncoder* DriveEncoders::GetEncoder(Side side)
{
	return m_encoders[side];
}


double DriveEncoders::GetMaxEncoderRate()
{
	return MAX_ENCODER_RATE;
}

double DriveEncoders::GetMaxSpeed()
{
	return MAX_ENCODER_RATE / PULSES_PER_REVOLUTION * GEAR_RATIO * WHEEL_DIAMETER * PI;
}

double DriveEncoders::GetMaxTurnRate()
{
	return MAX_TURNING_RATE / TICKS_PER_FULL_TURN * 360;
}

double DriveEncoders::GetTurnRadius()
{
	double faster;
	double slower;
	if (fabs(GetNormalizedSpeed(LEFT)) > fabs(GetNormalizedSpeed(RIGHT)))
	{
		faster = GetNormalizedSpeed(LEFT);
		slower = GetNormalizedSpeed(RIGHT);
	}
	else
	{
		faster = GetNormalizedSpeed(RIGHT);
		slower = GetNormalizedSpeed(LEFT);
	}
	if (faster == 0 && slower == 0)
		return 0;
	return RobotConfig::ROBOT_WIDTH / (1 - slower / faster) - RobotConfig::ROBOT_WIDTH / 2; // Radius to center of robot -RC
}

void DriveEncoders::Configure()
{
	PULSES_PER_REVOLUTION = GetConfig("pulses_per_revolution", 360.0);
	MAX_ENCODER_RATE = GetConfig("max_encoder_rate", 2214.762);
	MAX_TURNING_RATE = GetConfig("max_turning_rate", 3782);
	WHEEL_DIAMETER = GetConfig("wheel_diameter", 6.0); // Inches
	GEAR_RATIO = GetConfig("gear_ratio", 1.0);
	TICKS_PER_FULL_TURN = GetConfig("ticks_per_full_turn", 2.0 * 26.1 * PI / (GEAR_RATIO * WHEEL_DIAMETER * PI) * PULSES_PER_REVOLUTION);
}

void DriveEncoders::Log()
{
	LogToFile(GetNormalizedForwardSpeed(), "NormalizedDriveSpeed");
	LogToFile(GetNormalizedTurningSpeed(), "NormalizedTurningSpeed");
	LogToFile(GetRobotDist(), "DriveDistance");
	LogToFile(GetTurnAngle(), "TurnAngle");
	LogToFile(GetTurnTicks(), "TurnTicks");
	LogToFile(GetNormalizedSpeed(LEFT), "NormalizedLeftSpeed");
	LogToFile(GetNormalizedSpeed(RIGHT), "NormalizedRightSpeed");
}
