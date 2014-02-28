#include "ContinuousPotentiometer.h"
#include "SensorFactory.h"

ContinuousPotentiometer::ContinuousPotentiometer(uint32_t portA, uint32_t portB) :
	Configurable("ContinuousPotentiometer")
{
	m_channelA = SensorFactory::GetAnalogChannel(portA);
	m_channelB = SensorFactory::GetAnalogChannel(portB);
}

ContinuousPotentiometer::~ContinuousPotentiometer()
{
	
}

float ContinuousPotentiometer::GetAngle()
{
	float range = m_maxVoltage - m_minVoltage; // Full voltage range
	float middleRange = m_middleRangeAngle / 360.0 * range; // Voltage range of discontinuity of both phases
	float voltage_a = m_channelA->GetAverageVoltage();
	float voltage_b = m_channelB->GetAverageVoltage();
	// Linearize both channels
	if (voltage_a > m_minVoltage + range / 2 - middleRange / 2 && voltage_a < m_maxVoltage - range / 2 + middleRange / 2) // Channel B is discontinuous
	{
		if (voltage_a < m_minVoltage + range / 2) // Channel B is at maximum
		{
			voltage_b = voltage_a + range / 2; // Add half phase to Channel A to get correct value for Channel B
		}
		else // Channel B is at minimum
		{
			voltage_b = voltage_a - range / 2; // Subtract half phase from Channel A to get correct value for Channel B
		}
	}
	else if (voltage_b > m_minVoltage + range / 2 - middleRange / 2 && voltage_b < m_maxVoltage - range / 2 + middleRange / 2) // Channel A is discontinuous
	{
		if (voltage_b < m_minVoltage + range / 2) // Channel A is at maximum
		{
			voltage_a = voltage_b + range / 2; // Add half phase to Channel B to get correct value for Channel A
		}
		else // Channel A is at minimum
		{
			voltage_a = voltage_b - range / 2; // Subtract half phase from Channel B to get correct value for Channel A
		}
	}
	// Return average of both channels
	if (voltage_b > voltage_a) // 0-180 degrees
		return ((voltage_a + voltage_b - range / 2) / 2 - m_minVoltage) / range * 360;
	else // 180-360 degrees
		return ((voltage_a + voltage_b + range / 2) / 2 - m_minVoltage) / range * 360;
}

AnalogChannel* ContinuousPotentiometer::GetChannelA()
{
	return m_channelA;
}

AnalogChannel* ContinuousPotentiometer::GetChannelB()
{
	return m_channelB;
}

void ContinuousPotentiometer::Configure()
{
	m_minVoltage = GetConfig("min_voltage", 0.0);
	m_maxVoltage = GetConfig("max_voltage", 5.0);
	m_middleRangeAngle = GetConfig("mid_range_angle", 30.0);
}
