#include "LRTTalon.h"

using namespace std;

vector<LRTTalon*> LRTTalon::talon_vector;

LRTTalon::LRTTalon(uint32_t channel, std::string name, uint32_t jumperChannel) :
	Talon(channel),
	LRTSpeedController("LRTTalon" + name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	talon_vector.push_back(this);
	
	printf("Constructed LRTTalon %s on channel %2u\n", name.c_str(), (unsigned int)channel);
}

LRTTalon::LRTTalon(uint8_t moduleNumber, uint32_t channel, std::string name, uint32_t jumperChannel) :
	Talon(moduleNumber, channel),
	LRTSpeedController("LRTTalon" + name),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	talon_vector.push_back(this);
	
	printf("Constructed LRTTalon %s on channel %2u\n", name.c_str(), (unsigned int)channel);
}

LRTTalon::~LRTTalon()
{
	DELETE(m_brake_jumper);
}

void LRTTalon::SetDutyCycle(float speed)
{
	m_pwm = speed;
}

float LRTTalon::GetDutyCycle()
{
	return m_pwm;
}

float LRTTalon::GetHardwareValue()
{
	return Talon::Get();
}

void LRTTalon::Set(float speed)
{
	printf("[WARNING] Calling Set() in LRTTalon: %s, use SetDutyCycle() instead.\n;", GetName().c_str());
	SetDutyCycle(speed);
}

float LRTTalon::Get()
{
	return m_pwm;
}

void LRTTalon::Disable()
{
	m_pwm = kPwmDisabled;
}

void LRTTalon::PIDWrite(float output) 
{
	SetDutyCycle(output);
}

void LRTTalon::ConfigNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

LRTTalon::NeutralMode LRTTalon::GetNeutralMode()
{
	return m_neutral;
}

void LRTTalon::Update()
{
	Talon::Set(m_pwm);
	if (m_brake_jumper != NULL)
	{
		if(m_neutral == LRTSpeedController::kNeutralMode_Coast)
			m_brake_jumper->Set((uint32_t)1);
		if(m_neutral == LRTSpeedController::kNeutralMode_Brake)
			m_brake_jumper->Set((uint32_t)0);
	}
}
