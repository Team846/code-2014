#include "LRTVictor.h"

using namespace std;

vector<LRTVictor*> LRTVictor::victor_vector;

LRTVictor::LRTVictor(UINT32 channel, const char* name, UINT32 jumperChannel) :
	Victor(channel),
	LRTSpeedController(("LRTVictor" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	victor_vector.push_back(this);
	
	printf("Constructed LRTVictor %s on channel %2d\n", name, channel);
}

LRTVictor::LRTVictor(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel) :
	Victor(moduleNumber, channel),
	LRTSpeedController(("LRTVictor" + std::string(name)).c_str()),
	m_brake_jumper(jumperChannel != 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	m_pwm = 0.0;
	m_neutral = kNeutralMode_Coast;
	victor_vector.push_back(this);
	
	printf("Constructed LRTVictor %s on channel %2d\n", name, channel);
}

LRTVictor::~LRTVictor()
{
	DELETE(m_brake_jumper);
}

void LRTVictor::SetDutyCycle(float speed)
{
	m_pwm = speed;
}

float LRTVictor::GetDutyCycle()
{
	return m_pwm;
}

float LRTVictor::GetHardwareValue()
{
	return Victor::Get();
}

void LRTVictor::Set(float speed)
{
	printf("[WARNING] Calling Set() in LRTVictor: %s, use SetDutyCycle() instead.\n;", GetName());
	SetDutyCycle(speed);
}

float LRTVictor::Get()
{
	return m_pwm;
}

void LRTVictor::Disable()
{
	m_pwm = kPwmDisabled;
}

void LRTVictor::PIDWrite(float output) 
{
	SetDutyCycle(output);
}

void LRTVictor::ConfigNeutralMode(NeutralMode mode)
{
	m_neutral = mode;
}

LRTVictor::NeutralMode LRTVictor::GetNeutralMode()
{
	return m_neutral;
}

void LRTVictor::Output()
{
	Victor::Set(m_pwm);
	if (m_brake_jumper != NULL)
	{
		if(m_neutral == LRTSpeedController::kNeutralMode_Coast)
			m_brake_jumper->Set((UINT32)0);
		if(m_neutral == LRTSpeedController::kNeutralMode_Brake)
			m_brake_jumper->Set((UINT32)1);
	}
}
