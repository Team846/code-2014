#ifndef BALL_HOLDER_H_
#define BALL_HOLDER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/BallHolderData.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"

/*!
 * @brief Provides control over the ball holding mechanism.
 */
class BallHolder : public Component, public Configurable
{
public:
	BallHolder();
	~BallHolder();
	
	void OnEnabled();
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
	void Configure();
	
private:
	BallHolderData* m_ballHolderData;
	Pneumatics* m_pneumatics;
};

#endif
