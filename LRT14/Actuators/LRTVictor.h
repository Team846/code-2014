#ifndef LRT_VICTOR_H_
#define LRT_VICTOR_H_

#include <Victor.h>
#include "LRTSpeedController.h"

#include "DigitalOutput.h"
#include <vector>

/*!
 * @brief Wrapper for Victor speed controller.
 */
class LRTVictor : public Victor, public LRTSpeedController
{
public:
	LRTVictor(UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	LRTVictor(UINT8 moduleNumber, UINT32 channel, const char* name, UINT32 jumperChannel = 0);
	virtual ~LRTVictor();
	virtual void SetDutyCycle(float speed);
	virtual float GetDutyCycle();
	virtual float GetHardwareValue();
	void Set(float speed);
	virtual float Get();
	virtual void Disable();
	virtual void PIDWrite(float output);
	
	virtual void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);
	virtual NeutralMode GetNeutralMode();
	
	/*!
	 * @brief Writes the values to the Victor.
	 */
	void Update();
	
	static std::vector<LRTVictor*> victor_vector;
	
private:
	float m_pwm;
	
	DigitalOutput* m_brake_jumper;
	LRTSpeedController::NeutralMode m_neutral;
};

#endif /* LRT_TALON_H_ */
