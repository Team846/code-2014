#ifndef LRT_JAGUAR_H_
#define LRT_JAGUAR_H_

#include "Jaguar.h"
#include "LRTSpeedController.h"

#include "DigitalOutput.h"
#include <vector>

/*!
 * @brief Wrapper for PWM Jaguars.
 */
class LRTJaguar : public Jaguar, public LRTSpeedController
{
public:
	LRTJaguar(uint32_t channel, std::string name, uint32_t jumperChannel = 0);
	LRTJaguar(uint8_t moduleNumber, uint32_t channel, std::string name, uint32_t jumperChannel = 0);
	virtual ~LRTJaguar();
	virtual void SetDutyCycle(float value);
	virtual float GetDutyCycle();
	virtual float GetHardwareValue();
	void Set(float speed);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);
	virtual NeutralMode GetNeutralMode();
	
	/*!
	 * Writes the values to the Jaguar.
	 */
	void Update();
	
	static std::vector<LRTJaguar*> jaguar_vector;
	
private:
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	LRTSpeedController::NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
