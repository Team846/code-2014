#ifndef CONTINUOUS_POTENTIOMETER_H_
#define CONTINUOUS_POTENTIOMETER_H_

#include "../Config/Configurable.h"
#include <AnalogChannel.h>

/*!
 * @brief For continuous potentiometer with 2-phase output.
 */
class ContinuousPotentiometer : public Configurable
{
public:
	ContinuousPotentiometer(uint32_t portA, uint32_t portB);
	~ContinuousPotentiometer();
	
	/*!
	 * @brief Returns the angle of the potentiometer based on Channel A
	 */
	float GetAngle();
	
	AnalogChannel* GetChannelA();
	AnalogChannel* GetChannelB();
	
	void Configure();
	
private:
	AnalogChannel* m_channelA;
	AnalogChannel* m_channelB;
	
	float m_minVoltage;
	float m_maxVoltage;
	float m_middleRangeAngle;
};

#endif
